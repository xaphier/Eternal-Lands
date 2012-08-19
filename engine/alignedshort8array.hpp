/****************************************************************************
 *            alignedshort8array.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_7e219c00_fd68_47f9_bf5e_2aa9e8611ffd
#define	UUID_7e219c00_fd68_47f9_bf5e_2aa9e8611ffd

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "exceptions.hpp"

/**
 * @file
 * @brief The @c class AlignedShort8Array.
 * This file contains the @c class AlignedShort8Array.
 */
namespace eternal_lands
{

	class AlignedShort8Array
	{
		private:
			Sint16* m_data;
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
			AlignedShort8Array();
			AlignedShort8Array(const AlignedShort8Array &array);
			~AlignedShort8Array() noexcept;
			AlignedShort8Array &operator=(
				const AlignedShort8Array &array);
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

			inline Sint16* get_ptr_at(const Uint32 index)
			{
				assert(index < m_size);
				return &(m_data[index * 8]);
			}

			inline const Sint16* get_ptr_at(const Uint32 index)
				const
			{
				assert(index < m_size);
				return &(m_data[index * 8]);
			}

			inline Uint32 size() const noexcept
			{
				return m_size;
			}

			inline Uint32 capacity() const noexcept
			{
				return m_capacity;
			}

			inline glm::ivec4 get_value_low(const Uint32 index)
				const
			{
				assert(index < size());

				return glm::ivec4(m_data[index * 8 + 0],
					m_data[index * 8 + 1],
					m_data[index * 8 + 2],
					m_data[index * 8 + 3]);
			}

			inline glm::ivec4 get_value_high(const Uint32 index)
				const
			{
				assert(index < size());

				return glm::ivec4(m_data[index * 8 + 4],
					m_data[index * 8 + 5],
					m_data[index * 8 + 6],
					m_data[index * 8 + 7]);
			}

			inline void set_value_low(const glm::ivec4 &value,
				const Uint32 index) const
			{
				assert(index < size());

				m_data[index * 8 + 0] = value[0];
				m_data[index * 8 + 1] = value[1];
				m_data[index * 8 + 2] = value[2];
				m_data[index * 8 + 3] = value[3];
			}

			inline void set_value_high(const glm::ivec4 &value,
				const Uint32 index) const
			{
				assert(index < size());

				m_data[index * 8 + 4] = value[0];
				m_data[index * 8 + 5] = value[1];
				m_data[index * 8 + 6] = value[2];
				m_data[index * 8 + 7] = value[3];
			}

			inline void pop_back()
			{
				assert(m_size > 0);

				m_size--;
			}

			inline void push_back(const glm::ivec4 &v0,
				const glm::ivec4 &v1)
			{
				if (size() == capacity())
				{
					reserve(alloc_size());
				}

				m_data[size() * 8 + 0] = v0[0];
				m_data[size() * 8 + 1] = v0[1];
				m_data[size() * 8 + 2] = v0[2];
				m_data[size() * 8 + 3] = v0[3];
				m_data[size() * 8 + 4] = v1[0];
				m_data[size() * 8 + 5] = v1[1];
				m_data[size() * 8 + 6] = v1[2];
				m_data[size() * 8 + 7] = v1[3];

				m_size++;
			}

			/**
			 * resize changes the number of elements in the array.
			 */
			inline void resize(const Uint32 new_size)
			{
				Uint32 i;

				if (new_size > size())
				{
					reserve(new_size);
				}

				for (i = size(); i < new_size; ++i)
				{
					m_data[i * 8 + 0] = 0;
					m_data[i * 8 + 1] = 0;
					m_data[i * 8 + 2] = 0;
					m_data[i * 8 + 3] = 0;
					m_data[i * 8 + 4] = 0;
					m_data[i * 8 + 5] = 0;
					m_data[i * 8 + 6] = 0;
					m_data[i * 8 + 7] = 0;
				}

				m_size = new_size;
			}

	};

}

#endif	/* UUID_7e219c00_fd68_47f9_bf5e_2aa9e8611ffd */
