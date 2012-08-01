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
			static ImageSharedPtr uncompress(
				const ReaderSharedPtr &reader,
				const String &name, const glm::uvec3 &sizes,
				const TextureFormatType texture_format,
				const Uint16 mipmaps, const bool cube_map,
				const bool rg_formats);

	};

}

#endif	/* UUID_ea43f716_6c2b_4269_9570_a7f71b19caca */
