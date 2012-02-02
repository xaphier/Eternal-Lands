/****************************************************************************
 *            autoparameterutil.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_557c772f_bf4b_4997_a198_a4f90f53703b
#define	UUID_557c772f_bf4b_4997_a198_a4f90f53703b

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "parameterutil.hpp"
#include "parametersizeutil.hpp"

/**
 * @file
 * @brief The @c class AutoParameterUtil.
 * This file contains the @c class AutoParameterUtil.
 */
namespace eternal_lands
{

	/**
	 * Basic types of the shader texture format types.
	 * @{
	 */
	enum AutoParameterType
	{
		apt_world_transformation = 0,
		apt_view_matrix,
		apt_projection_matrix,
		apt_projection_view_matrix,	
		apt_reflection_matrix,
		apt_shadow_texture_matrix,
		apt_light_positions,
		apt_light_colors,
		/**
		 * vec4(red, green, blue, alpha)
		 */
		apt_ambient,
		/**
		 * ivec3(vertex_light_count, fragment_light_count, light_count)
		 */
		apt_dynamic_light_count,
		apt_bones,
		apt_time,
		/**
		 * vec4(red, green, blue, density)
		 */
		apt_fog_data,
		apt_camera,
		apt_shadow_camera,
		apt_split_distances,
		apt_layers,
		apt_texture_scale_offset
	};
	/**
	 * @}
	 */

	class AutoParameterUtil
	{
		public:
			static const String &get_str(
				const AutoParameterType auto_paramter);
			static ParameterType get_type(
				const AutoParameterType auto_parameter);
			static ParameterSizeType get_size(
				const AutoParameterType auto_parameter);
			static Uint16 get_scale(
				const AutoParameterType auto_parameter);
			static AutoParameterType get_auto_parameter(
				const String &str);
			static bool get_auto_parameter(const String &str,
				AutoParameterType &auto_parameter);
			static Uint32 get_auto_parameter_count();

	};

	OutStream& operator<<(OutStream &str, const AutoParameterType value);
	InStream& operator>>(InStream &str, AutoParameterType &value);

}

#endif	/* UUID_557c772f_bf4b_4997_a198_a4f90f53703b */
