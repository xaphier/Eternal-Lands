/****************************************************************************
 *            alignedshort8array.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "alignedshort8array.hpp"
#ifdef	USE_SSE2
#include <mm_malloc.h>
#endif	/* USE_SSE2 */

namespace eternal_lands
{

	AlignedShort8Array::AlignedShort8Array(): m_data(0), m_size(0),
		m_capacity(0)
	{
	}

	AlignedShort8Array::AlignedShort8Array(const AlignedShort8Array &array):
		m_data(0), m_size(0), m_capacity(0)
	{
		resize(array.m_size);
		memcpy(m_data, array.m_data, size() * 8 * sizeof(Sint16));
	}

	AlignedShort8Array::~AlignedShort8Array() noexcept
	{
		clear();
	}

	AlignedShort8Array &AlignedShort8Array::operator=(
		const AlignedShort8Array &array)
	{
		resize(array.m_size);

		memcpy(m_data, array.m_data, size() * 8 * sizeof(Sint16));

		return *this;
	}

	void AlignedShort8Array::clear()
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

	void AlignedShort8Array::reserve(const Uint32 new_capacity)
	{
		Sint16* data;
		Uint32 count;

		if (capacity() >= new_capacity)
		{
			return;
		}

		count = new_capacity * 8 * sizeof(Sint16);

#ifdef	USE_SSE2
		data = static_cast<Sint16*>(_mm_malloc(count, 16));
#else	/* USE_SSE2 */
		data = static_cast<Sint16*>(malloc(count));
#endif	/* USE_SSE2 */

		if (data == 0)
		{
			EL_THROW_EXCEPTION(BadAllocException());
		}

		count = std::min(new_capacity, size()) * 8 * sizeof(Sint16);

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
