/****************************************************************************
 *            pack.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "prerequisites.hpp"
#include "exceptions.hpp"
#include "packtool.hpp"
#include <boost/random.hpp>
#define BOOST_TEST_MODULE pack
#include <boost/test/unit_test.hpp>

namespace el = eternal_lands;

BOOST_AUTO_TEST_CASE(unpack_3_3_2)
{
	glm::vec3 tmp;
	Uint32 i, value;

	for (i = 0; i < std::numeric_limits<Uint8>::max(); i++)
	{
		tmp = el::PackTool::unpack_uint_3_3_2(false, i);

		BOOST_CHECK_LE(tmp[0], 7.0f);
		BOOST_CHECK_LE(tmp[1], 7.0f);
		BOOST_CHECK_LE(tmp[2], 3.0f);

		BOOST_CHECK_GE(tmp[0], 0.0f);
		BOOST_CHECK_GE(tmp[1], 0.0f);
		BOOST_CHECK_GE(tmp[2], 0.0f);

		value = el::PackTool::pack_uint_3_3_2(false, tmp);
		BOOST_CHECK_EQUAL(value, i);
	}
}

BOOST_AUTO_TEST_CASE(unpack_3_3_2_normalized)
{
	glm::vec3 tmp;
	Uint32 i, value;

	for (i = 0; i < std::numeric_limits<Uint8>::max(); i++)
	{
		tmp = el::PackTool::unpack_uint_3_3_2(true, i);

		BOOST_CHECK_LE(tmp[0], 1.0f);
		BOOST_CHECK_LE(tmp[1], 1.0f);
		BOOST_CHECK_LE(tmp[2], 1.0f);

		BOOST_CHECK_GE(tmp[0], 0.0f);
		BOOST_CHECK_GE(tmp[1], 0.0f);
		BOOST_CHECK_GE(tmp[2], 0.0f);

		value = el::PackTool::pack_uint_3_3_2(true, tmp);
		BOOST_CHECK_EQUAL(value, i);
	}
}

BOOST_AUTO_TEST_CASE(unpack_2_3_3_rev)
{
	glm::vec3 tmp;
	Uint32 i, value;

	for (i = 0; i < std::numeric_limits<Uint8>::max(); i++)
	{
		tmp = el::PackTool::unpack_uint_2_3_3_rev(false, i);

		BOOST_CHECK_LE(tmp[0], 7.0f);
		BOOST_CHECK_LE(tmp[1], 7.0f);
		BOOST_CHECK_LE(tmp[2], 3.0f);

		BOOST_CHECK_GE(tmp[0], 0.0f);
		BOOST_CHECK_GE(tmp[1], 0.0f);
		BOOST_CHECK_GE(tmp[2], 0.0f);

		value = el::PackTool::pack_uint_2_3_3_rev(false, tmp);
		BOOST_CHECK_EQUAL(value, i);
	}
}

BOOST_AUTO_TEST_CASE(unpack_2_3_3_rev_normalized)
{
	glm::vec3 tmp;
	Uint32 i, value;

	for (i = 0; i < std::numeric_limits<Uint8>::max(); i++)
	{
		tmp = el::PackTool::unpack_uint_2_3_3_rev(true, i);

		BOOST_CHECK_LE(tmp[0], 1.0f);
		BOOST_CHECK_LE(tmp[1], 1.0f);
		BOOST_CHECK_LE(tmp[2], 1.0f);

		BOOST_CHECK_GE(tmp[0], 0.0f);
		BOOST_CHECK_GE(tmp[1], 0.0f);
		BOOST_CHECK_GE(tmp[2], 0.0f);

		value = el::PackTool::pack_uint_2_3_3_rev(true, tmp);
		BOOST_CHECK_EQUAL(value, i);
	}
}

BOOST_AUTO_TEST_CASE(unpack_5_6_5)
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

BOOST_AUTO_TEST_CASE(unpack_5_6_5_normalized)
{
	glm::vec3 tmp;
	Uint32 i, value;

	for (i = 0; i < std::numeric_limits<Uint16>::max(); i++)
	{
		tmp = el::PackTool::unpack_uint_5_6_5(true, i);

		BOOST_CHECK_LE(tmp[0], 1.0f);
		BOOST_CHECK_LE(tmp[1], 1.0f);
		BOOST_CHECK_LE(tmp[2], 1.0f);

		BOOST_CHECK_GE(tmp[0], 0.0f);
		BOOST_CHECK_GE(tmp[1], 0.0f);
		BOOST_CHECK_GE(tmp[2], 0.0f);

		value = el::PackTool::pack_uint_5_6_5(true, tmp);
		BOOST_CHECK_EQUAL(value, i);
	}
}

BOOST_AUTO_TEST_CASE(unpack_5_6_5_rev)
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

BOOST_AUTO_TEST_CASE(unpack_5_6_5_rev_normalized)
{
	glm::vec3 tmp;
	Uint32 i, value;

	for (i = 0; i < std::numeric_limits<Uint16>::max(); i++)
	{
		tmp = el::PackTool::unpack_uint_5_6_5_rev(true, i);

		BOOST_CHECK_LE(tmp[0], 1.0f);
		BOOST_CHECK_LE(tmp[1], 1.0f);
		BOOST_CHECK_LE(tmp[2], 1.0f);

		BOOST_CHECK_GE(tmp[0], 0.0f);
		BOOST_CHECK_GE(tmp[1], 0.0f);
		BOOST_CHECK_GE(tmp[2], 0.0f);

		value = el::PackTool::pack_uint_5_6_5_rev(true, tmp);
		BOOST_CHECK_EQUAL(value, i);
	}
}

BOOST_AUTO_TEST_CASE(unpack_5_5_5_1)
{
	glm::vec4 tmp;
	Uint32 i, value;

	for (i = 0; i < std::numeric_limits<Uint16>::max(); i++)
	{
		tmp = el::PackTool::unpack_uint_5_5_5_1(false, i);

		BOOST_CHECK_LE(tmp[0], 31.0f);
		BOOST_CHECK_LE(tmp[1], 31.0f);
		BOOST_CHECK_LE(tmp[2], 31.0f);
		BOOST_CHECK_LE(tmp[3], 1.0f);

		BOOST_CHECK_GE(tmp[0], 0.0f);
		BOOST_CHECK_GE(tmp[1], 0.0f);
		BOOST_CHECK_GE(tmp[2], 0.0f);
		BOOST_CHECK_GE(tmp[3], 0.0f);

		value = el::PackTool::pack_uint_5_5_5_1(false, tmp);
		BOOST_CHECK_EQUAL(value, i);
	}
}

BOOST_AUTO_TEST_CASE(unpack_5_5_5_1_normalized)
{
	glm::vec4 tmp;
	Uint32 i, value;

	for (i = 0; i < std::numeric_limits<Uint16>::max(); i++)
	{
		tmp = el::PackTool::unpack_uint_5_5_5_1(true, i);

		BOOST_CHECK_LE(tmp[0], 1.0f);
		BOOST_CHECK_LE(tmp[1], 1.0f);
		BOOST_CHECK_LE(tmp[2], 1.0f);
		BOOST_CHECK_LE(tmp[3], 1.0f);

		BOOST_CHECK_GE(tmp[0], 0.0f);
		BOOST_CHECK_GE(tmp[1], 0.0f);
		BOOST_CHECK_GE(tmp[2], 0.0f);
		BOOST_CHECK_GE(tmp[3], 0.0f);

		value = el::PackTool::pack_uint_5_5_5_1(true, tmp);
		BOOST_CHECK_EQUAL(value, i);
	}
}

BOOST_AUTO_TEST_CASE(unpack_1_5_5_5_rev)
{
	glm::vec4 tmp;
	Uint32 i, value;

	for (i = 0; i < std::numeric_limits<Uint16>::max(); i++)
	{
		tmp = el::PackTool::unpack_uint_1_5_5_5_rev(false, i);

		BOOST_CHECK_LE(tmp[0], 31.0f);
		BOOST_CHECK_LE(tmp[1], 31.0f);
		BOOST_CHECK_LE(tmp[2], 31.0f);
		BOOST_CHECK_LE(tmp[3], 1.0f);

		BOOST_CHECK_GE(tmp[0], 0.0f);
		BOOST_CHECK_GE(tmp[1], 0.0f);
		BOOST_CHECK_GE(tmp[2], 0.0f);
		BOOST_CHECK_GE(tmp[3], 0.0f);

		value = el::PackTool::pack_uint_1_5_5_5_rev(false, tmp);
		BOOST_CHECK_EQUAL(value, i);
	}
}

BOOST_AUTO_TEST_CASE(unpack_1_5_5_5_rev_normalized)
{
	glm::vec4 tmp;
	Uint32 i, value;

	for (i = 0; i < std::numeric_limits<Uint16>::max(); i++)
	{
		tmp = el::PackTool::unpack_uint_1_5_5_5_rev(true, i);

		BOOST_CHECK_LE(tmp[0], 1.0f);
		BOOST_CHECK_LE(tmp[1], 1.0f);
		BOOST_CHECK_LE(tmp[2], 1.0f);
		BOOST_CHECK_LE(tmp[3], 1.0f);

		BOOST_CHECK_GE(tmp[0], 0.0f);
		BOOST_CHECK_GE(tmp[1], 0.0f);
		BOOST_CHECK_GE(tmp[2], 0.0f);
		BOOST_CHECK_GE(tmp[3], 0.0f);

		value = el::PackTool::pack_uint_1_5_5_5_rev(true, tmp);
		BOOST_CHECK_EQUAL(value, i);
	}
}

BOOST_AUTO_TEST_CASE(unpack_4_4_4_4)
{
	glm::vec4 tmp;
	Uint32 i, value;

	for (i = 0; i < std::numeric_limits<Uint16>::max(); i++)
	{
		tmp = el::PackTool::unpack_uint_4_4_4_4(false, i);

		BOOST_CHECK_LE(tmp[0], 15.0f);
		BOOST_CHECK_LE(tmp[1], 15.0f);
		BOOST_CHECK_LE(tmp[2], 15.0f);
		BOOST_CHECK_LE(tmp[3], 15.0f);

		BOOST_CHECK_GE(tmp[0], 0.0f);
		BOOST_CHECK_GE(tmp[1], 0.0f);
		BOOST_CHECK_GE(tmp[2], 0.0f);
		BOOST_CHECK_GE(tmp[3], 0.0f);

		value = el::PackTool::pack_uint_4_4_4_4(false, tmp);
		BOOST_CHECK_EQUAL(value, i);
	}
}

BOOST_AUTO_TEST_CASE(unpack_4_4_4_4_normalized)
{
	glm::vec4 tmp;
	Uint32 i, value;

	for (i = 0; i < std::numeric_limits<Uint16>::max(); i++)
	{
		tmp = el::PackTool::unpack_uint_4_4_4_4(true, i);

		BOOST_CHECK_LE(tmp[0], 1.0f);
		BOOST_CHECK_LE(tmp[1], 1.0f);
		BOOST_CHECK_LE(tmp[2], 1.0f);
		BOOST_CHECK_LE(tmp[3], 1.0f);

		BOOST_CHECK_GE(tmp[0], 0.0f);
		BOOST_CHECK_GE(tmp[1], 0.0f);
		BOOST_CHECK_GE(tmp[2], 0.0f);
		BOOST_CHECK_GE(tmp[3], 0.0f);

		value = el::PackTool::pack_uint_4_4_4_4(true, tmp);
		BOOST_CHECK_EQUAL(value, i);
	}
}

BOOST_AUTO_TEST_CASE(unpack_4_4_4_4_rev)
{
	glm::vec4 tmp;
	Uint32 i, value;

	for (i = 0; i < std::numeric_limits<Uint16>::max(); i++)
	{
		tmp = el::PackTool::unpack_uint_4_4_4_4_rev(false, i);

		BOOST_CHECK_LE(tmp[0], 15.0f);
		BOOST_CHECK_LE(tmp[1], 15.0f);
		BOOST_CHECK_LE(tmp[2], 15.0f);
		BOOST_CHECK_LE(tmp[3], 15.0f);

		BOOST_CHECK_GE(tmp[0], 0.0f);
		BOOST_CHECK_GE(tmp[1], 0.0f);
		BOOST_CHECK_GE(tmp[2], 0.0f);
		BOOST_CHECK_GE(tmp[3], 0.0f);

		value = el::PackTool::pack_uint_4_4_4_4_rev(false, tmp);
		BOOST_CHECK_EQUAL(value, i);
	}
}

BOOST_AUTO_TEST_CASE(unpack_4_4_4_4_rev_normalized)
{
	glm::vec4 tmp;
	Uint32 i, value;

	for (i = 0; i < std::numeric_limits<Uint16>::max(); i++)
	{
		tmp = el::PackTool::unpack_uint_4_4_4_4_rev(true, i);

		BOOST_CHECK_LE(tmp[0], 1.0f);
		BOOST_CHECK_LE(tmp[1], 1.0f);
		BOOST_CHECK_LE(tmp[2], 1.0f);
		BOOST_CHECK_LE(tmp[3], 1.0f);

		BOOST_CHECK_GE(tmp[0], 0.0f);
		BOOST_CHECK_GE(tmp[1], 0.0f);
		BOOST_CHECK_GE(tmp[2], 0.0f);
		BOOST_CHECK_GE(tmp[3], 0.0f);

		value = el::PackTool::pack_uint_4_4_4_4_rev(true, tmp);
		BOOST_CHECK_EQUAL(value, i);
	}
}

BOOST_AUTO_TEST_CASE(unpack_uint_10_10_10_2)
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

		BOOST_CHECK_LE(tmp[0], 1023.0f);
		BOOST_CHECK_LE(tmp[1], 1023.0f);
		BOOST_CHECK_LE(tmp[2], 1023.0f);
		BOOST_CHECK_LE(tmp[3], 3.0f);

		BOOST_CHECK_GE(tmp[0], 0.0f);
		BOOST_CHECK_GE(tmp[1], 0.0f);
		BOOST_CHECK_GE(tmp[2], 0.0f);
		BOOST_CHECK_GE(tmp[3], 0.0f);

		value = el::PackTool::pack_uint_10_10_10_2(false, tmp);
		BOOST_CHECK_EQUAL(value, temp);
	}
}

BOOST_AUTO_TEST_CASE(unpack_uint_10_10_10_2_normalized)
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

		BOOST_CHECK_LE(tmp[0], 1.0f);
		BOOST_CHECK_LE(tmp[1], 1.0f);
		BOOST_CHECK_LE(tmp[2], 1.0f);
		BOOST_CHECK_LE(tmp[3], 1.0f);

		BOOST_CHECK_GE(tmp[0], 0.0f);
		BOOST_CHECK_GE(tmp[1], 0.0f);
		BOOST_CHECK_GE(tmp[2], 0.0f);
		BOOST_CHECK_GE(tmp[3], 0.0f);

		value = el::PackTool::pack_uint_10_10_10_2(true, tmp);
		BOOST_CHECK_EQUAL(value, temp);
	}
}

BOOST_AUTO_TEST_CASE(unpack_uint_2_10_10_10_rev)
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

		BOOST_CHECK_LE(tmp[0], 1023.0f);
		BOOST_CHECK_LE(tmp[1], 1023.0f);
		BOOST_CHECK_LE(tmp[2], 1023.0f);
		BOOST_CHECK_LE(tmp[3], 3.0f);

		BOOST_CHECK_GE(tmp[0], 0.0f);
		BOOST_CHECK_GE(tmp[1], 0.0f);
		BOOST_CHECK_GE(tmp[2], 0.0f);
		BOOST_CHECK_GE(tmp[3], 0.0f);

		value = el::PackTool::pack_uint_2_10_10_10_rev(false, tmp);
		BOOST_CHECK_EQUAL(value, temp);
	}
}

BOOST_AUTO_TEST_CASE(unpack_uint_2_10_10_10_rev_normalized)
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

		BOOST_CHECK_LE(tmp[0], 1.0f);
		BOOST_CHECK_LE(tmp[1], 1.0f);
		BOOST_CHECK_LE(tmp[2], 1.0f);
		BOOST_CHECK_LE(tmp[3], 1.0f);

		BOOST_CHECK_GE(tmp[0], 0.0f);
		BOOST_CHECK_GE(tmp[1], 0.0f);
		BOOST_CHECK_GE(tmp[2], 0.0f);
		BOOST_CHECK_GE(tmp[3], 0.0f);

		value = el::PackTool::pack_uint_2_10_10_10_rev(true, tmp);
		BOOST_CHECK_EQUAL(value, temp);
	}
}

BOOST_AUTO_TEST_CASE(unpack_sint_10_10_10_2)
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

		BOOST_CHECK_LE(tmp[0], 511.0f);
		BOOST_CHECK_LE(tmp[1], 511.0f);
		BOOST_CHECK_LE(tmp[2], 511.0f);
		BOOST_CHECK_LE(tmp[3], 1.0f);

		BOOST_CHECK_GE(tmp[0], -512.0f);
		BOOST_CHECK_GE(tmp[1], -512.0f);
		BOOST_CHECK_GE(tmp[2], -512.0f);
		BOOST_CHECK_GE(tmp[3], -2.0f);

		value = el::PackTool::pack_sint_10_10_10_2(false, tmp);
		BOOST_CHECK_EQUAL(value, temp);
	}
}

BOOST_AUTO_TEST_CASE(unpack_sint_10_10_10_2_normalized)
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

		BOOST_CHECK_LE(tmp[0], 1.0f);
		BOOST_CHECK_LE(tmp[1], 1.0f);
		BOOST_CHECK_LE(tmp[2], 1.0f);
		BOOST_CHECK_LE(tmp[3], 1.0f);

		BOOST_CHECK_GE(tmp[0], -1.0f);
		BOOST_CHECK_GE(tmp[1], -1.0f);
		BOOST_CHECK_GE(tmp[2], -1.0f);
		BOOST_CHECK_GE(tmp[3], -1.0f);

		value = el::PackTool::pack_sint_10_10_10_2(true, tmp);
		BOOST_CHECK_EQUAL(value, temp);
	}
}

BOOST_AUTO_TEST_CASE(unpack_sint_2_10_10_10_rev)
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

		BOOST_CHECK_LE(tmp[0], 511.0f);
		BOOST_CHECK_LE(tmp[1], 511.0f);
		BOOST_CHECK_LE(tmp[2], 511.0f);
		BOOST_CHECK_LE(tmp[3], 1.0f);

		BOOST_CHECK_GE(tmp[0], -512.0f);
		BOOST_CHECK_GE(tmp[1], -512.0f);
		BOOST_CHECK_GE(tmp[2], -512.0f);
		BOOST_CHECK_GE(tmp[3], -2.0f);

		value = el::PackTool::pack_sint_2_10_10_10_rev(false, tmp);
		BOOST_CHECK_EQUAL(value, temp);
	}
}

BOOST_AUTO_TEST_CASE(unpack_sint_2_10_10_10_rev_normalized)
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

		BOOST_CHECK_LE(tmp[0], 1.0f);
		BOOST_CHECK_LE(tmp[1], 1.0f);
		BOOST_CHECK_LE(tmp[2], 1.0f);
		BOOST_CHECK_LE(tmp[3], 1.0f);

		BOOST_CHECK_GE(tmp[0], -1.0f);
		BOOST_CHECK_GE(tmp[1], -1.0f);
		BOOST_CHECK_GE(tmp[2], -1.0f);
		BOOST_CHECK_GE(tmp[3], -1.0f);

		value = el::PackTool::pack_sint_2_10_10_10_rev(true, tmp);
		BOOST_CHECK_EQUAL(value, temp);
	}
}

BOOST_AUTO_TEST_CASE(compress_uncompress_normalized)
{
	glm::vec3 normal, similar;
	Uint32 i, value, duplicates, max_duplicates;

	duplicates = 0;

	for (i = 0; i < std::numeric_limits<Uint16>::max(); i++)
	{
		normal = el::PackTool::uncompress_normalized(i);
		BOOST_CHECK_CLOSE(glm::length(normal), 1.0, 0.01);

		value = el::PackTool::compress_normalized(normal);

		if (value != i)
		{
			duplicates++;
			similar = el::PackTool::uncompress_normalized(value);
			BOOST_CHECK_SMALL(glm::distance(normal, similar),
				0.01f);
		}
	}

	max_duplicates = std::numeric_limits<Uint16>::max() * 0.035f;

	BOOST_CHECK_LT(duplicates, (max_duplicates));
}

BOOST_AUTO_TEST_CASE(encode_decode_normal)
{
	glm::vec3 normal;
	glm::vec2 temp, tmp, min, max;
	Uint32 i;

	min = glm::vec2(std::numeric_limits<float>::max());
	max = glm::vec2(-std::numeric_limits<float>::max());

	for (i = 0; i < std::numeric_limits<Uint16>::max(); i++)
	{
		temp.x = i % 256;
		temp.y = static_cast<Uint32>(i / 256);

		tmp = temp / 255.0f;

		BOOST_CHECK_LE(tmp.x, 1.0);
		BOOST_CHECK_LE(tmp.y, 1.0);
		BOOST_CHECK_GE(tmp.x, 0.0);
		BOOST_CHECK_GE(tmp.y, 0.0);

		normal = el::PackTool::decode_normal(tmp);

		if (!glm::all(glm::lessThanEqual(glm::abs(normal),
			glm::vec3(1.0f))))
		{
			continue;
		}

		BOOST_CHECK_CLOSE(glm::length(normal), 1.0, 0.01);

		tmp = el::PackTool::encode_normal(normal);

		BOOST_CHECK_LE(tmp.x, 1.0);
		BOOST_CHECK_LE(tmp.y, 1.0);
		BOOST_CHECK_GE(tmp.x, 0.0);
		BOOST_CHECK_GE(tmp.y, 0.0);

		normal = el::PackTool::decode_normal(tmp);

		BOOST_CHECK_CLOSE(glm::length(normal), 1.0, 0.01);
	}
}

BOOST_AUTO_TEST_CASE(encode_decode_normal_optimized)
{
	glm::vec3 normal;
	glm::vec2 temp, tmp, min, max;
	Uint32 i;

	min = glm::vec2(std::numeric_limits<float>::max());
	max = glm::vec2(-std::numeric_limits<float>::max());

	for (i = 0; i < std::numeric_limits<Uint16>::max(); i++)
	{
		temp.x = i % 256;
		temp.y = static_cast<Uint32>(i / 256);

		tmp = temp / 255.0f;

		BOOST_CHECK_LE(tmp.x, 1.0);
		BOOST_CHECK_LE(tmp.y, 1.0);
		BOOST_CHECK_GE(tmp.x, 0.0);
		BOOST_CHECK_GE(tmp.y, 0.0);

		normal = el::PackTool::decode_normal_optimized(tmp);

		if (!glm::all(glm::lessThanEqual(glm::abs(normal),
			glm::vec3(1.0f))))
		{
			continue;
		}

		BOOST_CHECK_CLOSE(glm::length(normal), 1.0, 0.01);

		tmp = el::PackTool::encode_normal_optimized(normal);

		BOOST_CHECK_LE(tmp.x, 1.0);
		BOOST_CHECK_LE(tmp.y, 1.0);
		BOOST_CHECK_GE(tmp.x, 0.0);
		BOOST_CHECK_GE(tmp.y, 0.0);

		normal = el::PackTool::decode_normal_optimized(tmp);

		BOOST_CHECK_CLOSE(glm::length(normal), 1.0, 0.01);
	}
}

BOOST_AUTO_TEST_CASE(encode_decode_normal_optimized_uint8)
{
	glm::vec3 normal;
	glm::vec2 min, max;
	glm::uvec2 tmp;
	Uint32 i;

	min = glm::vec2(std::numeric_limits<float>::max());
	max = glm::vec2(-std::numeric_limits<float>::max());

	for (i = 0; i < std::numeric_limits<Uint16>::max(); i++)
	{
		tmp.x = i % 256;
		tmp.y = static_cast<Uint32>(i / 256);

		BOOST_CHECK_LE(tmp.x, 255);
		BOOST_CHECK_LE(tmp.y, 255);
		BOOST_CHECK_GE(tmp.x, 0);
		BOOST_CHECK_GE(tmp.y, 0);

		normal = el::PackTool::decode_normal_optimized_uint8(tmp);

		if (!glm::all(glm::lessThanEqual(glm::abs(normal),
			glm::vec3(1.0f))))
		{
			continue;
		}

		BOOST_CHECK_CLOSE(glm::length(normal), 1.0, 0.01);

		tmp = el::PackTool::encode_normal_optimized_uint8(normal);

		BOOST_CHECK_LE(tmp.x, 255);
		BOOST_CHECK_LE(tmp.y, 255);
		BOOST_CHECK_GE(tmp.x, 0);
		BOOST_CHECK_GE(tmp.y, 0);

		normal = el::PackTool::decode_normal_optimized_uint8(tmp);

		BOOST_CHECK_CLOSE(glm::length(normal), 1.0, 0.01);
	}
}
