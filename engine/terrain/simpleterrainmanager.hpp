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

			void add_terrain_pages(const String &effect,
				const ImageSharedPtr &vector_map,
				const ImageSharedPtr &normal_map,
				const ImageSharedPtr &dudv_map,
				const MeshBuilderSharedPtr &mesh_builder,
				const MaterialSharedPtrVector &materials,
				const bool low_quality, const bool use_simd);
			void set_terrain_page(const ImageSharedPtr &vector_map,
				const ImageSharedPtr &normal_map,
				const ImageSharedPtr &dudv_map,
				const AbstractMeshSharedPtr &mesh,
				const glm::uvec2 &tile_offset,
				const glm::vec2 &position_scale,
				const Uint32 vertex_count,
				const Uint32 index_count,
				const bool low_quality);

		public:
			SimpleTerrainManager(const ImageSharedPtr &vector_map,
				const ImageSharedPtr &normal_map,
				const ImageSharedPtr &dudv_map,
				const GlobalVarsSharedPtr &global_vars,
				const MeshBuilderSharedPtr &mesh_builder,
				const MaterialSharedPtrVector &materials);
			virtual ~SimpleTerrainManager() throw();
			virtual void intersect(const Frustum &frustum,
				ObjectVisitor &visitor) const;

	};

}

#endif	/* UUID_bd8dfa8f_50ff_410f_b978_bffb88e599a0 */
