/****************************************************************************
 *            effect.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "effect.hpp"
#include "glslprogram.hpp"
#include "exceptions.hpp"
#include "lua.hpp"
#include "shader/shadersourcebuilder.hpp"
#include "shader/shadersourcedescriptionloader.hpp"
#include "logging.hpp"
#include "shader/shadertextureutil.hpp"
#include "filesystem.hpp"

namespace eternal_lands
{

	namespace
	{

		const StringType vertex_shader = UTF8(""
			"#version 120\n"
			"\n"
			"varying vec2 uv;\n"
			"\n"
			"uniform mat4x3 world_matrix;\n"
			"uniform mat4 projection_view_matrix;\n"
			"\n"
			"attribute vec2 texture_coordinate_0;\n"
			"attribute vec4 position;\n"
			"\n"
			"void main ()\n"
			"{\n"
			"\tvec3 world_position;\n"
			"\n"
			"\tworld_position = world_matrix * "
				"vec4(position.xyz, 1.0);\n"
			"\tgl_Position = projection_view_matrix * "
				"vec4(world_position, 1.0);\n"
			"\tuv = texture_coordinate_0;\n"
			"}\n");

		const StringType fragment_shader = UTF8(""
			"#version 120\n"
			"\n"
			"varying vec2 uv;\n"
			"\n"
			"uniform sampler2D diffuse_sampler_0;\n"
			"uniform vec4 ambient;\n"
			"uniform float alpha_value;\n"
			"\n"
			"void main ()\n"
			"{\n"
			"\tgl_FragColor.rgb = texture2D(diffuse_sampler_0, "
				"uv).rgb * ambient.rgb;\n"
			"\tgl_FragColor.a = alpha_value;\n"
			"}\n");

		const StringType depth_vertex_shader = UTF8(""
			"#version 120\n"
			"\n"
			"uniform mat4x3 world_matrix;\n"
			"uniform mat4 projection_view_matrix;\n"
			"\n"
			"attribute vec4 position;\n"
			"\n"
			"void main ()\n"
			"{\n"
			"\tvec3 world_position;\n"
			"\n"
			"\tworld_position = world_matrix * "
				"vec4(position.xyz, 1.0);\n"
			"\tgl_Position = projection_view_matrix * "
				"vec4(world_position, 1.0);\n"
			"}\n");

		const StringType depth_fragment_shader = UTF8(""
			"#version 120\n"
			"\n"
			"void main ()\n"
			"{\n"
			"\tgl_FragColor = gl_FragCoord.zzzz;\n"
			"}\n");

	}

	Effect::Effect(): m_name(UTF8("simple")), m_culling(true)
	{
		error_load();
	}

	Effect::Effect(const ShaderSourceBuilderWeakPtr &shader_source_builder,
		const FileSystemWeakPtr &file_system, const String &name):
		m_shader_source_builder(shader_source_builder),
		m_file_system(file_system), m_name(name), m_culling(true)
	{
		assert(!m_shader_source_builder.expired());

		if (name == UTF8(""))
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_parameter_name(UTF8("name"))
				<< errinfo_message(UTF8("Paramter is empty")));
		}

		load();
	}

	Effect::~Effect() throw()
	{
	}

	void Effect::load_culling(const String &name, Lua &lua)
	{
		// Fetch the table from the Lua context and make sure it exists:
		lua.get_global(name);

		if (!lua.is_table(-1))
		{
			lua.pop(1);

			return;
		}

		lua.push_string(UTF8("culling"));
		lua.get_table(-2);

		if (!lua.is_bool(-1))
		{
			lua.pop(2);

			return;
		}

		m_culling = lua.to_bool(-1);

		lua.pop(2);
	}

	void Effect::load(String &name, Lua &lua) const
	{
		StringType str;
		std::vector<StringType> tokens;
		std::vector<StringType>::iterator it;

		boost::split(tokens, get_name().get(), boost::is_any_of(L"."),
			boost::token_compress_on);

		it = tokens.begin();

		while (it != tokens.end())
		{
			boost::algorithm::trim(*it);

			if (it->empty())
			{
				it = tokens.erase(it);
			}
			else
			{
				it++;
			}
		}

		if (tokens.size() != 2)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_message(UTF8("Invalid effect name"))
				<< errinfo_string_value(get_name()));
		}

		str = UTF8("shaders/");
		str += tokens[0];
		str += UTF8(".lua");

		lua.do_string(get_file_system()->get_file_string(String(str)),
			String(str));

		name = String(tokens[1]);
	}

	void Effect::do_load()
	{
		Lua lua;
		boost::scoped_ptr<ShaderSourceDescriptionLoader> loader;
		String name;
		StringType vertex, fragment;
		StringVariantMap values;
		Uint16 light_count, i, fragment_light_count, vertex_light_count;

		load(name, lua);
		load_culling(name, lua);

		loader.reset(new ShaderSourceDescriptionLoader(
			get_shader_source_builder()->get_global_vars(),
			get_shader_source_builder()->get_defaults(), name,
			get_shader_source_builder()->get_quality(), lua));

		m_light_counts.clear();
		m_default_programs.clear();

		fragment_light_count = get_shader_source_builder(
			)->get_fragment_light_count();
		vertex_light_count = get_shader_source_builder(
			)->get_vertex_light_count();

		if (get_shader_source_builder()->get_dynamic_light_count())
		{
			light_count = 0;
		}
		else
		{
			light_count = fragment_light_count;
		}

		m_light_counts.reserve(light_count + 1);
		m_default_programs.reserve(light_count + 1);

		/* Light shader for different light counts */
		for (i = 0; i < light_count; i++)
		{
			m_light_counts.push_back(glm::ivec2(0, i));

			get_shader_source_builder()->build(
				loader->get_shader_source_description(
					sbt_color, i), vertex, fragment,
					values);

			m_default_programs.push_back(
				boost::make_shared<GlslProgram>(vertex,
				fragment, values, get_name()));
		}

		light_count = fragment_light_count + vertex_light_count;

		/* Max light shader */
		m_light_counts.push_back(glm::ivec2(vertex_light_count,
			fragment_light_count));

		get_shader_source_builder()->build(
			loader->get_shader_source_description(sbt_color,
				light_count), vertex, fragment, values);

		m_default_programs.push_back(boost::make_shared<GlslProgram>(
			vertex, fragment, values, get_name()));

		/* Depth shader */
		get_shader_source_builder()->build(
			loader->get_shader_source_description(sbt_depth, 0),
			vertex, fragment, values);

		m_depth_program = boost::make_shared<GlslProgram>(
			vertex, fragment, values, get_name());

		/* Shadow shader */
		get_shader_source_builder()->build(
			loader->get_shader_source_description(sbt_shadow, 0),
			vertex, fragment, values);

		m_shadow_program = boost::make_shared<GlslProgram>(
			vertex, fragment, values, get_name());
	}

	void Effect::error_load()
	{
		StringVariantMap values;

		m_culling = true;

		m_light_counts.clear();
		m_default_programs.clear();

		/* Default shader */

		values[ShaderTextureUtil::get_str(stt_diffuse_0)] =
			Variant(static_cast<Sint64>(stt_diffuse_0));

		m_light_counts.push_back(glm::ivec2(0));

		m_default_programs.push_back(boost::make_shared<GlslProgram>(
			vertex_shader, fragment_shader, values, get_name()));

		/* Depth shader */
		m_depth_program = boost::make_shared<GlslProgram>(
			depth_vertex_shader, depth_fragment_shader,
			StringVariantMap(), get_name());

		/* Shadow shader */
		m_shadow_program = m_depth_program;
	}

	void Effect::load()
	{
		try
		{
			if (!m_shader_source_builder.expired())
			{
				do_load();

				return;
			}
		}
		catch (boost::exception &exception)
		{
			exception << errinfo_item_name(get_name());

			LOG_EXCEPTION(exception);
		}
		catch (std::exception &exception)
		{
			LOG_EXCEPTION(exception);
		}

		error_load();
	}

}
