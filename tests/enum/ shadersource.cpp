/****************************************************************************
 *            shadersource.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "prerequisites.hpp"
#include "exceptions.hpp"
#include "shader/shadersourceutil.hpp"
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

	for (i = 0; i < el::ShaderSourceUtil::get_shader_source_count(); i++)
	{
		BOOST_CHECK_NO_THROW(el::ShaderSourceUtil::get_str(
			static_cast<el::ShaderSourceType>(i)));
	}
}

BOOST_AUTO_TEST_CASE(convert)
{
	Uint32 i;
	el::ShaderSourceType type;

	for (i = 0; i < el::ShaderSourceUtil::get_shader_source_count(); i++)
	{
		type = static_cast<el::ShaderSourceType>(i);

		BOOST_CHECK_EQUAL(el::ShaderSourceUtil::get_shader_source(
			el::ShaderSourceUtil::get_str(type)), type);
	}
}

BOOST_AUTO_TEST_CASE(get_str_value)
{
	BOOST_CHECK_EQUAL(el::string_to_utf8(el::ShaderSourceUtil::get_str(
		el::sst_world_depth_transform)), "world_depth_transform");
	BOOST_CHECK_EQUAL(el::string_to_utf8(el::ShaderSourceUtil::get_str(
		el::sst_light)), "light");
	BOOST_CHECK_EQUAL(el::string_to_utf8(el::ShaderSourceUtil::get_str(
		el::sst_normal_mapping)), "normal_mapping");
	BOOST_CHECK_EQUAL(el::string_to_utf8(el::ShaderSourceUtil::get_str(
		el::sst_shadow_map)), "shadow_map");
}
