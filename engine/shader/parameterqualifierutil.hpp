/****************************************************************************
 *            parameterqualifierutil.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_5e6ef96e_8865_40eb_b956_7ee194cd37a3
#define	UUID_5e6ef96e_8865_40eb_b956_7ee194cd37a3

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class ParameterUtil.
 * This file contains the @c class ParameterUtil.
 */
namespace eternal_lands
{

	/**
	 * Basic types of the shader parameter qualifiers.
	 * @{
	 */
	enum ParameterQualifierType
	{
		pqt_in = 0,
		pqt_out = 1,
		pqt_inout = 2
	};
	/**
	 * @}
	 */

	class ParameterQualifierUtil
	{
		public:
			static const String &get_str(
				const ParameterQualifierType parameter);
			static ParameterQualifierType get_parameter_qualifier(
				const String &str);
			static bool get_parameter_qualifier(const String &str,
				ParameterQualifierType &parameter_qualifier);
			static Uint32 get_parameter_qualifier_count();

	};

	OutStream& operator<<(OutStream &str,
		const ParameterQualifierType value);
	InStream& operator>>(InStream &str, ParameterQualifierType &value);

}

#endif	/* UUID_5e6ef96e_8865_40eb_b956_7ee194cd37a3 */
