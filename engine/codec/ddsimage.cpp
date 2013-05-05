/****************************************************************************
 *            ddsimage.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "ddsimage.hpp"
#include "exceptions.hpp"
#include "logging.hpp"
#include "reader.hpp"
#include "image.hpp"
#include "codecmanager.hpp"
#include "dds.hpp"
#include "writer.hpp"
#include "dxt.hpp"

namespace eternal_lands
{

	namespace
	{

		class DdsErrorException: public virtual IoErrorException {};
		class DdsUnkownFormatException:
			public virtual UnkownFormatException {};
		class DdsFormatNotSupportedException:
			public virtual NotImplementedException {};

		bool get_alpha(const Uint32 &pixel_flags)
		{
			return ((pixel_flags & dds::DDPF_ALPHA) ==
				dds::DDPF_ALPHA) ||
				((pixel_flags & dds::DDPF_ALPHAPIXELS) ==
				dds::DDPF_ALPHAPIXELS);
		}

		bool check_dds(const Uint8Array4 &ID)
		{
			return (ID[0] == 'D') && (ID[1] == 'D') &&
				(ID[2] == 'S') && (ID[3] == ' ');
		}

		void validate_header(const String &name, dds::DdsHeader &header)
		{
			Uint32 bit_count;

			if (header.m_size != dds::DDS_HEADER_SIZE)
			{
				EL_THROW_MESSAGE_EXCEPTION(UTF8("DDS file "
					"'%1%' has invalid header size %2% "
					"instead of %3%."), name %
					header.m_size % dds::DDS_HEADER_SIZE,
					DdsErrorException()
					<< boost::errinfo_file_name(name));
			}

			if (header.m_pixel_format.m_size !=
				dds::DDS_PIXEL_FORMAT_SIZE)
			{
				EL_THROW_MESSAGE_EXCEPTION(UTF8("DDS file "
					"'%1%' has invalid pixel format size "
					"%2% instead of %3%."), name %
					header.m_pixel_format.m_size %
					dds::DDS_PIXEL_FORMAT_SIZE,
					DdsErrorException()
					<< boost::errinfo_file_name(name));
			}

			if ((header.m_flags & dds::DDSD_MIN_FLAGS) !=
				dds::DDSD_MIN_FLAGS)
			{
				EL_THROW_MESSAGE_EXCEPTION(UTF8("DDS file "
					"'%1%' has texture flag not set"),
					name, DdsErrorException()
					<< boost::errinfo_file_name(name));
			}

			if ((header.m_caps.m_caps1 & dds::DDSCAPS_TEXTURE) !=
				dds::DDSCAPS_TEXTURE)
			{
				EL_THROW_MESSAGE_EXCEPTION(UTF8("DDS file "
					"'%1%' has texture flag not set"),
					name, DdsErrorException()
					<< boost::errinfo_file_name(name));
			}

			if (((header.m_caps.m_caps2 & dds::DDSCAPS2_CUBEMAP) ==
				dds::DDSCAPS2_CUBEMAP) &&
				((header.m_caps.m_caps2 &
				dds::DDSCAPS2_CUBEMAP_ALL_FACES) == 0))
			{
				EL_THROW_MESSAGE_EXCEPTION(UTF8("DDS file "
					"'%1%' is a cube map but has no cube "
					"faces."), name, DdsErrorException()
					<< boost::errinfo_file_name(name));
			}

			if (((header.m_caps.m_caps2 & dds::DDSCAPS2_CUBEMAP) ==
				dds::DDSCAPS2_CUBEMAP) &&
				((header.m_caps.m_caps2 & dds::DDSCAPS2_VOLUME)
				== dds::DDSCAPS2_VOLUME))
			{
				EL_THROW_MESSAGE_EXCEPTION(UTF8("DDS file "
					"'%1%' is a cube map and a volume "
					"texture."), name, DdsErrorException()
					<< boost::errinfo_file_name(name));
			}

			if (((header.m_flags & dds::DDSD_DEPTH) ==
				dds::DDSD_DEPTH) &&
				((header.m_caps.m_caps2 & dds::DDSCAPS2_VOLUME)
				!= dds::DDSCAPS2_VOLUME))
			{
				LOG_WARNING(lt_dds_image, UTF8("DDS file is "
					"'%1%' invalid. DDSCAPS2_VOLUME cap "
					"should be set for a valid volume "
					"DDS file."), name);
			}

			if (((header.m_caps.m_caps2 & dds::DDSCAPS2_CUBEMAP) ==
				dds::DDSCAPS2_CUBEMAP) &&
				((header.m_caps.m_caps1 & dds::DDSCAPS_COMPLEX)
				!= dds::DDSCAPS_COMPLEX))
			{
				LOG_WARNING(lt_dds_image, UTF8("DDS file '%1%'"
					" is invalid. DDSCAPS_COMPLEX cap "
					"should be set for a valid cube map "
					"DDS file."), name);
			}

			if (((header.m_caps.m_caps2 & dds::DDSCAPS2_VOLUME) ==
				dds::DDSCAPS2_VOLUME) &&
				((header.m_caps.m_caps1 & dds::DDSCAPS_COMPLEX)
				!= dds::DDSCAPS_COMPLEX))
			{
				LOG_WARNING(lt_dds_image, UTF8("DDS file '%1%'"
					" is invalid. DDSCAPS_COMPLEX cap "
					"should be set for a valid volume DDS "
					"file."), name);
			}

			if (((header.m_caps.m_caps1 & dds::DDSCAPS_MIPMAP) ==
				dds::DDSCAPS_MIPMAP) &&
				((header.m_caps.m_caps1 & dds::DDSCAPS_COMPLEX)
				!= dds::DDSCAPS_COMPLEX))
			{
				LOG_WARNING(lt_dds_image, UTF8("DDS file '%1%'"
					" is invalid. DDSCAPS_COMPLEX cap "
					"should be set for a valid DDS file "
					"with mipmaps."), name);
			}

			if (((header.m_caps.m_caps1 & dds::DDSCAPS_COMPLEX) ==
				dds::DDSCAPS_COMPLEX) &&
				((header.m_caps.m_caps1 & dds::DDSCAPS_MIPMAP)
				!= dds::DDSCAPS_MIPMAP) &&
				((header.m_caps.m_caps2 & dds::DDSCAPS2_VOLUME)
				!= dds::DDSCAPS2_VOLUME)
				&& ((header.m_caps.m_caps2 &
				dds::DDSCAPS2_CUBEMAP) !=
				dds::DDSCAPS2_CUBEMAP))
			{
				LOG_WARNING(lt_dds_image, UTF8("DDS file '%1%'"
					" is invalid. DDSCAPS_COMPLEX cap "
					"should be set only if the DDS file is"
					" a cube map, a volume and/or has "
					"mipmaps."), name);
			}

			if ((((header.m_pixel_format.m_flags & dds::DDPF_FOURCC)
				== dds::DDPF_FOURCC) &&
				((header.m_pixel_format.m_flags & dds::DDPF_RGB)
				== dds::DDPF_RGB)) &&
				(header.m_pixel_format.m_fourcc !=
					dds::DDSFMT_DX10))
			{
				EL_THROW_MESSAGE_EXCEPTION(UTF8("DDS file "
					"'%1%' has fourcc and rgb flag set"),
					name, DdsErrorException()
					<< boost::errinfo_file_name(name));
			}

			if (((header.m_pixel_format.m_flags & dds::DDPF_FOURCC)
				!= dds::DDPF_FOURCC) &&
				((header.m_pixel_format.m_flags & dds::DDPF_RGB)
				!= dds::DDPF_RGB))
			{
				if ((header.m_pixel_format.m_flags &
					dds::DDPF_LUMINANCE) ==
					dds::DDPF_LUMINANCE)
				{
					header.m_pixel_format.m_green_mask = 0;
					header.m_pixel_format.m_blue_mask = 0;
				}
				else
				{
					if (!get_alpha(
						header.m_pixel_format.m_flags))
					{
						EL_THROW_EXCEPTION(
							DdsErrorException());
					}
				}
			}

			if (((header.m_flags & dds::DDSD_DEPTH) !=
				dds::DDSD_DEPTH) || ((header.m_caps.m_caps2 &
				dds::DDSCAPS2_VOLUME) != dds::DDSCAPS2_VOLUME))
			{
				header.m_depth = 0;
			}
			else
			{
				if (header.m_depth == 0)
				{
					EL_THROW_EXCEPTION(DdsErrorException());
				}
			}

			if ((header.m_caps.m_caps1 & dds::DDSCAPS_MIPMAP) !=
				dds::DDSCAPS_MIPMAP)
			{
				header.m_mipmap_count = 0;
			}
			else
			{
				header.m_mipmap_count = std::max(
					header.m_mipmap_count,
					static_cast<Uint32>(1)) - 1;
			}

			bit_count = __builtin_popcount(
				header.m_pixel_format.m_red_mask |
				header.m_pixel_format.m_blue_mask |
				header.m_pixel_format.m_green_mask |
				header.m_pixel_format.m_alpha_mask);

			if (((header.m_pixel_format.m_flags & dds::DDPF_RGB) ==
				dds::DDPF_RGB) &&
				(header.m_pixel_format.m_bit_count != bit_count))
			{
				header.m_pixel_format.m_alpha_mask = 0xFFFFFFFF;
				header.m_pixel_format.m_alpha_mask ^=
					header.m_pixel_format.m_red_mask;
				header.m_pixel_format.m_alpha_mask ^=
					header.m_pixel_format.m_blue_mask;
				header.m_pixel_format.m_alpha_mask ^=
					header.m_pixel_format.m_green_mask;
			}
		}

		void convert_ycocg_block(Vec4Array16 &values)
		{
			float Y, Co, Cg, R, G, B;
			Uint32 i;
			Uint8 a;

			for (i = 0; i < 16; ++i)
			{
				Y  = values[i][3];
				Co = values[i][0];
				Cg = values[i][1];
				a  = values[i][2];

				/* convert YCoCg to RGB */
				Co -= 0.5f * 256.0f / 255.0f;
				Cg -= 0.5f * 256.0f / 255.0f;

				R = std::max(Y + Co - Cg, 0.0f);
				G = std::max(Y + Cg, 0.0f);
				B = std::max(Y - Co - Cg, 0.0f);

				values[i][0] = R;
				values[i][1] = G;
				values[i][2] = B;
				values[i][3] = a;
			}
		}

		void init_dxt10_dds_image(const ReaderSharedPtr &reader,
			dds::DdsHeader &header, dds::DdsHeader10 &header_dxt10)
		{
			header_dxt10.m_dxgi_format = reader->read_u32_le();
			header_dxt10.m_resource_dimension =
				reader->read_u32_le();
			header_dxt10.m_misc_flag = reader->read_u32_le();
			header_dxt10.m_array_size = reader->read_u32_le();
			header_dxt10.m_reserved = reader->read_u32_le();

			if ((header_dxt10.m_resource_dimension ==
				dds::DDS_DIMENSION_TEXTURE2D) &&
				((header_dxt10.m_misc_flag &
					dds::DDS_MISC_TEXTURECUBE)
						!= 0))
			{
				header.m_caps.m_caps2 |= dds::DDSCAPS2_CUBEMAP |
					dds::DDSCAPS2_CUBEMAP_ALL_FACES;
			}
			else
			{
				if (header_dxt10.m_resource_dimension ==
					dds::DDS_DIMENSION_TEXTURE3D)
				{
					header.m_flags |= dds::DDSD_DEPTH;
					header.m_caps.m_caps2 |= dds::DDSCAPS2_VOLUME;
				}
			}

			if ((header_dxt10.m_resource_dimension !=
				dds::DDS_DIMENSION_TEXTURE1D) &&
				(header_dxt10.m_resource_dimension !=
					dds::DDS_DIMENSION_TEXTURE2D) &&
				(header_dxt10.m_resource_dimension !=
					dds::DDS_DIMENSION_TEXTURE3D))
			{
				EL_THROW_EXCEPTION(DdsErrorException()
					<< boost::errinfo_file_name(
						reader->get_name()));
			}
		}

		void init_dds_image(const ReaderSharedPtr &reader,
			dds::DdsHeader &header, dds::DdsHeader10 &header_dxt10)
		{
			Uint8Array4 magic;

			BOOST_FOREACH(Uint8 &value, magic)
			{
				value = reader->read_u8();
			}

			if (!check_dds(magic))
			{
				EL_THROW_EXCEPTION(DdsErrorException()
					<< boost::errinfo_file_name(
						reader->get_name()));
			}

			header.m_size = reader->read_u32_le();
			header.m_flags = reader->read_u32_le();
			header.m_height = reader->read_u32_le();
			header.m_width = reader->read_u32_le();
			header.m_size_or_pitch = reader->read_u32_le();
			header.m_depth = reader->read_u32_le();
			header.m_mipmap_count = reader->read_u32_le();
			header.m_reserved1[0] = reader->read_u32_le();
			header.m_reserved1[1] = reader->read_u32_le();
			header.m_reserved1[2] = reader->read_u32_le();
			header.m_reserved1[3] = reader->read_u32_le();
			header.m_reserved1[4] = reader->read_u32_le();
			header.m_reserved1[5] = reader->read_u32_le();
			header.m_reserved1[6] = reader->read_u32_le();
			header.m_reserved1[7] = reader->read_u32_le();
			header.m_reserved1[8] = reader->read_u32_le();
			header.m_reserved1[9] = reader->read_u32_le();
			header.m_reserved1[10] = reader->read_u32_le();

			header.m_pixel_format.m_size = reader->read_u32_le();
			header.m_pixel_format.m_flags = reader->read_u32_le();
			header.m_pixel_format.m_fourcc = reader->read_u32_le();
			header.m_pixel_format.m_bit_count =
				reader->read_u32_le();
			header.m_pixel_format.m_red_mask =
				reader->read_u32_le();
			header.m_pixel_format.m_green_mask =
				reader->read_u32_le();
			header.m_pixel_format.m_blue_mask =
				reader->read_u32_le();
			header.m_pixel_format.m_alpha_mask =
				reader->read_u32_le();

			header.m_caps.m_caps1 = reader->read_u32_le();
			header.m_caps.m_caps2 = reader->read_u32_le();
			header.m_caps.m_caps3 = reader->read_u32_le();
			header.m_caps.m_caps4 = reader->read_u32_le();

			header.m_reserved2 = reader->read_u32_le();

			memset(&header_dxt10, 0, sizeof(header_dxt10));

			if (header.m_pixel_format.m_fourcc == dds::DDSFMT_DX10)
			{
				init_dxt10_dds_image(reader, header,
					header_dxt10);
			}

			validate_header(reader->get_name(), header);
		}

		bool get_cube_map(const dds::DdsHeader &header)
		{
			return ((header.m_caps.m_caps2 & dds::DDSCAPS2_CUBEMAP)
				== dds::DDSCAPS2_CUBEMAP);
		}

		TextureFormatType get_texture_format(
			const dds::DdsHeader &header, const bool rg_formats,
			const bool sRGB)
		{
			Uint32 red_mask, green_mask, blue_mask, alpha_mask;
			Uint32 luminace_mask;
			Uint32 red, green, blue, alpha, luminace;

			if ((header.m_pixel_format.m_flags & dds::DDPF_FOURCC)
				!= dds::DDPF_FOURCC)
			{
				red_mask = header.m_pixel_format.m_red_mask;
				green_mask = header.m_pixel_format.m_green_mask;
				blue_mask = header.m_pixel_format.m_blue_mask;
				alpha_mask = header.m_pixel_format.m_alpha_mask;
				luminace_mask = red_mask | green_mask |
					blue_mask;

				red = __builtin_popcount(red_mask);
				green = __builtin_popcount(green_mask);
				blue = __builtin_popcount(blue_mask);
				alpha = __builtin_popcount(alpha_mask);
				luminace = __builtin_popcount(luminace_mask);

				if (((luminace_mask == red_mask) || (red == 0))
					&& ((luminace_mask == green_mask) ||
					(green == 0)) && ((luminace_mask ==
					blue_mask) || (blue == 0)))
				{
					if ((luminace == 0) && (alpha <= 8))
					{
						if (rg_formats)
						{
							return tft_r8;
						}

						return tft_a8;
					}

					if ((luminace <= 8) && (alpha == 0))
					{
						if (rg_formats)
						{
							return tft_r8;
						}

						return tft_a8;
					}

					if ((luminace <= 8) && (alpha <= 8))
					{
						if (rg_formats)
						{
							return tft_rg8;
						}

						return tft_la8;
					}
				}

				if ((red <= 8) && (green == 0) && (blue == 0)
					&& (alpha == 0))
				{
					if (rg_formats)
					{
						return tft_r8;
					}

					return tft_l8;
				}

				if ((red <= 8) && (green <= 8) && (blue == 0)
					&& (alpha == 0))
				{
					if (rg_formats)
					{
						return tft_rg8;
					}

					return tft_la8;
				}

				if ((red <= 3) && (green <= 3) && (blue <= 2) &&
					(alpha == 0))
				{
					return tft_r3g3b2;
				}

				if ((red <= 4) && (green <= 4) && (blue <= 4) &&
					(alpha <= 4))
				{
					return tft_rgba4;
				}

				if ((red <= 5) && (green <= 6) && (blue <= 5) &&
					(alpha == 0))
				{
					return tft_r5g6b5;
				}

				if ((red <= 5) && (green <= 5) && (blue <= 5) &&
					(alpha == 1))
				{
					return tft_rgb5_a1;
				}

				if ((red <= 8) && (green <= 8) && (blue <= 8) &&
					(alpha == 0))
				{
					if (sRGB)
					{
						return tft_srgb8;
					}

					return tft_rgb8;
				}

				if ((red <= 8) && (green <= 8) && (blue <= 8) &&
					(alpha <= 8))
				{
					if (sRGB)
					{
						return tft_srgb8_a8;
					}

					return tft_rgba8;
				}

				if ((red <= 10) && (green <= 10) && (blue <= 10)
					&& (alpha <= 2))
				{
					return tft_rgb10_a2;
				}

				if (sRGB)
				{
					return tft_srgb8_a8;
				}

				return tft_rgba8;
			}
			else
			{
				switch (header.m_pixel_format.m_fourcc)
				{
					case dds::DDSFMT_DXT1:
						if (sRGB)
						{
							return tft_srgb_a_dxt1;
						}

						return tft_rgba_dxt1;
					case dds::DDSFMT_DXT2:
					case dds::DDSFMT_DXT3:
						if (sRGB)
						{
							return tft_srgb_a_dxt3;
						}

						return tft_rgba_dxt3;
					case dds::DDSFMT_DXT4:
					case dds::DDSFMT_DXT5:
						if (sRGB)
						{
							return tft_srgb_a_dxt5;
						}

						return tft_rgba_dxt5;
					case dds::DDSFMT_ATI1:
						return tft_r_rgtc1;
					case dds::DDSFMT_ATI2:
						return tft_rg_rgtc2;
					case dds::DDSFMT_R32F:
						return tft_r32f;
					case dds::DDSFMT_G32R32F:
						return tft_rg32f;
					case dds::DDSFMT_A32B32G32R32F:
						return tft_rgba32f;
					case dds::DDSFMT_R16F:
						return tft_r16f;
					case dds::DDSFMT_G16R16F:
						return tft_rg16f;
					case dds::DDSFMT_A16B16G16R16F:
						return tft_rgba16f;
					case dds::DDSFMT_R8G8B8:
						if (sRGB)
						{
							return tft_srgb8;
						}

						return tft_rgb8;
					case dds::DDSFMT_A8R8G8B8:
						return tft_rgba8;
					case dds::DDSFMT_R5G6B5:
						return tft_r5g6b5;
					case dds::DDSFMT_X1R5G5B5:
						return tft_r5g6b5;
					case dds::DDSFMT_A1R5G5B5:
						return tft_rgb5_a1;
					case dds::DDSFMT_A4R4G4B4:
						return tft_rgba4;
					case dds::DDSFMT_R3G3B2:
						return tft_r3g3b2;
					case dds::DDSFMT_A8:
						return tft_r8;
					case dds::DDSFMT_X4R4G4B4:
						return tft_r5g6b5;
					case dds::DDSFMT_A2B10G10R10:
						return tft_rgb10_a2;
					case dds::DDSFMT_A8B8G8R8:
						if (sRGB)
						{
							return tft_srgb8_a8;
						}

						return tft_rgba8;
					case dds::DDSFMT_G16R16:
						return tft_rg16;
					case dds::DDSFMT_A2R10G10B10:
						return tft_rgb10_a2;
					case dds::DDSFMT_A16B16G16R16:
						return tft_rgba16;
					case dds::DDSFMT_A16B16G16R16_SIGNED:
						return tft_rgba16_snorm;
					case dds::DDSFMT_L8:
						if (rg_formats)
						{
							return tft_r8;
						}
						return tft_l8;
					case dds::DDSFMT_A8L8:
						if (rg_formats)
						{
							return tft_rg8;
						}
						return tft_la8;
					case dds::DDSFMT_L16:
						if (rg_formats)
						{
							return tft_r16;
						}
						return tft_l16;
				}

				EL_THROW_EXCEPTION(DdsUnkownFormatException());
			}
		}

		TextureFormatType get_texture_format(const Uint32 format)
		{
			switch (format)
			{
				case dds::DXGI_FORMAT_UNKNOWN:
					EL_THROW_EXCEPTION(
						DdsUnkownFormatException());
					break;
				case dds::DXGI_FORMAT_R32G32B32A32_TYPELESS:
				case dds::DXGI_FORMAT_R32G8X24_TYPELESS:
				case dds::DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
				case dds::DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
				case dds::DXGI_FORMAT_R32G32B32_TYPELESS:
				case dds::DXGI_FORMAT_R16G16B16A16_TYPELESS:
				case dds::DXGI_FORMAT_R32G32_TYPELESS:
				case dds::DXGI_FORMAT_R10G10B10A2_TYPELESS:
				case dds::DXGI_FORMAT_R8G8B8A8_TYPELESS:
				case dds::DXGI_FORMAT_R16G16_TYPELESS:
				case dds::DXGI_FORMAT_R32_TYPELESS:
				case dds::DXGI_FORMAT_R24G8_TYPELESS:
				case dds::DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
				case dds::DXGI_FORMAT_X24_TYPELESS_G8_UINT:
				case dds::DXGI_FORMAT_R8G8_TYPELESS:
				case dds::DXGI_FORMAT_R16_TYPELESS:
				case dds::DXGI_FORMAT_R8_TYPELESS:
				case dds::DXGI_FORMAT_BC1_TYPELESS:
				case dds::DXGI_FORMAT_BC2_TYPELESS:
				case dds::DXGI_FORMAT_BC3_TYPELESS:
				case dds::DXGI_FORMAT_BC4_TYPELESS:
				case dds::DXGI_FORMAT_BC5_TYPELESS:
				case dds::DXGI_FORMAT_B8G8R8A8_TYPELESS:
				case dds::DXGI_FORMAT_B8G8R8X8_TYPELESS:
				case dds::DXGI_FORMAT_BC6H_TYPELESS:
				case dds::DXGI_FORMAT_BC7_TYPELESS:
					EL_THROW_EXCEPTION(
						DdsFormatNotSupportedException()
						<< errinfo_item_id(format));
					break;
				case dds::DXGI_FORMAT_R8G8_B8G8_UNORM:
				case dds::DXGI_FORMAT_G8R8_G8B8_UNORM:
				case dds::DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM:
				case dds::DXGI_FORMAT_BC6H_UF16:
				case dds::DXGI_FORMAT_BC6H_SF16:
				case dds::DXGI_FORMAT_BC7_UNORM:
				case dds::DXGI_FORMAT_BC7_UNORM_SRGB:
					EL_THROW_EXCEPTION(
						DdsFormatNotSupportedException()
						<< errinfo_item_id(format));
					break;
				case dds::DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
					return tft_depth32f_stencil8;
				case dds::DXGI_FORMAT_R32G32B32A32_FLOAT:
					return tft_rgba32f;
				case dds::DXGI_FORMAT_R32G32B32A32_UINT:
					return tft_rgba32_ui;
				case dds::DXGI_FORMAT_R32G32B32A32_SINT:
					return tft_rgba32_i;
				case dds::DXGI_FORMAT_R32G32B32_FLOAT:
					return tft_rgb32f;
				case dds::DXGI_FORMAT_R32G32B32_UINT:
					return tft_rgb32_ui;
				case dds::DXGI_FORMAT_R32G32B32_SINT:
					return tft_rgb32_i;
				case dds::DXGI_FORMAT_R16G16B16A16_FLOAT:
					return tft_rgba16f;
				case dds::DXGI_FORMAT_R16G16B16A16_UNORM:
					return tft_rgba16;
				case dds::DXGI_FORMAT_R16G16B16A16_UINT:
					return tft_rgba16_ui;
				case dds::DXGI_FORMAT_R16G16B16A16_SNORM:
					return tft_rgba16_snorm;
				case dds::DXGI_FORMAT_R16G16B16A16_SINT:
					return tft_rgba16_i;
				case dds::DXGI_FORMAT_R32G32_FLOAT:
					return tft_rg32f;
				case dds::DXGI_FORMAT_R32G32_UINT:
					return tft_rg32_ui;
				case dds::DXGI_FORMAT_R32G32_SINT:
					return tft_rg32_i;
				case dds::DXGI_FORMAT_R10G10B10A2_UNORM:
					return tft_rgb10_a2;
				case dds::DXGI_FORMAT_R10G10B10A2_UINT:
					return tft_rgb10_a2_ui;
				case dds::DXGI_FORMAT_R11G11B10_FLOAT:
					return tft_r11f_b11f_g10f;
				case dds::DXGI_FORMAT_R8G8B8A8_UNORM:
					return tft_rgba8;
				case dds::DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
					return tft_srgb8_a8;
				case dds::DXGI_FORMAT_R8G8B8A8_UINT:
					return tft_rgba8_ui;
				case dds::DXGI_FORMAT_R8G8B8A8_SNORM:
					return tft_rgba8_snorm;
				case dds::DXGI_FORMAT_R8G8B8A8_SINT:
					return tft_rgba8_i;
				case dds::DXGI_FORMAT_R16G16_FLOAT:
					return tft_rg16f;
				case dds::DXGI_FORMAT_R16G16_UNORM:
					return tft_rg16;
				case dds::DXGI_FORMAT_R16G16_UINT:
					return tft_rg16_ui;
				case dds::DXGI_FORMAT_R16G16_SNORM:
					return tft_rg16_snorm;
				case dds::DXGI_FORMAT_R16G16_SINT:
					return tft_rg16_i;
				case dds::DXGI_FORMAT_D32_FLOAT:
					return tft_depth32f;
				case dds::DXGI_FORMAT_R32_FLOAT:
					return tft_r32f;
				case dds::DXGI_FORMAT_R32_UINT:
					return tft_r32_ui;
				case dds::DXGI_FORMAT_R32_SINT:
					return tft_r32_i;
				case dds::DXGI_FORMAT_D24_UNORM_S8_UINT:
					return tft_depth24_stencil8;
				case dds::DXGI_FORMAT_R8G8_UNORM:
					return tft_rg8;
				case dds::DXGI_FORMAT_R8G8_UINT:
					return tft_rg8_ui;
				case dds::DXGI_FORMAT_R8G8_SNORM:
					return tft_rg8_snorm;
				case dds::DXGI_FORMAT_R8G8_SINT:
					return tft_rg8_i;
				case dds::DXGI_FORMAT_R16_FLOAT:
					return tft_r16f;
				case dds::DXGI_FORMAT_D16_UNORM:
					return tft_depth16;
				case dds::DXGI_FORMAT_R16_UNORM:
					return tft_r16;
				case dds::DXGI_FORMAT_R16_UINT:
					return tft_r16_ui;
				case dds::DXGI_FORMAT_R16_SNORM:
					return tft_r16_snorm;
				case dds::DXGI_FORMAT_R16_SINT:
					return tft_r16_i;
				case dds::DXGI_FORMAT_R8_UNORM:
					return tft_r8;
				case dds::DXGI_FORMAT_R8_UINT:
					return tft_r8_ui;
				case dds::DXGI_FORMAT_R8_SNORM:
					return tft_r8_snorm;
				case dds::DXGI_FORMAT_R8_SINT:
					return tft_r8_i;
				case dds::DXGI_FORMAT_A8_UNORM:
					return tft_a8;
				case dds::DXGI_FORMAT_R1_UNORM:
					break;
				case dds::DXGI_FORMAT_R9G9B9E5_SHAREDEXP:
					return tft_rgb9_e5;
				case dds::DXGI_FORMAT_BC1_UNORM:
					return tft_rgba_dxt1;
				case dds::DXGI_FORMAT_BC1_UNORM_SRGB:
					return tft_srgb_a_dxt1;
				case dds::DXGI_FORMAT_BC2_UNORM:
					return tft_rgba_dxt3;
				case dds::DXGI_FORMAT_BC2_UNORM_SRGB:
					return tft_srgb_a_dxt3;
				case dds::DXGI_FORMAT_BC3_UNORM:
					return tft_rgba_dxt5;
				case dds::DXGI_FORMAT_BC3_UNORM_SRGB:
					return tft_srgb_a_dxt5;
				case dds::DXGI_FORMAT_BC4_UNORM:
					return tft_r_rgtc1;
				case dds::DXGI_FORMAT_BC4_SNORM:
					return tft_signed_r_rgtc1;
				case dds::DXGI_FORMAT_BC5_UNORM:
					return tft_rg_rgtc2;
				case dds::DXGI_FORMAT_BC5_SNORM:
					return tft_signed_rg_rgtc2;
				case dds::DXGI_FORMAT_B5G6R5_UNORM:
					return tft_r5g6b5;
				case dds::DXGI_FORMAT_B5G5R5A1_UNORM:
					return tft_rgb5_a1;
				case dds::DXGI_FORMAT_B8G8R8A8_UNORM:
					return tft_rgba8;
				case dds::DXGI_FORMAT_B8G8R8X8_UNORM:
					return tft_rgb8;
				case dds::DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
					return tft_srgb8;
				case dds::DXGI_FORMAT_B8G8R8X8_UNORM_SRGB:
					return tft_srgb8_a8;
				case dds::DXGI_FORMAT_B4G4R4A4_UNORM:
					return tft_rgba4;
			}

			EL_THROW_EXCEPTION(DdsUnkownFormatException()
				<< errinfo_item_id(format));
		}

		bool get_dxt10_format(const TextureFormatType texture_format,
			glm::uvec4 &position, Uint32 &format, Uint32 &size,
			Uint32 &swap_size, Uint32 &flags, bool &linear_size)
		{
			linear_size = TextureFormatUtil::get_compressed(
				texture_format);

			size = TextureFormatUtil::get_size(texture_format);

			position = glm::uvec4(0, 1, 2, 3);

			switch (texture_format)
			{
				case tft_depth32f_stencil8:
					format = dds::DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
					swap_size = 4;
					return true;
				case tft_rgba32f:
					format = dds::DXGI_FORMAT_R32G32B32A32_FLOAT;
					swap_size = 4;
					flags = dds::DDPF_RGBA;
					return true;
				case tft_rgba32_ui:
					format = dds::DXGI_FORMAT_R32G32B32A32_UINT;
					swap_size = 4;
					flags = dds::DDPF_RGBA;
					return true;
				case tft_rgba32_i:
					format = dds::DXGI_FORMAT_R32G32B32A32_SINT;
					swap_size = 4;
					flags = dds::DDPF_RGBA;
					return true;
				case tft_rgb32f:
					format = dds::DXGI_FORMAT_R32G32B32_FLOAT;
					swap_size = 4;
					flags = dds::DDPF_RGBA;
					return true;
				case tft_rgb32_ui:
					format = dds::DXGI_FORMAT_R32G32B32_UINT;
					swap_size = 4;
					flags = dds::DDPF_RGB;
					return true;
				case tft_rgb32_i:
					format = dds::DXGI_FORMAT_R32G32B32_SINT;
					swap_size = 4;
					flags = dds::DDPF_RGB;
					return true;
				case tft_rgba16f:
					format = dds::DXGI_FORMAT_R16G16B16A16_FLOAT;
					swap_size = 2;
					flags = dds::DDPF_RGBA;
					return true;
				case tft_rgba16:
					format = dds::DXGI_FORMAT_R16G16B16A16_UNORM;
					swap_size = 2;
					flags = dds::DDPF_RGBA;
					return true;
				case tft_rgba16_ui:
					format = dds::DXGI_FORMAT_R16G16B16A16_UINT;
					swap_size = 2;
					flags = dds::DDPF_RGBA;
					return true;
				case tft_rgba16_snorm:
					format = dds::DXGI_FORMAT_R16G16B16A16_SNORM;
					swap_size = 2;
					flags = dds::DDPF_RGBA;
					return true;
				case tft_rgba16_i:
					format = dds::DXGI_FORMAT_R16G16B16A16_SINT;
					swap_size = 2;
					flags = dds::DDPF_RGBA;
					return true;
				case tft_rg32f:
					format = dds::DXGI_FORMAT_R32G32_FLOAT;
					swap_size = 4;
					flags = dds::DDPF_RGB;
					return true;
				case tft_rg32_ui:
					format = dds::DXGI_FORMAT_R32G32_UINT;
					swap_size = 4;
					flags = dds::DDPF_RGB;
					return true;
				case tft_rg32_i:
					format = dds::DXGI_FORMAT_R32G32_SINT;
					swap_size = 4;
					flags = dds::DDPF_RGB;
					return true;
				case tft_rgb10_a2:
					format = dds::DXGI_FORMAT_R10G10B10A2_UNORM;
					swap_size = 4;
					flags = dds::DDPF_RGBA;
					position = glm::uvec4(3, 2, 1, 0);
					return true;
				case tft_rgb10_a2_ui:
					format = dds::DXGI_FORMAT_R10G10B10A2_UINT;
					swap_size = 4;
					flags = dds::DDPF_RGBA;
					position = glm::uvec4(3, 2, 1, 0);
					return true;
				case tft_r11f_b11f_g10f:
					format = dds::DXGI_FORMAT_R11G11B10_FLOAT;
					swap_size = 4;
					flags = dds::DDPF_RGB;
					return true;
				case tft_rgba8:
					format = dds::DXGI_FORMAT_R8G8B8A8_UNORM;
					swap_size = 1;
					flags = dds::DDPF_RGBA;
					return true;
				case tft_srgb8_a8:
					format = dds::DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
					swap_size = 1;
					flags = dds::DDPF_RGBA;
					return true;
				case tft_rgba8_ui:
					format = dds::DXGI_FORMAT_R8G8B8A8_UINT;
					swap_size = 1;
					flags = dds::DDPF_RGBA;
					return true;
				case tft_rgba8_snorm:
					format = dds::DXGI_FORMAT_R8G8B8A8_SNORM;
					swap_size = 1;
					flags = dds::DDPF_RGBA;
					return true;
				case tft_rgba8_i:
					format = dds::DXGI_FORMAT_R8G8B8A8_SINT;
					swap_size = 1;
					flags = dds::DDPF_RGBA;
					return true;
				case tft_rg16f:
					format = dds::DXGI_FORMAT_R16G16_FLOAT;
					swap_size = 2;
					flags = dds::DDPF_RGB;
					return true;
				case tft_rg16:
					format = dds::DXGI_FORMAT_R16G16_UNORM;
					swap_size = 2;
					flags = dds::DDPF_RGB;
					return true;
				case tft_rg16_ui:
					format = dds::DXGI_FORMAT_R16G16_UINT;
					swap_size = 2;
					flags = dds::DDPF_RGB;
					return true;
				case tft_rg16_snorm:
					format = dds::DXGI_FORMAT_R16G16_SNORM;
					swap_size = 2;
					flags = dds::DDPF_RGB;
					return true;
				case tft_rg16_i:
					format = dds::DXGI_FORMAT_R16G16_SINT;
					swap_size = 2;
					flags = dds::DDPF_LUMINANCE;
					return true;
				case tft_depth32f:
					format = dds::DXGI_FORMAT_D32_FLOAT;
					swap_size = 4;
					flags = dds::DDPF_LUMINANCE;
					return true;
				case tft_r32f:
					format = dds::DXGI_FORMAT_R32_FLOAT;
					swap_size = 4;
					flags = dds::DDPF_LUMINANCE;
					return true;
				case tft_r32_ui:
					format = dds::DXGI_FORMAT_R32_UINT;
					swap_size = 4;
					flags = dds::DDPF_LUMINANCE;
					return true;
				case tft_r32_i:
					format = dds::DXGI_FORMAT_R32_SINT;
					swap_size = 4;
					flags = dds::DDPF_LUMINANCE;
					return true;
				case tft_depth24_stencil8:
					format = dds::DXGI_FORMAT_D24_UNORM_S8_UINT;
					swap_size = 4;
					return true;
				case tft_rg8:
					format = dds::DXGI_FORMAT_R8G8_UNORM;
					swap_size = 1;
					flags = dds::DDPF_RGB;
					return true;
				case tft_rg8_ui:
					format = dds::DXGI_FORMAT_R8G8_UINT;
					swap_size = 1;
					flags = dds::DDPF_RGB;
					return true;
				case tft_rg8_snorm:
					format = dds::DXGI_FORMAT_R8G8_SNORM;
					swap_size = 1;
					flags = dds::DDPF_RGB;
					return true;
				case tft_rg8_i:
					format = dds::DXGI_FORMAT_R8G8_SINT;
					swap_size = 1;
					flags = dds::DDPF_RGB;
					return true;
				case tft_r16f:
					format = dds::DXGI_FORMAT_R16_FLOAT;
					swap_size = 2;
					flags = dds::DDPF_LUMINANCE;
					return true;
				case tft_depth16:
					format = dds::DXGI_FORMAT_D16_UNORM;
					swap_size = 2;
					flags = dds::DDPF_LUMINANCE;
					return true;
				case tft_r16:
					format = dds::DXGI_FORMAT_R16_UNORM;
					swap_size = 2;
					flags = dds::DDPF_LUMINANCE;
					return true;
				case tft_r16_ui:
					format = dds::DXGI_FORMAT_R16_UINT;
					swap_size = 2;
					flags = dds::DDPF_LUMINANCE;
					return true;
				case tft_r16_snorm:
					format = dds::DXGI_FORMAT_R16_SNORM;
					swap_size = 2;
					flags = dds::DDPF_LUMINANCE;
					return true;
				case tft_r16_i:
					format = dds::DXGI_FORMAT_R16_SINT;
					swap_size = 2;
					flags = dds::DDPF_LUMINANCE;
					return true;
				case tft_r8:
					format = dds::DXGI_FORMAT_R8_UNORM;
					swap_size = 1;
					flags = dds::DDPF_LUMINANCE;
					return true;
				case tft_r8_ui:
					format = dds::DXGI_FORMAT_R8_UINT;
					swap_size = 1;
					flags = dds::DDPF_LUMINANCE;
					return true;
				case tft_r8_snorm:
					format = dds::DXGI_FORMAT_R8_SNORM;
					swap_size = 1;
					flags = dds::DDPF_LUMINANCE;
					return true;
				case tft_r8_i:
					format = dds::DXGI_FORMAT_R8_SINT;
					swap_size = 1;
					flags = dds::DDPF_LUMINANCE;
					return true;
				case tft_a8:
					format = dds::DXGI_FORMAT_A8_UNORM;
					swap_size = 1;
					flags = dds::DDPF_ALPHA;
					return true;
				case tft_rgb9_e5:
					format = dds::DXGI_FORMAT_R9G9B9E5_SHAREDEXP;
					swap_size = 4;
					flags = dds::DDPF_RGB;
					return true;
				case tft_rgb_dxt1:
				case tft_rgba_dxt1:
					format = dds::DXGI_FORMAT_BC1_UNORM;
					swap_size = 1;
					flags = dds::DDPF_FOURCC;
					return true;
				case tft_srgb_dxt1:
				case tft_srgb_a_dxt1:
					format = dds::DXGI_FORMAT_BC1_UNORM_SRGB;
					swap_size = 1;
					flags = dds::DDPF_FOURCC;
					return true;
				case tft_rgba_dxt3:
					format = dds::DXGI_FORMAT_BC2_UNORM;
					swap_size = 1;
					flags = dds::DDPF_FOURCC;
					return true;
				case tft_srgb_a_dxt3:
					format = dds::DXGI_FORMAT_BC2_UNORM_SRGB;
					swap_size = 1;
					flags = dds::DDPF_FOURCC;
					return true;
				case tft_rgba_dxt5:
					format = dds::DXGI_FORMAT_BC3_UNORM;
					swap_size = 1;
					flags = dds::DDPF_FOURCC;
					return true;
				case tft_srgb_a_dxt5:
					format = dds::DXGI_FORMAT_BC3_UNORM_SRGB;
					swap_size = 1;
					flags = dds::DDPF_FOURCC;
					return true;
				case tft_r_rgtc1:
					format = dds::DXGI_FORMAT_BC4_UNORM;
					swap_size = 1;
					flags = dds::DDPF_FOURCC;
					return true;
				case tft_signed_r_rgtc1:
					format = dds::DXGI_FORMAT_BC4_SNORM;
					swap_size = 1;
					flags = dds::DDPF_FOURCC;
					return true;
				case tft_rg_rgtc2:
					format = dds::DXGI_FORMAT_BC5_UNORM;
					swap_size = 1;
					flags = dds::DDPF_FOURCC;
					return true;
				case tft_signed_rg_rgtc2:
					format = dds::DXGI_FORMAT_BC5_SNORM;
					swap_size = 1;
					flags = dds::DDPF_FOURCC;
					return true;
				case tft_l8:
					format = dds::DXGI_FORMAT_R8_UNORM;
					swap_size = 1;
					flags = dds::DDPF_LUMINANCE;
					return true;
				case tft_la8:
					format = dds::DXGI_FORMAT_R8G8_UNORM;
					swap_size = 1;
					flags = dds::DDPF_RGB;
					return true;
				case tft_l16:
					format = dds::DXGI_FORMAT_R16_UNORM;
					swap_size = 2;
					flags = dds::DDPF_LUMINANCE;
					return true;
				case tft_la16:
					format = dds::DXGI_FORMAT_R16G16_UNORM;
					swap_size = 2;
					flags = dds::DDPF_RGB;
					return true;
				case tft_r5g6b5:
					format = dds::DXGI_FORMAT_B5G6R5_UNORM;
					swap_size = 2;
					flags = dds::DDPF_RGB;
					position = glm::uvec4(2, 1, 0, 3);
					return true;
				case tft_rgb5_a1:
					format = dds::DXGI_FORMAT_B5G5R5A1_UNORM;
					swap_size = 2;
					flags = dds::DDPF_RGBA;
					position = glm::uvec4(2, 1, 0, 3);
					return true;
				case tft_rgba4:
					format = dds::DXGI_FORMAT_B4G4R4A4_UNORM;
					swap_size = 2;
					flags = dds::DDPF_RGBA;
					position = glm::uvec4(2, 1, 0, 3);
					return true;
				default:
					break;
			}

			return false;
		}

		bool get_rgba_masks_needed(const Uint32 fourcc)
		{
			switch (fourcc)
			{
				case dds::DDSFMT_DXT1:
				case dds::DDSFMT_DXT2:
				case dds::DDSFMT_DXT3:
				case dds::DDSFMT_DXT4:
				case dds::DDSFMT_DXT5:
				case dds::DDSFMT_ATI1:
				case dds::DDSFMT_ATI2:
				case dds::DDSFMT_R32F:
				case dds::DDSFMT_G32R32F:
				case dds::DDSFMT_A32B32G32R32F:
				case dds::DDSFMT_R16F:
				case dds::DDSFMT_G16R16F:
				case dds::DDSFMT_A16B16G16R16F:
				case dds::DDSFMT_L16:
				case dds::DDSFMT_G16R16:
				case dds::DDSFMT_A16B16G16R16:
				case dds::DDSFMT_A16B16G16R16_SIGNED:
					return false;
				case dds::DDSFMT_R8G8B8:
				case dds::DDSFMT_A8R8G8B8:
				case dds::DDSFMT_R5G6B5:
				case dds::DDSFMT_X1R5G5B5:
				case dds::DDSFMT_A1R5G5B5:
				case dds::DDSFMT_A4R4G4B4:
				case dds::DDSFMT_X4R4G4B4:
				case dds::DDSFMT_R3G3B2:
				case dds::DDSFMT_A8:
				case dds::DDSFMT_A2R10G10B10:
				case dds::DDSFMT_A2B10G10R10:
				case dds::DDSFMT_L8:
				case dds::DDSFMT_A8L8:
				case dds::DDSFMT_A8B8G8R8:
					return true;
			}

			EL_THROW_EXCEPTION(DdsUnkownFormatException());
		}

		void get_rgba_masks(const Uint32 fourcc, glm::uvec4 &masks)
		{
			switch (fourcc)
			{
				case dds::DDSFMT_DXT1:
				case dds::DDSFMT_DXT2:
				case dds::DDSFMT_DXT3:
				case dds::DDSFMT_DXT4:
				case dds::DDSFMT_DXT5:
				case dds::DDSFMT_ATI1:
				case dds::DDSFMT_ATI2:
				case dds::DDSFMT_R32F:
				case dds::DDSFMT_G32R32F:
				case dds::DDSFMT_A32B32G32R32F:
				case dds::DDSFMT_R16F:
				case dds::DDSFMT_G16R16F:
				case dds::DDSFMT_A16B16G16R16F:
				case dds::DDSFMT_L16:
				case dds::DDSFMT_G16R16:
				case dds::DDSFMT_A16B16G16R16:
				case dds::DDSFMT_A16B16G16R16_SIGNED:
					return;
				case dds::DDSFMT_R8G8B8:
					masks.r = 0x00FF0000;
					masks.g = 0x0000FF00;
					masks.b = 0x000000FF;
					masks.a = 0x00000000;
					return;
				case dds::DDSFMT_A8R8G8B8:
					masks.r = 0x00FF0000;
					masks.g = 0x0000FF00;
					masks.b = 0x000000FF;
					masks.a = 0xFF000000;
					return;
				case dds::DDSFMT_R5G6B5:
					masks.r = 0x0000F800;
					masks.g = 0x000007E0;
					masks.b = 0x0000001F;
					masks.a = 0x00000000;
					return;
				case dds::DDSFMT_X1R5G5B5:
					masks.r = 0x00007C00;
					masks.g = 0x000003E0;
					masks.b = 0x0000001F;
					masks.a = 0x00008000;
					return;
				case dds::DDSFMT_A1R5G5B5:
					masks.r = 0x00007C00;
					masks.g = 0x000003E0;
					masks.b = 0x0000001F;
					masks.a = 0x00008000;
					return;
				case dds::DDSFMT_A4R4G4B4:
					masks.r = 0x00000F00;
					masks.g = 0x000000F0;
					masks.b = 0x0000000F;
					masks.a = 0x0000F000;
					return;
				case dds::DDSFMT_X4R4G4B4:
					masks.r = 0x00000F00;
					masks.g = 0x000000F0;
					masks.b = 0x0000000F;
					masks.a = 0x0000F000;
					return;
				case dds::DDSFMT_R3G3B2:
					masks.r = 0x000000E0;
					masks.g = 0x0000001C;
					masks.b = 0x00000003;
					masks.a = 0x00000000;
					return;
				case dds::DDSFMT_A8:
					masks.r = 0x00000000;
					masks.g = 0x00000000;
					masks.b = 0x00000000;
					masks.a = 0x000000FF;
					return;
				case dds::DDSFMT_A2R10G10B10:
					masks.r = 0x000003FF;
					masks.g = 0x000FFC00;
					masks.b = 0x3FF00000;
					masks.a = 0xC0000000;
					return;
				case dds::DDSFMT_A2B10G10R10:
					masks.r = 0x3FF00000;
					masks.g = 0x000FFC00;
					masks.b = 0x000003FF;
					masks.a = 0xC0000000;
					return;
				case dds::DDSFMT_L8:
					masks.r = 0x000000FF;
					masks.g = 0x00000000;
					masks.b = 0x00000000;
					masks.a = 0x00000000;
					return;
				case dds::DDSFMT_A8L8:
					masks.r = 0x000000FF;
					masks.g = 0x00000000;
					masks.b = 0x00000000;
					masks.a = 0x0000FF00;
					return;
				case dds::DDSFMT_A8B8G8R8:
					masks.r = 0x000000FF;
					masks.g = 0x0000FF00;
					masks.b = 0x00FF0000;
					masks.a = 0xFF000000;
					return;
			}

			EL_THROW_EXCEPTION(DdsUnkownFormatException());
		}

		bool get_format(const Uint32 fourcc, Uint32 &pixel_size,
			GLenum &format, GLenum &type)
		{
			switch (fourcc)
			{
				case dds::DDSFMT_DXT1:
				case dds::DDSFMT_ATI1:
					pixel_size = 4;
					format = GL_NONE;
					type = GL_NONE;
					return true;
				case dds::DDSFMT_DXT2:
				case dds::DDSFMT_DXT3:
				case dds::DDSFMT_DXT4:
				case dds::DDSFMT_DXT5:
				case dds::DDSFMT_ATI2:
					pixel_size = 8;
					format = GL_NONE;
					type = GL_NONE;
					return true;
				case dds::DDSFMT_R32F:
					pixel_size = 32;
					format = GL_RED;
					type = GL_FLOAT;
					return true;
				case dds::DDSFMT_G32R32F:
					pixel_size = 64;
					format = GL_RG;
					type = GL_FLOAT;
					return true;
				case dds::DDSFMT_A32B32G32R32F:
					pixel_size = 128;
					format = GL_RGBA;
					type = GL_FLOAT;
					return true;
				case dds::DDSFMT_R16F:
					pixel_size = 16;
					format = GL_RED;
					type = GL_HALF_FLOAT;
					return true;
				case dds::DDSFMT_G16R16F:
					pixel_size = 32;
					format = GL_RG;
					type = GL_HALF_FLOAT;
					return true;
				case dds::DDSFMT_A16B16G16R16F:
					pixel_size = 64;
					format = GL_RGBA;
					type = GL_HALF_FLOAT;
					return true;
				case dds::DDSFMT_X1R5G5B5:
				case dds::DDSFMT_X4R4G4B4:
				case dds::DDSFMT_R8G8B8:
				case dds::DDSFMT_A8R8G8B8:
				case dds::DDSFMT_R5G6B5:
				case dds::DDSFMT_A1R5G5B5:
				case dds::DDSFMT_A4R4G4B4:
				case dds::DDSFMT_R3G3B2:
				case dds::DDSFMT_A8:
				case dds::DDSFMT_A2R10G10B10:
				case dds::DDSFMT_A2B10G10R10:
				case dds::DDSFMT_L8:
				case dds::DDSFMT_A8L8:
				case dds::DDSFMT_A8B8G8R8:
					return false;
				case dds::DDSFMT_L16:
					pixel_size = 16;
					format = GL_RED;
					type = GL_UNSIGNED_SHORT;
					return true;
				case dds::DDSFMT_G16R16:
					pixel_size = 32;
					format = GL_RG;
					type = GL_UNSIGNED_SHORT;
					return true;
				case dds::DDSFMT_A16B16G16R16:
					pixel_size = 64;
					format = GL_RGBA;
					type = GL_UNSIGNED_SHORT;
					return true;
				case dds::DDSFMT_A16B16G16R16_SIGNED:
					pixel_size = 64;
					format = GL_RGBA;
					type = GL_SHORT;
					return true;
			}

			EL_THROW_EXCEPTION(DdsUnkownFormatException());
		}

		void check_fourcc_support(const Uint32 fourcc,
			const String &format_str, const Uint32 pixel_size,
				const bool rg_formats)
		{
			glm::uvec4 masks;
			Uint32 bits;
			GLenum format, type;
			bool supported;

			if (get_rgba_masks_needed(fourcc))
			{
				get_rgba_masks(fourcc, masks);

				supported =
					CodecManager::is_fast_load_supported(
						masks, rg_formats, false);

				bits = __builtin_popcount(masks[0] | masks[1] |
					masks[2] | masks[3]);
			}
			else
			{
				supported = get_format(fourcc, bits, format,
					type);
			}

			if (!supported)
			{
				EL_THROW_EXCEPTION(
					DdsFormatNotSupportedException()
					<< errinfo_message(format_str));
			}

			if (bits != pixel_size)
			{
				EL_THROW_EXCEPTION(DdsUnkownFormatException()
					<< errinfo_message(format_str));
			}

			LOG_DEBUG_VERBOSE(lt_dds_image, UTF8("Loading of DDS "
				"FourCC %1% is supported."), format_str);
		}

		class DdsImageLoader
		{
			private:
				dds::DdsHeader m_header;
				dds::DdsHeader10 m_header_dxt10;
				ImageSharedPtr m_image;
				ReaderSharedPtr m_reader;

				void load_supported(
					const ImageCompressionTypeSet
					&compression, const bool rg_formats,
					const bool sRGB);
				void load_supported_dxt10(
					const ImageCompressionTypeSet
					&compression, const bool rg_formats,
					const bool merge_layers);
				void set_format(const bool rg_formats,
					const bool sRGB);
				void set_dxt10_format(
					const TextureFormatType texture_format,
					const bool rg_formats);
				void uncompress(const TextureFormatType format,
					const bool rg_formats, const bool sRGB,
					const bool merge_layers);
				void load(const ImageCompressionTypeSet
					&compression, const bool rg_formats,
					const bool sRGB,
					const bool merge_layers);
				void load(const Uint16 swap_size);
				void set_format_mask(const glm::uvec4 &masks,
					const bool rg_formats,
					const bool sRGB);
				void set_format_mask(const glm::uvec4 &masks,
					const TextureFormatType format,
					const bool rg_formats,
					const bool sRGB);
				void set_format(const Uint32 pixel_size,
					const GLenum format, const GLenum type,
					const Uint16 swap_size,
					const bool rg_formats,
					const bool sRGB);
				Uint16 get_faces() const;
				glm::uvec3 get_size() const;
				bool get_array() const;

			public:
				DdsImageLoader(const ReaderSharedPtr &reader,
					const ImageCompressionTypeSet
					&compression, const bool rg_formats,
					const bool sRGB,
					const bool merge_layers);

				inline const ImageSharedPtr &get_image()
				{
					return m_image;
				}

		};

		DdsImageLoader::DdsImageLoader(const ReaderSharedPtr &reader,
			const ImageCompressionTypeSet &compression,
			const bool rg_formats, const bool sRGB,
			const bool merge_layers): m_reader(reader)
		{
			try
			{
				init_dds_image(m_reader, m_header,
					m_header_dxt10);
				load(compression, rg_formats, sRGB,
					merge_layers);
			}
			catch (boost::exception &exception)
			{
				exception << boost::errinfo_file_name(
					m_reader->get_name());
				throw;
			}
		}

		glm::uvec3 DdsImageLoader::get_size() const
		{
			Uint32 layer;

			if (m_header.m_pixel_format.m_fourcc !=
				dds::DDSFMT_DX10)
			{
				return glm::uvec3(m_header.m_width,
					m_header.m_height, m_header.m_depth);
			}

			layer = 0;

			if (get_array())
			{
				layer = m_header_dxt10.m_array_size;
			}

			switch (m_header_dxt10.m_resource_dimension)
			{
				case dds::DDS_DIMENSION_TEXTURE1D:
					return glm::uvec3(m_header.m_width,
						layer, 0);
				case dds::DDS_DIMENSION_TEXTURE2D:
					return glm::uvec3(m_header.m_width,
						m_header.m_height, layer);
				case dds::DDS_DIMENSION_TEXTURE3D:
					return glm::uvec3(m_header.m_width,
						m_header.m_height,
						m_header.m_depth);
			};

			return glm::uvec3(0);
		}

		bool DdsImageLoader::get_array() const
		{
			if (m_header.m_pixel_format.m_fourcc !=
				dds::DDSFMT_DX10)
			{
				return false;
			}

			return m_header_dxt10.m_array_size > 1;
		}

		void DdsImageLoader::set_format_mask(const glm::uvec4 &masks,
			const bool rg_formats, const bool sRGB)
		{
			Uint32 pixel_size;
			Uint32 swap_size;
			GLenum format;
			GLenum type;

			if (CodecManager::is_fast_load_supported(masks,
				rg_formats, false, type, format, pixel_size,
				swap_size))
			{
				m_image = boost::make_shared<Image>(
					m_reader->get_name(),
					get_cube_map(m_header),
					get_texture_format(m_header,
						rg_formats, sRGB), get_size(),
					static_cast<Uint16>(
					m_header.m_mipmap_count), format, type,
					false, get_array());

				load(swap_size);
			}
			else
			{
				EL_THROW_EXCEPTION(DdsErrorException()
					<< boost::errinfo_file_name(
						m_reader->get_name()));
			}
		}

		void DdsImageLoader::set_format_mask(const glm::uvec4 &masks,
			const TextureFormatType texture_format,
			const bool rg_formats, const bool sRGB)
		{
			Uint32 pixel_size;
			Uint32 swap_size;
			GLenum format;
			GLenum type;

			if (CodecManager::is_fast_load_supported(masks,
				rg_formats, false, type, format, pixel_size,
				swap_size))
			{
				m_image = boost::make_shared<Image>(
					m_reader->get_name(),
					get_cube_map(m_header), texture_format,
					get_size(), static_cast<Uint16>(
					m_header.m_mipmap_count), format, type,
					sRGB, get_array());
				load(swap_size);
			}
			else
			{
				EL_THROW_EXCEPTION(DdsErrorException()
					<< boost::errinfo_file_name(
						m_reader->get_name()));
			}
		}

		void DdsImageLoader::load(
			const ImageCompressionTypeSet &compression,
			const bool rg_formats, const bool sRGB,
			const bool merge_layers)
		{
			LOG_DEBUG(lt_dds_image, UTF8("Loading file '%1%'."),
				m_reader->get_name());

			if (m_header.m_pixel_format.m_fourcc ==
				dds::DDSFMT_DX10)
			{
				load_supported_dxt10(compression, rg_formats,
					merge_layers);

				return;
			}

			if ((m_header.m_pixel_format.m_flags & dds::DDPF_FOURCC)
				!= dds::DDPF_FOURCC)
			{
				set_format_mask(glm::uvec4(
					m_header.m_pixel_format.m_red_mask,
					m_header.m_pixel_format.m_green_mask,
					m_header.m_pixel_format.m_blue_mask,
					m_header.m_pixel_format.m_alpha_mask),
					rg_formats, sRGB);

				return;
			}

			load_supported(compression, rg_formats, sRGB);
		}

		void DdsImageLoader::set_format(const Uint32 pixel_size,
			const GLenum format, const GLenum type,
			const Uint16 swap_size, const bool rg_formats,
			const bool sRGB)
		{
			m_image = boost::make_shared<Image>(
				m_reader->get_name(), get_cube_map(m_header),
				get_texture_format(m_header, rg_formats, sRGB),
				get_size(),
				static_cast<Uint16>(m_header.m_mipmap_count),
				format, type, sRGB, get_array());

			load(swap_size);
		}

		void DdsImageLoader::set_format(const bool rg_formats,
			const bool sRGB)
		{
			assert(TextureFormatUtil::get_compressed(
				get_texture_format(m_header, rg_formats,
				sRGB)));

			m_image = boost::make_shared<Image>(
				m_reader->get_name(), get_cube_map(m_header),
				get_texture_format(m_header, rg_formats, sRGB),
				get_size(), static_cast<Uint16>(
					m_header.m_mipmap_count), get_array());

			load(1);
		}

		void DdsImageLoader::set_dxt10_format(
			const TextureFormatType texture_format,
			const bool rg_formats)
		{
			glm::uvec4 position;
			TextureFormatType tmp;
			Uint32 format, bpp, swap_size, flags;
			bool linear_size;

			get_dxt10_format(texture_format, position, format, bpp,
				swap_size, flags, linear_size), 

			tmp = texture_format;

			if (!rg_formats)
			{
				switch (texture_format)
				{
					case tft_r8:
						tmp = tft_l8;
						break;
					case tft_rg8:
						tmp = tft_la8;
						break;
					case tft_r16:
						tmp = tft_l16;
						break;
					case tft_rg16:
						tmp = tft_la16;
						break;
					default:
						break;
				};
			}

			m_image = boost::make_shared<Image>(
				m_reader->get_name(), get_cube_map(m_header),
				tmp, get_size(), static_cast<Uint16>(
					m_header.m_mipmap_count), get_array());

			load(swap_size);
		}

		void DdsImageLoader::load(const Uint16 swap_size)
		{
			if (m_header.m_pixel_format.m_fourcc ==
				dds::DDSFMT_DX10)
			{
				m_reader->set_position(dds::DDS_DATA_POSITION +
					dds::DDS_HEADER_SIZE_DX10);
			}
			else
			{
				m_reader->set_position(dds::DDS_DATA_POSITION);
			}

			m_reader->read_le(*m_image->get_buffer(), swap_size);
		}

		void DdsImageLoader::load_supported(
			const ImageCompressionTypeSet &compression,
			const bool rg_formats, const bool sRGB)
		{
			glm::uvec4 masks;
			Uint32 fourcc, pixel_size, format, type;

			fourcc = m_header.m_pixel_format.m_fourcc;

			switch (fourcc)
			{
				case dds::DDSFMT_DXT1:
					if (compression.count(ict_s3tc) == 0)
					{
						uncompress(tft_rgba_dxt1,
							rg_formats, sRGB,
							false);
						return;
					}

					set_format(rg_formats, sRGB);
					return;
				case dds::DDSFMT_DXT2:
				case dds::DDSFMT_DXT3:
					if (compression.count(ict_s3tc) == 0)
					{
						uncompress(tft_rgba_dxt3,
							rg_formats, sRGB,
							false);
						return;
					}

					set_format(rg_formats, sRGB);
					return;
				case dds::DDSFMT_DXT4:
				case dds::DDSFMT_DXT5:
					if (compression.count(ict_s3tc) == 0)
					{
						uncompress(tft_rgba_dxt5,
							rg_formats, sRGB,
							false);
						return;
					}

					set_format(rg_formats, sRGB);
					return;
				case dds::DDSFMT_ATI1:
					if (compression.count(ict_rgtc) == 0)
					{
						uncompress(tft_r_rgtc1,
							rg_formats, false,
							false);
						return;
					}

					set_format(rg_formats, false);
					return;
				case dds::DDSFMT_ATI2:
					if (compression.count(ict_rgtc) == 0)
					{
						uncompress(tft_rg_rgtc2,
							rg_formats, false,
							false);
						return;
					}

					set_format(rg_formats, false);
					return;
				case dds::DDSFMT_R32F:
				case dds::DDSFMT_G32R32F:
				case dds::DDSFMT_A32B32G32R32F:
					get_format(fourcc, pixel_size, format,
						type);
					set_format(pixel_size, format, type, 4,
						rg_formats, false);
					return;
				case dds::DDSFMT_R16F:
				case dds::DDSFMT_G16R16F:
				case dds::DDSFMT_A16B16G16R16F:
					get_format(fourcc, pixel_size, format,
						type);
					set_format(pixel_size, format, type, 2,
						rg_formats, false);
					return;
				case dds::DDSFMT_X1R5G5B5:
				case dds::DDSFMT_X4R4G4B4:
				case dds::DDSFMT_R8G8B8:
				case dds::DDSFMT_A8R8G8B8:
				case dds::DDSFMT_R5G6B5:
				case dds::DDSFMT_A1R5G5B5:
				case dds::DDSFMT_A4R4G4B4:
				case dds::DDSFMT_R3G3B2:
				case dds::DDSFMT_A8:
				case dds::DDSFMT_A2R10G10B10:
				case dds::DDSFMT_A2B10G10R10:
				case dds::DDSFMT_L8:
				case dds::DDSFMT_A8B8G8R8:
				case dds::DDSFMT_A8L8:
					assert(get_rgba_masks_needed(fourcc));
					get_rgba_masks(fourcc, masks);
					set_format_mask(masks, rg_formats,
						sRGB);
					return;
				case dds::DDSFMT_L16:
				case dds::DDSFMT_G16R16:
				case dds::DDSFMT_A16B16G16R16:
					get_format(fourcc, pixel_size, format,
						type);
					set_format(pixel_size, format, type, 2,
						rg_formats, false);
					return;
				case dds::DDSFMT_A16B16G16R16_SIGNED:
					get_format(fourcc, pixel_size, format,
						type);
					set_format(pixel_size, format, type, 2,
						rg_formats, false);
					return;
			}
			EL_THROW_EXCEPTION(DdsUnkownFormatException());
		}

		void DdsImageLoader::load_supported_dxt10(
			const ImageCompressionTypeSet &compression,
			const bool rg_formats, const bool merge_layers)
		{
			Uint32 format;

			format = m_header_dxt10.m_dxgi_format;

			switch (format)
			{
				case dds::DXGI_FORMAT_UNKNOWN:
					EL_THROW_EXCEPTION(
						DdsUnkownFormatException());
					break;
				case dds::DXGI_FORMAT_R32G32B32A32_TYPELESS:
				case dds::DXGI_FORMAT_R32G8X24_TYPELESS:
				case dds::DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
				case dds::DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
				case dds::DXGI_FORMAT_R32G32B32_TYPELESS:
				case dds::DXGI_FORMAT_R16G16B16A16_TYPELESS:
				case dds::DXGI_FORMAT_R32G32_TYPELESS:
				case dds::DXGI_FORMAT_R10G10B10A2_TYPELESS:
				case dds::DXGI_FORMAT_R8G8B8A8_TYPELESS:
				case dds::DXGI_FORMAT_R16G16_TYPELESS:
				case dds::DXGI_FORMAT_R32_TYPELESS:
				case dds::DXGI_FORMAT_R24G8_TYPELESS:
				case dds::DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
				case dds::DXGI_FORMAT_X24_TYPELESS_G8_UINT:
				case dds::DXGI_FORMAT_R8G8_TYPELESS:
				case dds::DXGI_FORMAT_R16_TYPELESS:
				case dds::DXGI_FORMAT_R8_TYPELESS:
				case dds::DXGI_FORMAT_BC1_TYPELESS:
				case dds::DXGI_FORMAT_BC2_TYPELESS:
				case dds::DXGI_FORMAT_BC3_TYPELESS:
				case dds::DXGI_FORMAT_BC4_TYPELESS:
				case dds::DXGI_FORMAT_BC5_TYPELESS:
				case dds::DXGI_FORMAT_B8G8R8A8_TYPELESS:
				case dds::DXGI_FORMAT_B8G8R8X8_TYPELESS:
				case dds::DXGI_FORMAT_BC6H_TYPELESS:
				case dds::DXGI_FORMAT_BC7_TYPELESS:
					EL_THROW_EXCEPTION(
						DdsFormatNotSupportedException()
						<< errinfo_item_id(format));
					break;
				case dds::DXGI_FORMAT_R8G8_B8G8_UNORM:
				case dds::DXGI_FORMAT_G8R8_G8B8_UNORM:
				case dds::DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM:
				case dds::DXGI_FORMAT_BC6H_UF16:
				case dds::DXGI_FORMAT_BC6H_SF16:
				case dds::DXGI_FORMAT_BC7_UNORM:
				case dds::DXGI_FORMAT_BC7_UNORM_SRGB:
					EL_THROW_EXCEPTION(
						DdsFormatNotSupportedException()
						<< errinfo_item_id(format));
					break;
				case dds::DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
					set_dxt10_format(tft_depth32f_stencil8,
						rg_formats);
					return;
				case dds::DXGI_FORMAT_R32G32B32A32_FLOAT:
					set_dxt10_format(tft_rgba32f,
						rg_formats);
					return;
				case dds::DXGI_FORMAT_R32G32B32A32_UINT:
					set_dxt10_format(tft_rgba32_ui,
						rg_formats);
					return;
				case dds::DXGI_FORMAT_R32G32B32A32_SINT:
					set_dxt10_format(tft_rgba32_i,
						rg_formats);
					return;
				case dds::DXGI_FORMAT_R32G32B32_FLOAT:
					set_dxt10_format(tft_rgb32f,
						rg_formats);
					return;
				case dds::DXGI_FORMAT_R32G32B32_UINT:
					set_dxt10_format(tft_rgb32_ui,
						rg_formats);
					return;
				case dds::DXGI_FORMAT_R32G32B32_SINT:
					set_dxt10_format(tft_rgb32_i,
						rg_formats);
					return;
				case dds::DXGI_FORMAT_R16G16B16A16_FLOAT:
					set_dxt10_format(tft_rgba16f,
						rg_formats);
					return;
				case dds::DXGI_FORMAT_R16G16B16A16_UNORM:
					set_dxt10_format(tft_rgba16,
						rg_formats);
					return;
				case dds::DXGI_FORMAT_R16G16B16A16_UINT:
					set_dxt10_format(tft_rgba16_ui,
						rg_formats);
					return;
				case dds::DXGI_FORMAT_R16G16B16A16_SNORM:
					set_dxt10_format(tft_rgba16_snorm,
						rg_formats);
					return;
				case dds::DXGI_FORMAT_R16G16B16A16_SINT:
					set_dxt10_format(tft_rgba16_i,
						rg_formats);
					return;
				case dds::DXGI_FORMAT_R32G32_FLOAT:
					set_dxt10_format(tft_rg32f,
						rg_formats);
					return;
				case dds::DXGI_FORMAT_R32G32_UINT:
					set_dxt10_format(tft_rg32_ui,
						rg_formats);
					return;
				case dds::DXGI_FORMAT_R32G32_SINT:
					set_dxt10_format(tft_rg32_i,
						rg_formats);
					return;
				case dds::DXGI_FORMAT_R10G10B10A2_UNORM:
					set_dxt10_format(tft_rgb10_a2,
						rg_formats);
					return;
				case dds::DXGI_FORMAT_R10G10B10A2_UINT:
					set_dxt10_format(tft_rgb10_a2_ui,
						rg_formats);
					return;
				case dds::DXGI_FORMAT_R11G11B10_FLOAT:
					set_dxt10_format(tft_r11f_b11f_g10f,
						rg_formats);
					return;
				case dds::DXGI_FORMAT_R8G8B8A8_UNORM:
					set_dxt10_format(tft_rgba8,
						rg_formats);
					return;
				case dds::DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
					set_dxt10_format(tft_srgb8_a8,
						rg_formats);
					return;
				case dds::DXGI_FORMAT_R8G8B8A8_UINT:
					set_dxt10_format(tft_rgba8_ui,
						rg_formats);
					return;
				case dds::DXGI_FORMAT_R8G8B8A8_SNORM:
					set_dxt10_format(tft_rgba8_snorm,
						rg_formats);
					return;
				case dds::DXGI_FORMAT_R8G8B8A8_SINT:
					set_dxt10_format(tft_rgba8_i,
						rg_formats);
					return;
				case dds::DXGI_FORMAT_R16G16_FLOAT:
					set_dxt10_format(tft_rg16f,
						rg_formats);
					return;
				case dds::DXGI_FORMAT_R16G16_UNORM:
					set_dxt10_format(tft_rg16,
						rg_formats);
					return;
				case dds::DXGI_FORMAT_R16G16_UINT:
					set_dxt10_format(tft_rg16_ui,
						rg_formats);
					return;
				case dds::DXGI_FORMAT_R16G16_SNORM:
					set_dxt10_format(tft_rg16_snorm,
						rg_formats);
					return;
				case dds::DXGI_FORMAT_R16G16_SINT:
					set_dxt10_format(tft_rg16_i,
						rg_formats);
					return;
				case dds::DXGI_FORMAT_D32_FLOAT:
					set_dxt10_format(tft_depth32f,
						rg_formats);
					return;
				case dds::DXGI_FORMAT_R32_FLOAT:
					set_dxt10_format(tft_r32f,
						rg_formats);
					return;
				case dds::DXGI_FORMAT_R32_UINT:
					set_dxt10_format(tft_r32_ui,
						rg_formats);
					return;
				case dds::DXGI_FORMAT_R32_SINT:
					set_dxt10_format(tft_r32_i,
						rg_formats);
					return;
				case dds::DXGI_FORMAT_D24_UNORM_S8_UINT:
					set_dxt10_format(tft_depth24_stencil8,
						rg_formats);
					return;
				case dds::DXGI_FORMAT_R8G8_UNORM:
					set_dxt10_format(tft_rg8,
						rg_formats);
					return;
				case dds::DXGI_FORMAT_R8G8_UINT:
					set_dxt10_format(tft_rg8_ui,
						rg_formats);
					return;
				case dds::DXGI_FORMAT_R8G8_SNORM:
					set_dxt10_format(tft_rg8_snorm,
						rg_formats);
					return;
				case dds::DXGI_FORMAT_R8G8_SINT:
					set_dxt10_format(tft_rg8_i,
						rg_formats);
					return;
				case dds::DXGI_FORMAT_R16_FLOAT:
					set_dxt10_format(tft_r16f,
						rg_formats);
					return;
				case dds::DXGI_FORMAT_D16_UNORM:
					set_dxt10_format(tft_depth16,
						rg_formats);
					return;
				case dds::DXGI_FORMAT_R16_UNORM:
					set_dxt10_format(tft_r16,
						rg_formats);
					return;
				case dds::DXGI_FORMAT_R16_UINT:
					set_dxt10_format(tft_r16_ui,
						rg_formats);
					return;
				case dds::DXGI_FORMAT_R16_SNORM:
					set_dxt10_format(tft_r16_snorm,
						rg_formats);
					return;
				case dds::DXGI_FORMAT_R16_SINT:
					set_dxt10_format(tft_r16_i,
						rg_formats);
					return;
				case dds::DXGI_FORMAT_R8_UNORM:
					set_dxt10_format(tft_r8, rg_formats);
					return;
				case dds::DXGI_FORMAT_R8_UINT:
					set_dxt10_format(tft_r8_ui,
						rg_formats);
					return;
				case dds::DXGI_FORMAT_R8_SNORM:
					set_dxt10_format(tft_r8_snorm,
						rg_formats);
					return;
				case dds::DXGI_FORMAT_R8_SINT:
					set_dxt10_format(tft_r8_i, rg_formats);
					return;
				case dds::DXGI_FORMAT_A8_UNORM:
					set_dxt10_format(tft_a8, rg_formats);
					return;
				case dds::DXGI_FORMAT_R1_UNORM:
					break;
				case dds::DXGI_FORMAT_R9G9B9E5_SHAREDEXP:
					set_dxt10_format(tft_rgb9_e5,
						rg_formats);
					return;
				case dds::DXGI_FORMAT_BC1_UNORM:
					if (compression.count(ict_s3tc) == 0)
					{
						uncompress(tft_rgba_dxt1,
							rg_formats, false,
							merge_layers);
						return;
					}

					set_dxt10_format(tft_rgba_dxt1,
						rg_formats);
					return;
				case dds::DXGI_FORMAT_BC1_UNORM_SRGB:
					if (compression.count(ict_s3tc) == 0)
					{
						uncompress(tft_srgb_a_dxt1,
							rg_formats, true,
							merge_layers);
						return;
					}

					set_dxt10_format(tft_srgb_a_dxt1,
						rg_formats);
					return;
				case dds::DXGI_FORMAT_BC2_UNORM:
					if (compression.count(ict_s3tc) == 0)
					{
						uncompress(tft_rgba_dxt3,
							rg_formats, false,
							merge_layers);
						return;
					}

					set_dxt10_format(tft_rgba_dxt3,
						rg_formats);
					return;
				case dds::DXGI_FORMAT_BC2_UNORM_SRGB:
					if (compression.count(ict_s3tc) == 0)
					{
						uncompress(tft_srgb_a_dxt3,
							rg_formats, true,
							merge_layers);
						return;
					}

					set_dxt10_format(tft_srgb_a_dxt3,
						rg_formats);
					return;
				case dds::DXGI_FORMAT_BC3_UNORM:
					if (compression.count(ict_s3tc) == 0)
					{
						uncompress(tft_rgba_dxt5,
							rg_formats, false,
							merge_layers);
						return;
					}

					set_dxt10_format(tft_rgba_dxt5,
						rg_formats);
					return;
				case dds::DXGI_FORMAT_BC3_UNORM_SRGB:
					if (compression.count(ict_s3tc) == 0)
					{
						uncompress(tft_srgb_a_dxt5,
							rg_formats, true,
							merge_layers);
						return;
					}

					set_dxt10_format(tft_srgb_a_dxt5,
						rg_formats);
					return;
				case dds::DXGI_FORMAT_BC4_UNORM:
					if (compression.count(ict_rgtc) == 0)
					{
						uncompress(tft_r_rgtc1,
							rg_formats, false,
							merge_layers);
						return;
					}

					set_dxt10_format(tft_r_rgtc1,
						rg_formats);
					return;
				case dds::DXGI_FORMAT_BC4_SNORM:
					if (compression.count(ict_rgtc) == 0)
					{
						uncompress(tft_signed_r_rgtc1,
							rg_formats, false,
							merge_layers);
						return;
					}

					set_dxt10_format(tft_signed_r_rgtc1,
						rg_formats);
					return;
				case dds::DXGI_FORMAT_BC5_UNORM:
					if (compression.count(ict_rgtc) == 0)
					{
						uncompress(tft_rg_rgtc2,
							rg_formats, false,
							merge_layers);
						return;
					}

					set_dxt10_format(tft_rg_rgtc2,
						rg_formats);
					return;
				case dds::DXGI_FORMAT_BC5_SNORM:
					if (compression.count(ict_rgtc) == 0)
					{
						uncompress(tft_signed_rg_rgtc2,
							rg_formats, false,
							merge_layers);
						return;
					}

					set_dxt10_format(tft_signed_rg_rgtc2,
						rg_formats);
					return;
				case dds::DXGI_FORMAT_B5G6R5_UNORM:
					set_format_mask(glm::uvec4(0x0000F800,
						0x000007E0, 0x0000001F,
						0x00000000), tft_r5g6b5,
						rg_formats, false);
					return;
				case dds::DXGI_FORMAT_B5G5R5A1_UNORM:
					set_format_mask(glm::uvec4(0x00007C00,
						0x000003E0, 0x0000001F,
						0x00008000), tft_rgb5_a1,
						rg_formats, false);
					return;
				case dds::DXGI_FORMAT_B8G8R8A8_UNORM:
					set_format_mask(glm::uvec4(0x00FF0000,
						0x0000FF00, 0x000000FF,
						0xFF000000), tft_rgba8,
						rg_formats, false);
					return;
				case dds::DXGI_FORMAT_B8G8R8X8_UNORM:
					set_format_mask(glm::uvec4(0x00FF0000,
						0x0000FF00, 0x000000FF,
						0xFF000000), tft_rgb8,
						rg_formats, false);
					return;
				case dds::DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
					set_format_mask(glm::uvec4(0x00FF0000,
						0x0000FF00, 0x000000FF,
						0xFF000000), tft_srgb8,
						rg_formats, true);
					return;
				case dds::DXGI_FORMAT_B8G8R8X8_UNORM_SRGB:
					set_format_mask(glm::uvec4(0x00FF0000,
						0x0000FF00, 0x000000FF,
						0xFF000000), tft_srgb8_a8,
						rg_formats, true);
					return;
				case dds::DXGI_FORMAT_B4G4R4A4_UNORM:
					set_format_mask(glm::uvec4(0x00000F00,
						0x000000F0, 0x0000000F,
						0x0000F000), tft_rgba4,
						rg_formats, false);
					return;
			}

			EL_THROW_EXCEPTION(DdsUnkownFormatException()
				<< errinfo_item_id(format));
		}

		void DdsImageLoader::uncompress(const TextureFormatType format,
			const bool rg_formats, const bool sRGB,
			const bool merge_layers)
		{
			glm::uvec3 size;

			LOG_DEBUG(lt_dds_image, UTF8("Uncompressing DDS file "
				"'%1%'."), m_reader->get_name());

			if ((m_header.m_height % 4) != 0)
			{
				EL_THROW_EXCEPTION(InvalidParameterException()
					<< boost::errinfo_file_name(
						m_reader->get_name()));
			}

			if ((m_header.m_width % 4) != 0)
			{
				EL_THROW_EXCEPTION(InvalidParameterException()
					<< boost::errinfo_file_name(
						m_reader->get_name()));
			}

			m_header.m_pixel_format.m_flags = 0;

			if ((format == tft_rgb_dxt1) ||
				(format == tft_rgba_dxt1) ||
				(format == tft_rgba_dxt3) ||
				(format == tft_rgba_dxt5) ||
				(format == tft_srgb_dxt1) ||
				(format == tft_srgb_a_dxt1) ||
				(format == tft_srgb_a_dxt3) ||
				(format == tft_srgb_a_dxt5))
			{
				m_header.m_pixel_format.m_red_mask = 0xFF000000;
				m_header.m_pixel_format.m_green_mask = 0x00FF0000;
				m_header.m_pixel_format.m_blue_mask = 0x0000FF00;
				m_header.m_pixel_format.m_alpha_mask = 0x000000FF;
			}
			else
			{
				if (format == tft_rg_rgtc2)
				{
					m_header.m_pixel_format.m_red_mask = 0x000000FF00;
					m_header.m_pixel_format.m_green_mask = 0x000000FF;
					m_header.m_pixel_format.m_blue_mask = 0x00000000;
					m_header.m_pixel_format.m_alpha_mask = 0x00000000;
				}
				else
				{
					if (format == tft_r_rgtc1)
					{
						m_header.m_pixel_format.m_red_mask = 0x000000FF;
						m_header.m_pixel_format.m_green_mask = 0x00000000;
						m_header.m_pixel_format.m_blue_mask = 0x00000000;
						m_header.m_pixel_format.m_alpha_mask = 0x00000000;
					}
					else
					{
						EL_THROW_EXCEPTION(
							InvalidParameterException()
								<< boost::errinfo_file_name(
									m_reader->get_name()));
					}
				}
			}

			size[0] = m_header.m_width;
			size[1] = m_header.m_height;
			size[2] = m_header.m_depth;

			m_image = Dxt::uncompress(m_reader,
				m_reader->get_name(), size, format,
				static_cast<Uint16>(m_header.m_mipmap_count),
				get_cube_map(m_header), get_array(), rg_formats,
				sRGB, merge_layers);
		}

	}

	ImageSharedPtr DdsImage::load_image(const ReaderSharedPtr &reader,
		const ImageCompressionTypeSet &compression,
		const bool rg_formats, const bool sRGB, const bool merge_layers)
	{
		try
		{
			DdsImageLoader dds_image_loader(reader, compression,
				rg_formats, sRGB, merge_layers);

			return dds_image_loader.get_image();
		}
		catch (boost::exception &exception)
		{
			exception << boost::errinfo_file_name(
				reader->get_name());
			throw;
		}
	}

	void DdsImage::get_image_information(const ReaderSharedPtr &reader,
		const bool rg_formats, const bool sRGB,
		TextureFormatType &texture_format, glm::uvec3 &size,
		Uint16 &mipmaps, bool &cube_map, bool &array)
	{
		dds::DdsHeader header;
		dds::DdsHeader10 header_dxt10;

		try
		{
			init_dds_image(reader, header, header_dxt10);

			texture_format = get_texture_format(header,
				rg_formats, sRGB);

			size[0] = header.m_width;
			size[1] = header.m_height;
			size[2] = header.m_depth;

			mipmaps = header.m_mipmap_count;

			cube_map = get_cube_map(header);

			if (header_dxt10.m_array_size > 0)
			{
				array = true;
				size[2] = header_dxt10.m_array_size;
			}
			else
			{
				array = false;
			}
		}
		catch (boost::exception &exception)
		{
			exception << boost::errinfo_file_name(
				reader->get_name());
			throw;
		}
	}

	bool DdsImage::check_load(const Uint8Array32 &id)
	{
		return (id[0] == 'D') && (id[1] == 'D') && (id[2] == 'S') &&
			(id[3] == ' ');
	}

	String DdsImage::get_image_str()
	{
		return String(UTF8("dds-image"));
	}

	void DdsImage::check_all_fourcc_support(const bool rg_formats)
	{
		check_fourcc_support(dds::DDSFMT_R8G8B8,
			String(UTF8("R8G8B8")), 24, rg_formats);
		check_fourcc_support(dds::DDSFMT_A8R8G8B8,
			String(UTF8("A8R8G8B8")), 32, rg_formats);
		check_fourcc_support(dds::DDSFMT_R5G6B5,
			String(UTF8("R5G6B5")), 16, rg_formats);
		check_fourcc_support(dds::DDSFMT_X1R5G5B5,
			String(UTF8("X1R5G5B5")), 16, rg_formats);
		check_fourcc_support(dds::DDSFMT_A1R5G5B5,
			String(UTF8("A1R5G5B5")), 16, rg_formats);
		check_fourcc_support(dds::DDSFMT_A4R4G4B4,
			String(UTF8("A4R4G4B4")), 16, rg_formats);
		check_fourcc_support(dds::DDSFMT_R3G3B2,
			String(UTF8("R3G3B2")), 8, rg_formats);
		check_fourcc_support(dds::DDSFMT_A8,
			String(UTF8("A8")), 8, rg_formats);
		check_fourcc_support(dds::DDSFMT_X4R4G4B4,
			String(UTF8("X4R4G4B4")), 16, rg_formats);
		check_fourcc_support(dds::DDSFMT_A2B10G10R10,
			String(UTF8("A2B10G10R10")), 32, rg_formats);
		check_fourcc_support(dds::DDSFMT_A8B8G8R8,
			String(UTF8("A8B8G8R8")), 32, rg_formats);
		check_fourcc_support(dds::DDSFMT_G16R16,
			String(UTF8("G16R16")), 32, rg_formats);
		check_fourcc_support(dds::DDSFMT_A2R10G10B10,
			String(UTF8("A2R10G10B10")), 32, rg_formats);
		check_fourcc_support(dds::DDSFMT_A16B16G16R16,
			String(UTF8("A16B16G16R16")), 64, rg_formats);
		check_fourcc_support(dds::DDSFMT_A16B16G16R16_SIGNED,
			String(UTF8("A16B16G16R16_SIGNED")), 64, rg_formats);
		check_fourcc_support(dds::DDSFMT_L8,
			String(UTF8("L8")), 8, rg_formats);
		check_fourcc_support(dds::DDSFMT_A8L8,
			String(UTF8("A8L8")), 16, rg_formats);
		check_fourcc_support(dds::DDSFMT_L16,
			String(UTF8("L16")), 16, rg_formats);
		check_fourcc_support(dds::DDSFMT_DXT1,
			String(UTF8("DXT1")), 4, rg_formats);
		check_fourcc_support(dds::DDSFMT_DXT2,
			String(UTF8("DXT2")), 8, rg_formats);
		check_fourcc_support(dds::DDSFMT_DXT3,
			String(UTF8("DXT3")), 8, rg_formats);
		check_fourcc_support(dds::DDSFMT_DXT4,
			String(UTF8("DXT4")), 8, rg_formats);
		check_fourcc_support(dds::DDSFMT_DXT5,
			String(UTF8("DXT5")), 8, rg_formats);
		check_fourcc_support(dds::DDSFMT_ATI1,
			String(UTF8("ATI1")), 4, rg_formats);
		check_fourcc_support(dds::DDSFMT_ATI2,
			String(UTF8("ATI2")), 8, rg_formats);
		check_fourcc_support(dds::DDSFMT_R16F,
			String(UTF8("R16F")), 16, rg_formats);
		check_fourcc_support(dds::DDSFMT_G16R16F,
			String(UTF8("G16R16F")), 32, rg_formats);
		check_fourcc_support(dds::DDSFMT_A16B16G16R16F,
			String(UTF8("A16B16G16R16F")), 64, rg_formats);
		check_fourcc_support(dds::DDSFMT_R32F,
			String(UTF8("R32F")), 32, rg_formats);
		check_fourcc_support(dds::DDSFMT_G32R32F,
			String(UTF8("G32R32F")), 64, rg_formats);
		check_fourcc_support(dds::DDSFMT_A32B32G32R32F,
			String(UTF8("A32B32G32R32F")), 128, rg_formats);
	}

	void DdsImage::save_image(const ImageConstSharedPtr &image,
		const WriterSharedPtr &writer)
	{
		dds::DdsHeader header;
		glm::uvec4 masks;
		GLenum type, format;
		Uint32 size, swap_size, fourcc, width, height, depth, mipmaps;
		bool cube_map, integer_format;

		try
		{
			type = image->get_type();
			format = image->get_format();
			width = image->get_width();
			height = image->get_height();
			depth = image->get_depth();
			cube_map = image->get_cube_map();
			mipmaps = image->get_mipmap_count() + 1;

			if (dds::get_fourcc(type, format, size, swap_size,
				fourcc))
			{
				dds::build_dds_fourcc_header(width, height,
					depth, mipmaps, fourcc, size, cube_map,
					header);
			}
			else
			{
				if (CodecManager::has_color_bit_mask(type,
					format, masks, size, swap_size,
					integer_format))
				{
					if (integer_format)
					{
						EL_THROW_EXCEPTION(
							DdsFormatNotSupportedException(
							));
					}
					dds::build_dds_header(width, height,
						depth, mipmaps, masks.r,
						masks.g, masks.b, masks.a,
						cube_map, header);
				}
				else
				{
					EL_THROW_EXCEPTION(
						DdsFormatNotSupportedException(
						));
				}
			}

			writer->write_u32_le(dds::DDSMAGIC);
			writer->write_u32_le(header.m_size);
			writer->write_u32_le(header.m_flags);
			writer->write_u32_le(header.m_height);
			writer->write_u32_le(header.m_width);
			writer->write_u32_le(header.m_size_or_pitch);
			writer->write_u32_le(header.m_depth);
			writer->write_u32_le(header.m_mipmap_count);
			writer->write_u32_le(header.m_reserved1[0]);
			writer->write_u32_le(header.m_reserved1[1]);
			writer->write_u32_le(header.m_reserved1[2]);
			writer->write_u32_le(header.m_reserved1[3]);
			writer->write_u32_le(header.m_reserved1[4]);
			writer->write_u32_le(header.m_reserved1[5]);
			writer->write_u32_le(header.m_reserved1[6]);
			writer->write_u32_le(header.m_reserved1[7]);
			writer->write_u32_le(header.m_reserved1[8]);
			writer->write_u32_le(header.m_reserved1[9]);
			writer->write_u32_le(header.m_reserved1[10]);
			writer->write_u32_le(header.m_pixel_format.m_size);
			writer->write_u32_le(header.m_pixel_format.m_flags);
			writer->write_u32_le(header.m_pixel_format.m_fourcc);
			writer->write_u32_le(header.m_pixel_format.m_bit_count);
			writer->write_u32_le(header.m_pixel_format.m_red_mask);
			writer->write_u32_le(
				header.m_pixel_format.m_green_mask);
			writer->write_u32_le(
				header.m_pixel_format.m_blue_mask);
			writer->write_u32_le(
				header.m_pixel_format.m_alpha_mask);
			writer->write_u32_le(header.m_caps.m_caps1);
			writer->write_u32_le(header.m_caps.m_caps2);
			writer->write_u32_le(header.m_caps.m_caps3);
			writer->write_u32_le(header.m_caps.m_caps4);
			writer->write_u32_le(header.m_reserved2);

			writer->write_le(*image->get_buffer(), swap_size);
		}
		catch (boost::exception &exception)
		{
			exception << errinfo_name(image->get_name());
			throw;
		}
	}

	void DdsImage::save_image_dxt10(const ImageConstSharedPtr &image,
		const WriterSharedPtr &writer)
	{
		dds::DdsHeader header;
		dds::DdsHeader10 header_dxt10;
		glm::uvec4 position;
		TextureFormatType texture_format;
		Uint32 size, swap_size, format, layer_count, flags;
		Uint32 width, height, depth, mipmaps;
		bool cube_map, linear_size;

		try
		{
			texture_format = image->get_texture_format();
			width = image->get_width();
			height = image->get_height();
			depth = image->get_depth();
			layer_count = image->get_layer_count();
			mipmaps = image->get_mipmap_count() + 1;
			cube_map = image->get_cube_map();
			swap_size = 0;

			if (image->get_array())
			{
				depth = 0;
			}

			if (get_dxt10_format(texture_format, position, format,
				size, swap_size, flags, linear_size))
			{
				build_dxt10_dds_header(width, height, depth,
					layer_count, mipmaps, format, size,
					flags, cube_map, linear_size, header,
					header_dxt10);
			}
			else
			{
				EL_THROW_EXCEPTION(
					DdsFormatNotSupportedException());
			}

			writer->write_u32_le(dds::DDSMAGIC);
			writer->write_u32_le(header.m_size);
			writer->write_u32_le(header.m_flags);
			writer->write_u32_le(header.m_height);
			writer->write_u32_le(header.m_width);
			writer->write_u32_le(header.m_size_or_pitch);
			writer->write_u32_le(header.m_depth);
			writer->write_u32_le(header.m_mipmap_count);
			writer->write_u32_le(header.m_reserved1[0]);
			writer->write_u32_le(header.m_reserved1[1]);
			writer->write_u32_le(header.m_reserved1[2]);
			writer->write_u32_le(header.m_reserved1[3]);
			writer->write_u32_le(header.m_reserved1[4]);
			writer->write_u32_le(header.m_reserved1[5]);
			writer->write_u32_le(header.m_reserved1[6]);
			writer->write_u32_le(header.m_reserved1[7]);
			writer->write_u32_le(header.m_reserved1[8]);
			writer->write_u32_le(header.m_reserved1[9]);
			writer->write_u32_le(header.m_reserved1[10]);
			writer->write_u32_le(header.m_pixel_format.m_size);
			writer->write_u32_le(header.m_pixel_format.m_flags);
			writer->write_u32_le(header.m_pixel_format.m_fourcc);
			writer->write_u32_le(header.m_pixel_format.m_bit_count);
			writer->write_u32_le(header.m_pixel_format.m_red_mask);
			writer->write_u32_le(
				header.m_pixel_format.m_green_mask);
			writer->write_u32_le(
				header.m_pixel_format.m_blue_mask);
			writer->write_u32_le(
				header.m_pixel_format.m_alpha_mask);
			writer->write_u32_le(header.m_caps.m_caps1);
			writer->write_u32_le(header.m_caps.m_caps2);
			writer->write_u32_le(header.m_caps.m_caps3);
			writer->write_u32_le(header.m_caps.m_caps4);
			writer->write_u32_le(header.m_reserved2);
			writer->write_u32_le(header_dxt10.m_dxgi_format);
			writer->write_u32_le(header_dxt10.m_resource_dimension);
			writer->write_u32_le(header_dxt10.m_misc_flag);
			writer->write_u32_le(header_dxt10.m_array_size);
			writer->write_u32_le(header_dxt10.m_reserved);

			writer->write_le(*image->swizzle(position), swap_size);
		}
		catch (boost::exception &exception)
		{
			exception << errinfo_name(image->get_name());
			throw;
		}
	}

	bool DdsImage::can_save(const ImageConstSharedPtr &image)
	{
		glm::uvec4 masks;
		GLenum type, format;
		Uint32 swap_size, fourcc, size;
		bool integer_format;

		type = image->get_type();
		format = image->get_format();
		swap_size = 0;
		integer_format = false;

		return dds::get_fourcc(type, format, size, swap_size, fourcc) ||
			(CodecManager::has_color_bit_mask(type, format, masks,
				size, swap_size, integer_format) &&
					!integer_format);
	}

	bool DdsImage::can_save_dxt10(const ImageConstSharedPtr &image)
	{
		glm::uvec4 position;
		TextureFormatType texture_format;
		Uint32 size, swap_size, dxt10_format, flags;
		bool linear_size;

		swap_size = 0;

		texture_format = image->get_texture_format();

		return get_dxt10_format(texture_format, position, dxt10_format,
			size, swap_size, flags, linear_size);
	}

	void DdsImage::get_dxt10_formats(
		std::vector<TextureFormatType> &texture_formats)
	{
		texture_formats.push_back(tft_depth32f_stencil8);
		texture_formats.push_back(tft_rgba32f);
		texture_formats.push_back(tft_rgba32_ui);
		texture_formats.push_back(tft_rgba32_i);
		texture_formats.push_back(tft_rgb32f);
		texture_formats.push_back(tft_rgb32_ui);
		texture_formats.push_back(tft_rgb32_i);
		texture_formats.push_back(tft_rgba16f);
		texture_formats.push_back(tft_rgba16);
		texture_formats.push_back(tft_rgba16_ui);
		texture_formats.push_back(tft_rgba16_snorm);
		texture_formats.push_back(tft_rgba16_i);
		texture_formats.push_back(tft_rg32f);
		texture_formats.push_back(tft_rg32_ui);
		texture_formats.push_back(tft_rg32_i);
		texture_formats.push_back(tft_rgb10_a2);
		texture_formats.push_back(tft_rgb10_a2_ui);
		texture_formats.push_back(tft_r11f_b11f_g10f);
		texture_formats.push_back(tft_rgba8);
		texture_formats.push_back(tft_srgb8_a8);
		texture_formats.push_back(tft_rgba8_ui);
		texture_formats.push_back(tft_rgba8_snorm);
		texture_formats.push_back(tft_rgba8_i);
		texture_formats.push_back(tft_rg16f);
		texture_formats.push_back(tft_rg16);
		texture_formats.push_back(tft_rg16_ui);
		texture_formats.push_back(tft_rg16_snorm);
		texture_formats.push_back(tft_rg16_i);
		texture_formats.push_back(tft_depth32f);
		texture_formats.push_back(tft_r32f);
		texture_formats.push_back(tft_r32_ui);
		texture_formats.push_back(tft_r32_i);
		texture_formats.push_back(tft_depth24_stencil8);
		texture_formats.push_back(tft_rg8);
		texture_formats.push_back(tft_rg8_ui);
		texture_formats.push_back(tft_rg8_snorm);
		texture_formats.push_back(tft_rg8_i);
		texture_formats.push_back(tft_r16f);
		texture_formats.push_back(tft_depth16);
		texture_formats.push_back(tft_r16);
		texture_formats.push_back(tft_r16_ui);
		texture_formats.push_back(tft_r16_snorm);
		texture_formats.push_back(tft_r16_i);
		texture_formats.push_back(tft_r8);
		texture_formats.push_back(tft_r8_ui);
		texture_formats.push_back(tft_r8_snorm);
		texture_formats.push_back(tft_r8_i);
		texture_formats.push_back(tft_a8);
		texture_formats.push_back(tft_rgb9_e5);
		texture_formats.push_back(tft_rgb_dxt1);
		texture_formats.push_back(tft_rgba_dxt1);
		texture_formats.push_back(tft_srgb_dxt1);
		texture_formats.push_back(tft_srgb_a_dxt1);
		texture_formats.push_back(tft_rgba_dxt3);
		texture_formats.push_back(tft_srgb_a_dxt3);
		texture_formats.push_back(tft_rgba_dxt5);
		texture_formats.push_back(tft_srgb_a_dxt5);
		texture_formats.push_back(tft_r_rgtc1);
		texture_formats.push_back(tft_signed_r_rgtc1);
		texture_formats.push_back(tft_rg_rgtc2);
		texture_formats.push_back(tft_signed_rg_rgtc2);
		texture_formats.push_back(tft_rgb5_a1);
		texture_formats.push_back(tft_r5g6b5);
		texture_formats.push_back(tft_rgba4);
	}

}
