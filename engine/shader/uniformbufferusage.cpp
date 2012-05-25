/****************************************************************************
 *            uniformbufferusage.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "uniformbufferusage.hpp"
#include "autoparameterutil.hpp"
#include "uniformbufferdescription.hpp"
#include "uniformbufferdescriptioncache.hpp"
#include "shadersourceparameter.hpp"
#include "exceptions.hpp"

namespace eternal_lands
{

	UniformBufferUsage::UniformBufferUsage()
	{
	}

	UniformBufferUsage::UniformBufferUsage(
		const UniformBufferDescriptionCacheSharedPtr
			&uniform_buffer_description_cache)
	{
#if	0
		Uint32 i, count;
		UniformBufferType type;

		count = UniformBufferUtil::get_uniform_buffer_count();

		for (i = 0; i < count; ++i)
		{
			type = static_cast<UniformBufferType>(i);

			m_usable_uniform_buffers.push_back(
				uniform_buffer_description_cache->
					get_uniform_buffer_description(type));
		}
#else
		m_usable_uniform_buffers.push_back(
			uniform_buffer_description_cache->
				get_uniform_buffer_description(
					ubt_terrain_instances));
#endif
	}

	UniformBufferUsage::~UniformBufferUsage() noexcept
	{
	}

	void UniformBufferUsage::write(const String &indent, OutStream &str)
		const
	{
		BOOST_FOREACH(const UniformBufferDescriptionSharedPtr
			&uniform_buffer_description, m_usable_uniform_buffers)
		{
			if (m_used_uniform_buffers.count(
				uniform_buffer_description->get_type()) > 0)
			{
				uniform_buffer_description->write(indent, str);
			}
		}
	}

	bool UniformBufferUsage::add_parameter(
		const ShaderSourceParameter &parameter)
	{
		AutoParameterType auto_parameter;

		if (!parameter.get_auto_parameter(auto_parameter))
		{
			return false;
		}

		BOOST_FOREACH(const UniformBufferDescriptionSharedPtr
			&uniform_buffer_description, m_usable_uniform_buffers)
		{
			if (uniform_buffer_description->has_auto_parameter(
				auto_parameter))
			{
				m_used_uniform_buffers.insert(
					uniform_buffer_description->get_type());

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
