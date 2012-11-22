/****************************************************************************
 *            ddsimage.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
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
				const bool rg_formats, const bool sRGB,
				TextureFormatType &texture_format,
				glm::uvec3 &size, Uint16 &mipmaps,
				bool &cube_map, bool &array);
			static ImageSharedPtr load_image(
				const ReaderSharedPtr &reader,
				const ImageCompressionTypeSet &compression,
				const bool rg_formats, const bool sRGB,
				const bool merge_layers);
			static bool check_load(const Uint8Array32 &id);
			static String get_image_str();
			static void save_image(
				const ImageSharedPtr &image,
				const WriterSharedPtr &writer);
			static void save_image_dxt10(
				const ImageSharedPtr &image,
				const WriterSharedPtr &writer);
			static bool can_save(const ImageSharedPtr &image);
			static bool can_save_dxt10(const ImageSharedPtr &image);
			static void check_all_fourcc_support(
				const bool rg_formats);
			static void get_dxt10_formats(
				std::vector<TextureFormatType>
					&texture_formats);

	};

}

#endif	/* UUID_19416688_7d53_4bc6_a572_acee1862b9db */
