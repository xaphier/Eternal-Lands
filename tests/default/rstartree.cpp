/****************************************************************************
 *            rstartree.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "prerequisites.hpp"
#include "rstartree.hpp"
#include "abstractnodevisitor.hpp"
#include <boost/random.hpp>
#define BOOST_TEST_MODULE rstartree
#include <boost/test/unit_test.hpp>

namespace el = eternal_lands;

namespace
{

	class SimpleBoundedObject: public el::BoundedObject
	{
		public:
			SimpleBoundedObject(const glm::vec3 &min,
				const glm::vec3 &max);
			virtual ~SimpleBoundedObject() throw();

	};

	SimpleBoundedObject::SimpleBoundedObject(const glm::vec3 &min,
		const glm::vec3 &max)
	{
		set_bounding_box(el::BoundingBox(min, max));
	}

	SimpleBoundedObject::~SimpleBoundedObject() throw()
	{
	}

	typedef boost::shared_ptr<SimpleBoundedObject>
		SimpleBoundedObjectSharedPtr;

	class ContainsNodeVisitor: public el::AbstractNodeVisitor
	{
		private:
			el::BoundingBox m_bounding_box;

		public:
			ContainsNodeVisitor();
			virtual ~ContainsNodeVisitor() throw();
			virtual bool operator()(
				const el::BoundedObjectSharedPtr &node);
			void set_bounding_box(
				const el::BoundingBox &bounding_box);

	};

	ContainsNodeVisitor::ContainsNodeVisitor()
	{
	}

	ContainsNodeVisitor::~ContainsNodeVisitor() throw()
	{
	}

	bool ContainsNodeVisitor::operator()(
		const el::BoundedObjectSharedPtr &node)
	{
		return m_bounding_box.contains(node->get_bounding_box());
	}

	void ContainsNodeVisitor::set_bounding_box(
		const el::BoundingBox &bounding_box)
	{
		m_bounding_box = bounding_box;
	}

	class CountBoundedBoxNodeVisitor: public el::AbstractNodeVisitor
	{
		private:
			el::BoundingBox m_bounding_box;
			Uint32 m_count;
			Uint32 m_index;

		public:
			CountBoundedBoxNodeVisitor();
			virtual ~CountBoundedBoxNodeVisitor() throw();
			virtual bool operator()(
				const el::BoundedObjectSharedPtr &node);
			void set_count(const Uint32 count);
			void reset_index();
			const el::BoundingBox &get_bounding_box() const;

	};

	CountBoundedBoxNodeVisitor::CountBoundedBoxNodeVisitor()
	{
	}

	CountBoundedBoxNodeVisitor::~CountBoundedBoxNodeVisitor() throw()
	{
	}

	bool CountBoundedBoxNodeVisitor::operator()(
		const el::BoundedObjectSharedPtr &node)
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

	const el::BoundingBox &CountBoundedBoxNodeVisitor::get_bounding_box()
		const
	{
		return m_bounding_box;
	}

	typedef	std::vector<SimpleBoundedObjectSharedPtr>
		SimpleBoundedObjectSharedPtrVector;

	class BoundingBoxBuilder
	{
		private:
			SimpleBoundedObjectSharedPtrVector m_boxes;

		public:
			BoundingBoxBuilder();

			inline const SimpleBoundedObjectSharedPtrVector
				&get_boxes() const
			{
				return m_boxes;
			}

			inline const SimpleBoundedObjectSharedPtr
				&get_box(const Uint32 index) const
			{
				return m_boxes[index];
			}

			static inline Uint32 get_count()
			{
				return 1024;
			}

	};

	BoundingBoxBuilder::BoundingBoxBuilder()
	{
		boost::mt19937 rng;
		boost::uniform_int<Sint32> range(-16777216, 16777216);
		boost::variate_generator<boost::mt19937&,
			boost::uniform_int<Sint32> > random_int(rng, range);
		SimpleBoundedObjectSharedPtr box;
		glm::vec3 min, max;
		Uint32 i;

		for (i = 0; i < get_count(); ++i)
		{
			min.x = random_int() * 0.01f;
			min.y = random_int() * 0.01f;
			min.z = random_int() * 0.01f;

			max = min;
			max.x += std::abs(random_int() * 0.01f) + 1e-5f;
			max.y += std::abs(random_int() * 0.01f) + 1e-5f;
			max.z += std::abs(random_int() * 0.01f) + 1e-5f;

			box = boost::make_shared<SimpleBoundedObject>(min, max);
			m_boxes.push_back(box);
			box.reset();
		}
	}

}

BOOST_FIXTURE_TEST_SUITE(random_boxes, BoundingBoxBuilder)

BOOST_AUTO_TEST_CASE(default_creation)
{
	boost::scoped_ptr<el::RStarTree> tree;

	BOOST_CHECK_NO_THROW(tree.reset(new el::RStarTree()));
}

BOOST_AUTO_TEST_CASE(add_objects)
{
	boost::scoped_ptr<el::RStarTree> tree;
	Uint32 i;

	BOOST_CHECK_NO_THROW(tree.reset(new el::RStarTree()));

	for (i = 0; i < get_count(); ++i)
	{
		BOOST_CHECK_NO_THROW(tree->add(get_box(i)));
		BOOST_CHECK_EQUAL(get_box(i).use_count(), 2);
		BOOST_CHECK(tree->check_tree());
	}

	for (i = 0; i < get_count(); ++i)
	{
		BOOST_CHECK_EQUAL(get_box(i).use_count(), 2);
	}
}

BOOST_AUTO_TEST_CASE(add_remove_objects)
{
	boost::scoped_ptr<el::RStarTree> tree;
	Uint32 i;

	BOOST_CHECK_NO_THROW(tree.reset(new el::RStarTree()));

	for (i = 0; i < get_count(); ++i)
	{
		BOOST_CHECK_EQUAL(get_box(i).use_count(), 1);
		BOOST_CHECK_NO_THROW(tree->add(get_box(i)));
		BOOST_CHECK_EQUAL(get_box(i).use_count(), 2);
		BOOST_CHECK(tree->check_tree());
	}

	for (i = 0; i < get_count(); ++i)
	{
		BOOST_CHECK_EQUAL(get_box(i).use_count(), 2);
		BOOST_CHECK_NO_THROW(tree->remove(get_box(i)));
		BOOST_CHECK_EQUAL(get_box(i).use_count(), 1);
		BOOST_CHECK(tree->check_tree());
	}

	for (i = 0; i < get_count(); ++i)
	{
		BOOST_CHECK_EQUAL(get_box(i).use_count(), 1);
	}
}

BOOST_AUTO_TEST_CASE(random_add_remove_objects)
{
	boost::mt19937 rng;
	boost::uniform_int<Uint16> index_range(0, get_count() - 1);
	boost::variate_generator<boost::mt19937&, boost::uniform_int<Uint16> >
		random_index(rng, index_range);
	boost::scoped_ptr<el::RStarTree> tree;
	el::Uint32Vector index;
	Uint32 i, idx0, idx1, idx;

	BOOST_CHECK_NO_THROW(tree.reset(new el::RStarTree()));

	index.reserve(get_count());

	for (i = 0; i < get_count(); ++i)
	{
		BOOST_CHECK_EQUAL(get_box(i).use_count(), 1);
		BOOST_CHECK_NO_THROW(tree->add(get_box(i)));
		BOOST_CHECK_EQUAL(get_box(i).use_count(), 2);
		BOOST_CHECK(tree->check_tree());

		index.push_back(i);
	}

	for (i = 0; i < get_count(); ++i)
	{
		idx0 = random_index();
		idx1 = random_index();

		BOOST_CHECK_EQUAL(get_box(idx0).use_count(), 2);
		BOOST_CHECK_EQUAL(get_box(idx1).use_count(), 2);

		boost::swap(index[idx0], index[idx1]);
	}

	for (i = 0; i < get_count(); ++i)
	{
		idx = index[i];

		BOOST_CHECK_EQUAL(get_box(idx).use_count(), 2);
		BOOST_CHECK_NO_THROW(tree->remove(get_box(idx)));
		BOOST_CHECK_EQUAL(get_box(idx).use_count(), 1);
		BOOST_CHECK(tree->check_tree());
	}

	for (i = 0; i < get_count(); ++i)
	{
		BOOST_CHECK_EQUAL(get_box(i).use_count(), 1);
	}
}

BOOST_AUTO_TEST_CASE(bounding_box)
{
	boost::mt19937 rng;
	boost::uniform_int<Uint16> index_range(0, get_count() - 1);
	boost::variate_generator<boost::mt19937&, boost::uniform_int<Uint16> >
		random_index(rng, index_range);
	boost::scoped_ptr<el::RStarTree> tree;
	el::Uint32Vector index;
	Uint32 i, idx0, idx1, idx;

	BOOST_CHECK_NO_THROW(tree.reset(new el::RStarTree()));

	index.reserve(get_count());

	for (i = 0; i < get_count(); ++i)
	{
		BOOST_CHECK_EQUAL(get_box(i).use_count(), 1);
		BOOST_CHECK_NO_THROW(tree->add(get_box(i)));
		BOOST_CHECK_EQUAL(get_box(i).use_count(), 2);
		BOOST_CHECK(tree->check_tree());
		BOOST_CHECK(tree->get_bounding_box().contains(
			get_box(i)->get_bounding_box()));

		index.push_back(i);
	}

	for (i = 0; i < get_count(); ++i)
	{
		idx0 = random_index();
		idx1 = random_index();

		BOOST_CHECK_EQUAL(get_box(idx0).use_count(), 2);
		BOOST_CHECK_EQUAL(get_box(idx1).use_count(), 2);

		boost::swap(index[idx0], index[idx1]);
	}

	for (i = 0; i < get_count(); ++i)
	{
		idx = index[i];

		BOOST_CHECK(tree->get_bounding_box().contains(
			get_box(idx)->get_bounding_box()));
		BOOST_CHECK_EQUAL(get_box(idx).use_count(), 2);
		BOOST_CHECK_NO_THROW(tree->remove(get_box(idx)));
		BOOST_CHECK_EQUAL(get_box(idx).use_count(), 1);
		BOOST_CHECK(tree->check_tree());
	}

	for (i = 0; i < get_count(); ++i)
	{
		BOOST_CHECK_EQUAL(get_box(i).use_count(), 1);
	}
}

BOOST_AUTO_TEST_CASE(random_add_remove_optimize_objects)
{
	boost::mt19937 rng;
	boost::uniform_int<Uint16> index_range(0, get_count() - 1);
	boost::variate_generator<boost::mt19937&, boost::uniform_int<Uint16> >
		random_index(rng, index_range);
	boost::scoped_ptr<el::RStarTree> tree;
	el::Uint32Vector index;
	Uint32 i, idx0, idx1, idx;

	BOOST_CHECK_NO_THROW(tree.reset(new el::RStarTree()));

	index.reserve(get_count());

	for (i = 0; i < get_count(); ++i)
	{
		BOOST_CHECK_EQUAL(get_box(i).use_count(), 1);
		BOOST_CHECK_NO_THROW(tree->add(get_box(i)));
		BOOST_CHECK_EQUAL(get_box(i).use_count(), 2);
		BOOST_CHECK(tree->check_tree());
		BOOST_CHECK(tree->get_bounding_box().contains(
			get_box(i)->get_bounding_box()));

		index.push_back(i);
	}

	BOOST_CHECK_NO_THROW(tree->optimize(1024));

	BOOST_CHECK(tree->check_tree());

	for (i = 0; i < get_count(); ++i)
	{
		idx0 = random_index();
		idx1 = random_index();

		BOOST_CHECK_EQUAL(get_box(idx0).use_count(), 2);
		BOOST_CHECK_EQUAL(get_box(idx1).use_count(), 2);

		boost::swap(index[idx0], index[idx1]);
	}

	for (i = 0; i < get_count(); ++i)
	{
		idx = index[i];

		BOOST_CHECK(tree->get_bounding_box().contains(
			get_box(idx)->get_bounding_box()));
		BOOST_CHECK_EQUAL(get_box(idx).use_count(), 2);
		BOOST_CHECK_NO_THROW(tree->remove(get_box(idx)));
		BOOST_CHECK_EQUAL(get_box(idx).use_count(), 1);
		BOOST_CHECK(tree->check_tree());
	}

	for (i = 0; i < get_count(); ++i)
	{
		BOOST_CHECK_EQUAL(get_box(i).use_count(), 1);
	}
}

BOOST_AUTO_TEST_CASE(add_select_all_objects)
{
	boost::mt19937 rng;
	boost::uniform_real<float> float_range(0.0f, 1.0f);
	boost::variate_generator<boost::mt19937&, boost::uniform_real<float> >
		random_float(rng, float_range);
	boost::scoped_ptr<el::RStarTree> tree;
	el::BoundingBox bounding_box;
	boost::scoped_ptr<ContainsNodeVisitor> visitor;
	el::BoundedObjectSharedPtrVector objects;
	glm::vec3 min, max;
	glm::vec3 size, offset, center, half_size;
	Uint32 i, count;

	BOOST_CHECK_NO_THROW(tree.reset(new el::RStarTree()));

	for (i = 0; i < get_count(); ++i)
	{
		BOOST_CHECK_NO_THROW(tree->add(get_box(i)));
		BOOST_CHECK_EQUAL(get_box(i).use_count(), 2);
		BOOST_CHECK(tree->check_tree());
	}

	min = tree->get_bounding_box().get_min();
	max = tree->get_bounding_box().get_max();

	BOOST_CHECK_NO_THROW(visitor.reset(new ContainsNodeVisitor()));

	bounding_box = el::BoundingBox(min, max);

	BOOST_CHECK_NO_THROW(visitor->set_bounding_box(bounding_box));
	BOOST_CHECK_NO_THROW(tree->select_objects(*visitor, objects));
	BOOST_CHECK(tree->check_tree());
	BOOST_CHECK_EQUAL(objects.size(), get_boxes().size());

	count = objects.size();

	for (i = 0; i < count; ++i)
	{
		BOOST_CHECK(bounding_box.contains(
			objects[i]->get_bounding_box()));
		BOOST_CHECK_EQUAL(objects[i].use_count(), 3);
	}

	objects.clear();

	for (i = 0; i < get_count(); ++i)
	{
		BOOST_CHECK_EQUAL(get_box(i).use_count(), 2);
	}
}

BOOST_AUTO_TEST_CASE(add_random_select_objects)
{
	boost::mt19937 rng;
	boost::uniform_real<float> float_range(0.0f, 1.0f);
	boost::variate_generator<boost::mt19937&, boost::uniform_real<float> >
		random_float(rng, float_range);
	boost::scoped_ptr<el::RStarTree> tree;
	el::BoundingBox bounding_box;
	boost::scoped_ptr<ContainsNodeVisitor> visitor;
	el::BoundedObjectSharedPtrVector objects;
	glm::vec3 min, max;
	glm::vec3 size, offset, center, half_size;
	Uint32 i, j, count;

	BOOST_CHECK_NO_THROW(tree.reset(new el::RStarTree()));

	for (i = 0; i < get_count(); ++i)
	{
		BOOST_CHECK_NO_THROW(tree->add(get_box(i)));
		BOOST_CHECK_EQUAL(get_box(i).use_count(), 2);
		BOOST_CHECK(tree->check_tree());
	}

	min = tree->get_bounding_box().get_min();
	max = tree->get_bounding_box().get_max();

	BOOST_CHECK_NO_THROW(visitor.reset(new ContainsNodeVisitor()));

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

		for (j = 0; j < get_count(); ++j)
		{
			BOOST_CHECK_EQUAL(get_box(j).use_count(), 2);
		}
	}
}

BOOST_AUTO_TEST_CASE(add_random_select_count_objects)
{
	boost::mt19937 rng;
	boost::uniform_real<float> float_range(0.0f, 1.0f);
	boost::variate_generator<boost::mt19937&, boost::uniform_real<float> >
		random_float(rng, float_range);
	boost::scoped_ptr<el::RStarTree> tree;
	el::BoundingBox bounding_box;
	boost::scoped_ptr<CountBoundedBoxNodeVisitor> visitor;
	el::BoundedObjectSharedPtrVector objects;
	glm::vec3 min, max;
	Uint32 i, j, count;

	BOOST_CHECK_NO_THROW(tree.reset(new el::RStarTree()));

	for (i = 0; i < get_count(); ++i)
	{
		BOOST_CHECK_NO_THROW(tree->add(get_box(i)));
		BOOST_CHECK_EQUAL(get_box(i).use_count(), 2);
		BOOST_CHECK(tree->check_tree());
	}

	BOOST_CHECK_NO_THROW(visitor.reset(new CountBoundedBoxNodeVisitor()));

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
			BOOST_CHECK_EQUAL(objects[j].use_count(), 3);
		}

		objects.clear();

		for (j = 0; j < get_count(); ++j)
		{
			BOOST_CHECK_EQUAL(get_box(j).use_count(), 2);
		}
	}
}

BOOST_AUTO_TEST_CASE(add_select_and_remove_all_objects)
{
	boost::mt19937 rng;
	boost::uniform_real<float> float_range(0.0f, 1.0f);
	boost::variate_generator<boost::mt19937&, boost::uniform_real<float> >
		random_float(rng, float_range);
	boost::scoped_ptr<el::RStarTree> tree;
	el::BoundingBox bounding_box;
	boost::scoped_ptr<ContainsNodeVisitor> visitor;
	el::BoundedObjectSharedPtrVector objects;
	glm::vec3 min, max;
	glm::vec3 size, offset, center, half_size;
	Uint32 i, count;

	BOOST_CHECK_NO_THROW(tree.reset(new el::RStarTree()));

	for (i = 0; i < get_count(); ++i)
	{
		BOOST_CHECK_NO_THROW(tree->add(get_box(i)));
		BOOST_CHECK_EQUAL(get_box(i).use_count(), 2);
		BOOST_CHECK(tree->check_tree());
	}

	min = tree->get_bounding_box().get_min();
	max = tree->get_bounding_box().get_max();

	BOOST_CHECK_NO_THROW(visitor.reset(new ContainsNodeVisitor()));

	bounding_box = el::BoundingBox(min, max);

	BOOST_CHECK_NO_THROW(visitor->set_bounding_box(bounding_box));
	BOOST_CHECK_NO_THROW(tree->select_and_remove_objects(*visitor,
		objects));
	BOOST_CHECK(tree->check_tree());
	BOOST_CHECK_EQUAL(objects.size(), get_boxes().size());

	count = objects.size();

	for (i = 0; i < count; ++i)
	{
		BOOST_CHECK(bounding_box.contains(
			objects[i]->get_bounding_box()));
		BOOST_CHECK_EQUAL(objects[i].use_count(), 2);
	}

	objects.clear();

	for (i = 0; i < get_count(); ++i)
	{
		BOOST_CHECK_EQUAL(get_box(i).use_count(), 1);
	}
}

BOOST_AUTO_TEST_CASE(add_random_select_and_remove_objects)
{
	boost::mt19937 rng;
	boost::uniform_real<float> float_range(0.0f, 1.0f);
	boost::variate_generator<boost::mt19937&, boost::uniform_real<float> >
		random_float(rng, float_range);
	boost::scoped_ptr<el::RStarTree> tree;
	el::BoundingBox bounding_box;
	boost::scoped_ptr<ContainsNodeVisitor> visitor;
	el::BoundedObjectSharedPtrVector objects;
	glm::vec3 min, max;
	glm::vec3 size, offset, center, half_size;
	Uint32 i, j, count;

	BOOST_CHECK_NO_THROW(tree.reset(new el::RStarTree()));

	for (i = 0; i < get_count(); ++i)
	{
		BOOST_CHECK_NO_THROW(tree->add(get_box(i)));
		BOOST_CHECK_EQUAL(get_box(i).use_count(), 2);
		BOOST_CHECK(tree->check_tree());
	}

	min = tree->get_bounding_box().get_min();
	max = tree->get_bounding_box().get_max();

	BOOST_CHECK_NO_THROW(visitor.reset(new ContainsNodeVisitor()));

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

		for (j = 0; j < get_count(); ++j)
		{
			BOOST_CHECK_LE(get_box(j).use_count(), 2);
		}
	}
}

BOOST_AUTO_TEST_CASE(add_random_select_and_remove_count_objects)
{
	boost::mt19937 rng;
	boost::uniform_real<float> float_range(0.0f, 1.0f);
	boost::variate_generator<boost::mt19937&, boost::uniform_real<float> >
		random_float(rng, float_range);
	boost::scoped_ptr<el::RStarTree> tree;
	el::BoundingBox bounding_box;
	boost::scoped_ptr<CountBoundedBoxNodeVisitor> visitor;
	el::BoundedObjectSharedPtrVector objects;
	glm::vec3 min, max;
	Uint32 i, j, count;

	BOOST_CHECK_NO_THROW(tree.reset(new el::RStarTree()));

	for (i = 0; i < get_count(); ++i)
	{
		BOOST_CHECK_NO_THROW(tree->add(get_box(i)));
		BOOST_CHECK_EQUAL(get_box(i).use_count(), 2);
		BOOST_CHECK(tree->check_tree());
	}

	BOOST_CHECK_NO_THROW(visitor.reset(new CountBoundedBoxNodeVisitor()));

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

		for (j = 0; j < get_count(); ++j)
		{
			BOOST_CHECK_LE(get_box(j).use_count(), 2);
		}
	}
}

BOOST_AUTO_TEST_SUITE_END()
