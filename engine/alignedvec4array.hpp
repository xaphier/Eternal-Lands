/****************************************************************************
 *            alignedvec4array.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_fcd96f88_a2ed_4cb5_8bd8_e293ca03c69f
#define	UUID_fcd96f88_a2ed_4cb5_8bd8_e293ca03c69f

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "exceptions.hpp"
#include <mm_malloc.h>

/**
 * @file
 * @brief The @c class AlignedObjectArray.
 * This file contains the @c class AlignedObjectArray.
 */
namespace eternal_lands
{

	class AlignedVec4Array
	{
		private:
			glm::vec4* m_data;
			Uint32 m_size;
			Uint32 m_capacity;

			inline Uint32 alloc_size() const
			{
				if (capacity() == 0)
				{
					return 1;
				}

				return capacity() * 2;
			}

		public:
			inline AlignedVec4Array(): m_data(0), m_size(0),
				m_capacity(0)
			{
			}

			inline AlignedVec4Array(const AlignedVec4Array &array):
				m_data(0), m_size(0), m_capacity(0)
			{
				resize(array.m_size);
				memcpy(m_data, array.m_data, size() * 4 *
					sizeof(float));
			}

			inline ~AlignedVec4Array()
			{
				clear();
			}

			inline AlignedVec4Array &operator=(
				const AlignedVec4Array &array)
			{
				resize(array.m_size);

				memcpy(m_data, array.m_data, size() * 4 *
					sizeof(float));

				return *this;
			}

			inline void* get_ptr() const
			{
				return m_data;
			}

			inline Uint32 size() const
			{
				return m_size;
			}

			inline Uint32 capacity() const
			{
				return m_capacity;
			}

			inline const glm::vec4 &at(const Uint32 index) const
			{
				assert(index < size());
				return m_data[index];
			}

			inline glm::vec4 &at(const Uint32 index)
			{
				assert(index < size());
				return m_data[index];
			}

			inline const glm::vec4 &operator[](const Uint32 index)
				const
			{
				assert(index < size());
				return m_data[index];
			}

			inline glm::vec4 &operator[](const Uint32 index)
			{
				assert(index < size());
				return m_data[index];
			}

			inline void clear()
			{
				m_size = 0;
				m_capacity = 0;

				if (m_data != 0)
				{
					_mm_free(m_data);
					m_data = 0;
				}
			}

			inline void pop_back()
			{
				assert(m_size > 0);

				m_size--;
			}

			inline void push_back(const glm::vec4 &value)
			{
				if (size() == capacity())
				{
					reserve(alloc_size());
				}

				m_data[size()] = value;

				m_size++;
			}

			/**
			 * resize changes the number of elements in the array.
			 */
			inline void resize(const Uint32 new_size,
				const glm::vec4 &fill_value = glm::vec4())
			{
				Uint32 i;

				if (new_size > size())
				{
					reserve(new_size);
				}

				for (i = size(); i < new_size; ++i)
				{
					m_data[i] = fill_value;
				}

				m_size = new_size;
			}

			inline void reserve(const Uint32 new_capacity)
			{
				glm::vec4* data;
				Uint32 count;

				if (capacity() >= new_capacity)
				{
					return;
				}

				count = new_capacity * 4 * sizeof(float);

				data = static_cast<glm::vec4*>(
					_mm_malloc(count, 16));

				if (data == 0)
				{
					EL_THROW_EXCEPTION(BadAllocException());
				}

				count = std::min(new_capacity, size()) * 4 *
					sizeof(float);

				std::swap(m_data, data);

				if ((count > 0) && (data != 0))
				{
					memcpy(m_data, data, count);
				}

				if (data != 0)
				{
					_mm_free(data);
				}

				m_capacity = new_capacity;
			}

	};

}

#endif	/* UUID_fcd96f88_a2ed_4cb5_8bd8_e293ca03c69f */
