/****************************************************************************
 *            j2kimage.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_0948309a_96e1_444f_8a93_89d91e550513
#define	UUID_0948309a_96e1_444f_8a93_89d91e550513

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "textureformatutil.hpp"

namespace eternal_lands
{

	class J2kImage
	{
		public:
			static void get_image_information(
				const ReaderSharedPtr &reader,
				const bool rg_formats,
				TextureFormatType &texture_format,
				glm::uvec3 &sizes, Uint16 &mipmaps);
			static ImageSharedPtr load_image(
				const ReaderSharedPtr &reader,
				const bool rg_formats);
			static bool check_load(const Uint8Array32 &id);
			static String get_image_str();
			static void save_image(const ImageSharedPtr &image,
				OutStream &saver);
			static bool can_save(const ImageSharedPtr &image);

	};

}

#endif	/* UUID_0948309a_96e1_444f_8a93_89d91e550513 */
