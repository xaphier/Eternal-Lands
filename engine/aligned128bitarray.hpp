/****************************************************************************
 *            aligned128bitarray.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_987ac961_bb89_4694_9a0f_c40ffbc5d01b
#define	UUID_987ac961_bb89_4694_9a0f_c40ffbc5d01b

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "exceptions.hpp"

/**
 * @file
 * @brief The @c class Aligned128BitArray.
 * This file contains the @c class Aligned128BitArray.
 */
namespace eternal_lands
{

	class Aligned128BitArray
	{
		private:
			void* m_data;
			size_t m_size;
			size_t m_capacity;

			inline size_t alloc_size() const
			{
				if (capacity() == 0)
				{
					return 1;
				}

				return capacity() * 2;
			}

			static inline ptrdiff_t get_offset(const size_t index)
				noexcept
			{
				return index * item_size();
			}

		protected:

			inline void prepare_push_back()
			{
				if (size() == capacity())
				{
					reserve(alloc_size());
				}

				m_size++;
			}

		public:
			Aligned128BitArray();
			Aligned128BitArray(const Aligned128BitArray &array);
			~Aligned128BitArray() noexcept;
			Aligned128BitArray &operator=(
				const Aligned128BitArray &array);
			void clear();
			void reserve(const size_t new_capacity);

			static inline size_t item_size() noexcept
			{
				return 16;
			}

			inline void* get_typeless_ptr() noexcept
			{
				return m_data;
			}

			inline const void* get_typeless_ptr() const noexcept
			{
				return m_data;
			}

			inline void* get_typeless_ptr_at(const size_t index)
				noexcept
			{
				assert(index < m_size);
				return static_cast<unsigned char*>(m_data) +
					get_offset(index);
			}

			inline const void* get_typeless_ptr_at(
				const size_t index) const noexcept
			{
				assert(index < m_size);
				return static_cast<const unsigned char*>(m_data)
					+ get_offset(index);
			}

			inline size_t size() const noexcept
			{
				return m_size;
			}

			inline size_t capacity() const noexcept
			{
				return m_capacity;
			}

			inline void pop_back()
			{
				assert(m_size > 0);

				m_size--;
			}

			/**
			 * resize changes the number of elements in the array.
			 */
			inline void resize(const size_t new_size)
			{
				size_t old_size;

				if (new_size > size())
				{
					reserve(new_size);
				}

				old_size = m_size;

				m_size = new_size;

				if (new_size > old_size)
				{
					memset(get_typeless_ptr_at(old_size),
						0, (new_size - old_size) *
						item_size());
				}
			}

	};

}

#endif	/* UUID_987ac961_bb89_4694_9a0f_c40ffbc5d01b */
