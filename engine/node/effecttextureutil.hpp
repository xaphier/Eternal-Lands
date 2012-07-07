/****************************************************************************
 *            effecttextureutil.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_430b0ad7_32dc_4e28_b46b_e0239232142e
#define	UUID_430b0ad7_32dc_4e28_b46b_e0239232142e

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class EffectTextureUtil.
 * This file contains the @c class EffectTextureUtil.
 */
namespace eternal_lands
{

	/**
	 * Basic types of the effect textures.
	 * @{
	 */
	enum EffectTextureType
	{
		ett_texture_1d = 0,
		ett_texture_2d,
		ett_texture_3d,
		ett_texture_cube_map,
		ett_texture_1d_array,
		ett_texture_2d_array,
		ett_texture_cube_map_array,
		ett_texture_rectangle,
		ett_texture_1d_project,
		ett_texture_2d_project,
		ett_texture_3d_project,
		ett_texture_rectangle_project
	};
	/**
	 * @}
	 */

	class EffectTextureUtil
	{
		public:
			static const String &get_str(
				const EffectTextureType effect_texture);
			static EffectTextureType get_effect_texture(
				const String &str);
			static bool get_effect_texture(const String &str,
				EffectTextureType &effect_texture) noexcept;
			static Uint32 get_effect_texture_count() noexcept;

	};

	OutStream& operator<<(OutStream &str, const EffectTextureType value);
	InStream& operator>>(InStream &str, EffectTextureType &value);

}

#endif	/* UUID_430b0ad7_32dc_4e28_b46b_e0239232142e */
