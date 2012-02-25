/***************************************************************************
 *            rstartree.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_65213858_5fe9_4ae2_a056_df83e55a160f
#define	UUID_65213858_5fe9_4ae2_a056_df83e55a160f

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "frustum.hpp"
#include "rstartreenode.hpp"

/**
 * @file
 * @brief The @c class RStarTree.
 * This file contains the @c class RStarTree.
 */
namespace eternal_lands
{

	/**
	 * @brief The r*-tree for RStarTreeNode's.
	 *
	 * This is the main object for the r*-tree.
	 */
	class RStarTree: public boost::noncopyable
	{
		friend class RStarTreeNode;
		private:
			/**
			 * @brief Root node.
			 *
			 * The root node.
			 */
			RStarTreeNodeSharedPtr m_root_node;

			/**
			 * @brief Split distribution factor.
			 *
			 * The split distribution factor. Must be between 0.0 and 1.0.
			 */
			float m_split_distribution_factor;

			/**
			 * @brief Reinsert factor.
			 *
			 * The reinsert factor. Must be between 0.0 and 1.0.
			 */
			float m_reinsert_factor;

			/**
			 * @brief Fill factor.
			 *
			 * The fill factor. Must be between 0.0 and 1.0.
			 */
			float m_fill_factor;

		protected:
			/**
			 * @brief Validates the factor.
			 *
			 * Validates the factor. The factor must be between 0.0
			 * and 1.0.
			 * @param factor The factor to validate.
			 * @return True if the factor is valid, else false.
			 */
			bool valid_factor(const float factor) const throw ();

			/**
			 * @brief Sets root node.
			 *
			 * Sets the root node and deletes the old one.
			 * @param node The new root node.
			 */
			void set_root_node(const RStarTreeNodeSharedPtr &node);

			/**
			 * @brief Adds a new root node.
			 *
			 * Adds a new node an sets the root node to it.
			 * @param level The level of the new root node.
			 * @return The new root node.
			 */
			const RStarTreeNodeSharedPtr &add_new_root_node(
				const Uint32 level);

			/**
			 * @brief Gets the root node.
			 *
			 * Gets the root node.
			 * @return The root node.
			 */
			inline const RStarTreeNodeSharedPtr &get_root_node() const
			{
				return m_root_node;
			}

			/**
			 * @brief Add reinsert items.
			 *
			 * Add items to the tree that got removed while
			 * condensing the tree.
			 * @parameter element The element to add.
			 * @parameter level The level of the node where to add.
			 * @parameter oft The overflow table used to prevent
			 * endless reinsertation.
			 */
			void add_data(const BoundedObjectSharedPtr &element,
				const Uint32 level, BitSet32 &oft);

			/**
			 * @brief Condense the tree.
			 *
			 * Condense the tree and updates the bounding boxes of
			 * the nodes after a remove of an element. Nodes with
			 * insufficient elements are saved in the reinsert
			 * vector and then are removed from the tree.
			 * @param reinsert The vector that get filled with the
			 * removed nodes.
			 * @param path_buffer The path to the node.
			 * @param node The node to start with.
			 * @return The id of the new root node.
			 */
			RStarTreeNodeSharedPtr condense_tree(
				RStarTreeNodeSharedPtrVector &reinsert,
				RStarTreeNodeSharedPtrStack &path_buffer,
				const RStarTreeNodeSharedPtr &node);

			/**
			 * @brief Reinsert elements of nodes.
			 *
			 * The given nodes are removed and the elements of this
			 * nodes are insert again in the tree.
			 * @param reinsert The id's of the nodes.
			 */
			void reinsert_nodes(
				const RStarTreeNodeSharedPtrVector &reinsert);

		public:
			/**
			 * @brief Default constructor.
			 *
			 * Default constructor.
			 */
			RStarTree();

			/**
			 * @brief Default destructor.
			 *
			 * Default destructor.
			 */
			~RStarTree() throw();

			/**
			 * @brief Split distribution factor.
			 *
			 * Returns the split distribution factor. Is between
			 * 0.0 and 1.0.
			 * @return The split distribution factor.
			 */
			inline float get_split_distribution_factor() const
			{
				return m_split_distribution_factor;
			}

