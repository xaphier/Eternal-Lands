/****************************************************************************
 *            texturetargetutil.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_c6d9e817_5694_4627_9ba2_eb54a8834153
#define	UUID_c6d9e817_5694_4627_9ba2_eb54a8834153

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class TextureTargetUtil.
 * This file contains the @c class TextureTargetUtil.
 */
namespace eternal_lands
{

	/**
	 * Types of texture targets.
	 * @{
	 */
	enum TextureTargetType
	{
		ttt_texture_1d = 0,
		ttt_texture_2d,
		ttt_texture_3d,
		ttt_texture_cube_map,
		ttt_texture_1d_array,
		ttt_texture_2d_array,
		ttt_texture_cube_map_array,
		ttt_texture_rectangle,
		ttt_texture_2d_multisample,
		ttt_texture_2d_multisample_array
	};
	/**
	 * @}
	 */

	/**
	 * @brief @c class for textures.
	 *
	 * @c class for textures.
	 */
	class TextureTargetUtil
	{
		public:
			static const String &get_str(
				const TextureTargetType texture_target);
			static const String &get_description(
				const TextureTargetType texture_target);
			static TextureTargetType get_texture_target(
				const String &str);
			static GLenum get_gl_type(
				const TextureTargetType texture_target);
			static Uint32 get_texture_target_count() noexcept;
			static TextureTargetType get_depth_texture_target(
				const TextureTargetType texture_target);

	};

	OutStream &operator<<(OutStream &str, const TextureTargetType value);
	InStream &operator>>(InStream &str, TextureTargetType &value);

}

#endif	/* UUID_c6d9e817_5694_4627_9ba2_eb54a8834153 */
