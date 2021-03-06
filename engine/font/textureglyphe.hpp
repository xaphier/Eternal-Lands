/****************************************************************************
 *            textureglyphe.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_63546ba5_5ad4_4a8c_b9c8_f617cff66a2c
#define	UUID_63546ba5_5ad4_4a8c_b9c8_f617cff66a2c

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class TextureGlyphe.
 * This file contains the @c class TextureGlyphe.
 */
namespace eternal_lands
{

	/**
	 * @brief @c class for texture glyphe.
	 *
	 * @c class for texture glyphe.
	 */
	class TextureGlyphe
	{
		private:
			Utf32Char m_char_code;
			Uint16 m_width;
			Uint16 m_height;
			glm::vec2 m_offset;
			glm::vec2 m_advance;
			glm::vec4 m_uv;
			Utf32CharFloatMap m_kerning;

		public:
			/**
			 * Default constructor.
			 */
			TextureGlyphe(const Utf32Char char_code,
				const Uint16 width, const Uint16 height,
				const glm::vec2 &offset,
				const glm::vec2 &advance,
				const glm::vec4 &uv);

			/**
			 * Default destructor.
			 */
			~TextureGlyphe() noexcept;

			void write_to_stream(const TextAttribute &attribute,
				const float height, const float kerning,
				const VertexStreamsSharedPtr &streams,
				glm::vec2 &position) const;
			void advance(const TextAttribute &attribute,
				const float kerning, glm::vec2 &position) const;
			glm::vec2 get_size(const TextAttribute &attribute,
				const float kerning) const;

			inline void set_kerning(
				const Utf32CharFloatMap &kerning) noexcept
			{
				m_kerning = kerning;
			}

			inline Utf32Char get_char_code() const noexcept
			{
				return m_char_code;
			}

			inline Uint16 get_width() const noexcept
			{
				return m_width;
			}

			inline Uint16 get_height() const noexcept
			{
				return m_height;
			}

			inline const glm::vec2 &get_offset() const noexcept
			{
				return m_offset;
			}

			inline const glm::vec2 &get_advance() const noexcept
			{
				return m_advance;
			}

			inline const glm::vec4 &get_uv() const noexcept
			{
				return m_uv;
			}

			inline float get_kerning(const TextureGlyphe &glyphe)
				const noexcept
			{
				return get_kerning(glyphe.get_char_code());
			}

			inline float get_kerning(const Utf32Char char_code)
				const noexcept
			{
				Utf32CharFloatMap::const_iterator found;

				found = m_kerning.find(char_code);

				if (found == m_kerning.end())
				{
					return 0.0f;
				}

				return found->second;
			}

	};

	typedef std::vector<TextureGlyphe> TextureGlypheVector;

}

#endif	/* UUID_63546ba5_5ad4_4a8c_b9c8_f617cff66a2c */
