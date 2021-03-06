/****************************************************************************
 *            autoparameterutil.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
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
		 * 3x4 matrix
		 */
		apt_world_transformation = 0,
		/**
		 * 4x4 scene view matrix
		 */
		apt_view_matrix,
		/**
		 * 4x4 scene projection matrix
		 */
		apt_projection_matrix,
		/**
		 * 4x4 scene projection view matrix
		 */
		apt_projection_view_matrix,
		apt_reflection_matrix,
		/**
		 * 4x4 shadow texture matrices, number depends on the number
		 * of screen splits if pssm is used.
		 */
		apt_shadow_texture_matrices,
		/**
		 * vec4(position.x, position.y, position.z, 1.0 / radius^2)
		 * array of light data.
		 */
		apt_light_positions,
		/**
		 * vec4(red, green, blue, 1.0) array of light colors.
		 */
		apt_light_colors,
		/**
		 * mat2x4(ground_red, ground_green, ground_blue,
		 * 	ground_specular, sky_red - ground_red,
		 * 	sky_green - ground_green, sky_blue - ground_blue,
		 * 	sky_specular - ground_specular)
		 * The data alows with a simple map instruction to linear blend
		 * between sky and ground hemisphere. 
		 */
		apt_sky_ground_hemispheres,
		/**
		 * ivec3(vertex_lights_count, fragment_lights_count,
		 * 	lights_count)
		 */
		apt_dynamic_lights_count,
		apt_bones,
		apt_time,
		/**
		 * vec4(red, green, blue, density) fog
		 */
		apt_fog_data,
		/**
		 * vec4(position.x, position.y, position.z, 1.0) camera
		 * world space position.
		 */
		apt_camera,
		apt_current_shadow_distance_transform,
		apt_shadow_distance_transforms,
		apt_split_distances,
		apt_layers,
		apt_texture_matrices,
		apt_dudv_scale_offset,
		/**
		 * vec4 (red, green, blue, alpha) material color
		 */
		apt_material_color,
		apt_terrain_scale,
		apt_terrain_texture_size,
		apt_clipmap_terrain_matrices,
		/**
		 * vec4(z_near, z_far, z_near + z_far, z_far - z_near)
		 */
		apt_z_params,
		apt_terrain_lod_offset,
		/**
		 * terrain translation, vec4(x, y, z, 1.0f)
		 */
		apt_terrain_translation,
		/**
		 * screen size vec4(x, y, 1.0f/x, 1.0f/y);
		 */
		apt_screen_size
	};
	/**
	 * @}
	 */

	VECTOR(AutoParameterType);

	class AutoParameterUtil
	{
		public:
			static const String &get_str(
				const AutoParameterType auto_parameter);
			static ParameterType get_type(
				const AutoParameterType auto_parameter);
			static ParameterSizeType get_size(
				const AutoParameterType auto_parameter);
			static Uint16 get_scale(
				const AutoParameterType auto_parameter);
			static Uint32 get_max_size(
				const AutoParameterType auto_parameter);
			static AutoParameterType get_auto_parameter(
				const String &str);
			static bool get_auto_parameter(const String &str,
				AutoParameterType &auto_parameter) noexcept;
			static Uint32 get_auto_parameter_count() noexcept;

	};

	OutStream &operator<<(OutStream &str, const AutoParameterType value);
	InStream &operator>>(InStream &str, AutoParameterType &value);

}

#endif	/* UUID_557c772f_bf4b_4997_a198_a4f90f53703b */
