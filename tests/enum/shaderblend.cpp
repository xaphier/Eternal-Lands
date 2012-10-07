/****************************************************************************
 *            shaderblend.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "prerequisites.hpp"
#include "exceptions.hpp"
#include "shader/shaderblendutil.hpp"
#define BOOST_TEST_MODULE shader_blend
#include <boost/test/unit_test.hpp>

namespace el = eternal_lands;

BOOST_AUTO_TEST_CASE(get_str)
{
	Uint32 i;

	for (i = 0; i < el::ShaderBlendUtil::get_shader_blend_count(); i++)
	{
		BOOST_CHECK_NO_THROW(el::ShaderBlendUtil::get_str(
			static_cast<el::ShaderBlendType>(i)));
	}
}

BOOST_AUTO_TEST_CASE(convert)
{
	Uint32 i;
	el::ShaderBlendType type;

	for (i = 0; i < el::ShaderBlendUtil::get_shader_blend_count(); i++)
	{
		type = static_cast<el::ShaderBlendType>(i);

		BOOST_CHECK_EQUAL(el::ShaderBlendUtil::get_shader_blend(
			el::ShaderBlendUtil::get_str(type)), type);
	}
}

BOOST_AUTO_TEST_CASE(stream)
{
	Uint32 i, count;
	el::ShaderBlendType type, tmp;

	count = el::ShaderBlendUtil::get_shader_blend_count();

	for (i = 0; i < count; i++)
	{
		el::StringStream str;

		type = static_cast<el::ShaderBlendType>(i);

		BOOST_CHECK_NO_THROW(str << type);

		BOOST_CHECK_EQUAL(str.str(),
			el::ShaderBlendUtil::get_str(type).get());

		BOOST_CHECK_NO_THROW(str >> tmp);

		BOOST_CHECK_EQUAL(tmp, type);
	}
}

BOOST_AUTO_TEST_CASE(get_str_value)
{
	BOOST_CHECK_EQUAL(el::ShaderBlendUtil::get_str(
		el::sbt_texture).get(), "texture");
	BOOST_CHECK_EQUAL(el::ShaderBlendUtil::get_str(
		el::sbt_slope).get(), "slope");
	BOOST_CHECK_EQUAL(el::ShaderBlendUtil::get_str(
		el::sbt_height).get(), "height");
	BOOST_CHECK_EQUAL(el::ShaderBlendUtil::get_str(
		el::sbt_blend).get(), "blend");
	BOOST_CHECK_EQUAL(el::ShaderBlendUtil::get_str(
		el::sbt_size).get(), "size");
}
