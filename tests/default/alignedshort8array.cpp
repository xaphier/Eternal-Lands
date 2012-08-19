/****************************************************************************
 *            alignedshort8array.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "prerequisites.hpp"
#include "exceptions.hpp"
#include "alignedshort8array.hpp"
#include <boost/random.hpp>
#define BOOST_TEST_MODULE aligned_short_8_array
#include <boost/test/unit_test.hpp>

namespace el = eternal_lands;

namespace
{

	class RandomValueBuilder
	{
		private:
			std::vector<Sint16> m_values;

		public:
			RandomValueBuilder();

			inline Sint16 get_value(const Uint32 index) const
				noexcept
			{
				return m_values[index];
			}

			static inline Uint32 get_count()
			{
				return 262144;
			}

	};

	RandomValueBuilder::RandomValueBuilder()
	{
		boost::mt19937 rng;
		boost::uniform_int<Sint32> range(-32768, 32767);
		boost::variate_generator<boost::mt19937&,
			boost::uniform_int<Sint32> > random_int(rng, range);
		Uint32 i;

		m_values.reserve(get_count());

		for (i = 0; i < get_count(); ++i)
		{
			m_values.push_back(random_int());
		}

		assert(m_values.size() == get_count());
	}

}

BOOST_FIXTURE_TEST_SUITE(random_values, RandomValueBuilder)

BOOST_AUTO_TEST_CASE(create)
{
	boost::scoped_ptr<el::AlignedShort8Array> aligned_short_8_array;

	BOOST_CHECK_NO_THROW(aligned_short_8_array.reset(
		new el::AlignedShort8Array()));
}

BOOST_AUTO_TEST_CASE(push_back)
{
	el::AlignedShort8Array aligned_short_8_array;
	Uint32 i, count;

	count = std::numeric_limits<Uint16>::max();

	for (i = 0; i < count; ++i)
	{
		BOOST_CHECK_NO_THROW(aligned_short_8_array.push_back(
			glm::ivec4(), glm::ivec4()));
	}

	for (i = 0; i < count; ++i)
	{
		BOOST_CHECK_EQUAL(aligned_short_8_array.get_value_low(i)[0],
			0);
		BOOST_CHECK_EQUAL(aligned_short_8_array.get_value_low(i)[1],
			0);
		BOOST_CHECK_EQUAL(aligned_short_8_array.get_value_low(i)[2],
			0);
		BOOST_CHECK_EQUAL(aligned_short_8_array.get_value_low(i)[3],
			0);

		BOOST_CHECK_EQUAL(aligned_short_8_array.get_value_high(i)[0],
			0);
		BOOST_CHECK_EQUAL(aligned_short_8_array.get_value_high(i)[1],
			0);
		BOOST_CHECK_EQUAL(aligned_short_8_array.get_value_high(i)[2],
			0);
		BOOST_CHECK_EQUAL(aligned_short_8_array.get_value_high(i)[3],
			0);
	}

	aligned_short_8_array.clear();

	for (i = 0; i < count; ++i)
	{
		BOOST_CHECK_NO_THROW(aligned_short_8_array.push_back(
			glm::ivec4(1, 2, 3, 4), glm::ivec4(-1, -2, -3, -4)));
	}

	for (i = 0; i < count; ++i)
	{
		BOOST_CHECK_EQUAL(aligned_short_8_array.get_value_low(i)[0],
			1);
		BOOST_CHECK_EQUAL(aligned_short_8_array.get_value_low(i)[1],
			2);
		BOOST_CHECK_EQUAL(aligned_short_8_array.get_value_low(i)[2],
			3);
		BOOST_CHECK_EQUAL(aligned_short_8_array.get_value_low(i)[3],
			4);

		BOOST_CHECK_EQUAL(aligned_short_8_array.get_value_high(i)[0],
			-1);
		BOOST_CHECK_EQUAL(aligned_short_8_array.get_value_high(i)[1],
			-2);
		BOOST_CHECK_EQUAL(aligned_short_8_array.get_value_high(i)[2],
			-3);
		BOOST_CHECK_EQUAL(aligned_short_8_array.get_value_high(i)[3],
			-4);
	}
}

BOOST_AUTO_TEST_CASE(push_back_2)
{
	el::AlignedShort8Array aligned_short_8_array;
	glm::ivec4 v0, v1, t0, t1;
	Uint32 i, count;

	count = get_count() / 8;

	for (i = 0; i < count; ++i)
	{
		v0[0] = get_value(i * 8 + 0);
		v0[1] = get_value(i * 8 + 1);
		v0[2] = get_value(i * 8 + 2);
		v0[3] = get_value(i * 8 + 3);
		v1[0] = get_value(i * 8 + 4);
		v1[1] = get_value(i * 8 + 5);
		v1[2] = get_value(i * 8 + 6);
		v1[3] = get_value(i * 8 + 7);

		BOOST_CHECK_NO_THROW(aligned_short_8_array.push_back(v0, v1));
	}

	for (i = 0; i < count; ++i)
	{
		v0[0] = get_value(i * 8 + 0);
		v0[1] = get_value(i * 8 + 1);
		v0[2] = get_value(i * 8 + 2);
		v0[3] = get_value(i * 8 + 3);
		v1[0] = get_value(i * 8 + 4);
		v1[1] = get_value(i * 8 + 5);
		v1[2] = get_value(i * 8 + 6);
		v1[3] = get_value(i * 8 + 7);

		BOOST_CHECK_EQUAL(aligned_short_8_array.get_value_low(i)[0],
			v0[0]);
		BOOST_CHECK_EQUAL(aligned_short_8_array.get_value_low(i)[1],
			v0[1]);
		BOOST_CHECK_EQUAL(aligned_short_8_array.get_value_low(i)[2],
			v0[2]);
		BOOST_CHECK_EQUAL(aligned_short_8_array.get_value_low(i)[3],
			v0[3]);

		BOOST_CHECK_EQUAL(aligned_short_8_array.get_value_high(i)[0],
			v1[0]);
		BOOST_CHECK_EQUAL(aligned_short_8_array.get_value_high(i)[1],
			v1[1]);
		BOOST_CHECK_EQUAL(aligned_short_8_array.get_value_high(i)[2],
			v1[2]);
		BOOST_CHECK_EQUAL(aligned_short_8_array.get_value_high(i)[3],
			v1[3]);
	}
}

BOOST_AUTO_TEST_SUITE_END()
