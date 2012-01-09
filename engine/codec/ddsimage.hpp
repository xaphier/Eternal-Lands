/****************************************************************************
 *            ddsimage.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_19416688_7d53_4bc6_a572_acee1862b9db
#define	UUID_19416688_7d53_4bc6_a572_acee1862b9db

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "textureformatutil.hpp"

/**
 * @file
 * @brief The @c class Dir.
 * This file contains the @c class Dir.
 */
namespace eternal_lands
{

	class DdsImage
	{
		public:
			static void get_image_information(
				const ReaderSharedPtr &reader,
				const bool rg_formats,
				TextureFormatType &texture_format,
				Uint32Array3 &sizes, Uint16 &mipmaps);
			static ImageSharedPtr load_image(
				const CodecManager &codec_manager,
				const ReaderSharedPtr &reader,
				const ImageCompressionTypeSet &compression,
				const bool rg_formats);
			static bool check_load(const Uint8Array32 &id);
			static String get_image_str();
			static void check_all_fourcc_support(
				const CodecManager &codec_manager,
				const bool rg_formats);

	};

}

#endif	/* UUID_19416688_7d53_4bc6_a572_acee1862b9db */
