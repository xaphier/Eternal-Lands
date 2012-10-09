/****************************************************************************
 *            shaderblendutil.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_375ee338_2a58_4c92_9b0c_7dd7ca87aec4
#define	UUID_375ee338_2a58_4c92_9b0c_7dd7ca87aec4

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class ShaderBuildUtil.
 * This file contains the @c class ShaderBuildUtil.
 */
namespace eternal_lands
{

	/**
	 * Basic types of the shader blend.
	 * @{
	 */
	enum ShaderBlendType
	{
		sbt_blend = 0,
		sbt_slope,
		sbt_height,
		sbt_texture,
		sbt_smoothstep_slope,
		sbt_smoothstep_height,
		sbt_smoothstep_texture
	};
	/**
	 * @}
	 */

	class ShaderBlendUtil
	{
		public:
			static const String &get_str(
				const ShaderBlendType shader_blend);
			static ShaderBlendType get_shader_blend(
				const String &str);
			static Uint32 get_shader_blend_count() noexcept;

	};

	OutStream &operator<<(OutStream &str, const ShaderBlendType value);
	InStream &operator>>(InStream &str, ShaderBlendType &value);

}

#endif	/* UUID_375ee338_2a58_4c92_9b0c_7dd7ca87aec4 */
