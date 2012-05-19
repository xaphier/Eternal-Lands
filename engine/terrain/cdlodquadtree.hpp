/****************************************************************************
 *            cdlodquadtree.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
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

	struct PatchData;

	class CdLodQuadTree
	{
		private:
			struct LodDescription
			{
				glm::vec2 morph_params;
				float range_start;
				Uint32 patch_size;
			};

			boost::array<LodDescription, 8> m_lods;
			glm::vec3 m_min, m_max;
			glm::uvec2 m_grid_size;
			float m_cell_size;
			float m_morph_zone_ratio;
			Uint32 m_lod_count;
			Uint32 m_patch_size;

		protected:
			void calculate_lod_params();
			void add_patch_to_queue(const glm::uvec2 &position,
				const Uint16 level, Mat2x4Vector &instances)
				const;
			void select_quads_for_drawing(const Frustum &frustum,
				const glm::vec3 &camera_position,
				const glm::uvec2 &position,
				const PlanesMask mask, const Uint16 level,
				Mat2x4Vector &instances) const;
			static Uint16 get_quad_order(const glm::vec2 &dir)
				noexcept;
			static const glm::uvec2 &get_quad_order(
				const Uint16 quad_order, const Uint16 index)
				noexcept;

			static inline glm::uvec2 get_quad_order(
				const Uint32 offset, const Uint16 quad_order,
				const Uint16 index) noexcept
			{
				return get_quad_order(quad_order, index) *
					offset;
			}

		public:
			CdLodQuadTree(const glm::vec3 &min,
				const glm::vec3 &max, const glm::uvec2 &size);
			~CdLodQuadTree() noexcept;
			void select_quads_for_drawing(const Frustum &frustum,
				const glm::vec3 &camera,
				Mat2x4Vector &instances) const;

			inline Uint32 get_max_lod_count() const noexcept
			{
				return m_lods.size();
			}

			static inline Uint32 get_max_patch_count() noexcept
			{
				return 4096;
			}

	};

}

#endif	/* UUID_710c4558_b937_42c2_af6b_9e26370d4e50 */
