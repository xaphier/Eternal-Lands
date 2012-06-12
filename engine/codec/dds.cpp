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
