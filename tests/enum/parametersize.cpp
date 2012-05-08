/****************************************************************************
 *            parametersize.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "prerequisites.hpp"
#include "shader/parametersizeutil.hpp"
#define BOOST_TEST_MODULE parameter_size
#include <boost/test/unit_test.hpp>

namespace el = eternal_lands;

BOOST_AUTO_TEST_CASE(get_parameter_size_count)
{
	BOOST_CHECK_GT(el::ParameterSizeUtil::get_parameter_size_count(), 0);
}

BOOST_AUTO_TEST_CASE(get_str)
{
	Uint32 i, count;

	count = el::ParameterSizeUtil::get_parameter_size_count();

	for (i = 0; i < count; i++)
	{
		BOOST_CHECK_NO_THROW(el::ParameterSizeUtil::get_str(
			static_cast<el::ParameterSizeType>(i)));
	}
}

BOOST_AUTO_TEST_CASE(convert)
{
	Uint32 i, count;
	el::ParameterSizeType type;

	count = el::ParameterSizeUtil::get_parameter_size_count();

	for (i = 0; i < count; i++)
	{
		type = static_cast<el::ParameterSizeType>(i);

		BOOST_CHECK_EQUAL(el::ParameterSizeUtil::get_parameter_size(
			el::ParameterSizeUtil::get_str(type)), type);
	}
}

BOOST_AUTO_TEST_CASE(get_parameter_qualifier)
{
	Uint32 i, count;
	el::ParameterSizeType type, tmp;

	count = el::ParameterSizeUtil::get_parameter_size_count();

	for (i = 0; i < count; i++)
	{
		type = static_cast<el::ParameterSizeType>(i);

		BOOST_CHECK(el::ParameterSizeUtil::get_parameter_size(
			el::ParameterSizeUtil::get_str(type), tmp));

		BOOST_CHECK_EQUAL(type, tmp);
	}
}

BOOST_AUTO_TEST_CASE(get_str_value)
{
	BOOST_CHECK_EQUAL(el::ParameterSizeUtil::get_str(el::pst_one).get(),
		"one");
	BOOST_CHECK_EQUAL(el::ParameterSizeUtil::get_str(
		el::pst_light_count).get(), "light_count");
	BOOST_CHECK_EQUAL(el::ParameterSizeUtil::get_str(
		el::pst_bone_count).get(), "bone_count");
	BOOST_CHECK_EQUAL(el::ParameterSizeUtil::get_str(
		el::pst_shadow_map_count).get(), "shadow_map_count");
	BOOST_CHECK_EQUAL(el::ParameterSizeUtil::get_str(
		el::pst_clipmap_slices).get(), "clipmap_slices");
}
