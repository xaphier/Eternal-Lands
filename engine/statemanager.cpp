/****************************************************************************
 *            statemanager.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "statemanager.hpp"
#include "shader/glslprogram.hpp"
#include "texture.hpp"
#include "abstractmesh.hpp"
#include "logging.hpp"
#include "exceptions.hpp"
#include "globalvars.hpp"

namespace eternal_lands
{

	StateManager::StateManager(const GlobalVarsSharedPtr &global_vars):
		m_global_vars(global_vars)
	{
		m_program_used_texture_units.set();
	}

	StateManager::~StateManager() noexcept
	{
	}

	void StateManager::set_state_set(const StateSet &state_set)
	{
		switch_color_mask(state_set.get_color_mask());
		switch_restart_index(state_set.get_restart_index());
		switch_texture_unit(state_set.get_texture_unit());
		switch_multisample(state_set.get_multisample());
		switch_blend(state_set.get_blend());
		switch_culling(state_set.get_culling());
		switch_depth_mask(state_set.get_depth_mask());
		switch_depth_test(state_set.get_depth_test());
		switch_scissor_test(state_set.get_scissor_test());
		switch_sample_alpha_to_coverage(
			state_set.get_sample_alpha_to_coverage());
		switch_use_restart_index(state_set.get_use_restart_index());
		switch_polygon_offset_fill(state_set.get_polygon_offset_fill());
		switch_stencil_test(state_set.get_stencil_test());
		switch_flip_back_face_culling(
			state_set.get_flip_back_face_culling());
		switch_depth_clamp(state_set.get_depth_clamp());
		switch_framebuffer_sRGB(state_set.get_framebuffer_sRGB());
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
		set_flip_back_face_culling(false);

		set_restart_index(std::numeric_limits<Uint32>::max());
		set_use_restart_index(true);
	}

	void StateManager::set_mesh(const AbstractMeshSharedPtr &mesh)
	{
		m_mesh = mesh;

		m_mesh->bind(m_used_attributes);

		switch_restart_index(m_mesh->get_restart_index());
		switch_use_restart_index(m_mesh->get_use_restart_index());
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

	void StateManager::set_multisample(const bool multisample) noexcept
	{
		m_state_set.set_multisample(multisample);

		if (m_state_set.get_multisample())
		{
			glEnable(GL_MULTISAMPLE);
		}
		else
		{
			glDisable(GL_MULTISAMPLE);
		}
	}

	void StateManager::set_blend(const bool blend) noexcept
	{
		m_state_set.set_blend(blend);

		if (m_state_set.get_blend())
		{
			glEnable(GL_BLEND);
		}
		else
		{
			glDisable(GL_BLEND);
		}
	}

	void StateManager::set_culling(const bool culling) noexcept
	{
		m_state_set.set_culling(culling);

		if (m_state_set.get_culling())
		{
			glEnable(GL_CULL_FACE);
		}
		else
		{
			glDisable(GL_CULL_FACE);
		}
	}

	void StateManager::set_depth_mask(const bool depth_mask) noexcept
	{
		m_state_set.set_depth_mask(depth_mask);

		if (m_state_set.get_depth_mask())
		{
			glDepthMask(GL_TRUE);
		}
		else
		{
			glDepthMask(GL_FALSE);
		}
	}

	void StateManager::set_stencil_test(const bool stencil_test) noexcept
	{
		m_state_set.set_stencil_test(stencil_test);

		if (m_state_set.get_stencil_test())
		{
			glEnable(GL_STENCIL_TEST);
		}
		else
		{
			glDisable(GL_STENCIL_TEST);
		}
	}

	void StateManager::set_depth_test(const bool depth_test) noexcept
	{
		m_state_set.set_depth_test(depth_test);

		if (m_state_set.get_depth_test())
		{
			glEnable(GL_DEPTH_TEST);
		}
		else
		{
			glDisable(GL_DEPTH_TEST);
		}
	}

	void StateManager::set_scissor_test(const bool scissor_test) noexcept
	{
		m_state_set.set_scissor_test(scissor_test);

		if (m_state_set.get_scissor_test())
		{
			glEnable(GL_SCISSOR_TEST);
		}
		else
		{
			glDisable(GL_SCISSOR_TEST);
		}
	}

	void StateManager::set_color_mask(const glm::bvec4 &color_mask) noexcept
	{
		glm::ivec4 mask;
		Uint16 i;

		m_state_set.set_color_mask(color_mask);

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
		const bool polygon_offset_fill) noexcept
	{
		m_state_set.set_polygon_offset_fill(polygon_offset_fill);

		if (m_state_set.get_polygon_offset_fill())
		{
			glEnable(GL_POLYGON_OFFSET_FILL);
		}
		else
		{
			glDisable(GL_POLYGON_OFFSET_FILL);
		}
	}

	void StateManager::set_sample_alpha_to_coverage(
		const bool sample_alpha_to_coverage) noexcept
	{
		m_state_set.set_sample_alpha_to_coverage(
			sample_alpha_to_coverage);

		if (m_state_set.get_sample_alpha_to_coverage())
		{
			glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);
		}
		else
		{
			glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);
		}
	}

	void StateManager::set_restart_index(const Uint32 restart_index)
		noexcept
	{
		m_state_set.set_restart_index(restart_index);

		if (get_global_vars()->get_opengl_3_1())
		{
			glPrimitiveRestartIndex(restart_index);
		}
	}

	void StateManager::set_use_restart_index(const bool use_restart_index)
		noexcept
	{
		m_state_set.set_use_restart_index(use_restart_index);

		if (!get_global_vars()->get_opengl_3_1())
		{
			return;
		}

		if (m_state_set.get_use_restart_index())
		{
			glEnable(GL_PRIMITIVE_RESTART);
		}
		else
		{
			glDisable(GL_PRIMITIVE_RESTART);
		}
	}

	void StateManager::set_flip_back_face_culling(
		const bool flip_back_face_culling) noexcept
	{
		m_state_set.set_flip_back_face_culling(flip_back_face_culling);

		if (m_state_set.get_flip_back_face_culling())
		{
			glCullFace(GL_FRONT);
		}
		else
		{
			glCullFace(GL_BACK);
		}
	}

	void StateManager::set_depth_clamp(const bool depth_clamp) noexcept
	{
		m_state_set.set_depth_clamp(depth_clamp);

		if (!get_global_vars()->get_opengl_3_2())
		{
			return;
		}

		if (m_state_set.get_depth_clamp())
		{
			glEnable(GL_DEPTH_CLAMP);
		}
		else
		{
			glDisable(GL_DEPTH_CLAMP);
		}
	}

	void StateManager::set_framebuffer_sRGB(const bool framebuffer_sRGB)
		noexcept
	{
		m_state_set.set_framebuffer_sRGB(framebuffer_sRGB);

		if (!get_global_vars()->get_opengl_3_0() &&
			!GL_EXT_framebuffer_sRGB && !GL_ARB_framebuffer_sRGB)
		{
			return;
		}

		if (m_state_set.get_framebuffer_sRGB())
		{
			glEnable(GL_FRAMEBUFFER_SRGB);
		}
		else
		{
			glDisable(GL_FRAMEBUFFER_SRGB);
		}
	}

	bool StateManager::switch_texture_unit(const Uint16 texture_unit)
	{
		RANGE_CECK_MAX(texture_unit, m_textures.size() - 1,
			UTF8("texture_unit value too big"));

		if (m_state_set.get_texture_unit() == texture_unit)
		{
			return false;
		}

		m_state_set.set_texture_unit(texture_unit);

		glActiveTexture(GL_TEXTURE0 + texture_unit);

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
		result |= switch_stencil_test(false);
		result |= switch_sample_alpha_to_coverage(false);
		result |= switch_use_restart_index(false);
		result |= switch_flip_back_face_culling(false);
		result |= switch_depth_clamp(false);
		result |= switch_framebuffer_sRGB(false);

		switch_texture_unit(0);
		glCullFace(GL_BACK);

		return result;
	}

	bool StateManager::unbind_mesh()
	{
		m_used_attributes.reset();

		if (m_mesh.get() != nullptr)
		{
			m_mesh->unbind();
			m_mesh.reset();

			return true;
		}

		return false;
	}

	bool StateManager::unbind_program()
	{
		if (m_program.get() != nullptr)
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
		if (m_textures[texture_unit].get() != nullptr)
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

	void StateManager::draw(const Uint32 index, const Uint32 instances)
	{
#if	0
#ifndef	NDEBUG
		if (m_program.get() != nullptr)
		{
			m_program->log_validate_error();
		}
#endif	/* NDEBUG */
/*
		LOG_DEBUG(lt_opengl, UTF8("shader used: %1%"), m_program->get_uuid());

		for (int i = 0; i < 16; ++i)
		{
			if (!m_program_used_texture_units[i])
			{
				continue;
			}

			String name;

			if (m_textures[i].get() != nullptr)
			{
				name = m_textures[i]->get_name();
			}
			else
			{
				name = UTF8("(nullptr)");
			}

			LOG_DEBUG(lt_opengl, UTF8("texture[%1%] used[%2%]: %3%"), i % m_used_texture_units[i] % name);
		}
*/
#endif
		m_mesh->draw(index, instances);
	}

	void StateManager::draw(const MeshDrawData &draw_data,
		const Uint32 instances)
	{
#if	0
#ifndef	NDEBUG
		if (m_program.get() != nullptr)
		{
			m_program->log_validate_error();
		}
#endif	/* NDEBUG */
/*
		LOG_DEBUG(lt_opengl, UTF8("shader used: %1%"), m_program->get_uuid());

		for (int i = 0; i < 16; ++i)
		{
			if (!m_program_used_texture_units[i])
			{
				continue;
			}

			String name;

			if (m_textures[i].get() != nullptr)
			{
				name = m_textures[i]->get_name();
			}
			else
			{
				name = UTF8("(nullptr)");
			}

			LOG_DEBUG(lt_opengl, UTF8("texture[%1%] used[%2%]: %3%"), i % m_used_texture_units[i] % name);
		}
*/
#endif
		m_mesh->draw(draw_data, instances, m_mesh->get_primitive());
	}

}
