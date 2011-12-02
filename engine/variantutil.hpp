/****************************************************************************
 *            variantutil.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_eaa8162d_661b_4529_a5a1_ee968a97d015
#define	UUID_eaa8162d_661b_4529_a5a1_ee968a97d015

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class VariantUtil.
 * This file contains the @c class VariantUtil.
 */
namespace eternal_lands
{

	class VariantUtil
	{
		public:
			static String get_type_string(
				const Variant &variant);
			static Uint32 get_size(
				const Variant &variant);
			static StringType get_value_string(
				const Variant &variant);

	};

	OutStream& operator<<(OutStream &str, const Variant &value);

}

#endif	/* UUID_eaa8162d_661b_4529_a5a1_ee968a97d015 */
