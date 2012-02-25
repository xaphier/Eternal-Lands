/****************************************************************************
 *            dds.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_d0afb6f2_e1d0_479b_a4ea_9d25d9f64d48
#define	UUID_d0afb6f2_e1d0_479b_a4ea_9d25d9f64d48

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The dds structures.
 * This file contains the dds structures.
 */
namespace eternal_lands
{

	namespace dds
	{

#ifndef	MAKEFOURCC
	#define MAKEFOURCC(c0, c1, c2, c3) (c0 | (c1 << 8) | (c2 << 16) | (c3 << 24))
#endif	/* MAKEFOURCC */

		struct DdsPixelFormat
		{
			Uint32 m_size;
			Uint32 m_flags;
			Uint32 m_fourcc;
			Uint32 m_bit_count;
			Uint32 m_red_mask;
			Uint32 m_green_mask;
			Uint32 m_blue_mask;
			Uint32 m_alpha_mask;
		};

		struct DdsCaps
		{
			Uint32 m_caps1;
			Uint32 m_caps2;
			Uint32 m_caps3;
			Uint32 m_caps4;
		};

		struct DdsHeader
		{
			Uint32 m_size;
			Uint32 m_flags;
			Uint32 m_height;
			Uint32 m_width;
			Uint32 m_size_or_pitch;
			Uint32 m_depth;
			Uint32 m_mipmap_count;
			Uint32 m_reserved1[11];
			DdsPixelFormat m_pixel_format;
			DdsCaps m_caps;
			Uint32 m_reserved2;
		};

		const Uint32 DDS_MAGIC_NUMBER_SIZE	= sizeof(Uint32);
		const Uint32 DDS_PIXEL_FORMAT_SIZE	= 8 * sizeof(Uint32);
		const Uint32 DDS_CAPS_SIZE		= 4 * sizeof(Uint32);
		const Uint32 DDS_HEADER_SIZE		= 19 * sizeof(Uint32)
			+ DDS_PIXEL_FORMAT_SIZE + DDS_CAPS_SIZE;
		const Uint32 DDS_DATA_POSITION		= DDS_MAGIC_NUMBER_SIZE + DDS_HEADER_SIZE;

		/* Header caps. */
		const Uint32 DDSD_CAPS			= 0x00000001;
		const Uint32 DDSD_HEIGHT		= 0x00000002;
		const Uint32 DDSD_WIDTH			= 0x00000004;
		const Uint32 DDSD_PITCH			= 0x00000008;
		const Uint32 DDSD_PIXELFORMAT		= 0x00001000;
		const Uint32 DDSD_MIPMAPCOUNT		= 0x00020000;
		const Uint32 DDSD_LINEARSIZE		= 0x00080000;
		const Uint32 DDSD_DEPTH			= 0x00800000;
		const Uint32 DDSD_MIN_FLAGS		= DDSD_CAPS | DDSD_PIXELFORMAT |
			DDSD_WIDTH | DDSD_HEIGHT;

		/* Pixel format constants. */
		const Uint32 DDPF_ALPHAPIXELS		= 0x00000001;
		const Uint32 DDPF_ALPHA			= 0x00000002;
		const Uint32 DDPF_FOURCC		= 0x00000004;
		const Uint32 DDPF_RGB			= 0x00000040;
		const Uint32 DDPF_RGBA			= 0x00000041;
		const Uint32 DDPF_PALETTEINDEXED1	= 0x00000800;
		const Uint32 DDPF_PALETTEINDEXED2	= 0x00001000;
		const Uint32 DDPF_PALETTEINDEXED4	= 0x00000008;
		const Uint32 DDPF_PALETTEINDEXED8	= 0x00000020;
		const Uint32 DDPF_LUMINANCE		= 0x00020000;
		const Uint32 DDPF_ALPHAPREMULT		= 0x00008000;
		const Uint32 DDPF_NORMAL		= 0x80000000;

		const Uint32 DDSCAPS_COMPLEX		= 0x00000008;
		const Uint32 DDSCAPS_TEXTURE		= 0x00001000;
		const Uint32 DDSCAPS_MIPMAP		= 0x00400000;

		/* Cube map constants. */
		const Uint32 DDSCAPS2_CUBEMAP		= 0x00000200;
		const Uint32 DDSCAPS2_CUBEMAP_POSITIVEX	= 0x00000400;
		const Uint32 DDSCAPS2_CUBEMAP_NEGATIVEX	= 0x00000800;
		const Uint32 DDSCAPS2_CUBEMAP_POSITIVEY	= 0x00001000;
		const Uint32 DDSCAPS2_CUBEMAP_NEGATIVEY	= 0x00002000;
		const Uint32 DDSCAPS2_CUBEMAP_POSITIVEZ	= 0x00004000;
		const Uint32 DDSCAPS2_CUBEMAP_NEGATIVEZ	= 0x00008000;
		const Uint32 DDSCAPS2_CUBEMAP_ALL_FACES	= DDSCAPS2_CUBEMAP_POSITIVEX |
			DDSCAPS2_CUBEMAP_NEGATIVEX | DDSCAPS2_CUBEMAP_POSITIVEY |
			DDSCAPS2_CUBEMAP_NEGATIVEY | DDSCAPS2_CUBEMAP_POSITIVEZ |
			DDSCAPS2_CUBEMAP_NEGATIVEZ;

