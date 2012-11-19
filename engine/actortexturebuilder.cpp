/****************************************************************************
 *            actortexturebuilder.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "actortexturebuilder.hpp"
#include "image.hpp"
#include "reader.hpp"
#include "exceptions.hpp"
#include "texture.hpp"
#include "logging.hpp"
#include "codec/codecmanager.hpp"
#include "filesystem.hpp"
#include "globalvars.hpp"

namespace eternal_lands
{

	namespace
	{

		class ActorTextureErrorException:
			public virtual IoErrorException {};

		boost::array<glm::uvec2, 12> actor_part_sizes =
		{ {
			glm::uvec2(32, 32),	/* head */
			glm::uvec2(39, 36),	/* shield */
			glm::uvec2(62, 38),	/* cape */
			glm::uvec2(39, 14),	/* helmet */
			glm::uvec2(39, 36),	/* weapon */
			glm::uvec2(49, 54),	/* torso */
			glm::uvec2(40, 40),	/* arms */
			glm::uvec2(34, 48),	/* hair */
			glm::uvec2(39, 40),	/* boots */
			glm::uvec2(40, 40),	/* pants */
			glm::uvec2(16, 16),	/* hands */
			glm::uvec2(10, 26)	/* neck **/
		} };

		boost::array<glm::uvec2, 12> actor_part_offsets =
		{ {
			glm::uvec2(34,  0),	/* head */
			glm::uvec2(50, 38),	/* shield */
			glm::uvec2(66,  0),	/* cape */
			glm::uvec2(40, 74),	/* helmet */
			glm::uvec2(89, 38),	/* weapon */
			glm::uvec2(79, 74),	/* torso */
			glm::uvec2( 0, 48),	/* arms */
			glm::uvec2( 0,  0),	/* hair */
			glm::uvec2( 0, 88),	/* boots */
			glm::uvec2(39, 88),	/* pants */
			glm::uvec2(34, 32),	/* hands */
			glm::uvec2(40, 48)	/* neck **/
		} };

		TextureFormatType check_size(
			const CodecManagerSharedPtr &codec_manager,
			const ReaderSharedPtr &reader,
			const glm::uvec2 &size, const Uint16 scale,
			const bool rg_formats, const bool sRGB)
		{
			TextureFormatType texture_format;
			glm::uvec3 image_size;
			Uint32 width, height;
			Uint16 mipmap, mipmap_count;
			bool cube_map, array;

			codec_manager->get_image_information(reader, rg_formats,
				sRGB, texture_format, image_size, mipmap_count,
				cube_map, array);

			if (image_size[2] != 0)
			{
				EL_THROW_EXCEPTION(ActorTextureErrorException()
					<< boost::errinfo_file_name(
						reader->get_name())
					<< errinfo_expected_value(0)
					<< errinfo_value(image_size[2])
					<< errinfo_message(UTF8("Wrong image "
						"depth")));
			}

			if (cube_map)
			{
				EL_THROW_EXCEPTION(ActorTextureErrorException()
					<< boost::errinfo_file_name(
						reader->get_name())
					<< errinfo_expected_value(0)
					<< errinfo_value(cube_map)
					<< errinfo_message(UTF8("Wrong image, "
						"cube map")));
			}

			width = image_size[0];
			height = image_size[1];
			mipmap = 0;

			while ((width > (size[0] * scale)) ||
				(height > (size[1] * scale)))
			{
				width /= 2;
				height /= 2;
				mipmap++;
			}

			if (mipmap > mipmap_count)
			{
				EL_THROW_EXCEPTION(ActorTextureErrorException()
					<< boost::errinfo_file_name(
						reader->get_name())
					<< errinfo_expected_value(mipmap)
					<< errinfo_value(mipmap_count)
					<< errinfo_message(UTF8("Not enought "
						"mipmaps")));
			}

			if (width != (size[0] * scale))
			{
				EL_THROW_EXCEPTION(ActorTextureErrorException()
					<< boost::errinfo_file_name(
						reader->get_name())
					<< errinfo_expected_value(
						size[0] * scale)
					<< errinfo_value(width)
					<< errinfo_message(UTF8("Wrong image "
						"width")));
			}

			if (height != (size[1] * scale))
			{
				EL_THROW_EXCEPTION(ActorTextureErrorException()
					<< boost::errinfo_file_name(
						reader->get_name())
					<< errinfo_expected_value(
						size[1] * scale)
					<< errinfo_value(height)
					<< errinfo_message(UTF8("Wrong image "
						"height")));
			}

			return texture_format;
		}

		bool get_alpha(const TextureFormatType texture_format)
		{
			return (TextureFormatUtil::get_count(texture_format)
				== 4) && (texture_format != tft_rgba_dxt1) &&
				(texture_format != tft_srgb_a_dxt1);
		}

		bool get_alpha(const CodecManagerSharedPtr &codec_manager,
			const ReaderSharedPtr &reader,
			const glm::uvec2 &size, const Uint16 scale,
			const bool sRGB, bool &compressed)
		{
			TextureFormatType texture_format;

			texture_format = check_size(codec_manager, reader,
				size, scale, false, sRGB);

			compressed &= (texture_format == tft_rgb_dxt1) ||
				(texture_format == tft_srgb_dxt1) ||
				(texture_format == tft_rgba_dxt1) ||
				(texture_format == tft_srgb_a_dxt1) ||
				(texture_format == tft_rgba_dxt5) ||
				(texture_format == tft_srgb_a_dxt5);

			return get_alpha(texture_format);
		}

		bool get_alpha(const CodecManagerSharedPtr &codec_manager,
			const ReaderSharedPtr &texture_reader,
			const ReaderSharedPtr &base_reader,
			const ReaderSharedPtr &mask_reader,
			const glm::uvec2 &size, const Uint16 scale,
			const bool sRGB, bool &compressed)
		{
			Uint32 count;
			TextureFormatType texture_format;
			bool alpha;

			if ((base_reader.get() == nullptr) ||
				(mask_reader.get() == nullptr))
			{
				return get_alpha(codec_manager, texture_reader,
					size, scale, sRGB, compressed);
			}

			alpha = get_alpha(check_size(codec_manager,
				texture_reader, size, scale, false, sRGB));
			alpha |= get_alpha(check_size(codec_manager,
				base_reader, size, scale, false, sRGB));

			texture_format = check_size(codec_manager,
				mask_reader, size, scale, false, sRGB);

			count = TextureFormatUtil::get_count(texture_format);

			if (count > 1)
			{
				LOG_WARNING(lt_actor_texture, UTF8("Only one "
					"channel for mask image needed, but "
					"file '%1%' has %2% channels."),
					mask_reader->get_name() % count);
			}

			if (count > 0)
			{
				compressed = false;

				return alpha;
			}

			EL_THROW_EXCEPTION(ActorTextureErrorException()
				<< boost::errinfo_file_name(
					mask_reader->get_name())
				<< errinfo_string_value(
					TextureFormatUtil::get_str(
						texture_format))
				<< errinfo_message(UTF8("A format with one channel"
					"is needed")));
		}

		ImageSharedPtr get_image(
			const CodecManagerSharedPtr &codec_manager,
			const ReaderSharedPtr &reader,
			const glm::uvec2 &size, const Uint16 scale,
			const ImageCompressionTypeSet &compressions,
			const bool rg_formats, const bool sRGB,
			Uint16 &mipmap)
		{
			ImageSharedPtr result;
			Uint32 width, height;

			result = codec_manager->load_image(reader,
				compressions, rg_formats, sRGB, false);

			if (result->get_depth() != 0)
			{
				EL_THROW_EXCEPTION(ActorTextureErrorException()
					<< boost::errinfo_file_name(
						reader->get_name()))
					<< errinfo_expected_value(0)
					<< errinfo_value(result->get_depth())
					<< errinfo_message(UTF8("Wrong image "
						"depth"));
			}

			width = result->get_width();
			height = result->get_height();
			mipmap = 0;

			while ((width > (size[0] * scale)) ||
				(height > (size[1] * scale)))
			{
				width /= 2;
				height /= 2;
				mipmap++;
			}

			if (mipmap > result->get_mipmap_count())
			{
				EL_THROW_EXCEPTION(ActorTextureErrorException()
					<< boost::errinfo_file_name(
						reader->get_name()))
					<< errinfo_expected_value(mipmap)
					<< errinfo_value(
						result->get_mipmap_count())
					<< errinfo_message(UTF8("Not enought "
						"mipmaps"));
			}

			if (width != (size[0] * scale))
			{
				EL_THROW_EXCEPTION(ActorTextureErrorException()
					<< boost::errinfo_file_name(
						reader->get_name()))
					<< errinfo_expected_value(
						size[0] * scale)
					<< errinfo_value(width)
					<< errinfo_message(UTF8("Wrong image "
						"width"));
			}

			if (height != (size[1] * scale))
			{
				EL_THROW_EXCEPTION(ActorTextureErrorException()
					<< boost::errinfo_file_name(
						reader->get_name()))
					<< errinfo_expected_value(
						size[1] * scale)
					<< errinfo_value(height)
					<< errinfo_message(UTF8("Wrong image "
						"height"));
			}

			return result;
		}

		void set_image_lines(const ImageSharedPtr &src_image,
			const glm::uvec2 &size, const Uint16 scale,
			const glm::uvec2 &offset, const Uint16 mipmap,
			ImageSharedPtr image)
		{
			Uint32 y, height, offset_x, offset_y, memory_size;

			height = size[1] * scale;
			offset_x = offset[0] * scale;
			offset_y = offset[1] * scale;

			memory_size = size[0] * scale * image->get_pixel_size()
				/ 8;

			for (y = 0; y < height; y++)
			{
				memcpy(image->get_pixel_data(offset_x,
					y + offset_y, 0, 0, 0),
					src_image->get_pixel_data(0, y, 0, 0,
					mipmap), memory_size);
			}
		}

		void set_image_block_lines(const ImageSharedPtr &src_image,
			const glm::uvec2 &size, const Uint16 scale,
			const glm::uvec2 &offset, const Uint16 mipmap,
			ImageSharedPtr image)
		{
			Uint32 y, height, offset_x, offset_y, memory_size;

			assert((scale % 4) == 0);

			height = size[1] * scale / 4;
			offset_x = offset[0] * scale / 4;
			offset_y = offset[1] * scale / 4;

			memory_size = size[0] * scale * image->get_block_size()
				/ 4;

			for (y = 0; y < height; y++)
			{
				memcpy(image->get_block_data(offset_x,
					y + offset_y, 0, 0, 0),
					src_image->get_block_data(0, y, 0, 0,
					mipmap), memory_size);
			}
		}

		void set_image_blocks(const ImageSharedPtr &src_image,
			const glm::uvec2 &size, const Uint16 scale,
			const glm::uvec2 &offset, const Uint16 mipmap,
			ImageSharedPtr image)
		{
			Uint32 x, y, widht, height, offset_x, offset_y;
			Uint32 memory_size;

			assert((scale % 4) == 0);

			widht = size[0] * scale / 4;
			height = size[1] * scale / 4;
			offset_x = offset[0] * scale / 4;
			offset_y = offset[1] * scale / 4;

			memory_size = src_image->get_block_size();

			for (y = 0; y < height; y++)
			{
				for (x = 0; x < widht; x++)
				{
					memset(image->get_block_data(
						x + offset_x, y + offset_y, 0,
						0, 0), 0xFF, 8);
					memcpy(static_cast<Uint8*>(
						image->get_block_data(
						x + offset_x, y + offset_y, 0,
						0, 0)) + 8,
						src_image->get_block_data(x, y,
						0, 0, mipmap), memory_size);
				}
			}
		}

		void set_image(const CodecManagerSharedPtr &codec_manager,
			const ReaderSharedPtr &reader,
			const glm::uvec2 &size, const Uint16 scale,
			const glm::uvec2 &offset, const ImageSharedPtr &image,
			const ImageCompressionTypeSet &compressions,
			const bool sRGB)
		{
			glm::vec4 temp;
			ImageSharedPtr tmp;
			Uint32 x, y, width, height;
			Uint16 mipmap;

			tmp = get_image(codec_manager, reader, size, scale,
				compressions, false, sRGB, mipmap);

			if ((tmp->get_texture_format() == tft_rgb_dxt1) ||
				(tmp->get_texture_format() == tft_srgb_dxt1) ||
				(tmp->get_texture_format() == tft_rgba_dxt1) ||
				(tmp->get_texture_format() == tft_srgb_a_dxt1))
			{
				if ((image->get_texture_format() ==
					tft_rgb_dxt1) ||
					(image->get_texture_format() ==
						tft_srgb_dxt1) ||
					(image->get_texture_format() ==
						tft_rgba_dxt1) ||
					(image->get_texture_format() ==
						tft_srgb_a_dxt1))
				{
					set_image_block_lines(tmp, size, scale,
						offset, mipmap, image);
					return;
				}

				if ((image->get_texture_format() ==
					tft_rgba_dxt5) ||
					(image->get_texture_format() ==
						tft_srgb_a_dxt5))
				{
					set_image_blocks(tmp, size, scale,
						offset, mipmap, image);
					return;
				}
			}

			if ((tmp->get_texture_format() == tft_rgba_dxt5) ||
				(tmp->get_texture_format() == tft_srgb_a_dxt5))
			{
				if ((image->get_texture_format() ==
					tft_rgba_dxt5) ||
					(image->get_texture_format() ==
						tft_srgb_a_dxt5))
				{
					set_image_block_lines(tmp, size, scale,
						offset, mipmap, image);
					return;
				}
			}

			if (TextureFormatUtil::get_compressed(
				tmp->get_texture_format()))
			{
				tmp = get_image(codec_manager, reader, size,
					scale, ImageCompressionTypeSet(),
					false, true, mipmap);
			}

			if ((tmp->get_texture_format() == tft_rgba8) ||
				(tmp->get_texture_format() == tft_srgb8_a8))
			{
				if ((image->get_texture_format() == tft_rgba8)
					|| (image->get_texture_format() ==
					tft_srgb8_a8))
				{
					set_image_lines(tmp, size, scale,
						offset, mipmap, image);
					return;
				}
			}

			if ((tmp->get_texture_format() == tft_rgb8) ||
				(tmp->get_texture_format() == tft_srgb8))
			{
				if ((image->get_texture_format() == tft_rgb8) ||
					(image->get_texture_format() ==
						tft_srgb8))
				{
					set_image_lines(tmp, size, scale,
						offset, mipmap, image);
					return;
				}
			}

			width = size[0] * scale;
			height = size[1] * scale;

			for (y = 0; y < height; y++)
			{
				for (x = 0; x < width; x++)
				{
					temp = tmp->get_pixel(x, y, 0, 0,
						mipmap);

					image->set_pixel(
						x + offset[0] * scale,
						y + offset[1] * scale, 0, 0, 0,
						temp);
				}
			}
		}

		void set_image(const CodecManagerSharedPtr &codec_manager,
			const ReaderSharedPtr &texture_reader,
			const ReaderSharedPtr &base_reader,
			const ReaderSharedPtr &mask_reader,
			const glm::uvec2 &size, const Uint16 scale,
			const glm::uvec2 &offset, const ImageSharedPtr &image,
			const ImageCompressionTypeSet &compressions,
			const bool sRGB)
		{
			glm::vec4 t0, t1, value, temp;
			ImageSharedPtr texture_image, base_image, mask_image;
			float blend;
			Uint32 x, y, width, height;
			Uint16 texture_mipmap, base_mipmap, mask_mipmap;

			if ((base_reader.get() == nullptr) ||
				(mask_reader.get() == nullptr))
			{
				return set_image(codec_manager, texture_reader,
					size, scale, offset, image,
					compressions, sRGB);
			}

			texture_image = get_image(codec_manager, texture_reader,
				size, scale, ImageCompressionTypeSet(),
				false, sRGB, texture_mipmap);

			base_image = get_image(codec_manager, base_reader,
				size, scale, ImageCompressionTypeSet(),
				false, sRGB, base_mipmap);

			mask_image = get_image(codec_manager, mask_reader,
				size, scale, ImageCompressionTypeSet(),
				false, sRGB, mask_mipmap);

			width = size[0] * scale;
			height = size[1] * scale;

			for (y = 0; y < height; y++)
			{
				for (x = 0; x < width; x++)
				{
					t0 = texture_image->get_pixel(x, y, 0,
						0, texture_mipmap);
					t1 = base_image->get_pixel(x, y, 0, 0,
						base_mipmap);
					value = mask_image->get_pixel(x, y, 0,
						0, mask_mipmap);

					blend = value.r;

					temp = t0 * blend;
					temp += t1 * (1.0f - blend);
					image->set_pixel(
						x + offset[0] * scale,
						y + offset[1] * scale, 0, 0, 0,
						temp);
				}
			}
		}

	}

	void ActorTextureBuilder::build_actor_images()
	{
		ActorPartTextureTypeStringMap::iterator it, end;
		std::map<ActorPartTextureType, ReaderSharedPtr> parts;
		glm::uvec3 size;
		ImageCompressionTypeSet compressions;
		TextureFormatType texture_format;
		bool compressed, sRGB;

		LOG_DEBUG(lt_actor_texture, UTF8("Building actor '%1%' images "
			"%2%"), get_name() % UTF8("started"));

		end = m_parts.end();

		for (it = m_parts.begin(); it != end; ++it)
		{
			try
			{
				if (it->second != String())
				{
					parts[it->first] =
						get_file_system()->get_file(
							it->second);
				}
			}
			catch (const boost::exception &exception)
			{
				LOG_EXCEPTION(exception);
			}
			catch (const std::exception &exception)
			{
				LOG_EXCEPTION(exception);
			}
		}

		m_alphas.reset();
		compressed = m_global_vars->get_use_s3tc_for_actors();
		sRGB = get_global_vars()->get_use_linear_lighting();

		if (parts[aptt_pants_tex].get() != nullptr)
		{
			m_alphas[apt_pants] = get_alpha(get_codec_manager(),
				parts[aptt_pants_tex], parts[aptt_legs_base],
				parts[aptt_pants_mask],
				actor_part_sizes[apt_pants], m_scale,
				sRGB, compressed);
		}

		if (parts[aptt_boots_tex].get() != nullptr)
		{
			m_alphas[apt_boots] = get_alpha(get_codec_manager(),
				parts[aptt_boots_tex], parts[aptt_boots_base],
				parts[aptt_boots_mask],
				actor_part_sizes[apt_boots], m_scale,
				sRGB, compressed);
		}

		if (parts[aptt_torso_tex].get() != nullptr)
		{
			m_alphas[apt_torso] = get_alpha(get_codec_manager(),
				parts[aptt_torso_tex], parts[aptt_body_base],
				parts[aptt_torso_mask],
				actor_part_sizes[apt_torso], m_scale,
				sRGB, compressed);
		}

		if (parts[aptt_arms_tex].get() != nullptr)
		{
			m_alphas[apt_arms] = get_alpha(get_codec_manager(),
				parts[aptt_arms_tex], parts[aptt_arms_base],
				parts[aptt_arms_mask],
				actor_part_sizes[apt_arms], m_scale,
				sRGB, compressed);
		}

		if (parts[aptt_hands_tex].get() != nullptr)
		{
			m_alphas[apt_pants] = get_alpha(get_codec_manager(),
				parts[aptt_hands_tex],
				parts[aptt_hands_tex_save],
				parts[aptt_hands_mask],
				actor_part_sizes[apt_hands], m_scale,
				sRGB, compressed);
		}

		if (parts[aptt_head_tex].get() != nullptr)
		{
			m_alphas[apt_head] = get_alpha(get_codec_manager(),
				parts[aptt_head_tex], parts[aptt_head_base],
				parts[aptt_head_mask],
				actor_part_sizes[apt_head], m_scale,
				sRGB, compressed);
		}

		if (parts[aptt_hair_tex].get() != nullptr)
		{
			m_alphas[apt_hair] = get_alpha(get_codec_manager(),
				parts[aptt_hair_tex],
				actor_part_sizes[apt_hair], m_scale,
				sRGB, compressed);
		}

		if (parts[aptt_weapon_tex].get() != nullptr)
		{
			m_alphas[apt_weapon] = get_alpha(get_codec_manager(),
				parts[aptt_weapon_tex],
				actor_part_sizes[apt_weapon], m_scale,
				sRGB, compressed);
		}

		if (parts[aptt_shield_tex].get() != nullptr)
		{
			m_alphas[apt_shield] = get_alpha(get_codec_manager(),
				parts[aptt_shield_tex],
				actor_part_sizes[apt_shield], m_scale,
				sRGB, compressed);
		}

		if (parts[aptt_helmet_tex].get() != nullptr)
		{
			m_alphas[apt_helmet] = get_alpha(get_codec_manager(),
				parts[aptt_helmet_tex],
				actor_part_sizes[apt_helmet], m_scale,
				sRGB, compressed);
		}

		if (parts[aptt_neck_tex].get() != nullptr)
		{
			m_alphas[apt_neck] = get_alpha(get_codec_manager(),
				parts[aptt_neck_tex],
				actor_part_sizes[apt_neck], m_scale,
				sRGB, compressed);
		}

		if (parts[aptt_cape_tex].get() != nullptr)
		{
			m_alphas[apt_cape] = get_alpha(get_codec_manager(),
				parts[aptt_cape_tex],
				actor_part_sizes[apt_cape], m_scale,
				sRGB, compressed);
		}

		size[0] = m_size;
		size[1] = m_size;
		size[2] = 0;

		if (compressed)
		{
			assert(GLEW_EXT_texture_compression_s3tc);

			compressions.insert(ict_s3tc);

			if (sRGB)
			{
				texture_format = tft_srgb_a_dxt5;
			}
			else
			{
				texture_format = tft_rgba_dxt5;
			}
		}
		else
		{
			if (sRGB)
			{
				texture_format = tft_srgb8_a8;
			}
			else
			{
				texture_format = tft_rgba8;
			}
		}

		m_image = boost::make_shared<Image>(m_name, false,
			texture_format, size, 0);

		if (parts[aptt_pants_tex].get() != nullptr)
		{
			set_image(get_codec_manager(), parts[aptt_pants_tex],
				parts[aptt_legs_base], parts[aptt_pants_mask],
				actor_part_sizes[apt_pants], m_scale,
				actor_part_offsets[apt_pants], m_image,
				compressions, sRGB);
		}

		if (parts[aptt_boots_tex].get() != nullptr)
		{
			set_image(get_codec_manager(), parts[aptt_boots_tex],
				parts[aptt_boots_base], parts[aptt_boots_mask],
				actor_part_sizes[apt_boots], m_scale,
				actor_part_offsets[apt_boots], m_image,
				compressions, sRGB);
		}

		if (parts[aptt_torso_tex].get() != nullptr)
		{
			set_image(get_codec_manager(), parts[aptt_torso_tex],
				parts[aptt_body_base], parts[aptt_torso_mask],
				actor_part_sizes[apt_torso], m_scale,
				actor_part_offsets[apt_torso], m_image,
				compressions, sRGB);
		}

		if (parts[aptt_arms_tex].get() != nullptr)
		{
			set_image(get_codec_manager(), parts[aptt_arms_tex],
				parts[aptt_arms_base], parts[aptt_arms_mask],
				actor_part_sizes[apt_arms], m_scale,
				actor_part_offsets[apt_arms], m_image,
				compressions, sRGB);
		}

		if (parts[aptt_hands_tex].get() != nullptr)
		{
			set_image(get_codec_manager(), parts[aptt_hands_tex],
				parts[aptt_hands_tex_save],
				parts[aptt_hands_mask],
				actor_part_sizes[apt_hands], m_scale,
				actor_part_offsets[apt_hands], m_image,
				compressions, sRGB);
		}

		if (parts[aptt_head_tex].get() != nullptr)
		{
			set_image(get_codec_manager(), parts[aptt_head_tex],
				parts[aptt_head_base], parts[aptt_head_mask],
				actor_part_sizes[apt_head], m_scale,
				actor_part_offsets[apt_head], m_image,
				compressions, sRGB);
		}

		if (parts[aptt_hair_tex].get() != nullptr)
		{
			set_image(get_codec_manager(), parts[aptt_hair_tex],
				actor_part_sizes[apt_hair], m_scale,
				actor_part_offsets[apt_hair], m_image,
				compressions, sRGB);
		}

		if (parts[aptt_weapon_tex].get() != nullptr)
		{
			set_image(get_codec_manager(), parts[aptt_weapon_tex],
				actor_part_sizes[apt_weapon], m_scale,
				actor_part_offsets[apt_weapon], m_image,
				compressions, sRGB);
		}

		if (parts[aptt_shield_tex].get() != nullptr)
		{
			set_image(get_codec_manager(), parts[aptt_shield_tex],
				actor_part_sizes[apt_shield], m_scale,
				actor_part_offsets[apt_shield], m_image,
				compressions, sRGB);
		}

		if (parts[aptt_helmet_tex].get() != nullptr)
		{
			set_image(get_codec_manager(), parts[aptt_helmet_tex],
				actor_part_sizes[apt_helmet], m_scale,
				actor_part_offsets[apt_helmet], m_image,
				compressions, sRGB);
		}

		if (parts[aptt_neck_tex].get() != nullptr)
		{
			set_image(get_codec_manager(), parts[aptt_neck_tex],
				actor_part_sizes[apt_neck], m_scale,
				actor_part_offsets[apt_neck], m_image,
				compressions, sRGB);
		}

		if (parts[aptt_cape_tex].get() != nullptr)
		{
			set_image(get_codec_manager(), parts[aptt_cape_tex],
				actor_part_sizes[apt_cape], m_scale,
				actor_part_offsets[apt_cape], m_image,
				compressions, sRGB);
		}

		LOG_DEBUG(lt_actor_texture, UTF8("Building actor '%1%' images "
			"%2%"), get_name() % UTF8("done"));
	}

	void ActorTextureBuilder::build_actor_texture()
	{
		LOG_DEBUG(lt_actor_texture, UTF8("Building actor '%1%' texture"
			" %2%"), get_name() % UTF8("started"));

		CHECK_GL_ERROR();

		m_texture->set_image(m_image);

		CHECK_GL_ERROR_NAME(get_name());

		LOG_DEBUG(lt_actor_texture, UTF8("Building actor '%1%' texture"
			" %2%"), get_name() % UTF8("done"));
	}

	void ActorTextureBuilder::set_parts(
		const ActorPartTextureTypeStringMap &parts)
	{
		m_parts = parts;
	}

	ActorTextureBuilder::ActorTextureBuilder(
		const CodecManagerWeakPtr &codec_manager,
		const FileSystemSharedPtr &file_system,
		const GlobalVarsSharedPtr &global_vars, const String &name):
		m_name(name), m_codec_manager(codec_manager),
		m_file_system(file_system), m_global_vars(global_vars)
	{
		TextureFormatType texture_format;
		bool sRGB;

		assert(!m_codec_manager.expired());
		assert(m_file_system.get() != nullptr);
		assert(m_global_vars.get() != nullptr);

		m_scale = 4;
		m_size = 128 * m_scale;
		m_compression = m_global_vars->get_use_s3tc_for_actors();
		m_alphas.reset();

		sRGB = get_global_vars()->get_use_linear_lighting();

		if (m_compression)
		{
			if (sRGB)
			{
				texture_format = tft_srgb_a_dxt5;
			}
			else
			{
				texture_format = tft_rgba_dxt5;
			}
		}
		else
		{
			if (sRGB)
			{
				texture_format = tft_srgb8_a8;
			}
			else
			{
				texture_format = tft_rgba8;
			}
		}

		m_texture = boost::make_shared<Texture>(m_name, m_size,
			m_size, 0, 0xFFFF, 0, texture_format, ttt_texture_2d);
	}

	ActorTextureBuilder::~ActorTextureBuilder() noexcept
	{
	}

	String ActorTextureBuilder::get_effect() const noexcept
	{
		if (get_uses_alpha())
		{
			return String(UTF8("actor-alpha"));
		}
		else
		{
			return String(UTF8("actor"));
		}
	}

}
