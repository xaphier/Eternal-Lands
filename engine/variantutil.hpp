/****************************************************************************
 *            variantutil.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_eaa8162d_661b_4529_a5a1_ee968a97d015
#define	UUID_eaa8162d_661b_4529_a5a1_ee968a97d015

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "shader/parameterutil.hpp"

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
			static Uint32 get_size(
				const Variant &variant);
			static String get_value_string(
				const Variant &variant);
			static const String &get_type_string(
				const Variant &variant);
			static ParameterType get_parameter_type(
				const Variant &variant);
			static Variant get_variant(const ParameterType type,
				const String &values);
			static Variant get_variant(const String &type,
				const String &values);

	};

	OutStream &operator<<(OutStream &str, const Variant &value);

}

#endif	/* UUID_eaa8162d_661b_4529_a5a1_ee968a97d015 */
