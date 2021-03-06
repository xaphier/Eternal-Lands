/****************************************************************************
 *            rstartreenode.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "rstartreenode.hpp"
#include "rstartree.hpp"
#include "abstractnodevisitor.hpp"
#include "abstractboundedobjectvisitor.hpp"
#include "exceptions.hpp"

namespace eternal_lands
{

	namespace
	{

		typedef boost::array<BoundedObjectSharedPtr,
			BoundedObjectSharedPtrElements::static_size + 1>
			BoundedObjectSharedPtrSplitElements;

		class BoundedObjectSplitCmp
		{
			private:
				const Uint16 m_index;
				const bool m_max_split;

			public:
				BoundedObjectSplitCmp(const Uint16 index,
					const bool max_split);

				bool operator()(
					const BoundedObjectSharedPtr &split_1,
					const BoundedObjectSharedPtr &split_2)
					const noexcept;

		};

		BoundedObjectSplitCmp::BoundedObjectSplitCmp(const Uint16 index,
			const bool max_split): m_index(index),
			m_max_split(max_split)
		{
		}

		bool BoundedObjectSplitCmp::operator()(
			const BoundedObjectSharedPtr &split_1,
			const BoundedObjectSharedPtr &split_2) const noexcept
		{
			if (m_max_split)
			{
				return split_1->get_bounding_box().get_max(
					)[m_index] >
					split_2->get_bounding_box().get_max(
					)[m_index];
			}
			else
			{
				return split_1->get_bounding_box().get_min(
					)[m_index] >
					split_2->get_bounding_box().get_min(
					)[m_index];
			}
		}

		float split_margin(
			BoundedObjectSharedPtrSplitElements &split_data,
			const Sint32 min_index, const Sint32 max_index,
			const Uint16 index, const bool max_split)
		{
			BoundingBox bounding_box;
			Sint32 i, size;
			float margin_value;

			std::sort(split_data.begin(), split_data.end(),
				BoundedObjectSplitCmp(index, max_split));

			size = split_data.size();

			margin_value = 0.0f;

			bounding_box = split_data[0]->get_bounding_box();

			for (i = 1; i < min_index; ++i)
			{
				bounding_box.merge(
					split_data[i]->get_bounding_box());
			}

			for (i = min_index; i < max_index; ++i)
			{
				margin_value += bounding_box.get_margin();

				bounding_box.merge(
					split_data[i]->get_bounding_box());
			}

			bounding_box = split_data[size - 1]->get_bounding_box();

			for (i = max_index; i < (size - 1); ++i)
			{
				bounding_box.merge(
					split_data[i]->get_bounding_box());
			}

			for (i = max_index - 1; i > (min_index - 1); i--)
			{
				bounding_box.merge(
					split_data[i]->get_bounding_box());

				margin_value += bounding_box.get_margin();
			}

			return margin_value;
		}

		Sint32 split_array(
			BoundedObjectSharedPtrSplitElements &split_data,
			const Uint32 min_size, const Uint32 split_axe)
		{
			FloatArray2 margin;
			BoundingBox bounding_box_min, bounding_box_max;
			BoundingBox bounding_box;
			float overlap_tmp, min_overlap, dist, min_dist;
			Sint32 size, split_point, min_index, max_index;
			Sint32 i, j;
			bool max_split;

			size = split_data.size();

			min_index = min_size;
			max_index = size - min_index;

			if ((max_index < min_index) || (min_index < 1) ||
				(max_index > (size - 1)))
			{
				return -1;
			}

			if (max_index == min_index)
			{
				return min_index;
			}

			min_dist = std::numeric_limits<float>::max();
			min_overlap = std::numeric_limits<float>::max();
			split_point = -1;

			margin[0] = split_margin(split_data, min_index,
				max_index, split_axe, false);
			margin[1] = split_margin(split_data, min_index,
				max_index, split_axe, true);

			max_split = margin[1] < margin[0];

			std::sort(split_data.begin(), split_data.end(),
				BoundedObjectSplitCmp(split_axe, max_split));

			bounding_box_min = split_data[0]->get_bounding_box();

			for (i = 1; i < min_index; ++i)
			{
				bounding_box_min.merge(
					split_data[i]->get_bounding_box());
			}

			bounding_box_max =
				split_data[size - 1]->get_bounding_box();

			for (i = max_index; i < (size - 1); ++i)
			{
				bounding_box_max.merge(
					split_data[i]->get_bounding_box());
			}

			for (i = min_index; i < max_index; ++i)
			{
				bounding_box = bounding_box_max;

				for (j = i; j < max_index; j++)
				{
					bounding_box.merge(
						split_data[
							j]->get_bounding_box());
				}

				overlap_tmp = bounding_box.overlap(
					bounding_box_min);

				dist = std::abs((size - 1 ) * 0.5f - i);

				if ((overlap_tmp < min_overlap) ||
					((overlap_tmp == min_overlap) &&
					(dist < min_dist)))
				{
					min_overlap = overlap_tmp;
					split_point = i;
					min_dist = dist;
				}

				bounding_box_min.merge(
					split_data[i]->get_bounding_box());
			}

			return split_point;
		}

	}

	const BoundingBox &RStarTreeNode::get_element_bounding_box(
		const Uint32 index) const
	{
		assert(check_index(index));
		assert(m_elements[index].get() != nullptr);

		return m_elements[index]->get_bounding_box();
	}


	bool RStarTreeNode::add_element(const BoundedObjectSharedPtr &element)
		noexcept
	{
		Uint32 index;

		assert(element.get() != nullptr);

		if (get_count() < get_max_count())
		{
			index = get_count();

			m_count++;

			assert(get_count() <= get_max_count());

			set_element(element, index);

			return true;
		}
		else
		{
			return false;
		}
	}

	void RStarTreeNode::view_node(AbstractBoundedObjectVisitor &visitor)
		const
	{
		Uint32 i;

		if (get_leaf())
		{
			for (i = 0; i < get_count(); ++i)
			{
				visitor(get_element(i));
			}

			return;
		}

		for (i = 0; i < get_count(); ++i)
		{
			get_node(i)->view_node(visitor);
		}
	}

	void RStarTreeNode::add_node(BoundedObjectSharedPtrVector &objects)
		const
	{
		Uint32 i;

		if (get_leaf())
		{
			for (i = 0; i < get_count(); ++i)
			{
				objects.push_back(get_element(i));
			}

			return;
		}

		for (i = 0; i < get_count(); ++i)
		{
			get_node(i)->add_node(objects);
		}
	}

	void RStarTreeNode::remove_element(const Uint32 index) noexcept
	{
		Uint32 end;

		assert(check_index(index));

		end = get_count() - 1;

		if ((index != (get_count() - 1)) && (get_count() > 1))
		{
			m_elements[index] = get_element(end);
		}

		m_elements[end].reset();

		m_count--;
	}

	void RStarTreeNode::intersect_node(const Frustum &frustum,
		const BitSet64 in_mask, AbstractBoundedObjectVisitor &visitor)
		const
	{
		Uint32 i;

		assert(get_leaf());

		for (i = 0; i < get_count(); ++i)
		{
			if (frustum.intersect(get_element_bounding_box(i),
				in_mask) != it_outside)
			{
				visitor(get_element(i));
			}
		}
	}

	void RStarTreeNode::intersect_tree(const Frustum &frustum,
		const BitSet64 mask, AbstractBoundedObjectVisitor &visitor)
		const
	{
		Uint32 i;
		BitSet64 out_mask;
		IntersectionType result;

		if (get_leaf())
		{
			intersect_node(frustum, mask, visitor);
			return;
		}

		for (i = 0; i < get_count(); ++i)
		{
			result = frustum.intersect(get_element_bounding_box(i),
				mask, out_mask);

			switch (result)
			{
				case it_inside:
				{
					get_node(i)->view_node(visitor);
					break;
				}
				case it_intersect:
				{
					get_node(i)->intersect_tree(frustum,
						out_mask, visitor);
					break;
				}
				case it_outside:
				{
					break;
				}
			}
		}
	}

	Uint32 RStarTreeNode::get_item_count() const
	{
		Uint32 i, count;

		if (get_leaf())
		{
			return get_count();
		}

		count = 0;

		for (i = 0; i < get_count(); ++i)
		{
			count += get_node(i)->get_item_count();
		}

		return count;
	}

	RStarTreeNodeSharedPtr RStarTreeNode::find_leaf(
		const BoundedObjectSharedPtr &element,
		RStarTreeNodeSharedPtrStack &path_buffer)
	{
		RStarTreeNodeSharedPtr found;
		Uint32 i;

		assert(element.get() != nullptr);

		if (get_leaf())
		{
			if (find_element(element) != -1)
			{
				return get_shared_from_this();
			}
			else
			{
				return RStarTreeNodeSharedPtr();
			}
		}

		path_buffer.push(get_shared_from_this());

		for (i = 0; i < get_count(); ++i)
		{
			if (get_element_bounding_box(i).contains(
				element->get_bounding_box()))
			{
				found = get_node(i)->find_leaf(element,
					path_buffer);

				if (found.get() != nullptr)
				{
					return found;
				}
			}
		}

		path_buffer.pop();

		return RStarTreeNodeSharedPtr();
	}

	RStarTreeNodeSharedPtr RStarTreeNode::choose_sub_tree(
		const BoundingBox &bounding_box, const Uint32 insertion_level,
		RStarTreeNodeSharedPtrStack &path_buffer)
	{
		RStarTreeNodeSharedPtr node;

		if (get_leaf())
		{
			return get_shared_from_this();
		}

		if (insertion_level == get_level())
		{
			return get_shared_from_this();
		}

		path_buffer.push(get_shared_from_this());

		if (get_level() == 1)
		{
			node = find_least_overlap(bounding_box);
		}
		else
		{
			node = find_least_enlargement(bounding_box);
		}

		assert(node.get() != nullptr);

		return node->choose_sub_tree(bounding_box, insertion_level,
			path_buffer);
	}

	bool RStarTreeNode::insert(const BoundedObjectSharedPtr &element)
	{
		Uint32 i;

		if (get_leaf())
		{
			return add_element(element);
		}

		for (i = 0; i < get_count(); ++i)
		{
			if (get_element_bounding_box(i).contains(
				element->get_bounding_box()))
			{
				if (get_node(i)->insert(element))
				{
					return true;
				}
			}
		}

		return false;
	}

	RStarTreeNodeSharedPtr RStarTreeNode::find_least_overlap(
		const BoundingBox &bounding_box) const
	{
		BoundingBox tmp_bounding_box;
		float overlap, min_overlap;
		Uint32 i, j, index;

		assert(!get_leaf());
		assert(get_count() > 0);

		min_overlap = std::numeric_limits<float>::max();

		index = std::numeric_limits<Uint32>::max();

		for (i = 0; i < get_count(); ++i)
		{
			tmp_bounding_box = enclose(bounding_box,
				get_element_bounding_box(i));

			overlap = 0.0f;

			for (j = 0; j < get_count(); j++)
			{
				if (i != j)
				{
					overlap += tmp_bounding_box.overlap(
						get_element_bounding_box(j));
				}
			}

			if (overlap < min_overlap)
			{
				min_overlap = overlap;
				index = i;
			}
		}

		assert(index < get_count());

		return get_node(index);
	}

	RStarTreeNodeSharedPtr RStarTreeNode::find_least_enlargement(
		const BoundingBox &bounding_box) const
	{
		float enlargement, min_enlargement;
		Uint32 i, index;

		assert(!get_leaf());
		assert(get_count() > 0);

		min_enlargement = std::numeric_limits<float>::max();

		index = std::numeric_limits<Uint32>::max();

		for (i = 0; i < get_count(); ++i)
		{
			enlargement = get_element_bounding_box(i).enlargement(
				bounding_box);

			if (enlargement < min_enlargement)
			{
				min_enlargement = enlargement;
				index = i;
			}
		}

		return get_node(index);
	}

	void RStarTreeNode::calculate_enclosing_bounding_box()
	{
		BoundingBox bounding_box;
		glm::vec3 min, max;
		Uint32 i;

		assert(get_count() > 0);

		min = get_element_bounding_box(0).get_min();
		max = get_element_bounding_box(0).get_max();

		for (i = 1; i < get_count(); ++i)
		{
			min = glm::min(min,
				get_element_bounding_box(i).get_min());
			max = glm::max(max,
				get_element_bounding_box(i).get_max());
		}

		bounding_box.set_min_max(min, max);

		bounding_box.scale(1.01f);

		set_bounding_box(bounding_box);
	}

	void RStarTreeNode::update_enclosing_bounding_box(
		const BoundedObjectSharedPtr &element)
	{
		BoundingBox bounding_box;

		if (get_count() > 1)
		{
			bounding_box = get_bounding_box();

			bounding_box.merge(element->get_bounding_box());

			bounding_box.scale(1.01f);

			set_bounding_box(bounding_box);
		}
		else
		{
			bounding_box = element->get_bounding_box();

			bounding_box.scale(1.01f);

			set_bounding_box(bounding_box);
		}
	}

	void RStarTreeNode::split(RStarTreePtr tree,
		const BoundedObjectSharedPtr &element,
		const RStarTreeNodeSharedPtr &new_node)
	{
		BoundedObjectSharedPtrSplitElements split_data;
		Sint32 i, node_spf, new_size, split_point;
		Uint32 j, split_axe;

		assert(get_count() == get_max_count());
		assert(new_node.get() != nullptr);
		assert(element.get() != nullptr);
		assert(tree);

		new_size = get_max_count() + 1;

		node_spf = boost::numeric_cast<Sint32>(new_size *
			tree->m_split_distribution_factor * 0.5f);

		for (j = 0; j < get_max_count(); j++)
		{
			split_data[j] = get_element(j);
		}

		split_data[get_max_count()] = element;

		split_axe = get_main_axe(element);

		split_point = split_array(split_data, node_spf, split_axe);

		assert(split_point > 0);
		assert(split_point < static_cast<Sint32>(get_max_count()));

		clear();

		for (i = 0; i < split_point; ++i)
		{
			new_node->add_element(split_data[i]);
		}

		assert(new_node->get_count() > 0);

		for (i = split_point; i < new_size; ++i)
		{
			add_element(split_data[i]);
		}

		assert(get_count() > 0);

		calculate_enclosing_bounding_box();

		new_node->calculate_enclosing_bounding_box();
	}

	bool RStarTreeNode::insert_element(RStarTreePtr tree,
		const BoundedObjectSharedPtr &element,
		RStarTreeNodeSharedPtrStack &path_buffer, BitSet64 &oft)
	{
		RStarTreeNodeSharedPtr node;
		RStarTreeNodeSharedPtr new_node;
		BoundedObjectSharedPtrVector reinsert;
		Uint32 i;

		assert(tree);
		assert(element.get() != nullptr);

		if (add_element(element))
		{
			if (!path_buffer.empty())
			{
				calculate_enclosing_bounding_box();
				adjust_tree_add(path_buffer);

				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			assert(get_level() < oft.size());

			if (!path_buffer.empty() && !oft[get_level()])
			{
				oft[get_level()] = true;

				reinsert_elements(tree, element, reinsert);

				adjust_tree(path_buffer);

				for (i = 0; i < reinsert.size(); ++i)
				{
					tree->add_data(reinsert[i],
						get_level(), oft);
				}

				return true;
			}
			else
			{
				new_node = boost::make_shared<RStarTreeNode>(
					get_level());

				split(tree, element, new_node);

				assert(new_node.get() != nullptr);
				assert(new_node->get_count() > 0);
				assert(get_count() > 0);

				if (path_buffer.empty())
				{
					node = tree->add_new_root_node(
						get_level() + 1);

					assert(node.get() != nullptr);

					node->add_element(
						get_shared_from_this());
					node->add_element(new_node);

					node->calculate_enclosing_bounding_box();
				}
				else
				{
					node = path_buffer.top();

					path_buffer.pop();

					assert(node.get() != nullptr);

					node->adjust_tree(tree, new_node,
						path_buffer, oft);

					assert(node->get_count() > 0);
				}

				return true;
			}
		}
	}

	RStarTreeNodeSharedPtr RStarTreeNode::find_small_leaf(
		const Uint32 minimum_load,
		RStarTreeNodeSharedPtrStack &path_buffer)
	{
		RStarTreeNodeSharedPtr found;
		Uint32 i;

		if (get_leaf())
		{
			if (get_count() < minimum_load)
			{
				return get_shared_from_this();
			}
			else
			{
				return RStarTreeNodeSharedPtr();
			}
		}

		path_buffer.push(get_shared_from_this());

		for (i = 0; i < get_count(); ++i)
		{
			found = get_node(i)->find_small_leaf(minimum_load,
				path_buffer);

			if (found)
			{
				return found;
			}
		}

		path_buffer.pop();

		return RStarTreeNodeSharedPtr();
	}

	void RStarTreeNode::adjust_tree(
		RStarTreeNodeSharedPtrStack &path_buffer)
	{
		RStarTreeNodeSharedPtr parent;

		calculate_enclosing_bounding_box();

		if (!path_buffer.empty())
		{
			parent = path_buffer.top();

			path_buffer.pop();

			assert(parent);

			parent->adjust_tree(path_buffer);
		}
	}

	void RStarTreeNode::adjust_tree_add(
		RStarTreeNodeSharedPtrStack &path_buffer)
	{
		RStarTreeNodeSharedPtr parent;

		assert(get_count() > 0);

		if (!path_buffer.empty())
		{
			parent = path_buffer.top();

			path_buffer.pop();

			assert(parent);

			parent->update_enclosing_bounding_box(
				get_shared_from_this());
			parent->adjust_tree_add(path_buffer);
		}
	}

	void RStarTreeNode::adjust_tree(RStarTreePtr tree,
		const RStarTreeNodeSharedPtr &node,
		RStarTreeNodeSharedPtrStack &path_buffer, BitSet64 &oft)
	{
		bool adjust;

		adjust = insert_element(tree, node, path_buffer, oft);

		if (!adjust)
		{
			calculate_enclosing_bounding_box();
			adjust_tree_add(path_buffer);
		}
	}

	void RStarTreeNode::reinsert_elements(RStarTreePtr tree,
		const BoundedObjectSharedPtr &element,
		BoundedObjectSharedPtrVector &reinsert)
	{
		Uint32 i, reinsert_count;

		assert(get_count() == get_max_count());

		reinsert_count = static_cast<Sint32>(get_max_count() *
			tree->get_reinsert_factor());

		assert(reinsert_count < get_count());

		reinsert.reserve(reinsert_count + 1);

		for (i = 0; i < reinsert_count; ++i)
		{
			reinsert.push_back(get_element(i));
		}

		for (i = 0; i < reinsert_count; ++i)
		{
			assert(get_index(reinsert[i]) == i);
			remove_element(i);
		}

		reinsert.push_back(element);

		assert(get_count() > 0);
	}

	RStarTreeNode::RStarTreeNode(const Uint32 level): m_level(level),
		m_count(0)
	{
	}

	RStarTreeNode::~RStarTreeNode() noexcept
	{
	}

	Uint32 RStarTreeNode::get_sub_nodes_count() const noexcept
	{
		Uint32 i, result;

		if (get_leaf())
		{
			return 0;
		}

		result = get_count();

		for (i = 0; i < get_count(); ++i)
		{
			result += get_node(i)->get_sub_nodes_count();
		}

		return result;
	}

	Uint32 RStarTreeNode::get_index(const BoundedObjectSharedPtr &element)
		const
	{
		Sint32 result;

		result = find_element(element);

		if (result >= 0)
		{
			return result;
		}

		EL_THROW_EXCEPTION(ItemNotFoundException());
	}

	bool RStarTreeNode::check_nodes(const Uint16 level,
		const bool root_node) const
	{
		Uint32 i;

		if (get_level() != level)
		{
			return false;
		}

		for (i = 0; i < get_count(); ++i)
		{
			if (!get_bounding_box().contains(
				get_element_bounding_box(i)))
			{
				return false;
			}

			if (!get_leaf())
			{
				if (!get_node(i)->check_nodes(get_level() - 1,
					false))
				{
					return false;
				}
			}
		}

		return (get_count() > 0) || root_node;
	}

	Uint32 RStarTreeNode::get_main_axe(
		const BoundedObjectSharedPtr &element) const
	{
		BoundingBox bounding_box;
		glm::vec3 half_size;

		bounding_box = get_bounding_box();
		bounding_box.merge(element->get_bounding_box());
		half_size = bounding_box.get_half_size();

		if (half_size[0] > half_size[1])
		{
			if (half_size[0] > half_size[2])
			{
				return 0;
			}
			else
			{
				return 2;
			}
		}
		else
		{
			if (half_size[1] > half_size[2])
			{
				return 1;
			}
			else
			{
				return 2;
			}
		}
	}

	RStarTreeNodeSharedPtr RStarTreeNode::select_objects(
		AbstractNodeVisitor &visitor,
		BoundedObjectSharedPtrVector &objects,
		RStarTreeNodeSharedPtrStack &path_buffer)
	{
		RStarTreeNodeSharedPtr node;
		Uint32 i;

		if (visitor(get_shared_from_this()))
		{
			add_node(objects);

			return get_shared_from_this();
		}

		if (get_leaf())
		{
			return RStarTreeNodeSharedPtr();
		}

		path_buffer.push(get_shared_from_this());

		for (i = 0; i < get_count(); ++i)
		{
			node = get_node(i)->select_objects(visitor, objects,
				path_buffer);

			if (node.get() != nullptr)
			{
				return node;
			}
		}

		path_buffer.pop();

		return RStarTreeNodeSharedPtr();
	}

	bool RStarTreeNode::visit(AbstractNodeVisitor &visitor)
	{
		RStarTreeNodeSharedPtr node;
		Uint32 i;

		if (!visitor(shared_from_this()))
		{
			return false;
		}

		if (get_leaf())
		{
			for (i = 0; i < get_count(); ++i)
			{
				if (!visitor(get_element(i)))
				{
					return false;
				}
			}

			return true;
		}

		for (i = 0; i < get_count(); ++i)
		{
			if (!get_node(i)->visit(visitor))
			{
				return false;
			}
		}

		return true;
	}

	void RStarTreeNode::select_objects(AbstractNodeVisitor &visitor,
		BoundedObjectSharedPtrVector &objects)
	{
		RStarTreeNodeSharedPtr node;
		Uint32 i;

		if (visitor(shared_from_this()))
		{
			add_node(objects);

			return;
		}

		if (get_leaf())
		{
			for (i = 0; i < get_count(); ++i)
			{
				if (visitor(get_element(i)))
				{
					objects.push_back(get_element(i));
				}
			}

			return;
		}

		for (i = 0; i < get_count(); ++i)
		{
			get_node(i)->select_objects(visitor, objects);
		}
	}

	void RStarTreeNode::select_objects(const BoundingBox &bounding_box,
		const bool contains_only,
		BoundedObjectSharedPtrVector &objects)
	{
		RStarTreeNodeSharedPtr node;
		Uint32 i;

		if (bounding_box.contains(get_bounding_box()))
		{
			add_node(objects);

			return;
		}

		if (get_leaf())
		{
			for (i = 0; i < get_count(); ++i)
			{
				if (contains_only)
				{
					if (bounding_box.contains(get_element(
						i)->get_bounding_box()))
					{
						objects.push_back(
							get_element(i));
					}
				}
				else
				{
					if (bounding_box.intersect(get_element(
						i)->get_bounding_box()))
					{
						objects.push_back(
							get_element(i));
					}
				}
			}

			return;
		}

		for (i = 0; i < get_count(); ++i)
		{
			get_node(i)->select_objects(bounding_box,
				contains_only, objects);
		}
	}

	void RStarTreeNode::log(const Uint16 indent, OutStream &log) const
	{
		BoostFormat str(UTF8("node %1%, level %2%, count %3%, box "
			"%4%"));
		Uint32 i, j;

		str % this % get_level() % get_count() % get_bounding_box();

		for (i = 0; i < indent; ++i)
		{
			log << UTF8("\t");
		}

		log << str.str() << std::endl;

		if (get_leaf())
		{
			for (i = 0; i < get_count(); ++i)
			{
				for (j = 0; j <= indent; ++j)
				{
					log << UTF8("\t");
				}

				log << get_element(i)->get_bounding_box();
				log << std::endl;
			}

			return;
		}

		for (i = 0; i < get_count(); ++i)
		{
			get_node(i)->log(indent + 1, log);
		}
	}

	void RStarTreeNode::randomize_order()
	{
		Uint32 i, idx0, idx1;

		for (i = 0; i < get_count(); ++i)
		{
			idx0 = rand() % m_count;
			idx1 = rand() % m_count;

			std::swap(m_elements[idx0], m_elements[idx1]);
		}

		if (get_leaf())
		{
			return;
		}

		for (i = 0; i < get_count(); ++i)
		{
			get_node(i)->randomize_order();
		}
	}

}
