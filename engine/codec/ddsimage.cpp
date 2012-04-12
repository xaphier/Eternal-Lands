/****************************************************************************
 *            ddsimage.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "ddsimage.hpp"
#include "../exceptions.hpp"
#include "../logging.hpp"
#include "../reader.hpp"
#include "../image.hpp"
#include "codecmanager.hpp"
#include "dds.hpp"

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

		void validate_header(dds::DdsHeader &header)
		{
			Uint32 bit_count;

			if (header.m_size != dds::DDS_HEADER_SIZE)
			{
				EL_THROW_EXCEPTION(DdsErrorException());
			}

			if (header.m_pixel_format.m_size !=
				dds::DDS_PIXEL_FORMAT_SIZE)
			{
				EL_THROW_EXCEPTION(DdsErrorException());
			}

			if ((header.m_flags & dds::DDSD_MIN_FLAGS) !=
				dds::DDSD_MIN_FLAGS)
			{
				EL_THROW_EXCEPTION(DdsErrorException());
			}

			if ((header.m_caps.m_caps1 & dds::DDSCAPS_TEXTURE) !=
				dds::DDSCAPS_TEXTURE)
			{
				EL_THROW_EXCEPTION(DdsErrorException());
			}

			if (((header.m_caps.m_caps2 & dds::DDSCAPS2_CUBEMAP) ==
				dds::DDSCAPS2_CUBEMAP) &&
				((header.m_caps.m_caps2 &
				dds::DDSCAPS2_CUBEMAP_ALL_FACES) == 0))
			{
				EL_THROW_EXCEPTION(DdsErrorException());
			}

			if (((header.m_caps.m_caps2 & dds::DDSCAPS2_CUBEMAP) ==
				dds::DDSCAPS2_CUBEMAP) &&
				((header.m_caps.m_caps2 & dds::DDSCAPS2_VOLUME)
				== dds::DDSCAPS2_VOLUME))
			{
				EL_THROW_EXCEPTION(DdsErrorException());
			}

			if (((header.m_flags & dds::DDSD_DEPTH) ==
				dds::DDSD_DEPTH) &&
				((header.m_caps.m_caps2 & dds::DDSCAPS2_VOLUME)
				!= dds::DDSCAPS2_VOLUME))
			{
				EL_THROW_EXCEPTION(DdsErrorException());
			}

			if (((header.m_caps.m_caps2 & dds::DDSCAPS2_CUBEMAP) ==
				dds::DDSCAPS2_CUBEMAP) &&
				((header.m_caps.m_caps1 & dds::DDSCAPS_COMPLEX)
				!= dds::DDSCAPS_COMPLEX))
			{
				LOG_WARNING(UTF8("%s%"), UTF8("File is invalid."
					" DDSCAPS_COMPLEX cap should be set for"
					" a valid cube map DDS file."));
			}

			if (((header.m_caps.m_caps2 & dds::DDSCAPS2_VOLUME) ==
				dds::DDSCAPS2_VOLUME) &&
				((header.m_caps.m_caps1 & dds::DDSCAPS_COMPLEX)
				!= dds::DDSCAPS_COMPLEX))
			{
				LOG_WARNING(UTF8("%s%"), UTF8("File is invalid."
					" DDSCAPS_COMPLEX cap should be set"
					" for a valid volume DDS file."));
			}

			if (((header.m_caps.m_caps1 & dds::DDSCAPS_MIPMAP) ==
				dds::DDSCAPS_MIPMAP) &&
				((header.m_caps.m_caps1 & dds::DDSCAPS_COMPLEX)
				!= dds::DDSCAPS_COMPLEX))
			{
				LOG_WARNING(UTF8("%s%"), UTF8("File is invalid."
					" DDSCAPS_COMPLEX cap should be set"
					" for a valid DDS file with mipmaps."));
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
				LOG_WARNING(UTF8("%s%"), UTF8("File is invalid."
					" DDSCAPS_COMPLEX cap should be set "
					"only if the DDS file is a cube map, a"
					" volume and/or has mipmaps."));
			}

			if (((header.m_pixel_format.m_flags & dds::DDPF_FOURCC)
				== dds::DDPF_FOURCC) &&
				((header.m_pixel_format.m_flags & dds::DDPF_RGB)
				== dds::DDPF_RGB))
			{
				EL_THROW_EXCEPTION(DdsErrorException());
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

			if ((header.m_flags & dds::DDSD_DEPTH) !=
				dds::DDSD_DEPTH)
			{
				header.m_depth = 1;
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

		void read_colors_block(const ReaderSharedPtr &reader,
			dds::DXTColorBlock &colors)
		{
			BOOST_FOREACH(Uint16 &value, colors.m_colors)
			{
				value = reader->read_u16_le();
			}

			BOOST_FOREACH(Uint8 &value, colors.m_indices)
			{
				value = reader->read_u8();
			}
		}

		void read_alphas_block(const ReaderSharedPtr &reader,
			dds::DXTInterpolatedAlphaBlock &alphas)
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
			dds::DXTExplicitAlphaBlock &alphas)
		{
			BOOST_FOREACH(Uint16 &value, alphas.m_alphas)
			{
				value = reader->read_u16_le();
			}
		}

		void read_and_uncompress_dxt1_block(
			const ReaderSharedPtr &reader, Vec4Array16 &values)
		{
			dds::DXTColorBlock colors;

			read_colors_block(reader, colors);

			dds::unpack_dxt1(colors, values);
		}

		void read_and_uncompress_dxt3_block(
			const ReaderSharedPtr &reader, Vec4Array16 &values)
		{
			dds::DXTExplicitAlphaBlock alphas;
			dds::DXTColorBlock colors;

			read_alphas_block(reader, alphas);
			read_colors_block(reader, colors);

			dds::unpack_dxt3(alphas, colors, values);
		}

		void read_and_uncompress_dxt5_block(
			const ReaderSharedPtr &reader, Vec4Array16 &values)
		{
			dds::DXTInterpolatedAlphaBlock alphas;
			dds::DXTColorBlock colors;

			read_alphas_block(reader, alphas);
			read_colors_block(reader, colors);

			dds::unpack_dxt5(alphas, colors, values);
		}

		void read_and_uncompress_ati1_block(
			const ReaderSharedPtr &reader, Vec4Array16 &values)
		{
			dds::DXTInterpolatedAlphaBlock alphas;

			read_alphas_block(reader, alphas);

			dds::unpack_ati1(alphas, values);
		}

		void read_and_uncompress_ati2_block(
			const ReaderSharedPtr &reader, Vec4Array16 &values)
		{
			dds::DXTInterpolatedAlphaBlock first_block;
			dds::DXTInterpolatedAlphaBlock second_block;

			read_alphas_block(reader, first_block);
			read_alphas_block(reader, second_block);

			dds::unpack_ati2(first_block, second_block, values);
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

		void init_dds_image(const ReaderSharedPtr &reader,
			dds::DdsHeader &header)
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
			header.m_pixel_format.m_bit_count = reader->read_u32_le();
			header.m_pixel_format.m_red_mask = reader->read_u32_le();
			header.m_pixel_format.m_green_mask = reader->read_u32_le();
			header.m_pixel_format.m_blue_mask = reader->read_u32_le();
			header.m_pixel_format.m_alpha_mask = reader->read_u32_le();

			header.m_caps.m_caps1 = reader->read_u32_le();
			header.m_caps.m_caps2 = reader->read_u32_le();
			header.m_caps.m_caps3 = reader->read_u32_le();
			header.m_caps.m_caps4 = reader->read_u32_le();

			header.m_reserved2 = reader->read_u32_le();

			validate_header(header);
		}

		bool get_cube_map(const dds::DdsHeader &header)
		{
			return ((header.m_caps.m_caps2 & dds::DDSCAPS2_CUBEMAP)
				== dds::DDSCAPS2_CUBEMAP);
		}

		TextureFormatType get_texture_format(
			const dds::DdsHeader &header, const bool rg_formats)
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
					return tft_rgb8;
				}

				if ((red <= 8) && (green <= 8) && (blue <= 8) &&
					(alpha <= 8))
				{
					return tft_rgba8;
				}

				if ((red <= 10) && (green <= 10) && (blue <= 10)
					&& (alpha <= 2))
				{
					return tft_rgb10_a2;
				}

				return tft_rgba8;
			}
			else
			{
				switch (header.m_pixel_format.m_fourcc)
				{
					case dds::DDSFMT_DXT1:
						return tft_rgba_dxt1;
					case dds::DDSFMT_DXT2:
					case dds::DDSFMT_DXT3:
						return tft_rgba_dxt3;
					case dds::DDSFMT_DXT4:
					case dds::DDSFMT_DXT5:
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
						return tft_rgba8;
					case dds::DDSFMT_G16R16:
						return tft_rg16;
					case dds::DDSFMT_A2R10G10B10:
						return tft_rgb10_a2;
					case dds::DDSFMT_A16B16G16R16:
						return tft_rgba16;
					case dds::DDSFMT_L8:
						return tft_r8;
					case dds::DDSFMT_L16:
						return tft_r16;
				}

				EL_THROW_EXCEPTION(DdsUnkownFormatException());
			}
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
				case dds::DDSFMT_A8B8G8R8:
					return true;
			}

			EL_THROW_EXCEPTION(DdsUnkownFormatException());
		}

		void get_rgba_masks(const Uint32 fourcc, Uint32 &red_mask,
			Uint32 &green_mask, Uint32 &blue_mask,
			Uint32 &alpha_mask)
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
					red_mask = 0x00FF0000;
					green_mask = 0x0000FF00;
					blue_mask = 0x000000FF;
					alpha_mask = 0x00000000;
					return;
				case dds::DDSFMT_A8R8G8B8:
					red_mask = 0x00FF0000;
					green_mask = 0x0000FF00;
					blue_mask = 0x000000FF;
					alpha_mask = 0xFF000000;
					return;
				case dds::DDSFMT_R5G6B5:
					red_mask = 0x0000F800;
					green_mask = 0x000007E0;
					blue_mask = 0x0000001F;
					alpha_mask = 0x00000000;
					return;
				case dds::DDSFMT_X1R5G5B5:
					red_mask = 0x00007C00;
					green_mask = 0x000003E0;
					blue_mask = 0x0000001F;
					alpha_mask = 0x00008000;
					return;
				case dds::DDSFMT_A1R5G5B5:
					red_mask = 0x00007C00;
					green_mask = 0x000003E0;
					blue_mask = 0x0000001F;
					alpha_mask = 0x00008000;
					return;
				case dds::DDSFMT_A4R4G4B4:
					red_mask = 0x00000F00;
					green_mask = 0x000000F0;
					blue_mask = 0x0000000F;
					alpha_mask = 0x0000F000;
					return;
				case dds::DDSFMT_X4R4G4B4:
					red_mask = 0x00000F00;
					green_mask = 0x000000F0;
					blue_mask = 0x0000000F;
					alpha_mask = 0x0000F000;
					return;
				case dds::DDSFMT_R3G3B2:
					red_mask = 0x000000E0;
					green_mask = 0x0000001C;
					blue_mask = 0x00000003;
					alpha_mask = 0x00000000;
					return;
				case dds::DDSFMT_A8:
					red_mask = 0x00000000;
					green_mask = 0x00000000;
					blue_mask = 0x00000000;
					alpha_mask = 0x000000FF;
					return;
				case dds::DDSFMT_A2R10G10B10:
					red_mask = 0x000003FF;
					green_mask = 0x000FFC00;
					blue_mask = 0x3FF00000;
					alpha_mask = 0xC0000000;
					return;
				case dds::DDSFMT_A2B10G10R10:
					red_mask = 0x3FF00000;
					green_mask = 0x000FFC00;
					blue_mask = 0x000003FF;
					alpha_mask = 0xC0000000;
					return;
				case dds::DDSFMT_L8:
					red_mask = 0x000000FF;
					green_mask = 0x00000000;
					blue_mask = 0x00000000;
					alpha_mask = 0x00000000;
					return;
				case dds::DDSFMT_A8B8G8R8:
					red_mask = 0x000000FF;
					green_mask = 0x0000FF00;
					blue_mask = 0x00FF0000;
					alpha_mask = 0xFF000000;
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
					format = GL_LUMINANCE;
					type = GL_FLOAT;
					return true;
				case dds::DDSFMT_G32R32F:
					pixel_size = 64;
					format = GL_LUMINANCE_ALPHA;
					type = GL_FLOAT;
					return true;
				case dds::DDSFMT_A32B32G32R32F:
					pixel_size = 128;
					format = GL_RGBA;
					type = GL_FLOAT;
					return true;
				case dds::DDSFMT_R16F:
					pixel_size = 16;
					format = GL_LUMINANCE;
					type = GL_HALF_FLOAT;
					return true;
				case dds::DDSFMT_G16R16F:
					pixel_size = 32;
					format = GL_LUMINANCE_ALPHA;
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
				case dds::DDSFMT_A8B8G8R8:
					return false;
				case dds::DDSFMT_L16:
					pixel_size = 16;
					format = GL_LUMINANCE;
					type = GL_UNSIGNED_SHORT;
					return true;
				case dds::DDSFMT_G16R16:
					pixel_size = 32;
					format = GL_LUMINANCE_ALPHA;
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

		void check_fourcc_support(const CodecManager &codec_manager,
			const Uint32 fourcc, String format_str,
			const Uint32 pixel_size, const bool rg_formats)
		{
			Uint32 red_mask, green_mask, blue_mask, alpha_mask;
			Uint32 bits;
			GLenum format, type;
			bool supported;

			if (get_rgba_masks_needed(fourcc))
			{
				get_rgba_masks(fourcc, red_mask, green_mask,
					blue_mask, alpha_mask);

				supported =
					codec_manager.is_fast_load_supported(
					red_mask, green_mask, blue_mask,
					alpha_mask, rg_formats);

				bits = __builtin_popcount(red_mask | blue_mask |
					green_mask | alpha_mask);
			}
			else
			{
				supported = get_format(fourcc, bits, format,
					type);
			}

			if (!supported)
			{
				EL_THROW_EXCEPTION(DdsFormatNotSupportedException());
			}

			if (bits != pixel_size)
			{
				EL_THROW_EXCEPTION(DdsUnkownFormatException());
			}

			LOG_DEBUG_VERBOSE(UTF8("Loading of DDS FourCC %1% is "
				"supported."), format_str);
		}

		Uint32 get_fourcc(const Image &image, Uint32 &swap_size)
		{
			switch (image.get_type())
			{
				case GL_UNSIGNED_SHORT:
					swap_size = 2;
					switch (image.get_format())
					{
						case GL_RED:
						case GL_LUMINANCE:
							return dds::DDSFMT_L16;
						case GL_RG:
						case GL_LUMINANCE_ALPHA:
							return dds::DDSFMT_G16R16;
						case GL_RGBA:
							return dds::DDSFMT_A16B16G16R16;
					}
					break;
				case GL_FLOAT:
					swap_size = 4;
					switch (image.get_format())
					{
						case GL_RED:
						case GL_LUMINANCE:
							return dds::DDSFMT_R32F;
						case GL_RG:
						case GL_LUMINANCE_ALPHA:
							return dds::DDSFMT_G32R32F;
						case GL_RGBA:
							return dds::DDSFMT_A32B32G32R32F;
					}
					break;
				case GL_HALF_FLOAT:
					swap_size = 2;
					switch (image.get_format())
					{
						case GL_RED:
						case GL_LUMINANCE:
							return dds::DDSFMT_R16F;
						case GL_RG:
						case GL_LUMINANCE_ALPHA:
							return dds::DDSFMT_G16R16F;
						case GL_RGBA:
							return dds::DDSFMT_A16B16G16R16F;
					}
					break;
			}

			EL_THROW_EXCEPTION(NotImplementedException());
		}

		class DdsImageLoader
		{
			private:
				dds::DdsHeader m_header;
				ImageSharedPtr m_image;
				ReaderSharedPtr m_reader;
				const CodecManager &m_codec_manager;

				void load_supported(
					const ImageCompressionTypeSet
					&compression, const bool rg_formats);
				void set_format(const TextureFormatType tft,
					const bool rg_formats);
				void uncompress_block(
					const TextureFormatType format,
					const Uint32 x, const Uint32 y,
					const Uint32 z, const Uint16 face,
					const Uint16 mipmap, const Uint32 width,
					const Uint32 height);
				void uncompress(const TextureFormatType format,
					const bool rg_formats);
				void load(const ImageCompressionTypeSet
					&compression, const bool rg_formats);
				void load(const Uint16 swap_size);
				void set_format_mask(const Uint32 red_mask,
					const Uint32 green_mask,
					const Uint32 blue_mask,
					const Uint32 alpha_mask,
					const bool rg_formats);
				void set_format(const Uint32 pixel_size,
					const GLenum format, const GLenum type,
					const Uint16 swap_size,
					const bool rg_formats);
				Uint16 get_faces() const;
				void read_uncompress_face(
					const TextureFormatType format,
					const Uint32 width, const Uint32 height,
					const Uint32 depth, const Uint16 face,
					const Uint16 mipmap);

				inline const CodecManager &get_codec_manager()
					const
				{
					return m_codec_manager;
				}

			public:
				DdsImageLoader(
					const CodecManager &codec_manager,
					const ReaderSharedPtr &reader,
					const ImageCompressionTypeSet
					&compression, const bool rg_formats);

				inline const ImageSharedPtr &get_image()
				{
					return m_image;
				}

		};

		DdsImageLoader::DdsImageLoader(
			const CodecManager &codec_manager,
			const ReaderSharedPtr &reader,
			const ImageCompressionTypeSet &compression,
			const bool rg_formats):
			m_reader(reader), m_codec_manager(codec_manager)
		{
			try
			{
				init_dds_image(m_reader, m_header);
				load(compression, rg_formats);
			}
			catch (boost::exception &exception)
			{
				exception << boost::errinfo_file_name(
					m_reader->get_name());
				throw;
			}
		}

		void DdsImageLoader::set_format_mask(const Uint32 red_mask,
			const Uint32 green_mask, const Uint32 blue_mask,
			const Uint32 alpha_mask, const bool rg_formats)
		{
			glm::uvec3 size;
			Uint32 pixel_size;
			Uint32 swap_size;
			GLenum format;
			GLenum type;

			size[0] = m_header.m_width;
			size[1] = m_header.m_height;
			size[2] = m_header.m_depth;

			if (get_codec_manager().is_fast_load_supported(red_mask,
				green_mask, blue_mask, alpha_mask, rg_formats,
				type, format, pixel_size, swap_size))
			{
				m_image = boost::make_shared<Image>(
					m_reader->get_name(),
					get_cube_map(m_header),
					get_texture_format(m_header,
						rg_formats), size,
					static_cast<Uint16>(
					m_header.m_mipmap_count),
					static_cast<Uint16>(pixel_size * 8),
					format, type, false);
				load(1);
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
			const bool rg_formats)
		{
			LOG_DEBUG(UTF8("Loading file '%1%'."),
				m_reader->get_name());

			if ((m_header.m_pixel_format.m_flags & dds::DDPF_FOURCC)
				!= dds::DDPF_FOURCC)
			{
				set_format_mask(
					m_header.m_pixel_format.m_red_mask,
					m_header.m_pixel_format.m_green_mask,
					m_header.m_pixel_format.m_blue_mask,
					m_header.m_pixel_format.m_alpha_mask,
					rg_formats);
			}
			else
			{
				load_supported(compression, rg_formats);
			}
		}

		void DdsImageLoader::set_format(const Uint32 pixel_size,
			const GLenum format, const GLenum type,
			const Uint16 swap_size, const bool rg_formats)
		{
			glm::uvec3 size;

			size[0] = m_header.m_width;
			size[1] = m_header.m_height;
			size[2] = m_header.m_depth;

			m_image = boost::make_shared<Image>(
				m_reader->get_name(), get_cube_map(m_header),
				get_texture_format(m_header, rg_formats), size,
				static_cast<Uint16>(m_header.m_mipmap_count),
				static_cast<Uint16>(pixel_size), format, type,
				false);
			load(swap_size);
		}

		void DdsImageLoader::set_format(const TextureFormatType tft,
			const bool rg_formats)
		{
			glm::uvec3 size;

			size[0] = m_header.m_width;
			size[1] = m_header.m_height;
			size[2] = m_header.m_depth;

			assert(TextureFormatUtil::get_compressed(tft));

			m_image = boost::make_shared<Image>(
				m_reader->get_name(), get_cube_map(m_header),
				get_texture_format(m_header, rg_formats), size,
				static_cast<Uint16>(m_header.m_mipmap_count));
			load(1);
		}

		void DdsImageLoader::load(const Uint16 swap_size)
		{
			m_reader->set_position(dds::DDS_DATA_POSITION);

			m_reader->read_le(m_image->get_buffer(), swap_size);
		}

		void DdsImageLoader::load_supported(
			const ImageCompressionTypeSet &compression,
			const bool rg_formats)
		{
			Uint32 fourcc, red_mask, green_mask, blue_mask;
			Uint32 alpha_mask;
			Uint32 pixel_size, format, type;

			fourcc = m_header.m_pixel_format.m_fourcc;

			switch (fourcc)
			{
				case dds::DDSFMT_DXT1:
					if (compression.count(ict_s3tc) == 0)
					{
						uncompress(tft_rgba_dxt1,
							rg_formats);
						return;
					}

					set_format(tft_rgba_dxt1, rg_formats);
					return;
				case dds::DDSFMT_DXT2:
				case dds::DDSFMT_DXT3:
					if (compression.count(ict_s3tc) == 0)
					{
						uncompress(tft_rgba_dxt3,
							rg_formats);
						return;
					}

					set_format(tft_rgba_dxt3, rg_formats);
					return;
				case dds::DDSFMT_DXT4:
				case dds::DDSFMT_DXT5:
					if (compression.count(ict_s3tc) == 0)
					{
						uncompress(tft_rgba_dxt5,
							rg_formats);
						return;
					}

					set_format(tft_rgba_dxt5, rg_formats);
					return;
				case dds::DDSFMT_ATI1:
					if (compression.count(ict_rgtc) == 0)
					{
						uncompress(tft_r_rgtc1,
							rg_formats);
						return;
					}

					set_format(tft_r_rgtc1, rg_formats);
					return;
				case dds::DDSFMT_ATI2:
					if (compression.count(ict_rgtc) == 0)
					{
						uncompress(tft_rg_rgtc2,
							rg_formats);
						return;
					}

					set_format(tft_rg_rgtc2, rg_formats);
					return;
				case dds::DDSFMT_R32F:
				case dds::DDSFMT_G32R32F:
				case dds::DDSFMT_A32B32G32R32F:
					get_format(fourcc, pixel_size, format,
						type);
					set_format(pixel_size, format, type, 4,
						rg_formats);
					return;
				case dds::DDSFMT_R16F:
				case dds::DDSFMT_G16R16F:
				case dds::DDSFMT_A16B16G16R16F:
					get_format(fourcc, pixel_size, format,
						type);
					set_format(pixel_size, format, type, 2,
						rg_formats);
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
					assert(get_rgba_masks_needed(fourcc));
					red_mask = 0x00000000;
					green_mask = 0x00000000;
					blue_mask = 0x00000000;
					alpha_mask = 0x00000000;
					get_rgba_masks(fourcc, red_mask,
						green_mask, blue_mask,
						alpha_mask);
					set_format_mask(red_mask, green_mask,
						blue_mask, alpha_mask,
						rg_formats);
					return;
				case dds::DDSFMT_L16:
				case dds::DDSFMT_G16R16:
				case dds::DDSFMT_A16B16G16R16:
					get_format(fourcc, pixel_size, format,
						type);
					set_format(pixel_size, format, type, 2,
						rg_formats);
					return;
				case dds::DDSFMT_A16B16G16R16_SIGNED:
					get_format(fourcc, pixel_size, format,
						type);
					set_format(pixel_size, format, type, 2,
						rg_formats);
					return;
			}
			EL_THROW_EXCEPTION(DdsUnkownFormatException());
		}

		void DdsImageLoader::uncompress(const TextureFormatType format,
			const bool rg_formats)
		{
			glm::uvec3 size;
			Uint32 width, height, depth, face_count, mipmap_count;
			Uint32 i, j;
			Uint32 dst_bpp;
			GLenum gl_format;
			GLenum gl_type;
			TextureFormatType uncompressed_format;

			LOG_DEBUG(UTF8("Uncompressing DDS file '%1%'."),
				m_reader->get_name());

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
				(format == tft_rgba_dxt5))
			{
				m_header.m_pixel_format.m_red_mask = 0xFF000000;
				m_header.m_pixel_format.m_green_mask = 0x00FF0000;
				m_header.m_pixel_format.m_blue_mask = 0x0000FF00;
				m_header.m_pixel_format.m_alpha_mask = 0x000000FF;

				dst_bpp = 4;

				gl_type = GL_UNSIGNED_BYTE;
				gl_format = GL_RGBA;
				uncompressed_format = tft_rgba8;
			}
			else
			{
				if (format == tft_rg_rgtc2)
				{
					m_header.m_pixel_format.m_red_mask = 0x000000FF00;
					m_header.m_pixel_format.m_green_mask = 0x000000FF;
					m_header.m_pixel_format.m_blue_mask = 0x00000000;
					m_header.m_pixel_format.m_alpha_mask = 0x00000000;

					dst_bpp = 2;
					gl_type = GL_UNSIGNED_BYTE;

					if (rg_formats)
					{
						gl_format = GL_RG;
						uncompressed_format = tft_rg8;
					}
					else
					{
						gl_format = GL_LUMINANCE_ALPHA;
						uncompressed_format = tft_la8;
					}
				}
				else
				{
					if (format == tft_r_rgtc1)
					{
						m_header.m_pixel_format.m_red_mask = 0x000000FF;
						m_header.m_pixel_format.m_green_mask = 0x00000000;
						m_header.m_pixel_format.m_blue_mask = 0x00000000;
						m_header.m_pixel_format.m_alpha_mask = 0x00000000;

						dst_bpp = 1;

						gl_type = GL_UNSIGNED_BYTE;

						if (rg_formats)
						{
							gl_format = GL_RED;
							uncompressed_format =
								tft_r8;
						}
						else
						{
							gl_format =
								GL_LUMINANCE;
							uncompressed_format =
								tft_l8;
						}
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

			m_image = boost::make_shared<Image>(
				m_reader->get_name(), get_cube_map(m_header),
				uncompressed_format, size,
				static_cast<Uint16>(m_header.m_mipmap_count),
				static_cast<Uint16>(dst_bpp * 8), gl_format,
				gl_type, false);

			face_count = m_image->get_face_count();
			mipmap_count = m_image->get_mipmap_count();

			for (i = 0; i < face_count; ++i)
			{
				width = m_image->get_width();
				height = m_image->get_height();
				depth = m_image->get_depth();

				for (j = 0; j <= mipmap_count; ++j)
				{
					read_uncompress_face(format,
						width, height, depth, i, j);

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
		}

		void DdsImageLoader::read_uncompress_face(
			const TextureFormatType format, const Uint32 width,
			const Uint32 height, const Uint32 depth,
			const Uint16 face, const Uint16 mipmap)
		{
			Uint32 x, y, z;

			// slices are done individually
			for (z = 0; z < depth; z++)
			{
				// 4x4 blocks in x/y
				for (y = 0; y < height; y += 4)
				{
					for (x = 0; x < width; x += 4)
					{
						uncompress_block(format, x, y,
							z, face, mipmap,
							width, height);
					}
				}
			}
		}

		void DdsImageLoader::uncompress_block(
			const TextureFormatType format,
			const Uint32 x, const Uint32 y, const Uint32 z,
			const Uint16 face, const Uint16 mipmap,
			const Uint32 width, const Uint32 height)
		{
			Vec4Array16 values;
			Uint32 bx, by, sx, sy;

			switch (format)
			{
				case tft_rgb_dxt1:
				case tft_rgba_dxt1:
					read_and_uncompress_dxt1_block(m_reader,
						values);
					break;
				case tft_rgba_dxt3:
					read_and_uncompress_dxt3_block(m_reader,
						values);
					break;
				case tft_rgba_dxt5:
					read_and_uncompress_dxt5_block(m_reader,
						values);
					break;
				case tft_r_rgtc1:
					read_and_uncompress_ati1_block(m_reader,
						values);
					break;
				case tft_rg_rgtc2:
					read_and_uncompress_ati2_block(m_reader,
						values);
					break;
				default:
					EL_THROW_EXCEPTION(
						InvalidParameterException()
						<< boost::errinfo_file_name(
							m_reader->get_name()));
			}

			sx = std::min(width - x, static_cast<Uint32>(4));
			sy = std::min(height - y, static_cast<Uint32>(4));

			// write 4x4 block to uncompressed version
			for (by = 0; by < sy; by++)
			{
				for (bx = 0; bx < sx; bx++)
				{
					m_image->set_pixel(x + bx, y + by, z,
						face, mipmap,
						values[by * 4 + bx]);
				}
			}
		}

	}

	ImageSharedPtr DdsImage::load_image(const CodecManager &codec_manager,
		const ReaderSharedPtr &reader,
		const ImageCompressionTypeSet &compression,
		const bool rg_formats)
	{
		try
		{
			DdsImageLoader dds_image_loader(codec_manager, reader,
				compression, rg_formats);

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
		const bool rg_formats, TextureFormatType &texture_format,
		glm::uvec3 &sizes, Uint16 &mipmaps)
	{
		dds::DdsHeader header;

		try
		{
			init_dds_image(reader, header);

			texture_format = get_texture_format(header,
				rg_formats);

			sizes[0] = header.m_width;
			sizes[1] = header.m_height;
			sizes[2] = header.m_depth;

			mipmaps = header.m_mipmap_count;
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

	void DdsImage::check_all_fourcc_support(
		const CodecManager &codec_manager, const bool rg_formats)
	{
		check_fourcc_support(codec_manager, dds::DDSFMT_R8G8B8,
			String(UTF8("R8G8B8")), 24, rg_formats);
		check_fourcc_support(codec_manager, dds::DDSFMT_A8R8G8B8,
			String(UTF8("A8R8G8B8")), 32, rg_formats);
		check_fourcc_support(codec_manager, dds::DDSFMT_R5G6B5,
			String(UTF8("R5G6B5")), 16, rg_formats);
		check_fourcc_support(codec_manager, dds::DDSFMT_X1R5G5B5,
			String(UTF8("X1R5G5B5")), 16, rg_formats);
		check_fourcc_support(codec_manager, dds::DDSFMT_A1R5G5B5,
			String(UTF8("A1R5G5B5")), 16, rg_formats);
		check_fourcc_support(codec_manager, dds::DDSFMT_A4R4G4B4,
			String(UTF8("A4R4G4B4")), 16, rg_formats);
		check_fourcc_support(codec_manager, dds::DDSFMT_R3G3B2,
			String(UTF8("R3G3B2")), 8, rg_formats);
		check_fourcc_support(codec_manager, dds::DDSFMT_A8,
			String(UTF8("A8")), 8, rg_formats);
		check_fourcc_support(codec_manager, dds::DDSFMT_X4R4G4B4,
			String(UTF8("X4R4G4B4")), 16, rg_formats);
		check_fourcc_support(codec_manager, dds::DDSFMT_A2B10G10R10,
			String(UTF8("A2B10G10R10")), 32, rg_formats);
		check_fourcc_support(codec_manager, dds::DDSFMT_A8B8G8R8,
			String(UTF8("A8B8G8R8")), 32, rg_formats);
		check_fourcc_support(codec_manager, dds::DDSFMT_G16R16,
			String(UTF8("G16R16")), 32, rg_formats);
		check_fourcc_support(codec_manager, dds::DDSFMT_A2R10G10B10,
			String(UTF8("A2R10G10B10")), 32, rg_formats);
		check_fourcc_support(codec_manager, dds::DDSFMT_A16B16G16R16,
			String(UTF8("A16B16G16R16")), 64, rg_formats);
		check_fourcc_support(codec_manager,
			dds::DDSFMT_A16B16G16R16_SIGNED,
			String(UTF8("A16B16G16R16_SIGNED")), 64, rg_formats);
		check_fourcc_support(codec_manager, dds::DDSFMT_L8,
			String(UTF8("L8")), 8, rg_formats);
		check_fourcc_support(codec_manager, dds::DDSFMT_L16,
			String(UTF8("L16")), 16, rg_formats);
		check_fourcc_support(codec_manager, dds::DDSFMT_DXT1,
			String(UTF8("DXT1")), 4, rg_formats);
		check_fourcc_support(codec_manager, dds::DDSFMT_DXT2,
			String(UTF8("DXT2")), 8, rg_formats);
		check_fourcc_support(codec_manager, dds::DDSFMT_DXT3,
			String(UTF8("DXT3")), 8, rg_formats);
		check_fourcc_support(codec_manager, dds::DDSFMT_DXT4,
			String(UTF8("DXT4")), 8, rg_formats);
		check_fourcc_support(codec_manager, dds::DDSFMT_DXT5,
			String(UTF8("DXT5")), 8, rg_formats);
		check_fourcc_support(codec_manager, dds::DDSFMT_ATI1,
			String(UTF8("ATI1")), 4, rg_formats);
		check_fourcc_support(codec_manager, dds::DDSFMT_ATI2,
			String(UTF8("ATI2")), 8, rg_formats);
		check_fourcc_support(codec_manager, dds::DDSFMT_R16F,
			String(UTF8("R16F")), 16, rg_formats);
		check_fourcc_support(codec_manager, dds::DDSFMT_G16R16F,
			String(UTF8("G16R16F")), 32, rg_formats);
		check_fourcc_support(codec_manager, dds::DDSFMT_A16B16G16R16F,
			String(UTF8("A16B16G16R16F")), 64, rg_formats);
		check_fourcc_support(codec_manager, dds::DDSFMT_R32F,
			String(UTF8("R32F")), 32, rg_formats);
		check_fourcc_support(codec_manager, dds::DDSFMT_G32R32F,
			String(UTF8("G32R32F")), 64, rg_formats);
		check_fourcc_support(codec_manager, dds::DDSFMT_A32B32G32R32F,
			String(UTF8("A32B32G32R32F")), 128, rg_formats);
	}

}
