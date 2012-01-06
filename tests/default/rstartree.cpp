/****************************************************************************
 *            rstartree.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "prerequisites.hpp"
#include "rstartree.hpp"
#include <glm/gtx/epsilon.hpp>
#include <boost/random.hpp>
#include <boost/exception/diagnostic_information.hpp>
#define BOOST_TEST_MODULE rstartree
#include <boost/test/unit_test.hpp>

namespace eternal_lands
{

	class SimpleBoundedObject: public BoundedObject
	{
		public:
			SimpleBoundedObject(const glm::vec3 &min,
				const glm::vec3 &max);
			virtual ~SimpleBoundedObject() throw();

	};

	SimpleBoundedObject::SimpleBoundedObject(const glm::vec3 &min,
		const glm::vec3 &max)
	{
		set_bounding_box(BoundingBox(min, max));
	}

	SimpleBoundedObject::~SimpleBoundedObject() throw()
	{
	}

	typedef boost::shared_ptr<SimpleBoundedObject>
		SimpleBoundedObjectSharedPtr;

}

namespace el = eternal_lands;

BOOST_AUTO_TEST_CASE(rstartree_default_creation)
{
	boost::scoped_ptr<el::RStarTree> tree;

	BOOST_CHECK_NO_THROW(tree.reset(new el::RStarTree()));
}

BOOST_AUTO_TEST_CASE(rstartree_add_objects)
{
	boost::mt19937 rng;
	boost::uniform_int<Sint32> range(-16777216, 16777216);
	boost::variate_generator<boost::mt19937&, boost::uniform_int<Sint32> >
		random_int(rng, range);
	boost::scoped_ptr<el::RStarTree> tree;
	el::SimpleBoundedObjectSharedPtr box;
	std::vector<el::SimpleBoundedObjectSharedPtr> boxes;
	glm::vec3 min, max;
	Uint32 i;

	BOOST_CHECK_NO_THROW(tree.reset(new el::RStarTree()));

	for (i = 0; i < 16384; ++i)
	{
		min.x = random_int() * 0.01f;
		min.y = random_int() * 0.01f;
		min.z = random_int() * 0.01f;

		max = min;
		max.x += std::abs(random_int() * 0.01f) + 1e-7f;
		max.y += std::abs(random_int() * 0.01f) + 1e-7f;
		max.z += std::abs(random_int() * 0.01f) + 1e-7f;

		box.reset(new el::SimpleBoundedObject(min, max));
		boxes.push_back(box);
		box.reset();

		BOOST_CHECK_NO_THROW(tree->add(boxes[i]));
		BOOST_CHECK_EQUAL(boxes[i].use_count(), 2);
	}

	for (i = 0; i < 16384; ++i)
	{
		BOOST_CHECK_EQUAL(boxes[i].use_count(), 2);
	}
}

BOOST_AUTO_TEST_CASE(rstartree_add_remove_objects)
{
	boost::mt19937 rng;
	boost::uniform_int<Sint32> range(-16777216, 16777216);
	boost::variate_generator<boost::mt19937&, boost::uniform_int<Sint32> >
		random_int(rng, range);
	boost::scoped_ptr<el::RStarTree> tree;
	std::vector<el::SimpleBoundedObjectSharedPtr> boxes;
	el::SimpleBoundedObjectSharedPtr box;
	glm::vec3 min, max;
	Uint32 i;

	BOOST_CHECK_NO_THROW(tree.reset(new el::RStarTree()));

	for (i = 0; i < 16384; ++i)
	{
		min.x = random_int() * 0.01f;
		min.y = random_int() * 0.01f;
		min.z = random_int() * 0.01f;

		max = min;
		max.x += std::abs(random_int() * 0.01f) + 1e-7f;
		max.y += std::abs(random_int() * 0.01f) + 1e-7f;
		max.z += std::abs(random_int() * 0.01f) + 1e-7f;

		box.reset(new el::SimpleBoundedObject(min, max));
		boxes.push_back(box);
		box.reset();

		BOOST_CHECK_EQUAL(boxes[i].use_count(), 1);
		BOOST_CHECK_NO_THROW(tree->add(boxes[i]));
		BOOST_CHECK_EQUAL(boxes[i].use_count(), 2);
	}

	for (i = 0; i < 16384; ++i)
	{
		BOOST_CHECK_EQUAL(boxes[i].use_count(), 2);
		BOOST_CHECK_NO_THROW(tree->remove(boxes[i]));
		BOOST_CHECK_EQUAL(boxes[i].use_count(), 1);
	}

	for (i = 0; i < 16384; ++i)
	{
		BOOST_CHECK_EQUAL(boxes[i].use_count(), 1);
	}
}

BOOST_AUTO_TEST_CASE(rstartree_random_add_remove_objects)
{
	boost::mt19937 rng;
	boost::uniform_int<Sint32> range(-16777216, 16777216);
	boost::variate_generator<boost::mt19937&, boost::uniform_int<Sint32> >
		random_int(rng, range);
	boost::uniform_int<Uint16> index_range(0, 16383);
	boost::variate_generator<boost::mt19937&, boost::uniform_int<Uint16> >
		random_index(rng, index_range);
	boost::scoped_ptr<el::RStarTree> tree;
	std::vector<el::SimpleBoundedObjectSharedPtr> boxes;
	el::SimpleBoundedObjectSharedPtr box;
	glm::vec3 min, max;
	Uint32 i, idx0, idx1;

	BOOST_CHECK_NO_THROW(tree.reset(new el::RStarTree()));

	for (i = 0; i < 16384; ++i)
	{
		min.x = random_int() * 0.01f;
		min.y = random_int() * 0.01f;
		min.z = random_int() * 0.01f;

		max = min;
		max.x += std::abs(random_int() * 0.01f) + 1e-7f;
		max.y += std::abs(random_int() * 0.01f) + 1e-7f;
		max.z += std::abs(random_int() * 0.01f) + 1e-7f;

		box.reset(new el::SimpleBoundedObject(min, max));
		boxes.push_back(box);
		box.reset();

		BOOST_CHECK_EQUAL(boxes[i].use_count(), 1);
		BOOST_CHECK_NO_THROW(tree->add(boxes[i]));
		BOOST_CHECK_EQUAL(boxes[i].use_count(), 2);
	}

	for (i = 0; i < 16384; ++i)
	{
		idx0 = random_index();
		idx1 = random_index();

		BOOST_CHECK_EQUAL(boxes[idx0].use_count(), 2);
		BOOST_CHECK_EQUAL(boxes[idx1].use_count(), 2);

		boost::swap(boxes[idx0], boxes[idx1]);
	}

	for (i = 0; i < 16384; ++i)
	{
		BOOST_CHECK_EQUAL(boxes[i].use_count(), 2);
		BOOST_CHECK_NO_THROW(tree->remove(boxes[i]));
		BOOST_CHECK_EQUAL(boxes[i].use_count(), 1);
	}

	for (i = 0; i < 16384; ++i)
	{
		BOOST_CHECK_EQUAL(boxes[i].use_count(), 1);
	}
}
