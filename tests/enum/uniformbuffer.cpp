/****************************************************************************
 *            uniformbuffer.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "prerequisites.hpp"
#include "shader/uniformbufferutil.hpp"
#define BOOST_TEST_MODULE uniform_buffer
#include <boost/test/unit_test.hpp>

namespace el = eternal_lands;

BOOST_AUTO_TEST_CASE(get_uniform_buffer_count)
{
	BOOST_CHECK_GT(el::UniformBufferUtil::get_uniform_buffer_count(), 0);
}

BOOST_AUTO_TEST_CASE(get_str)
{
	Uint32 i, count;

	count = el::UniformBufferUtil::get_uniform_buffer_count();

	for (i = 0; i < count; ++i)
	{
		BOOST_CHECK_NO_THROW(el::UniformBufferUtil::get_str(
			static_cast<el::UniformBufferType>(i)));
	}
}

BOOST_AUTO_TEST_CASE(get_identifier)
{
	Uint32 i, count;

	count = el::UniformBufferUtil::get_uniform_buffer_count();

	for (i = 0; i < count; ++i)
	{
		BOOST_CHECK_NO_THROW(el::UniformBufferUtil::get_identifier(
			static_cast<el::UniformBufferType>(i)));
	}
}

BOOST_AUTO_TEST_CASE(convert_str)
{
	Uint32 i, count;
	el::UniformBufferType type, tmp;

	count = el::UniformBufferUtil::get_uniform_buffer_count();

	for (i = 0; i < count; ++i)
	{
		type = static_cast<el::UniformBufferType>(i);

		BOOST_CHECK_EQUAL(el::UniformBufferUtil::get_uniform_buffer(
			el::UniformBufferUtil::get_str(type)), type);

		BOOST_CHECK(el::UniformBufferUtil::get_uniform_buffer(
			el::UniformBufferUtil::get_str(type), tmp));

		BOOST_CHECK_EQUAL(tmp, type);
	}
}

BOOST_AUTO_TEST_CASE(convert_identifier)
{
	Uint32 i, count;
	el::UniformBufferType type, tmp;

	count = el::UniformBufferUtil::get_uniform_buffer_count();

	for (i = 0; i < count; ++i)
	{
		type = static_cast<el::UniformBufferType>(i);

		BOOST_CHECK_EQUAL(el::UniformBufferUtil::
			get_uniform_buffer_from_identifier(
				el::UniformBufferUtil::get_identifier(type)),
			type);

		BOOST_CHECK(el::UniformBufferUtil::
			get_uniform_buffer_from_identifier(
				el::UniformBufferUtil::get_identifier(type),
				tmp));

		BOOST_CHECK_EQUAL(tmp, type);
	}
}

BOOST_AUTO_TEST_CASE(get_uniform_buffer)
{
	Uint32 i, count;
	el::UniformBufferType type, tmp;

	count = el::UniformBufferUtil::get_uniform_buffer_count();

	for (i = 0; i < count; i++)
	{
		type = static_cast<el::UniformBufferType>(i);

		BOOST_CHECK(el::UniformBufferUtil::get_uniform_buffer(
			el::UniformBufferUtil::get_str(type), tmp));

		BOOST_CHECK_EQUAL(type, tmp);
	}
}

BOOST_AUTO_TEST_CASE(stream)
{
	Uint32 i, count;
	el::UniformBufferType type, tmp;

	count = el::UniformBufferUtil::get_uniform_buffer_count();

	for (i = 0; i < count; i++)
	{
		el::StringStream str;

		type = static_cast<el::UniformBufferType>(i);

		BOOST_CHECK_NO_THROW(str << type);

		BOOST_CHECK_EQUAL(str.str(),
			el::UniformBufferUtil::get_str(type).get());

		BOOST_CHECK_NO_THROW(str >> tmp);

		BOOST_CHECK_EQUAL(tmp, type);
	}
}

BOOST_AUTO_TEST_CASE(get_str_value)
{
	BOOST_CHECK_EQUAL(el::UniformBufferUtil::get_str(el::ubt_scene).get(),
		"scene");
	BOOST_CHECK_EQUAL(el::UniformBufferUtil::get_str(
		el::ubt_material).get(), "material");
	BOOST_CHECK_EQUAL(el::UniformBufferUtil::get_str(
		el::ubt_terrain).get(), "terrain");
	BOOST_CHECK_EQUAL(el::UniformBufferUtil::get_str(
		el::ubt_terrain_instances).get(), "terrain_instances");
}

BOOST_AUTO_TEST_CASE(get_identifier_value)
{
	BOOST_CHECK_EQUAL(el::UniformBufferUtil::get_identifier(
		el::ubt_scene).get(), "Scene");
	BOOST_CHECK_EQUAL(el::UniformBufferUtil::get_identifier(
		el::ubt_material).get(), "Material");
	BOOST_CHECK_EQUAL(el::UniformBufferUtil::get_identifier(
		el::ubt_terrain).get(), "Terrain");
	BOOST_CHECK_EQUAL(el::UniformBufferUtil::get_identifier(
		el::ubt_terrain_instances).get(), "TerrainInstances");
}
