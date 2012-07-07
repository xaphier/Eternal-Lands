/****************************************************************************
 *            commonparameterutil.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_80cf44ef_4b58_4e07_a651_faefc3561bb0
#define	UUID_80cf44ef_4b58_4e07_a651_faefc3561bb0

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "parameterutil.hpp"
#include "parametersizeutil.hpp"

/**
 * @file
 * @brief The @c class CommonParameterUtil.
 * This file contains the @c class CommonParameterUtil.
 */
namespace eternal_lands
{

	/**
	 * Common types of shader parameter.
	 * @{
	 */
	enum CommonParameterType
	{
		cpt_diffuse_color = 0,
		cpt_specular_color,
		cpt_shadow,
		cpt_light_color,
		cpt_light_position,
		cpt_world_position,
		cpt_world_normal,
		cpt_world_tangent,
		cpt_world_view_direction,
		cpt_world_uv,
		cpt_view_position,
		cpt_albedo,
		cpt_specular,
		cpt_fragment_normal,
		cpt_fragment_uv,
		cpt_fragment_position,
		cpt_fog,
		cpt_shadow_uvs,
		cpt_shadow_map_data,
		cpt_tbn_matrix,
		cpt_emission,
		cpt_world_extra_uv,
		cpt_terrain_uvs,
		cpt_world_uv_ddx_ddy,
		cpt_world_extra_uv_ddx_ddy,
		cpt_shadow_uv_ddx_ddy,
		cpt_terrain_uv_ddx_ddy,
		cpt_vertex_color,
		cpt_fragment_color,
		cpt_output_data
	};
	/**
	 * @}
	 */

	class CommonParameterUtil
	{
		public:
			static const String &get_str(
				const CommonParameterType common_paramter);
			static ParameterType get_type(
				const CommonParameterType common_parameter);
			static ParameterSizeType get_size(
				const CommonParameterType common_parameter);
			static Uint16 get_scale(
				const CommonParameterType common_parameter);
			static CommonParameterType get_common_parameter(
				const String &str);
			static bool get_common_parameter(const String &str,
				CommonParameterType &common_parameter) noexcept;
			static Uint32 get_common_parameter_count() noexcept;

	};

	OutStream& operator<<(OutStream &str, const CommonParameterType value);
	InStream& operator>>(InStream &str, CommonParameterType &value);

}

#endif	/* UUID_80cf44ef_4b58_4e07_a651_faefc3561bb0 */
