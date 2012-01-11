/****************************************************************************
 *            meshdatacache.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "meshdatacache.hpp"
#include "filesystem.hpp"
#include "loader/e2dloader.hpp"
#include "loader/e3dloader.hpp"
#include "reader.hpp"
#include "meshdatatool.hpp"
#include "materialdescription.hpp"
#include "logging.hpp"
#include "indexbuilder.hpp"
#include "submesh.hpp"
#include "globalvars.hpp"
#include "exceptions.hpp"

namespace eternal_lands
{

	namespace
	{

		const Uint16 sphere_index_count = 240;
		const Uint16 sphere_vertex_count = 42;

		const glm::vec3 sphere_vertices[sphere_vertex_count] =
		{
			glm::vec3( 0.850651f,  0.525731f,  0.000000f),
			glm::vec3(-0.850651f,  0.525731f,  0.000000f),
			glm::vec3(-0.850651f, -0.525731f,  0.000000f),
			glm::vec3( 0.850651f, -0.525731f,  0.000000f),
			glm::vec3( 0.525731f,  0.000000f,  0.850651f),
			glm::vec3( 0.525731f,  0.000000f, -0.850651f),
			glm::vec3(-0.525731f,  0.000000f, -0.850651f),
			glm::vec3(-0.525731f,  0.000000f,  0.850651f),
			glm::vec3( 0.000000f,  0.850651f,  0.525731f),
			glm::vec3( 0.000000f, -0.850651f,  0.525731f),
			glm::vec3( 0.000000f, -0.850651f, -0.525731f),
			glm::vec3( 0.000000f,  0.850651f, -0.525731f),
			glm::vec3( 0.309017f,  0.500000f,  0.809017f),
			glm::vec3(-0.309017f,  0.500000f,  0.809017f),
			glm::vec3( 0.000000f,  0.000000f,  1.000000f),
			glm::vec3(-0.309017f, -0.500000f,  0.809017f),
			glm::vec3( 0.309017f, -0.500000f,  0.809017f),
			glm::vec3( 0.000000f,  0.000000f, -1.000000f),
			glm::vec3(-0.309017f,  0.500000f, -0.809017f),
			glm::vec3( 0.309017f,  0.500000f, -0.809017f),
			glm::vec3( 0.309017f, -0.500000f, -0.809017f),
			glm::vec3(-0.309017f, -0.500000f, -0.809017f),
			glm::vec3( 0.809017f,  0.309017f,  0.500000f),
			glm::vec3( 0.809017f, -0.309017f,  0.500000f),
			glm::vec3( 1.000000f,  0.000000f,  0.000000f),
			glm::vec3( 0.809017f, -0.309017f, -0.500000f),
			glm::vec3( 0.809017f,  0.309017f, -0.500000f),
			glm::vec3(-0.809017f, -0.309017f,  0.500000f),
			glm::vec3(-0.809017f,  0.309017f,  0.500000f),
			glm::vec3(-1.000000f,  0.000000f,  0.000000f),
			glm::vec3(-0.809017f,  0.309017f, -0.500000f),
			glm::vec3(-0.809017f, -0.309017f, -0.500000f),
			glm::vec3( 0.500000f,  0.809017f,  0.309017f),
			glm::vec3( 0.500000f,  0.809017f, -0.309017f),
			glm::vec3( 0.000000f,  1.000000f,  0.000000f),
			glm::vec3(-0.500000f,  0.809017f, -0.309017f),
			glm::vec3(-0.500000f,  0.809017f,  0.309017f),
			glm::vec3( 0.000000f, -1.000000f,  0.000000f),
			glm::vec3( 0.500000f, -0.809017f, -0.309017f),
			glm::vec3( 0.500000f, -0.809017f,  0.309017f),
			glm::vec3(-0.500000f, -0.809017f,  0.309017f),
			glm::vec3(-0.500000f, -0.809017f, -0.309017f)
		};

		const Uint16 sphere_indices[sphere_index_count] =
		{
			 4, 12, 14, 14, 12, 13, 14, 13,  7, 12,  8, 13,  4, 14,
			16, 16, 14, 15, 16, 15,  9, 14,  7, 15,  5, 17, 19, 19,
			17, 18, 19, 18, 11, 17,  6, 18,  5, 20, 17, 17, 20, 21,
			17, 21,  6, 20, 10, 21,  0, 22, 24, 24, 22, 23,	24, 23,
			 3, 22,  4, 23,  0, 24, 26, 26, 24, 25, 26, 25,  5, 24,
			 3, 25,  2, 27, 29, 29, 27, 28, 29, 28,  1, 27,  7, 28,
			 2, 29, 31, 31, 29, 30, 31, 30,  6, 29,  1, 30,  8, 32,
			34, 34, 32, 33, 34, 33, 11, 32,  0, 33,  8, 34, 36, 36,
			34, 35, 36, 35,  1, 34, 11, 35,  9, 37, 39, 39, 37, 38,
			39, 38,  3, 37, 10, 38,  9, 40, 37, 37, 40, 41, 37, 41,
			10, 40,  2, 41,  8, 12, 32, 32, 12, 22, 32, 22,  0, 12,
			 4, 22, 11, 33, 19, 19, 33, 26, 19, 26,  5, 33,  0, 26,
			 4, 16, 23, 23, 16, 39, 23, 39,  3, 16,  9, 39,  5, 25,
			20, 20, 25, 38, 20, 38, 10, 25,  3, 38,  7, 13, 28, 28,
			13, 36, 28, 36,  1, 13,  8, 36,  6, 30, 18, 18, 30, 35,
			18, 35, 11, 30,  1, 35,  7, 27, 15, 15, 27, 40, 15, 40,
			 9, 27,  2, 40,  6, 21, 31, 31, 21, 41, 31, 41,  2, 21,
			10, 41
		};

		void load_error_sphere(const bool use_simd,
			MeshDataToolSharedPtr &mesh_data_tool)
		{
			VertexSemanticTypeSet semantics;
			glm::vec3 position, normal;
			glm::vec2 uv;
			Uint32 i;

			semantics.insert(vst_position);
			semantics.insert(vst_texture_coordinate_0);
			semantics.insert(vst_normal);
			semantics.insert(vst_color);

			mesh_data_tool.reset(new MeshDataTool(
				String(UTF8("Sphere")), sphere_vertex_count,
				sphere_index_count, 1, semantics,
				std::numeric_limits<Uint32>::max(),
				pt_triangles, false, use_simd));

			for (i = 0; i < sphere_vertex_count; ++i)
			{
				position = sphere_vertices[i];

				normal = glm::normalize(position);

				uv.x = position.x / glm::length(position);
				uv.y = position.y / glm::length(position);

				uv = uv * 0.5f + 0.5f;

				mesh_data_tool->set_vertex_data(vst_position, i,
					glm::vec4(position, 1.0f));
				mesh_data_tool->set_vertex_data(vst_normal, i,
					glm::vec4(normal, 1.0f));
				mesh_data_tool->set_vertex_data(
					vst_texture_coordinate_0, i,
					glm::vec4(uv, 0.0f, 1.0f));
				mesh_data_tool->set_vertex_data(vst_color, i,
					glm::vec4(1.0f));
			}

			for (i = 0; i < sphere_index_count; ++i)
			{
				mesh_data_tool->set_index_data(i,
					sphere_indices[i]);
			}

			mesh_data_tool->set_sub_mesh_data(0, SubMesh(
				BoundingBox(glm::vec3(-1.0f), glm::vec3(1.0f)),
				0, sphere_index_count, 0,
				sphere_vertex_count - 1));
		}

		void load_plane(const String &name, const Uint16 tile_size,
			const bool split, const bool use_simd,
			MeshDataToolSharedPtr &mesh_data_tool)
		{
			glm::vec4 normal, tangent, data;
			glm::vec3 vmin, vmax;
			glm::vec2 size, uv;
			Uint32Vector indices;
			String dir, texture;
			VertexSemanticTypeSet semantics;
			StringType str;
			Uint32 vertex_count, index_count, i, index, x, y;
			Uint32 restart_index;
			PrimitiveType primitive_type;
			bool use_restart_index;

			vertex_count = tile_size + 1;
			vertex_count *= tile_size + 1;

			use_restart_index = false;//GLEW_VERSION_3_1;

			if (use_restart_index)
			{
				primitive_type = pt_triangle_fan;
			}
			else
			{
				primitive_type = pt_triangles;
			}

			restart_index = IndexBuilder::build_plane_indices(
				indices, tile_size, use_restart_index, 0, split,
				0);

			index_count = indices.size();

			semantics.insert(vst_position);
			semantics.insert(vst_texture_coordinate_0);
			semantics.insert(vst_normal);
			semantics.insert(vst_tangent);

			mesh_data_tool = boost::make_shared<MeshDataTool>(name,
				vertex_count, index_count, 1, semantics,
				restart_index, primitive_type,
				use_restart_index, use_simd);

			for (i = 0; i < index_count; ++i)
			{
				mesh_data_tool->set_index_data(i, indices[i]);
			}

			normal = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
			tangent = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

			index = 0;

			for (y = 0; y <= tile_size; ++y)
			{
				for (x = 0; x <= tile_size; ++x)
				{
					uv = glm::vec2(x, y) /
						static_cast<float>(tile_size);

					data = glm::vec4(uv, 0.0f, 1.0f);

					mesh_data_tool->set_vertex_data(
						vst_position, index, data);
					mesh_data_tool->set_vertex_data(
						vst_normal, index, normal);
					mesh_data_tool->set_vertex_data(
						vst_tangent, index, tangent);
					mesh_data_tool->set_vertex_data(
						vst_texture_coordinate_0,
						index, data);

					++index;
				}
			}

			vmin = glm::vec3(0.0f, 0.0f, -0.01f);
			vmax = glm::vec3(1.0f, 1.0f, 0.0f);

			mesh_data_tool->set_sub_mesh_data(0, SubMesh(
				BoundingBox(vmin, vmax), 0, index_count, 0,
				vertex_count - 1));
		}

		void load_e2d(const ReaderSharedPtr &reader,
			const bool use_simd,
			MeshDataToolSharedPtr &mesh_data_tool,
			MaterialDescriptionVector &materials)
		{
			E2dLoader e2d_loader(reader);

			e2d_loader.load(use_simd, mesh_data_tool, materials);
		}

		void load_e3d(const FileSystemSharedPtr &file_system,
			const ReaderSharedPtr &reader,
			const bool use_simd,
			MeshDataToolSharedPtr &mesh_data_tool,
			MaterialDescriptionVector &materials)
		{
			E3dLoader e3d_loader(reader);

			e3d_loader.load(file_system, use_simd, mesh_data_tool,
				materials);

			mesh_data_tool->update_sub_meshs_packed();
		}

		void do_load_mesh(const FileSystemSharedPtr &file_system,
			const ReaderSharedPtr &reader, const bool use_simd,
			MeshDataToolSharedPtr &mesh_data_tool,
			MaterialDescriptionVector &materials)
		{
			Uint8Array8 id;
			Uint32 i;

			for (i = 0; i < id.size(); ++i)
			{
				id[i] = reader->read_u8();
			}

			reader->set_position(0);

			if (E3dLoader::check_format(id))
			{
				load_e3d(file_system, reader, use_simd,
					mesh_data_tool, materials);

				assert(mesh_data_tool.get() != 0);
				return;
			}

			if (E2dLoader::check_format(id))
			{
				load_e2d(reader, use_simd, mesh_data_tool,
					materials);

				assert(mesh_data_tool.get() != 0);
				return;
			}

			EL_THROW_EXCEPTION(UnkownFormatException()
				<< boost::errinfo_file_name(
					reader->get_name()));
		}

		bool load_plane(const String &name, const bool use_simd,
			MeshDataToolSharedPtr &mesh_data_tool)
		{
			if (name == UTF8("plane_2"))
			{
				load_plane(name, 2, true, use_simd,
					mesh_data_tool);
				return true;
			}

			if (name == UTF8("plane_4"))
			{
				load_plane(name, 4, true, use_simd,
					mesh_data_tool);
				return true;
			}

			if (name == UTF8("plane_8"))
			{
				load_plane(name, 8, true, use_simd,
					mesh_data_tool);
				return true;
			}

			if (name == UTF8("plane_16"))
			{
				load_plane(name, 16, true, use_simd,
					mesh_data_tool);
				return true;
			}

			if (name == UTF8("plane_32"))
			{
				load_plane(name, 32, true, use_simd,
					mesh_data_tool);
				return true;
			}

			if (name == UTF8("plane_64"))
			{
				load_plane(name, 64, true, use_simd,
					mesh_data_tool);
				return true;
			}

			if (name == UTF8("plane_128"))
			{
				load_plane(name, 128, true, use_simd,
					mesh_data_tool);
				return true;
			}

			return false;
		}

	}

	class MeshDataCache::MeshDataCacheItem
	{
		public:
			MeshDataToolSharedPtr m_mesh_data_tool;
			MaterialDescriptionVector m_materials;
	};

	MeshDataCache::MeshDataCache(
		const FileSystemWeakPtr &file_system,
		const GlobalVarsSharedPtr &global_vars):
		m_file_system(file_system), m_global_vars(global_vars)
	{
		assert(!m_file_system.expired());
		assert(m_global_vars.get() != 0);
	}

	MeshDataCache::~MeshDataCache() throw()
	{
	}

	void MeshDataCache::load_mesh(const String &name,
		MeshDataToolSharedPtr &mesh_data_tool,
		MaterialDescriptionVector &materials)
	{
		ReaderSharedPtr reader;

		try
		{
			if (load_plane(name, get_global_vars()->get_use_simd(),
				mesh_data_tool))
			{
				return;
			}

			reader = get_file_system()->get_file(name);

			do_load_mesh(get_file_system(), reader,
				get_global_vars()->get_use_simd(),
				mesh_data_tool, materials);

			mesh_data_tool->optimize();

			if (!get_global_vars()->get_opengl_3_1())
			{
				mesh_data_tool->disable_restart_index();
			}

			return;
		}
		catch (boost::exception &exception)
		{
			LOG_EXCEPTION(exception);
		}
		catch (std::exception &exception)
		{
			LOG_EXCEPTION(exception);
		}

		load_error_sphere(get_global_vars()->get_use_simd(),
			mesh_data_tool);
	}

	void MeshDataCache::get_mesh_data(const String &name,
		MeshDataToolSharedPtr &mesh_data_tool,
		MaterialDescriptionVector &materials)
	{
		MeshDataCache::MeshDataCacheItem tmp;
		MeshDataCacheMap::iterator found;

		found = m_mesh_data_cache.find(name);

		if (found == m_mesh_data_cache.end())
		{
			load_mesh(name, tmp.m_mesh_data_tool, tmp.m_materials);

			m_mesh_data_cache[name] = tmp;

			mesh_data_tool = tmp.m_mesh_data_tool;
			materials = tmp.m_materials;
		}
		else
		{
			mesh_data_tool = found->second.m_mesh_data_tool;
			materials = found->second.m_materials;
		}
	}

	void MeshDataCache::get_mesh_data(const String &name,
		MeshDataToolSharedPtr &mesh_data_tool)
	{
		MaterialDescriptionVector materials;

		get_mesh_data(name, mesh_data_tool, materials);
	}

}
