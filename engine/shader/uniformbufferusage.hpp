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
			bool add_parameter(
				const ShaderSourceParameter &parameter);
			bool used_parameter(
				const ShaderSourceParameter &parameter) const;

	};

}

#endif	/* UUID_f03201cd_d7ee_4d8b_9fc0_6865141a7091 */
