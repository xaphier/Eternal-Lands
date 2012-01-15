/****************************************************************************
 *            parameterqualifier.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "prerequisites.hpp"
#include "exceptions.hpp"
#include "shader/parameterqualifierutil.hpp"
#include "utf.hpp"
#define BOOST_TEST_MODULE parameter_qualifier
#include <boost/test/unit_test.hpp>

namespace el = eternal_lands;

BOOST_AUTO_TEST_CASE(get_str)
{
	Uint32 i;

	for (i = 0; i <
		el::ParameterQualifierUtil::get_parameter_qualifier_count();
			i++)
	{
		BOOST_CHECK_NO_THROW(el::ParameterQualifierUtil::get_str(
			static_cast<el::ParameterQualifierType>(i)));
	}
}

BOOST_AUTO_TEST_CASE(convert)
{
	Uint32 i;
	el::ParameterQualifierType type;

	for (i = 0; i <
		el::ParameterQualifierUtil::get_parameter_qualifier_count();
			i++)
	{
		type = static_cast<el::ParameterQualifierType>(i);

		BOOST_CHECK_EQUAL(
			el::ParameterQualifierUtil::get_parameter_qualifier(
				el::ParameterQualifierUtil::get_str(type)),
					type);
	}
}

BOOST_AUTO_TEST_CASE(get_str_value)
{
	BOOST_CHECK_EQUAL(el::ParameterQualifierUtil::get_str(
		el::pqt_in).get(), "in");
	BOOST_CHECK_EQUAL(el::ParameterQualifierUtil::get_str(
		el::pqt_out).get(), "out");
	BOOST_CHECK_EQUAL(el::ParameterQualifierUtil::get_str(
		el::pqt_inout).get(), "inout");
}
