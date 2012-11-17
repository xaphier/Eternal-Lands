/****************************************************************************
 *            stateset.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "stateset.hpp"

namespace eternal_lands
{

	StateSet::StateSet(): m_color_mask(true),
		m_restart_index(std::numeric_limits<Uint32>::max()),
		m_texture_unit(0), m_multisample(false), m_blend(false),
		m_culling(false), m_depth_mask(false), m_depth_test(false),
		m_scissor_test(false), m_sample_alpha_to_coverage(false),
		m_use_restart_index(false), m_polygon_offset_fill(false),
		m_stencil_test(false), m_flip_back_face_culling(false),
		m_depth_clamp(false), m_framebuffer_sRGB(false)
	{
	}

	StateSet::~StateSet() noexcept
	{
	}

}
