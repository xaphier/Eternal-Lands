/****************************************************************************
 *            simpleterrain.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_bd8dfa8f_50ff_410f_b978_bffb88e599a0
#define	UUID_bd8dfa8f_50ff_410f_b978_bffb88e599a0

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "abstractterrain.hpp"

/**
 * @file
 * @brief The @c class SimpleTerrain.
 * This file contains the @c class SimpleTerrain.
 */
namespace eternal_lands
{

	class SimpleTerrain: public AbstractTerrain
	{
		private:
			boost::scoped_ptr<RStarTree> m_object_tree;
			AbstractMeshSharedPtr m_mesh;

			void init_terrain_pages(
				const MeshBuilderSharedPtr &mesh_builder,
				const bool use_simd);
			void set_terrain_page(
				const ImageSharedPtr &displacement_map,
				const ImageSharedPtr &normal_tangent_map,
				const ImageSharedPtr &dudv_map,
				const AbstractMeshSharedPtr &mesh,
				const glm::uvec2 &tile_offset,
				const glm::vec2 &position_scale);
			void set_terrain_page_low_quality(
				const ImageSharedPtr &displacement_map,
				const ImageSharedPtr &normal_tangent_map,
				const ImageSharedPtr &dudv_map,
				const AbstractMeshSharedPtr &mesh,
				const glm::uvec2 &tile_offset,
				const glm::vec2 &position_scale);
			virtual void do_set_geometry_maps(
				const ImageSharedPtr &displacement_map,
				const ImageSharedPtr &normal_tangent_map,
				const ImageSharedPtr &dudv_map) override;
			virtual void do_update_geometry_maps(
				const ImageSharedPtr &displacement_map,
				const ImageSharedPtr &normal_tangent_map,
				const ImageSharedPtr &dudv_map) override;

		protected:
			void add_terrain_page(
				const ImageSharedPtr &displacement_map,
				const ImageSharedPtr &normal_tangent_map,
				const ImageSharedPtr &dudv_map,
				const glm::uvec2 &position);
			void remove_terrain_page(const glm::uvec2 &position);

		public:
			SimpleTerrain(const GlobalVarsSharedPtr &global_vars,
				const EffectCacheSharedPtr &effect_cache,
				const MeshBuilderSharedPtr &mesh_builder,
				const MaterialBuilderSharedPtr
					&material_builder,
				const MaterialCacheSharedPtr &material_cache,
				const StringArray3 &material,
				const String &effect);
			virtual ~SimpleTerrain() noexcept override;
			virtual void intersect(const Frustum &frustum,
				ObjectVisitor &visitor) const override;
			virtual void intersect(const Frustum &frustum,
				const glm::vec3 &camera,
				BoundingBox &bounding_box) const override;
			virtual void intersect(const Frustum &frustum,
				const glm::vec3 &camera,
				TerrainVisitor &terrain) const override;
			virtual void init_rendering_data(
				TerrainRenderingData &rendering_data) const
				override;
			virtual void clear() override;

	};

}

#endif	/* UUID_bd8dfa8f_50ff_410f_b978_bffb88e599a0 */
