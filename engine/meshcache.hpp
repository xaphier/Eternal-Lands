/****************************************************************************
 *            meshcache.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_31ce4a8c_bbf4_4d70_95d5_ad9816e26708
#define	UUID_31ce4a8c_bbf4_4d70_95d5_ad9816e26708

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class MeshCache.
 * This file contains the @c class MeshCache.
 */
namespace eternal_lands
{

	/**
	 * @brief @c class for mesh cache.
	 *
	 * @c class for mesh cache.
	 */
	class MeshCache: public boost::noncopyable
	{
		private:
			class MeshCacheItem;

			typedef std::map<String, MeshCacheItem> MeshCacheMap;

			MeshCacheMap m_mesh_cache;
			const MeshBuilderWeakPtr m_mesh_builder;
			const MeshDataCacheWeakPtr m_mesh_data_cache;

			inline MeshDataCacheSharedPtr get_mesh_data_cache()
				const noexcept
			{
				MeshDataCacheSharedPtr result;

				result = m_mesh_data_cache.lock();

				assert(result.get() != nullptr);

				return result;
			}

			inline MeshBuilderSharedPtr get_mesh_builder() const
				noexcept
			{
				MeshBuilderSharedPtr result;

				result = m_mesh_builder.lock();

				assert(result.get() != nullptr);

				return result;
			}

			void load_mesh(const String &name,
				AbstractMeshSharedPtr &mesh,
				StringVector &materials) const;

		public:
			/**
			 * Default constructor.
			 */
			MeshCache(const MeshBuilderWeakPtr &mesh_builder,
				const MeshDataCacheWeakPtr &mesh_data_cache);

			/**
			 * Default destructor.
			 */
			~MeshCache() noexcept;

			void get_mesh(const String &name,
				AbstractMeshSharedPtr &mesh,
				StringVector &materials);

			void get_mesh(const String &name,
				AbstractMeshSharedPtr &mesh);

			/**
			 * Sets the given mesh and material in the cache.
			 * @param name Index name of the mesh.
			 * @param mesh Mesh to set in the cache.
			 * @param materials Default materials for the mesh.
			 */
			void set_mesh(const String &name,
				const AbstractMeshSharedPtr &mesh,
				const StringVector &materials);

			/**
			 * Adds the given mesh and material to the cache.
			 * @param name Index name of the mesh.
			 * @return True if there is a mesh in the cache with
			 * the name, false else.
			 */
			bool get_has_mesh(const String &name) const;

	};

}

#endif	/* UUID_31ce4a8c_bbf4_4d70_95d5_ad9816e26708 */

