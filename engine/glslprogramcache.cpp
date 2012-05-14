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

	GlslProgramCache::GlslProgramCache()
	{
	}

	GlslProgramCache::~GlslProgramCache() noexcept
	{
	}

	const GlslProgramSharedPtr &GlslProgramCache::get_program(
		const GlslProgramDescription &description)
	{
		GlslProgramCacheMap::iterator found;
		GlslProgramSharedPtr glsl_program;
		boost::uuids::uuid uuid;

		found = m_glsl_program_cache.find(description);

		if (found != m_glsl_program_cache.end())
		{
			assert(found->second.get() != nullptr);

			return found->second;
		}

		glsl_program = boost::make_shared<GlslProgram>(description,
			uuid);

		m_glsl_program_cache[description] = glsl_program;

		return m_glsl_program_cache[description];
	}

}
