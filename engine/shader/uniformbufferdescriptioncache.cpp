/****************************************************************************
 *            uniformbufferdescriptioncache.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "uniformbufferdescriptioncache.hpp"
#include "shader/uniformbufferdescription.hpp"

namespace eternal_lands
{

	UniformBufferDescriptionCache::UniformBufferDescriptionCache()
	{
	}

	UniformBufferDescriptionCache::~UniformBufferDescriptionCache()
		noexcept
	{
	}

	const UniformBufferDescriptionSharedPtr
		&UniformBufferDescriptionCache::get_uniform_buffer_description(
			const UniformBufferType uniform_buffer)
	{
		UniformBufferDescriptionMap::iterator found;
		UniformBufferDescriptionSharedPtr uniform_buffer_description;

		found = m_uniform_buffer_description_cache.find(uniform_buffer);

		if (found != m_uniform_buffer_description_cache.end())
		{
			assert(found->second.get() != nullptr);

			return found->second;
		}

		uniform_buffer_description = boost::make_shared<
			UniformBufferDescription>(uniform_buffer);

		m_uniform_buffer_description_cache[uniform_buffer] =
			uniform_buffer_description;

		return m_uniform_buffer_description_cache[uniform_buffer];
	}

}
