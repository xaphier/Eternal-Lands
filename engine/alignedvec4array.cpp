/****************************************************************************
 *            alignedvec4array.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "alignedvec4array.hpp"
#ifdef	USE_SSE2
#include <mm_malloc.h>
#endif	/* USE_SSE2 */

namespace eternal_lands
{

	AlignedVec4Array::AlignedVec4Array(): m_data(0), m_size(0),
		m_capacity(0)
	{
	}

	AlignedVec4Array::AlignedVec4Array(const AlignedVec4Array &array):
		m_data(0), m_size(0), m_capacity(0)
	{
		resize(array.m_size);
		memcpy(m_data, array.m_data, size() * 4 * sizeof(float));
	}

	AlignedVec4Array::~AlignedVec4Array() throw()
	{
		clear();
	}

	AlignedVec4Array &AlignedVec4Array::operator=(
		const AlignedVec4Array &array)
	{
		resize(array.m_size);

		memcpy(m_data, array.m_data, size() * 4 * sizeof(float));

		return *this;
	}

	void AlignedVec4Array::clear()
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

	void AlignedVec4Array::reserve(const Uint32 new_capacity)
	{
		glm::vec4* data;
		Uint32 count;

		if (capacity() >= new_capacity)
		{
			return;
		}

		count = new_capacity * 4 * sizeof(float);

#ifdef	USE_SSE2
		data = static_cast<glm::vec4*>(_mm_malloc(count, 16));
#else	/* USE_SSE2 */
		data = static_cast<glm::vec4*>(malloc(count));
#endif	/* USE_SSE2 */

		if (data == 0)
		{
			EL_THROW_EXCEPTION(BadAllocException());
		}

		count = std::min(new_capacity, size()) * 4 * sizeof(float);

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
