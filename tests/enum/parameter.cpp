/****************************************************************************
 *            parameter.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "prerequisites.hpp"
#include "shader/parameterutil.hpp"
#define BOOST_TEST_MODULE parameterutil
#include <boost/test/unit_test.hpp>

namespace el = eternal_lands;

BOOST_AUTO_TEST_CASE(get_parameter_count)
{
	BOOST_CHECK_GT(el::ParameterUtil::get_parameter_count(), 0);
}

BOOST_AUTO_TEST_CASE(get_str)
{
	Uint32 i, count;

	count = el::ParameterUtil::get_parameter_count();

	for (i = 0; i < count; i++)
	{
		BOOST_CHECK_NO_THROW(el::ParameterUtil::get_str(
			static_cast<el::ParameterType>(i)));
	}
}

BOOST_AUTO_TEST_CASE(get_count)
{
	Uint32 i, count;

	count = el::ParameterUtil::get_parameter_count();

	for (i = 0; i < count; i++)
	{
		BOOST_CHECK_NO_THROW(el::ParameterUtil::get_count(
			static_cast<el::ParameterType>(i)));
	}
}

BOOST_AUTO_TEST_CASE(get_sampler)
{
	Uint32 i, count;

	count = el::ParameterUtil::get_parameter_count();

	for (i = 0; i < count; i++)
	{
		BOOST_CHECK_NO_THROW(el::ParameterUtil::get_sampler(
			static_cast<el::ParameterType>(i)));
	}
}

BOOST_AUTO_TEST_CASE(get_uint)
{
	Uint32 i, count;

	count = el::ParameterUtil::get_parameter_count();

	for (i = 0; i < count; i++)
	{
		BOOST_CHECK_NO_THROW(el::ParameterUtil::get_uint(
			static_cast<el::ParameterType>(i)));
	}
}

BOOST_AUTO_TEST_CASE(get_int)
{
	Uint32 i, count;

	count = el::ParameterUtil::get_parameter_count();

	for (i = 0; i < count; i++)
	{
		BOOST_CHECK_NO_THROW(el::ParameterUtil::get_int(
			static_cast<el::ParameterType>(i)));
	}
}

BOOST_AUTO_TEST_CASE(get_bool)
{
	Uint32 i, count;

	count = el::ParameterUtil::get_parameter_count();

	for (i = 0; i < count; i++)
	{
		BOOST_CHECK_NO_THROW(el::ParameterUtil::get_bool(
			static_cast<el::ParameterType>(i)));
	}
}

BOOST_AUTO_TEST_CASE(get_float)
{
	Uint32 i, count;

	count = el::ParameterUtil::get_parameter_count();

	for (i = 0; i < count; i++)
	{
		BOOST_CHECK_NO_THROW(el::ParameterUtil::get_float(
			static_cast<el::ParameterType>(i)));
	}
}

BOOST_AUTO_TEST_CASE(get_matrix)
{
	Uint32 i, count;

	count = el::ParameterUtil::get_parameter_count();

	for (i = 0; i < count; i++)
	{
		BOOST_CHECK_NO_THROW(el::ParameterUtil::get_matrix(
			static_cast<el::ParameterType>(i)));
	}
}

BOOST_AUTO_TEST_CASE(convert)
{
	Uint32 i, count;
	el::ParameterType type;

	count = el::ParameterUtil::get_parameter_count();

	for (i = 0; i < count; i++)
	{
		type = static_cast<el::ParameterType>(i);

		BOOST_CHECK_EQUAL(
			el::ParameterUtil::get_parameter(
			el::ParameterUtil::get_str(type)), type);
	}
}

BOOST_AUTO_TEST_CASE(convert_gl)
{
	Uint32 i, count;
	el::ParameterType type;

	count = el::ParameterUtil::get_parameter_count();

	for (i = 0; i < count; i++)
	{
		type = static_cast<el::ParameterType>(i);

		BOOST_CHECK_EQUAL(
			el::ParameterUtil::get_parameter(
			el::ParameterUtil::get_gl(type)), type);
	}
}

BOOST_AUTO_TEST_CASE(stream)
{
	Uint32 i, count;
	el::ParameterType type;

	count = el::ParameterUtil::get_parameter_count();

	for (i = 0; i < count; i++)
	{
		el::StringStream str;

		type = static_cast<el::ParameterType>(i);

		BOOST_CHECK_NO_THROW(str << type);

		BOOST_CHECK_EQUAL(str.str(),
			el::ParameterUtil::get_str(type).get());
	}
}

BOOST_AUTO_TEST_CASE(get_str_value)
{
	BOOST_CHECK_EQUAL(el::ParameterUtil::get_str(el::pt_bvec2).get(),
		"bvec2");

	BOOST_CHECK_EQUAL(el::ParameterUtil::get_str(el::pt_sampler1D).get(),
		"sampler1D");

	BOOST_CHECK_EQUAL(el::ParameterUtil::get_str(
		el::pt_usamplerBuffer).get(), "usamplerBuffer");
}

BOOST_AUTO_TEST_CASE(get_gl_value)
{
	BOOST_CHECK_EQUAL(el::ParameterUtil::get_gl(el::pt_isampler1D), 
		GL_INT_SAMPLER_1D);

	BOOST_CHECK_EQUAL(el::ParameterUtil::get_gl(el::pt_usampler3D), 
		GL_UNSIGNED_INT_SAMPLER_3D);
}
