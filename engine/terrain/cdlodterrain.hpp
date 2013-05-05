/****************************************************************************
 *            cdlodterrain.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_af0d2160_00a9_4a5b_b11e_2963cdb83743
#define	UUID_af0d2160_00a9_4a5b_b11e_2963cdb83743

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "abstractterrain.hpp"

/**
 * @file
 * @brief The @c class CdLodTerrain.
 * This file contains the @c class CdLodTerrain.
 */
namespace eternal_lands
{

	class CdLodTerrain: public AbstractTerrain
	{
		private:
			boost::scoped_ptr<CdLodQuadTree> m_cd_lod_quad_tree;
			AbstractMeshSharedPtr m_mesh;
			TextureSharedPtr m_displacement_texture;
			TextureSharedPtr m_normal_texture;
			TextureSharedPtr m_dudv_texture;

			virtual void do_set_geometry_maps(
				const ImageConstSharedPtr &displacement_map,
				const ImageConstSharedPtr &normal_tangent_map,
				const ImageConstSharedPtr &dudv_map) override;
			virtual void do_update_geometry_maps(
				const ImageConstSharedPtr &displacement_map,
				const ImageConstSharedPtr &normal_tangent_map,
				const ImageConstSharedPtr &dudv_map) override;

		public:
			CdLodTerrain(
				const GlobalVarsConstSharedPtr &global_vars,
				const EffectCacheSharedPtr &effect_cache,
				const MeshBuilderConstSharedPtr &mesh_builder,
				const MeshCacheSharedPtr &mesh_cache,
				const MeshDataCacheSharedPtr &mesh_data_cache,
				const MaterialBuilderConstSharedPtr
					&material_builder,
				const MaterialCacheSharedPtr &material_cache,
				const StringArray3 &material,
				const String &effect);
			virtual ~CdLodTerrain() noexcept override;
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

#endif	/* UUID_af0d2160_00a9_4a5b_b11e_2963cdb83743 */
