/****************************************************************************
 *            aligned128bitarray.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "aligned128bitarray.hpp"
#ifdef	USE_SSE2
#include <mm_malloc.h>
#endif	/* USE_SSE2 */

namespace eternal_lands
{

	Aligned128BitArray::Aligned128BitArray(): m_data(0), m_size(0),
		m_capacity(0)
	{
	}

	Aligned128BitArray::Aligned128BitArray(const Aligned128BitArray &array):
		m_data(0), m_size(0), m_capacity(0)
	{
		resize(array.m_size);
		memcpy(m_data, array.m_data, size() * item_size());
	}

	Aligned128BitArray::~Aligned128BitArray() noexcept
	{
		clear();
	}

	Aligned128BitArray &Aligned128BitArray::operator=(
		const Aligned128BitArray &array)
	{
		resize(array.m_size);

		memcpy(m_data, array.m_data, size() * item_size());

		return *this;
	}

	void Aligned128BitArray::clear()
	{
		m_size = 0;
		m_capacity = 0;

		if (m_data != 0)
		{
#ifdef	USE_SSE2
			_mm_free(m_data);
#else	/* USE_SSE2 */
			free(m_data);
#endif	/* USE_SSE2 */
			m_data = 0;
		}
	}

	void Aligned128BitArray::reserve(const size_t new_capacity)
	{
		void* data;
		size_t count;

		if (capacity() >= new_capacity)
		{
			return;
		}

		count = new_capacity * item_size();

#ifdef	USE_SSE2
		data = _mm_malloc(count, 16);
#else	/* USE_SSE2 */
		data = malloc(count);
#endif	/* USE_SSE2 */

		if (data == 0)
		{
			EL_THROW_EXCEPTION(BadAllocException());
		}

		count = std::min(new_capacity, size()) * item_size();

		std::swap(m_data, data);

		if ((count > 0) && (data != 0))
		{
			memcpy(m_data, data, count);
		}

		if (data != 0)
		{
#ifdef	USE_SSE2
			_mm_free(data);
#else	/* USE_SSE2 */
			free(data);
#endif	/* USE_SSE2 */
		}

		m_capacity = new_capacity;
	}

}
