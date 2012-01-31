/****************************************************************************
 *            rstartreenode.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_49455312_8ca1_427c_9d0a_a97d432ebde3
#define	UUID_49455312_8ca1_427c_9d0a_a97d432ebde3

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "frustum.hpp"
#include "boundedobject.hpp"

/**
 * @file
 * @brief The @c class RStarTreeNode.
 * This file contains the @c class RStarTreeNode.
 */
namespace eternal_lands
{

	typedef boost::array<BoundedObjectSharedPtr, 8>
		BoundedObjectSharedPtrElements;

	/**
	 * @brief Node of RStarTree for 3d objects.
	 *
	 */
	class RStarTreeNode: public BoundedObject,
		public boost::enable_shared_from_this<RStarTreeNode>
	{
		friend class RStarTree;
		private:
			/**
			 * @brief Bounding volumes of the elements.
			 *
			 * Bounding volumes of the elements. This can be
			 * subnotes or 3d objects.
			 */
			BoundedObjectSharedPtrElements m_elements;

			/**
			 * @brief Level for this node.
			 *
			 * Level of this node. Is zero for the leaf nodes, one
			 * for the parents etc.
			 */
			const Uint32 m_level;

			/**
			 * @brief The number of elements.
			 *
			 * The number of the elements. This can be subnotes or
			 * 3d objects.
			 */
			Uint32 m_count;

			inline RStarTreeNodeSharedPtr get_shared_from_this()
			{
				return shared_from_this();
			}

		protected:
			/**
			 * @brief Get element.
			 *
			 * Returns the pointer of the element at the given
			 * position.
			 * @param index The index of the element.
			 * @return The pointer of the element.
			 */
			inline const BoundedObjectSharedPtr &get_element(
				const Uint32 index) const
			{
				assert(check_index(index));
				assert(m_elements[index].get() != 0);

				return m_elements[index];
			}

			/**
			 * @brief Set element.
			 *
			 * Sets the pointer of the element at the given
			 * position.
			 * @param value The pointer of the element.
			 * @param index The index of the element.
			 */
			inline void set_element(
				const BoundedObjectSharedPtr &element,
				const Uint32 index)
			{
				assert(check_index(index));
				assert(element.get() != 0);
				assert(m_elements[index].get() == 0);

				m_elements[index] = element;
			}

			/**
			 * @brief Get node.
			 *
			 * Returns the pointer of the node at the given
			 * position.
			 * @param index The index of the node.
			 * @return The pointer of the node.
			 */
			inline RStarTreeNodeSharedPtr get_node(const Uint32 index)
				const
			{
				assert(!get_leaf());

				return boost::dynamic_pointer_cast<RStarTreeNode>(
					get_element(index));
			}

			/**
			 * @brief Checks the index.
			 *
			 * Checks if the index index is less than get_count().
			 * @param index The index to be checked.
			 * @return True if the index is valid, else false.
			 */
			inline bool check_index(const Uint32 index) const
				throw ()
			{
				return index < get_count();
			}

			/**
			 * @brief Returns the bounding volume at the specified
			 * position.
			 *
			 * Returns the bounding volume at the specified
			 * position.
			 * @param index Index of the bounding volume.
			 * @return The bounding volume at the given position.
			 */
			const BoundingBox &get_element_bounding_box(
				const Uint32 index) const;

			/**
			 * @brief Adds an element.
			 *
			 * Adds an element with bounding volume and its pointer
			 * to the node.
			 * @param element The pointer of the element.
			 * @return True if successful, else false.
			 */
			bool add_element(const BoundedObjectSharedPtr &element);

			/**
			 * @brief Adds all elements of the node and its
			 * children to the intersection list.
			 *
			 * Adds all elements of the node and its children to
			 * the intersection list. The pointers of the
			 * intersecting elements (only elements in leafs) are
			 * saved in the vector intersects.
			 * @param sub_frustums_mask The mask of the sub
			 * frustums.
			 * @param visitor The visitor that gets the pointers of
			 * the intersecting elements.
			 */
			void add_node(const SubFrustumsMask sub_frustums_mask,
				AbstractBoundedObjectVisitor &visitor) const;

			/**
			 * @brief Removes an element.
			 *
			 * Removes the element at the given position from the
			 * node.
			 * @param index Index where to remove the element.
			 */
			void remove_element(const Uint32 index);

			/**
			 * @brief Tests all elements of the node for
			 * intersection.
			 *
			 * Test the frustum for intersection wiht all elements
			 * in the node. The
			 * pointers of the intersecting elements are saved in
			 * the vector intersects.
			 * @param frustum The frustum used for the intersection
			 * test.
			 * @param visitor The visitor that gets the pointers of
			 * the intersecting elements.
			 */
			void intersect_node(const Frustum &frustum,
				const PlanesMask in_mask,
				AbstractBoundedObjectVisitor &visitor) const;

			/**
			 * @brief Tests all elements of the node and its
			 * children for intersection.
			 *
			 * Test the frustum for intersection wiht all elements
			 * in the node and all children of the node. The
			 * pointers of the intersecting elements (only
			 * elements in leafs) are saved in the vector
			 * intersects.
			 * @param frustum The frustum used for the intersection
			 * test.
			 * @param visitor The visitor that gets the pointers of
			 * the intersecting elements.
			 */
			void intersect_tree(const Frustum &frustum,
				const PlanesMask in_mask,
				AbstractBoundedObjectVisitor &visitor) const;

			/**
			 * @brief Number of items.
			 *
			 * If it's a leave, it returns the number of elements,
			 * else the sum of all items the sub notes hold.
			 * @return Returns the number of items this node or all
			 * it's sub notes have.
			 */
			Uint32 get_item_count() const;

			/**
			 * @brief Searches the leaf that holds the element.
			 *
			 * Searches the leaf that holds the element with the
			 * given bounding volume
			 * and the given pointer.
			 * @param element The pointer of the element.
			 * @param path_buffer The path to the element.
			 * @return The pointer of the leaf node if found, else 0.
			 */
			RStarTreeNodeSharedPtr find_leaf(
				const BoundedObjectSharedPtr &element,
				RStarTreeNodeSharedPtrStack &path_buffer);

			/**
			 * @brief Searches the node where we would insert the
			 * element.
			 *
			 * Searches the node where we would insert the element.
			 * @param bounding_box The bounding volume of the
			 * element.
			 * @param insertion_level The level of the node where
			 * we should insert.
			 * @param path_buffer The path to the node.
			 * @return The pointer of the node where we should
			 * insert.
			 */
			RStarTreeNodeSharedPtr choose_sub_tree(
				const BoundingBox &bounding_box,
				const Uint32 insertion_level,
				RStarTreeNodeSharedPtrStack &path_buffer);

			/**
			 * @brief Searches the node where we would insert the
			 * element.
			 *
			 * Searches the node where we would insert the element.
			 * @param bounding_volume The bounding volume of the
			 * element.
			 * @return The pointer of the node where we should
			 * insert.
			 */
			bool insert(const BoundedObjectSharedPtr &element);

			/**
			 * @brief Searches the node where the insertion would
			 * cause the least overlap.
			 *
			 * Searches the node where the insertion would cause
			 * the least enlargement of the bounding box of the
			 * node. If we can't find a child-node where the
			 * enlargement is zero, we search the child-node where
			 * the overlap of the bounding boxes of the child-nodes
			 * is the smallest after the insert of the element.
			 * @param bounding_box The bounding box of the element.
			 * @return The index of the child-node where we should
			 * insert.
			 */
			RStarTreeNodeSharedPtr find_least_overlap(
				const BoundingBox &bounding_box) const;

			/**
			 * @brief Searches the node where the insertion would
			 * cause the least enlargement.
			 *
			 * Searches the child-node where the insertion would
			 * cause the least enlargement of the bounding box of
			 * the node.
			 * @param bounding_box The bounding box of the element.
			 * @return The index of the child-node where we should
			 * insert.
			 */
			RStarTreeNodeSharedPtr find_least_enlargement(
				const BoundingBox &bounding_box) const;

			/**
			 * @brief Node bounding box calculation.
			 *
			 * Calculates the bounding box of the node.
			 */
			void calculate_enclosing_bounding_box();

			/**
			 * @brief Node bounding box calculation.
			 *
			 * Updates the bounding box of the node,
			 * merging with the bounding box of the last element.
			 */
			void update_enclosing_bounding_box(
				const BoundedObjectSharedPtr &element);

			/**
			 * @brief Splits the node.
			 *
			 * Calculates the split point and moves one part of the
			 * data to the new node, the rest remains in this node.
			 * @param tree The r*-tree to use.
			 * @param element The pointer of the element that
			 * triggered the split.
			 * @param new_node The new node where some data moves
			 * in.
			 */
			void split(RStarTreePtr tree,
				const BoundedObjectSharedPtr &element,
				const RStarTreeNodeSharedPtr &new_node);

			/**
			 * @brief Adjusts the tree.
			 *
			 * Updates the bounding volumes of the nodes after a
			 * remove of an element.
			 * @param path_buffer The path to the node.
			 */
			void adjust_tree(
				RStarTreeNodeSharedPtrStack &path_buffer);

			/**
			 * @brief Adjusts the tree.
			 *
			 * Updates the bounding volumes of the nodes after a
			 * remove of an element.
			 * @param path_buffer The path to the node.
			 */
			void adjust_tree_add(
				RStarTreeNodeSharedPtrStack &path_buffer);

			/**
			 * @brief Adjusts the tree.
			 *
			 * Updates the bounding volumes of the nodes after a
			 * split of a node.
			 * @param tree The r*-tree to use.
			 * @param node The new node of the split.
			 * @param path_buffer The path to the old node.
			 * @param oft Overflow table to prevent too much
			 * reinsert.
			 */
			void adjust_tree(RStarTreePtr tree,
				const RStarTreeNodeSharedPtr &node,
				RStarTreeNodeSharedPtrStack &path_buffer,
				BitSet32 &oft);

			/**
			 * @brief Selects elements for reinsert.
			 *
			 * Selects elements for reinsert and removes the
			 * elements from the node. The number of reinsert
			 * elements depends on the reinsert factor.
			 * @see RStarTree::get_reinsert_factor()
			 * @param tree The r*-tree to use.
			 * @param element The pointer of the element that
			 * triggered the reinsert.
			 * @param reinsert The vector that gets filled with the
			 * removed nodes.
			 */
			void reinsert_elements(RStarTreePtr tree,
				const BoundedObjectSharedPtr &element,
				BoundedObjectSharedPtrVector &reinsert);

			/**
			 * @brief The index of an element.
			 *
			 * Searches the index of the element.
			 * @param element The pointer of the element.
			 * @return The index of the element or -1 if not found.
			 */
			inline Sint32 find_element(
				const BoundedObjectSharedPtr &element) const
			{
				Uint32 i;

				assert(element);

				for (i = 0; i < get_count(); ++i)
				{
					if (get_element(i) == element)
					{
						return i;
					}
				}

				return -1;
			}

			/**
			 * @brief The index of the element.
			 *
			 * Searches the index of the element.
			 * @throw ExtendedException if the element is not in
			 * the node.
			 * @param element The pointer of the element.
			 * @return The index of the element.
			 */
			Uint32 get_index(const BoundedObjectSharedPtr &element)
				const;

			/**
			 * @brief Enclose calculation.
			 *
			 * Calculates the tightest enclosing bounding volume of
			 * two bounding volumes.
			 * @param bounding_box_1 First bounding volume.
			 * @param bounding_box_2 Second bounding volume.
			 * @return The tightest bounding volume enclosing the
			 * two bounding volumes.
			 */
			static inline BoundingBox enclose(
				const BoundingBox &bounding_box_1,
				const BoundingBox &bounding_box_2)
			{
				BoundingBox result;

				result = bounding_box_1;

				result.merge(bounding_box_2);

				return result;
			}

			/**
			 * @brief Inserts an element.
			 *
			 * Inserts an element to the node and adjusts the tree.
			 * First trys to reinsert elements of there is not
			 * enought free space. If this doesn't work, then
			 * spilts the node.
			 * @param tree The pointer of the tree.
			 * @param element The pointer of the element to insert.
			 * @param path_buffer Path to the node.
			 * @param oft Overflow table to prevent too much
			 * reinsert.
			 */
			bool insert_element(RStarTreePtr tree,
				const BoundedObjectSharedPtr &element,
				RStarTreeNodeSharedPtrStack &path_buffer,
				BitSet32 &oft);

			/**
			 * @brief Searches a leaf that holds too few element.
			 *
			 * Searches a leaf that holds less than minimum_load
			 * elements.
			 * @param minimum_load The minimum number of elementes.
			 * @param path_buffer The path to the element.
			 * @return The pointer of a leaf node if found, else 0.
			 */
			RStarTreeNodeSharedPtr find_small_leaf(
				const Uint32 minimum_load,
				RStarTreeNodeSharedPtrStack &path_buffer);

			bool check_nodes() const;

			/**
			 * @brief Deletes an element.
			 *
			 * Deletes an element from the node.
			 * @param element The pointer of the element to delete.
			 */
			inline void delete_element(
				const BoundedObjectSharedPtr &element)
			{
				assert(get_leaf());

				remove_element(get_index(element));

				assert(find_element(element) == -1);
			}

			/**
			 * @brief Removes all elements.
			 *
			 * Removes alls element from the node.
			 */
			inline void clear() throw ()
			{
				m_count = 0;

				BOOST_FOREACH(BoundedObjectSharedPtr &element,
					m_elements)
				{
					element.reset();
				}
			}

		public:
			/**
			 * @brief Constructor for the node.
			 *
			 * Constructor for the node.
			 * @param tree The tree where this node belongs to.
			 * @param level The level of this node.
			 */
			RStarTreeNode(const Uint32 level);
			virtual ~RStarTreeNode() throw();

			/**
			 * @brief Returns true if this is this a leaf node.
			 *
			 * Returns true if this node is a leaf.
			 * @return True if this node is a leaf else false.
			 */
			inline bool get_leaf() const throw ()
			{
				return get_level() == 0;
			}

			/**
			 * @brief Maximum item count.
			 *
			 * Returns the maximum number of items that the node
			 * can hold.
			 * @return The maximum item count.
			 */
			static inline Uint32 get_max_count() throw ()
			{
				return BoundedObjectSharedPtrElements::size();
			}

			/**
			 * @brief Item count.
			 *
			 * Returns the number of items that the node holds.
			 * @return The item count.
			 */
			inline Uint32 get_count() const throw ()
			{
				return m_count;
			}

			/**
			 * @brief Node level.
			 *
			 * Returns the level of the node.
			 * @return The level of the node.
			 */
			inline Uint32 get_level() const throw ()
			{
				return m_level;
			}

			/**
			 * @brief Sub nodes count.
			 *
			 * Returns the number of sub nodes that the node holds.
			 * @return The sub nodes count.
			 */
			Uint32 get_sub_nodes_count() const throw();

			Uint32 get_main_axe(
				const BoundedObjectSharedPtr &element) const;
	};
	/**
	 * @}
	 */

}
/**
 * @}
 */

#endif	/* UUID_49455312_8ca1_427c_9d0a_a97d432ebde3 */

