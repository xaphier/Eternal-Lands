/****************************************************************************
 *            textureglyphe.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "textureglyphe.hpp"

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

#if	0
void
texture_glyph_add_to_vertex_buffer( const TextureGlyph *self,
                                    VertexBuffer *buffer,
                                    const Markup *markup,
                                    Pen *pen, int kerning )
{
    TextureFont *font = self->font;
    float r = 1;
    float g = 1;
    float b = 1;
    float a = 1;
    int rise = 0;
    int spacing = 0;

    if( markup )
    {
        rise = markup->rise;
        spacing = markup->spacing;
    }
    
    pen->x += kerning;

    // Underline

    // Overline

    // Outline

    // Strikethrough

    // Actual glyph
    if( markup )
    {
        r = markup->foreground_color.r;
        g = markup->foreground_color.g;
        b = markup->foreground_color.b;
        a = markup->foreground_color.a;
    }
    int x0  = (int)( pen->x + self->offset_x );
    int y0  = (int)( pen->y + self->offset_y + rise );
    int x1  = (int)( x0 + self->width );
    int y1  = (int)( y0 - self->height );
    float u0 = self->u0;
    float v0 = self->v0;
    float u1 = self->u1;
    float v1 = self->v1;
    GLuint index = buffer->vertices->size;
    GLuint indices[] = {index, index+1, index+2,
                        index, index+2, index+3};
    TextureGlyphVertex vertices[] = { { x0,y0,0,  u0,v0,  r,g,b,a },
                                      { x0,y1,0,  u0,v1,  r,g,b,a },
                                      { x1,y1,0,  u1,v1,  r,g,b,a },
                                      { x1,y0,0,  u1,v0,  r,g,b,a } };
    vertex_buffer_push_back_indices( buffer, indices, 6 );
    vertex_buffer_push_back_vertices( buffer, vertices, 4 );

    pen->x += self->advance_x + spacing;
    pen->y += self->advance_y;

	void TextureGlyphe::write_to_memory(const glm::vec4 &color,
		const float kerning, const float spacing, const float rise,
		const AbstractWriteMemoryBuffer &buffer,
		glm::uvec2 &position)
	{
		position.x += kerning;

		x0 = position.x + get_offset().x;
		y0 = position.y + get_offset().y + rise;
		x1 = x0 + get_width();
		y1 = y0 - get_height();

		vertex_stream->set(vst_position, glm::vec4(x0, y0, 0.0f, 1.0f));
		vertex_stream->set(vst_texture_coordinate_0,
			glm::vec4(get_uv().x, get_uv().y, 0.0f, 1.0f));
		vertex_stream->set(vst_color, color);
		vertex_stream->push_vertex();

		vertex_stream->set(vst_position, glm::vec4(x0, y1, 0.0f, 1.0f));
		vertex_stream->set(vst_texture_coordinate_0,
			glm::vec4(get_uv().x, get_uv().w, 0.0f, 1.0f));
		vertex_stream->set(vst_color, color);
		vertex_stream->push_vertex();

		vertex_stream->set(vst_position, glm::vec4(x1, y1, 0.0f, 1.0f));
		vertex_stream->set(vst_texture_coordinate_0,
			glm::vec4(get_uv().z, get_uv().w, 0.0f, 1.0f));
		vertex_stream->set(vst_color, color);
		vertex_stream->push_vertex();

		vertex_stream->set(vst_position, glm::vec4(x1, y0, 0.0f, 1.0f));
		vertex_stream->set(vst_texture_coordinate_0,
			glm::vec4(get_uv().z, get_uv().y, 0.0f, 1.0f));
		vertex_stream->set(vst_color, color);
		vertex_stream->push_vertex();

		position.x = get_advance().x + spacing;
		position.y = get_advance().y;
	}

	TextureGlyphe
	class TextureGlyphe
	{
		private:
			typedef std::map<wchar_t, float> WcharFloatMap;

			wchar_t m_charcode;
			Uint16 m_width;
			Uint16 m_height;
			Uint16Array2 m_offset;
			glm::vec2 m_advance;
			glm::vec4 m_uv;
			WcharFloatMap m_kerning;

		public:
			/**
			 * Default constructor.
			 */
			TextureGlyphe();

			/**
			 * Default destructor.
			 */
			~TextureGlyphe() throw();

			void write_to_memory(const Uint32 color,
				const float kerning,
				const AbstractWriteMemoryBuffer &buffer,
				Uint16Array2 &position); 

			inline wchar_t get_charcode() const
			{
				return m_charcode;
			}

			inline Uint16 get_width() const
			{
				return m_width;
			}

			inline Uint16 get_height() const
			{
				return m_height;
			}

			inline const Uint16Array2 &get_offset() const
			{
				return m_offset;
			}

			inline const Uint16Array2 &get_advance() const
			{
				return m_advance;
			}

			inline const Uint16Array4 &get_uv() const
			{
				return m_uv;
			}

			inline float get_kerning(const TextureGlyphe &glyphe)
				const
			{
				return get_kerning(glyphe.get_charcode());
			}

			inline float get_kerning(const wchar_t charcode) const
			{
				WcharFloatMap::const_iterator found;

				found = m_kerning.find(charcode);

				if (found == m_kerning.end())
				{
					return 0.0f;
				}

				return found->second;
			}

	};

#endif

}
