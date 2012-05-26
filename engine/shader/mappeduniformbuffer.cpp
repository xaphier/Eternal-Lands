/****************************************************************************
 *            mappeduniformbuffer.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "mappeduniformbuffer.hpp"
#include "abstractwritememory.hpp"
#include "uniformdescription.hpp"
#include "uniformbufferdescription.hpp"
#include "uniformbufferdescriptioncache.hpp"

namespace eternal_lands
{

	namespace
	{

		template<typename T>
		void set_c_parameter(const UniformBufferDescriptionSharedPtr
				&uniform_buffer_description,
			const AutoParameterType parameter, const T value,
			const Uint32 index, void* ptr)
		{
			UniformDescription uniform_description;
			Uint64 ptr_offset;

			if (!uniform_buffer_description->
				get_uniform_description(parameter,
					uniform_description))
			{
				return;
			}

			if (index >= uniform_description.get_size())
			{
				return;
			}

			ptr_offset = uniform_description.get_offset();
			ptr_offset += uniform_description.get_array_stride() *
				index;

			memcpy(static_cast<Uint8*>(ptr) + ptr_offset, &value,
				sizeof(T));
		}

		template<typename T>
		void set_c_parameters(const UniformBufferDescriptionSharedPtr
				&uniform_buffer_description,
			const AutoParameterType parameter,
			const std::vector<T> &value, const Uint32 index,
			const Uint32 offset, void* ptr)
		{
			UniformDescription uniform_description;
			Uint64 ptr_offset;
			Uint32 size, i, count;

			if (offset >= value.size())
			{
				return;
			}

			if (!uniform_buffer_description->
				get_uniform_description(parameter,
					uniform_description))
			{
				return;
			}

			if (index >= uniform_description.get_size())
			{
				return;
			}

			ptr_offset = uniform_description.get_offset();
			ptr_offset += uniform_description.get_array_stride() *
				index;
			size = uniform_description.get_size() - index;
			count = value.size() - offset;

			count = std::min(size, count);

			if (uniform_description.get_compact())
			{
				memcpy(static_cast<Uint8*>(ptr) + ptr_offset,
					&value[offset], sizeof(T) * count);
				return;
			}

			for (i = 0; i < count; ++i)
			{
				memcpy(static_cast<Uint8*>(ptr) + ptr_offset,
					&value[offset + i], sizeof(T));

				ptr_offset += 
					uniform_description.get_array_stride();
			}
		}

		template<typename T>
		void set_glm_parameter(const UniformBufferDescriptionSharedPtr
				&uniform_buffer_description,
			const AutoParameterType parameter, const T value,
			const Uint32 index, void* ptr)
		{
			UniformDescription uniform_description;
			Uint64 ptr_offset;

			if (!uniform_buffer_description->
				get_uniform_description(parameter,
					uniform_description))
			{
				return;
			}

			if (index >= uniform_description.get_size())
			{
				return;
			}

			ptr_offset = uniform_description.get_offset();
			ptr_offset += uniform_description.get_array_stride() *
				index;

			memcpy(static_cast<Uint8*>(ptr) + ptr_offset,
				glm::value_ptr(value), sizeof(T));
		}

		template<typename T>
		void set_glm_parameters(const UniformBufferDescriptionSharedPtr
				&uniform_buffer_description,
			const AutoParameterType parameter,
			const std::vector<T> &value, const Uint32 index,
			const Uint32 offset, void* ptr)
		{
			UniformDescription uniform_description;
			Uint64 ptr_offset;
			Uint32 size, i, count;

			if (offset >= value.size())
			{
				return;
			}

			if (!uniform_buffer_description->
				get_uniform_description(parameter,
					uniform_description))
			{
				return;
			}

			if (index >= uniform_description.get_size())
			{
				return;
			}

			ptr_offset = uniform_description.get_offset();
			ptr_offset += uniform_description.get_array_stride() *
				index;
			size = uniform_description.get_size() - index;
			count = value.size() - offset;

			count = std::min(size, count);

			if (uniform_description.get_compact())
			{
				memcpy(static_cast<Uint8*>(ptr) + ptr_offset,
					glm::value_ptr(value[offset]),
					sizeof(T) * count);
				return;
			}

			for (i = 0; i < count; ++i)
			{
				memcpy(static_cast<Uint8*>(ptr) + ptr_offset,
					glm::value_ptr(value[offset + i]),
					sizeof(T));

				ptr_offset += 
					uniform_description.get_array_stride();
			}
		}

		template<typename T>
		void set_glm_matrix_parameter(
			const UniformBufferDescriptionSharedPtr
				&uniform_buffer_description,
			const AutoParameterType parameter, const T value,
			const Uint32 index, void* ptr)
		{
			UniformDescription uniform_description;
			Uint64 ptr_offset;
			Uint32 i;

			if (!uniform_buffer_description->
				get_uniform_description(parameter,
					uniform_description))
			{
				return;
			}

			if (index >= uniform_description.get_size())
			{
				return;
			}

			ptr_offset = uniform_description.get_offset();
			ptr_offset += uniform_description.get_array_stride() *
				index;

			if (uniform_description.get_compact())
			{
				memcpy(static_cast<Uint8*>(ptr) + ptr_offset,
					glm::value_ptr(value), sizeof(T));
				return;
			}

			for (i = 0; i < T::row_size(); ++i)
			{
				memcpy(static_cast<Uint8*>(ptr) + ptr_offset,
					glm::value_ptr(value[i]),
					sizeof(typename T::col_type));

				ptr_offset +=
					uniform_description.get_matrix_stride();
			}
		}

		template<typename T>
		void set_glm_matrix_parameters(
			const UniformBufferDescriptionSharedPtr
				&uniform_buffer_description,
			const AutoParameterType parameter,
			const std::vector<T> &value, const Uint32 index,
			const Uint32 offset, void* ptr)
		{
			UniformDescription uniform_description;
			Uint64 ptr_offset;
			Uint32 size, i, j, count;

			if (offset >= value.size())
			{
				return;
			}

			if (!uniform_buffer_description->
				get_uniform_description(parameter,
					uniform_description))
			{
				return;
			}

			if (index >= uniform_description.get_size())
			{
				return;
			}

			ptr_offset = uniform_description.get_offset();
			ptr_offset += uniform_description.get_array_stride() *
				index;
			size = uniform_description.get_size() - index;
			count = value.size() - offset;

			count = std::min(size, count);

			if (uniform_description.get_compact())
			{
				memcpy(static_cast<Uint8*>(ptr) + ptr_offset,
					glm::value_ptr(value[offset]),
					sizeof(T) * count);
				return;
			}

			for (i = 0; i < count; ++i)
			{
				ptr_offset = uniform_description.get_offset();
				ptr_offset += uniform_description.
					get_array_stride() * (index + i);

				for (j = 0; j < T::row_size(); ++j)
				{
					memcpy(static_cast<Uint8*>(ptr) +
						offset, glm::value_ptr(
							value[i][j]),
							sizeof(typename
								T::col_type));
					ptr_offset += uniform_description.
						get_matrix_stride();
				}
			}
		}

	}

	MappedUniformBuffer::MappedUniformBuffer(
		const AbstractWriteMemorySharedPtr &memory,
		const UniformBufferDescriptionSharedPtr
			&uniform_buffer_description): m_memory(memory),
		m_uniform_buffer_description(uniform_buffer_description)
	{
	}

	MappedUniformBuffer::~MappedUniformBuffer() noexcept
	{
	}

	void MappedUniformBuffer::set_parameter(
		const AutoParameterType parameter, const bool value,
		const Uint32 index)
	{
	}

	void MappedUniformBuffer::set_parameter(
		const AutoParameterType parameter, const glm::bvec2 &value,
		const Uint32 index)
	{
	}

	void MappedUniformBuffer::set_parameter(
		const AutoParameterType parameter, const glm::bvec3 &value,
		const Uint32 index)
	{
	}

	void MappedUniformBuffer::set_parameter(
		const AutoParameterType parameter, const glm::bvec4 &value,
		const Uint32 index)
	{
	}

	void MappedUniformBuffer::set_parameter(
		const AutoParameterType parameter, const Sint32 value,
		const Uint32 index)
	{
		set_c_parameter(m_uniform_buffer_description,
			parameter, value, index, m_memory->get_ptr());
	}
	
	void MappedUniformBuffer::set_parameter(
		const AutoParameterType parameter, const glm::ivec2 &value,
		const Uint32 index)
	{
		set_glm_parameter(m_uniform_buffer_description,
			parameter, value, index, m_memory->get_ptr());
	}

	void MappedUniformBuffer::set_parameter(
		const AutoParameterType parameter, const glm::ivec3 &value,
		const Uint32 index)
	{
		set_glm_parameter(m_uniform_buffer_description,
			parameter, value, index, m_memory->get_ptr());
	}

	void MappedUniformBuffer::set_parameter(
		const AutoParameterType parameter, const glm::ivec4 &value,
		const Uint32 index)
	{
		set_glm_parameter(m_uniform_buffer_description,
			parameter, value, index, m_memory->get_ptr());
	}

	void MappedUniformBuffer::set_parameter(
		const AutoParameterType parameter, const Ivec4Vector &value,
		const Uint32 offset)
	{
		set_glm_parameters(m_uniform_buffer_description,
			parameter, value, 0, offset, m_memory->get_ptr());
	}

	void MappedUniformBuffer::set_parameter(
		const AutoParameterType parameter, const Uint32 value,
		const Uint32 index)
	{
		set_c_parameter(m_uniform_buffer_description,
			parameter, value, index, m_memory->get_ptr());
	}

	void MappedUniformBuffer::set_parameter(
		const AutoParameterType parameter, const glm::uvec2 &value,
		const Uint32 index)
	{
		set_glm_parameter(m_uniform_buffer_description,
			parameter, value, index, m_memory->get_ptr());
	}

	void MappedUniformBuffer::set_parameter(
		const AutoParameterType parameter, const glm::uvec3 &value,
		const Uint32 index)
	{
		set_glm_parameter(m_uniform_buffer_description,
			parameter, value, index, m_memory->get_ptr());
	}

	void MappedUniformBuffer::set_parameter(
		const AutoParameterType parameter, const glm::uvec4 &value,
		const Uint32 index)
	{
		set_glm_parameter(m_uniform_buffer_description,
			parameter, value, index, m_memory->get_ptr());
	}

	void MappedUniformBuffer::set_parameter(
		const AutoParameterType parameter, const Uvec4Vector &value,
		const Uint32 offset)
	{
		set_glm_parameters(m_uniform_buffer_description,
			parameter, value, 0, offset, m_memory->get_ptr());
	}

	void MappedUniformBuffer::set_parameter(
		const AutoParameterType parameter, const float value,
		const Uint32 index)
	{
		set_c_parameter(m_uniform_buffer_description,
			parameter, value, index, m_memory->get_ptr());
	}

	void MappedUniformBuffer::set_parameter(
		const AutoParameterType parameter, const Vec4Vector &value,
		const Uint32 offset)
	{
		set_glm_parameters(m_uniform_buffer_description,
			parameter, value, 0, offset, m_memory->get_ptr());
	}

	void MappedUniformBuffer::set_parameter(
		const AutoParameterType parameter, const glm::vec2 &value,
		const Uint32 index)
	{
		set_glm_parameter(m_uniform_buffer_description,
			parameter, value, index, m_memory->get_ptr());
	}

	void MappedUniformBuffer::set_parameter(
		const AutoParameterType parameter, const glm::vec3 &value,
		const Uint32 index)
	{
		set_glm_parameter(m_uniform_buffer_description,
			parameter, value, index, m_memory->get_ptr());
	}

	void MappedUniformBuffer::set_parameter(
		const AutoParameterType parameter, const glm::vec4 &value,
		const Uint32 index)
	{
		set_glm_parameter(m_uniform_buffer_description,
			parameter, value, index, m_memory->get_ptr());
	}

	void MappedUniformBuffer::set_parameter(
		const AutoParameterType parameter, const Mat2x4Vector &value,
		const Uint32 offset)
	{
		set_glm_matrix_parameters(m_uniform_buffer_description,
			parameter, value, 0, offset, m_memory->get_ptr());
	}

	void MappedUniformBuffer::set_parameter(
		const AutoParameterType parameter, const Mat3x4Vector &value,
		const Uint32 offset)
	{
		set_glm_matrix_parameters(m_uniform_buffer_description,
			parameter, value, 0, offset, m_memory->get_ptr());
	}

	void MappedUniformBuffer::set_parameter(
		const AutoParameterType parameter, const Mat4x4Vector &value,
		const Uint32 offset)
	{
		set_glm_matrix_parameters(m_uniform_buffer_description,
			parameter, value, 0, offset, m_memory->get_ptr());
	}

	void MappedUniformBuffer::set_parameter(
		const AutoParameterType parameter, const glm::mat2x2 &value,
		const Uint32 index)
	{
		set_glm_matrix_parameter(m_uniform_buffer_description,
			parameter, value, index, m_memory->get_ptr());
	}

	void MappedUniformBuffer::set_parameter(
		const AutoParameterType parameter, const glm::mat2x3 &value,
		const Uint32 index)
	{
		set_glm_matrix_parameter(m_uniform_buffer_description,
			parameter, value, index, m_memory->get_ptr());
	}

	void MappedUniformBuffer::set_parameter(
		const AutoParameterType parameter, const glm::mat2x4 &value,
		const Uint32 index)
	{
		set_glm_matrix_parameter(m_uniform_buffer_description,
			parameter, value, index, m_memory->get_ptr());
	}

	void MappedUniformBuffer::set_parameter(
		const AutoParameterType parameter, const glm::mat3x2 &value,
		const Uint32 index)
	{
		set_glm_matrix_parameter(m_uniform_buffer_description,
			parameter, value, index, m_memory->get_ptr());
	}

	void MappedUniformBuffer::set_parameter(
		const AutoParameterType parameter, const glm::mat3x3 &value,
		const Uint32 index)
	{
		set_glm_matrix_parameter(m_uniform_buffer_description,
			parameter, value, index, m_memory->get_ptr());
	}

	void MappedUniformBuffer::set_parameter(
		const AutoParameterType parameter, const glm::mat3x4 &value,
		const Uint32 index)
	{
		set_glm_matrix_parameter(m_uniform_buffer_description,
			parameter, value, index, m_memory->get_ptr());
	}

	void MappedUniformBuffer::set_parameter(
		const AutoParameterType parameter, const glm::mat4x2 &value,
		const Uint32 index)
	{
		set_glm_matrix_parameter(m_uniform_buffer_description,
			parameter, value, index, m_memory->get_ptr());
	}

	void MappedUniformBuffer::set_parameter(
		const AutoParameterType parameter, const glm::mat4x3 &value,
		const Uint32 index)
	{
		set_glm_matrix_parameter(m_uniform_buffer_description,
			parameter, value, index, m_memory->get_ptr());
	}

	void MappedUniformBuffer::set_parameter(
		const AutoParameterType parameter, const glm::mat4x4 &value,
		const Uint32 index)
	{
		set_glm_matrix_parameter(m_uniform_buffer_description,
			parameter, value, index, m_memory->get_ptr());
	}

}
