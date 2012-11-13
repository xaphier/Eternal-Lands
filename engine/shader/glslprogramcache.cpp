/****************************************************************************
 *            glslprogramcache.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "glslprogramcache.hpp"
#include "glslprogram.hpp"
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

	String GlslProgramCache::get_index(
		const ShaderTypeStringMap &description) const
	{
		StringStream index, result;
		ShaderTypeStringMap::const_iterator found, end;
		std::size_t hash;
		Uint32 i, count;

		end = description.end();
		hash = 0;
		count = ShaderUtil::get_shader_count();

		for (i = 0; i < count; ++i)
		{
			found = description.find(static_cast<ShaderType>(i));

			if (found != end)
			{
				boost::hash_combine(hash, found->second.get());
				index << found->second;
			}
		}

		result << hash << index.str();

		return String(result.str());
	}

	const GlslProgramSharedPtr &GlslProgramCache::get_program(
		const ShaderTypeStringMap &description)
	{
		GlslProgramCacheMap::iterator found;
		GlslProgramSharedPtr glsl_program;
		String index;

		index = get_index(description);

		found = m_glsl_program_cache.find(index);

		if (found != m_glsl_program_cache.end())
		{
			assert(found->second.get() != nullptr);

			return found->second;
		}

		glsl_program = boost::make_shared<GlslProgram>(
			get_uniform_buffer_description_cache(), description,
			boost::uuids::random_generator()());

		m_glsl_program_cache[index] = glsl_program;

		return m_glsl_program_cache[index];
	}

}
