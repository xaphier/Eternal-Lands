/****************************************************************************
 *            shadersourceutil.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
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
		sst_world_depth_transform = 0,
		sst_world_normal_transform,
		sst_world_tangent_transform,
		sst_view_direction,
		sst_view_transform,
		sst_fog,
		sst_vertex_light,
		sst_fragment_light,
		sst_uv,
		sst_shadow_uv,
		sst_shadow_mapping,
		sst_normal_mapping,
		sst_normal_depth_mapping,
		sst_diffuse_mapping,
		sst_specular_mapping,
		sst_emission_mapping,
		sst_transparent,
		sst_shadow_map,
		sst_tbn_matrix
	};

	typedef std::map<ShaderSourceType, String> ShaderSourceTypeStringMap;

	class ShaderSourceUtil
	{
		public:
			static ShaderSourceType get_shader_source(
				const String &str);
			static const String &get_str(
				const ShaderSourceType source);
			static Uint32 get_shader_source_count();

	};

	OutStream& operator<<(OutStream &str, const ShaderSourceType value);
	InStream& operator>>(InStream &str, ShaderSourceType &value);

}

#endif	/* UUID_58d09f0e_2901_4094_a4f1_140dd1d5de2e */
