/****************************************************************************
 *            effect.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "effect.hpp"
#include "glslprogram.hpp"
#include "exceptions.hpp"
#include "shader/shadersourcebuilder.hpp"
#include "shader/shadertextureutil.hpp"
#include "logging.hpp"

namespace eternal_lands
{

	namespace
	{

		const StringType vertex_shader = UTF8(""
			"#version 120\n"
			"\n"
			"varying vec2 uv;\n"
			"\n"
			"uniform mat2x4 world_transformation;\n"
			"uniform mat4 projection_view_matrix;\n"
			"\n"
			"attribute vec2 texture_coordinate_0;\n"
			"attribute vec4 position;\n"
			"\n"
			"void main ()\n"
			"{\n"
			"\tvec3 world_position;\n"
			"\n"
			"\tworld_position = position.xyz;\n"
			"\tworld_position += 2.0 * cross("
			"world_transformation[0].xyz, cross("
			"world_transformation[0].xyz, "
			"world_position) + world_transformation[0].w * "
			"world_position);\n"
			"\tworld_position *= world_transformation[1].w;\n"
			"\tworld_position += world_transformation[1].xyz;\n"
			"\tgl_Position = projection_view_matrix * "
				"vec4(world_position, 1.0);\n"
			"\tuv = texture_coordinate_0;\n"
			"}\n");

		const StringType fragment_shader = UTF8(""
			"#version 120\n"
			"\n"
			"varying vec2 uv;\n"
			"\n"
			"uniform sampler2D albedo_sampler_0;\n"
			"uniform vec4 ambient;\n"
			"uniform float alpha_value;\n"
			"\n"
			"void main ()\n"
			"{\n"
			"\tgl_FragColor.rgb = texture2D(albedo_sampler_0, "
				"uv).rgb * ambient.rgb;\n"
			"\tgl_FragColor.a = alpha_value;\n"
			"}\n");

		const StringType depth_vertex_shader = UTF8(""
			"#version 120\n"
			"\n"
			"uniform mat2x4 world_transformation;\n"
			"uniform mat4 projection_view_matrix;\n"
			"\n"
			"attribute vec4 position;\n"
			"\n"
			"void main ()\n"
			"{\n"
			"\tvec3 world_position;\n"
			"\n"
			"\tworld_position = position.xyz;\n"
			"\tworld_position += 2.0 * cross("
			"world_transformation[0].xyz, cross("
			"world_transformation[0].xyz, "
			"world_position) + world_transformation[0].w * "
			"world_position);\n"
			"\tworld_position *= world_transformation[1].w;\n"
			"\tworld_position += world_transformation[1].xyz;\n"
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

	Effect::Effect()
	{
		error_load();
	}

	Effect::Effect(const ShaderSourceBuilderWeakPtr &shader_source_builder,
		const EffectDescription &description):
		m_shader_source_builder(shader_source_builder),
		m_description(description)
	{
		assert(!m_shader_source_builder.expired());

		load();
	}

	Effect::~Effect() throw()
	{
	}

	void Effect::build_default_shader(const EffectDescription &description,
		const Uint16 vertex_light_count,
		const Uint16 fragment_light_count)
	{
		StringStream str;
		StringType vertex, tess_control, tess_evaluation, geometry;
		StringType fragment;
		StringVariantMap values;
		Uint16 light_count;

		str << description.get_name() << UTF8(" [");
		str << UTF8("vl-") << vertex_light_count;
		str << UTF8(" fl-") << fragment_light_count;
		str << UTF8("]");

		light_count = vertex_light_count + fragment_light_count;

		get_shader_source_builder()->build(description, sbt_color,
			light_count, vertex, tess_control, tess_evaluation,
			geometry, fragment, values);

		m_programs[ept_default] = boost::make_shared<GlslProgram>(
			vertex, tess_control, tess_evaluation, geometry,
			fragment, values, String(str.str()));
	}

	void Effect::build_deferred_shader(const EffectDescription &description)
	{
		StringType vertex, tess_control, tess_evaluation, geometry;
		StringType fragment;
		StringVariantMap values;

		get_shader_source_builder()->build(description, sbt_deferred,
			0, vertex, tess_control, tess_evaluation, geometry,
			fragment, values);

		m_programs[ept_default] = boost::make_shared<GlslProgram>(
			vertex, tess_control, tess_evaluation, geometry,
			fragment, values, String(description.get_name().get() +
				UTF8("[deferred]")));
	}

	void Effect::do_load()
	{
		StringType vertex, tess_control, tess_evaluation, geometry;
		StringType fragment;
		StringVariantMap values;
		String file_name;
		Uint16 fragment_light_count, vertex_light_count;

		m_programs[ept_default].reset();
		m_programs[ept_shadow].reset();
		m_programs[ept_depth].reset();

		fragment_light_count = get_shader_source_builder(
			)->get_fragment_light_count();
		vertex_light_count = get_shader_source_builder(
			)->get_vertex_light_count();

		/* Light shader */
		build_default_shader(m_description, vertex_light_count,
			fragment_light_count);

		/* Depth shader */
		get_shader_source_builder()->build(m_description, sbt_depth, 0,
			vertex, tess_control, tess_evaluation, geometry,
			fragment, values);

		m_programs[ept_depth] = boost::make_shared<GlslProgram>(vertex,
			tess_control, tess_evaluation, geometry, fragment,
			values, String(m_description.get_name().get() +
				UTF8("[depth]")));

		values.clear();

		/* Shadow shader */
		get_shader_source_builder()->build(m_description, sbt_shadow, 0,
			vertex, tess_control, tess_evaluation, geometry,
			fragment, values);

		m_programs[ept_shadow] = boost::make_shared<GlslProgram>(vertex,
			tess_control, tess_evaluation, geometry, fragment,
			values, String(m_description.get_name().get() +
				UTF8("[shadow]")));
	}

	void Effect::error_load()
	{
		StringVariantMap values;
		StringType none;

		m_programs[ept_default].reset();
		m_programs[ept_shadow].reset();
		m_programs[ept_depth].reset();

		/* Default shader */
		values[ShaderTextureUtil::get_str(stt_albedo_0)] =
			Variant(static_cast<Sint64>(stt_albedo_0));

		m_programs[ept_default] = boost::make_shared<GlslProgram>(
			vertex_shader, none, none, none, fragment_shader,
			values, String(UTF8("error")));

		values.clear();

		/* Depth shader */
		m_programs[ept_depth] = boost::make_shared<GlslProgram>(
			depth_vertex_shader, none, none, none,
			depth_fragment_shader, StringVariantMap(),
			String(UTF8("error [depth]")));

		/* Shadow shader */
		m_programs[ept_shadow] = m_programs[ept_depth];
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
			LOG_EXCEPTION(exception);
		}
		catch (std::exception &exception)
		{
			LOG_EXCEPTION(exception);
		}

		error_load();
	}

}
