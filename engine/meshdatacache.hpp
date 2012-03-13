/****************************************************************************
 *            meshdatacache.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_66b7f2e4_7d6a_45d3_a86d_561316e19087
#define	UUID_66b7f2e4_7d6a_45d3_a86d_561316e19087

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class MeshDataCache.
 * This file contains the @c class MeshDataCache.
 */
namespace eternal_lands
{

	/**
	 * @brief @c class for mesh data cache.
	 *
	 * @c class for mesh data cache.
	 */
	class MeshDataCache: public boost::noncopyable
	{
		private:
			class MeshDataCacheItem;

			typedef std::map<String, MeshDataCacheItem>
				MeshDataCacheMap;

			const MaterialDescriptionCacheWeakPtr
				m_material_description_cache;
			const FileSystemWeakPtr m_file_system;
			const GlobalVarsSharedPtr m_global_vars;
			MeshDataCacheMap m_mesh_data_cache;

			inline FileSystemSharedPtr get_file_system() const
			{
				FileSystemSharedPtr result;

				result = m_file_system.lock();

				assert(result.get() != 0);

				return result;
			}

			inline const GlobalVarsSharedPtr &get_global_vars()
				const
			{
				return m_global_vars;
			}

			inline MaterialDescriptionCacheSharedPtr
				get_material_description_cache() const
			{
				MaterialDescriptionCacheSharedPtr result;

				result = m_material_description_cache.lock();

				assert(result.get() != 0);

				return result;
			}

			void load_mesh(const String &name,
				MeshDataToolSharedPtr &mesh_data_tool,
				MaterialDescriptionVector &materials);

		public:
			/**
			 * Default constructor.
			 */
			MeshDataCache(const MaterialDescriptionCacheWeakPtr
					&material_description_cache,
				const FileSystemWeakPtr &file_system,
				const GlobalVarsSharedPtr &global_vars);

			/**
			 * Default destructor.
			 */
			~MeshDataCache() throw();

			void get_mesh_data(const String &name,
				MeshDataToolSharedPtr &mesh_data_tool,
				MaterialDescriptionVector &materials);

			void get_mesh_data(const String &name,
				MeshDataToolSharedPtr &mesh_data_tool);

	};

}

#endif	/* UUID_66b7f2e4_7d6a_45d3_a86d_561316e19087 */
