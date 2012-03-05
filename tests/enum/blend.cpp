/****************************************************************************
 *            blend.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "prerequisites.hpp"
#include "exceptions.hpp"
#include "blendutil.hpp"
#include "utf.hpp"
#define BOOST_TEST_MODULE blend
#include <boost/test/unit_test.hpp>

namespace el = eternal_lands;

BOOST_AUTO_TEST_CASE(get_str)
{
	Uint32 i, count;

	count = el::BlendUtil::get_blend_type_count();

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

	count = el::BlendUtil::get_blend_type_count();

	for (i = 0; i < count; ++i)
	{
		type = static_cast<el::BlendType>(i);

		BOOST_CHECK_EQUAL(el::BlendUtil::get_blend(
			el::BlendUtil::get_str(type)), type);
	}
}

BOOST_AUTO_TEST_CASE(get_str_value)
{
	BOOST_CHECK_EQUAL(el::BlendUtil::get_str(el::bt_disabled).get(),
		"disabled");
	BOOST_CHECK_EQUAL(el::BlendUtil::get_str(
		el::bt_alpha_transparency_source_value).get(),
		"alpha_transparency_source_value");
	BOOST_CHECK_EQUAL(el::BlendUtil::get_str(
		el::bt_alpha_transparency_value).get(),
		"alpha_transparency_value");
	BOOST_CHECK_EQUAL(el::BlendUtil::get_str(el::bt_additive).get(),
		"additive");
}
