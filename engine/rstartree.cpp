/***************************************************************************
 *            rstartree.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "rstartree.hpp"
#include "exceptions.hpp"

namespace eternal_lands
{

	void RStarTree::set_root_node(const RStarTreeNodeSharedPtr &node)
	{
		assert(node.get() != nullptr);

		m_root_node = node;
	}

	const RStarTreeNodeSharedPtr &RStarTree::add_new_root_node(
		const Uint32 level)
	{
		m_root_node = boost::make_shared<RStarTreeNode>(level);

		return m_root_node;
	}

	bool RStarTree::valid_factor(const float factor) const throw ()
	{
		return ((factor >= 0.0f) && (factor <= 1.0f));
	}

	void RStarTree::add_data(const BoundedObjectSharedPtr &element,
		const Uint32 level, BitSet64 &oft)
	{
		RStarTreeNodeSharedPtrStack path_buffer;
		RStarTreeNodeSharedPtr node;

		assert(element.get() != nullptr);

		node = get_root_node()->choose_sub_tree(
			element->get_bounding_box(), level, path_buffer);

		node->insert_element(this, element, path_buffer, oft);

		update_root_node_bounding_box();
	}

	RStarTreeNodeSharedPtr RStarTree::condense_tree(
		RStarTreeNodeSharedPtrVector &reinsert,
		RStarTreeNodeSharedPtrStack &path_buffer,
		const RStarTreeNodeSharedPtr &node)
	{
		RStarTreeNodeSharedPtr parent;
		Uint32 minimum_load;

		assert(node.get() != nullptr);

		minimum_load = node->get_max_count() * get_fill_factor();

		if (path_buffer.empty())
		{
			if (!node->get_leaf() && (node->get_count() == 1))
			{
				return node->get_node(0);
			}
			else
			{
				return RStarTreeNodeSharedPtr();
			}
		}
		else
		{
			parent = path_buffer.top();
			path_buffer.pop();

			if (node->get_count() <= minimum_load)
			{
				parent->delete_sub_node(node);

				reinsert.push_back(node);
			}
			else
			{
				node->calculate_enclosing_bounding_box();
			}

			return condense_tree(reinsert, path_buffer, parent);
		}
	}

	void RStarTree::reinsert_nodes(
		const RStarTreeNodeSharedPtrVector &reinsert)
	{
		BitSet64 oft;
		Uint32 i, count;

		BOOST_FOREACH(const RStarTreeNodeSharedPtr &node, reinsert)
		{
			count = node->get_count();

			for (i = 0; i < count; ++i)
			{
				oft.reset();
				add_data(node->get_element(i),
					node->get_level(), oft);

				assert(count == node->get_count());
			}
		}
	}

	RStarTree::RStarTree()
	{
		m_split_distribution_factor = 0.4f;
		m_reinsert_factor = 0.6f;
		m_fill_factor = 0.7f;

		add_new_root_node(0);
	}

	RStarTree::~RStarTree() noexcept
	{
	}

	void RStarTree::set_split_distribution_factor(const float value)
	{
		if (!valid_factor(value))
		{
			EL_THROW_EXCEPTION(RangeErrorException()
				<< errinfo_float_range_min(0.0f)
				<< errinfo_float_range_max(1.0f)
				<< errinfo_float_range_index(value));
		}
		else
		{
			m_split_distribution_factor = value;
		}
	}

	void RStarTree::set_reinsert_factor(const float value)
	{
		if (!valid_factor(value))
		{
			EL_THROW_EXCEPTION(RangeErrorException()
				<< errinfo_float_range_min(0.0f)
				<< errinfo_float_range_max(1.0f)
				<< errinfo_float_range_index(value));
		}
		else
		{
			m_reinsert_factor = value;
		}
	}

	void RStarTree::set_fill_factor(const float value)
	{
		if (!valid_factor(value))
		{
			EL_THROW_EXCEPTION(RangeErrorException()
				<< errinfo_float_range_min(0.0f)
				<< errinfo_float_range_max(1.0f)
				<< errinfo_float_range_index(value));
		}
		else
		{
			m_fill_factor = value;
		}
	}

	void RStarTree::add(const BoundedObjectSharedPtr &element)
	{
		RStarTreeNodeSharedPtrStack path_buffer;
		RStarTreeNodeSharedPtr node;
		BitSet64 oft;

		NULL_PTR_CHECK(element.get(), UTF8("element"));

		if (!get_root_node()->insert(element))
		{
			oft.reset();

			node = get_root_node()->choose_sub_tree(
				element->get_bounding_box(), 0, path_buffer);

			assert(node.get() != nullptr);

			node->insert_element(this, element, path_buffer, oft);
		}
		else
		{
			get_root_node()->update_enclosing_bounding_box(element);
		}
	}

	void RStarTree::remove(const BoundedObjectSharedPtr &element)
	{
		RStarTreeNodeSharedPtrStack path_buffer;
		RStarTreeNodeSharedPtrVector reinsert;
		RStarTreeNodeSharedPtr node;

		NULL_PTR_CHECK(element.get(), UTF8("element"));

		node = get_root_node()->find_leaf(element, path_buffer);

		if (node.get() == nullptr)
		{
			EL_THROW_EXCEPTION(ItemNotFoundException());
		}

		node->delete_element(element);

		node = condense_tree(reinsert, path_buffer, node);

		if (node.get() != nullptr)
		{
			set_root_node(node);
		}

		reinsert_nodes(reinsert);
	}

	Uint32 RStarTree::optimize(const Uint32 count)
	{
		Uint32 minimum_load, i;

		minimum_load = get_root_node()->get_max_count() * m_fill_factor;

		for (i = 0; i < count; ++i)
		{
			RStarTreeNodeSharedPtrStack path_buffer;
			RStarTreeNodeSharedPtrVector reinsert;
			RStarTreeNodeSharedPtr node;

			node = get_root_node()->find_small_leaf(minimum_load,
				path_buffer);

			if (node.get() == nullptr)
			{
				return i;
			}

			node = condense_tree(reinsert, path_buffer, node);

			if (node.get() != nullptr)
			{
				set_root_node(node);
			}

			/* assert(check_tree()); */

			reinsert_nodes(reinsert);

			/* assert(check_tree()); */
		}

		return count;
	}

	void RStarTree::intersect(const Frustum &frustum,
		AbstractBoundedObjectVisitor &visitor) const
	{
		get_root_node()->intersect_tree(frustum,
			frustum.get_planes_mask(), visitor);
	}

	void RStarTree::clear()
	{
		add_new_root_node(0);
	}

	Uint32 RStarTree::get_max_elements_per_node() noexcept
	{
		return RStarTreeNode::get_max_count();
	}

	const BoundingBox &RStarTree::get_bounding_box() const noexcept
	{
		return get_root_node()->get_bounding_box();
	}

	Uint32 RStarTree::get_nodes_count() const noexcept
	{
		return get_root_node()->get_sub_nodes_count() + 1;
	}

	bool RStarTree::get_empty() const noexcept
	{
		return get_root_node()->get_count() == 0;
	}

	Uint32 RStarTree::get_memory_usage() const noexcept
	{
		return sizeof(RStarTree) + get_nodes_count() *
			sizeof(RStarTreeNode);
	}

	bool RStarTree::check_tree() const
	{
		return get_root_node()->check_nodes(
			get_root_node()->get_level(), true);
	}

	void RStarTree::select_objects(const BoundingBox &bounding_box,
		const bool contains_only,
		BoundedObjectSharedPtrVector &objects) const
	{
		get_root_node()->select_objects(bounding_box, contains_only,
			objects);
	}

	void RStarTree::select_objects(AbstractNodeVisitor &visitor,
		BoundedObjectSharedPtrVector &objects) const
	{
		get_root_node()->select_objects(visitor, objects);
	}

	void RStarTree::visit(AbstractNodeVisitor &visitor) const
	{
		get_root_node()->visit(visitor);
	}

	void RStarTree::select_and_remove_objects(AbstractNodeVisitor &visitor,
		BoundedObjectSharedPtrVector &objects)
	{
		RStarTreeNodeSharedPtrStack path_buffer;
		RStarTreeNodeSharedPtrVector reinsert;
		RStarTreeNodeSharedPtr node, parent;

		node = get_root_node()->select_objects(visitor, objects,
			path_buffer);

		if (node.get() == nullptr)
		{
			return;
		}

		if (path_buffer.size() == 0)
		{
			add_new_root_node(0);

			return;
		}

		parent = path_buffer.top();
		path_buffer.pop();

		parent->delete_sub_node(node);

		node = condense_tree(reinsert, path_buffer, parent);

		if (node.get() != nullptr)
		{
			set_root_node(node);
		}

		reinsert_nodes(reinsert);
	}

	void RStarTree::update_root_node_bounding_box()
	{
		if (get_root_node()->get_count() > 0)
		{
			get_root_node()->calculate_enclosing_bounding_box();
		}
		else
		{
			get_root_node()->set_bounding_box(BoundingBox(
				glm::vec3(0.0f), glm::vec3(0.0f)));
		}
	}

	void RStarTree::log(OutStream &log) const
	{
		BoostFormat str(UTF8("split distribution %1%, reinsert %2%, "
			"fill %3%"));

		str % get_split_distribution_factor() % get_reinsert_factor();
		str % get_reinsert_factor();

		log << str.str() << std::endl;

		get_root_node()->log(0, log);
	}

	void RStarTree::randomize_order()
	{
		srand(time(0));

		get_root_node()->randomize_order();
	}

}
