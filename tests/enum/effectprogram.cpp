/****************************************************************************
 *            effectprogram.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "prerequisites.hpp"
#include "effect/effectprogramutil.hpp"
#define BOOST_TEST_MODULE effect_program
#include <boost/test/unit_test.hpp>

namespace el = eternal_lands;

BOOST_AUTO_TEST_CASE(get_effect_program_count)
{
	BOOST_CHECK_GT(el::EffectProgramUtil::get_effect_program_count(), 0);
}

BOOST_AUTO_TEST_CASE(get_str)
{
	Uint32 i, count;

	count = el::EffectProgramUtil::get_effect_program_count();

	for (i = 0; i < count; ++i)
	{
		BOOST_CHECK_NO_THROW(el::EffectProgramUtil::get_str(
			static_cast<el::EffectProgramType>(i)));
	}
}

BOOST_AUTO_TEST_CASE(convert)
{
	Uint32 i, count;
	el::EffectProgramType type;

	count = el::EffectProgramUtil::get_effect_program_count();

	for (i = 0; i < count; ++i)
	{
		type = static_cast<el::EffectProgramType>(i);

		BOOST_CHECK_EQUAL(el::EffectProgramUtil::get_effect_program(
			el::EffectProgramUtil::get_str(type)), type);
	}
}

BOOST_AUTO_TEST_CASE(stream)
{
	Uint32 i, count;
	el::EffectProgramType type, tmp;

	count = el::EffectProgramUtil::get_effect_program_count();

	for (i = 0; i < count; i++)
	{
		el::StringStream str;

		type = static_cast<el::EffectProgramType>(i);

		BOOST_CHECK_NO_THROW(str << type);

		BOOST_CHECK_EQUAL(str.str(),
			el::EffectProgramUtil::get_str(type).get());

		BOOST_CHECK_NO_THROW(str >> tmp);

		BOOST_CHECK_EQUAL(tmp, type);
	}
}

BOOST_AUTO_TEST_CASE(get_str_value)
{
	BOOST_CHECK_EQUAL(el::EffectProgramUtil::get_str(el::ept_default).get(),
		"default");
	BOOST_CHECK_EQUAL(el::EffectProgramUtil::get_str(el::ept_shadow).get(),
		"shadow");
	BOOST_CHECK_EQUAL(el::EffectProgramUtil::get_str(el::ept_depth).get(),
		"depth");
}
