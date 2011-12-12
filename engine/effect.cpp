/****************************************************************************
 *            effect.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "effect.hpp"
#include "glslprogram.hpp"
#include "exceptions.hpp"
#include "shader/shadersourcebuilder.hpp"
#include "shader/shadersourcedescription.hpp"
#include "logging.hpp"
#include "shader/shadertextureutil.hpp"
#include "xmlreader.hpp"

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

	Effect::Effect(): m_name(UTF8("simple")), m_max_index(0)
	{
		error_load();
	}

	Effect::Effect(const ShaderSourceBuilderWeakPtr &shader_source_builder,
		const FileSystemWeakPtr &file_system, const String &name):
		m_shader_source_builder(shader_source_builder),
		m_file_system(file_system), m_name(name), m_max_index(0)
	{
		assert(!m_shader_source_builder.expired());

		if (name.get().empty())
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

	void Effect::build_default_shader(ShaderSourceDescription description,
		const Uint16 vertex_light_count,
		const Uint16 fragment_light_count)
	{
		GlslProgramSharedPtr program;
		StringType vertex, fragment;
		StringVariantMap values;
		Uint16 light_count;

		m_light_counts.push_back(glm::ivec2(vertex_light_count,
			fragment_light_count));

		light_count = vertex_light_count + fragment_light_count;

		get_shader_source_builder()->build(light_count, sbt_color,
			description, vertex, fragment, values);

		program = boost::make_shared<GlslProgram>(vertex, fragment,
			values, get_name());

		m_default_programs.push_back(program);

		if (description.get_receives_shadows())
		{
			values.clear();

			description.set_receives_shadows(false);

			get_shader_source_builder()->build(light_count,
				sbt_color, description, vertex, fragment,
				values);

			program = boost::make_shared<GlslProgram>(vertex,
				fragment, values, get_name());
		}

		m_default_programs.push_back(program);
	}

	void Effect::do_load()
	{
		XmlReaderSharedPtr xml_reader;
		ShaderSourceDescription description;
		StringType vertex, fragment;
		StringVariantMap values;
		String file_name;
		Uint16 light_count, i, fragment_light_count, vertex_light_count;

		m_light_counts.clear();
		m_default_programs.clear();
		m_max_index = 0;

		xml_reader = XmlReaderSharedPtr(new XmlReader(String(
			UTF8("shaders/") + get_name().get() + UTF8(".xml"))));

		description.load_xml(xml_reader->get_root_node());

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

		m_max_index = light_count;

		m_light_counts.reserve(get_max_index() + 1);
		m_default_programs.reserve((get_max_index() + 1) * 2);

		/* Light shader for different light counts */
		for (i = 0; i < light_count; i++)
		{
			build_default_shader(description, 0, i);
		}

		/* Max light shader */
		build_default_shader(description, vertex_light_count,
			fragment_light_count);

		/* Depth shader */
		get_shader_source_builder()->build(0, sbt_depth, description,
			vertex, fragment, values);

		m_depth_program = boost::make_shared<GlslProgram>(
			vertex, fragment, values, get_name());

		values.clear();

		/* Shadow shader */
		get_shader_source_builder()->build(0, sbt_shadow, description,
			vertex, fragment, values);

		m_shadow_program = boost::make_shared<GlslProgram>(
			vertex, fragment, values, get_name());
	}

	void Effect::error_load()
	{
		GlslProgramSharedPtr program;
		StringVariantMap values;

		m_light_counts.clear();
		m_default_programs.clear();
		m_max_index = 0;

		/* Default shader */
		values[ShaderTextureUtil::get_str(stt_diffuse_0)] =
			Variant(static_cast<Sint64>(stt_diffuse_0));

		m_light_counts.push_back(glm::ivec2(0));

		program = boost::make_shared<GlslProgram>(vertex_shader,
			fragment_shader, values, get_name());

		m_default_programs.push_back(program);
		m_default_programs.push_back(program);

		values.clear();

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

		assert(get_index(std::numeric_limits<Uint16>::max())
			< m_light_counts.size());
		assert(get_index(std::numeric_limits<Uint16>::max(), true)
			< m_default_programs.size());
		assert(get_index(std::numeric_limits<Uint16>::max(), false)
			< m_default_programs.size());
	}

}
