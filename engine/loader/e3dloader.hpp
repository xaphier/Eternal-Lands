/****************************************************************************
 *            e3dloader.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_73016a6d_d28a_4164_ae25_a1e2f9944c6a
#define	UUID_73016a6d_d28a_4164_ae25_a1e2f9944c6a

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class E3dLoader.
 * This file contains the @c class E3dLoader.
 */
namespace eternal_lands
{

	/**
	 * @brief @c Class for loading e3d files.
	 *
	 * @c Class E3dLoader is used for loading e3d files.
	 * @see MeshCache
	 */
	class E3dLoader
	{
		private:
			ReaderSharedPtr m_reader;

			void load_header();
			void load_vertex(
				const MeshDataToolSharedPtr &mesh_data_tool,
				const Uint32 options, const Uint32 format,
				const Uint32 vertex_count,
				const Uint32 vertex_size,
				const Uint32 vertex_offset);
			void load_index(
				const MeshDataToolSharedPtr &mesh_data_tool,
				const Uint32 index_count,
				const Uint32 index_size,
				const Uint32 index_offset);
			SubMesh load_sub_mesh(const Uint32 material_offset,
				const Uint32 material_size,
				const Uint32 material_index);
			void load_sub_meshs(
				const MeshDataToolSharedPtr &mesh_data_tool,
				const Uint32 material_count,
				const Uint32 material_size,
				const Uint32 material_offset);
			MaterialDescription load_material(
				const FileSystemSharedPtr &file_system,
				const Uint32 material_offset,
				const Uint32 material_size,
				const Uint32 material_index,
				const StringType &dir);
			void load_materials(
				const FileSystemSharedPtr &file_system,
				const Uint32 material_count,
				const Uint32 material_size,
				const Uint32 material_offset,
				MaterialDescriptionVector &materials);
			void log_file_infos(const Uint8 options,
				const Uint8 format) const;
			void check_file_infos(Uint8 &options, Uint8 &format)
				const;
			void check_vertex_size(const Uint8 options,
				const Uint8 format, const Uint32 vertex_size)
				const;
			void check_index_size(const Uint32 index_size) const;

		public:
			E3dLoader(const ReaderSharedPtr &reader);
			~E3dLoader() throw();
			void load(const FileSystemSharedPtr &file_system,
				const bool use_simd,
				MeshDataToolSharedPtr &mesh_data_tool,
				MaterialDescriptionVector &materials);
			static bool check_format(const Uint8Array8 &id);

	};

}

#endif	/* UUID_73016a6d_d28a_4164_ae25_a1e2f9944c6a */

