/****************************************************************************
 *            glslprogramcache.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_3c61286e_d616_4b86_bde2_2acd44549f39
#define	UUID_3c61286e_d616_4b86_bde2_2acd44549f39

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "shaderutil.hpp"

/**
 * @file
 * @brief The @c class GlslProgramCache.
 * This file contains the @c class GlslProgramCache.
 */
namespace eternal_lands
{

	/**
	 * @brief @c class for glsl program cache.
	 *
	 * @c class for glsl program cache.
	 */
	class GlslProgramCache
	{
		private:
			typedef std::map<String, GlslProgramSharedPtr>
				GlslProgramCacheMap;

			const UniformBufferDescriptionCacheWeakPtr
				m_uniform_buffer_description_cache;
			GlslProgramCacheMap m_glsl_program_cache;
			boost::mt19937 m_ran;
			Mt19937RandomUuidGenerator m_uuid_generator;

			inline UniformBufferDescriptionCacheSharedPtr
				get_uniform_buffer_description_cache() const
				noexcept
			{
				UniformBufferDescriptionCacheSharedPtr result;

				result = m_uniform_buffer_description_cache.
					lock();

				assert(result.get() != nullptr);

				return result;
			}

			String get_index(	
				const ShaderTypeStringMap &description) const;

		public:
			/**
			 * Default constructor.
			 */
			GlslProgramCache(
				const UniformBufferDescriptionCacheWeakPtr
					&uniform_buffer_description_cache);

			/**
			 * Default destructor.
			 */
			~GlslProgramCache() noexcept;

			const GlslProgramSharedPtr &get_program(
				const ShaderTypeStringMap &description);

	};

}

#endif	/* UUID_3c61286e_d616_4b86_bde2_2acd44549f39 */
