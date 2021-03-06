/****************************************************************************
 *            e2dloader.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "e2dloader.hpp"
#include "reader.hpp"
#include "meshdatatool.hpp"
#include "submesh.hpp"
#include "exceptions.hpp"
#include "logging.hpp"
#include "indexbuilder.hpp"
#include "filesystem.hpp"
#include "utf.hpp"

namespace eternal_lands
{

	class E2dLoadingException: public virtual InvalidParameterException,
		public virtual IoErrorException {};

	class E2dReadErrorException: public virtual ReadErrorException {};

	namespace
	{

		String parse_e2d(const ReaderSharedPtr &reader, glm::vec2 &size,
			glm::vec4 &texture_coordinates)
		{
			glm::vec2 scale;
			std::vector<std::string> lines, line;
			std::vector<std::string>::iterator it;
			String texture;
			String e2d_data;
			float file_x_len;
			float file_y_len;
			float x_size;
			float y_size;
			float u_start;
			float u_end;
			float v_start;
			float v_end;

			e2d_data = reader->read_utf8_string(reader->get_size());

			boost::split(lines, e2d_data.get(),
				boost::is_any_of(UTF8("\n")),
				boost::token_compress_on);

			it = lines.begin();

			while (it != lines.end())
			{
				boost::algorithm::trim(*it);

				if (it->empty())
				{
					it = lines.erase(it);
				}
				else
				{
					it++;
				}
			}

			if (lines.size() < 10)
			{
				EL_THROW_EXCEPTION(E2dReadErrorException()
					<< errinfo_string_value(e2d_data)
					<< boost::errinfo_at_line(lines.size()));
			}

			boost::split(line, lines[0],
				boost::is_any_of(UTF8(":")),
				boost::token_compress_on);

			if (line.size() == 2)
			{
				boost::algorithm::trim(line[0]);
				boost::algorithm::trim(line[1]);
			}
			else
			{
				EL_THROW_EXCEPTION(E2dReadErrorException()
					<< boost::errinfo_at_line(1));
			}

			if (line[0].find(UTF8("texture")) != std::string::npos)
			{
				texture = line[1];
			}
			else
			{
				EL_THROW_EXCEPTION(E2dReadErrorException()
					<< boost::errinfo_at_line(1));
			}

			boost::split(line, lines[1],
				boost::is_any_of(UTF8(":")),
				boost::token_compress_on);

			if (line.size() == 2)
			{
				boost::algorithm::trim(line[0]);
				boost::algorithm::trim(line[1]);
			}
			else
			{
				EL_THROW_EXCEPTION(E2dReadErrorException()
					<< boost::errinfo_at_line(2));
			}

			if (line[0].find(UTF8("file_x_len")) !=
				std::string::npos)
			{
				file_x_len = boost::lexical_cast<float>(
					line[1]);
			}
			else
			{
				EL_THROW_EXCEPTION(E2dReadErrorException()
					<< boost::errinfo_at_line(2));
			}

			boost::split(line, lines[2],
				boost::is_any_of(UTF8(":")),
				boost::token_compress_on);

			if (line.size() == 2)
			{
				boost::algorithm::trim(line[0]);
				boost::algorithm::trim(line[1]);
			}
			else
			{
				EL_THROW_EXCEPTION(E2dReadErrorException()
					<< boost::errinfo_at_line(3));
			}

			if (line[0].find(UTF8("file_y_len")) !=
				std::string::npos)
			{
				file_y_len = boost::lexical_cast<float>(
					line[1]);
			}
			else
			{
				EL_THROW_EXCEPTION(E2dReadErrorException()
					<< boost::errinfo_at_line(3));
			}

			boost::split(line, lines[3],
				boost::is_any_of(UTF8(":")),
				boost::token_compress_on);

			if (line.size() == 2)
			{
				boost::algorithm::trim(line[0]);
				boost::algorithm::trim(line[1]);
			}
			else
			{
				EL_THROW_EXCEPTION(E2dReadErrorException()
					<< boost::errinfo_at_line(4));
			}

			if (line[0].find(UTF8("x_size")) != std::string::npos)
			{
				x_size = boost::lexical_cast<float>(line[1]);
			}
			else
			{
				EL_THROW_EXCEPTION(E2dReadErrorException()
					<< boost::errinfo_at_line(4));
			}

			boost::split(line, lines[4],
				boost::is_any_of(UTF8(":")),
				boost::token_compress_on);

			if (line.size() == 2)
			{
				boost::algorithm::trim(line[0]);
				boost::algorithm::trim(line[1]);
			}
			else
			{
				EL_THROW_EXCEPTION(E2dReadErrorException()
					<< boost::errinfo_at_line(5));
			}

			if (line[0].find(UTF8("y_size")) != std::string::npos)
			{
				y_size = boost::lexical_cast<float>(line[1]);
			}
			else
			{
				EL_THROW_EXCEPTION(E2dReadErrorException()
					<< boost::errinfo_at_line(5));
			}

			boost::split(line, lines[5],
				boost::is_any_of(UTF8(":")),
				boost::token_compress_on);

			if (line.size() == 2)
			{
				boost::algorithm::trim(line[0]);
				boost::algorithm::trim(line[1]);
			}
			else
			{
				EL_THROW_EXCEPTION(E2dReadErrorException()
					<< boost::errinfo_at_line(6));
			}

			if (line[0].find(UTF8("u_start")) != std::string::npos)
			{
				u_start = boost::lexical_cast<float>(line[1]);
			}
			else
			{
				EL_THROW_EXCEPTION(E2dReadErrorException()
					<< boost::errinfo_at_line(6));
			}

			boost::split(line, lines[6],
				boost::is_any_of(UTF8(":")),
				boost::token_compress_on);

			if (line.size() == 2)
			{
				boost::algorithm::trim(line[0]);
				boost::algorithm::trim(line[1]);
			}
			else
			{
				EL_THROW_EXCEPTION(E2dReadErrorException()
					<< boost::errinfo_at_line(7));
			}

			if (line[0].find(UTF8("u_end")) != std::string::npos)
			{
				u_end = boost::lexical_cast<float>(line[1]);
			}
			else
			{
				EL_THROW_EXCEPTION(E2dReadErrorException()
					<< boost::errinfo_at_line(7));
			}

			boost::split(line, lines[7],
				boost::is_any_of(UTF8(":")),
				boost::token_compress_on);

			if (line.size() == 2)
			{
				boost::algorithm::trim(line[0]);
				boost::algorithm::trim(line[1]);
			}
			else
			{
				EL_THROW_EXCEPTION(E2dReadErrorException()
					<< boost::errinfo_at_line(8));
			}

			if (line[0].find(UTF8("v_start")) != std::string::npos)
			{
				v_start = boost::lexical_cast<float>(line[1]);
			}
			else
			{
				EL_THROW_EXCEPTION(E2dReadErrorException()
					<< boost::errinfo_at_line(8));
			}

			boost::split(line, lines[8],
				boost::is_any_of(UTF8(":")),
				boost::token_compress_on);

			if (line.size() == 2)
			{
				boost::algorithm::trim(line[0]);
				boost::algorithm::trim(line[1]);
			}
			else
			{
				EL_THROW_EXCEPTION(E2dReadErrorException()
					<< boost::errinfo_at_line(9));
			}

			if (line[0].find(UTF8("v_end")) != std::string::npos)
			{
				v_end = boost::lexical_cast<float>(line[1]);
			}
			else
			{
				EL_THROW_EXCEPTION(E2dReadErrorException()
					<< boost::errinfo_at_line(9));
			}

			boost::split(line, lines[9],
				boost::is_any_of(UTF8(":")),
				boost::token_compress_on);

			if (line.size() == 2)
			{
				boost::algorithm::trim(line[0]);
				boost::algorithm::trim(line[1]);
			}
			else
			{
				EL_THROW_EXCEPTION(E2dReadErrorException()
					<< boost::errinfo_at_line(10));
			}

			if (line[0].find(UTF8("type")) != std::string::npos)
			{
				if (line[1].find(UTF8("ground")) ==
					std::string::npos)
				{
					EL_THROW_EXCEPTION(
						E2dReadErrorException()
						<< boost::errinfo_at_line(10));
				}
			}
			else
			{
				EL_THROW_EXCEPTION(E2dReadErrorException()
					<< boost::errinfo_at_line(10));
			}

			scale = glm::vec2(1.0f) /
				glm::vec2(file_x_len, file_y_len);

			texture_coordinates[0] = u_start * scale[0];
			texture_coordinates[1] = 1.0f - v_start * scale[1];
			texture_coordinates[2] = u_end * scale[0];
			texture_coordinates[3] = 1.0f - v_end * scale[1];

			size = glm::vec2(x_size, y_size) * 0.5f;

			return String(texture);
		}

	}

	E2dLoader::E2dLoader(const ReaderSharedPtr &reader): m_reader(reader)
	{
		LOG_DEBUG(lt_e2d_loader, UTF8("Loading file '%1%'."),
			m_reader->get_name());
	}

	E2dLoader::~E2dLoader() noexcept
	{
	}

	void E2dLoader::load_vertex_data(
		const MeshDataToolSharedPtr &mesh_data_tool,
		const glm::vec4 &texture_coordinates, const glm::vec2 &size)
	{
		glm::vec4 normal, tangent, uv, position;
		glm::vec2 scale, t0, t1;
		Uint32 x, y, index;

		if (glm::any(glm::lessThanEqual(size, glm::vec2(0.0f))))
		{
			EL_THROW_EXCEPTION(E2dLoadingException()
				<< boost::errinfo_file_name(
					m_reader->get_name()));
		}

		normal = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
		tangent = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
		position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		uv = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

		index = 0;

		t0[0] = texture_coordinates[0];
		t0[1] = texture_coordinates[1];

		t1[0] = texture_coordinates[2];
		t1[1] = texture_coordinates[3];

		for (y = 0; y <= get_tile_size(); ++y)
		{
			for (x = 0; x <= get_tile_size(); ++x)
			{
				scale = glm::vec2(x, y) /
					static_cast<float>(get_tile_size());

				uv = glm::vec4(glm::mix(t1, t0, scale), 0.0f,
					1.0f);
				position = glm::vec4(
					glm::mix(size, -size, scale), 0.0f,
					1.0f);

				mesh_data_tool->set_vertex_data(vst_position,
					index, position);
				mesh_data_tool->set_vertex_data(vst_normal,
					index, normal);
				mesh_data_tool->set_vertex_data(vst_tangent,
					index, tangent);
				mesh_data_tool->set_vertex_data(
					vst_texture_coordinate, index, uv);

				index++;
			}
		}
	}

	void E2dLoader::load(const bool use_simd,
		MeshDataToolSharedPtr &mesh_data_tool, StringVector &materials)
	{
		glm::vec4 texture_coordinates;
		glm::vec3 vmin, vmax;
		glm::vec2 size;
		Uint32Vector indices;
		String texture, name;
		VertexSemanticTypeSet semantics;
		std::string str;
		Uint32 vertex_count, index_count, i;

		vertex_count = get_tile_size() + 1;
		vertex_count *= get_tile_size() + 1;

		IndexBuilder::build_plane_indices(get_tile_size(), false, 0,
			true, indices);

		index_count = indices.size();

		semantics.insert(vst_position);
		semantics.insert(vst_texture_coordinate);
		semantics.insert(vst_normal);
		semantics.insert(vst_tangent);

		try
		{
			texture = parse_e2d(m_reader, size,
				texture_coordinates);
		}
		catch (const boost::exception &exception)
		{
			exception << boost::errinfo_file_name(
				m_reader->get_name());
			throw;
		}

		mesh_data_tool = boost::make_shared<MeshDataTool>(
			m_reader->get_name(), vertex_count, index_count, 1,
			semantics, pt_triangles, false, use_simd);

		for (i = 0; i < index_count; ++i)
		{
			mesh_data_tool->set_index_data(i, indices[i]);
		}

		load_vertex_data(mesh_data_tool, texture_coordinates, size);

		vmin = glm::vec3(-size[0], -size[1], -0.01f);
		vmax = glm::vec3(size[0], size[1], 0.0f);

		mesh_data_tool->set_sub_mesh_data(0, SubMesh(BoundingBox(vmin,
			vmax), 0, index_count, 0, vertex_count - 1));

		name = FileSystem::get_file_name_without_extension(texture);

		materials.clear();
		materials.push_back(String(UTF8("ground/") + name.get()));
	}

	bool E2dLoader::check_format(const Uint8Array8 &id)
	{
		return (id[0] == 't') && (id[1] == 'e') && (id[2] == 'x') &&
			(id[3] == 't') && (id[4] == 'u') && (id[5] == 'r') &&
			(id[6] == 'e') && (id[7] == ':');
	}

}
