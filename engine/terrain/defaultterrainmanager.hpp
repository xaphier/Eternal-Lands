/****************************************************************************
 *            defaultterrainmanager.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_f27767cc_1833_464b_949a_d25aef5e7d4b
#define	UUID_f27767cc_1833_464b_949a_d25aef5e7d4b

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "abstractterrainmanager.hpp"

/**
 * @file
 * @brief The @c class DefaultTerrainManager.
 * This file contains the @c class DefaultTerrainManager.
 */
namespace eternal_lands
{

	class DefaultTerrainManager: public AbstractTerrainManager
	{
		private:
			boost::scoped_ptr<RStarTree> m_object_tree;

			void add_terrain_pages(const String &effect,
				const ImageSharedPtr &height_map,
				const MeshBuilderSharedPtr &mesh_builder,
				const EffectCacheSharedPtr &effect_cache,
				const bool low_quality, const bool use_simd);
			void set_terrain_page(const HeightMapUvTool &uvs,
				const ImageSharedPtr &height_map,
				const glm::uvec2 &tile_offset,
				const glm::uvec2 &terrain_size,
				const Uint32Vector &index_count,
				const Uint32 vertex_count,
				MeshDataToolSharedPtr &mesh_data_tool);

		public:
			DefaultTerrainManager(
				const CodecManagerSharedPtr &codec_manager,
				const FileSystemSharedPtr &file_system,
				const GlobalVarsSharedPtr &global_vars,
				const MeshBuilderSharedPtr &mesh_builder,
				const EffectCacheSharedPtr &effect_cache,
				const TerrainData &terrain_data);
			virtual ~DefaultTerrainManager() throw();
			virtual void intersect(const Frustum &frustum,
				ObjectVisitor &visitor) const;

	};

}

#endif	/* UUID_f27767cc_1833_464b_949a_d25aef5e7d4b */
