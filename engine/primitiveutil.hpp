/****************************************************************************
 *            primitiveutil.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_0ae183f8_1879_42b5_909e_999d35cf56d1
#define	UUID_0ae183f8_1879_42b5_909e_999d35cf56d1

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class PrimitiveUtil.
 * This file contains the @c class PrimitiveUtil.
 */
namespace eternal_lands
{

	/**
	 * Types of primitives.
	 * @{
	 */
	enum PrimitiveType
	{
		pt_points = GL_POINTS,
		pt_lines = GL_LINES,
		pt_triangles = GL_TRIANGLES,
		pt_triangle_strip = GL_TRIANGLE_STRIP,
		pt_triangle_fan = GL_TRIANGLE_FAN
	};
	/**
	 * @}
	 */

	class PrimitiveUtil
	{
		public:
			static Uint32 get_index_count(
				const PrimitiveType primitive,
				const Uint32 count);
			static String get_str(const PrimitiveType primitive);
			static PrimitiveType get_type(const String &str);

	};

	OutStream &operator<<(OutStream &str, const PrimitiveType value);
	InStream &operator>>(InStream &str, PrimitiveType &value);

}

#endif	/* UUID_0ae183f8_1879_42b5_909e_999d35cf56d1 */
