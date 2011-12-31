/****************************************************************************
 *            texturefontcache.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "texturefontcache.hpp"
#include "image.hpp"
#include "atlas.hpp"
#include "texture.hpp"
#include "texturefont.hpp"
#include "meshbuilder.hpp"
#include "abstractmesh.hpp"
#include "submesh.hpp"
#include "vertexbuffers.hpp"
#include "statemanager.hpp"
#include "shader/shadertextureutil.hpp"
#include "glslprogram.hpp"
#include "xmlutil.hpp"
#include "xmlreader.hpp"
#include "filesystem.hpp"
#include "logging.hpp"

namespace eternal_lands
{

	namespace
	{

		const StringType vertex_shader = UTF8(""
			"#version 120\n"
			"\n"
			"varying vec2 uv;\n"
			"varying vec4 vertex_color;\n"
			"\n"
			"uniform mat4x3 world_matrix;\n"
			"uniform mat4 projection_view_matrix;\n"
			"\n"
			"attribute vec2 texture_coordinate_0;\n"
			"attribute vec3 position;\n"
			"attribute vec4 color;\n"
			"\n"
			"void main ()\n"
			"{\n"
			"\tvec3 world_position;\n"
			"\n"
			"\tworld_position = world_matrix * "
				"vec4(position.x * 0.01, 0.0, "
				"position.y * 0.01, 1.0);\n"
			"\tgl_Position = projection_view_matrix * "
				"vec4(world_position, 1.0);\n"
			"\tuv = texture_coordinate_0;\n"
			"\tvertex_color = color;\n"
			"}\n");

		const StringType fragment_shader = UTF8(""
			"#version 120\n"
			"\n"
			"varying vec2 uv;\n"
			"varying vec4 vertex_color;\n"
			"\n"
			"uniform sampler2D diffuse_sampler_0;\n"
			"\n"
			"void main ()\n"
			"{\n"
			"\tgl_FragColor = vertex_color;\n"
			"\tgl_FragColor *= texture2D(diffuse_sampler_0, uv).r;\n"
			"}\n");

	}

	TextureFontCache::TextureFontCache(
		const MeshBuilderSharedPtr &mesh_builder, const Uint16 width,
		const Uint16 height, const Uint16 max_char_count)
	{
		StringVariantMap values;
		Uint32Array3 sizes;
		SubMeshVector sub_meshs;
		Uint32Vector indices;
		Uint32 i;
		bool use_16_bit_indices;

		sizes[0] = width;
		sizes[1] = height;
		sizes[2] = 1;

		m_image = boost::make_shared<Image>(String(UTF8("Fonts")),
			false, tft_rgb8, sizes, 0);

		m_atlas = boost::make_shared<Atlas>(width, height);

		m_texture = boost::make_shared<Texture>(String(UTF8("Fonts")));

		m_mesh = mesh_builder->get_mesh();
		m_mesh->init_vertex(mesh_builder->get_vertex_format(vft_font),
			4 * max_char_count, false);

		for (i = 0; i < max_char_count; ++i)
		{
			indices.push_back(i * 4 + 0);
			indices.push_back(i * 4 + 1);
			indices.push_back(i * 4 + 2);

			indices.push_back(i * 4 + 2);
			indices.push_back(i * 4 + 3);
			indices.push_back(i * 4 + 0);
		}

		use_16_bit_indices = (4 * max_char_count) <
			std::numeric_limits<Uint16>::max();

		m_mesh->init_indices(use_16_bit_indices, pt_triangles,
			sub_meshs, indices);

		m_buffers = m_mesh->get_vertex_buffers();

		m_program = boost::make_shared<GlslProgram>(vertex_shader,
			fragment_shader, values, String(UTF8("Font")));
	}

	TextureFontCache::~TextureFontCache() throw()
	{
	}

	void TextureFontCache::load_font(const FileSystemSharedPtr &file_system,
		const xmlNodePtr node)
	{
		xmlNodePtr it;
		String name, file_name;
		float size;

		if (xmlStrcmp(node->name, BAD_CAST UTF8("font")) != 0)
		{
			return;
		}

		if (!XmlUtil::has_children(node, true))
		{
			return;
		}

		it = XmlUtil::children(node, true);

		size = 12.0f;

		do
		{
			if (xmlStrcmp(it->name, BAD_CAST UTF8("name")) == 0)
			{
				name = XmlUtil::get_string_value(it);
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("file_name"))
				== 0)
			{
				file_name = XmlUtil::get_string_value(it);
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("size")) == 0)
			{
				size = XmlUtil::get_float_value(it);
			}
		}
		while (XmlUtil::next(it, true));

		add_font(file_system, name, file_name, size);
	}

	void TextureFontCache::load_xml(const FileSystemSharedPtr &file_system,
		const xmlNodePtr node)
	{
		xmlNodePtr it;

		if (xmlStrcmp(node->name, BAD_CAST UTF8("fonts")) != 0)
		{
			return;
		}

		if (!XmlUtil::has_children(node, true))
		{
			return;
		}

		it = XmlUtil::children(node, true);

		do
		{
			if (xmlStrcmp(it->name, BAD_CAST UTF8("font")) == 0)
			{
				load_font(file_system, it);
			}
		}
		while (XmlUtil::next(it, true));
	}

	void TextureFontCache::load_xml(const FileSystemSharedPtr &file_system,
		const String &file_name)
	{
		XmlReaderSharedPtr reader;

		if (!file_system->get_file_readable(file_name))
		{
			return;
		}

		reader = XmlReaderSharedPtr(new XmlReader(file_system,
			file_name));

		load_xml(file_system, reader->get_root_node());
	}

	void TextureFontCache::add_font(const FileSystemSharedPtr &file_system,
		const String &name, const String &file_name, const float size)
	{
		std::auto_ptr<TextureFont> font;

		try
		{
			font.reset(new TextureFont(m_atlas, m_image,
				file_system, file_name, size));

			m_fonts.insert(name, font);

			update_texture();
		}
		catch (const boost::exception &exception)
		{
			LOG_EXCEPTION(exception);
		}
	}

	void TextureFontCache::update_texture()
	{
		m_texture->set_format(m_image->get_texture_format());
		m_texture->set_image(m_image);
	}

	void TextureFontCache::draw(StateManager &state_manager,
		const Utf32String &str, const String &name,
		const glm::vec2 &position, const glm::vec4 &color,
		const Uint32 max_lines, const float spacing, const float rise)
		const
	{
		MeshDrawData draw_data;
		StringTextureFontMap::const_iterator found;
		Uint32 count;

		found = m_fonts.find(name);

		assert(m_fonts.begin() != m_fonts.end());

		assert(found != m_fonts.end());

		if (found == m_fonts.end())
		{
			return;
		}

		m_buffers->reset();

		count = found->second->write_to_stream(str, m_buffers,
			position, color, max_lines, spacing, rise);

		if (count == 0)
		{
			return;
		}

		draw_data = MeshDrawData(0, count * 6, 0, count * 4 - 1);

		state_manager.switch_mesh(m_mesh);
		m_mesh->update_vertex(m_buffers);

		state_manager.switch_texture(stt_diffuse_0, m_texture);

		m_mesh->draw(draw_data);
	}

}
