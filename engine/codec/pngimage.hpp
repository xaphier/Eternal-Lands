/****************************************************************************
 *            pngimage.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
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
				const bool rg_formats,
				TextureFormatType &texture_format,
				Uint32Array3 &sizes, Uint16 &mipmaps);
			static ImageSharedPtr load_image(
				const ReaderSharedPtr &reader,
				const bool rg_formats);
			static bool check_load(const Uint8Array32 &id);
			static String get_image_str();

	};

}

#endif	/* UUID_5065f478_d8e2_4f81_b495_ef442b38f63e */
