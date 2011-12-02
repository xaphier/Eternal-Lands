/****************************************************************************
 *            jpegimage.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
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

	class JpegImage
	{
		public:
			static void get_image_information(
				const ReaderSharedPtr &reader,
				TextureFormatType &texture_format,
				Uint32Array3 &sizes, Uint16 &mipmaps);
			static ImageSharedPtr load_image(
				const ReaderSharedPtr &reader);
			static bool check_load(const Uint8Array32 &id);
			static String get_image_str();

	};

}

#endif	/* UUID_0948309a_96e1_444f_8a93_89d91e550513 */