		const Uint32 DDSCAPS2_VOLUME		= 0x00200000;

		/* FourCC codes. */
		const Uint32 DDSFMT_R8G8B8		= 20;
		const Uint32 DDSFMT_A8R8G8B8		= 21;
		const Uint32 DDSFMT_R5G6B5		= 23;
		const Uint32 DDSFMT_X1R5G5B5		= 24;
		const Uint32 DDSFMT_A1R5G5B5		= 25;
		const Uint32 DDSFMT_A4R4G4B4		= 26;
		const Uint32 DDSFMT_R3G3B2		= 27;
		const Uint32 DDSFMT_A8			= 28;
		const Uint32 DDSFMT_X4R4G4B4		= 30;
		const Uint32 DDSFMT_A2B10G10R10		= 31;
		const Uint32 DDSFMT_A8B8G8R8		= 32;
		const Uint32 DDSFMT_G16R16		= 34;
		const Uint32 DDSFMT_A2R10G10B10		= 35;
		const Uint32 DDSFMT_A16B16G16R16	= 36;
		const Uint32 DDSFMT_L8			= 50;
		const Uint32 DDSFMT_L16			= 81;
		const Uint32 DDSFMT_A16B16G16R16_SIGNED	= 110;

		/* Special FourCC codes. */
		const Uint32 DDSFMT_DXT1		= MAKEFOURCC('D', 'X', 'T', '1');
		const Uint32 DDSFMT_DXT2		= MAKEFOURCC('D', 'X', 'T', '2');
		const Uint32 DDSFMT_DXT3		= MAKEFOURCC('D', 'X', 'T', '3');
		const Uint32 DDSFMT_DXT4		= MAKEFOURCC('D', 'X', 'T', '4');
		const Uint32 DDSFMT_DXT5		= MAKEFOURCC('D', 'X', 'T', '5');
		const Uint32 DDSFMT_ATI1		= MAKEFOURCC('A', 'T', 'I', '1');
		const Uint32 DDSFMT_ATI2		= MAKEFOURCC('A', 'T', 'I', '2');
		/* FourCC code for DirectX 10 dds file. */
		const Uint32 DDSFMT_DX10		= MAKEFOURCC('D', 'X', '1', '0');

		/* Floating point formats. */
		const Uint32 DDSFMT_R16F		= 111;
		const Uint32 DDSFMT_G16R16F		= 112;
		const Uint32 DDSFMT_A16B16G16R16F	= 113;
		const Uint32 DDSFMT_R32F		= 114;
		const Uint32 DDSFMT_G32R32F		= 115;
		const Uint32 DDSFMT_A32B32G32R32F	= 116;

		/* Magic number of dds files. */
		const Uint32 DDSMAGIC			= MAKEFOURCC('D', 'D', 'S', ' ');

		struct DXTColorBlock
		{
			Uint16Array2 m_colors;
			Uint8Array4 m_indices;
		};

		struct DXTExplicitAlphaBlock
		{
			Uint16Array4 m_alphas;
		};

		struct DXTInterpolatedAlphaBlock
		{
			Uint8Array2 m_alphas;
			Uint8Array6 m_indices;
		};

		void unpack_dxt1(const DXTColorBlock &block,
			Vec4Array16 &values);
		void unpack_dxt3(const DXTExplicitAlphaBlock &alpha_block,
			const DXTColorBlock &color_block, Vec4Array16 &values);
		void unpack_dxt5(const DXTInterpolatedAlphaBlock &alpha_block,
			const DXTColorBlock &color_block, Vec4Array16 &values);
		void unpack_ati1(const DXTInterpolatedAlphaBlock &block,
			Vec4Array16 &values);
		void unpack_ati2(const DXTInterpolatedAlphaBlock &first_block,
			const DXTInterpolatedAlphaBlock &second_block,
			Vec4Array16 &values);
		bool alpha_check_dxt1(const DXTColorBlock &block);

		void build_dds_header(const Uint32 width, const Uint32 height,
			const Uint32 depth, const Uint32 mipmaps,
			const Uint32 red_mask, const Uint32 green_mask,
			const Uint32 blue_mask, const Uint32 alpha_mask,
			DdsHeader &header);
		void build_dds_fourcc_header(const Uint32 width,
			const Uint32 height, const Uint32 depth,
			const Uint32 mipmaps, const Uint32 fourcc,
			const Uint32 bpp, DdsHeader &header);

	}

}

#endif	/* UUID_d0afb6f2_e1d0_479b_a4ea_9d25d9f64d48 */

