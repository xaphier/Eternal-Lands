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

namespace eternal_lands
{

	StateManager::StateManager(): m_texture_unit(0), m_color_mask(true),
		m_multisample(false), m_blend(false), m_culling(true),
		m_depth_mask(true), m_depth_test(true), m_scissor_test(false),
		m_sample_alpha_to_coverage(false), m_polygon_offset_fill(false)
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
		glCullFace(GL_BACK);
	}

	void StateManager::set_mesh(const AbstractMeshSharedPtr &mesh)
	{
		m_mesh = mesh;
		m_mesh->bind();
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

		for (i = 0; i < 4; i++)
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

			return true;
		}

		return false;
	}

	bool StateManager::unbind_textures()
	{
		Uint32 i;
		bool result;

		result = false;

		for (i = 0; i < m_textures.size(); i++)
		{
			result |= unbind_texture(i);
		}

		return result;
	}

}
