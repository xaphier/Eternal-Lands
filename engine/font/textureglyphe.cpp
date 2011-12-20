/****************************************************************************
 *            textureglyphe.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "textureglyphe.hpp"
#include "vertexstreams.hpp"

namespace eternal_lands
{

	TextureGlyphe::TextureGlyphe(const Utf32Char char_code,
		const Uint16 width, const Uint16 height,
		const glm::uvec2 &offset, const glm::vec2 &advance,
		const glm::uvec4 &uv): m_char_code(char_code), m_width(width),
		m_height(height), m_offset(offset), m_advance(advance),
		m_uv(uv)
	{
	}

	TextureGlyphe::~TextureGlyphe() throw()
	{
	}

	void TextureGlyphe::write_to_stream(const glm::vec4 &color,
		const float kerning, const float spacing, const float rise,
		VertexStreams &streams, glm::uvec2 &position) const
	{
		float x0, x1, y0, y1;

		position.x += kerning;

		x0 = position.x + get_offset().x;
		y0 = position.y + get_offset().y + rise;
		x1 = x0 + get_width();
		y1 = y0 - get_height();

		streams.set(vst_position, glm::vec4(x0, y0, 0.0f, 1.0f));
		streams.set(vst_texture_coordinate_0,
			glm::vec4(get_uv().x, get_uv().y, 0.0f, 1.0f));
		streams.set(vst_color, color);
		streams.push_vertex();

		streams.set(vst_position, glm::vec4(x0, y1, 0.0f, 1.0f));
		streams.set(vst_texture_coordinate_0,
			glm::vec4(get_uv().x, get_uv().w, 0.0f, 1.0f));
		streams.set(vst_color, color);
		streams.push_vertex();

		streams.set(vst_position, glm::vec4(x1, y1, 0.0f, 1.0f));
		streams.set(vst_texture_coordinate_0,
			glm::vec4(get_uv().z, get_uv().w, 0.0f, 1.0f));
		streams.set(vst_color, color);
		streams.push_vertex();

		streams.set(vst_position, glm::vec4(x1, y0, 0.0f, 1.0f));
		streams.set(vst_texture_coordinate_0,
			glm::vec4(get_uv().z, get_uv().y, 0.0f, 1.0f));
		streams.set(vst_color, color);
		streams.push_vertex();

		position.x = get_advance().x + spacing;
		position.y = get_advance().y;
	}

}
