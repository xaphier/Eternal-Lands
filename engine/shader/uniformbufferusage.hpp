/****************************************************************************
 *            uniformbufferusage.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_f03201cd_d7ee_4d8b_9fc0_6865141a7091
#define	UUID_f03201cd_d7ee_4d8b_9fc0_6865141a7091

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "uniformbufferutil.hpp"

/**
 * @file
 * @brief The @c class UniformBufferUsage.
 * This file contains the @c class UniformBufferUsage.
 */
namespace eternal_lands
{

	class UniformBufferUsage
	{
		private:
			typedef std::set<UniformBufferType>
				UniformBufferTypeSet;

			UniformBufferTypeSet m_used_uniform_buffers;
			UniformBufferTypeSet m_usable_uniform_buffers;

		public:
			UniformBufferUsage();
			~UniformBufferUsage() noexcept;
			void write(const String &indent, OutStream &str) const;
			bool add_parameter(
				const ShaderSourceParameter &parameter);
			bool used_parameter(
				const ShaderSourceParameter &parameter) const;

			inline bool get_used_uniform_buffer(
				const UniformBufferType buffer) const noexcept
			{
				return m_used_uniform_buffers.count(buffer) > 0;
			}

			inline bool get_usable_uniform_buffer(
				const UniformBufferType buffer) const noexcept
			{
				return m_usable_uniform_buffers.count(buffer)
					> 0;
			}

			inline void set_usable_uniform_buffer(
				const UniformBufferType buffer,
				const bool enable) noexcept
			{
				m_usable_uniform_buffers.insert(buffer);
			}

			inline void clear() noexcept
			{
				m_used_uniform_buffers.clear();
				m_usable_uniform_buffers.clear();
			}

	};

}

#endif	/* UUID_f03201cd_d7ee_4d8b_9fc0_6865141a7091 */