			/**
			 * @brief Sets the split distribution factor.
			 *
			 * Sets the split distribution factor. Must be between
			 * 0.0 and 1.0.
			 * @throw ExtendedException If factor is not between
			 * 0.0 and 1.0.
			 * @param value The new split distribution factor.
			 */
			void set_split_distribution_factor(const float value);

			/**
			 * @brief Reinsert factor.
			 *
			 * Returns the reinsert factor. Is between 0.0 and 1.0.
			 * @return The reinsert factor.
			 */
			inline float get_reinsert_factor() const
			{
				return m_reinsert_factor;
			}

			/**
			 * @brief Sets the reinsert factor.
			 *
			 * Sets the reinsert factor. Must be between 0.0 and
			 * 1.0.
			 * @throw ExtendedException If factor is not between
			 * 0.0 and 1.0.
			 * @param value The new reinsert factor.
			 */
			void set_reinsert_factor(const float value);

			/**
			 * @brief Fill factor.
			 *
			 * Returns the fill factor. Is between 0.0 and 1.0.
			 * @return The fill factor.
			 */
			inline float get_fill_factor() const
			{
				return m_fill_factor;
			}

			/**
			 * @brief Sets the fill factor.
			 *
			 * Sets the fill factor. Must be between 0.0 and 1.0.
			 * @throw ExtendedException If factor is not between
			 * 0.0 and 1.0.
			 * @param value The new fill factor.
			 */
			void set_fill_factor(const float value);

			/**
			 * @brief Adds an element.
			 *
			 * Adds an element to the tree.
			 * @throw ExtendedException If the bounding volume of
			 * the item or the item itself is not valid.
			 * @throw ExtendedException If the box is not valid.
			 * @param element The element to add.
			 */
			void add(const BoundedObjectSharedPtr &element);

			/**
			 * @brief Removes an element.
			 *
			 * Removes an element from the tree.
			 * @throw ExtendedException If the bounding volume of
			 * the item or the item itself is not valid or if the
			 * item is not in the tree.
			 * @param element The element to delete.
			 */
			void remove(const BoundedObjectSharedPtr &element);

			/**
			 * @brief Removes an item.
			 *
			 * Removes an item from the tree.
			 * @throw ExtendedException If the bounding volume of
			 * the item or the item itself is not valid or if the
			 * item is not in the tree.
			 * @param bounding_volume The bounding volume of the
			 * item.
			 * @param item The item to delete.
			 */
			Uint32 optimize(const Uint32 count);

			/**
			 * @brief Tests all elements of the tree for
			 * intersection.
			 *
			 * Test the frustum for intersection wiht all elements
			 * in the tree. The id's of the intersecting elements
			 * (only elements in leafs) are saved in the vector
			 * intersects.
			 * @param frustum The frustum used for the intersection
			 * test.
			 * @param visitor The visitor that gets the
			 * intersecting items.
			 */
			void intersect(const Frustum &frustum,
				AbstractBoundedObjectVisitor &visitor) const;

			/**
			 * @brief Clears the tree.
			 *
			 * Clears the tree and removes all nodes, then a fresh
			 * root node is added.
			 */
			void clear();

			/**
			 * @brief Returns the maximun of elements per node.
			 *
			 * Returns the maximun of elements per node.
			 * @return The maximun of elements per node.
			 */
			static Uint32 get_max_elements_per_node() throw();

			/**
			 * @brief Axis aligned bounding box of the tree.
			 *
			 * The axis aligned bounding box of the tree that
			 * encloses all items of the tree.
			 * @return The axis aligned bounding box of the tree.
			 */
			const BoundingBox &get_bounding_box() const;

			/**
			 * @brief Returns if the tree is empty.
			 *
			 * Returns true if the tree contains no items, else false.
			 * @return True if the tree contains no items, else false.
			 */
			bool get_empty() const;

			/**
			 * @brief Nodes count.
			 *
			 * Returns the number of nodes in the tree.
			 * @return The nodes count.
			 */
			Uint32 get_nodes_count() const throw();

			/**
			 * @brief Memory usage.
			 *
			 * Returns the memory usage of the tree and all its nodes.
			 * @return The memory usage.
			 */
			Uint32 get_memory_usage() const throw();

			bool check_tree() const;

	};

}

#endif	/* UUID_65213858_5fe9_4ae2_a056_df83e55a160f */

