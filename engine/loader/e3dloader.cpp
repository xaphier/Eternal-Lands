/****************************************************************************
 *            e3dloader.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "e3dloader.hpp"
#include "reader.hpp"
#include "meshdatatool.hpp"
#include "submesh.hpp"
#include "materialdescription.hpp"
#include "exceptions.hpp"
#include "packtool.hpp"
#include "logging.hpp"
#include "materialdescriptioncache.hpp"
#include "filesystem.hpp"

namespace eternal_lands
{

	namespace
	{
			Uint8Array4 e3d_magic_number = { {'e', '3', 'd', 'x'} };
			Uint8Array4 e3d_version_number = { {1, 1, 0, 0} };

			inline bool get_normal(const Uint32 options)
			{
				return (options & 0x01) != 0;
			}

			inline bool get_tangent(const Uint32 options)
			{
				return (options & 0x02) != 0;
			}

			inline bool get_secondary_texture_coordinate(
				const Uint32 options)
			{
				return (options & 0x04) != 0;
			}

			inline bool get_color(const Uint32 options)
			{
				return (options & 0x08) != 0;
			}

			inline bool get_half_position(const Uint32 format)
			{
				return (format & 0x01) != 0;
			}

			inline bool get_half_uv(const Uint32 format)
			{
				return (format & 0x02) != 0;
			}

			inline bool get_half_extra_uv(const Uint32 format)
			{
				return (format & 0x04) != 0;
			}

			inline bool get_compressed_normalized_vector(
				const Uint32 format)
			{
				return (format & 0x08) != 0;
			}

			inline bool get_short_index(const Uint32 format)
			{
				return (format & 0x10) != 0;
			}

			inline VertexSemanticTypeSet get_semantics(
				const Uint8 options)
			{
				VertexSemanticTypeSet result;

				result.insert(vst_position);
				result.insert(vst_texture_coordinate);
				result.insert(vst_normal);
				result.insert(vst_tangent);

				if (get_color(options))
				{
					result.insert(vst_color);
				}

				return result;
			}

	}

	bool E3dLoader::check_format(const Uint8Array8 &id)
	{
		Uint32 i;

		for (i = 0; i < 4; ++i)
		{
			if (id[i] != e3d_magic_number[i])
			{
				return false;
			}
		}

		return true;
	}

	void E3dLoader::load_header()
	{
		Uint8Array4 version_number;
		Uint8Array4 magic_number;
		Uint32 position;

		magic_number[0] = m_reader->read_u8();
		magic_number[1] = m_reader->read_u8();
		magic_number[2] = m_reader->read_u8();
		magic_number[3] = m_reader->read_u8();

		version_number[0] = m_reader->read_s8();
		version_number[1] = m_reader->read_s8();
		version_number[2] = m_reader->read_s8();
		version_number[3] = m_reader->read_s8();

		if (magic_number != e3d_magic_number)
		{
			EL_THROW_EXCEPTION(IoErrorException()
				<< boost::errinfo_file_name(
					m_reader->get_name()));
		}

		if (version_number != e3d_version_number)
		{
			EL_THROW_EXCEPTION(IoErrorException()
				<< boost::errinfo_file_name(
					m_reader->get_name()));
		}

		m_reader->skip(16);

		position = m_reader->read_u32_le();

		m_reader->set_position(position);
	}

	void E3dLoader::load(const bool use_simd,
		MeshDataToolSharedPtr &mesh_data_tool, StringVector &materials)
	{
		VertexSemanticTypeSet semantics;
		Uint32 vertex_count, vertex_size, vertex_offset;
		Uint32 index_count, index_size, index_offset;
		Uint32 material_count, material_size, material_offset;
		Uint8 options, format;

		load_header();

		vertex_count = m_reader->read_u32_le();
		vertex_size = m_reader->read_u32_le();
		vertex_offset = m_reader->read_u32_le();

		index_count = m_reader->read_u32_le();
		index_size = m_reader->read_u32_le();
		index_offset = m_reader->read_u32_le();

		material_count = m_reader->read_u32_le();
		material_size = m_reader->read_u32_le();
		material_offset = m_reader->read_u32_le();

		options = m_reader->read_u8();
		format = m_reader->read_u8();

		check_file_infos(options, format);
		log_file_infos(options, format);
		check_vertex_size(options, format, vertex_size);
		check_index_size(index_size);

		semantics = get_semantics(options);

		mesh_data_tool = boost::make_shared<MeshDataTool>(
			m_reader->get_name(), vertex_count, index_count,
			material_count, semantics, pt_triangles, false,
			use_simd);

		load_vertex(mesh_data_tool, options, format, vertex_count,
			vertex_size, vertex_offset);

		load_index(mesh_data_tool, index_count, index_size,
			index_offset);

		load_sub_meshs(mesh_data_tool, material_count, material_size,
			material_offset);

		if (!get_normal(options))
		{
			mesh_data_tool->build_normal(false);
		}

		mesh_data_tool->build_tangent(false, true);

		materials.clear();
		load_materials(material_count, material_size, material_offset,
			materials);
	}

	void E3dLoader::load_vertex(const MeshDataToolSharedPtr &mesh_data_tool,
		const Uint32 options, const Uint32 format,
		const Uint32 vertex_count, const Uint32 vertex_size,
		const Uint32 vertex_offset)
	{
		glm::vec4 position, uv, normal, tangent, color;
		Uint32 i;

		position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		normal = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
		tangent = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
		uv = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

		for (i = 0; i < vertex_count; ++i)
		{
			m_reader->set_position(vertex_offset + i * vertex_size);

			if (get_half_uv(format))
			{
				uv[0] = glm::detail::toFloat32(
					m_reader->read_u16_le());
				uv[1] = glm::detail::toFloat32(
					m_reader->read_u16_le());
			}
			else
			{
				uv[0] = m_reader->read_float_le();
				uv[1] = m_reader->read_float_le();
			}

			if (get_secondary_texture_coordinate(options))
			{
				if (get_half_extra_uv(format))
				{
					uv[2] = glm::detail::toFloat32(
						m_reader->read_u16_le());
					uv[3] = glm::detail::toFloat32(
						m_reader->read_u16_le());
				}
				else
				{
					uv[2] = m_reader->read_float_le();
					uv[3] = m_reader->read_float_le();
				}
			}

			if (get_normal(options))
			{
				if (get_compressed_normalized_vector(format))
				{
					normal = glm::vec4(
						PackTool::uncompress_normalized(
						m_reader->read_u16_le()), 1.0f);
				}
				else
				{
					normal[0] = m_reader->read_float_le();
					normal[1] = m_reader->read_float_le();
					normal[2] = m_reader->read_float_le();
				}
			}

			if (get_tangent(options))
			{
				if (get_compressed_normalized_vector(format))
				{
					tangent = glm::vec4(
						PackTool::uncompress_normalized(
						m_reader->read_u16_le()), 1.0f);
				}
				else
				{
					tangent[0] = m_reader->read_float_le();
					tangent[1] = m_reader->read_float_le();
					tangent[2] = m_reader->read_float_le();
				}
			}

			if (get_half_position(format))
			{
				position[0] = glm::detail::toFloat32(
					m_reader->read_u16_le());
				position[1] = glm::detail::toFloat32(
					m_reader->read_u16_le());
				position[2] = glm::detail::toFloat32(
					m_reader->read_u16_le());
			}
			else
			{
				position[0] = m_reader->read_float_le();
				position[1] = m_reader->read_float_le();
				position[2] = m_reader->read_float_le();
			}

			if (get_color(options))
			{
				color[0] = m_reader->read_u8() / 255.0f;
				color[1] = m_reader->read_u8() / 255.0f;
				color[2] = m_reader->read_u8() / 255.0f;
				color[3] = m_reader->read_u8() / 255.0f;
			}
			else
			{
				color[0] = 1.0f;
				color[1] = 1.0f;
				color[2] = 1.0f;
				color[3] = 1.0f;
			}

			mesh_data_tool->set_vertex_data(
				vst_texture_coordinate, i, uv);
			mesh_data_tool->set_vertex_data(vst_color, i, color);
			mesh_data_tool->set_vertex_data(vst_normal, i, normal);
			mesh_data_tool->set_vertex_data(vst_tangent, i,
				tangent);
			mesh_data_tool->set_vertex_data(vst_position, i,
				position);
		}
	}

	void E3dLoader::load_index(const MeshDataToolSharedPtr &mesh_data_tool,
		const Uint32 index_count, const Uint32 index_size,
		const Uint32 index_offset)
	{
		Uint32 i, index;

		m_reader->set_position(index_offset);

		for (i = 0; i < index_count; ++i)
		{
			if (index_size == sizeof(Uint16))
			{
				index = m_reader->read_u16_le();
			}
			else
			{
				index = m_reader->read_u32_le();
			}

			mesh_data_tool->set_index_data(i, index);
		}
	}

	SubMesh E3dLoader::load_sub_mesh(const Uint32 material_offset,
		const Uint32 material_size, const Uint32 material_index)
	{
		BoundingBox box;
		glm::vec3 vmin, vmax;
		Uint32 min_index, max_index, offset, count;

		m_reader->set_position(material_offset + material_size *
			material_index);

		m_reader->skip(sizeof(Uint32));
		m_reader->skip(128);

		vmin[0] = m_reader->read_float_le();
		vmin[1] = m_reader->read_float_le();
		vmin[2] = m_reader->read_float_le();

		vmax[0] = m_reader->read_float_le();
		vmax[1] = m_reader->read_float_le();
		vmax[2] = m_reader->read_float_le();

		box.set_min_max(vmin, vmax);
		box.set_half_size(glm::max(box.get_half_size(),
			glm::vec3(0.01f)));

		min_index = m_reader->read_u32_le();
		max_index = m_reader->read_u32_le();
		offset = m_reader->read_u32_le();
		count = m_reader->read_u32_le();

		if ((static_cast<Uint64>(min_index) + 1) >= max_index)
		{
			EL_THROW_EXCEPTION(IoErrorException()
				<< boost::errinfo_file_name(
					m_reader->get_name()));
		}

		return SubMesh(box, offset, count, min_index, max_index);
	}

	void E3dLoader::load_sub_meshs(
		const MeshDataToolSharedPtr &mesh_data_tool,
		const Uint32 material_count, const Uint32 material_size,
		const Uint32 material_offset)
	{
		Uint32 i;

		for (i = 0; i < material_count; ++i)
		{
			mesh_data_tool->set_sub_mesh_data(i, load_sub_mesh(
				material_offset, material_size, i));
		}
	}

	String E3dLoader::load_material(
		const Uint32 material_offset, const Uint32 material_size,
		const Uint32 material_index, const StringType &dir)
	{
		StringType file_name;
		String name;
		Uint32 options;

		m_reader->set_position(material_offset + material_size *
			material_index);

		options = m_reader->read_u32_le();

		file_name = m_reader->read_utf8_string(128);

		return FileSystem::get_file_name_without_extension(
			String(file_name));
	}

	void E3dLoader::load_materials(const Uint32 material_count,
		const Uint32 material_size, const Uint32 material_offset,
		StringVector &materials)
	{
		StringType str;
		Uint32 i;

		str = FileSystem::get_dir_name(m_reader->get_name());

		if (str.length() > 0)
		{
			str += UTF8("/");
		}

		for (i = 0; i < material_count; ++i)
		{
			materials.push_back(load_material(material_offset,
				material_size, i, str));
		}
	}

	E3dLoader::E3dLoader(const ReaderSharedPtr &reader): m_reader(reader)
	{
		LOG_DEBUG(lt_e3d_loader, UTF8("Loading file '%1%'."),
			m_reader->get_name());
	}

	E3dLoader::~E3dLoader() noexcept
	{
	}

	void E3dLoader::check_file_infos(Uint8 &options, Uint8 &format) const
	{
		if ((options & 0xF0) != 0)
		{
		}

		options &= 0x0F;

		if ((format & 0xE0) != 0)
		{
		}

		format &= 0x1F;
	}

	void E3dLoader::log_file_infos(const Uint8 options,
		const Uint8 format) const
	{
		StringType str;

		if (get_normal(options))
		{
			if (!str.empty())
			{
				str += UTF8(", ");
			}
			str += UTF8("normal");
		}

		if (get_tangent(options))
		{
			if (!str.empty())
			{
				str += UTF8(", ");
			}
			str += UTF8("tangent");
		}

		if (get_secondary_texture_coordinate(options))
		{
			if (!str.empty())
			{
				str += UTF8(", ");
			}
			str += UTF8("secondary_texture_coordinate");
		}

		if (get_color(options))
		{
			if (!str.empty())
			{
				str += UTF8(", ");
			}
			str += UTF8("color");
		}

		if (get_half_position(format))
		{
			if (!str.empty())
			{
				str += UTF8(", ");
			}
			str += UTF8("half_position");
		}

		if (get_half_uv(format))
		{
			if (!str.empty())
			{
				str += UTF8(", ");
			}
			str += UTF8("half_uv");
		}

		if (get_half_extra_uv(format))
		{
			if (!str.empty())
			{
				str += UTF8(", ");
			}
			str += UTF8("half_extra_uv");
		}

		if (get_compressed_normalized_vector(format))
		{
			if (!str.empty())
			{
				str += UTF8(", ");
			}
			str += UTF8("compressed_normalized_vector");
		}

		if (get_short_index(format))
		{
			if (!str.empty())
			{
				str += UTF8(", ");
			}
			str += UTF8("short_index");
		}

		LOG_DEBUG(lt_e3d_loader, UTF8("Formats [%1%] for file '%2%'."),
			str % m_reader->get_name());
	}

	void E3dLoader::check_vertex_size(const Uint8 options,
		const Uint8 format, const Uint32 vertex_size) const
	{
		Uint32 size;

		if (get_half_uv(format))
		{
			size = 2 * sizeof(Uint16);
		}
		else
		{
			size = 2 * sizeof(float);
		}

		if (get_normal(options))
		{
			if (get_compressed_normalized_vector(format))
			{
				size += sizeof(Uint16);
			}
			else
			{
				size += 3 * sizeof(float);
			}
		}

		if (get_tangent(options))
		{
			if (get_compressed_normalized_vector(format))
			{
				size += sizeof(Uint16);
			}
			else
			{
				size += 3 * sizeof(float);
			}
		}

		if (get_half_position(format))
		{
			size += 3 * sizeof(Uint16);
		}
		else
		{
			size += 3 * sizeof(float);
		}

		if (get_secondary_texture_coordinate(options))
		{
			if (get_half_extra_uv(format))
			{
				size += 2 * sizeof(Uint16);
			}
			else
			{
				size += 2 * sizeof(float);
			}
		}

		if (get_color(options))
		{
			size += 4 * sizeof(Uint8);
		}

		if (vertex_size != size)
		{
			EL_THROW_EXCEPTION(IoErrorException()
				<< errinfo_value(vertex_size)
				<< boost::errinfo_file_name(
					m_reader->get_name()));
		}
	}

	void E3dLoader::check_index_size(const Uint32 index_size) const
	{
		if ((index_size != sizeof(Uint16)) &&
			(index_size != sizeof(Uint32)))
		{
			EL_THROW_EXCEPTION(IoErrorException()
				<< errinfo_value(index_size)
				<< boost::errinfo_file_name(
					m_reader->get_name()));
		}
	}

}
