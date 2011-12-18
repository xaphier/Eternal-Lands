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

namespace eternal_lands
{

	TextureFontCache::TextureFontCache(const Uint16 width,
		const Uint16 height)
	{
		Uint32Array3 sizes;

		sizes[0] = width;
		sizes[1] = height;
		sizes[2] = 1;

		m_image = boost::make_shared<Image>(String(UTF8("Fonts")),
			false, tft_rgb8, sizes, 0);

		m_atlas = boost::make_shared<Atlas>(width, height);

		m_texture = boost::make_shared<Texture>(String(UTF8("font")));
	}

	TextureFontCache::~TextureFontCache() throw()
	{
	}
                
	void TextureFontCache::add_font(const String &family, const float size,
		const bool bold, const bool italic)
	{
		m_fonts.push_back(new TextureFont(m_atlas, m_image, family,
			size, bold, italic));
	}

	void TextureFontCache::add_font(const String &file_name,
		const float size)
	{
		m_fonts.push_back(new TextureFont(m_atlas, m_image, file_name,
			size));
	}

	void TextureFontCache::update_texture()
	{
		m_texture->set_format(m_image->get_texture_format());
		m_texture->set_image(m_image);
	}

}
