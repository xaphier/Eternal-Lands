/****************************************************************************
 *            uniformbufferdescriptioncache.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_7742c2d6_814a_4240_bcfb_507888d8c200
#define	UUID_7742c2d6_814a_4240_bcfb_507888d8c200

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "shader/uniformbufferutil.hpp"

/**
 * @file
 * @brief The @c class UniformBufferDescriptionCache.
 * This file contains the @c class UniformBufferDescriptionCache.
 */
namespace eternal_lands
{

	class UniformBufferDescriptionCache: public boost::noncopyable
	{
		private:
			typedef std::map<UniformBufferType,
				UniformBufferDescriptionSharedPtr>
				UniformBufferDescriptionMap;
			UniformBufferDescriptionMap
				m_uniform_buffer_description_cache;

		public:
			/**
			 * Default constructor.
			 */
			UniformBufferDescriptionCache();

			/**
			 * Default destructor.
			 */
			~UniformBufferDescriptionCache() noexcept;

			const UniformBufferDescriptionSharedPtr
				&get_uniform_buffer_description(
					const UniformBufferType uniform_buffer);

	};

}

#endif	/* UUID_7742c2d6_814a_4240_bcfb_507888d8c200 */
