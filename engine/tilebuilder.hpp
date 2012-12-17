/****************************************************************************
 *            tilebuilder.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_83d73ea6_026b_4989_976d_4d8c337ebbb2
#define	UUID_83d73ea6_026b_4989_976d_4d8c337ebbb2

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class TileTool.
 * This file contains the @c class TileTool.
 */
namespace eternal_lands
{

	class TileBuilder
	{
		private:
			const MaterialCacheWeakPtr m_material_cache;
			AbstractMeshSharedPtr m_mesh;

			inline MaterialCacheSharedPtr get_material_cache() const
				noexcept
			{
				MaterialCacheSharedPtr result;

				result = m_material_cache.lock();

				assert(result.get() != nullptr);

				return result;
			}

		public:
			/**
			 * Default constructor.
			 */
			TileBuilder(const GlobalVarsSharedPtr &global_vars,
				const MeshBuilderSharedPtr &mesh_builder,
				const MaterialCacheSharedPtr &material_cache);

			/**
			 * Default destructor.
			 */
			~TileBuilder() noexcept;

			void get_tile(const Uint16MultiArray2 &tile_page,
				AbstractMeshSharedPtr &mesh,
				MaterialSharedPtrVector &materials,
				BitSet64 &blend_mask);

			static inline Uint32 get_tile_size() noexcept
			{
				return 16;
			}

			static inline float get_tile_world_scale() noexcept
			{
				return 0.5f;
			}

			static inline float get_tile_uv_scale() noexcept
			{
				return 0.25f;
			}

	};

}

#endif	/* UUID_83d73ea6_026b_4989_976d_4d8c337ebbb2 */
