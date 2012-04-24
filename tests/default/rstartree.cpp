/****************************************************************************
 *            rstartree.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "prerequisites.hpp"
#include "rstartree.hpp"
#include "abstractnodevisitor.hpp"
#include <boost/random.hpp>
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

	class ContainsNodeVisitor: public AbstractNodeVisitor
	{
		private:
			BoundingBox m_bounding_box;

		public:
			ContainsNodeVisitor();
			virtual ~ContainsNodeVisitor() throw();
			virtual bool operator()(
				const BoundedObjectSharedPtr &node);
			void set_bounding_box(const BoundingBox &bounding_box);

	};

	ContainsNodeVisitor::ContainsNodeVisitor()
	{
	}

	ContainsNodeVisitor::~ContainsNodeVisitor() throw()
	{
	}

	bool ContainsNodeVisitor::operator()(const BoundedObjectSharedPtr &node)
	{
		return m_bounding_box.contains(node->get_bounding_box());
	}

	void ContainsNodeVisitor::set_bounding_box(
		const BoundingBox &bounding_box)
	{
		m_bounding_box = bounding_box;
	}

	class CountBoundedBoxNodeVisitor: public AbstractNodeVisitor
	{
		private:
			BoundingBox m_bounding_box;
			Uint32 m_count;
			Uint32 m_index;

		public:
			CountBoundedBoxNodeVisitor();
			virtual ~CountBoundedBoxNodeVisitor() throw();
			virtual bool operator()(
				const BoundedObjectSharedPtr &node);
			void set_count(const Uint32 count);
			void reset_index();
			const BoundingBox &get_bounding_box() const;

	};

	CountBoundedBoxNodeVisitor::CountBoundedBoxNodeVisitor()
	{
	}

	CountBoundedBoxNodeVisitor::~CountBoundedBoxNodeVisitor() throw()
	{
	}

	bool CountBoundedBoxNodeVisitor::operator()(
		const BoundedObjectSharedPtr &node)
	{
		if (m_count == m_index)
		{
			m_bounding_box = node->get_bounding_box();

			return true;
		}

		++m_index;

		return false;
	}

	void CountBoundedBoxNodeVisitor::set_count(const Uint32 count)
	{
		m_count = count;
	}

	void CountBoundedBoxNodeVisitor::reset_index()
	{
		m_index = 0;
	}

	const BoundingBox &CountBoundedBoxNodeVisitor::get_bounding_box() const
	{
		return m_bounding_box;
	}

}

namespace el = eternal_lands;

BOOST_AUTO_TEST_CASE(default_creation)
{
	boost::scoped_ptr<el::RStarTree> tree;

	BOOST_CHECK_NO_THROW(tree.reset(new el::RStarTree()));
}

BOOST_AUTO_TEST_CASE(add_objects)
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
		BOOST_CHECK(tree->check_tree());
	}

	for (i = 0; i < 16384; ++i)
	{
		BOOST_CHECK_EQUAL(boxes[i].use_count(), 2);
	}
}

BOOST_AUTO_TEST_CASE(add_remove_objects)
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
		BOOST_CHECK(tree->check_tree());
	}

	for (i = 0; i < 16384; ++i)
	{
		BOOST_CHECK_EQUAL(boxes[i].use_count(), 2);
		BOOST_CHECK_NO_THROW(tree->remove(boxes[i]));
		BOOST_CHECK_EQUAL(boxes[i].use_count(), 1);
		BOOST_CHECK(tree->check_tree());
	}

	for (i = 0; i < 16384; ++i)
	{
		BOOST_CHECK_EQUAL(boxes[i].use_count(), 1);
	}
}

BOOST_AUTO_TEST_CASE(random_add_remove_objects)
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
		BOOST_CHECK(tree->check_tree());
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
		BOOST_CHECK(tree->check_tree());
	}

	for (i = 0; i < 16384; ++i)
	{
		BOOST_CHECK_EQUAL(boxes[i].use_count(), 1);
	}
}

BOOST_AUTO_TEST_CASE(bounding_box)
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
		BOOST_CHECK(tree->check_tree());
		BOOST_CHECK(tree->get_bounding_box().contains(
			boxes[i]->get_bounding_box()));
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
		BOOST_CHECK(tree->get_bounding_box().contains(
			boxes[i]->get_bounding_box()));
		BOOST_CHECK_EQUAL(boxes[i].use_count(), 2);
		BOOST_CHECK_NO_THROW(tree->remove(boxes[i]));
		BOOST_CHECK_EQUAL(boxes[i].use_count(), 1);
		BOOST_CHECK(tree->check_tree());
	}

	for (i = 0; i < 16384; ++i)
	{
		BOOST_CHECK_EQUAL(boxes[i].use_count(), 1);
	}
}

BOOST_AUTO_TEST_CASE(random_add_remove_optimize_objects)
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
		BOOST_CHECK(tree->check_tree());
	}

	BOOST_CHECK_NO_THROW(tree->optimize(1024));

	BOOST_CHECK(tree->check_tree());

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
		BOOST_CHECK(tree->check_tree());
	}

	for (i = 0; i < 16384; ++i)
	{
		BOOST_CHECK_EQUAL(boxes[i].use_count(), 1);
	}
}

BOOST_AUTO_TEST_CASE(add_select_all_objects)
{
	boost::mt19937 rng;
	boost::uniform_int<Sint32> range(-16777216, 16777216);
	boost::variate_generator<boost::mt19937&, boost::uniform_int<Sint32> >
		random_int(rng, range);
	boost::uniform_real<float> float_range(0.0f, 1.0f);
	boost::variate_generator<boost::mt19937&, boost::uniform_real<float> >
		random_float(rng, float_range);
	boost::scoped_ptr<el::RStarTree> tree;
	el::SimpleBoundedObjectSharedPtr box;
	el::BoundingBox bounding_box;
	std::vector<el::SimpleBoundedObjectSharedPtr> boxes;
	boost::scoped_ptr<el::ContainsNodeVisitor> visitor;
	el::BoundedObjectSharedPtrVector objects;
	glm::vec3 min, max;
	glm::vec3 size, offset, center, half_size;
	Uint32 i, count;

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
		BOOST_CHECK(tree->check_tree());
	}

	min = tree->get_bounding_box().get_min();
	max = tree->get_bounding_box().get_max();

	BOOST_CHECK_NO_THROW(visitor.reset(new el::ContainsNodeVisitor()));

	bounding_box = el::BoundingBox(min, max);

	BOOST_CHECK_NO_THROW(visitor->set_bounding_box(bounding_box));
	BOOST_CHECK_NO_THROW(tree->select_objects(*visitor, objects));
	BOOST_CHECK(tree->check_tree());
	BOOST_CHECK_EQUAL(objects.size(), boxes.size());

	count = objects.size();

	for (i = 0; i < count; ++i)
	{
		BOOST_CHECK(bounding_box.contains(
			objects[i]->get_bounding_box()));
		BOOST_CHECK_EQUAL(objects[i].use_count(), 3);
	}

	objects.clear();

	count = boxes.size();

	for (i = 0; i < count; ++i)
	{
		BOOST_CHECK_EQUAL(boxes[i].use_count(), 2);
	}
}

BOOST_AUTO_TEST_CASE(add_random_select_objects)
{
	boost::mt19937 rng;
	boost::uniform_int<Sint32> range(-16777216, 16777216);
	boost::variate_generator<boost::mt19937&, boost::uniform_int<Sint32> >
		random_int(rng, range);
	boost::uniform_real<float> float_range(0.0f, 1.0f);
	boost::variate_generator<boost::mt19937&, boost::uniform_real<float> >
		random_float(rng, float_range);
	boost::scoped_ptr<el::RStarTree> tree;
	el::SimpleBoundedObjectSharedPtr box;
	el::BoundingBox bounding_box;
	std::vector<el::SimpleBoundedObjectSharedPtr> boxes;
	boost::scoped_ptr<el::ContainsNodeVisitor> visitor;
	el::BoundedObjectSharedPtrVector objects;
	glm::vec3 min, max;
	glm::vec3 size, offset, center, half_size;
	Uint32 i, j, count;

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
		BOOST_CHECK(tree->check_tree());
	}

	min = tree->get_bounding_box().get_min();
	max = tree->get_bounding_box().get_max();

	BOOST_CHECK_NO_THROW(visitor.reset(new el::ContainsNodeVisitor()));

	size = max - min;

	for (i = 0; i < 256; ++i)
	{
		center = min;

		for (j = 0; j < 3; ++j)
		{
			offset[j] = random_float() * size[j];
			half_size[j] = random_float() *
				(max[j] - offset[j]) * 0.5f;
		}

		center += offset;

		bounding_box.set_center(center);
		bounding_box.set_half_size(half_size);

		BOOST_CHECK_NO_THROW(visitor->set_bounding_box(bounding_box));
		BOOST_CHECK_NO_THROW(tree->select_objects(*visitor, objects));
		BOOST_CHECK(tree->check_tree());

		count = objects.size();

		for (j = 0; j < count; ++j)
		{
			BOOST_CHECK(bounding_box.contains(
				objects[j]->get_bounding_box()));
			BOOST_CHECK_EQUAL(objects[j].use_count(), 3);
		}

		objects.clear();

		count = boxes.size();

		for (j = 0; j < count; ++j)
		{
			BOOST_CHECK_EQUAL(boxes[j].use_count(), 2);
		}

		BOOST_CHECK(tree->check_tree());
	}
}

BOOST_AUTO_TEST_CASE(add_random_select_count_objects)
{
	boost::mt19937 rng;
	boost::uniform_int<Sint32> range(-16777216, 16777216);
	boost::variate_generator<boost::mt19937&, boost::uniform_int<Sint32> >
		random_int(rng, range);
	boost::uniform_real<float> float_range(0.0f, 1.0f);
	boost::variate_generator<boost::mt19937&, boost::uniform_real<float> >
		random_float(rng, float_range);
	boost::scoped_ptr<el::RStarTree> tree;
	el::SimpleBoundedObjectSharedPtr box;
	el::BoundingBox bounding_box;
	std::vector<el::SimpleBoundedObjectSharedPtr> boxes;
	boost::scoped_ptr<el::CountBoundedBoxNodeVisitor> visitor;
	el::BoundedObjectSharedPtrVector objects;
	glm::vec3 min, max;
	Uint32 i, j, count;

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
		BOOST_CHECK(tree->check_tree());
	}

	BOOST_CHECK_NO_THROW(visitor.reset(
		new el::CountBoundedBoxNodeVisitor()));

	for (i = 0; i < 256; ++i)
	{
		count = random_float() * tree->get_nodes_count();

		BOOST_CHECK_NO_THROW(visitor->set_count(count));
		BOOST_CHECK_NO_THROW(visitor->reset_index());
		BOOST_CHECK_NO_THROW(tree->select_objects(*visitor, objects));
		BOOST_CHECK(tree->check_tree());

		bounding_box = visitor->get_bounding_box();

		count = objects.size();

		for (j = 0; j < count; ++j)
		{
			BOOST_CHECK(bounding_box.contains(
				objects[j]->get_bounding_box()));
			BOOST_CHECK_EQUAL(objects[j].use_count(), 3);
		}

		objects.clear();

		count = boxes.size();

		for (j = 0; j < count; ++j)
		{
			BOOST_CHECK_EQUAL(boxes[j].use_count(), 2);
		}

		BOOST_CHECK(tree->check_tree());
	}
}

BOOST_AUTO_TEST_CASE(add_select_and_remove_all_objects)
{
	boost::mt19937 rng;
	boost::uniform_int<Sint32> range(-16777216, 16777216);
	boost::variate_generator<boost::mt19937&, boost::uniform_int<Sint32> >
		random_int(rng, range);
	boost::uniform_real<float> float_range(0.0f, 1.0f);
	boost::variate_generator<boost::mt19937&, boost::uniform_real<float> >
		random_float(rng, float_range);
	boost::scoped_ptr<el::RStarTree> tree;
	el::SimpleBoundedObjectSharedPtr box;
	el::BoundingBox bounding_box;
	std::vector<el::SimpleBoundedObjectSharedPtr> boxes;
	boost::scoped_ptr<el::ContainsNodeVisitor> visitor;
	el::BoundedObjectSharedPtrVector objects;
	glm::vec3 min, max;
	glm::vec3 size, offset, center, half_size;
	Uint32 i, count;

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
		BOOST_CHECK(tree->check_tree());
	}

	min = tree->get_bounding_box().get_min();
	max = tree->get_bounding_box().get_max();

	BOOST_CHECK_NO_THROW(visitor.reset(new el::ContainsNodeVisitor()));

	bounding_box = el::BoundingBox(min, max);

	BOOST_CHECK_NO_THROW(visitor->set_bounding_box(bounding_box));
	BOOST_CHECK_NO_THROW(tree->select_and_remove_objects(*visitor,
		objects));
	BOOST_CHECK(tree->check_tree());
	BOOST_CHECK_EQUAL(objects.size(), boxes.size());

	count = objects.size();

	for (i = 0; i < count; ++i)
	{
		BOOST_CHECK(bounding_box.contains(
			objects[i]->get_bounding_box()));
		BOOST_CHECK_EQUAL(objects[i].use_count(), 2);
	}

	objects.clear();

	count = boxes.size();

	for (i = 0; i < count; ++i)
	{
		BOOST_CHECK_EQUAL(boxes[i].use_count(), 1);
	}

	BOOST_CHECK(tree->check_tree());
}

BOOST_AUTO_TEST_CASE(add_random_select_and_remove_objects)
{
	boost::mt19937 rng;
	boost::uniform_int<Sint32> range(-16777216, 16777216);
	boost::variate_generator<boost::mt19937&, boost::uniform_int<Sint32> >
		random_int(rng, range);
	boost::uniform_real<float> float_range(0.0f, 1.0f);
	boost::variate_generator<boost::mt19937&, boost::uniform_real<float> >
		random_float(rng, float_range);
	boost::scoped_ptr<el::RStarTree> tree;
	el::SimpleBoundedObjectSharedPtr box;
	el::BoundingBox bounding_box;
	std::vector<el::SimpleBoundedObjectSharedPtr> boxes;
	boost::scoped_ptr<el::ContainsNodeVisitor> visitor;
	el::BoundedObjectSharedPtrVector objects;
	glm::vec3 min, max;
	glm::vec3 size, offset, center, half_size;
	Uint32 i, j, count;

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
		BOOST_CHECK(tree->check_tree());
	}

	min = tree->get_bounding_box().get_min();
	max = tree->get_bounding_box().get_max();

	BOOST_CHECK_NO_THROW(visitor.reset(new el::ContainsNodeVisitor()));

	size = max - min;

	for (i = 0; i < 256; ++i)
	{
		center = min;

		for (j = 0; j < 3; ++j)
		{
			offset[j] = random_float() * size[j];
			half_size[j] = random_float() *
				(max[j] - offset[j]) * 0.5f;
		}

		center += offset;

		bounding_box.set_center(center);
		bounding_box.set_half_size(half_size);

		BOOST_CHECK_NO_THROW(visitor->set_bounding_box(bounding_box));
		BOOST_CHECK_NO_THROW(tree->select_and_remove_objects(*visitor,
			objects));
		BOOST_CHECK(tree->check_tree());

		count = objects.size();

		for (j = 0; j < count; ++j)
		{
			BOOST_CHECK(bounding_box.contains(
				objects[j]->get_bounding_box()));
			BOOST_CHECK_EQUAL(objects[j].use_count(), 2);
		}

		objects.clear();

		count = boxes.size();

		for (j = 0; j < count; ++j)
		{
			BOOST_CHECK_LE(boxes[j].use_count(), 2);
		}

		BOOST_CHECK(tree->check_tree());
	}
}

BOOST_AUTO_TEST_CASE(add_random_select_and_remove_count_objects)
{
	boost::mt19937 rng;
	boost::uniform_int<Sint32> range(-16777216, 16777216);
	boost::variate_generator<boost::mt19937&, boost::uniform_int<Sint32> >
		random_int(rng, range);
	boost::uniform_real<float> float_range(0.0f, 1.0f);
	boost::variate_generator<boost::mt19937&, boost::uniform_real<float> >
		random_float(rng, float_range);
	boost::scoped_ptr<el::RStarTree> tree;
	el::SimpleBoundedObjectSharedPtr box;
	el::BoundingBox bounding_box;
	std::vector<el::SimpleBoundedObjectSharedPtr> boxes;
	boost::scoped_ptr<el::CountBoundedBoxNodeVisitor> visitor;
	el::BoundedObjectSharedPtrVector objects;
	glm::vec3 min, max;
	Uint32 i, j, count;

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
		BOOST_CHECK(tree->check_tree());
	}

	BOOST_CHECK_NO_THROW(visitor.reset(
		new el::CountBoundedBoxNodeVisitor()));

	for (i = 0; i < 256; ++i)
	{
		count = random_float() * tree->get_nodes_count();

		BOOST_CHECK_NO_THROW(visitor->set_count(count));
		BOOST_CHECK_NO_THROW(visitor->reset_index());
		BOOST_CHECK_NO_THROW(tree->select_and_remove_objects(*visitor,
			objects));
		BOOST_CHECK(tree->check_tree());

		bounding_box = visitor->get_bounding_box();

		count = objects.size();

		for (j = 0; j < count; ++j)
		{
			BOOST_CHECK(bounding_box.contains(
				objects[j]->get_bounding_box()));
			BOOST_CHECK_EQUAL(objects[j].use_count(), 2);
		}

		objects.clear();

		count = boxes.size();

		for (j = 0; j < count; ++j)
		{
			BOOST_CHECK_LE(boxes[j].use_count(), 2);
		}

		BOOST_CHECK(tree->check_tree());
	}
}
