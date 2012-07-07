/****************************************************************************
 *            parameterqualifier.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "prerequisites.hpp"
#include "shader/parameterqualifierutil.hpp"
#define BOOST_TEST_MODULE parameter_qualifier
#include <boost/test/unit_test.hpp>

namespace el = eternal_lands;

BOOST_AUTO_TEST_CASE(get_parameter_qualifier_count)
{
	BOOST_CHECK_GT(
		el::ParameterQualifierUtil::get_parameter_qualifier_count(), 0);
}

BOOST_AUTO_TEST_CASE(get_str)
{
	Uint32 i, count;

	count = el::ParameterQualifierUtil::get_parameter_qualifier_count();

	for (i = 0; i < count; i++)
	{
		BOOST_CHECK_NO_THROW(el::ParameterQualifierUtil::get_str(
			static_cast<el::ParameterQualifierType>(i)));
	}
}

BOOST_AUTO_TEST_CASE(convert)
{
	Uint32 i, count;
	el::ParameterQualifierType type;

	count = el::ParameterQualifierUtil::get_parameter_qualifier_count();

	for (i = 0; i < count; i++)
	{
		type = static_cast<el::ParameterQualifierType>(i);

		BOOST_CHECK_EQUAL(
			el::ParameterQualifierUtil::get_parameter_qualifier(
				el::ParameterQualifierUtil::get_str(type)),
					type);
	}
}

BOOST_AUTO_TEST_CASE(get_parameter_qualifier)
{
	Uint32 i, count;
	el::ParameterQualifierType type, tmp;

	count = el::ParameterQualifierUtil::get_parameter_qualifier_count();

	for (i = 0; i < count; i++)
	{
		type = static_cast<el::ParameterQualifierType>(i);

		BOOST_CHECK(el::ParameterQualifierUtil::get_parameter_qualifier(
			el::ParameterQualifierUtil::get_str(type), tmp));

		BOOST_CHECK_EQUAL(type, tmp);
	}
}

BOOST_AUTO_TEST_CASE(stream)
{
	Uint32 i, count;
	el::ParameterQualifierType type, tmp;

	count = el::ParameterQualifierUtil::get_parameter_qualifier_count();

	for (i = 0; i < count; i++)
	{
		el::StringStream str;

		type = static_cast<el::ParameterQualifierType>(i);

		BOOST_CHECK_NO_THROW(str << type);

		BOOST_CHECK_EQUAL(str.str(),
			el::ParameterQualifierUtil::get_str(type).get());

		BOOST_CHECK_NO_THROW(str >> tmp);

		BOOST_CHECK_EQUAL(tmp, type);
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
