/****************************************************************************
 *            glmutil.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "prerequisites.hpp"
#include "glmutil.hpp"
#include <boost/random.hpp>
#define BOOST_TEST_MODULE glmutil
#include <boost/test/unit_test.hpp>

using namespace eternal_lands;

BOOST_AUTO_TEST_CASE(quat_in_out)
{
	boost::mt19937 rng;
	boost::uniform_int<Sint32> range(-16777216, 16777216);
	boost::variate_generator<boost::mt19937&, boost::uniform_int<Sint32> >
		random_int(rng, range);
	glm::quat tmp0, tmp1;
	Uint16 i, j;

	for (i = 0; i < 4096; ++i)
	{
		StringStream str;

		for (j = 0; j < 4; ++j)
		{
			tmp0[j] = random_int() * 0.01f;
		}

		BOOST_CHECK_NO_THROW(str << tmp0);
		BOOST_CHECK_NO_THROW(str >> tmp1);

		for (j = 0; j < 4; ++j)
		{
			BOOST_CHECK_CLOSE(tmp1[j], tmp0[j], 0.001);
		}
	}
}

typedef boost::mpl::list<glm::vec2, glm::vec3, glm::vec4> vec_types;

BOOST_AUTO_TEST_CASE_TEMPLATE(vec_in_out, T, vec_types)
{
	boost::mt19937 rng;
	boost::uniform_int<Sint32> range(-16777216, 16777216);
	boost::variate_generator<boost::mt19937&, boost::uniform_int<Sint32> >
		random_int(rng, range);
	T tmp0, tmp1;
	Uint16 i, j;

	for (i = 0; i < 4096; ++i)
	{
		StringStream str;

		for (j = 0; j < tmp0.length(); ++j)
		{
			tmp0[j] = random_int() * 0.01f;
		}

		BOOST_CHECK_NO_THROW(str << tmp0);
		BOOST_CHECK_NO_THROW(str >> tmp1);

		for (j = 0; j < tmp0.length(); ++j)
		{
			BOOST_CHECK_CLOSE(tmp1[j], tmp0[j], 0.001);
		}
	}
}

typedef boost::mpl::list<glm::ivec2, glm::ivec3, glm::ivec4> ivec_types;

BOOST_AUTO_TEST_CASE_TEMPLATE(ivec_in_out, T, ivec_types)
{
	boost::mt19937 rng;
	boost::uniform_int<Sint32> range(-16777216, 16777216);
	boost::variate_generator<boost::mt19937&, boost::uniform_int<Sint32> >
		random_int(rng, range);
	T tmp0, tmp1;
	Uint16 i, j;

	for (i = 0; i < 4096; ++i)
	{
		StringStream str;

		for (j = 0; j < tmp0.length(); ++j)
		{
			tmp0[j] = random_int();
		}

		BOOST_CHECK_NO_THROW(str << tmp0);
		BOOST_CHECK_NO_THROW(str >> tmp1);

		for (j = 0; j < tmp0.length(); ++j)
		{
			BOOST_CHECK_EQUAL(tmp1[j], tmp0[j]);
		}
	}
}

typedef boost::mpl::list<glm::uvec2, glm::uvec3, glm::uvec4> uvec_types;

BOOST_AUTO_TEST_CASE_TEMPLATE(uvec_in_out, T, uvec_types)
{
	boost::mt19937 rng;
	boost::uniform_int<Uint32> range(0, 16777216);
	boost::variate_generator<boost::mt19937&, boost::uniform_int<Uint32> >
		random_uint(rng, range);
	T tmp0, tmp1;
	Uint16 i, j;

	for (i = 0; i < 4096; ++i)
	{
		StringStream str;

		for (j = 0; j < tmp0.length(); ++j)
		{
			tmp0[j] = random_uint();
		}

		BOOST_CHECK_NO_THROW(str << tmp0);
		BOOST_CHECK_NO_THROW(str >> tmp1);

		for (j = 0; j < tmp0.length(); ++j)
		{
			BOOST_CHECK_EQUAL(tmp1[j], tmp0[j]);
		}
	}
}

typedef boost::mpl::list<glm::bvec2, glm::bvec3, glm::bvec4> bvec_types;

BOOST_AUTO_TEST_CASE_TEMPLATE(bvec_in_out, T, bvec_types)
{
	boost::mt19937 rng;
	boost::uniform_int<Uint32> range(0, 1);
	boost::variate_generator<boost::mt19937&, boost::uniform_int<Uint32> >
		random_uint(rng, range);
	T tmp0, tmp1;
	Uint16 i, j;

	for (i = 0; i < 4096; ++i)
	{
		StringStream str;

		for (j = 0; j < tmp0.length(); ++j)
		{
			tmp0[j] = random_uint() == 0;
		}

		BOOST_CHECK_NO_THROW(str << tmp0);
		BOOST_CHECK_NO_THROW(str >> tmp1);

		for (j = 0; j < tmp0.length(); ++j)
		{
			BOOST_CHECK_EQUAL(tmp1[j], tmp0[j]);
		}
	}
}

typedef boost::mpl::list<glm::mat2x2, glm::mat2x3, glm::mat2x4, glm::mat3x2,
	glm::mat3x3, glm::mat3x4, glm::mat4x2, glm::mat4x3, glm::mat4x4>
	mat_types;

BOOST_AUTO_TEST_CASE_TEMPLATE(mat_in_out, T, mat_types)
{
	boost::mt19937 rng;
	boost::uniform_int<Sint32> range(-16777216, 16777216);
	boost::variate_generator<boost::mt19937&, boost::uniform_int<Sint32> >
		random_int(rng, range);
	T tmp0, tmp1;
	Uint16 i, j, k;

	for (i = 0; i < 4096; ++i)
	{
		StringStream str;

		for (j = 0; j < tmp0.row_size(); ++j)
		{
			for (k = 0; k < tmp0.col_size(); ++k)
			{
				tmp0[j][k] = random_int() * 0.01f;
			}
		}

		BOOST_CHECK_NO_THROW(str << tmp0);
		BOOST_CHECK_NO_THROW(str >> tmp1);

		for (j = 0; j < tmp0.length(); ++j)
		{
			for (k = 0; k < tmp0.col_size(); ++k)
			{
				BOOST_CHECK_CLOSE(tmp1[j][k], tmp0[j][k],
					0.001);
			}
		}
	}
}
