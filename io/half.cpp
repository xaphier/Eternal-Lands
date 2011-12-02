/****************************************************************************
 *            half.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "half.h"
#include <glm/glm.hpp>

extern "C" float half_to_float(const Uint16 value)
{
	return glm::detail::toFloat32(value);
}

extern "C" Uint16 float_to_half(const float value)
{
	return glm::detail::toFloat16(value);
}

