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
			false, tft_rgb8, sizes, 0);

		m_atlas = boost::make_shared<Atlas>(width, height);

		m_data.reset(new double[width * height]);

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

		m_buffers = m_mesh->get_vertex_buffers();

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
		boost::scoped_array<double> gx, gy, outside, inside;
		double min, max, value;
		Uint32 size, width, height, x, y, i, index;

		width = m_atlas->get_width();
		height = m_atlas->get_height();

		size = width * height;

		xdist.reset(new short[size]);
		ydist.reset(new short[size]);

		gx.reset(new double[size]);
		gy.reset(new double[size]);
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
			m_data[i] = (m_data[i] - min) / max;
		}

		// Compute outside = edtaa3(bitmap); % Transform background (0's)
		computegradient(m_data.get(), height, width, gx.get(),
			gy.get());
		edtaa3(m_data.get(), gx.get(), gy.get(), height, width,
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
			m_data[i] = 1.0 - m_data[i];
		}

		computegradient(m_data.get(), height, width, gx.get(),
			gy.get());
		edtaa3(m_data.get(), gx.get(), gy.get(), height, width,
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

				value = outside[index] - inside[index];
				value = 128 + value * 16;
				value = std::min(255.0, std::max(0.0, value));

				m_image->set_pixel(x, y, 0, 0, 0,
					glm::vec4(255 - value));
			}
		}

		m_texture->set_format(m_image->get_texture_format());
		m_texture->set_image(m_image);
	}

	void TextureFontCache::draw(StateManager &state_manager,
		const Utf32String &str, const String &name,
		const glm::vec2 &position, const glm::vec4 &color,
		const Uint32 max_lines, const float max_width,
		const float spacing, const float rise) const
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
			position, color, max_lines, max_width, spacing, rise);

		if (count == 0)
		{
			return;
		}

		draw_data = MeshDrawData(0, count * 6, 0, count * 4 - 1);

		state_manager.switch_mesh(m_mesh);
		m_mesh->update_vertex(m_buffers);

		state_manager.switch_texture(stt_diffuse_0, m_texture);

		state_manager.draw(draw_data);
	}

}
