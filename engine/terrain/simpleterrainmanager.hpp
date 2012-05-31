/****************************************************************************
 *            simpleterrainmanager.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_bd8dfa8f_50ff_410f_b978_bffb88e599a0
#define	UUID_bd8dfa8f_50ff_410f_b978_bffb88e599a0

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "abstractterrainmanager.hpp"

/**
 * @file
 * @brief The @c class SimpleTerrainManager.
 * This file contains the @c class SimpleTerrainManager.
 */
namespace eternal_lands
{

	class SimpleTerrainManager: public AbstractTerrainManager
	{
		private:
			boost::scoped_ptr<RStarTree> m_object_tree;
			MaterialSharedPtr m_material;

			void add_terrain_pages(const ImageSharedPtr &vector_map,
				const ImageSharedPtr &normal_map,
				const ImageSharedPtr &dudv_map,
				const MeshBuilderSharedPtr &mesh_builder,
				const bool low_quality, const bool use_simd);
			void set_terrain_page(const ImageSharedPtr &vector_map,
				const ImageSharedPtr &normal_map,
				const ImageSharedPtr &dudv_map,
				const AbstractMeshSharedPtr &mesh,
				const glm::uvec2 &tile_offset,
				const glm::vec2 &position_scale,
				const Uint32 vertex_count,
				const Uint32 index_count);
			void set_terrain_page_low_quality(
				const ImageSharedPtr &vector_map,
				const ImageSharedPtr &normal_map,
				const ImageSharedPtr &dudv_map,
				const AbstractMeshSharedPtr &mesh,
				const glm::uvec2 &tile_offset,
				const glm::vec2 &position_scale,
				const Uint32 vertex_count,
				const Uint32 index_count);

		public:
			SimpleTerrainManager(const ImageSharedPtr &vector_map,
				const ImageSharedPtr &normal_map,
				const ImageSharedPtr &dudv_map,
				const GlobalVarsSharedPtr &global_vars,
				const MeshBuilderSharedPtr &mesh_builder,
				const MaterialSharedPtr &material);
			virtual ~SimpleTerrainManager() noexcept;
			virtual void intersect(const Frustum &frustum,
				ObjectVisitor &visitor) const;
			virtual void intersect(const Frustum &frustum,
				const glm::vec3 &camera,
				TerrainVisitor &terrain) const;
			virtual void set_clipmap_texture(
				const TextureSharedPtr &texture);

			inline const MaterialSharedPtr &get_material() const
				noexcept
			{
				return m_material;
			}

	};

}

#endif	/* UUID_bd8dfa8f_50ff_410f_b978_bffb88e599a0 */
