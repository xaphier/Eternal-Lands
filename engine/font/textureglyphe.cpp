/****************************************************************************
 *            textureglyphe.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "textureglyphe.hpp"
#include "vertexstreams.hpp"
#include "textattribute.hpp"

namespace eternal_lands
{

	TextureGlyphe::TextureGlyphe(const Utf32Char char_code,
		const Uint16 width, const Uint16 height,
		const glm::vec2 &offset, const glm::vec2 &advance,
		const glm::vec4 &uv): m_char_code(char_code), m_width(width),
		m_height(height), m_offset(offset), m_advance(advance),
		m_uv(uv)
	{
	}

	TextureGlyphe::~TextureGlyphe() throw()
	{
	}

	void TextureGlyphe::write_to_stream(const TextAttribute &attribute,
		const Uint32 padd, const float height, const float kerning,
		const VertexStreamsSharedPtr &streams, glm::vec2 &position)
		const
	{
		float x0, x1, y0, y1;
#if	0
		position.x += kerning;

		x0 = position.x + get_offset().x;
		y0 = position.y + get_offset().y + attribute.get_rise();
		x1 = x0 + get_width();
		y1 = y0 - get_height();
#else
		position.x += kerning;

		x0 = get_offset().x - padd;
		y0 = get_offset().y + attribute.get_rise() + padd;
		x1 = x0 + get_width() + padd;
		y1 = y0 - get_height() - padd;

		x0 = x0 + position.x;
		y0 = height * 0.5 - y0 + position.y;
		x1 = x1 + position.x;
		y1 = height * 0.5 - y1 + position.y;
#endif

		streams->set(vst_position, glm::vec4(x0, y0, 0.0f, 1.0f));
		streams->set(vst_texture_coordinate_0,
			glm::vec4(get_uv().x, get_uv().y, 0.0f, 1.0f));
		streams->set(vst_color, attribute.get_color());
		streams->push_vertex();

		streams->set(vst_position, glm::vec4(x0, y1, 0.0f, 1.0f));
		streams->set(vst_texture_coordinate_0,
			glm::vec4(get_uv().x, get_uv().w, 0.0f, 1.0f));
		streams->set(vst_color, attribute.get_color());
		streams->push_vertex();

		streams->set(vst_position, glm::vec4(x1, y1, 0.0f, 1.0f));
		streams->set(vst_texture_coordinate_0,
			glm::vec4(get_uv().z, get_uv().w, 0.0f, 1.0f));
		streams->set(vst_color, attribute.get_color());
		streams->push_vertex();

		streams->set(vst_position, glm::vec4(x1, y0, 0.0f, 1.0f));
		streams->set(vst_texture_coordinate_0,
			glm::vec4(get_uv().z, get_uv().y, 0.0f, 1.0f));
		streams->set(vst_color, attribute.get_color());
		streams->push_vertex();

		position.x += get_advance().x + attribute.get_spacing();
		position.y += get_advance().y;
	}

	glm::vec2 TextureGlyphe::get_size(const TextAttribute &attribute,
		const float kerning) const
	{
		glm::vec2 size;

		size.x += kerning;

		size.x = get_advance().x + attribute.get_spacing();
		size.y = get_advance().y;

		return size;
	}

}
