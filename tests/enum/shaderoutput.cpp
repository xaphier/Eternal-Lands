/****************************************************************************
 *            shaderoutput.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "prerequisites.hpp"
#include "exceptions.hpp"
#include "shader/shaderoutpututil.hpp"
#include "utf.hpp"
#define BOOST_TEST_MODULE shader_output
#include <boost/test/unit_test.hpp>

namespace el = eternal_lands;

BOOST_AUTO_TEST_CASE(get_str)
{
	Uint32 i, count;

	count = el::ShaderOutputUtil::get_shader_output_count();

	for (i = 0; i < count; ++i)
	{
		BOOST_CHECK_NO_THROW(el::ShaderOutputUtil::get_str(
			static_cast<el::ShaderOutputType>(i)));
	}
}

BOOST_AUTO_TEST_CASE(convert)
{
	Uint32 i, count;
	el::ShaderOutputType type;

	count = el::ShaderOutputUtil::get_shader_output_count();

	for (i = 0; i < count; ++i)
	{
		type = static_cast<el::ShaderOutputType>(i);

		BOOST_CHECK_EQUAL(el::ShaderOutputUtil::get_shader_output(
			el::ShaderOutputUtil::get_str(type)), type);
	}
}

BOOST_AUTO_TEST_CASE(stream)
{
	Uint32 i, count;
	el::ShaderOutputType type, tmp;

	count = el::ShaderOutputUtil::get_shader_output_count();

	for (i = 0; i < count; i++)
	{
		el::StringStream str;

		type = static_cast<el::ShaderOutputType>(i);

		BOOST_CHECK_NO_THROW(str << type);

		BOOST_CHECK_EQUAL(str.str(),
			el::ShaderOutputUtil::get_str(type).get());

		BOOST_CHECK_NO_THROW(str >> tmp);

		BOOST_CHECK_EQUAL(tmp, type);
	}
}

BOOST_AUTO_TEST_CASE(get_str_value)
{
	BOOST_CHECK_EQUAL(el::ShaderOutputUtil::get_str(el::sot_float).get(),
		"float");
	BOOST_CHECK_EQUAL(el::ShaderOutputUtil::get_str(el::sot_int).get(),
		"int");
	BOOST_CHECK_EQUAL(el::ShaderOutputUtil::get_str(el::sot_uint).get(),
		"uint");
}
