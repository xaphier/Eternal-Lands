/****************************************************************************
 *            uniformbufferusage.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "uniformbufferusage.hpp"
#include "autoparameterutil.hpp"
#include "terraininstancesbuffer.hpp"
#include "shadersourceparameter.hpp"
#include "exceptions.hpp"

namespace eternal_lands
{

	UniformBufferUsage::UniformBufferUsage()
	{
	}

	UniformBufferUsage::~UniformBufferUsage() noexcept
	{
	}

	bool UniformBufferUsage::add_parameter(
		const ShaderSourceParameter &parameter)
	{
		AutoParameterType auto_parameter;

		if (!parameter.get_auto_parameter(auto_parameter))
		{
			return false;
		}

		if (m_usable_uniform_buffers.count(ubt_terrain_instances) > 0)
		{
			if (TerrainInstancesBuffer::has_auto_parameter(
				auto_parameter))
			{
				m_used_uniform_buffers.insert(
					ubt_terrain_instances);

				return true;
			}
		}

		return false;
	}

	bool UniformBufferUsage::used_parameter(
		const ShaderSourceParameter &parameter) const
	{
		return false;
	}

}
