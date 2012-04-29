/****************************************************************************
 *            meshcache.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
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
				const
			{
				MeshDataCacheSharedPtr result;

				result = m_mesh_data_cache.lock();

				assert(result.get() != nullptr);

				return result;
			}

			inline MeshBuilderSharedPtr get_mesh_builder() const
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
			~MeshCache() throw();

			void get_mesh(const String &name,
				AbstractMeshSharedPtr &mesh,
				StringVector &materials);

			void get_mesh(const String &name,
				AbstractMeshSharedPtr &mesh);

	};

}

#endif	/* UUID_31ce4a8c_bbf4_4d70_95d5_ad9816e26708 */

