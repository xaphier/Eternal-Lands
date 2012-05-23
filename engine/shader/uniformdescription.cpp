/****************************************************************************
 *            uniformdescription.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "uniformdescription.hpp"

namespace eternal_lands
{

	UniformDescription::UniformDescription(): m_auto_parameter(apt_time),
		m_offset(std::numeric_limits<Uint32>::max()),
		m_size(std::numeric_limits<Uint32>::max()),
		m_array_stride(std::numeric_limits<Uint32>::max()),
		m_matrix_stride(std::numeric_limits<Uint32>::max()),
		m_is_row_major(false)
	{
	}

	UniformDescription::UniformDescription(
		const AutoParameterType auto_parameter):
		m_auto_parameter(auto_parameter),
		m_offset(std::numeric_limits<Uint32>::max()),
		m_size(std::numeric_limits<Uint32>::max()),
		m_array_stride(std::numeric_limits<Uint32>::max()),
		m_matrix_stride(std::numeric_limits<Uint32>::max()),
		m_is_row_major(false)
	{
	}

	UniformDescription::UniformDescription(
		const AutoParameterType auto_parameter, const Uint32 offset,
		const Uint32 size, const Uint32 array_stride,
		const Uint32 matrix_stride, const bool is_row_major):
		m_auto_parameter(auto_parameter), m_offset(offset),
		m_size(size), m_array_stride(array_stride),
		m_matrix_stride(matrix_stride), m_is_row_major(is_row_major)
	{
	}

	UniformDescription::~UniformDescription() noexcept
	{
	}

}
