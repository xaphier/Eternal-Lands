/****************************************************************************
 *            parametersize.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "prerequisites.hpp"
#include "exceptions.hpp"
#include "shader/parametersizeutil.hpp"
#include "utf.hpp"
#include <glm/gtx/epsilon.hpp>
#include <boost/random.hpp>
#include <boost/exception/diagnostic_information.hpp>
#define BOOST_TEST_MODULE pack
#include <boost/test/unit_test.hpp>

namespace el = eternal_lands;

BOOST_AUTO_TEST_CASE(get_str)
{
	Uint32 i;

	for (i = 0; i < el::ParameterSizeUtil::get_parameter_size_count(); i++)
	{
		BOOST_CHECK_NO_THROW(el::ParameterSizeUtil::get_str(
			static_cast<el::ParameterSizeType>(i)));
	}
}

BOOST_AUTO_TEST_CASE(convert)
{
	Uint32 i;
	el::ParameterSizeType type;

	for (i = 0; i < el::ParameterSizeUtil::get_parameter_size_count(); i++)
	{
		type = static_cast<el::ParameterSizeType>(i);

		BOOST_CHECK_EQUAL(el::ParameterSizeUtil::get_parameter_size(
			el::ParameterSizeUtil::get_str(type)), type);
	}
}

BOOST_AUTO_TEST_CASE(get_str_value)
{
	BOOST_CHECK_EQUAL(el::string_to_utf8(el::ParameterSizeUtil::get_str(
		el::pst_one)), "one");
	BOOST_CHECK_EQUAL(el::string_to_utf8(el::ParameterSizeUtil::get_str(
		el::pst_light_count)), "light_count");
	BOOST_CHECK_EQUAL(el::string_to_utf8(el::ParameterSizeUtil::get_str(
		el::pst_bone_count)), "bone_count");
	BOOST_CHECK_EQUAL(el::string_to_utf8(el::ParameterSizeUtil::get_str(
		el::pst_shadow_map_count)), "shadow_map_count");
}
