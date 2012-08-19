/****************************************************************************
 *            alignedvec4array.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_fcd96f88_a2ed_4cb5_8bd8_e293ca03c69f
#define	UUID_fcd96f88_a2ed_4cb5_8bd8_e293ca03c69f

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "exceptions.hpp"

/**
 * @file
 * @brief The @c class AlignedVec4Array.
 * This file contains the @c class AlignedVec4Array.
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
			AlignedVec4Array();
			AlignedVec4Array(const AlignedVec4Array &array);
			~AlignedVec4Array() noexcept;
			AlignedVec4Array &operator=(
				const AlignedVec4Array &array);
			void clear();
			void reserve(const Uint32 new_capacity);

			inline void* get_ptr() noexcept
			{
				return m_data;
			}

			inline const void* get_ptr() const noexcept
			{
				return m_data;
			}

			inline float* get_ptr_at(const Uint32 index)
			{
				assert(index < m_size);
				return glm::value_ptr(m_data[index]);
			}

			inline const float* get_ptr_at(const Uint32 index) const
			{
				assert(index < m_size);
				return glm::value_ptr(m_data[index]);
			}

			inline Uint32 size() const noexcept
			{
				return m_size;
			}

			inline Uint32 capacity() const noexcept
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

	};

}

#endif	/* UUID_fcd96f88_a2ed_4cb5_8bd8_e293ca03c69f */
