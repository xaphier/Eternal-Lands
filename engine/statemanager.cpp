/****************************************************************************
 *            statemanager.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "statemanager.hpp"
#include "glslprogram.hpp"
#include "texture.hpp"
#include "abstractmesh.hpp"
#include "logging.hpp"

namespace eternal_lands
{

	StateManager::StateManager(): m_layer_index(0.0f), m_color_mask(true),
		m_texture_unit(0), m_multisample(false), m_blend(false),
		m_culling(true), m_depth_mask(true), m_depth_test(true),
		m_scissor_test(false), m_sample_alpha_to_coverage(false),
		m_polygon_offset_fill(false)
	{
		m_program_used_texture_units.set();
	}

	StateManager::~StateManager() throw()
	{
	}

	void StateManager::init()
	{
		set_multisample(true);
		set_blend(false);
		set_culling(true);
		set_color_mask(glm::bvec4(true));
		set_depth_mask(true);
		set_depth_test(true);
		set_scissor_test(false);
		set_sample_alpha_to_coverage(false);
		set_polygon_offset_fill(false);
		set_layer_index(glm::vec4(0.0f));
		glCullFace(GL_BACK);
	}

	void StateManager::set_mesh(const AbstractMeshSharedPtr &mesh)
	{
		m_mesh = mesh;

		m_mesh->bind(m_used_attributes);
	}

	void StateManager::set_program(const GlslProgramSharedPtr &program)
	{
		m_program = program;
		m_program->bind();
		m_program_used_texture_units =
			m_program->get_used_texture_units();
	}

	void StateManager::set_texture(const Uint16 texture_unit,
		const TextureSharedPtr &texture)
	{
		switch_texture_unit(texture_unit);

		m_textures[texture_unit] = texture;
		m_textures[texture_unit]->bind();
		m_used_texture_units[texture_unit] = true;
	}

	void StateManager::set_multisample(const bool multisample)
	{
		m_multisample = multisample;

		if (m_multisample)
		{
			glEnable(GL_MULTISAMPLE);
		}
		else
		{
			glDisable(GL_MULTISAMPLE);
		}
	}

	void StateManager::set_blend(const bool blend)
	{
		m_blend = blend;

		if (m_blend)
		{
			glEnable(GL_BLEND);
		}
		else
		{
			glDisable(GL_BLEND);
		}
	}

	void StateManager::set_culling(const bool culling)
	{
		m_culling = culling;

		if (m_culling)
		{
			glEnable(GL_CULL_FACE);
		}
		else
		{
			glDisable(GL_CULL_FACE);
		}
	}

	void StateManager::set_depth_mask(const bool depth_mask)
	{
		m_depth_mask = depth_mask;

		if (m_depth_mask)
		{
			glDepthMask(GL_TRUE);
		}
		else
		{
			glDepthMask(GL_FALSE);
		}
	}

	void StateManager::set_depth_test(const bool depth_test)
	{
		m_depth_test = depth_test;

		if (m_depth_test)
		{
			glEnable(GL_DEPTH_TEST);
		}
		else
		{
			glDisable(GL_DEPTH_TEST);
		}
	}

	void StateManager::set_scissor_test(const bool scissor_test)
	{
		m_scissor_test = scissor_test;

		if (m_scissor_test)
		{
			glEnable(GL_SCISSOR_TEST);
		}
		else
		{
			glDisable(GL_SCISSOR_TEST);
		}
	}

	void StateManager::set_color_mask(const glm::bvec4 &color_mask)
	{
		glm::ivec4 mask;
		Uint16 i;

		m_color_mask = color_mask;

		for (i = 0; i < color_mask.length(); ++i)
		{
			if (color_mask[i])
			{
				mask[i] = GL_TRUE;
			}
			else
			{
				mask[i] = GL_FALSE;
			}
		}

		glColorMask(mask[0], mask[1], mask[2], mask[3]);
	}

	void StateManager::set_polygon_offset_fill(
		const bool polygon_offset_fill)
	{
		m_polygon_offset_fill = polygon_offset_fill;

		if (m_polygon_offset_fill)
		{
			glEnable(GL_POLYGON_OFFSET_FILL);
		}
		else
		{
			glDisable(GL_POLYGON_OFFSET_FILL);
		}
	}

	void StateManager::set_sample_alpha_to_coverage(
		const bool sample_alpha_to_coverage)
	{
		m_sample_alpha_to_coverage = sample_alpha_to_coverage;

		if (m_sample_alpha_to_coverage)
		{
			glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);
		}
		else
		{
			glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);
		}
	}

	void StateManager::set_layer_index(const glm::vec4 &layer_index)
	{
		m_layer_index = layer_index;

		glVertexAttrib4fv(vst_layer_index,
			glm::value_ptr(m_layer_index));
	}

	bool StateManager::switch_texture_unit(const Uint16 texture_unit)
	{
		assert(texture_unit < m_textures.size());

		if (m_texture_unit == texture_unit)
		{
			return false;
		}

		m_texture_unit = texture_unit;

		glActiveTexture(GL_TEXTURE0 + m_texture_unit);

		return true;
	}

	bool StateManager::unbind_all()
	{
		bool result;

		result = unbind_mesh();
		result |= unbind_program();
		result |= unbind_textures();
		result |= switch_multisample(false);
		result |= switch_blend(false);
		result |= switch_culling(true);
		result |= switch_color_mask(glm::bvec4(true));
		result |= switch_depth_mask(true);
		result |= switch_depth_test(true);
		result |= switch_scissor_test(false);
		result |= switch_polygon_offset_fill(false);
		result |= switch_sample_alpha_to_coverage(false);
		switch_texture_unit(0);

		return result;
	}

	bool StateManager::unbind_mesh()
	{
		m_used_attributes.reset();

		if (m_mesh.get() != 0)
		{
			m_mesh->unbind();
			m_mesh.reset();

			return true;
		}

		return false;
	}

	bool StateManager::unbind_program()
	{
		if (m_program.get() != 0)
		{
			m_program->unbind();
			m_program.reset();
			m_program_used_texture_units.set();

			return true;
		}

		return false;
	}

	bool StateManager::unbind_texture(const Uint16 texture_unit)
	{
		if (m_textures[texture_unit].get() != 0)
		{
			switch_texture_unit(texture_unit);

			m_textures[texture_unit]->unbind();
			m_textures[texture_unit].reset();
			m_used_texture_units[texture_unit] = false;

			return true;
		}

		return false;
	}

	bool StateManager::unbind_textures()
	{
		Uint32 i;
		bool result;

		result = false;

		for (i = 0; i < m_textures.size(); ++i)
		{
			result |= unbind_texture(i);
		}

		return result;
	}

	void StateManager::log_texture_units()
	{
		Uint32 i, count;
		GLint id;

		count = m_textures.size();

		for (i = 0; i < count; ++i)
		{
			if (m_textures[i].get() == 0)
			{
				continue;
			}

			LOG_ERROR(UTF8("Texture '%1%' with id %2% bound to "
				"unit %3%."), m_textures[i]->get_name() %
				m_textures[i]->get_texture_id() % i);
		}

		for (i = 0; i < count; ++i)
		{
			if (m_textures[i].get() == 0)
			{
				continue;
			}

			switch_texture_unit(i);

			id = 0;

			glGetIntegerv(GL_TEXTURE_BINDING_2D, &id);

			LOG_ERROR(UTF8("Bound texture id: %1%"), id);
			assert(static_cast<GLuint>(id) ==
				m_textures[i]->get_texture_id());
		}
	}

	void StateManager::gl_error_check()
	{
		GLint gl_error;

		gl_error = glGetError();

		if (gl_error != GL_NO_ERROR)
		{
			LOG_ERROR(UTF8("GL error %1%: '%2%'"), gl_error %
				reinterpret_cast<const char*>(
					gluErrorString(gl_error)));
			log_texture_units();
			m_program->validate();
			LOG_ERROR(UTF8("Mesh '%1%' used."), m_mesh->get_name());
		}

		if ((m_program_used_texture_units & m_used_texture_units) !=
			m_program_used_texture_units)
		{
			LOG_ERROR(UTF8("Used texture units %1%."),
				m_used_texture_units);
			LOG_ERROR(UTF8("Program used texture units %1%."),
				m_program_used_texture_units);
			log_texture_units();
			m_program->validate();
			LOG_ERROR(UTF8("Mesh '%1%' used."), m_mesh->get_name());
		}
	}

	void StateManager::draw(const Uint32 index)
	{
		m_mesh->draw(index);
#ifndef	NDEBUG
		gl_error_check();
#endif	/* NDEBUG */
	}

	void StateManager::draw(const MeshDrawData &draw_data)
	{
		m_mesh->draw(draw_data);
#ifndef	NDEBUG
		gl_error_check();
#endif	/* NDEBUG */
	}

}
