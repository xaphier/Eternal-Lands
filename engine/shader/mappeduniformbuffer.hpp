/****************************************************************************
 *            mappeduniformbuffer.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_c9aad7c2_7e85_4f33_bfc7_8322111c9c04
#define	UUID_c9aad7c2_7e85_4f33_bfc7_8322111c9c04

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "hardwarebuffer/hardwarebuffer.hpp"
#include "uniformbufferutil.hpp"

/**
 * @file
 * @brief The @c class UniformBuffer.
 * This file contains the @c class UniformBuffer.
 */
namespace eternal_lands
{

	class MappedUniformBuffer
	{
		private:
			const AbstractWriteMemorySharedPtr m_memory;
			const UniformBufferDescriptionSharedPtr
				m_uniform_buffer_description;

		public:
			MappedUniformBuffer(
				const AbstractWriteMemorySharedPtr &memory,
				const UniformBufferDescriptionSharedPtr
					&uniform_buffer_description);
			~MappedUniformBuffer() noexcept;

			inline const AbstractWriteMemorySharedPtr &get_memory()
				const
			{
				return m_memory;
			}

			void set_parameter(const AutoParameterType parameter,
				const bool value, const Uint32 index = 0);
			void set_parameter(const AutoParameterType parameter,
				const glm::bvec2 &value,
				const Uint32 index = 0);
			void set_parameter(const AutoParameterType parameter,
				const glm::bvec3 &value,
				const Uint32 index = 0);
			void set_parameter(const AutoParameterType parameter,
				const glm::bvec4 &value,
				const Uint32 index = 0);
			void set_parameter(const AutoParameterType parameter,
				const Sint32 value, const Uint32 index = 0);
			void set_parameter(const AutoParameterType parameter,
				const glm::ivec2 &value,
				const Uint32 index = 0);
			void set_parameter(const AutoParameterType parameter,
				const glm::ivec3 &value,
				const Uint32 index = 0);
			void set_parameter(const AutoParameterType parameter,
				const glm::ivec4 &value,
				const Uint32 index = 0);
			void set_parameter(const AutoParameterType parameter,
				const Ivec4Vector &value,
				const Uint32 offset = 0);
			void set_parameter(const AutoParameterType parameter,
				const Uint32 value, const Uint32 index = 0);
			void set_parameter(const AutoParameterType parameter,
				const glm::uvec2 &value,
				const Uint32 index = 0);
			void set_parameter(const AutoParameterType parameter,
				const glm::uvec3 &value,
				const Uint32 index = 0);
			void set_parameter(const AutoParameterType parameter,
				const glm::uvec4 &value,
				const Uint32 index = 0);
			void set_parameter(const AutoParameterType parameter,
				const Uvec4Vector &value,
				const Uint32 offset = 0);
			void set_parameter(const AutoParameterType parameter,
				const float value, const Uint32 index = 0);
			void set_parameter(const AutoParameterType parameter,
				const Vec4Vector &value,
				const Uint32 offset = 0);
			void set_parameter(const AutoParameterType parameter,
				const glm::vec2 &value,
				const Uint32 index = 0);
			void set_parameter(const AutoParameterType parameter,
				const glm::vec3 &value,
				const Uint32 index = 0);
			void set_parameter(const AutoParameterType parameter,
				const glm::vec4 &value,
				const Uint32 index = 0);
			void set_parameter(const AutoParameterType parameter,
				const Mat2x4Vector &value,
				const Uint32 offset = 0);
			void set_parameter(const AutoParameterType parameter,
				const Mat3x4Vector &value,
				const Uint32 offset = 0);
			void set_parameter(const AutoParameterType parameter,
				const Mat4x4Vector &value,
				const Uint32 offset = 0);
			void set_parameter(const AutoParameterType parameter,
				const glm::mat2x2 &value,
				const Uint32 index = 0);
			void set_parameter(const AutoParameterType parameter,
				const glm::mat2x3 &value,
				const Uint32 index = 0);
			void set_parameter(const AutoParameterType parameter,
				const glm::mat2x4 &value,
				const Uint32 index = 0);
			void set_parameter(const AutoParameterType parameter,
				const glm::mat3x2 &value,
				const Uint32 index = 0);
			void set_parameter(const AutoParameterType parameter,
				const glm::mat3x3 &value,
				const Uint32 index = 0);
			void set_parameter(const AutoParameterType parameter,
				const glm::mat3x4 &value,
				const Uint32 index = 0);
			void set_parameter(const AutoParameterType parameter,
				const glm::mat4x2 &value,
				const Uint32 index = 0);
			void set_parameter(const AutoParameterType parameter,
				const glm::mat4x3 &value,
				const Uint32 index = 0);
			void set_parameter(const AutoParameterType parameter,
				const glm::mat4x4 &value,
				const Uint32 index = 0);

	};

}

#endif	/* UUID_c9aad7c2_7e85_4f33_bfc7_8322111c9c04 */
