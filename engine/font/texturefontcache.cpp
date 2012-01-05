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
#include "edtaa3func.hpp"
#include "text.hpp"

namespace eternal_lands
{

	TextureFontCache::TextureFontCache(
		const FileSystemSharedPtr &file_system,
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
			false, tft_r16, sizes, 0);

		m_atlas = boost::make_shared<Atlas>(width, height);

		m_data.reset(new double[width * height]);

		memset(m_data.get(), 0, width * height * sizeof(double));

		m_texture = boost::make_shared<Texture>(String(UTF8("Fonts")));

		m_mesh = mesh_builder->get_mesh(String(UTF8("Fonts")));
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

		m_buffers = m_mesh->get_vertex_buffers(4 * max_char_count);

		m_program = boost::make_shared<GlslProgram>(file_system,
			String(UTF8("shaders/font.xml")));
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
			font.reset(new TextureFont(m_atlas, m_data,
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
		boost::scoped_array<short> xdist, ydist;
		boost::scoped_array<double> gx, gy, data, outside, inside;
		double min, max, value;
		Uint32 size, width, height, x, y, i, index;

		width = m_atlas->get_width();
		height = m_atlas->get_height();

		size = width * height;

		xdist.reset(new short[size]);
		ydist.reset(new short[size]);

		gx.reset(new double[size]);
		gy.reset(new double[size]);
		data.reset(new double[size]);
		outside.reset(new double[size]);
		inside.reset(new double[size]);

		// Convert img into double (data)
		min = 255.0;
		max = -255.0;

		for (i = 0; i < size; ++i)
		{
			min = std::min(min, m_data[i]);
			max = std::max(max, m_data[i]);
		}

		// Rescale image levels between 0 and 1
		for (i = 0; i < size; ++i)
		{
			data[i] = (m_data[i] - min) / (max - min);
		}

		// Compute outside = edtaa3(bitmap); % Transform background (0's)
		computegradient(data.get(), height, width, gx.get(),
			gy.get());
		edtaa3(data.get(), gx.get(), gy.get(), height, width,
			xdist.get(), ydist.get(), outside.get());

		for (i = 0; i < size; ++i)
		{
			outside[i] = std::max(0.0, outside[i]);
		}

		// Compute inside = edtaa3(1-bitmap); % Transform foreground (1's)
		memset(gx.get(), 0, size * sizeof(double));
		memset(gy.get(), 0, size * sizeof(double));

		for (i = 0; i < size; ++i)
		{
			data[i] = 1.0 - data[i];
		}

		computegradient(data.get(), height, width, gx.get(),
			gy.get());
		edtaa3(data.get(), gx.get(), gy.get(), height, width,
			xdist.get(), ydist.get(), inside.get());

		for (i = 0; i < size; ++i)
		{
			inside[i] = std::max(0.0, inside[i]);
		}

		for (y = 0; y < height; ++y)
		{
			for (x = 0; x < width; ++x)
			{
				index = x + y * width;

				value = 0.5 + (inside[index] - outside[index]) / 16.0;
				value = std::min(1.0, std::max(0.0, value));

				m_image->set_pixel(x, y, 0, 0, 0,
					glm::vec4(value));
			}
		}

		m_texture->set_format(m_image->get_texture_format());
		m_texture->set_image(m_image);
	}

	Uint32 TextureFontCache::draw(StateManager &state_manager,
		const Text &text, const glm::vec2 &position,
		const Uint32 max_lines, const float max_width)
	{
		Uint32 count, line;

		line = build_buffer(text, position, max_lines, max_width,
			m_buffers, count);

		m_mesh->update_vertex(m_buffers);

		draw(state_manager, m_mesh, count);

		return line;
	}

	Uint32 TextureFontCache::build_buffer(const Text &text,
		const glm::vec2 &position, const Uint32 max_lines,
		const float max_width, VertexBuffersSharedPtr &buffers,
		Uint32 &count) const
	{
		StringTextureFontMap::const_iterator found;
		glm::vec2 pos;
		Uint32 line;

		assert(m_fonts.begin() != m_fonts.end());

		count = text.get_length() * 4;

		if (count == 0)
		{
			return 0;
		}

		if (buffers.get() == 0)
		{
			buffers = m_mesh->get_vertex_buffers(count);
		}

		if (buffers->get_vertex_count() < count)
		{
			buffers = m_mesh->get_vertex_buffers(count);
		}

		buffers->reset();

		pos = position;
		count = 0;
		line = 0;

		BOOST_FOREACH(const Utf32StringTextAttributePair &data,
			text.get_text())
		{
			found = m_fonts.find(data.second.get_font());

			assert(found != m_fonts.end());

			if (found == m_fonts.end())
			{
				continue;
			}

			count += data.first.length();

			found->second->write_to_stream(data.first,
				data.second, buffers, position, pos, line,
				max_lines, max_width);
		}

		if (count == 0)
		{
			return 0;
		}

		return line;
	}

	Uint32 TextureFontCache::build_mesh(const Text &text,
		const glm::vec2 &position, const Uint32 max_lines,
		const float max_width, AbstractMeshSharedPtr &mesh,
		Uint32 &count) const
	{
		VertexBuffersSharedPtr buffers;
		Uint32 line;

		line = build_buffer(text, position, max_lines, max_width,
			buffers, count);

		if (count == 0)
		{
			return 0;
		}

		if (mesh.get() == 0)
		{
			mesh = m_mesh->clone_index_data();
		}

		mesh->init_vertex(buffers);

		return line;
	}

	void TextureFontCache::draw(StateManager &state_manager,
		const AbstractMeshSharedPtr &mesh, const Uint32 count) const
	{
		MeshDrawData draw_data;

		if (count == 0)
		{
			return;
		}

		draw_data = MeshDrawData(0, count * 6, 0, count * 4 - 1);

		state_manager.switch_mesh(mesh);

		state_manager.switch_texture(stt_diffuse_0, m_texture);

		state_manager.draw(draw_data);
	}

	glm::vec2 TextureFontCache::get_size(const Text &text) const
	{
		StringTextureFontMap::const_iterator found;
		glm::vec2 result, size;

		assert(m_fonts.begin() != m_fonts.end());

		BOOST_FOREACH(const Utf32StringTextAttributePair &data,
			text.get_text())
		{
			found = m_fonts.find(data.second.get_font());

			assert(found != m_fonts.end());

			if (found == m_fonts.end())
			{
				continue;
			}

			size = found->second->get_size(data.first, data.second);

			result.x += size.x;
			result.y = std::max(result.y, size.y);
		}

		return result;
	}

}
