/****************************************************************************
 *            shaderbuildutil.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_5e192113_1988_427a_9ec4_2cdf82549f35
#define	UUID_5e192113_1988_427a_9ec4_2cdf82549f35

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
	 * Basic types of the shader build.
	 * @{
	 */
	enum ShaderBuildType
	{
		sbt_default = 0,
		sbt_depth,
		sbt_shadow,
		sbt_screen_quad,
		sbt_debug_uv,
		sbt_debug_depth,
		sbt_debug_alpha,
		sbt_debug_albedo,
		sbt_debug_normal,
		sbt_debug_shadow,
		sbt_debug_specular,
		sbt_debug_emissive,
		sbt_debug_diffuse_light,
		sbt_debug_specular_light,
		sbt_debug_packed_light_index,
		sbt_debug_light_index
	};
	/**
	 * @}
	 */

	class ShaderBuildUtil
	{
		public:
			static const String &get_str(
				const ShaderBuildType shader_build);
			static ShaderBuildType get_shader_build(
				const String &str);
			static Uint32 get_shader_build_count() noexcept;

	};

	OutStream& operator<<(OutStream &str, const ShaderBuildType value);
	InStream& operator>>(InStream &str, ShaderBuildType &value);

}

#endif	/* UUID_5e192113_1988_427a_9ec4_2cdf82549f35 */
