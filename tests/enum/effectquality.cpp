/****************************************************************************
 *            effectquality.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "prerequisites.hpp"
#include "node/effectqualityutil.hpp"
#define BOOST_TEST_MODULE effect_quality
#include <boost/test/unit_test.hpp>

namespace el = eternal_lands;

BOOST_AUTO_TEST_CASE(get_effect_quality_count)
{
	BOOST_CHECK_GT(el::EffectQualityUtil::get_effect_quality_count(), 0);
}

BOOST_AUTO_TEST_CASE(get_str)
{
	Uint32 i, count;

	count = el::EffectQualityUtil::get_effect_quality_count();

	for (i = 0; i < count; i++)
	{
		BOOST_CHECK_NO_THROW(el::EffectQualityUtil::get_str(
			static_cast<el::EffectQualityType>(i)));
	}
}

BOOST_AUTO_TEST_CASE(convert)
{
	Uint32 i, count;
	el::EffectQualityType type;

	count = el::EffectQualityUtil::get_effect_quality_count();

	for (i = 0; i < count; i++)
	{
		type = static_cast<el::EffectQualityType>(i);

		BOOST_CHECK_EQUAL(
			el::EffectQualityUtil::get_effect_quality(
			el::EffectQualityUtil::get_str(type)), type);
	}
}

BOOST_AUTO_TEST_CASE(get_effect_quality)
{
	Uint32 i, count;
	el::EffectQualityType type, tmp;

	count = el::EffectQualityUtil::get_effect_quality_count();

	for (i = 0; i < count; i++)
	{
		type = static_cast<el::EffectQualityType>(i);

		BOOST_CHECK(el::EffectQualityUtil::get_effect_quality(
			el::EffectQualityUtil::get_str(type), tmp));

		BOOST_CHECK_EQUAL(type, tmp);
	}
}

BOOST_AUTO_TEST_CASE(stream)
{
	Uint32 i, count;
	el::EffectQualityType type, tmp;

	count = el::EffectQualityUtil::get_effect_quality_count();

	for (i = 0; i < count; i++)
	{
		el::StringStream str;

		type = static_cast<el::EffectQualityType>(i);

		BOOST_CHECK_NO_THROW(str << type);

		BOOST_CHECK_EQUAL(str.str(),
			el::EffectQualityUtil::get_str(type).get());

		BOOST_CHECK_NO_THROW(str >> tmp);

		BOOST_CHECK_EQUAL(tmp, type);
	}
}

BOOST_AUTO_TEST_CASE(get_str_value)
{
	BOOST_CHECK_EQUAL(el::EffectQualityUtil::get_str(
		el::eqt_low).get(), "low");
	BOOST_CHECK_EQUAL(el::EffectQualityUtil::get_str(
		el::eqt_medium).get(), "medium");
	BOOST_CHECK_EQUAL(el::EffectQualityUtil::get_str(
		el::eqt_high).get(), "high");
}
