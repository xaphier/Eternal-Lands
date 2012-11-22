/****************************************************************************
 *            dds.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "dds.hpp"

namespace eternal_lands
{

	namespace dds
	{

		void build_dds_header(const Uint32 width, const Uint32 height,
			const Uint32 depth, const Uint32 mipmap_count,
			const Uint32 red_mask, const Uint32 green_mask,
			const Uint32 blue_mask, const Uint32 alpha_mask,
			bool cube_map, DdsHeader &header)
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

			if (cube_map)
			{
				header.m_caps.m_caps1 |= DDSCAPS_COMPLEX;
				header.m_caps.m_caps2 |= DDSCAPS2_CUBEMAP |
					DDSCAPS2_CUBEMAP_ALL_FACES;
			}

			if (mipmap_count > 0)
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
			header.m_mipmap_count = mipmap_count;
		}

		void build_dds_fourcc_header(const Uint32 width,
			const Uint32 height, const Uint32 depth,
			const Uint32 mipmap_count, const Uint32 fourcc,
			const Uint32 bpp, bool cube_map, DdsHeader &header)
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

			if (cube_map)
			{
				header.m_caps.m_caps1 |= DDSCAPS_COMPLEX;
				header.m_caps.m_caps2 |= DDSCAPS2_CUBEMAP |
					DDSCAPS2_CUBEMAP_ALL_FACES;
			}

			if (mipmap_count > 0)
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
			header.m_mipmap_count = mipmap_count;
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

		void build_dxt10_dds_header(const Uint32 width,
			const Uint32 height, const Uint32 depth,
			const Uint32 layer_count, const Uint32 mipmap_count,
			const Uint32 format, const Uint32 bpp,
			const Uint32 flags, const bool cube_map,
			const bool linear_size, DdsHeader &header,
			DdsHeader10 &header_dxt10)
		{
			Uint32 size;

			memset(&header, 0, sizeof(header));
			memset(&header_dxt10, 0, sizeof(header_dxt10));

			header.m_size = DDS_HEADER_SIZE;
			header.m_flags = DDSD_MIN_FLAGS | DDSD_PITCH;

			if (linear_size)
			{
				header.m_flags |= DDSD_LINEARSIZE;
				size = ((width + 3) >> 2) * ((height + 3) >> 2)
					* bpp * 16;
			}
			else
			{
				header.m_flags |= DDSD_PITCH;
				size = width * bpp;
			}

			if (depth > 0)
			{
				header.m_flags |= DDSD_DEPTH;
				header.m_caps.m_caps1 = DDSCAPS_COMPLEX |
					DDSCAPS_TEXTURE;
				header.m_caps.m_caps2 = DDSCAPS2_VOLUME;

				header_dxt10.m_resource_dimension =
					DDS_DIMENSION_TEXTURE3D;
			}
			else
			{
				header.m_caps.m_caps1 = DDSCAPS_TEXTURE;

				if (height > 0)
				{
					header_dxt10.m_resource_dimension =
						DDS_DIMENSION_TEXTURE2D;
				}
				else
				{
					header_dxt10.m_resource_dimension =
						DDS_DIMENSION_TEXTURE1D;
				}
			}

			header_dxt10.m_dxgi_format = format;
			header_dxt10.m_array_size = std::max(1u, layer_count);

			if (cube_map)
			{
				header_dxt10.m_misc_flag = DDS_MISC_TEXTURECUBE;
				header.m_caps.m_caps2 |= DDSCAPS2_CUBEMAP |
					DDSCAPS2_CUBEMAP_ALL_FACES;
			}

			if (mipmap_count > 0)
			{
				header.m_flags |= DDSD_MIPMAPCOUNT;
				header.m_caps.m_caps1 |= DDSCAPS_MIPMAP;
				header.m_caps.m_caps1 |= DDSCAPS_COMPLEX;
			}

			header.m_pixel_format.m_size = DDS_PIXEL_FORMAT_SIZE;

			header.m_pixel_format.m_flags = flags | DDPF_FOURCC;

			header.m_pixel_format.m_fourcc = DDSFMT_DX10;

			header.m_pixel_format.m_bit_count = bpp;

			header.m_height = std::max(1u, height);
			header.m_width = std::max(1u, width);
			header.m_size_or_pitch = size;
			header.m_depth = depth;
			header.m_mipmap_count = mipmap_count;
		}

	}

}
