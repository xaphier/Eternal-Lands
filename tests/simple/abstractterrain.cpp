/****************************************************************************
 *            abstractterrain.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "prerequisites.hpp"
#include "abstractterrain.hpp"
#include "packtool.hpp"
#include <boost/random.hpp>
#define BOOST_TEST_MODULE light
#include <boost/test/unit_test.hpp>

namespace el = eternal_lands;

BOOST_AUTO_TEST_CASE(get_vector_scale)
{
	BOOST_CHECK_GT(el::AbstractTerrain::get_vector_scale(), 0.0f);
}

BOOST_AUTO_TEST_CASE(get_vector_min)
{
	BOOST_CHECK_LE(el::AbstractTerrain::get_vector_min().x, 0.0f);
	BOOST_CHECK_LE(el::AbstractTerrain::get_vector_min().y, 0.0f);
	BOOST_CHECK_LE(el::AbstractTerrain::get_vector_min().z, 0.0f);
}

BOOST_AUTO_TEST_CASE(get_vector_max)
{
	BOOST_CHECK_GT(el::AbstractTerrain::get_vector_max().x, 0.0f);
	BOOST_CHECK_GT(el::AbstractTerrain::get_vector_max().y, 0.0f);
	BOOST_CHECK_GT(el::AbstractTerrain::get_vector_max().z, 0.0f);
}

BOOST_AUTO_TEST_CASE(get_tile_size)
{
	BOOST_CHECK_GT(el::AbstractTerrain::get_tile_size(), 1);
	BOOST_CHECK_EQUAL(__builtin_popcount(
		el::AbstractTerrain::get_tile_size()), 1);
}

BOOST_AUTO_TEST_CASE(get_patch_scale)
{
	BOOST_CHECK_GT(el::AbstractTerrain::get_patch_scale(), 0.0f);
}

BOOST_AUTO_TEST_CASE(get_offset_rgb10_a2)
{
	boost::mt19937 rng;
	boost::uniform_int<Uint32> range(0, std::numeric_limits<Uint32>::max());
	boost::variate_generator<boost::mt19937&, boost::uniform_int<Uint32> >
		random_uint32(rng, range);
	glm::uvec4 tmp;
	glm::vec3 temp;
	Uint32 i;

	for (i = 0; i < std::numeric_limits<Uint16>::max(); i++)
	{
		tmp.x = random_uint32() % 1024;
		tmp.y = random_uint32() % 1024;
		tmp.z = random_uint32() % 1024;
		tmp.w = random_uint32() % 4;

		temp = el::AbstractTerrain::get_offset_rgb10_a2(tmp);

		BOOST_CHECK_GE(temp.x, -1.0f - el::epsilon);
		BOOST_CHECK_LE(temp.x, 1.0f + el::epsilon);

		BOOST_CHECK_GE(temp.y, -1.0f - el::epsilon);
		BOOST_CHECK_LE(temp.y, 1.0f + el::epsilon);

		BOOST_CHECK_GE(temp.z, 0.0f - el::epsilon);
		BOOST_CHECK_LE(temp.z, 1.0f + el::epsilon);
	}
}

BOOST_AUTO_TEST_CASE(get_offset_scaled_rgb10_a2)
{
	boost::mt19937 rng;
	boost::uniform_int<Uint32> range(0, std::numeric_limits<Uint32>::max());
	boost::variate_generator<boost::mt19937&, boost::uniform_int<Uint32> >
		random_uint32(rng, range);
	glm::uvec4 tmp;
	glm::vec3 temp, scale;
	Uint32 i;

	scale = glm::vec3(el::AbstractTerrain::get_vector_scale());

	for (i = 0; i < std::numeric_limits<Uint16>::max(); i++)
	{
		tmp.x = random_uint32() % 1024;
		tmp.y = random_uint32() % 1024;
		tmp.z = random_uint32() % 1024;
		tmp.w = random_uint32() % 4;

		temp = el::AbstractTerrain::get_offset_scaled_rgb10_a2(
			tmp);

		BOOST_CHECK_GE(temp.x, -scale.x - el::epsilon);
		BOOST_CHECK_LE(temp.x, scale.x + el::epsilon);

		BOOST_CHECK_GE(temp.y, -scale.y - el::epsilon);
		BOOST_CHECK_LE(temp.y, scale.y + el::epsilon);

		BOOST_CHECK_GE(temp.z, 0.0f - el::epsilon);
		BOOST_CHECK_LE(temp.z, scale.z + el::epsilon);
	}
}

BOOST_AUTO_TEST_CASE(get_value_scaled_rgb10_a2)
{
	boost::mt19937 rng;
	boost::uniform_real<float> range(0.0f, 1.0f);
	boost::variate_generator<boost::mt19937&, boost::uniform_real<float> >
		random_float(rng, range);
	glm::uvec4 temp;
	glm::vec3 tmp, scale;
	Uint32 i;

	scale = glm::vec3(el::AbstractTerrain::get_vector_scale());

	for (i = 0; i < std::numeric_limits<Uint16>::max(); i++)
	{
		tmp.x = random_float() * 2.0f - 1.0f;
		tmp.y = random_float() * 2.0f - 1.0f;
		tmp.z = random_float();

		temp = el::AbstractTerrain::get_value_scaled_rgb10_a2(
			tmp * scale);

		BOOST_CHECK_LE(temp.x, 1023);
		BOOST_CHECK_LE(temp.y, 1023);
		BOOST_CHECK_LE(temp.z, 1023);
		BOOST_CHECK_LE(temp.w, 3);
	}
}

BOOST_AUTO_TEST_CASE(get_value_rgb10_a2)
{
	boost::mt19937 rng;
	boost::uniform_real<float> range(0.0f, 1.0f);
	boost::variate_generator<boost::mt19937&, boost::uniform_real<float> >
		random_float(rng, range);
	glm::uvec4 temp;
	glm::vec3 tmp;
	Uint32 i;

	for (i = 0; i < std::numeric_limits<Uint16>::max(); i++)
	{
		tmp.x = random_float() * 2.0f - 1.0f;
		tmp.y = random_float() * 2.0f - 1.0f;
		tmp.z = random_float();

		temp = el::AbstractTerrain::get_value_rgb10_a2(tmp);

		BOOST_CHECK_LE(temp.x, 1023);
		BOOST_CHECK_LE(temp.y, 1023);
		BOOST_CHECK_LE(temp.z, 1023);
		BOOST_CHECK_LE(temp.w, 3);
	}
}

BOOST_AUTO_TEST_CASE(convert_scaled_rgb10_a2)
{
	boost::mt19937 rng;
	boost::uniform_int<Uint32> range(0, std::numeric_limits<Uint32>::max());
	boost::variate_generator<boost::mt19937&, boost::uniform_int<Uint32> >
		random_uint32(rng, range);
	glm::uvec4 offset, tmp;
	glm::vec3 temp;
	float scale;
	Uint32 i;

	scale = el::AbstractTerrain::get_vector_scale();

	for (i = 0; i < std::numeric_limits<Uint16>::max(); i++)
	{
		offset.x = random_uint32() % 1024;
		offset.y = random_uint32() % 1024;
		offset.z = random_uint32() % 1024;
		offset.w = random_uint32() % 4;

		if (offset.x == 0)
		{
			offset.w = offset.w & 0x02;
		}

		if (offset.y == 0)
		{
			offset.w = offset.w & 0x01;
		}

		temp = el::AbstractTerrain::get_offset_scaled_rgb10_a2(offset);

		BOOST_CHECK_GE(temp.x, -scale - el::epsilon);
		BOOST_CHECK_LE(temp.x, scale + el::epsilon);

		BOOST_CHECK_GE(temp.y, -scale - el::epsilon);
		BOOST_CHECK_LE(temp.y, scale + el::epsilon);

		BOOST_CHECK_GE(temp.z, 0.0f - el::epsilon);
		BOOST_CHECK_LE(temp.z, scale + el::epsilon);

		tmp = el::AbstractTerrain::get_value_scaled_rgb10_a2(temp);

		BOOST_CHECK_EQUAL(offset.x, tmp.x);
		BOOST_CHECK_EQUAL(offset.y, tmp.y);
		BOOST_CHECK_EQUAL(offset.z, tmp.z);
		BOOST_CHECK_EQUAL(offset.w, tmp.w);
	}
}

BOOST_AUTO_TEST_CASE(convert_rgb10_a2)
{
	boost::mt19937 rng;
	boost::uniform_int<Uint32> range(0, std::numeric_limits<Uint32>::max());
	boost::variate_generator<boost::mt19937&, boost::uniform_int<Uint32> >
		random_uint32(rng, range);
	glm::uvec4 offset, tmp;
	glm::vec3 temp;
	Uint32 i;

	for (i = 0; i < std::numeric_limits<Uint16>::max(); i++)
	{
		offset.x = random_uint32() % 1024;
		offset.y = random_uint32() % 1024;
		offset.z = random_uint32() % 1024;
		offset.w = random_uint32() % 4;

		if (offset.x == 0)
		{
			offset.w = offset.w & 0x02;
		}

		if (offset.y == 0)
		{
			offset.w = offset.w & 0x01;
		}

		temp = el::AbstractTerrain::get_offset_rgb10_a2(offset);

		BOOST_CHECK_GE(temp.x, -1.0f - el::epsilon);
		BOOST_CHECK_LE(temp.x, 1.0f + el::epsilon);

		BOOST_CHECK_GE(temp.y, -1.0f - el::epsilon);
		BOOST_CHECK_LE(temp.y, 1.0f + el::epsilon);

		BOOST_CHECK_GE(temp.z, 0.0f - el::epsilon);
		BOOST_CHECK_LE(temp.z, 1.0f + el::epsilon);

		tmp = el::AbstractTerrain::get_value_rgb10_a2(temp);

		BOOST_CHECK_EQUAL(offset.x, tmp.x);
		BOOST_CHECK_EQUAL(offset.y, tmp.y);
		BOOST_CHECK_EQUAL(offset.z, tmp.z);
		BOOST_CHECK_EQUAL(offset.w, tmp.w);
	}
}

BOOST_AUTO_TEST_CASE(convert_packed_scaled_rgb10_a2)
{
	boost::mt19937 rng;
	boost::uniform_int<Uint32> range(0, std::numeric_limits<Uint32>::max());
	boost::variate_generator<boost::mt19937&, boost::uniform_int<Uint32> >
		random_uint32(rng, range);
	glm::uvec4 offset, tmp;
	glm::vec3 temp;
	float scale;
	Uint32 i, value;

	scale = el::AbstractTerrain::get_vector_scale();

	for (i = 0; i < std::numeric_limits<Uint16>::max(); i++)
	{
		offset.x = random_uint32() % 1024;
		offset.y = random_uint32() % 1024;
		offset.z = random_uint32() % 1024;
		offset.w = random_uint32() % 4;

		if (offset.x == 0)
		{
			offset.w = offset.w & 0x02;
		}

		if (offset.y == 0)
		{
			offset.w = offset.w & 0x01;
		}

		value = el::PackTool::pack_uint_10_10_10_2(false,
			glm::vec4(offset));

		temp = el::AbstractTerrain::get_offset_scaled_rgb10_a2(value);

		BOOST_CHECK_GE(temp.x, -scale - el::epsilon);
		BOOST_CHECK_LE(temp.x, scale + el::epsilon);

		BOOST_CHECK_GE(temp.y, -scale - el::epsilon);
		BOOST_CHECK_LE(temp.y, scale + el::epsilon);

		BOOST_CHECK_GE(temp.z, 0.0f - el::epsilon);
		BOOST_CHECK_LE(temp.z, scale + el::epsilon);

		tmp = el::AbstractTerrain::get_value_scaled_rgb10_a2(temp);

		BOOST_CHECK_EQUAL(offset.x, tmp.x);
		BOOST_CHECK_EQUAL(offset.y, tmp.y);
		BOOST_CHECK_EQUAL(offset.z, tmp.z);
		BOOST_CHECK_EQUAL(offset.w, tmp.w);
	}
}
