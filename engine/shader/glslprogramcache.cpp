/****************************************************************************
 *            glslprogramcache.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "glslprogramcache.hpp"
#include "glslprogram.hpp"
#include "glslprogramdescription.hpp"
#include "xmlreader.hpp"
#include "xmlutil.hpp"
#include "xmlwriter.hpp"

namespace eternal_lands
{

	GlslProgramCache::GlslProgramCache(
		const UniformBufferDescriptionCacheWeakPtr
			&uniform_buffer_description_cache):
		m_uniform_buffer_description_cache(
			uniform_buffer_description_cache),
		m_uuid_generator(&m_ran)

	{
		assert(!m_uniform_buffer_description_cache.expired());

		m_ran.seed(time(0));
	}

	GlslProgramCache::~GlslProgramCache() noexcept
	{
	}

	const GlslProgramSharedPtr &GlslProgramCache::get_program(
		const GlslProgramDescription &description)
	{
		GlslProgramCacheMap::iterator found;
		GlslProgramSharedPtr glsl_program;

		found = m_glsl_program_cache.find(description);

		if (found != m_glsl_program_cache.end())
		{
			assert(found->second.get() != nullptr);

			return found->second;
		}

		glsl_program = boost::make_shared<GlslProgram>(
			get_uniform_buffer_description_cache(), description,
			boost::uuids::random_generator()());

		m_glsl_program_cache[description] = glsl_program;

		return m_glsl_program_cache[description];
	}

}
