/****************************************************************************
 *            dds.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
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

		/* DDS file header for DX10. */
		struct DdsHeader10
		{
			Uint32 m_dxgi_format;
			Uint32 m_resource_dimension;
			Uint32 m_misc_flag;
			Uint32 m_array_size;
			Uint32 m_reserved;
		};

		const Uint32 DDS_MAGIC_NUMBER_SIZE	= sizeof(Uint32);
		const Uint32 DDS_PIXEL_FORMAT_SIZE	= 8 * sizeof(Uint32);
		const Uint32 DDS_CAPS_SIZE		= 4 * sizeof(Uint32);
		const Uint32 DDS_HEADER_SIZE		= 19 * sizeof(Uint32)
			+ DDS_PIXEL_FORMAT_SIZE + DDS_CAPS_SIZE;
		const Uint32 DDS_DATA_POSITION		= DDS_MAGIC_NUMBER_SIZE
			+ DDS_HEADER_SIZE;
		const Uint32 DDS_HEADER_SIZE_DX10	= 5 * sizeof(Uint32);

		/* Header caps. */
		const Uint32 DDSD_CAPS			= 0x00000001;
		const Uint32 DDSD_HEIGHT		= 0x00000002;
		const Uint32 DDSD_WIDTH			= 0x00000004;
		const Uint32 DDSD_PITCH			= 0x00000008;
		const Uint32 DDSD_PIXELFORMAT		= 0x00001000;
		const Uint32 DDSD_MIPMAPCOUNT		= 0x00020000;
		const Uint32 DDSD_LINEARSIZE		= 0x00080000;
		const Uint32 DDSD_DEPTH			= 0x00800000;
		const Uint32 DDSD_MIN_FLAGS		= DDSD_CAPS |
			DDSD_PIXELFORMAT | DDSD_WIDTH | DDSD_HEIGHT;

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
		const Uint32 DDSCAPS2_CUBEMAP_ALL_FACES	=
			DDSCAPS2_CUBEMAP_POSITIVEX | DDSCAPS2_CUBEMAP_NEGATIVEX
			| DDSCAPS2_CUBEMAP_POSITIVEY |
			DDSCAPS2_CUBEMAP_NEGATIVEY | DDSCAPS2_CUBEMAP_POSITIVEZ
			| DDSCAPS2_CUBEMAP_NEGATIVEZ;

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
		const Uint32 D3DFMT_A8R3G3B2		= 29;
		const Uint32 DDSFMT_X4R4G4B4		= 30;
		const Uint32 DDSFMT_A2B10G10R10		= 31;
		const Uint32 DDSFMT_A8B8G8R8		= 32;
		const Uint32 DDSFMT_X8B8G8R8		= 33;
		const Uint32 DDSFMT_G16R16		= 34;
		const Uint32 DDSFMT_A2R10G10B10		= 35;
		const Uint32 DDSFMT_A16B16G16R16	= 36;
		const Uint32 DDSFMT_A16B16G16R16_SIGNED	= 110;

		/* Luminance formats. */
		const Uint32 DDSFMT_L8			= 50;
		const Uint32 DDSFMT_A8L8		= 51;
		const Uint32 DDSFMT_A4L4		= 52;
		const Uint32 DDSFMT_L16			= 81;

		/* Floating point formats. */
		const Uint32 DDSFMT_R16F		= 111;
		const Uint32 DDSFMT_G16R16F		= 112;
		const Uint32 DDSFMT_A16B16G16R16F	= 113;
		const Uint32 DDSFMT_R32F		= 114;
		const Uint32 DDSFMT_G32R32F		= 115;
		const Uint32 DDSFMT_A32B32G32R32F	= 116;

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

		/* Magic number of dds files. */
		const Uint32 DDSMAGIC			= MAKEFOURCC('D', 'D', 'S', ' ');

		/* D3D1x resource dimensions. */
		enum DDS_DIMENSION // D3D10_RESOURCE_DIMENSION
		{
			DDS_DIMENSION_UNKNOWN = 0,
			DDS_DIMENSION_BUFFER = 1,
			DDS_DIMENSION_TEXTURE1D = 2,
			DDS_DIMENSION_TEXTURE2D = 3,
			DDS_DIMENSION_TEXTURE3D = 4
		};

		enum DDS_MISC_FLAG
		{
			DDS_MISC_TEXTURECUBE = 0x4
		};

		// DXGI formats.
		enum DXGI_FORMAT
		{
			DXGI_FORMAT_UNKNOWN = 0,

			DXGI_FORMAT_R32G32B32A32_TYPELESS = 1,
			DXGI_FORMAT_R32G32B32A32_FLOAT = 2,
			DXGI_FORMAT_R32G32B32A32_UINT = 3,
			DXGI_FORMAT_R32G32B32A32_SINT = 4,

			DXGI_FORMAT_R32G32B32_TYPELESS = 5,
			DXGI_FORMAT_R32G32B32_FLOAT = 6,
			DXGI_FORMAT_R32G32B32_UINT = 7,
			DXGI_FORMAT_R32G32B32_SINT = 8,

			DXGI_FORMAT_R16G16B16A16_TYPELESS = 9,
			DXGI_FORMAT_R16G16B16A16_FLOAT = 10,
			DXGI_FORMAT_R16G16B16A16_UNORM = 11,
			DXGI_FORMAT_R16G16B16A16_UINT = 12,
			DXGI_FORMAT_R16G16B16A16_SNORM = 13,
			DXGI_FORMAT_R16G16B16A16_SINT = 14,

			DXGI_FORMAT_R32G32_TYPELESS = 15,
			DXGI_FORMAT_R32G32_FLOAT = 16,
			DXGI_FORMAT_R32G32_UINT = 17,
			DXGI_FORMAT_R32G32_SINT = 18,

			DXGI_FORMAT_R32G8X24_TYPELESS = 19,
			DXGI_FORMAT_D32_FLOAT_S8X24_UINT = 20,
			DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS = 21,
			DXGI_FORMAT_X32_TYPELESS_G8X24_UINT = 22,

			DXGI_FORMAT_R10G10B10A2_TYPELESS = 23,
			DXGI_FORMAT_R10G10B10A2_UNORM = 24,
			DXGI_FORMAT_R10G10B10A2_UINT = 25,

			DXGI_FORMAT_R11G11B10_FLOAT = 26,

			DXGI_FORMAT_R8G8B8A8_TYPELESS = 27,
			DXGI_FORMAT_R8G8B8A8_UNORM = 28,
			DXGI_FORMAT_R8G8B8A8_UNORM_SRGB = 29,
			DXGI_FORMAT_R8G8B8A8_UINT = 30,
			DXGI_FORMAT_R8G8B8A8_SNORM = 31,
			DXGI_FORMAT_R8G8B8A8_SINT = 32,

			DXGI_FORMAT_R16G16_TYPELESS = 33,
			DXGI_FORMAT_R16G16_FLOAT = 34,
			DXGI_FORMAT_R16G16_UNORM = 35,
			DXGI_FORMAT_R16G16_UINT = 36,
			DXGI_FORMAT_R16G16_SNORM = 37,
			DXGI_FORMAT_R16G16_SINT = 38,

			DXGI_FORMAT_R32_TYPELESS = 39,
			DXGI_FORMAT_D32_FLOAT = 40,
			DXGI_FORMAT_R32_FLOAT = 41,
			DXGI_FORMAT_R32_UINT = 42,
			DXGI_FORMAT_R32_SINT = 43,

			DXGI_FORMAT_R24G8_TYPELESS = 44,
			DXGI_FORMAT_D24_UNORM_S8_UINT = 45,
			DXGI_FORMAT_R24_UNORM_X8_TYPELESS = 46,
			DXGI_FORMAT_X24_TYPELESS_G8_UINT = 47,

			DXGI_FORMAT_R8G8_TYPELESS = 48,
			DXGI_FORMAT_R8G8_UNORM = 49,
			DXGI_FORMAT_R8G8_UINT = 50,
			DXGI_FORMAT_R8G8_SNORM = 51,
			DXGI_FORMAT_R8G8_SINT = 52,

			DXGI_FORMAT_R16_TYPELESS = 53,
			DXGI_FORMAT_R16_FLOAT = 54,
			DXGI_FORMAT_D16_UNORM = 55,
			DXGI_FORMAT_R16_UNORM = 56,
			DXGI_FORMAT_R16_UINT = 57,
			DXGI_FORMAT_R16_SNORM = 58,
			DXGI_FORMAT_R16_SINT = 59,

			DXGI_FORMAT_R8_TYPELESS = 60,
			DXGI_FORMAT_R8_UNORM = 61,
			DXGI_FORMAT_R8_UINT = 62,
			DXGI_FORMAT_R8_SNORM = 63,
			DXGI_FORMAT_R8_SINT = 64,
			DXGI_FORMAT_A8_UNORM = 65,

			DXGI_FORMAT_R1_UNORM = 66,

			DXGI_FORMAT_R9G9B9E5_SHAREDEXP = 67,

			DXGI_FORMAT_R8G8_B8G8_UNORM = 68,
			DXGI_FORMAT_G8R8_G8B8_UNORM = 69,

			DXGI_FORMAT_BC1_TYPELESS = 70,
			DXGI_FORMAT_BC1_UNORM = 71,
			DXGI_FORMAT_BC1_UNORM_SRGB = 72,

			DXGI_FORMAT_BC2_TYPELESS = 73,
			DXGI_FORMAT_BC2_UNORM = 74,
			DXGI_FORMAT_BC2_UNORM_SRGB = 75,

			DXGI_FORMAT_BC3_TYPELESS = 76,
			DXGI_FORMAT_BC3_UNORM = 77,
			DXGI_FORMAT_BC3_UNORM_SRGB = 78,

			DXGI_FORMAT_BC4_TYPELESS = 79,
			DXGI_FORMAT_BC4_UNORM = 80,
			DXGI_FORMAT_BC4_SNORM = 81,

			DXGI_FORMAT_BC5_TYPELESS = 82,
			DXGI_FORMAT_BC5_UNORM = 83,
			DXGI_FORMAT_BC5_SNORM = 84,

			DXGI_FORMAT_B5G6R5_UNORM = 85,
			DXGI_FORMAT_B5G5R5A1_UNORM = 86,
			DXGI_FORMAT_B8G8R8A8_UNORM = 87,
			DXGI_FORMAT_B8G8R8X8_UNORM = 88,

			DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM = 89,
			DXGI_FORMAT_B8G8R8A8_TYPELESS = 90,
			DXGI_FORMAT_B8G8R8A8_UNORM_SRGB = 91,
			DXGI_FORMAT_B8G8R8X8_TYPELESS = 92,
			DXGI_FORMAT_B8G8R8X8_UNORM_SRGB = 93,

			DXGI_FORMAT_BC6H_TYPELESS = 94,
			DXGI_FORMAT_BC6H_UF16 = 95,
			DXGI_FORMAT_BC6H_SF16 = 96,

			DXGI_FORMAT_BC7_TYPELESS = 97,
			DXGI_FORMAT_BC7_UNORM = 98,
			DXGI_FORMAT_BC7_UNORM_SRGB = 99,
			DXGI_FORMAT_B4G4R4A4_UNORM = 115,
		};

		void build_dds_header(const Uint32 width, const Uint32 height,
			const Uint32 depth, const Uint32 mipmap_count,
			const Uint32 red_mask, const Uint32 green_mask,
			const Uint32 blue_mask, const Uint32 alpha_mask,
			const bool cube_map, DdsHeader &header);
		void build_dds_fourcc_header(const Uint32 width,
			const Uint32 height, const Uint32 depth,
			const Uint32 mipmap_count, const Uint32 fourcc,
			const Uint32 bpp, const bool cube_map,
			DdsHeader &header);
		void build_dxt10_dds_header(const Uint32 width,
			const Uint32 height, const Uint32 depth,
			const Uint32 layer_count, const Uint32 mipmap_count,
			const Uint32 format, const Uint32 bpp,
			const Uint32 flags, const bool cube_map,
			const bool linear_size, DdsHeader &header,
			DdsHeader10 &header_dxt10);
		bool get_fourcc(const GLenum type, const GLenum format,
			Uint32 &size, Uint32 &swap_size, Uint32 &fourcc);

	}

}

#endif	/* UUID_d0afb6f2_e1d0_479b_a4ea_9d25d9f64d48 */
