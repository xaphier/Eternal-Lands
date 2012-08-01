/****************************************************************************
 *            shaderbuild.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "prerequisites.hpp"
#include "exceptions.hpp"
#include "shader/shaderbuildutil.hpp"
#include "utf.hpp"
#define BOOST_TEST_MODULE shader_build
#include <boost/test/unit_test.hpp>

namespace el = eternal_lands;

BOOST_AUTO_TEST_CASE(get_str)
{
	Uint32 i, count;

	count = el::ShaderBuildUtil::get_shader_build_count();

	for (i = 0; i < count; ++i)
	{
		BOOST_CHECK_NO_THROW(el::ShaderBuildUtil::get_str(
			static_cast<el::ShaderBuildType>(i)));
	}
}

BOOST_AUTO_TEST_CASE(convert)
{
	Uint32 i, count;
	el::ShaderBuildType type;

	count = el::ShaderBuildUtil::get_shader_build_count();

	for (i = 0; i < count; ++i)
	{
		type = static_cast<el::ShaderBuildType>(i);

		BOOST_CHECK_EQUAL(el::ShaderBuildUtil::get_shader_build(
			el::ShaderBuildUtil::get_str(type)), type);
	}
}

BOOST_AUTO_TEST_CASE(stream)
{
	Uint32 i, count;
	el::ShaderBuildType type, tmp;

	count = el::ShaderBuildUtil::get_shader_build_count();

	for (i = 0; i < count; i++)
	{
		el::StringStream str;

		type = static_cast<el::ShaderBuildType>(i);

		BOOST_CHECK_NO_THROW(str << type);

		BOOST_CHECK_EQUAL(str.str(),
			el::ShaderBuildUtil::get_str(type).get());

		BOOST_CHECK_NO_THROW(str >> tmp);

		BOOST_CHECK_EQUAL(tmp, type);
	}
}

BOOST_AUTO_TEST_CASE(get_str_value)
{
	BOOST_CHECK_EQUAL(el::ShaderBuildUtil::get_str(el::sbt_default).get(),
		"default");
	BOOST_CHECK_EQUAL(el::ShaderBuildUtil::get_str(
		el::sbt_light_index).get(), "light_index");
	BOOST_CHECK_EQUAL(el::ShaderBuildUtil::get_str(el::sbt_depth).get(),
		"depth");
	BOOST_CHECK_EQUAL(el::ShaderBuildUtil::get_str(el::sbt_shadow).get(),
		"shadow");
	BOOST_CHECK_EQUAL(el::ShaderBuildUtil::get_str(
		el::sbt_debug_depth).get(), "debug_depth");
	BOOST_CHECK_EQUAL(el::ShaderBuildUtil::get_str(
		el::sbt_debug_normal).get(), "debug_normal");
	BOOST_CHECK_EQUAL(el::ShaderBuildUtil::get_str(
		el::sbt_debug_emissive).get(), "debug_emissive");
}
