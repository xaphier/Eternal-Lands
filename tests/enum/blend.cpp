/****************************************************************************
 *            blend.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "prerequisites.hpp"
#include "blendutil.hpp"
#define BOOST_TEST_MODULE blend
#include <boost/test/unit_test.hpp>

namespace el = eternal_lands;

BOOST_AUTO_TEST_CASE(get_blend_count)
{
	BOOST_CHECK_GT(el::BlendUtil::get_blend_count(), 0);
}

BOOST_AUTO_TEST_CASE(get_str)
{
	Uint32 i, count;

	count = el::BlendUtil::get_blend_count();

	for (i = 0; i < count; ++i)
	{
		BOOST_CHECK_NO_THROW(el::BlendUtil::get_str(
			static_cast<el::BlendType>(i)));
	}
}

BOOST_AUTO_TEST_CASE(convert)
{
	Uint32 i, count;
	el::BlendType type;

	count = el::BlendUtil::get_blend_count();

	for (i = 0; i < count; ++i)
	{
		type = static_cast<el::BlendType>(i);

		BOOST_CHECK_EQUAL(el::BlendUtil::get_blend(
			el::BlendUtil::get_str(type)), type);
	}
}

BOOST_AUTO_TEST_CASE(stream)
{
	Uint32 i, count;
	el::BlendType type, tmp;

	count = el::BlendUtil::get_blend_count();

	for (i = 0; i < count; i++)
	{
		el::StringStream str;

		type = static_cast<el::BlendType>(i);

		BOOST_CHECK_NO_THROW(str << type);

		BOOST_CHECK_EQUAL(str.str(),
			el::BlendUtil::get_str(type).get());

		BOOST_CHECK_NO_THROW(str >> tmp);

		BOOST_CHECK_EQUAL(tmp, type);
	}
}

BOOST_AUTO_TEST_CASE(get_str_value)
{
	BOOST_CHECK_EQUAL(el::BlendUtil::get_str(
		el::bt_alpha_transparency_source_value).get(),
		"alpha_transparency_source_value");
	BOOST_CHECK_EQUAL(el::BlendUtil::get_str(
		el::bt_alpha_transparency_value).get(),
		"alpha_transparency_value");
	BOOST_CHECK_EQUAL(el::BlendUtil::get_str(el::bt_additive).get(),
		"additive");
}
