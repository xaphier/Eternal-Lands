/****************************************************************************
 *            dds.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "dds.hpp"

namespace eternal_lands
{

	namespace dds
	{

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

			for (i = 0; i < 4; ++i)
			{
				for (j = 0; j < 4; ++j)
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

		void build_dds_header(const Uint32 width, const Uint32 height,
			const Uint32 depth, const Uint32 mipmaps,
			const Uint32 red_mask, const Uint32 green_mask,
			const Uint32 blue_mask, const Uint32 alpha_mask,
			DdsHeader &header)
		{
			Uint32 bpp;

			memset(&header, 0, sizeof(header));

			header.m_size = DDS_HEADER_SIZE;
			header.m_flags = DDSD_MIN_FLAGS | DDSD_PITCH;

			if (depth > 0)
			{
				header.m_flags |= DDSD_DEPTH;
				header.m_caps.m_caps1 = DDSCAPS_COMPLEX |
					DDSCAPS_TEXTURE;
				header.m_caps.m_caps2 = DDSCAPS2_VOLUME;
			}
			else
			{
				header.m_caps.m_caps1 = DDSCAPS_TEXTURE;
			}

			if (mipmaps > 0)
			{
				header.m_flags |= DDSD_MIPMAPCOUNT;
				header.m_caps.m_caps1 |= DDSCAPS_MIPMAP;
				header.m_caps.m_caps1 |= DDSCAPS_COMPLEX;
			}

			header.m_pixel_format.m_size = DDS_PIXEL_FORMAT_SIZE;

			header.m_pixel_format.m_flags = DDPF_RGB;

			if (alpha_mask > 0)
			{
				header.m_pixel_format.m_flags |=
					DDPF_ALPHAPIXELS;
			}

			header.m_pixel_format.m_fourcc = 0;

			bpp = __builtin_popcount(red_mask | green_mask |
				blue_mask | alpha_mask);

			header.m_pixel_format.m_bit_count = bpp;

			header.m_pixel_format.m_red_mask = red_mask;
			header.m_pixel_format.m_green_mask = green_mask;
			header.m_pixel_format.m_blue_mask = blue_mask;
			header.m_pixel_format.m_alpha_mask = alpha_mask;

			header.m_height = height;
			header.m_width = width;
			header.m_size_or_pitch = ((bpp + 7) / 8) * width;
			header.m_depth = depth;
			header.m_mipmap_count = mipmaps;
		}

		void build_dds_fourcc_header(const Uint32 width,
			const Uint32 height, const Uint32 depth,
			const Uint32 mipmaps, const Uint32 fourcc,
			const Uint32 bpp, DdsHeader &header)
		{
			memset(&header, 0, sizeof(header));

			header.m_size = DDS_HEADER_SIZE;
			header.m_flags = DDSD_MIN_FLAGS | DDSD_PITCH;

			if (depth > 0)
			{
				header.m_flags |= DDSD_DEPTH;
				header.m_caps.m_caps1 = DDSCAPS_COMPLEX |
					DDSCAPS_TEXTURE;
				header.m_caps.m_caps2 = DDSCAPS2_VOLUME;
			}
			else
			{
				header.m_caps.m_caps1 = DDSCAPS_TEXTURE;
			}

			if (mipmaps > 0)
			{
				header.m_flags |= DDSD_MIPMAPCOUNT;
				header.m_caps.m_caps1 |= DDSCAPS_MIPMAP;
				header.m_caps.m_caps1 |= DDSCAPS_COMPLEX;
			}

			header.m_pixel_format.m_size = DDS_PIXEL_FORMAT_SIZE;

			header.m_pixel_format.m_flags = DDPF_FOURCC;

			header.m_pixel_format.m_fourcc = fourcc;

			header.m_pixel_format.m_bit_count = bpp;

			header.m_height = height;
			header.m_width = width;
			header.m_size_or_pitch = ((bpp + 7) / 8) * width;
			header.m_depth = depth;
			header.m_mipmap_count = mipmaps;
		}

		bool get_fourcc(const GLenum type, const GLenum format,
			Uint32 &size, Uint32 &swap_size, Uint32 &fourcc)
		{
			switch (type)
			{
				case GL_NONE:
					switch (format)
					{
						case GL_COMPRESSED_RGB_S3TC_DXT1_EXT:
						case GL_COMPRESSED_RGBA_S3TC_DXT1_EXT:
							size = 4;
							swap_size = 1;
							fourcc = DDSFMT_DXT1;
							return true;
						case GL_COMPRESSED_RGBA_S3TC_DXT3_EXT:
							size = 8;
							swap_size = 1;
							fourcc = DDSFMT_DXT3;
							return true;
						case GL_COMPRESSED_RGBA_S3TC_DXT5_EXT:
							size = 8;
							swap_size = 1;
							fourcc = DDSFMT_DXT5;
							return true;
						case GL_COMPRESSED_RED_RGTC1:
							size = 4;
							swap_size = 1;
							fourcc = DDSFMT_ATI1;
							return true;
						case GL_COMPRESSED_RG_RGTC2:
							size = 8;
							swap_size = 1;
							fourcc = DDSFMT_ATI2;
							return true;
					}
					return false;
				case GL_SHORT:
					if (format == GL_RGBA)
					{
						size = 64;
						swap_size = 2;
						fourcc = dds::DDSFMT_A16B16G16R16_SIGNED;
						return true;
					}
					return false;
				case GL_UNSIGNED_SHORT:
					switch (format)
					{
						case GL_RED:
						case GL_LUMINANCE:
							size = 16;
							swap_size = 2;
							fourcc = dds::DDSFMT_L16;
							return true;
						case GL_RG:
						case GL_LUMINANCE_ALPHA:
							size = 32;
							swap_size = 2;
							fourcc = dds::DDSFMT_G16R16;
							return true;
						case GL_RGBA:
							size = 64;
							swap_size = 2;
							fourcc = dds::DDSFMT_A16B16G16R16;
							return true;
					}
					return false;
				case GL_FLOAT:
					switch (format)
					{
						case GL_RED:
						case GL_LUMINANCE:
							size = 32;
							swap_size = 4;
							fourcc = dds::DDSFMT_R32F;
							return true;
						case GL_RG:
						case GL_LUMINANCE_ALPHA:
							size = 64;
							swap_size = 4;
							fourcc = dds::DDSFMT_G32R32F;
							return true;
						case GL_RGBA:
							size = 128;
							swap_size = 4;
							fourcc = dds::DDSFMT_A32B32G32R32F;
							return true;
					}
					return false;
				case GL_HALF_FLOAT:
					switch (format)
					{
						case GL_RED:
						case GL_LUMINANCE:
							size = 16;
							swap_size = 2;
							fourcc = dds::DDSFMT_R16F;
							return true;
						case GL_RG:
						case GL_LUMINANCE_ALPHA:
							size = 32;
							swap_size = 2;
							fourcc = dds::DDSFMT_G16R16F;
							return true;
						case GL_RGBA:
							size = 64;
							swap_size = 2;
							fourcc = dds::DDSFMT_A16B16G16R16F;
							return true;
					}
					return false;
			}

			return false;
		}

	}

}
