/****************************************************************************
 *            shaderversion.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "prerequisites.hpp"
#include "exceptions.hpp"
#include "shader/shaderversionutil.hpp"
#include "utf.hpp"
#define BOOST_TEST_MODULE shader_version
#include <boost/test/unit_test.hpp>

namespace el = eternal_lands;

BOOST_AUTO_TEST_CASE(get_str)
{
	Uint32 i, count;

	count = el::ShaderVersionUtil::get_shader_version_count();

	for (i = 0; i < count; ++i)
	{
		BOOST_CHECK_NO_THROW(el::ShaderVersionUtil::get_str(
			static_cast<el::ShaderVersionType>(i)));
	}
}

BOOST_AUTO_TEST_CASE(convert)
{
	Uint32 i, count;
	el::ShaderVersionType type;

	count = el::ShaderVersionUtil::get_shader_version_count();

	for (i = 0; i < count; ++i)
	{
		type = static_cast<el::ShaderVersionType>(i);

		BOOST_CHECK_EQUAL(el::ShaderVersionUtil::get_shader_version(
			el::ShaderVersionUtil::get_str(type)), type);
	}
}

BOOST_AUTO_TEST_CASE(get_str_value)
{
	BOOST_CHECK_EQUAL(el::ShaderVersionUtil::get_str(el::svt_120).get(),
		"glsl_120");
	BOOST_CHECK_EQUAL(el::ShaderVersionUtil::get_str(el::svt_130).get(),
		"glsl_130");
	BOOST_CHECK_EQUAL(el::ShaderVersionUtil::get_str(el::svt_140).get(),
		"glsl_140");
	BOOST_CHECK_EQUAL(el::ShaderVersionUtil::get_str(el::svt_150).get(),
		"glsl_150");
	BOOST_CHECK_EQUAL(el::ShaderVersionUtil::get_str(el::svt_330).get(),
		"glsl_330");
}

BOOST_AUTO_TEST_CASE(get_shader_version_number)
{
	BOOST_CHECK_EQUAL(el::ShaderVersionUtil::get_shader_version_number(
		el::svt_120), 120);
	BOOST_CHECK_EQUAL(el::ShaderVersionUtil::get_shader_version_number(
		el::svt_130), 130);
	BOOST_CHECK_EQUAL(el::ShaderVersionUtil::get_shader_version_number(
		el::svt_140), 140);
	BOOST_CHECK_EQUAL(el::ShaderVersionUtil::get_shader_version_number(
		el::svt_150), 150);
	BOOST_CHECK_EQUAL(el::ShaderVersionUtil::get_shader_version_number(
		el::svt_330), 330);
}
