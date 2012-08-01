/****************************************************************************
 *            texturetarget.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "prerequisites.hpp"
#include "exceptions.hpp"
#include "texturetargetutil.hpp"
#include "utf.hpp"
#define BOOST_TEST_MODULE texture_target
#include <boost/test/unit_test.hpp>

namespace el = eternal_lands;

BOOST_AUTO_TEST_CASE(get_texture_target_count)
{
	BOOST_CHECK_GT(el::TextureTargetUtil::get_texture_target_count(), 0);
}

BOOST_AUTO_TEST_CASE(get_str)
{
	Uint32 i, count;

	count = el::TextureTargetUtil::get_texture_target_count();

	for (i = 0; i < count; ++i)
	{
		BOOST_CHECK_NO_THROW(el::TextureTargetUtil::get_str(
			static_cast<el::TextureTargetType>(i)));
	}
}

BOOST_AUTO_TEST_CASE(convert)
{
	Uint32 i, count;
	el::TextureTargetType type;

	count = el::TextureTargetUtil::get_texture_target_count();

	for (i = 0; i < count; ++i)
	{
		type = static_cast<el::TextureTargetType>(i);

		BOOST_CHECK_EQUAL(el::TextureTargetUtil::get_texture_target(
			el::TextureTargetUtil::get_str(type)), type);
	}
}

BOOST_AUTO_TEST_CASE(stream)
{
	Uint32 i, count;
	el::TextureTargetType type, tmp;

	count = el::TextureTargetUtil::get_texture_target_count();

	for (i = 0; i < count; i++)
	{
		el::StringStream str;

		type = static_cast<el::TextureTargetType>(i);

		BOOST_CHECK_NO_THROW(str << type);

		BOOST_CHECK_EQUAL(str.str(),
			el::TextureTargetUtil::get_str(type).get());

		BOOST_CHECK_NO_THROW(str >> tmp);

		BOOST_CHECK_EQUAL(tmp, type);
	}
}

BOOST_AUTO_TEST_CASE(get_str_value)
{
	BOOST_CHECK_EQUAL(el::TextureTargetUtil::get_str(
		el::ttt_texture_1d).get(), "texture_1d");
	BOOST_CHECK_EQUAL(el::TextureTargetUtil::get_str(
		el::ttt_texture_2d).get(), "texture_2d");
	BOOST_CHECK_EQUAL(el::TextureTargetUtil::get_str(
		el::ttt_texture_3d).get(), "texture_3d");
	BOOST_CHECK_EQUAL(el::TextureTargetUtil::get_str(
		el::ttt_texture_cube_map).get(), "texture_cube_map");
	BOOST_CHECK_EQUAL(el::TextureTargetUtil::get_str(
		el::ttt_texture_1d_array).get(), "texture_1d_array");
	BOOST_CHECK_EQUAL(el::TextureTargetUtil::get_str(
		el::ttt_texture_2d_array).get(), "texture_2d_array");
	BOOST_CHECK_EQUAL(el::TextureTargetUtil::get_str(
		el::ttt_texture_cube_map_array).get(),
		"texture_cube_map_array");
	BOOST_CHECK_EQUAL(el::TextureTargetUtil::get_str(
		el::ttt_texture_rectangle).get(), "texture_rectangle");
}
