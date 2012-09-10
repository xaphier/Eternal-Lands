/****************************************************************************
 *            effect.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
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
			"uniform mat3x4 world_transformation;\n"
			"uniform mat4 projection_view_matrix;\n"
			"\n"
			"attribute vec2 texture_coordinate_0;\n"
			"attribute vec4 position;\n"
			"\n"
			"void main ()\n"
			"{\n"
			"\tvec3 world_position;\n"
			"\n"
			"\tworld_position = vec4(position.xyz, 1.0) * "
			"world_transformation;\n"
			"\tgl_Position = projection_view_matrix * "
				"vec4(world_position, 1.0);\n"
			"\tuv = texture_coordinate_0;\n"
			"}\n"));

		const String fragment_shader = String(UTF8(""
			"#version 120\n"
			"\n"
			"varying vec2 uv;\n"
			"\n"
			"uniform sampler2D effect_sampler_0;\n"
			"uniform vec4 ambient;\n"
			"uniform float alpha_value;\n"
			"\n"
			"void main ()\n"
			"{\n"
			"\tgl_FragColor.rgb = texture2D(effect_sampler_0, "
				"uv).rgb * ambient.rgb;\n"
			"\tgl_FragColor.a = alpha_value;\n"
			"}\n"));

		const String depth_vertex_shader = String(UTF8(""
			"#version 120\n"
			"\n"
			"uniform mat3x4 world_transformation;\n"
			"uniform mat4 projection_view_matrix;\n"
			"\n"
			"attribute vec4 position;\n"
			"\n"
			"void main ()\n"
			"{\n"
			"\tvec3 world_position;\n"
			"\n"
			"\tworld_position = vec4(position.xyz, 1.0) * "
			"world_transformation;\n"
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
		m_glsl_program_cache(glsl_program_cache),
		m_debug_shader(sbt_debug_uv)
	{
		error_load();
	}

	Effect::Effect(const GlslProgramCacheWeakPtr &glsl_program_cache,
		const ShaderSourceBuilderWeakPtr &shader_source_builder,
		const EffectDescription &description):
		m_glsl_program_cache(glsl_program_cache),
		m_shader_source_builder(shader_source_builder),
		m_description(description), m_debug_shader(sbt_debug_uv)
	{
		assert(!m_shader_source_builder.expired());

		load();
	}

	Effect::~Effect() noexcept
	{
	}

	void Effect::do_load()
	{
		GlslProgramDescription program_description;
		Uint16 fragment_lights_count, vertex_lights_count;
		Uint16 lights_count;

		m_programs[ept_default].reset();
		m_programs[ept_shadow].reset();
		m_programs[ept_depth].reset();

		fragment_lights_count = get_shader_source_builder(
			)->get_fragment_lights_count();
		vertex_lights_count = get_shader_source_builder(
			)->get_vertex_lights_count();

		/* default shader */
		lights_count = 0;

		if (get_description().get_description() != edt_default)
		{
			LOG_DEBUG(lt_shader_source, UTF8("%1% effect '%2%'"),
				sbt_screen_quad % m_description.get_name());

			get_shader_source_builder()->build(m_description,
				sbt_screen_quad,
				get_description().get_output(), lights_count,
				program_description);

			m_programs[ept_default] = get_glsl_program_cache(
				)->get_program(program_description);

			m_programs[ept_depth] = get_glsl_program_cache(
				)->get_program(program_description);

			m_programs[ept_shadow] = get_glsl_program_cache(
				)->get_program(program_description);

			m_programs[ept_debug] = get_glsl_program_cache(
				)->get_program(program_description);

			return;
		}

		LOG_DEBUG(lt_shader_source, UTF8("%1% effect '%2%'"),
			sbt_default % m_description.get_name());

		lights_count = vertex_lights_count + fragment_lights_count;

		get_shader_source_builder()->build(m_description, sbt_default,
			get_description().get_output(), lights_count,
			program_description);

		m_programs[ept_default] = get_glsl_program_cache()->get_program(
			program_description);

		/* Depth shader */
		LOG_DEBUG(lt_shader_source, UTF8("%1% effect '%2%'"),
			sbt_depth % m_description.get_name());

		get_shader_source_builder()->build(m_description, sbt_depth,
			sot_float, 0, program_description);

		m_programs[ept_depth] = get_glsl_program_cache()->get_program(
			program_description);

		/* Shadow shader */
		LOG_DEBUG(lt_shader_source, UTF8("%1% effect '%2%'"),
			sbt_shadow % m_description.get_name());

		get_shader_source_builder()->build(m_description, sbt_shadow,
			sot_float, 0, program_description);

		m_programs[ept_shadow] = get_glsl_program_cache()->get_program(
			program_description);

		/* debug shader */
		LOG_DEBUG(lt_shader_source, UTF8("%1% effect '%2%'"),
			get_debug_shader() % m_description.get_name());

		lights_count = vertex_lights_count + fragment_lights_count;

		get_shader_source_builder()->build(m_description,
			get_debug_shader(), sot_float, lights_count,
			program_description);

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
