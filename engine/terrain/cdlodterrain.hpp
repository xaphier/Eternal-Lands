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

			virtual TextureSharedPtr get_displacement_texture()
				const override;
			virtual TextureSharedPtr get_normal_texture() const
				override;
			virtual TextureSharedPtr get_dudv_texture() const
				override;
			virtual void do_set_geometry_maps(
				const ImageSharedPtr &displacement_map,
				const ImageSharedPtr &normal_map,
				const ImageSharedPtr &dudv_map) override;
			virtual void do_update_geometry_maps(
				const ImageUpdate &displacement_map,
				const ImageUpdate &normal_map,
				const ImageUpdate &dudv_map) override;

		public:
			CdLodTerrain(const GlobalVarsSharedPtr &global_vars,
				const EffectCacheSharedPtr &effect_cache,
				const MeshCacheSharedPtr &mesh_cache,
				const MaterialBuilderSharedPtr
					&material_builder,
				const MaterialCacheSharedPtr &material_cache,
				const String &material, const String &effect);
			virtual ~CdLodTerrain() noexcept override;
			virtual void intersect(const Frustum &frustum,
				ObjectVisitor &visitor) const override;
			virtual void intersect(const Frustum &frustum,
				const glm::vec3 &camera,
				BoundingBox &bounding_box) const override;
			virtual void intersect(const Frustum &frustum,
				const glm::vec3 &camera,
				TerrainVisitor &terrain) const override;
			virtual void clear() override;

	};

}

#endif	/* UUID_af0d2160_00a9_4a5b_b11e_2963cdb83743 */
