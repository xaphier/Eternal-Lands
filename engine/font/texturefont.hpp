/****************************************************************************
 *            texturefont.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_749cf992_67c9_4259_b638_8e28d20e797c
#define	UUID_749cf992_67c9_4259_b638_8e28d20e797c

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "texturefontcache.hpp"

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
			Utf32CharTextureGlypheMap m_glyphs;
			String m_file_name;
			Utf32CharTextureGlypheMap::const_iterator m_default_char;
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
				const Utf32String &char_codes,
				const String &file_name, const float size);

			/**
			 * Default destructor.
			 */
			~TextureFont() noexcept;

			Uint32 write_to_stream(const TextAttribute &attribute,
				const VertexStreamsSharedPtr &streams,
				const Utf32Char last_char_code,
				const Utf32Char char_code,
				glm::vec2 &position) const;
			Uint32 advance(const TextAttribute &attribute,
				const Utf32Char last_char_code,
				const Utf32Char char_code,
				glm::vec2 &position) const;
			float get_width(const TextAttribute &attribute,
				const Utf32Char last_char_code,
				const Utf32Char char_code) const;

			inline const String &get_file_name() const noexcept
			{
				return m_file_name;
			}

			inline float get_size() const noexcept
			{
				return m_size;
			}

			inline float get_height() const noexcept
			{
				return m_height;
			}

			inline float get_line_gap() const noexcept
			{
				return m_line_gap;
			}

			inline float get_ascender() const noexcept
			{
				return m_ascender;
			}

			inline float get_descender() const noexcept
			{
				return m_descender;
			}

			inline bool get_antialias() const noexcept
			{
				return m_antialias;
			}

			inline bool get_sub_pixel() const noexcept
			{
				return m_sub_pixel;
			}

			inline bool get_hinting() const noexcept
			{
				return m_hinting;
			}

	};

}

#endif	/* UUID_749cf992_67c9_4259_b638_8e28d20e797c */
