/****************************************************************************
 *            dxt.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_ea43f716_6c2b_4269_9570_a7f71b19caca
#define	UUID_ea43f716_6c2b_4269_9570_a7f71b19caca

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "textureformatutil.hpp"

/**
 * @file
 * @brief The @c class Dxt.
 * This file contains the @c class Dxt.
 */
namespace eternal_lands
{

	class Dxt
	{
		public:
			/**
			 * reader The reader used to read the compressed
			 * texture.
			 * @param name The name of the returned image.
			 * @param size The size of the file.
			 * @param texture_format Format for the file.
			 * @param mipmaps Number of mipmaps in the file.
			 * @param cube_map True if the file is a cube map, else
			 * false.
			 * @param rg_formats True if red or red-green format
			 * should be used for image.
			 * @param sRGB True if sRGB format should be used for
			 * image.
			 * @param merge_layers Merge multiple layers (two or
			 * four) of a red or red-green into in a rgba format.
			 */
			static ImageSharedPtr uncompress(
				const ReaderSharedPtr &reader,
				const String &name, const glm::uvec3 &size,
				const TextureFormatType texture_format,
				const Uint16 mipmaps, const bool cube_map,
				const bool rg_formats, const bool sRGB,
				const bool merge_layers);

	};

}

#endif	/* UUID_ea43f716_6c2b_4269_9570_a7f71b19caca */
