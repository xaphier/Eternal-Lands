/****************************************************************************
 *            abstractterrainmanager.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "prerequisites.hpp"
#include "abstractterrainmanager.hpp"
#include <boost/random.hpp>
#define BOOST_TEST_MODULE light
#include <boost/test/unit_test.hpp>

namespace el = eternal_lands;

BOOST_AUTO_TEST_CASE(get_vector_scale)
{
	BOOST_CHECK_GT(el::AbstractTerrainManager::get_vector_scale().x, 0.0f);
	BOOST_CHECK_GT(el::AbstractTerrainManager::get_vector_scale().y, 0.0f);
	BOOST_CHECK_GT(el::AbstractTerrainManager::get_vector_scale().z, 0.0f);
}

BOOST_AUTO_TEST_CASE(get_tile_size)
{
	BOOST_CHECK_GT(el::AbstractTerrainManager::get_tile_size(), 1);
	BOOST_CHECK_EQUAL(__builtin_popcount(
		el::AbstractTerrainManager::get_tile_size()), 1);
}

BOOST_AUTO_TEST_CASE(get_patch_scale)
{
	BOOST_CHECK_GT(el::AbstractTerrainManager::get_patch_scale(), 0.0f);
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

		temp = el::AbstractTerrainManager::get_offset_rgb10_a2(tmp);

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

	scale = el::AbstractTerrainManager::get_vector_scale();

	for (i = 0; i < std::numeric_limits<Uint16>::max(); i++)
	{
		tmp.x = random_uint32() % 1024;
		tmp.y = random_uint32() % 1024;
		tmp.z = random_uint32() % 1024;
		tmp.w = random_uint32() % 4;

		temp = el::AbstractTerrainManager::get_offset_scaled_rgb10_a2(
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

	scale = el::AbstractTerrainManager::get_vector_scale();

	for (i = 0; i < std::numeric_limits<Uint16>::max(); i++)
	{
		tmp.x = random_float() * 2.0f - 1.0f;
		tmp.y = random_float() * 2.0f - 1.0f;
		tmp.z = random_float();

		temp = el::AbstractTerrainManager::get_value_scaled_rgb10_a2(
			tmp * scale);

		scale = el::AbstractTerrainManager::get_vector_scale();

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

		temp = el::AbstractTerrainManager::get_value_rgb10_a2(tmp);

		BOOST_CHECK_LE(temp.x, 1023);
		BOOST_CHECK_LE(temp.y, 1023);
		BOOST_CHECK_LE(temp.z, 1023);
		BOOST_CHECK_LE(temp.w, 3);
	}
}
