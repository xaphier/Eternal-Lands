/****************************************************************************
 *            shadersource.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "prerequisites.hpp"
#include "exceptions.hpp"
#include "shader/shadersourceutil.hpp"
#include "utf.hpp"
#define BOOST_TEST_MODULE shader_source
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

BOOST_AUTO_TEST_CASE(stream)
{
	Uint32 i, count;
	el::ShaderSourceType type, tmp;

	count = el::ShaderSourceUtil::get_shader_source_count();

	for (i = 0; i < count; i++)
	{
		el::StringStream str;

		type = static_cast<el::ShaderSourceType>(i);

		BOOST_CHECK_NO_THROW(str << type);

		BOOST_CHECK_EQUAL(str.str(),
			el::ShaderSourceUtil::get_str(type).get());

		BOOST_CHECK_NO_THROW(str >> tmp);

		BOOST_CHECK_EQUAL(tmp, type);
	}
}

BOOST_AUTO_TEST_CASE(get_str_value)
{
	BOOST_CHECK_EQUAL(el::ShaderSourceUtil::get_str(
		el::sst_world_depth_transformation).get(),
		"world_depth_transformation");
	BOOST_CHECK_EQUAL(el::ShaderSourceUtil::get_str(
		el::sst_vertex_light).get(), "vertex_light");
	BOOST_CHECK_EQUAL(el::ShaderSourceUtil::get_str(
		el::sst_fragment_light).get(), "fragment_light");
	BOOST_CHECK_EQUAL(el::ShaderSourceUtil::get_str(
		el::sst_normal_mapping).get(), "normal_mapping");
	BOOST_CHECK_EQUAL(el::ShaderSourceUtil::get_str(
		el::sst_emission_mapping).get(), "emission_mapping");
	BOOST_CHECK_EQUAL(el::ShaderSourceUtil::get_str(
		el::sst_shadow_map).get(), "shadow_map");
	BOOST_CHECK_EQUAL(el::ShaderSourceUtil::get_str(
		el::sst_shadow_uv_ddx_ddy).get(), "shadow_uv_ddx_ddy");
}
