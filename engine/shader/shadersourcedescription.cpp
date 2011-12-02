/****************************************************************************
 *            shadersourcedescription.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "shadersourcedescription.hpp"

namespace eternal_lands
{

	ShaderSourceDescription::ShaderSourceDescription(
		const ShaderSourceTypeStringMap &sources,
		const ShaderBuildType shader_build_type,
		const Uint16 light_count): m_sources(sources),
		m_shader_build_type(shader_build_type),
		m_light_count(light_count)
	{
	}

	ShaderSourceDescription::~ShaderSourceDescription() throw()
	{
	}

}
