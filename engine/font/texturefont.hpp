/****************************************************************************
 *            texturefont.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_749cf992_67c9_4259_b638_8e28d20e797c
#define	UUID_749cf992_67c9_4259_b638_8e28d20e797c

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class TextureFont.
 * This file contains the @c class TextureFont.
 */
namespace eternal_lands
{

	/**
	 * @brief @c class for texture font.
	 *
	 * @c class for texture font.
	 */
	class TextureFont: public boost::noncopyable
	{
		private:
			typedef std::map<Utf32Char, TextureGlyphe>
				Utf32CharTextureGlypheMap;

			const AtlasSharedPtr m_atlas;
			const ImageSharedPtr m_image;
			Utf32CharTextureGlypheMap m_glyphs;
			String m_file_name;
			Uint8Array5 m_lcd_weights;
			float m_size;
			float m_gamma;
			float m_height;
			float m_line_gap;
			float m_ascender;
			float m_descender;
			bool m_bold;
			bool m_italic;
			bool m_antialias;
			bool m_sub_pixel;
			bool m_hinting;
			bool m_lcd_filter;

			String get_font_file_name(const String &family,
				const float size, const bool bold,
				const bool italic);
			size_t cache_glyphs(const Utf32String &char_codes);
			void init();

		public:
			/**
			 * Default constructor.
			 */
			TextureFont(const AtlasSharedPtr &atlas,
				const ImageSharedPtr &image,
				const String &family, const float size,
				const bool bold, const bool italic);

			TextureFont(const AtlasSharedPtr &atlas,
				const ImageSharedPtr &image,
				const String &file_name, const float size);

			/**
			 * Default destructor.
			 */
			~TextureFont() throw();

			void write_to_stream(const glm::vec4 &color,
				const glm::uvec2 &position, const String &str,
				const float spacing, const float rise,
				VertexStream &stream);

			glm::vec2 get_size(const String &str) const;

			inline const String &get_file_name() const
			{
				return m_file_name;
			}

			inline const Uint8Array5 &get_lcd_weights() const
			{
				return m_lcd_weights;
			}

			inline float get_size() const
			{
				return m_size;
			}

			inline float get_gamma() const
			{
				return m_gamma;
			}

			inline float get_height() const
			{
				return m_height;
			}

			inline float get_line_gap() const
			{
				return m_line_gap;
			}

			inline float get_ascender() const
			{
				return m_ascender;
			}

			inline float get_descender() const
			{
				return m_descender;
			}

			inline bool get_bold() const
			{
				return m_bold;
			}

			inline bool get_italic() const
			{
				return m_italic;
			}

			inline bool get_antialias() const
			{
				return m_antialias;
			}

			inline bool get_sub_pixel() const
			{
				return m_sub_pixel;
			}

			inline bool get_hinting() const
			{
				return m_hinting;
			}

			inline bool get_lcd_filter() const
			{
				return m_lcd_filter;
			}

	};

}

#endif	/* UUID_749cf992_67c9_4259_b638_8e28d20e797c */
