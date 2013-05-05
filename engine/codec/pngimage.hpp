/****************************************************************************
 *            pngimage.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_5065f478_d8e2_4f81_b495_ef442b38f63e
#define	UUID_5065f478_d8e2_4f81_b495_ef442b38f63e

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "textureformatutil.hpp"

namespace eternal_lands
{

	class PngImage
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
				const bool rg_formats, const bool sRGB);
			static bool check_load(const Uint8Array32 &id);
			static String get_image_str();
			static void save_image(const ImageConstSharedPtr &image,
				const WriterSharedPtr &writer);
			static bool can_save(const ImageConstSharedPtr &image);

	};

}

#endif	/* UUID_5065f478_d8e2_4f81_b495_ef442b38f63e */
