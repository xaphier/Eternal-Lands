/****************************************************************************
 *            effect.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "effect.hpp"
#include "shader/glslprogram.hpp"
#include "shader/glslprogramdescription.hpp"
#include "exceptions.hpp"
#include "shader/shadersourcebuilder.hpp"
#include "shader/samplerparameterutil.hpp"
#include "logging.hpp"
#include "shader/glslprogramcache.hpp"

namespace eternal_lands
{

	namespace
	{

		const String vertex_shader = String(UTF8(""
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
			"}\n"));

		const String fragment_shader = String(UTF8(""
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
			"}\n"));

		const String depth_vertex_shader = String(UTF8(""
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
			"}\n"));

		const String depth_fragment_shader = String(UTF8(""
			"#version 120\n"
			"\n"
			"void main ()\n"
			"{\n"
			"\tgl_FragColor = gl_FragCoord.zzzz;\n"
			"}\n"));

	}

	Effect::Effect(const GlslProgramCacheWeakPtr &glsl_program_cache):
		m_glsl_program_cache(glsl_program_cache)
	{
		assert(!m_shader_source_builder.expired());

		error_load();
	}

	Effect::Effect(const GlslProgramCacheWeakPtr &glsl_program_cache,
		const ShaderSourceBuilderWeakPtr &shader_source_builder,
		const EffectDescription &description):
		m_glsl_program_cache(glsl_program_cache),
		m_shader_source_builder(shader_source_builder),
		m_description(description), m_default_shader(sbt_default),
		m_debug_shader(sbt_debug_uv)
	{
		assert(!m_shader_source_builder.expired());

		load();
	}

	Effect::~Effect() noexcept
	{
	}

	void Effect::build_default_shader(const EffectDescription &description,
		const Uint16 vertex_light_count,
		const Uint16 fragment_light_count)
	{
		GlslProgramDescription program_description;
		Uint16 light_count;

		light_count = 0;

		if (get_default_shader() == sbt_default)
		{
			light_count = vertex_light_count + fragment_light_count;
		}

		LOG_DEBUG(lt_shader_source, UTF8("%1% effect '%2%'"),
			get_default_shader() % m_description.get_name());

		get_shader_source_builder()->build(description,
			get_default_shader(), light_count,
			program_description);

		m_programs[ept_default] = get_glsl_program_cache()->get_program(
			program_description);
	}

	void Effect::do_load()
	{
		GlslProgramDescription program_description;
		Uint16 fragment_light_count, vertex_light_count;
		Uint16 light_count;

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
		LOG_DEBUG(lt_shader_source, UTF8("%1% effect '%2%'"),
			sbt_depth % m_description.get_name());

		get_shader_source_builder()->build(m_description, sbt_depth, 0,
			program_description);

		m_programs[ept_depth] = get_glsl_program_cache()->get_program(
			program_description);

		/* Shadow shader */
		LOG_DEBUG(lt_shader_source, UTF8("%1% effect '%2%'"),
			sbt_shadow % m_description.get_name());

		get_shader_source_builder()->build(m_description, sbt_shadow, 0,
			program_description);

		m_programs[ept_shadow] = get_glsl_program_cache()->get_program(
			program_description);

		/* debug shader */
		LOG_DEBUG(lt_shader_source, UTF8("%1% effect '%2%'"),
			get_debug_shader() % m_description.get_name());

		light_count = vertex_light_count + fragment_light_count;

		get_shader_source_builder()->build(m_description,
			get_debug_shader(), light_count, program_description);

		m_programs[ept_debug] = get_glsl_program_cache()->get_program(
			program_description);
	}

	void Effect::error_load()
	{
		String none;

		m_programs[ept_default].reset();
		m_programs[ept_shadow].reset();
		m_programs[ept_depth].reset();

		/* Default shader */
		m_programs[ept_default] = get_glsl_program_cache()->get_program(
			GlslProgramDescription(vertex_shader, none, none, none,
				fragment_shader));

		/* Depth shader */
		m_programs[ept_depth] = get_glsl_program_cache()->get_program(
			GlslProgramDescription(depth_vertex_shader, none, none,
				none, depth_fragment_shader));

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

	bool Effect::get_simple_shadow() const
	{
		return get_program(ept_shadow)->get_used_texture_units().none();
	}

}
