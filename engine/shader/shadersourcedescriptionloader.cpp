/****************************************************************************
 *            shadersourcedescriptionloader.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "shadersourcedescriptionloader.hpp"
#include "globalvars.hpp"
#include "lua.hpp"

namespace eternal_lands
{

	namespace
	{

		void copy_default(const ShaderSourceTypeStringMap &defaults,
			const ShaderSourceType shader_source_type,
			ShaderSourceTypeStringMap &sources)
		{
			ShaderSourceTypeStringMap::const_iterator found;

			found = defaults.find(shader_source_type);

			if (found != defaults.end())
			{
				sources[shader_source_type] = found->second;
			}
		}

	}

	ShaderSourceDescriptionLoader::ShaderSourceDescriptionLoader(
		const GlobalVarsSharedPtr &global_vars,
		const ShaderSourceTypeStringMap &defaults, const String &name,
		const String &quality, Lua &lua)
	{
		load(defaults, quality, name, lua);
	}

	ShaderSourceDescriptionLoader::~ShaderSourceDescriptionLoader() throw()
	{
	}

	void ShaderSourceDescriptionLoader::load_bool_or_string(
		const ShaderSourceTypeStringMap &defaults,
		const String &name, const ShaderSourceType shader_source_type,
		Lua &lua)
	{
		lua.push_string(name);
		lua.get_table(-2);

		if (lua.is_bool(-1))
		{
			if (lua.to_bool(-1))
			{
				copy_default(defaults, shader_source_type,
					m_sources);
			}
		}
		else
		{
			if (lua.is_string(-1))
			{
				m_sources[shader_source_type] =
					lua.to_string(-1);
			}
			else
			{
				copy_default(defaults, shader_source_type,
					m_sources);
			}
		}

		lua.pop(1);
	}

	void ShaderSourceDescriptionLoader::load_string(
		const ShaderSourceTypeStringMap &defaults,
		const String &name, const ShaderSourceType shader_source_type,
		Lua &lua)
	{
		lua.push_string(name);
		lua.get_table(-2);

		if (lua.is_string(-1))
		{
			m_sources[shader_source_type] = lua.to_string(-1);
		}
		else
		{
			copy_default(defaults, shader_source_type, m_sources);
		}

		lua.pop(1);
	}

	void ShaderSourceDescriptionLoader::load_bool(
		const ShaderSourceTypeStringMap &defaults,
		const String &name, const ShaderSourceType shader_source_type,
		Lua &lua)
	{
		lua.push_string(name);
		lua.get_table(-2);

		if (!lua.is_bool(-1) || lua.to_bool(-1))
		{
			copy_default(defaults, shader_source_type, m_sources);
		}

		lua.pop(1);
	}

	void ShaderSourceDescriptionLoader::load(
		const ShaderSourceTypeStringMap &defaults,
		const String &quality, const String &name, Lua &lua)
	{
		// Fetch the table from the Lua context and make sure it exists:
		lua.get_global(name);

		if (!lua.is_table(-1))
		{
			lua.pop(1);

			return;
		}

		lua.push_string(quality);
		lua.get_table(-2);

		if (!lua.is_table(-1))
		{
			lua.pop(2);

			return;
		}

		m_sources.clear();

		load_string(defaults, String(UTF8("world_transform")),
			sst_world_depth_transform, lua);
		load_string(defaults, String(UTF8("world_transform")),
			sst_world_normal_transform, lua);
		load_string(defaults, String(UTF8("world_transform")),
			sst_world_tangent_transform, lua);
		load_bool(defaults, String(UTF8("fog")), sst_fog, lua);
		load_bool_or_string(defaults, String(UTF8("light")), sst_light,
			lua);
		load_bool(defaults, String(UTF8("shadow")), sst_shadow_uv,
			lua);
		load_bool(defaults, String(UTF8("shadow")), sst_shadow_mapping,
			lua);
		load_string(defaults, String(UTF8("normal_mapping")),
			sst_normal_mapping, lua);
		load_string(defaults, String(UTF8("normal_depth_mapping")),
			sst_normal_depth_mapping, lua);
		load_string(defaults, String(UTF8("uv")), sst_uv, lua);
		load_string(defaults, String(UTF8("diffuse_mapping")),
			sst_diffuse_mapping, lua);
		load_string(defaults, String(UTF8("specular_mapping")),
			sst_specular_mapping, lua);
		load_bool(defaults, String(UTF8("transparent")),
			sst_transparent, lua);
		copy_default(defaults, sst_view_direction, m_sources);
		copy_default(defaults, sst_view_transform, m_sources);
		copy_default(defaults, sst_shadow_map, m_sources);

		lua.pop(2);
	}

	ShaderSourceDescription
		ShaderSourceDescriptionLoader::get_shader_source_description(
		const ShaderBuildType shader_build_type,
		const Uint16 light_count) const
	{
		return ShaderSourceDescription(get_sources(),
			shader_build_type, light_count);
	}

}
