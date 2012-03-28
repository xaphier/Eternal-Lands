/****************************************************************************
 *            globalvars.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "globalvars.hpp"
#include "exceptions.hpp"
#include "simd/simd.hpp"

namespace eternal_lands
{

	GlobalVars::GlobalVars()
	{
		m_quality = String(UTF8("high"));
		m_shadow_distance = 40.0f;
		m_view_distance = 40.0f;
		m_shadow_quality = sqt_no;
		m_shadow_map_size = 2;
		m_opengl_version = ovt_2_1;
		m_fog = true;
		m_optmize_shader_source = true;
#ifdef	USE_SSE2
		m_use_simd = true;
#else	/* USE_SSE2 */
		m_use_simd = false;
#endif	/* USE_SSE2 */
		m_use_s3tc_for_actors = true;
		m_use_block = true;
		m_use_alias = false;
		m_use_in_out = true;
		m_use_functions = false;
		m_use_layered_rendering = false;
		m_low_quality_terrain = false;
#ifdef	DEBUG
		m_draw_objects = true;
		m_draw_actors = true;
#endif	/* DEBUG */
	}

	GlobalVars::~GlobalVars() throw()
	{
	}

}
