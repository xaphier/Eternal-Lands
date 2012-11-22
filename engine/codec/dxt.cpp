/****************************************************************************
 *            dxt.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "dxt.hpp"
#include "exceptions.hpp"
#include "logging.hpp"
#include "reader.hpp"
#include "image.hpp"
#include "dds.hpp"

namespace eternal_lands
{

	namespace
	{

		struct DXTColorBlock
		{
			Uint16Array2 m_colors;
			Uint8Array4 m_indices;
		};

		struct DXTExplicitAlphaBlock
		{
			Uint8Array8 m_alphas;
		};

		struct DXTInterpolatedAlphaBlock
		{
			Uint8Array2 m_alphas;
			Uint8Array6 m_indices;
		};

		void unpack_dxt_color(const DXTColorBlock &block, const bool dxt1,
			Vec4Array16 &values)
		{
			Vec4Array4 colors;
			Uint32 i, j, index;

			colors[0][0] = boost::numeric_cast<float>(
				(block.m_colors[0] & 0xF800) >> 11);
			colors[0][1] = boost::numeric_cast<float>(
				(block.m_colors[0] & 0x07E0) >> 5);
			colors[0][2] = boost::numeric_cast<float>(
				block.m_colors[0] & 0x001F);
			colors[0][3] = 1.0f;
			colors[0] *= glm::vec4(1.0f / 31.0f, 1.0f / 63.0f, 1.0f / 31.0f, 1.0f);

			colors[1][0] = boost::numeric_cast<float>(
				(block.m_colors[1] & 0xF800) >> 11);
			colors[1][1] = boost::numeric_cast<float>(
				(block.m_colors[1] & 0x07E0) >> 5);
			colors[1][2] = boost::numeric_cast<float>(
				block.m_colors[1] & 0x001F);
			colors[1][3] = 1.0f;
			colors[1] *= glm::vec4(1.0f / 31.0f, 1.0f / 63.0f, 1.0f / 31.0f, 1.0f);

			if (dxt1 && (block.m_colors[0] <= block.m_colors[1]))
			{
				// 1-bit alpha
				// one intermediate colour, half way between the other two
				colors[2] = (colors[0] + colors[1]) / 2.0f;
				// transparent colour
				colors[3] = glm::vec4(0.0f);
			}
			else
			{
				// first interpolated colour, 1/3 of the way along
				colors[2] = (2.0f * colors[0] + colors[1]) / 3.0f;
				// second interpolated colour, 2/3 of the way along
				colors[3] = (colors[0] + 2.0f * colors[1]) / 3.0f;
			}

			// Process 4x4 block of texels
			for (i = 0; i < 4; ++i)
			{
				for (j = 0; j < 4; ++j)
				{
					// LSB come first
					index = (block.m_indices[i] >> (j * 2)) & 0x3;

					values[(i * 4) + j] = colors[index];
				}
			}
		}

		void unpack_dxt_alpha(const DXTExplicitAlphaBlock &block,
			const Uint16 channel, Vec4Array16 &values)
		{
			float value;
			Uint32 i, j, index;

			index = 0;

			for (i = 0; i < 8; ++i)
			{
				for (j = 0; j < 2; ++j)
				{
					value = boost::numeric_cast<float>(
						(block.m_alphas[i] >> (j * 4)) & 0xF);
					values[index][channel] = value / 15.0f;
					index++;
				}
			}
		}

		void unpack_dxt_alpha(const DXTInterpolatedAlphaBlock &block,
			const Uint16 channel, Vec4Array16 &values)
		{
			FloatArray8 alphas;
			Uint32 i, index, idx0, idx1;

			alphas[0] = block.m_alphas[0];
			alphas[1] = block.m_alphas[1];

			if (block.m_alphas[0] > block.m_alphas[1])
			{
				for (i = 0; i < 6; ++i)
				{
					alphas[i + 2] = (((6 - i) *
						block.m_alphas[0]) +
						((i + 1) * block.m_alphas[1]))
						/ 7.0f;
				}
			}
			else
			{
				// 4 interpolated alphas, plus zero and one
				// full range including extremes at [0] and [5]
				// we want to fill in [1] through [4] at weights ranging
				// from 1/5 to 4/5
				for (i = 0; i < 4; ++i)
				{
					alphas[i + 2] = (((4 - i) *
						block.m_alphas[0]) +
						((i + 1) * block.m_alphas[1]))
						/ 5.0f;
				}

				alphas[6] = 0.0f;
				alphas[7] = 255.0f;
			}

			for (i = 0; i < 16; ++i)
			{
				idx0 = (i * 3) / 8;
				idx1 = (i * 3) % 8;
				index = (block.m_indices[idx0] >> idx1) & 0x07;

				if ((idx1 > 5) && (idx0 < 4))
				{
					index |= (block.m_indices[idx0 + 1] << (8 - idx1)) & 0x07;
				}

				values[i][channel] = alphas[index] / 255.0f;
			}
		}

		void unpack_dxt1(const DXTColorBlock &block, Vec4Array16 &values)
		{
			unpack_dxt_color(block, true, values);
		}

		void unpack_dxt3(const DXTExplicitAlphaBlock &alpha_block,
			const DXTColorBlock &color_block, Vec4Array16 &values)
		{
			unpack_dxt_color(color_block, false, values);
			unpack_dxt_alpha(alpha_block, 3, values);
		}

		void unpack_dxt5(const DXTInterpolatedAlphaBlock &alpha_block,
			const DXTColorBlock &color_block, Vec4Array16 &values)
		{
			unpack_dxt_color(color_block, false, values);
			unpack_dxt_alpha(alpha_block, 3, values);
		}

		void unpack_ati1(const DXTInterpolatedAlphaBlock &block,
			Vec4Array16 &values)
		{
			unpack_dxt_alpha(block, 0, values);
		}

		void unpack_ati2(const DXTInterpolatedAlphaBlock &first_block,
			const DXTInterpolatedAlphaBlock &second_block,
			Vec4Array16 &values)
		{
			unpack_dxt_alpha(first_block, 0, values);
			unpack_dxt_alpha(second_block, 1, values);
		}

		bool alpha_check_dxt1(const DXTColorBlock &block)
		{
			Uint32 i, j, index;

			if (block.m_colors[0] <= block.m_colors[1])
			{
				// Process 4x4 block of texels
				for (i = 0; i < 4; ++i)
				{
					for (j = 0; j < 4; ++j)
					{
						// LSB come first
						index = block.m_indices[i] >>
							(j * 2) & 0x3;

						if (index == 3)
						{
							return true;
						}
					}
				}
			}

			return false;
		}

		void read_colors_block(const ReaderSharedPtr &reader,
			DXTColorBlock &colors)
		{
			BOOST_FOREACH(Uint16 &value, colors.m_colors)
			{
				value = reader->read_u8();
				value += reader->read_u8() * 256;
			}

			BOOST_FOREACH(Uint8 &value, colors.m_indices)
			{
				value = reader->read_u8();
			}
		}

		void read_alphas_block(const ReaderSharedPtr &reader,
			DXTInterpolatedAlphaBlock &alphas)
		{
			BOOST_FOREACH(Uint8 &value, alphas.m_alphas)
			{
				value = reader->read_u8();
			}

			BOOST_FOREACH(Uint8 &value, alphas.m_indices)
			{
				value = reader->read_u8();
			}
		}

		void read_alphas_block(const ReaderSharedPtr &reader,
			DXTExplicitAlphaBlock &alphas)
		{
			BOOST_FOREACH(Uint8 &value, alphas.m_alphas)
			{
				value = reader->read_u8();
			}
		}

		void read_and_uncompress_dxt1_block(
			const ReaderSharedPtr &reader, Vec4Array16 &values)
		{
			DXTColorBlock colors;

			read_colors_block(reader, colors);

			unpack_dxt1(colors, values);
		}

		void read_and_uncompress_dxt3_block(
			const ReaderSharedPtr &reader, Vec4Array16 &values)
		{
			DXTExplicitAlphaBlock alphas;
			DXTColorBlock colors;

			read_alphas_block(reader, alphas);
			read_colors_block(reader, colors);

			unpack_dxt3(alphas, colors, values);
		}

		void read_and_uncompress_dxt5_block(
			const ReaderSharedPtr &reader, Vec4Array16 &values)
		{
			DXTInterpolatedAlphaBlock alphas;
			DXTColorBlock colors;

			read_alphas_block(reader, alphas);
			read_colors_block(reader, colors);

			unpack_dxt5(alphas, colors, values);
		}

		void read_and_uncompress_ati1_block(
			const ReaderSharedPtr &reader, Vec4Array16 &values)
		{
			DXTInterpolatedAlphaBlock alphas;

			read_alphas_block(reader, alphas);

			unpack_ati1(alphas, values);
		}

		void read_and_uncompress_ati2_block(
			const ReaderSharedPtr &reader, Vec4Array16 &values)
		{
			DXTInterpolatedAlphaBlock first_block;
			DXTInterpolatedAlphaBlock second_block;

			read_alphas_block(reader, first_block);
			read_alphas_block(reader, second_block);

			unpack_ati2(first_block, second_block, values);
		}

		void merge_r_layers(const glm::vec4 &current_value,
			const Uint16 layer, glm::vec4 &value)
		{
			float color;

			color = value.r;

			value = current_value;

			value[layer] = color;
		}

		void merge_rg_layers(const glm::vec4 &current_value,
			const Uint16 layer, glm::vec4 &value)
		{
			if (layer > 0)
			{
				value.r = current_value.r;
				value.g = current_value.g;

				value.b = value.r;
				value.a = value.g;
			}
		}

		void uncompress_block(const ReaderSharedPtr &reader,
			const ImageSharedPtr &image,
			const TextureFormatType format,
			const Uint32 x, const Uint32 y, const Uint32 z,
			const Uint16 face, const Uint16 mipmap,
			const Uint32 width, const Uint32 height,
			const Uint16 layer, const bool merge_layers)
		{
			Vec4Array16 values;
			glm::vec4 value, current_value;
			Uint32 bx, by, sx, sy;

			switch (format)
			{
				case tft_rgb_dxt1:
				case tft_rgba_dxt1:
				case tft_srgb_dxt1:
				case tft_srgb_a_dxt1:
					read_and_uncompress_dxt1_block(reader,
						values);
					break;
				case tft_rgba_dxt3:
				case tft_srgb_a_dxt3:
					read_and_uncompress_dxt3_block(reader,
						values);
					break;
				case tft_rgba_dxt5:
				case tft_srgb_a_dxt5:
					read_and_uncompress_dxt5_block(reader,
						values);
					break;
				case tft_r_rgtc1:
				case tft_signed_r_rgtc1:
					read_and_uncompress_ati1_block(reader,
						values);
					break;
				case tft_rg_rgtc2:
				case tft_signed_rg_rgtc2:
					read_and_uncompress_ati2_block(reader,
						values);
					break;
				default:
					EL_THROW_EXCEPTION(
						InvalidParameterException()
						<< boost::errinfo_file_name(
							reader->get_name()));
			}

			sx = std::min(width - x, static_cast<Uint32>(4));
			sy = std::min(height - y, static_cast<Uint32>(4));

			// write 4x4 block to uncompressed version
			for (by = 0; by < sy; by++)
			{
				for (bx = 0; bx < sx; bx++)
				{
					value = values[by * 4 + bx];

					current_value = image->get_pixel(x + bx,
						y + by, z, face, mipmap);

					if (((format == tft_signed_r_rgtc1) ||
						(format == tft_r_rgtc1)) &&
						merge_layers)
					{
						merge_r_layers(current_value,
							layer, value);
					}

					if (((format == tft_signed_rg_rgtc2) ||
						(format == tft_rg_rgtc2)) &&
						merge_layers)
					{
						merge_rg_layers(current_value,
							layer, value);
					}

					image->set_pixel(x + bx, y + by, z,
						face, mipmap, value);
				}
			}
		}

		void read_uncompress_face(const ReaderSharedPtr &reader,
			const ImageSharedPtr &image,
			const TextureFormatType format, const Uint32 width,
			const Uint32 height, const Uint32 depth,
			const Uint16 face, const Uint16 mipmap,
			const bool merge_layers)
		{
			Uint32 x, y, z, layers;

			layers = 1;

			if (((format == tft_signed_rg_rgtc2) ||
				(format == tft_rg_rgtc2)) && merge_layers)
			{
				layers = 2;
			}

			if (((format == tft_signed_r_rgtc1) ||
				(format == tft_r_rgtc1)) && merge_layers)
			{
				layers = 4;
			}

			// slices are done individually
			for (z = 0; z < depth; z++)
			{
				// 4x4 blocks in x/y
				for (y = 0; y < height; y += 4)
				{
					for (x = 0; x < width; x += 4)
					{
						uncompress_block(reader, image,
							format, x, y,
							z / layers, face,
							mipmap, width, height,
							z % layers,
							merge_layers);
					}
				}
			}
		}

		TextureFormatType get_uncompressed_texture_format(
			const String &name,
			const TextureFormatType texture_format,
			const Uint16 layers, const bool rg_formats,
			const bool sRGB, const bool merge_layers,
			bool &can_merge_layers)
		{
			can_merge_layers = false;

			switch (texture_format)
			{
				case tft_rgb_dxt1:
				case tft_rgba_dxt1:
				case tft_rgba_dxt3:
				case tft_rgba_dxt5:
					if (sRGB)
					{
						return tft_srgb8_a8;
					}

					return tft_rgba8;
				case tft_srgb_dxt1:
				case tft_srgb_a_dxt1:
				case tft_srgb_a_dxt3:
				case tft_srgb_a_dxt5:
					return tft_srgb8_a8;
				case tft_r_rgtc1:
					if (merge_layers && ((layers % 4) == 0))
					{
						can_merge_layers = true;

						return tft_rgba8;
					}

					if (rg_formats)
					{
						return tft_r8;
					}

					return tft_l8;
				case tft_signed_r_rgtc1:
					if (merge_layers && ((layers % 4) == 0))
					{
						can_merge_layers = true;

						return tft_rgba8_snorm;
					}

					return tft_r8_snorm;
				case tft_rg_rgtc2:
					if (merge_layers && ((layers % 2) == 0))
					{
						can_merge_layers = true;

						return tft_rgba8;
					}

					if (rg_formats)
					{
						return tft_rg8;
					}

					return tft_la8;
				case tft_signed_rg_rgtc2:
					if (merge_layers && ((layers % 2) == 0))
					{
						can_merge_layers = true;

						return tft_rgba8_snorm;
					}

					return tft_rg8_snorm;
				default:
					EL_THROW_EXCEPTION(
						InvalidParameterException()
						<< boost::errinfo_file_name(
							name));
			}
		}

	}

	ImageSharedPtr Dxt::uncompress(const ReaderSharedPtr &reader,
		const String &name, const glm::uvec3 &size,
		const TextureFormatType texture_format, const Uint16 mipmaps,
		const bool cube_map, const bool array, const bool rg_formats,
		const bool sRGB, const bool merge_layers)
	{
		ImageSharedPtr image;
		Uint32 width, height, depth, face_count, mipmap_count;
		Uint32 i, j;
		TextureFormatType uncompressed_format;
		bool can_merge_layers;

		LOG_DEBUG(lt_dds_image, UTF8("Uncompressing DDS file '%1%'."),
			reader->get_name());

		if ((size.x % 4) != 0)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< boost::errinfo_file_name(
					reader->get_name()));
		}

		if ((size.y % 4) != 0)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< boost::errinfo_file_name(
					reader->get_name()));
		}

		uncompressed_format = get_uncompressed_texture_format(
			reader->get_name(), texture_format, size.z,
			rg_formats, sRGB, merge_layers, can_merge_layers);

		image = boost::make_shared<Image>(name, cube_map,
			uncompressed_format, size, mipmaps, array);

		face_count = image->get_face_count();
		mipmap_count = image->get_mipmap_count();

		for (i = 0; i < face_count; ++i)
		{
			width = std::max(1u, image->get_width());
			height = std::max(1u, image->get_height());
			depth = std::max(1u, image->get_depth());

			for (j = 0; j <= mipmap_count; ++j)
			{
				read_uncompress_face(reader, image,
					texture_format, width, height, depth,
					i, j, can_merge_layers);

				if (width > 1)
				{
					width /= 2;
				}

				if (height > 1)
				{
					height /= 2;
				}

				if (depth > 1)
				{
					depth /= 2;
				}
			}
		}

		return image;
	}

}
