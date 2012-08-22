/****************************************************************************
 *            alignedarrays.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "prerequisites.hpp"
#include "exceptions.hpp"
#include "alignedarrays.hpp"
#include <boost/random.hpp>
#define BOOST_TEST_MODULE aligned_arrays
#include <boost/test/unit_test.hpp>

namespace el = eternal_lands;

namespace
{

	class RandomValueBuilder
	{
		private:
			std::vector<Uint8> m_values;

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
		boost::uniform_int<Sint32> range(0, 127);
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

typedef boost::mpl::list<glm::vec4, glm::ivec4, glm::uvec4, el::Sint16Array8,
	el::Uint16Array8, el::Sint8Array16, el::Uint8Array16> AllAlignedArrays;

BOOST_AUTO_TEST_CASE_TEMPLATE(create, T, AllAlignedArrays)
{
	boost::scoped_ptr<el::AlignedArrays<T> > aligned_array;

	BOOST_CHECK_NO_THROW(aligned_array.reset(new el::AlignedArrays<T>()));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(push_back, T, AllAlignedArrays)
{
	boost::scoped_ptr<el::AlignedArrays<T> > aligned_array;
	T value;
	size_t i, j, size, count;

	BOOST_CHECK_NO_THROW(aligned_array.reset(new el::AlignedArrays<T>()));

	size = 16 / sizeof(T);

	count = get_count() / size;

	for (i = 0; i < count; ++i)
	{
		for (j = 0; j < size; ++j)
		{
			value[j] = get_value(i * size + j);
		}

		aligned_array->push_back(value);

		BOOST_CHECK_EQUAL(aligned_array->size(), i + 1);
	}
}

BOOST_AUTO_TEST_CASE_TEMPLATE(alignment, T, AllAlignedArrays)
{
	boost::scoped_ptr<el::AlignedArrays<T> > aligned_array;
	T value;
	size_t i, j, size, count;

	BOOST_CHECK_NO_THROW(aligned_array.reset(new el::AlignedArrays<T>()));

	size = 16 / sizeof(T);

	count = get_count() / size;

	for (i = 0; i < count; ++i)
	{
		for (j = 0; j < size; ++j)
		{
			value[j] = get_value(i * size + j);
		}

		aligned_array->push_back(value);

		BOOST_CHECK_EQUAL(aligned_array->size(), i + 1);
		BOOST_CHECK_EQUAL(reinterpret_cast<uintptr_t>(
			aligned_array->get_ptr()) & 0xF, 0);
		BOOST_CHECK_EQUAL(reinterpret_cast<uintptr_t>(
			aligned_array->get_ptr_at(i)) & 0xF, 0);
	}
}

BOOST_AUTO_TEST_CASE_TEMPLATE(at, T, AllAlignedArrays)
{
	boost::scoped_ptr<el::AlignedArrays<T> > aligned_array;
	T value;
	size_t i, j, size, count;

	BOOST_CHECK_NO_THROW(aligned_array.reset(new el::AlignedArrays<T>()));

	size = 16 / sizeof(T);

	count = get_count() / size;

	for (i = 0; i < count; ++i)
	{
		for (j = 0; j < size; ++j)
		{
			value[j] = get_value(i * size + j);
		}

		aligned_array->push_back(value);

		BOOST_CHECK_EQUAL(aligned_array->size(), i + 1);
	}

	for (i = 0; i < count; ++i)
	{
		value = (*aligned_array)[i];

		for (j = 0; j < size; ++j)
		{
			BOOST_CHECK_EQUAL(value[j], get_value(i * size + j));
		}
	}
}

BOOST_AUTO_TEST_CASE_TEMPLATE(resize, T, AllAlignedArrays)
{
	boost::scoped_ptr<el::AlignedArrays<T> > aligned_array;
	T value;
	size_t i, j, size, count;

	BOOST_CHECK_NO_THROW(aligned_array.reset(new el::AlignedArrays<T>()));

	size = 16 / sizeof(T);

	count = get_count() / size;

	aligned_array->resize(count);

	BOOST_CHECK_EQUAL(aligned_array->size(), count);

	for (i = 0; i < count; ++i)
	{
		for (j = 0; j < size; ++j)
		{
			value[j] = get_value(i * size + j);
		}

		BOOST_CHECK_EQUAL(aligned_array->size(), count);

		aligned_array->at(i) = value;
	}

	for (i = 0; i < count; ++i)
	{
		BOOST_CHECK_EQUAL(aligned_array->size(), count);

		value = (*aligned_array)[i];

		for (j = 0; j < size; ++j)
		{
			BOOST_CHECK_EQUAL(value[j], get_value(i * size + j));
		}
	}
}

BOOST_AUTO_TEST_SUITE_END()
