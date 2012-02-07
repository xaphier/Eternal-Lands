/****************************************************************************
 *            simpleterrainmanager.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_bd8dfa8f_50ff_410f_b978_bffb88e599a0
#define	UUID_bd8dfa8f_50ff_410f_b978_bffb88e599a0

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "basicterrainmanager.hpp"

/**
 * @file
 * @brief The @c class SimpleTerrainManager.
 * This file contains the @c class SimpleTerrainManager.
 */
namespace eternal_lands
{

	class SimpleTerrainManager: public BasicTerrainManager
	{
		private:
			boost::scoped_ptr<RStarTree> m_object_tree;

			void add_terrain_pages(const EffectDescription &effect,
				const ImageSharedPtr &height_map,
				const MeshBuilderSharedPtr &mesh_builder,
				const EffectCacheSharedPtr &effect_cache,
				const TextureCacheSharedPtr &texture_cache, 
				const bool low_quality, const bool use_simd);
			void set_terrain_page(const HeightMapUvTool &uvs,
				const ImageSharedPtr &height_map,
				const glm::uvec2 &tile_offset,
				const glm::uvec2 &terrain_size,
				const Uint32Vector &index_count,
				const Uint32 vertex_count,
				const Uint16 height_scale,
				MeshDataToolSharedPtr &mesh_data_tool);

		public:
			SimpleTerrainManager(
				const CodecManagerSharedPtr &codec_manager,
				const FileSystemSharedPtr &file_system,
				const GlobalVarsSharedPtr &global_vars,
				const MeshBuilderSharedPtr &mesh_builder,
				const EffectCacheSharedPtr &effect_cache,
				const TextureCacheSharedPtr &texture_cache,
				const String &name);
			virtual ~SimpleTerrainManager() throw();
			virtual void intersect(const Frustum &frustum,
				ObjectVisitor &visitor) const;

	};

}

#endif	/* UUID_bd8dfa8f_50ff_410f_b978_bffb88e599a0 */
