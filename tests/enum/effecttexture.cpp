/****************************************************************************
 *            effecttexture.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "prerequisites.hpp"
#include "node/effecttextureutil.hpp"
#define BOOST_TEST_MODULE effect_texture
#include <boost/test/unit_test.hpp>

namespace el = eternal_lands;

BOOST_AUTO_TEST_CASE(get_effect_texture_count)
{
	BOOST_CHECK_GT(el::EffectTextureUtil::get_effect_texture_count(), 0);
}

BOOST_AUTO_TEST_CASE(get_str)
{
	Uint32 i, count;

	count = el::EffectTextureUtil::get_effect_texture_count();

	for (i = 0; i < count; i++)
	{
		BOOST_CHECK_NO_THROW(el::EffectTextureUtil::get_str(
			static_cast<el::EffectTextureType>(i)));
	}
}

BOOST_AUTO_TEST_CASE(convert)
{
	Uint32 i, count;
	el::EffectTextureType type;

	count = el::EffectTextureUtil::get_effect_texture_count();

	for (i = 0; i < count; i++)
	{
		type = static_cast<el::EffectTextureType>(i);

		BOOST_CHECK_EQUAL(
			el::EffectTextureUtil::get_effect_texture(
			el::EffectTextureUtil::get_str(type)), type);
	}
}

BOOST_AUTO_TEST_CASE(get_effect_texture)
{
	Uint32 i, count;
	el::EffectTextureType type, tmp;

	count = el::EffectTextureUtil::get_effect_texture_count();

	for (i = 0; i < count; i++)
	{
		type = static_cast<el::EffectTextureType>(i);

		BOOST_CHECK(el::EffectTextureUtil::get_effect_texture(
			el::EffectTextureUtil::get_str(type), tmp));

		BOOST_CHECK_EQUAL(type, tmp);
	}
}

BOOST_AUTO_TEST_CASE(stream)
{
	Uint32 i, count;
	el::EffectTextureType type, tmp;

	count = el::EffectTextureUtil::get_effect_texture_count();

	for (i = 0; i < count; i++)
	{
		el::StringStream str;

		type = static_cast<el::EffectTextureType>(i);

		BOOST_CHECK_NO_THROW(str << type);

		BOOST_CHECK_EQUAL(str.str(),
			el::EffectTextureUtil::get_str(type).get());

		BOOST_CHECK_NO_THROW(str >> tmp);

		BOOST_CHECK_EQUAL(tmp, type);
	}
}

BOOST_AUTO_TEST_CASE(get_str_value)
{
	BOOST_CHECK_EQUAL(el::EffectTextureUtil::get_str(
		el::ett_default).get(), "default");
	BOOST_CHECK_EQUAL(el::EffectTextureUtil::get_str(
		el::ett_albedo).get(), "albedo");
	BOOST_CHECK_EQUAL(el::EffectTextureUtil::get_str(
		el::ett_parallax).get(), "parallax");
}
