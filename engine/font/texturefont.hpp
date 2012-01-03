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
			typedef std::pair<Utf32Char, TextureGlyphe>
				Utf32CharTextureGlyphePair;
			typedef std::map<Utf32Char, TextureGlyphe>
				Utf32CharTextureGlypheMap;

			Utf32CharTextureGlypheMap m_glyphs;
			String m_file_name;
			float m_size;
			float m_height;
			float m_line_gap;
			float m_ascender;
			float m_descender;
			bool m_antialias;
			bool m_sub_pixel;
			bool m_hinting;

			void cache_glyphs(const AtlasSharedPtr &atlas,
				const DoubleSharedArray &data,
				const Utf32String &char_codes);
			void init(const AtlasSharedPtr &atlas,
				const DoubleSharedArray &data,
				const Utf32String &char_codes);

		public:
			/**
			 * Default constructor.
			 */
			TextureFont(const AtlasSharedPtr &atlas,
				const DoubleSharedArray &data,
				const FileSystemSharedPtr &file_system,
				const String &file_name, const float size);

			/**
			 * Default destructor.
			 */
			~TextureFont() throw();

			Uint32 write_to_stream(const Utf32String &str,
				const VertexStreamsSharedPtr &streams,
				const glm::vec2 &position,
				const glm::vec4 &color = glm::vec4(1.0f),
				const Uint32 max_lines = 1,
				const float max_width =
					std::numeric_limits<Uint32>::max(),
				const float spacing = 0.0f,
				const float rise = 0.0f) const;
			glm::vec2 get_size(const Utf32String &str,
				const float spacing, const float rise) const;
			glm::vec2 get_size(const String &str) const;

			inline const String &get_file_name() const
			{
				return m_file_name;
			}

			inline float get_size() const
			{
				return m_size;
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

	};

}

#endif	/* UUID_749cf992_67c9_4259_b638_8e28d20e797c */
