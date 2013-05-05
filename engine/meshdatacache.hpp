/****************************************************************************
 *            meshdatacache.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
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

			const GlobalVarsConstSharedPtr m_global_vars;
			const FileSystemConstSharedPtr m_file_system;
			MeshDataCacheMap m_mesh_data_cache;

			inline const FileSystemConstSharedPtr &get_file_system()
				const noexcept
			{
				return m_file_system;
			}

			inline const GlobalVarsConstSharedPtr &get_global_vars()
				const noexcept
			{
				return m_global_vars;
			}

			void load_mesh(const String &name,
				MeshDataToolSharedPtr &mesh_data_tool,
				StringVector &materials);

		public:
			/**
			 * Default constructor.
			 */
			MeshDataCache(
				const GlobalVarsConstSharedPtr &global_vars,
				const FileSystemConstSharedPtr &file_system);

			/**
			 * Default destructor.
			 */
			~MeshDataCache() noexcept;
			void get_mesh_data(const String &name,
				MeshDataToolSharedPtr &mesh_data_tool,
				StringVector &materials);
			void get_mesh_data(const String &name,
				MeshDataToolSharedPtr &mesh_data_tool);
			const StringVector &get_mesh_materials(
				const String &name);

	};

}

#endif	/* UUID_66b7f2e4_7d6a_45d3_a86d_561316e19087 */
