/****************************************************************************
 *            cdlodterrainmanager.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_af0d2160_00a9_4a5b_b11e_2963cdb83743
#define	UUID_af0d2160_00a9_4a5b_b11e_2963cdb83743

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "abstractterrainmanager.hpp"

/**
 * @file
 * @brief The @c class CdLodTerrainManager.
 * This file contains the @c class CdLodTerrainManager.
 */
namespace eternal_lands
{

	class CdLodTerrainManager: public AbstractTerrainManager
	{
		private:
			boost::scoped_ptr<CdLodQuadTree> m_cd_lod_quad_tree;
			MaterialSharedPtr m_material;
			AbstractMeshSharedPtr m_mesh;

		public:
			CdLodTerrainManager(const ImageSharedPtr &vector_map,
				const ImageSharedPtr &normal_map,
				const ImageSharedPtr &dudv_map,
				const GlobalVarsSharedPtr &global_vars,
				const MeshCacheSharedPtr &mesh_cache,
				const MaterialSharedPtr &material);
			virtual ~CdLodTerrainManager() noexcept;
			virtual void intersect(const Frustum &frustum,
				ObjectVisitor &visitor) const override;
			virtual void intersect(const Frustum &frustum,
				const glm::vec3 &camera,
				BoundingBox &bounding_box) const;
			virtual void intersect(const Frustum &frustum,
				const glm::vec3 &camera,
				TerrainVisitor &terrain) const override;
			virtual void set_clipmap_texture(
				const TextureSharedPtr &texture) override;
			virtual void update(const ImageSharedPtr &vector_map,
				const ImageSharedPtr &normal_map,
				const ImageSharedPtr &dudv_map);

			inline const MaterialSharedPtr &get_material() const
				noexcept
			{
				return m_material;
			}

	};

}

#endif	/* UUID_af0d2160_00a9_4a5b_b11e_2963cdb83743 */
