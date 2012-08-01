/****************************************************************************
 *            shadersourceutil.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_58d09f0e_2901_4094_a4f1_140dd1d5de2e
#define	UUID_58d09f0e_2901_4094_a4f1_140dd1d5de2e

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class ShaderSource.
 * This file contains the @c class ShaderSource.
 */
namespace eternal_lands
{

	enum ShaderSourceType
	{
		sst_world_depth_transformation = 0,
		sst_world_normal_transformation,
		sst_world_tangent_transformation,
		sst_view_direction,
		sst_view_transformation,
		sst_fog,
		sst_vertex_light,
		sst_fragment_light,
		sst_uv,
		sst_shadow_uv,
		sst_shadow_mapping,
		sst_shadow_map,
		sst_tbn_matrix,
		sst_shadow_uv_ddx_ddy,
		sst_terrain_uv_ddx_ddy,
		sst_world_uv_ddx_ddy,
		sst_world_extra_uv_ddx_ddy,
		sst_uv_mapping,
		sst_uv_depth_mapping,
		sst_normal_mapping,
		sst_albedo_mapping,
		sst_specular_mapping,
		sst_emission_mapping
	};

	typedef std::map<ShaderSourceType, String> ShaderSourceTypeStringMap;

	class ShaderSourceUtil
	{
		public:
			static ShaderSourceType get_shader_source(
				const String &str);
			static const String &get_str(
				const ShaderSourceType source);
			static Uint32 get_shader_source_count() noexcept;

	};

	OutStream& operator<<(OutStream &str, const ShaderSourceType value);
	InStream& operator>>(InStream &str, ShaderSourceType &value);

}

#endif	/* UUID_58d09f0e_2901_4094_a4f1_140dd1d5de2e */
