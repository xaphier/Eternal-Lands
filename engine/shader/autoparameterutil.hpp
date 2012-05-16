/****************************************************************************
 *            autoparameterutil.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
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
		/**
		 * 2x4 matrix with
		 * { { translate_x, translate_y, translate_z, scale},
		 * { rotation_quaternion_x, rotation_quaternion_y,
		 * rotation_quaternion_z, rotation_quaternion_w } }
		 */
		apt_world_transformation = 0,
		/**
		 * 3x3 matrix with only the rotation part of the view matrix
		 */
		apt_view_rotation_matrix,
		/**
		 * 4x4 scene view matrix
		 */
		apt_view_matrix,
		/**
		 * 4x4 scene projection matrix
		 */
		apt_projection_matrices,
		apt_projection_view_matrices,
		apt_reflection_matrix,
		apt_shadow_texture_matrices,
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
		apt_texture_matrices,
		apt_albedo_scale_offsets,
		apt_emission_scale_offset,
		apt_specular_scale_offset,
		apt_dudv_scale,
		apt_terrain_scale,
		apt_terrain_texture_size,
		apt_clipmap_matrices,
		/**
		 * vec4(z_near, z_far, z_near + z_far, z_far - z_near)
		 */
		apt_z_params,
		/**
		 * mat2x4(x_offset, y_offset, scale, scale * 2,
		 * 1.0 / (morph_end - morph_start),
		 * -morph_start / (morph_end - morph_start), lod_level, 0.0)
		 */
		apt_terrain_instances,
		apt_terrain_lod_offset
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
				AutoParameterType &auto_parameter) noexcept;
			static Uint32 get_auto_parameter_count() noexcept;

	};

	OutStream& operator<<(OutStream &str, const AutoParameterType value);
	InStream& operator>>(InStream &str, AutoParameterType &value);

}

#endif	/* UUID_557c772f_bf4b_4997_a198_a4f90f53703b */
