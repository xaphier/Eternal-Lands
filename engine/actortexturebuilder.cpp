/****************************************************************************
 *            actortexturebuilder.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
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

		boost::array<Uint32Array2, 12> actor_part_sizes =
		{ {
			{ { 32, 32 } },	/* head */
			{ { 39, 36 } },	/* shield */
			{ { 62, 38 } },	/* cape */
			{ { 39, 14 } },	/* helmet */
			{ { 39, 36 } },	/* weapon */
			{ { 49, 54 } },	/* torso */
			{ { 40, 40 } },	/* arms */
			{ { 34, 48 } },	/* hair */
			{ { 39, 40 } },	/* boots */
			{ { 40, 40 } },	/* pants */
			{ { 16, 16 } },	/* hands */
			{ { 10, 26 } }	/* neck **/
		} };

		boost::array<Uint32Array2, 12> actor_part_offsets =
		{ {
			{ { 34,  0 } },	/* head */
			{ { 50, 38 } },	/* shield */
			{ { 66,  0 } },	/* cape */
			{ { 40, 74 } },	/* helmet */
			{ { 89, 38 } },	/* weapon */
			{ { 79, 74 } },	/* torso */
			{ {  0, 48 } },	/* arms */
			{ {  0,  0 } },	/* hair */
			{ {  0, 88 } },	/* boots */
			{ { 39, 88 } },	/* pants */
			{ { 34, 32 } },	/* hands */
			{ { 40, 48 } }	/* neck **/
		} };

		TextureFormatType check_size(
			const CodecManagerSharedPtr &codec_manager,
			const ReaderSharedPtr &reader,
			const Uint32Array2 &sizes, const Uint16 scale,
			const bool rg_formats)
		{
			TextureFormatType texture_format;
			Uint32Array3 image_sizes;
			Uint32 width, height;
			Uint16 mipmap, mipmap_count;

			codec_manager->get_image_information(reader, rg_formats,
				texture_format, image_sizes, mipmap_count);

			if (image_sizes[2] != 1)
			{
				EL_THROW_EXCEPTION(ActorTextureErrorException()
					<< boost::errinfo_file_name(
						reader->get_name())
					<< errinfo_expected_value(1)
					<< errinfo_value(width)
					<< errinfo_message(UTF8("Wrong image "
						"depth")));
			}

			width = image_sizes[0];
			height = image_sizes[1];
			mipmap = 0;

			while ((width > (sizes[0] * scale)) ||
				(height > (sizes[1] * scale)))
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

			if (width != (sizes[0] * scale))
			{
				EL_THROW_EXCEPTION(ActorTextureErrorException()
					<< boost::errinfo_file_name(
						reader->get_name())
					<< errinfo_expected_value(
						sizes[0] * scale)
					<< errinfo_value(width)
					<< errinfo_message(UTF8("Wrong image "
						"width")));
			}

			if (height != (sizes[1] * scale))
			{
				EL_THROW_EXCEPTION(ActorTextureErrorException()
					<< boost::errinfo_file_name(
						reader->get_name())
					<< errinfo_expected_value(
						sizes[1] * scale)
					<< errinfo_value(height)
					<< errinfo_message(UTF8("Wrong image "
						"height")));
			}

			return texture_format;
		}

		bool get_alpha(const TextureFormatType texture_format)
		{
			return (TextureFormatUtil::get_count(texture_format)
				== 4) && (texture_format != tft_rgba_dxt1);
		}

		bool get_alpha(const CodecManagerSharedPtr &codec_manager,
			const ReaderSharedPtr &reader,
			const Uint32Array2 &sizes, const Uint16 scale,
			bool &compressed)
		{
			TextureFormatType texture_format;

			texture_format = check_size(codec_manager, reader,
				sizes, scale, false);

			compressed &= (texture_format == tft_rgb_dxt1) ||
				(texture_format == tft_rgba_dxt1) ||
				(texture_format == tft_rgba_dxt5);

			return get_alpha(texture_format);
		}

		bool get_alpha(const CodecManagerSharedPtr &codec_manager,
			const ReaderSharedPtr &texture_reader,
			const ReaderSharedPtr &base_reader,
			const ReaderSharedPtr &mask_reader,
			const Uint32Array2 &sizes, const Uint16 scale,
			bool &compressed)
		{
			Uint32 count;
			TextureFormatType texture_format;
			bool alpha;

			if ((base_reader.get() == 0) ||
				(mask_reader.get() == 0))
			{
				return get_alpha(codec_manager, texture_reader,
					sizes, scale, compressed);
			}

			alpha = get_alpha(check_size(codec_manager,
				texture_reader, sizes, scale, false));
			alpha |= get_alpha(check_size(codec_manager,
				base_reader, sizes, scale, false));

			texture_format = check_size(codec_manager,
				mask_reader, sizes, scale, false);

			count = TextureFormatUtil::get_count(texture_format);

			if (count > 1)
			{
				LOG_WARNING(UTF8("Only one channel for mask "
					"image needed, but file '%1%' has %2% "
					"channels."), mask_reader->get_name() %
					count);
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
			const Uint32Array2 &sizes, const Uint16 scale,
			const ImageCompressionTypeSet &compressions,
			const bool rg_formats, Uint16 &mipmap)
		{
			ImageSharedPtr result;
			Uint32 width, height;

			result = codec_manager->load_image(reader,
				compressions, rg_formats);

			if (result->get_depth() != 1)
			{
				EL_THROW_EXCEPTION(ActorTextureErrorException()
					<< boost::errinfo_file_name(
						reader->get_name()))
					<< errinfo_expected_value(1)
					<< errinfo_value(width)
					<< errinfo_message(UTF8("Wrong image "
						"depth"));
			}

			width = result->get_width();
			height = result->get_height();
			mipmap = 0;

			while ((width > (sizes[0] * scale)) ||
				(height > (sizes[1] * scale)))
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

			if (width != (sizes[0] * scale))
			{
				EL_THROW_EXCEPTION(ActorTextureErrorException()
					<< boost::errinfo_file_name(
						reader->get_name()))
					<< errinfo_expected_value(
						sizes[0] * scale)
					<< errinfo_value(width)
					<< errinfo_message(UTF8("Wrong image "
						"width"));
			}

			if (height != (sizes[1] * scale))
			{
				EL_THROW_EXCEPTION(ActorTextureErrorException()
					<< boost::errinfo_file_name(
						reader->get_name()))
					<< errinfo_expected_value(
						sizes[1] * scale)
					<< errinfo_value(height)
					<< errinfo_message(UTF8("Wrong image "
						"height"));
			}

			return result;
		}

		void set_image_lines(const ImageSharedPtr &src_image,
			const Uint32Array2 &sizes, const Uint16 scale,
			const Uint32Array2 &offsets, const Uint16 mipmap,
			ImageSharedPtr image)
		{
			Uint32 y, height, offset_x, offset_y, size;

			height = sizes[1] * scale;
			offset_x = offsets[0] * scale;
			offset_y = offsets[1] * scale;

			size = sizes[0] * scale * image->get_pixel_size() / 8;

			for (y = 0; y < height; y++)
			{
				memcpy(image->get_pixel_data(offset_x,
					y + offset_y, 0, 0, 0),
					src_image->get_pixel_data(0, y, 0, 0,
					mipmap), size);
			}
		}

		void set_image_block_lines(const ImageSharedPtr &src_image,
			const Uint32Array2 &sizes, const Uint16 scale,
			const Uint32Array2 &offsets, const Uint16 mipmap,
			ImageSharedPtr image)
		{
			Uint32 y, height, offset_x, offset_y, size;

			assert((scale % 4) == 0);

			height = sizes[1] * scale / 4;
			offset_x = offsets[0] * scale / 4;
			offset_y = offsets[1] * scale / 4;

			size = sizes[0] * scale * image->get_block_size() / 4;

			for (y = 0; y < height; y++)
			{
				memcpy(image->get_block_data(offset_x,
					y + offset_y, 0, 0, 0),
					src_image->get_block_data(0, y, 0, 0,
					mipmap), size);
			}
		}

		void set_image_blocks(const ImageSharedPtr &src_image,
			const Uint32Array2 &sizes, const Uint16 scale,
			const Uint32Array2 &offsets, const Uint16 mipmap,
			ImageSharedPtr image)
		{
			Uint32 x, y, widht, height, offset_x, offset_y, size;

			assert((scale % 4) == 0);

			widht = sizes[0] * scale / 4;
			height = sizes[1] * scale / 4;
			offset_x = offsets[0] * scale / 4;
			offset_y = offsets[1] * scale / 4;

			size = src_image->get_block_size();

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
						0, 0, mipmap), size);
				}
			}
		}

		void set_image(const CodecManagerSharedPtr &codec_manager,
			const ReaderSharedPtr &reader,
			const Uint32Array2 &sizes, const Uint16 scale,
			const Uint32Array2 &offsets, ImageSharedPtr image,
			const ImageCompressionTypeSet &compressions)
		{
			glm::vec4 temp;
			ImageSharedPtr tmp;
			Uint32 x, y, width, height;
			Uint16 mipmap;

			tmp = get_image(codec_manager, reader, sizes, scale,
				compressions, false, mipmap);

			if ((tmp->get_texture_format() == tft_rgb_dxt1) ||
				(tmp->get_texture_format() == tft_rgba_dxt1))
			{
				if ((image->get_texture_format() ==
					tft_rgb_dxt1) ||
					(image->get_texture_format() ==
						tft_rgba_dxt1))
				{
					set_image_block_lines(tmp, sizes, scale,
						offsets, mipmap, image);
					return;
				}

				if ((image->get_texture_format() ==
					tft_rgba_dxt5))
				{
					set_image_blocks(tmp, sizes, scale,
						offsets, mipmap, image);
					return;
				}
			}

			if (tmp->get_texture_format() == tft_rgba_dxt5)
			{
				if ((image->get_texture_format() ==
					tft_rgba_dxt5))
				{
					set_image_block_lines(tmp, sizes, scale,
						offsets, mipmap, image);
					return;
				}
			}

			if (TextureFormatUtil::get_compressed(
				tmp->get_texture_format()))
			{
				tmp = get_image(codec_manager, reader, sizes,
					scale, ImageCompressionTypeSet(),
					false, mipmap);
			}

			if (tmp->get_texture_format() == tft_rgba8)
			{
				if ((image->get_texture_format() == tft_rgba8))
				{
					set_image_lines(tmp, sizes, scale,
						offsets, mipmap, image);
					return;
				}
			}

			if (tmp->get_texture_format() == tft_rgb8)
			{
				if ((image->get_texture_format() == tft_rgb8))
				{
					set_image_lines(tmp, sizes, scale,
						offsets, mipmap, image);
					return;
				}
			}

			width = sizes[0] * scale;
			height = sizes[1] * scale;

			for (y = 0; y < height; y++)
			{
				for (x = 0; x < width; x++)
				{
					temp = tmp->get_pixel(x, y, 0, 0,
						mipmap);

					image->set_pixel(
						x + offsets[0] * scale,
						y + offsets[1] * scale, 0, 0, 0,
						temp);
				}
			}
		}

		void set_image(const CodecManagerSharedPtr &codec_manager,
			const ReaderSharedPtr &texture_reader,
			const ReaderSharedPtr &base_reader,
			const ReaderSharedPtr &mask_reader,
			const Uint32Array2 &sizes, const Uint16 scale,
			const Uint32Array2 &offsets, ImageSharedPtr image,
			const ImageCompressionTypeSet &compressions)
		{
			glm::vec4 t0, t1, value, temp;
			ImageSharedPtr texture_image, base_image, mask_image;
			float blend;
			Uint32 x, y, width, height;
			Uint16 texture_mipmap, base_mipmap, mask_mipmap;

			if ((base_reader.get() == 0) ||
				(mask_reader.get() == 0))
			{
				return set_image(codec_manager, texture_reader,
					sizes, scale, offsets, image,
					compressions);
			}

			texture_image = get_image(codec_manager, texture_reader,
				sizes, scale, ImageCompressionTypeSet(),
				false, texture_mipmap);

			base_image = get_image(codec_manager, base_reader,
				sizes, scale, ImageCompressionTypeSet(),
				false, base_mipmap);

			mask_image = get_image(codec_manager, mask_reader,
				sizes, scale, ImageCompressionTypeSet(),
				false, mask_mipmap);

			width = sizes[0] * scale;
			height = sizes[1] * scale;

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
						x + offsets[0] * scale,
						y + offsets[1] * scale, 0, 0, 0,
						temp);
				}
			}
		}

	}

	void ActorTextureBuilder::build_actor_images()
	{
		ActorPartTextureTypeStringMap::iterator it, end;
		std::map<ActorPartTextureType, ReaderSharedPtr> parts;
		Uint32Array3 sizes;
		ImageCompressionTypeSet compressions;
		bool compressed;

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
		compressed = GLEW_EXT_texture_compression_s3tc;

		if (parts[aptt_pants_tex].get() != 0)
		{
			m_alphas[apt_pants] = get_alpha(get_codec_manager(),
				parts[aptt_pants_tex], parts[aptt_legs_base],
				parts[aptt_pants_mask],
				actor_part_sizes[apt_pants], m_scale,
				compressed);
		}

		if (parts[aptt_boots_tex].get() != 0)
		{
			m_alphas[apt_boots] = get_alpha(get_codec_manager(),
				parts[aptt_boots_tex], parts[aptt_boots_base],
				parts[aptt_boots_mask],
				actor_part_sizes[apt_boots], m_scale,
				compressed);
		}

		if (parts[aptt_torso_tex].get() != 0)
		{
			m_alphas[apt_torso] = get_alpha(get_codec_manager(),
				parts[aptt_torso_tex], parts[aptt_body_base],
				parts[aptt_torso_mask],
				actor_part_sizes[apt_torso], m_scale,
				compressed);
		}

		if (parts[aptt_arms_tex].get() != 0)
		{
			m_alphas[apt_arms] = get_alpha(get_codec_manager(),
				parts[aptt_arms_tex], parts[aptt_arms_base],
				parts[aptt_arms_mask],
				actor_part_sizes[apt_arms], m_scale,
				compressed);
		}

		if (parts[aptt_hands_tex].get() != 0)
		{
			m_alphas[apt_pants] = get_alpha(get_codec_manager(),
				parts[aptt_hands_tex],
				parts[aptt_hands_tex_save],
				parts[aptt_hands_mask],
				actor_part_sizes[apt_hands], m_scale,
				compressed);
		}

		if (parts[aptt_head_tex].get() != 0)
		{
			m_alphas[apt_head] = get_alpha(get_codec_manager(),
				parts[aptt_head_tex], parts[aptt_head_base],
				parts[aptt_head_mask],
				actor_part_sizes[apt_head], m_scale,
				compressed);
		}

		if (parts[aptt_hair_tex].get() != 0)
		{
			m_alphas[apt_hair] = get_alpha(get_codec_manager(),
				parts[aptt_hair_tex],
				actor_part_sizes[apt_hair], m_scale,
				compressed);
		}

		if (parts[aptt_weapon_tex].get() != 0)
		{
			m_alphas[apt_weapon] = get_alpha(get_codec_manager(),
				parts[aptt_weapon_tex],
				actor_part_sizes[apt_weapon], m_scale,
				compressed);
		}

		if (parts[aptt_shield_tex].get() != 0)
		{
			m_alphas[apt_shield] = get_alpha(get_codec_manager(),
				parts[aptt_shield_tex],
				actor_part_sizes[apt_shield], m_scale,
				compressed);
		}

		if (parts[aptt_helmet_tex].get() != 0)
		{
			m_alphas[apt_helmet] = get_alpha(get_codec_manager(),
				parts[aptt_helmet_tex],
				actor_part_sizes[apt_helmet], m_scale,
				compressed);
		}

		if (parts[aptt_neck_tex].get() != 0)
		{
			m_alphas[apt_neck] = get_alpha(get_codec_manager(),
				parts[aptt_neck_tex],
				actor_part_sizes[apt_neck], m_scale,
				compressed);
		}

		if (parts[aptt_cape_tex].get() != 0)
		{
			m_alphas[apt_cape] = get_alpha(get_codec_manager(),
				parts[aptt_cape_tex],
				actor_part_sizes[apt_cape], m_scale,
				compressed);
		}

		sizes[0] = m_size;
		sizes[1] = m_size;
		sizes[2] = 1;

		if (compressed)
		{
			assert(GLEW_EXT_texture_compression_s3tc);

			compressions.insert(ict_s3tc);

			if (get_uses_alpha())
			{
				m_image = boost::make_shared<Image>(m_name,
					false, tft_rgba_dxt5, sizes, 0);
			}
			else
			{
				m_image = boost::make_shared<Image>(m_name,
					false, tft_rgb_dxt1, sizes, 0);
			}
		}
		else
		{
			if (get_uses_alpha())
			{
				m_image = boost::make_shared<Image>(m_name,
					false, tft_rgba8, sizes, 0);
			}
			else
			{
				m_image = boost::make_shared<Image>(m_name,
					false, tft_rgb8, sizes, 0);
			}
		}

		if (parts[aptt_pants_tex].get() != 0)
		{
			set_image(get_codec_manager(), parts[aptt_pants_tex],
				parts[aptt_legs_base], parts[aptt_pants_mask],
				actor_part_sizes[apt_pants], m_scale,
				actor_part_offsets[apt_pants], m_image,
				compressions);
		}

		if (parts[aptt_boots_tex].get() != 0)
		{
			set_image(get_codec_manager(), parts[aptt_boots_tex],
				parts[aptt_boots_base], parts[aptt_boots_mask],
				actor_part_sizes[apt_boots], m_scale,
				actor_part_offsets[apt_boots], m_image,
				compressions);
		}

		if (parts[aptt_torso_tex].get() != 0)
		{
			set_image(get_codec_manager(), parts[aptt_torso_tex],
				parts[aptt_body_base], parts[aptt_torso_mask],
				actor_part_sizes[apt_torso], m_scale,
				actor_part_offsets[apt_torso], m_image,
				compressions);
		}

		if (parts[aptt_arms_tex].get() != 0)
		{
			set_image(get_codec_manager(), parts[aptt_arms_tex],
				parts[aptt_arms_base], parts[aptt_arms_mask],
				actor_part_sizes[apt_arms], m_scale,
				actor_part_offsets[apt_arms], m_image,
				compressions);
		}

		if (parts[aptt_hands_tex].get() != 0)
		{
			set_image(get_codec_manager(), parts[aptt_hands_tex],
				parts[aptt_hands_tex_save],
				parts[aptt_hands_mask],
				actor_part_sizes[apt_hands], m_scale,
				actor_part_offsets[apt_hands], m_image,
				compressions);
		}

		if (parts[aptt_head_tex].get() != 0)
		{
			set_image(get_codec_manager(), parts[aptt_head_tex],
				parts[aptt_head_base], parts[aptt_head_mask],
				actor_part_sizes[apt_head], m_scale,
				actor_part_offsets[apt_head], m_image,
				compressions);
		}

		if (parts[aptt_hair_tex].get() != 0)
		{
			set_image(get_codec_manager(), parts[aptt_hair_tex],
				actor_part_sizes[apt_hair], m_scale,
				actor_part_offsets[apt_hair], m_image,
				compressions);
		}

		if (parts[aptt_weapon_tex].get() != 0)
		{
			set_image(get_codec_manager(), parts[aptt_weapon_tex],
				actor_part_sizes[apt_weapon], m_scale,
				actor_part_offsets[apt_weapon], m_image,
				compressions);
		}

		if (parts[aptt_shield_tex].get() != 0)
		{
			set_image(get_codec_manager(), parts[aptt_shield_tex],
				actor_part_sizes[apt_shield], m_scale,
				actor_part_offsets[apt_shield], m_image,
				compressions);
		}

		if (parts[aptt_helmet_tex].get() != 0)
		{
			set_image(get_codec_manager(), parts[aptt_helmet_tex],
				actor_part_sizes[apt_helmet], m_scale,
				actor_part_offsets[apt_helmet], m_image,
				compressions);
		}

		if (parts[aptt_neck_tex].get() != 0)
		{
			set_image(get_codec_manager(), parts[aptt_neck_tex],
				actor_part_sizes[apt_neck], m_scale,
				actor_part_offsets[apt_neck], m_image,
				compressions);
		}

		if (parts[aptt_cape_tex].get() != 0)
		{
			set_image(get_codec_manager(), parts[aptt_cape_tex],
				actor_part_sizes[apt_cape], m_scale,
				actor_part_offsets[apt_cape], m_image,
				compressions);
		}
	}

	void ActorTextureBuilder::build_actor_texture()
	{
		TextureFormatType texture_format;

		if (m_compression)
		{
			if (get_uses_alpha())
			{
				texture_format = tft_rgba_dxt5;
			}
			else
			{
				texture_format = tft_rgb_dxt1;
			}
		}
		else
		{
			if (get_uses_alpha())
			{
				texture_format = tft_rgba8;
			}
			else
			{
				texture_format = tft_r5g6b5;
			}
		}

		CHECK_GL_ERROR();

		m_texture->set_format(texture_format);

		CHECK_GL_ERROR();

		m_texture->set_image(m_image);

		CHECK_GL_ERROR();
	}

	void ActorTextureBuilder::set_parts(
		const ActorPartTextureTypeStringMap &parts)
	{
		m_parts = parts;
	}

	ActorTextureBuilder::ActorTextureBuilder(
		const CodecManagerWeakPtr &codec_manager,
		const FileSystemWeakPtr &file_system,
		const GlobalVarsSharedPtr &global_vars, const String &name):
		m_name(name), m_codec_manager(codec_manager),
		m_file_system(file_system), m_global_vars(global_vars)
	{
		assert(!m_codec_manager.expired());
		assert(!m_file_system.expired());
		assert(m_global_vars.get() != 0);

		m_scale = 4;
		m_size = 128 * m_scale;
		m_compression = GLEW_EXT_texture_compression_s3tc;
		m_alphas.reset();

		m_texture = boost::make_shared<Texture>(name);
	}

	ActorTextureBuilder::~ActorTextureBuilder() throw()
	{
	}

}
