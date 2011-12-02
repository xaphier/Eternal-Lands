/****************************************************************************
 *            pack.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "prerequisites.hpp"
#include "exceptions.hpp"
#include "packtool.hpp"
#include <glm/gtx/epsilon.hpp>
#include <boost/random.hpp>
#include <boost/exception/diagnostic_information.hpp>
#define BOOST_TEST_MODULE pack
#include <boost/test/unit_test.hpp>

namespace el = eternal_lands;

BOOST_AUTO_TEST_CASE(pack_unpack_3_3_2)
{
	glm::vec3 tmp;
	Uint32 i, value;

	for (i = 0; i < std::numeric_limits<Uint8>::max(); i++)
	{
		tmp = el::PackTool::unpack_uint_3_3_2(false, i);
		value = el::PackTool::pack_uint_3_3_2(false, tmp);
		BOOST_CHECK_EQUAL(value, i);
	}
}

BOOST_AUTO_TEST_CASE(pack_unpack_3_3_2_normalized)
{
	glm::vec3 tmp;
	Uint32 i, value;

	for (i = 0; i < std::numeric_limits<Uint8>::max(); i++)
	{
		tmp = el::PackTool::unpack_uint_3_3_2(true, i);
		value = el::PackTool::pack_uint_3_3_2(true, tmp);
		BOOST_CHECK_EQUAL(value, i);
	}
}

BOOST_AUTO_TEST_CASE(pack_unpack_2_3_3_rev)
{
	glm::vec3 tmp;
	Uint32 i, value;

	for (i = 0; i < std::numeric_limits<Uint8>::max(); i++)
	{
		tmp = el::PackTool::unpack_uint_2_3_3_rev(false, i);
		value = el::PackTool::pack_uint_2_3_3_rev(false, tmp);
		BOOST_CHECK_EQUAL(value, i);
	}
}

BOOST_AUTO_TEST_CASE(pack_unpack_2_3_3_rev_normalized)
{
	glm::vec3 tmp;
	Uint32 i, value;

	for (i = 0; i < std::numeric_limits<Uint8>::max(); i++)
	{
		tmp = el::PackTool::unpack_uint_2_3_3_rev(true, i);
		value = el::PackTool::pack_uint_2_3_3_rev(true, tmp);
		BOOST_CHECK_EQUAL(value, i);
	}
}

BOOST_AUTO_TEST_CASE(pack_unpack_5_6_5)
{
	glm::vec3 tmp;
	Uint32 i, value;

	for (i = 0; i < std::numeric_limits<Uint16>::max(); i++)
	{
		tmp = el::PackTool::unpack_uint_5_6_5(false, i);
		value = el::PackTool::pack_uint_5_6_5(false, tmp);
		BOOST_CHECK_EQUAL(value, i);
	}
}

BOOST_AUTO_TEST_CASE(pack_unpack_5_6_5_normalized)
{
	glm::vec3 tmp;
	Uint32 i, value;

	for (i = 0; i < std::numeric_limits<Uint16>::max(); i++)
	{
		tmp = el::PackTool::unpack_uint_5_6_5(true, i);
		value = el::PackTool::pack_uint_5_6_5(true, tmp);
		BOOST_CHECK_EQUAL(value, i);
	}
}

BOOST_AUTO_TEST_CASE(pack_unpack_5_6_5_rev)
{
	glm::vec3 tmp;
	Uint32 i, value;

	for (i = 0; i < std::numeric_limits<Uint16>::max(); i++)
	{
		tmp = el::PackTool::unpack_uint_5_6_5_rev(false, i);
		value = el::PackTool::pack_uint_5_6_5_rev(false, tmp);
		BOOST_CHECK_EQUAL(value, i);
	}
}

BOOST_AUTO_TEST_CASE(pack_unpack_5_6_5_rev_normalized)
{
	glm::vec3 tmp;
	Uint32 i, value;

	for (i = 0; i < std::numeric_limits<Uint16>::max(); i++)
	{
		tmp = el::PackTool::unpack_uint_5_6_5_rev(true, i);
		value = el::PackTool::pack_uint_5_6_5_rev(true, tmp);
		BOOST_CHECK_EQUAL(value, i);
	}
}

BOOST_AUTO_TEST_CASE(pack_unpack_5_5_5_1)
{
	glm::vec4 tmp;
	Uint32 i, value;

	for (i = 0; i < std::numeric_limits<Uint16>::max(); i++)
	{
		tmp = el::PackTool::unpack_uint_5_5_5_1(false, i);
		value = el::PackTool::pack_uint_5_5_5_1(false, tmp);
		BOOST_CHECK_EQUAL(value, i);
	}
}

BOOST_AUTO_TEST_CASE(pack_unpack_5_5_5_1_normalized)
{
	glm::vec4 tmp;
	Uint32 i, value;

	for (i = 0; i < std::numeric_limits<Uint16>::max(); i++)
	{
		tmp = el::PackTool::unpack_uint_5_5_5_1(true, i);
		value = el::PackTool::pack_uint_5_5_5_1(true, tmp);
		BOOST_CHECK_EQUAL(value, i);
	}
}

BOOST_AUTO_TEST_CASE(pack_unpack_1_5_5_5_rev)
{
	glm::vec4 tmp;
	Uint32 i, value;

	for (i = 0; i < std::numeric_limits<Uint16>::max(); i++)
	{
		tmp = el::PackTool::unpack_uint_1_5_5_5_rev(false, i);
		value = el::PackTool::pack_uint_1_5_5_5_rev(false, tmp);
		BOOST_CHECK_EQUAL(value, i);
	}
}

BOOST_AUTO_TEST_CASE(pack_unpack_1_5_5_5_rev_normalized)
{
	glm::vec4 tmp;
	Uint32 i, value;

	for (i = 0; i < std::numeric_limits<Uint16>::max(); i++)
	{
		tmp = el::PackTool::unpack_uint_1_5_5_5_rev(true, i);
		value = el::PackTool::pack_uint_1_5_5_5_rev(true, tmp);
		BOOST_CHECK_EQUAL(value, i);
	}
}

BOOST_AUTO_TEST_CASE(pack_unpack_4_4_4_4)
{
	glm::vec4 tmp;
	Uint32 i, value;

	for (i = 0; i < std::numeric_limits<Uint16>::max(); i++)
	{
		tmp = el::PackTool::unpack_uint_4_4_4_4(false, i);
		value = el::PackTool::pack_uint_4_4_4_4(false, tmp);
		BOOST_CHECK_EQUAL(value, i);
	}
}

BOOST_AUTO_TEST_CASE(pack_unpack_4_4_4_4_normalized)
{
	glm::vec4 tmp;
	Uint32 i, value;

	for (i = 0; i < std::numeric_limits<Uint16>::max(); i++)
	{
		tmp = el::PackTool::unpack_uint_4_4_4_4(true, i);
		value = el::PackTool::pack_uint_4_4_4_4(true, tmp);
		BOOST_CHECK_EQUAL(value, i);
	}
}

BOOST_AUTO_TEST_CASE(pack_unpack_4_4_4_4_rev)
{
	glm::vec4 tmp;
	Uint32 i, value;

	for (i = 0; i < std::numeric_limits<Uint16>::max(); i++)
	{
		tmp = el::PackTool::unpack_uint_4_4_4_4_rev(false, i);
		value = el::PackTool::pack_uint_4_4_4_4_rev(false, tmp);
		BOOST_CHECK_EQUAL(value, i);
	}
}

BOOST_AUTO_TEST_CASE(pack_unpack_4_4_4_4_rev_normalized)
{
	glm::vec4 tmp;
	Uint32 i, value;

	for (i = 0; i < std::numeric_limits<Uint16>::max(); i++)
	{
		tmp = el::PackTool::unpack_uint_4_4_4_4_rev(true, i);
		value = el::PackTool::pack_uint_4_4_4_4_rev(true, tmp);
		BOOST_CHECK_EQUAL(value, i);
	}
}

BOOST_AUTO_TEST_CASE(pack_unpack_uint_10_10_10_2)
{
	boost::mt19937 rng;
	boost::uniform_int<Uint32> range(0, std::numeric_limits<Uint32>::max());
	boost::variate_generator<boost::mt19937&, boost::uniform_int<Uint32> >
		random_uint32(rng, range);
	glm::vec4 tmp;
	Uint32 i, temp, value;

	for (i = 0; i < std::numeric_limits<Uint16>::max(); i++)
	{
		temp = random_uint32();
		tmp = el::PackTool::unpack_uint_10_10_10_2(false, temp);
		value = el::PackTool::pack_uint_10_10_10_2(false, tmp);
		BOOST_CHECK_EQUAL(value, temp);
	}
}

BOOST_AUTO_TEST_CASE(pack_unpack_uint_10_10_10_2_normalized)
{
	boost::mt19937 rng;
	boost::uniform_int<Uint32> range(0, std::numeric_limits<Uint32>::max());
	boost::variate_generator<boost::mt19937&, boost::uniform_int<Uint32> >
		random_uint32(rng, range);
	glm::vec4 tmp;
	Uint32 i, temp, value;

	for (i = 0; i < std::numeric_limits<Uint16>::max(); i++)
	{
		temp = random_uint32();
		tmp = el::PackTool::unpack_uint_10_10_10_2(true, temp);
		value = el::PackTool::pack_uint_10_10_10_2(true, tmp);
		BOOST_CHECK_EQUAL(value, temp);
	}
}

BOOST_AUTO_TEST_CASE(pack_unpack_uint_2_10_10_10_rev)
{
	boost::mt19937 rng;
	boost::uniform_int<Uint32> range(0, std::numeric_limits<Uint32>::max());
	boost::variate_generator<boost::mt19937&, boost::uniform_int<Uint32> >
		random_uint32(rng, range);
	glm::vec4 tmp;
	Uint32 i, temp, value;

	for (i = 0; i < std::numeric_limits<Uint16>::max(); i++)
	{
		temp = random_uint32();
		tmp = el::PackTool::unpack_uint_2_10_10_10_rev(false, temp);
		value = el::PackTool::pack_uint_2_10_10_10_rev(false, tmp);
		BOOST_CHECK_EQUAL(value, temp);
	}
}

BOOST_AUTO_TEST_CASE(pack_unpack_uint_2_10_10_10_rev_normalized)
{
	boost::mt19937 rng;
	boost::uniform_int<Uint32> range(0, std::numeric_limits<Uint32>::max());
	boost::variate_generator<boost::mt19937&, boost::uniform_int<Uint32> >
		random_uint32(rng, range);
	glm::vec4 tmp;
	Uint32 i, temp, value;

	for (i = 0; i < std::numeric_limits<Uint16>::max(); i++)
	{
		temp = random_uint32();
		tmp = el::PackTool::unpack_uint_2_10_10_10_rev(true, temp);
		value = el::PackTool::pack_uint_2_10_10_10_rev(true, tmp);
		BOOST_CHECK_EQUAL(value, temp);
	}
}

BOOST_AUTO_TEST_CASE(pack_unpack_sint_10_10_10_2)
{
	boost::mt19937 rng;
	boost::uniform_int<Uint32> range(0, std::numeric_limits<Uint32>::max());
	boost::variate_generator<boost::mt19937&, boost::uniform_int<Uint32> >
		random_uint32(rng, range);
	glm::vec4 tmp;
	Uint32 i, temp, value;

	for (i = 0; i < std::numeric_limits<Uint16>::max(); i++)
	{
		temp = random_uint32();
		tmp = el::PackTool::unpack_sint_10_10_10_2(false, temp);
		value = el::PackTool::pack_sint_10_10_10_2(false, tmp);
		BOOST_CHECK_EQUAL(value, temp);
	}
}

BOOST_AUTO_TEST_CASE(pack_unpack_sint_10_10_10_2_normalized)
{
	boost::mt19937 rng;
	boost::uniform_int<Uint32> range(0, std::numeric_limits<Uint32>::max());
	boost::variate_generator<boost::mt19937&, boost::uniform_int<Uint32> >
		random_uint32(rng, range);
	glm::vec4 tmp;
	Uint32 i, temp, value;

	for (i = 0; i < std::numeric_limits<Uint16>::max(); i++)
	{
		temp = random_uint32();
		tmp = el::PackTool::unpack_sint_10_10_10_2(true, temp);
		value = el::PackTool::pack_sint_10_10_10_2(true, tmp);
		BOOST_CHECK_EQUAL(value, temp);
	}
}

BOOST_AUTO_TEST_CASE(pack_unpack_sint_2_10_10_10_rev)
{
	boost::mt19937 rng;
	boost::uniform_int<Uint32> range(0, std::numeric_limits<Uint32>::max());
	boost::variate_generator<boost::mt19937&, boost::uniform_int<Uint32> >
		random_uint32(rng, range);
	glm::vec4 tmp;
	Uint32 i, temp, value;

	for (i = 0; i < std::numeric_limits<Uint16>::max(); i++)
	{
		temp = random_uint32();
		tmp = el::PackTool::unpack_sint_2_10_10_10_rev(false, temp);
		value = el::PackTool::pack_sint_2_10_10_10_rev(false, tmp);
		BOOST_CHECK_EQUAL(value, temp);
	}
}

BOOST_AUTO_TEST_CASE(pack_unpack_sint_2_10_10_10_rev_normalized)
{
	boost::mt19937 rng;
	boost::uniform_int<Uint32> range(0, std::numeric_limits<Uint32>::max());
	boost::variate_generator<boost::mt19937&, boost::uniform_int<Uint32> >
		random_uint32(rng, range);
	glm::vec4 tmp;
	Uint32 i, temp, value;

	for (i = 0; i < std::numeric_limits<Uint16>::max(); i++)
	{
		temp = random_uint32();
		tmp = el::PackTool::unpack_sint_2_10_10_10_rev(true, temp);
		value = el::PackTool::pack_sint_2_10_10_10_rev(true, tmp);
		BOOST_CHECK_EQUAL(value, temp);
	}
}

BOOST_AUTO_TEST_CASE(compress_uncompress_normalized)
{
	glm::vec3 normal, similar;
	float l;
	Uint32 i, value, duplicates, max_duplicates;

	duplicates = 0;

	for (i = 0; i < std::numeric_limits<Uint16>::max(); i++)
	{
		normal = el::PackTool::uncompress_normalized(i);
		l = glm::length(normal);
		BOOST_CHECK_CLOSE(l, 1.0, 0.01);

		value = el::PackTool::compress_normalized(normal);

		if (value != i)
		{
			duplicates++;
			similar = el::PackTool::uncompress_normalized(value);
			BOOST_CHECK_SMALL(glm::distance(normal, similar), 0.01f);
		}
	}

	max_duplicates = std::numeric_limits<Uint16>::max() * 0.035f;

	BOOST_CHECK_LT(duplicates, (max_duplicates));
}

