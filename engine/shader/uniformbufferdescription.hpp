/****************************************************************************
 *            uniformbufferdescription.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_687c6606_8efe_4027_bef5_8082304c7bee
#define	UUID_687c6606_8efe_4027_bef5_8082304c7bee

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "uniformbufferutil.hpp"

/**
 * @file
 * @brief The @c class UniformBufferDescription.
 * This file contains the @c class UniformBufferDescription.
 */
namespace eternal_lands
{

	class UniformBufferDescription
	{
		private:
			UniformDescriptionVector m_uniforms;
			UniformBufferType m_type;
			Uint32 m_size;

		public:
			UniformBufferDescription();
			~UniformBufferDescription() noexcept;

			inline UniformBufferType get_type() const noexcept
			{
				return m_type;
			}

			inline const String &get_name() const
			{
				return UniformBufferUtil::get_str(get_type());
			}

			inline Uint32 get_size() const noexcept
			{
				return m_size;
			}

			inline void set_type(const UniformBufferType type)
				noexcept
			{
				m_type = type;
			}

			inline void set_size(const Uint32 size) noexcept
			{
				m_size = size;
			}

	};

}

#endif	/* UUID_687c6606_8efe_4027_bef5_8082304c7bee */
