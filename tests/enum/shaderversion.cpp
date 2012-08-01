/****************************************************************************
 *            shaderversion.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
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

BOOST_AUTO_TEST_CASE(stream)
{
	Uint32 i, count;
	el::ShaderVersionType type, tmp;

	count = el::ShaderVersionUtil::get_shader_version_count();

	for (i = 0; i < count; i++)
	{
		el::StringStream str;

		type = static_cast<el::ShaderVersionType>(i);

		BOOST_CHECK_NO_THROW(str << type);

		BOOST_CHECK_EQUAL(str.str(),
			el::ShaderVersionUtil::get_str(type).get());

		BOOST_CHECK_NO_THROW(str >> tmp);

		BOOST_CHECK_EQUAL(tmp, type);
	}
}

BOOST_AUTO_TEST_CASE(get_str_value)
{
	BOOST_CHECK_EQUAL(el::ShaderVersionUtil::get_str(el::svt_120).get(),
		"120");
	BOOST_CHECK_EQUAL(el::ShaderVersionUtil::get_str(el::svt_130).get(),
		"130");
	BOOST_CHECK_EQUAL(el::ShaderVersionUtil::get_str(el::svt_140).get(),
		"140");
	BOOST_CHECK_EQUAL(el::ShaderVersionUtil::get_str(el::svt_150).get(),
		"150");
	BOOST_CHECK_EQUAL(el::ShaderVersionUtil::get_str(el::svt_330).get(),
		"330");
	BOOST_CHECK_EQUAL(el::ShaderVersionUtil::get_str(el::svt_400).get(),
		"400");
	BOOST_CHECK_EQUAL(el::ShaderVersionUtil::get_str(el::svt_410).get(),
		"410");
	BOOST_CHECK_EQUAL(el::ShaderVersionUtil::get_str(el::svt_420).get(),
		"420");
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
	BOOST_CHECK_EQUAL(el::ShaderVersionUtil::get_shader_version_number(
		el::svt_400), 400);
	BOOST_CHECK_EQUAL(el::ShaderVersionUtil::get_shader_version_number(
		el::svt_410), 410);
	BOOST_CHECK_EQUAL(el::ShaderVersionUtil::get_shader_version_number(
		el::svt_420), 420);
}
