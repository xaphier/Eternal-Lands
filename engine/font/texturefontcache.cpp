/****************************************************************************
 *            texturefontcache.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
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
#include "globalvars.hpp"

namespace eternal_lands
{

	namespace
	{

		const Utf32String init_char_codes = L" abcdefghijklmnopqrstuv"
			"wxyzäöüßABCDEFGHIJKLMNOPQRSTUVWXYZ!\"§$%&/()=?+-_.:,"
			";<>|µ@^°{[]}\\1234567890#'~²³";

	}

	TextureFontCache::TextureFontCache(
		const GlobalVarsSharedPtr &global_vars,
		const FileSystemSharedPtr &file_system,
		const MeshBuilderSharedPtr &mesh_builder, const Uint16 width,
		const Uint16 height, const Uint16 max_char_count)
	{
		StringVariantMap values;
		glm::uvec3 sizes;
		SubMeshVector sub_meshs;
		Uint32Vector indices;
		Uint32 i;
		bool use_16_bit_indices;

		sizes[0] = width;
		sizes[1] = height;
		sizes[2] = 1;

		if (global_vars->get_opengl_3_0())
		{
			m_image = boost::make_shared<Image>(String(
				UTF8("Fonts")), false, tft_r8, sizes, 0);
		}
		else
		{
			m_image = boost::make_shared<Image>(String(
				UTF8("Fonts")), false, tft_l8, sizes, 0);
		}

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

	TextureFontCache::~TextureFontCache() noexcept
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
		XmlReaderSharedPtr xml_reader;

		xml_reader = XmlReaderSharedPtr(new XmlReader(file_system,
			file_name));

		load_xml(file_system, xml_reader->get_root_node());
	}

	void TextureFontCache::add_font(const FileSystemSharedPtr &file_system,
		const String &name, const String &file_name, const float size)
	{
		std::auto_ptr<TextureFont> font;

		try
		{
			font.reset(new TextureFont(m_atlas, m_data,
				file_system, init_char_codes, file_name, size));

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
		const Uint32 min_line, const Uint32 max_line,
		const float max_width, const float max_height,
		const WrapModeType wrap)
	{
		Uint32 count, line;

		line = build_buffer(text, position, min_line, max_line,
			max_width, max_height, wrap, m_buffers, count);

		m_mesh->update_vertex(m_buffers);

		draw(state_manager, m_mesh, count);

		return line;
	}

	Uint32 TextureFontCache::build_buffer(const Text &text,
		const glm::vec2 &position, const Uint32 min_line,
		const Uint32 max_line, const float max_width,
		const float max_height, const WrapModeType wrap,
		VertexBuffersSharedPtr &buffers, Uint32 &count) const
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

		if (buffers.get() == nullptr)
		{
			buffers = m_mesh->get_vertex_buffers(count);
		}

		if (buffers->get_vertex_count() < count)
		{
			buffers = m_mesh->get_vertex_buffers(count);
		}

		buffers->reset();

		pos = position;
		line = 0;

		assert(max_width > 0.0f);
		assert(max_height > 0.0f);
		assert(max_line >= min_line);

		count = write_to_stream(text, buffers, pos, min_line, max_line,
			max_width, max_height, wrap, line);

		assert(count > 0);

		if (count == 0)
		{
			return 0;
		}

		return line;
	}

	Uint32 TextureFontCache::build_mesh(const Text &text,
		const glm::vec2 &position, const Uint32 min_line,
		const Uint32 max_line, const float max_width,
		const float max_height, const WrapModeType wrap,
		AbstractMeshSharedPtr &mesh, Uint32 &count) const
	{
		VertexBuffersSharedPtr buffers;
		Uint32 line;

		line = build_buffer(text, position, min_line, max_line,
			max_width, max_height, wrap, buffers, count);

		if (count == 0)
		{
			return 0;
		}

		if (mesh.get() == nullptr)
		{
			mesh = m_mesh->clone_index_data();
		}

		mesh->init_vertex(buffers);

		return line;
	}

	void TextureFontCache::build_mesh(const VertexBuffersSharedPtr &buffers,
		const Uint32 count, AbstractMeshSharedPtr &mesh) const
	{
		if (mesh.get() == nullptr)
		{
			mesh = m_mesh->clone_index_data();
		}

		mesh->init_vertex(buffers);
	}

	void TextureFontCache::draw(StateManager &state_manager,
		const VertexBuffersSharedPtr &buffers, const Uint32 count) const
	{
		m_mesh->update_vertex(buffers);

		draw(state_manager, m_mesh, count);
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

		state_manager.switch_texture(stt_albedo_0, m_texture);

		state_manager.draw(draw_data, 1);
	}

	float TextureFontCache::get_width(const Text &text) const
	{
		TextAttribute attribute;
		StringTextureFontMap::const_iterator found, end;
		glm::vec2 position;
		Utf32Char last_char_code, char_code;
		float width;
		Uint32 i, j, count, size;

		count = text.get_length();

		if (count == 0)
		{
			return 0;
		}

		width = 0.0f;
		end = m_fonts.end();
		last_char_code = L'\0';
		i = 0;

		while (i < count)
		{
			const TextAttribute &attribute =
				text.get_text_attribute(i, size);
			found = m_fonts.find(attribute.get_font());

			assert(found != end);
			assert(size > 0);
			assert((i + size) <= count);

			if (found == end)
			{
				i += size;
				continue;
			}

			for (j = 0; j < size; ++j, ++i)
			{
				char_code = text.get_char(i);

				if ((char_code == L'\n') ||
					(char_code == L'\r') ||
					(char_code == L'\0'))
				{
					return width;
				}

				width += found->second->get_width(attribute,
					last_char_code, char_code);

				last_char_code = char_code;
			}
		}

		return width;
	}

	float TextureFontCache::get_height(const String &font) const
	{
		StringTextureFontMap::const_iterator found, end;

		found = m_fonts.find(font);

		if (found == end)
		{
			return 0.0f;
		}

		return found->second->get_height() -
			found->second->get_line_gap();
	}

	Uint32 TextureFontCache::write_to_stream(const Text &text,
		const VertexStreamsSharedPtr &streams,
		const glm::vec2 &start_position, const Uint32 min_line,
		const Uint32 max_line, const float max_width,
		const float max_height, const WrapModeType wrap, Uint32 &line)
		const
	{
		StringTextureFontMap::const_iterator found, end;
		glm::vec2 position;
		Utf32Char last_char_code, char_code;
		float max_position, width, height, line_height;
		Uint32 i, j, count, chars, split_index, size;
		bool new_line;

		assert(max_width > 0.0f);

		count = text.get_length();

		if (count == 0)
		{
			return 0;
		}

		position = start_position;
		end = m_fonts.end();
		max_position = max_width + start_position.x;
		split_index = std::numeric_limits<Uint32>::max();
		last_char_code = L'\0';
		chars = 0;
		new_line = true;
		i = 0;

		if (wrap != wmt_none)
		{
			split_index = check_line(text, 0, max_width,
				last_char_code, wrap == wmt_word, new_line);
		}

		while (i < count)
		{
			const TextAttribute &attribute =
				text.get_text_attribute(i, size);
			found = m_fonts.find(attribute.get_font());

			assert(found != end);
			assert(size > 0);
			assert((i + size) <= count);

			if (found == end)
			{
				i += size;
				continue;
			}

			line_height = found->second->get_height() -
				found->second->get_line_gap();

			height = max_height - line_height + start_position.y;

			for (j = 0; j < size; ++j, ++i)
			{
				char_code = text.get_char(i);

				if (char_code == L'\0')
				{
					continue;
				}

				if ((char_code == L'\n') ||
					(char_code == L'\r') ||
					(split_index == i))
				{
					if (wrap != wmt_none)
					{
						split_index = check_line(text,
							i + 1, max_position -
								position.x,
							last_char_code,
							wrap == wmt_word,
							new_line);
					}

					position.y += line_height;
					position.x = start_position.x;

					line++;

					if ((line > max_line) ||
						(position.y > height))
					{
						return chars;
					}

					last_char_code = L'\0';
					new_line = true;

					continue;
				}

				width = found->second->get_width(attribute,
					last_char_code, char_code);

				if ((position.x + width) >= max_position)
				{
					position.y += line_height;
					position.x = start_position.x;

					line++;

					if ((line > max_line) ||
						(position.y > height))
					{
						return chars;
					}

					while (i < count)
					{
						if ((text.get_char(i) == L'\n')
							|| (text.get_char(i) ==
								L'\r'))
						{
							break;
						}
						++i;
					}

					last_char_code = L'\0';
					new_line = true;

					continue;
				}

				new_line = false;

				if (min_line > line)
				{
					chars += found->second->advance(
						attribute, last_char_code,
						char_code, position);

					last_char_code = char_code;

					continue;
				}

				chars += found->second->write_to_stream(
					attribute, streams, last_char_code,
					char_code, position);

				last_char_code = char_code;
			}
		}

		return chars;
	}

	Uint32 TextureFontCache::check_line(const Text &text,
		const Uint32 index, const float max_width,
		const Utf32Char start_char_code, const bool word_wrap,
		const bool new_line) const
	{
		TextAttribute attribute;
		StringTextureFontMap::const_iterator found, end;
		Utf32Char char_code, last_char_code;
		float width;
		Uint32 i, count, split_index, size;

		end = m_fonts.end();
		width = 0;
		split_index = index;
		count = text.get_length();
		last_char_code = start_char_code;

		for (i = index; i < count; ++i)
		{
			char_code = text.get_char(i);

			if ((char_code == L'\n') ||
				(char_code == L'\r'))
			{
				return i;
			}

			if ((char_code == L' ') ||
				(char_code == L'\t'))
			{
				split_index = i;
			}

			if (size <= i)
			{
				attribute = text.get_text_attribute(i, size);

				found = m_fonts.find(attribute.get_font());

				last_char_code = L'\0';

				size += i;

				if (found == m_fonts.end())
				{
					i = size;

					continue;
				}
			}

			width += found->second->get_width(attribute,
				char_code, last_char_code);

			if (width >= max_width)
			{
				if (word_wrap && !new_line &&
					(split_index != index))
				{
					return split_index;
				} 
				else
				{
					return i;
				}
			}

			last_char_code = char_code;
		}

		return count;
	}

}
