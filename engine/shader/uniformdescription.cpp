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

	void UniformDescription::write(const String &indent, OutStream &str)
		const
	{
		str << indent << get_type() << UTF8(" ") << get_name();

		if (get_auto_parameter_size() > 1)
		{
			str << UTF8("[") << get_auto_parameter_size();
			str << UTF8("]");
		}

		str << UTF8(";\n");
	}

	void UniformDescription::log(OutStream &str) const
	{
		str << UTF8("UniformDescription");
		str << UTF8(" [auto_parameter]: ") << get_auto_parameter();
		str << UTF8(" [offset]: ") << get_offset();
		str << UTF8(" [size]: ") << get_size();
		str << UTF8(" [array_stride]: ") << get_array_stride();
		str << UTF8(" [array_stride]: ") << get_array_stride();
		str << UTF8(" [matrix_stride]: ") << get_matrix_stride();
		str << UTF8(" [is_row_major]: ") << get_is_row_major();
	}

}
