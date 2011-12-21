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

namespace eternal_lands
{

	TextureFontCache::TextureFontCache(
		const MeshBuilderSharedPtr &mesh_builder, const Uint16 width,
		const Uint16 height, const Uint16 max_char_count)
	{
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
			indices.push_back(i * 4 + 2);
			indices.push_back(i * 4 + 1);

			indices.push_back(i * 4 + 1);
			indices.push_back(i * 4 + 2);
			indices.push_back(i * 4 + 3);
		}

		use_16_bit_indices = (4 * max_char_count) <
			std::numeric_limits<Uint16>::max();

		m_mesh->init_indices(use_16_bit_indices, pt_triangles,
			sub_meshs, indices);

		m_buffers = m_mesh->get_vertex_buffers();
	}

	TextureFontCache::~TextureFontCache() throw()
	{
	}
                
	void TextureFontCache::add_font(const String &index,
		const String &family, const float size, const bool bold,
		const bool italic)
	{
		std::auto_ptr<TextureFont> font;

		font.reset(new TextureFont(m_atlas, m_image, family, size,
			bold, italic));

		m_fonts.insert(index, font);
	}

	void TextureFontCache::add_font(const String &index,
		const String &file_name, const float size)
	{
		std::auto_ptr<TextureFont> font;

		font.reset(new TextureFont(m_atlas, m_image, file_name, size));

		m_fonts.insert(index, font);
	}

	void TextureFontCache::update_texture()
	{
		m_texture->set_format(m_image->get_texture_format());
		m_texture->set_image(m_image);
	}

	void TextureFontCache::draw(const Utf32String &str,
		const glm::vec2 &position, const glm::vec4 &color,
		const float spacing, const float rise) const
	{
	}

}
