/****************************************************************************
 *            simpleterrainmanager.hpp
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
			AbstractMeshSharedPtr m_mesh;
			const bool m_low_quality;

			void init_terrain_pages(
				const MeshBuilderSharedPtr &mesh_builder,
				const bool use_simd);
			void set_terrain_page(
				const ImageSharedPtr &displacement_map,
				const ImageSharedPtr &normal_map,
				const ImageSharedPtr &dudv_map,
				const AbstractMeshSharedPtr &mesh,
				const glm::uvec2 &tile_offset,
				const glm::vec2 &position_scale);
			void set_terrain_page_low_quality(
				const ImageSharedPtr &displacement_map,
				const ImageSharedPtr &normal_map,
				const ImageSharedPtr &dudv_map,
				const AbstractMeshSharedPtr &mesh,
				const glm::uvec2 &tile_offset,
				const glm::vec2 &position_scale);
			virtual TextureSharedPtr get_displacement_texture()
				const;
			virtual TextureSharedPtr get_normal_texture() const;
			virtual TextureSharedPtr get_dudv_texture() const;

		protected:
			void add_terrain_page(
				const ImageSharedPtr &displacement_map,
				const ImageSharedPtr &normal_map,
				const ImageSharedPtr &dudv_map,
				const glm::uvec2 &position);
			void remove_terrain_page(const glm::uvec2 &position);

		public:
			SimpleTerrainManager(
				const GlobalVarsSharedPtr &global_vars,
				const MeshBuilderSharedPtr &mesh_builder,
				const MaterialSharedPtr &material);
			virtual ~SimpleTerrainManager() noexcept override;
			virtual void intersect(const Frustum &frustum,
				ObjectVisitor &visitor) const override;
			virtual void intersect(const Frustum &frustum,
				const glm::vec3 &camera,
				BoundingBox &bounding_box) const override;
			virtual void intersect(const Frustum &frustum,
				const glm::vec3 &camera,
				TerrainVisitor &terrain) const override;
			virtual void update(
				const ImageSharedPtr &displacement_map,
				const ImageSharedPtr &normal_map,
				const ImageSharedPtr &dudv_map) override;
			virtual void set_clipmap_texture(
				const TextureSharedPtr &texture) override;
			virtual void clear() override;

			inline const MaterialSharedPtr &get_material() const
				noexcept
			{
				return m_material;
			}

			inline bool get_low_quality() const noexcept
			{
				return m_low_quality;
			}

	};

}

#endif	/* UUID_bd8dfa8f_50ff_410f_b978_bffb88e599a0 */
