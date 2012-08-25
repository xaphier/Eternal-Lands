/****************************************************************************
 *            transformation.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "prerequisites.hpp"
#include "transformation.hpp"
#include <boost/random.hpp>
#define BOOST_TEST_MODULE transformation
#include <boost/test/unit_test.hpp>

namespace el = eternal_lands;

BOOST_AUTO_TEST_CASE(default_creation)
{
	el::Transformation transformation;

	BOOST_CHECK_CLOSE(transformation.get_rotation().x, 0.0f, 0.001);
	BOOST_CHECK_CLOSE(transformation.get_rotation().y, 0.0f, 0.001);
	BOOST_CHECK_CLOSE(transformation.get_rotation().z, 0.0f, 0.001);
	BOOST_CHECK_CLOSE(transformation.get_rotation().w, 1.0f, 0.001);
	BOOST_CHECK_CLOSE(transformation.get_translation().x, 0.0f, 0.001);
	BOOST_CHECK_CLOSE(transformation.get_translation().y, 0.0f, 0.001);
	BOOST_CHECK_CLOSE(transformation.get_translation().z, 0.0f, 0.001);
	BOOST_CHECK_CLOSE(transformation.get_scale().x, 1.0f, 0.001);
	BOOST_CHECK_CLOSE(transformation.get_scale().y, 1.0f, 0.001);
	BOOST_CHECK_CLOSE(transformation.get_scale().z, 1.0f, 0.001);
}

BOOST_AUTO_TEST_CASE(rotate)
{
	boost::mt19937 rng;
	boost::uniform_int<Sint32> range(-16777216, 16777216);
	boost::variate_generator<boost::mt19937&, boost::uniform_int<Sint32> >
		random_int(rng, range);
	el::Transformation transformation;
	glm::quat rotation;
	Uint16 i;

	for (i = 0; i < std::numeric_limits<Uint16>::max(); ++i)
	{
		rotation.x = random_int() * 0.01f;
		rotation.y = random_int() * 0.01f;
		rotation.z = random_int() * 0.01f;
		rotation.w = random_int() * 0.01f;

		BOOST_CHECK_NO_THROW(transformation.set_rotation(rotation));

		BOOST_CHECK_CLOSE(transformation.get_rotation().x, rotation.x,
			0.001);
		BOOST_CHECK_CLOSE(transformation.get_rotation().y, rotation.y,
			0.001);
		BOOST_CHECK_CLOSE(transformation.get_rotation().z, rotation.z,
			0.001);
		BOOST_CHECK_CLOSE(transformation.get_rotation().w, rotation.w,
			0.001);
		BOOST_CHECK_CLOSE(transformation.get_translation().x, 0.0f,
			0.001);
		BOOST_CHECK_CLOSE(transformation.get_translation().y, 0.0f,
			0.001);
		BOOST_CHECK_CLOSE(transformation.get_translation().z, 0.0f,
			0.001);
		BOOST_CHECK_CLOSE(transformation.get_scale().x, 1.0f, 0.001);
		BOOST_CHECK_CLOSE(transformation.get_scale().y, 1.0f, 0.001);
		BOOST_CHECK_CLOSE(transformation.get_scale().z, 1.0f, 0.001);
	}
}

BOOST_AUTO_TEST_CASE(translate)
{
	boost::mt19937 rng;
	boost::uniform_int<Sint32> range(-16777216, 16777216);
	boost::variate_generator<boost::mt19937&, boost::uniform_int<Sint32> >
		random_int(rng, range);
	el::Transformation transformation;
	glm::vec3 translation;
	Uint16 i;

	for (i = 0; i < std::numeric_limits<Uint16>::max(); ++i)
	{
		translation.x = random_int() * 0.01f;
		translation.y = random_int() * 0.01f;
		translation.z = random_int() * 0.01f;

		BOOST_CHECK_NO_THROW(transformation.set_translation(
			translation));

		BOOST_CHECK_CLOSE(transformation.get_rotation().x, 0.0f, 0.001);
		BOOST_CHECK_CLOSE(transformation.get_rotation().y, 0.0f, 0.001);
		BOOST_CHECK_CLOSE(transformation.get_rotation().z, 0.0f, 0.001);
		BOOST_CHECK_CLOSE(transformation.get_rotation().w, 1.0f, 0.001);
		BOOST_CHECK_CLOSE(transformation.get_translation().x,
			translation.x, 0.001);
		BOOST_CHECK_CLOSE(transformation.get_translation().y,
			translation.y, 0.001);
		BOOST_CHECK_CLOSE(transformation.get_translation().z,
			translation.z, 0.001);
		BOOST_CHECK_CLOSE(transformation.get_scale().x, 1.0f, 0.001);
		BOOST_CHECK_CLOSE(transformation.get_scale().y, 1.0f, 0.001);
		BOOST_CHECK_CLOSE(transformation.get_scale().z, 1.0f, 0.001);
	}
}

BOOST_AUTO_TEST_CASE(scale)
{
	boost::mt19937 rng;
	boost::uniform_int<Sint32> range(-16777216, 16777216);
	boost::variate_generator<boost::mt19937&, boost::uniform_int<Sint32> >
		random_int(rng, range);
	el::Transformation transformation;
	glm::vec3 scale;
	Uint16 i;

	for (i = 0; i < std::numeric_limits<Uint16>::max(); ++i)
	{
		scale.x = random_int() * 0.01f;
		scale.y = random_int() * 0.01f;
		scale.z = random_int() * 0.01f;

		BOOST_CHECK_NO_THROW(transformation.set_scale(scale));

		BOOST_CHECK_CLOSE(transformation.get_rotation().x, 0.0f, 0.001);
		BOOST_CHECK_CLOSE(transformation.get_rotation().y, 0.0f, 0.001);
		BOOST_CHECK_CLOSE(transformation.get_rotation().z, 0.0f, 0.001);
		BOOST_CHECK_CLOSE(transformation.get_rotation().w, 1.0f, 0.001);
		BOOST_CHECK_CLOSE(transformation.get_translation().x, 0.0f,
			0.001);
		BOOST_CHECK_CLOSE(transformation.get_translation().y, 0.0f,
			0.001);
		BOOST_CHECK_CLOSE(transformation.get_translation().z, 0.0f,
			0.001);
		BOOST_CHECK_CLOSE(transformation.get_scale().x, scale.x,
			0.001);
		BOOST_CHECK_CLOSE(transformation.get_scale().y, scale.y,
			0.001);
		BOOST_CHECK_CLOSE(transformation.get_scale().z, scale.z,
			0.001);
	}
}

BOOST_AUTO_TEST_CASE(all)
{
	boost::mt19937 rng;
	boost::uniform_int<Sint32> range(-16777216, 16777216);
	boost::variate_generator<boost::mt19937&, boost::uniform_int<Sint32> >
		random_int(rng, range);
	el::Transformation transformation;
	glm::quat rotation;
	glm::vec3 translation, scale;
	Uint16 i;

	for (i = 0; i < std::numeric_limits<Uint16>::max(); ++i)
	{
		translation.x = random_int() * 0.01f;
		translation.y = random_int() * 0.01f;
		translation.z = random_int() * 0.01f;
		rotation.x = random_int() * 0.01f;
		rotation.y = random_int() * 0.01f;
		rotation.z = random_int() * 0.01f;
		rotation.w = random_int() * 0.01f;
		scale.x = random_int() * 0.01f;
		scale.y = random_int() * 0.01f;
		scale.z = random_int() * 0.01f;

		BOOST_CHECK_NO_THROW(transformation.set_translation(
			translation));
		BOOST_CHECK_NO_THROW(transformation.set_rotation(rotation));
		BOOST_CHECK_NO_THROW(transformation.set_scale(scale));

		BOOST_CHECK_CLOSE(transformation.get_rotation().x, rotation.x,
			0.001);
		BOOST_CHECK_CLOSE(transformation.get_rotation().y, rotation.y,
			0.001);
		BOOST_CHECK_CLOSE(transformation.get_rotation().z, rotation.z,
			0.001);
		BOOST_CHECK_CLOSE(transformation.get_rotation().w, rotation.w,
			0.001);
		BOOST_CHECK_CLOSE(transformation.get_translation().x,
			translation.x, 0.001);
		BOOST_CHECK_CLOSE(transformation.get_translation().y,
			translation.y, 0.001);
		BOOST_CHECK_CLOSE(transformation.get_translation().z,
			translation.z, 0.001);
		BOOST_CHECK_CLOSE(transformation.get_scale().x, scale.x,
			0.001);
		BOOST_CHECK_CLOSE(transformation.get_scale().y, scale.y,
			0.001);
		BOOST_CHECK_CLOSE(transformation.get_scale().z, scale.z,
			0.001);
	}
}

BOOST_AUTO_TEST_CASE(transform)
{
	boost::mt19937 rng_0, rng_1, rng_2;
	boost::uniform_int<Sint32> range(-65536, 65536);
	boost::variate_generator<boost::mt19937&, boost::uniform_int<Sint32> >
		random_int(rng_0, range);
	boost::uniform_int<Sint32> range_angle(-36000, 36000);
	boost::variate_generator<boost::mt19937&, boost::uniform_int<Sint32> >
		random_angle(rng_1, range_angle);
	boost::uniform_int<Sint32> range_scale(1, 10000);
	boost::variate_generator<boost::mt19937&, boost::uniform_int<Sint32> >
		random_scale(rng_2, range_scale);
	el::Transformation transformation;
	glm::mat4 tmp_matrix;
	glm::mat4x3 matrix;
	glm::quat rotation;
	glm::vec3 translation, scale, angle, point, p0, p1, p2, p3;
	Uint32 i;

	for (i = 0; i < std::numeric_limits<Uint16>::max(); ++i)
	{
		translation.x = random_int() * 0.1f;
		translation.y = random_int() * 0.1f;
		translation.z = random_int() * 0.1f;
		point.x = random_int() * 0.01f;
		point.y = random_int() * 0.01f;
		point.z = random_int() * 0.01f;
		angle.x = random_angle() * 0.01f;
		angle.y = random_angle() * 0.0f;
		angle.z = random_angle() * 0.01f;
		scale.x = random_int() * 0.01f;
		scale.y = random_int() * 0.01f;
		scale.z = random_int() * 0.01f;

		rotation = glm::quat();
		rotation = glm::rotate(rotation, angle.z,
			glm::vec3(0.0f, 0.0f, 1.0f));
		rotation = glm::rotate(rotation, angle.x,
			glm::vec3(1.0f, 0.0f, 0.0f));
		rotation = glm::rotate(rotation, angle.y,
			glm::vec3(0.0f, 1.0f, 0.0f));	

		BOOST_CHECK_NO_THROW(transformation.set_translation(
			translation));
		BOOST_CHECK_NO_THROW(transformation.set_rotation(rotation));
		BOOST_CHECK_NO_THROW(transformation.set_scale(scale));

		BOOST_CHECK_CLOSE(transformation.get_rotation().x, rotation.x,
			0.001);
		BOOST_CHECK_CLOSE(transformation.get_rotation().y, rotation.y,
			0.001);
		BOOST_CHECK_CLOSE(transformation.get_rotation().z, rotation.z,
			0.001);
		BOOST_CHECK_CLOSE(transformation.get_rotation().w, rotation.w,
			0.001);
		BOOST_CHECK_CLOSE(transformation.get_translation().x,
			translation.x, 0.001);
		BOOST_CHECK_CLOSE(transformation.get_translation().y,
			translation.y, 0.001);
		BOOST_CHECK_CLOSE(transformation.get_translation().z,
			translation.z, 0.001);
		BOOST_CHECK_CLOSE(transformation.get_scale().x, scale.x,
			0.001);
		BOOST_CHECK_CLOSE(transformation.get_scale().y, scale.y,
			0.001);
		BOOST_CHECK_CLOSE(transformation.get_scale().z, scale.z,
			0.001);

		tmp_matrix = glm::translate(translation);
		tmp_matrix = glm::rotate(tmp_matrix, angle.z,
			glm::vec3(0.0f, 0.0f, 1.0f));
		tmp_matrix = glm::rotate(tmp_matrix, angle.x,
			glm::vec3(1.0f, 0.0f, 0.0f));
		tmp_matrix = glm::rotate(tmp_matrix, angle.y,
			glm::vec3(0.0f, 1.0f, 0.0f));
		tmp_matrix = glm::scale(tmp_matrix, scale);

		matrix = glm::mat4x3(tmp_matrix);

		p0 = matrix * glm::vec4(point, 1.0f);
		p1 = transformation.get_matrix() * glm::vec4(point, 1.0f);
		p2 = transformation.transform_point(point);
		p3 = point;
		p3 *= scale;
		p3 = rotation * p3;
		p3 += translation;

		BOOST_CHECK_CLOSE(p0.x, p1.x, 7.5);
		BOOST_CHECK_CLOSE(p0.y, p1.y, 7.5);
		BOOST_CHECK_CLOSE(p0.z, p1.z, 7.5);

		BOOST_CHECK_CLOSE(p0.x, p2.x, 7.5);
		BOOST_CHECK_CLOSE(p0.y, p2.y, 7.5);
		BOOST_CHECK_CLOSE(p0.z, p2.z, 7.5);

		BOOST_CHECK_CLOSE(p0.x, p3.x, 7.5);
		BOOST_CHECK_CLOSE(p0.y, p3.y, 7.5);
		BOOST_CHECK_CLOSE(p0.z, p3.z, 7.5);

		BOOST_CHECK_CLOSE(p1.x, p2.x, 7.5);
		BOOST_CHECK_CLOSE(p1.y, p2.y, 7.5);
		BOOST_CHECK_CLOSE(p1.z, p2.z, 7.5);

		BOOST_CHECK_CLOSE(p1.x, p3.x, 7.5);
		BOOST_CHECK_CLOSE(p1.y, p3.y, 7.5);
		BOOST_CHECK_CLOSE(p1.z, p3.z, 7.5);

		BOOST_CHECK_CLOSE(p2.x, p3.x, 7.5);
		BOOST_CHECK_CLOSE(p2.y, p3.y, 7.5);
		BOOST_CHECK_CLOSE(p2.z, p3.z, 7.5);
	}
}
