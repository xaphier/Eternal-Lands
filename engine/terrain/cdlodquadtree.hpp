/****************************************************************************
 *            cdlodquadtree.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_710c4558_b937_42c2_af6b_9e26370d4e50
#define	UUID_710c4558_b937_42c2_af6b_9e26370d4e50

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class CdLodQuadTree.
 * This file contains the @c class CdLodQuadTree.
 */
namespace eternal_lands
{

	class CdLodQuadTree
	{
		private:
			struct LodDescription
			{
				Vec3Array2MultiArray2 min_max;
				glm::vec2 morph_params;
				float range_start;
				Uint32 patch_scale;
			};

			boost::array<LodDescription, 8> m_lods;
			glm::vec3 m_min, m_max;
			glm::uvec2 m_grid_size;
			float m_patch_scale;
			Uint32 m_lod_count;
			const bool m_low_quality;

		protected:
			void init_min_max(const glm::uvec2 &position,
				const Uint16 level, glm::vec3 &min,
				glm::vec3 &max);
			void get_min_max_rgb10_a2(const ImageSharedPtr &image,
				const glm::uvec2 &offset, const Uint32 size,
				glm::vec3 &min, glm::vec3 &max);
			void get_min_max(const ImageSharedPtr &image,
				const glm::uvec2 &offset, const Uint32 size,
				glm::vec3 &min, glm::vec3 &max);
			void calculate_lod_params();
			void add_patch_to_queue(const glm::uvec2 &position,
				const AbstractWriteMemorySharedPtr &instances,
				const Uint64 offset, const Uint16 level,
				const Uint16 max_instances,
				Uint32 &instance_index) const;
			void select_quads_for_drawing(const Frustum &frustum,
				const glm::vec3 &camera_position,
				const glm::uvec2 &position,
				const AbstractWriteMemorySharedPtr &instances,
				const Uint64 offset, const BitSet64 mask,
				const Uint16 level,
				const Uint16 max_instances,
				BoundingBox &bounding_box,
				Uint32 &instance_count) const;
			void select_bounding_box(const Frustum &frustum,
				const glm::vec3 &camera_position,
				const glm::uvec2 &position,
				const BitSet64 mask, const Uint16 level,
				const Uint16 max_instances,
				BoundingBox &bounding_box,
				Uint32 &instance_count) const;
			void update_level_zero(
				const ImageSharedPtr &displacement_map,
				const glm::uvec2 &offset,
				const glm::uvec2 &size);
			void update_level_zero_rgb10_a2(
				const ImageSharedPtr &displacement_map,
				const glm::uvec2 &offset,
				const glm::uvec2 &size);

		public:
			CdLodQuadTree(const bool low_quality);
			~CdLodQuadTree() noexcept;
			/**
			 * @param camera The camera position that is used for
			 * lod calculation.
			 * @param instances Memory where to write the instance
			 * data. Data is written as eight floats for each
			 * instance, no padding between the floats.
			 * @param offset Offset to the instance data.
			 * @param max_instances Maximum number of instances to
			 * write in the instances memory buffer.
			 */
			void select_quads_for_drawing(const Frustum &frustum,
				const glm::vec3 &camera,
				const AbstractWriteMemorySharedPtr &instances,
				const Uint64 offset, const Uint16 max_instances,
				BoundingBox &bounding_box,
				glm::vec4 &terrain_lod_offset, 
				Uint32 &instance_count) const;
			void select_bounding_box(const Frustum &frustum,
				const glm::vec3 &camera,
				const Uint16 max_instances,
				BoundingBox &bounding_box) const;
			void init(const ImageSharedPtr &displacement_map,
				const float patch_scale);
			void clear();
			static const Uvec2Array4 &get_quad_order(
				const glm::vec2 &dir) noexcept;

			inline Uint32 get_max_lod_count() const noexcept
			{
				return m_lods.size();
			}

			inline Uint32 get_lod_count() const noexcept
			{
				return m_lod_count;
			}

			inline float get_max_z() const noexcept
			{
				return m_max.z;
			}

			static inline Uint32 get_patch_size() noexcept
			{
				return 16;
			}

			inline float get_patch_scale() const noexcept
			{
				return m_patch_scale;
			}

			inline float get_morph_zone_ratio() const noexcept
			{
				return 0.3f;
			}

			inline const glm::uvec2 &get_grid_size() const noexcept
			{
				return m_grid_size;
			}

			inline const glm::vec3 &get_min() const noexcept
			{
				return m_min;
			}

			inline const glm::vec3 &get_max() const noexcept
			{
				return m_max;
			}

	};

}

#endif	/* UUID_710c4558_b937_42c2_af6b_9e26370d4e50 */
