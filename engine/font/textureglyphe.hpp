/****************************************************************************
 *            textureglyphe.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
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
			typedef std::map<wchar_t, float> WcharFloatMap;

			wchar_t m_charcode;
			Uint16 m_width;
			Uint16 m_height;
			glm::uvec2 m_offset;
			glm::vec2 m_advance;
			glm::uvec4 m_uv;
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

			void write_to_memory(const glm::vec4 &color,
				const float kerning, const float spacing,
				const float rise,
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

			inline const glm::uvec2 &get_offset() const
			{
				return m_offset;
			}

			inline const glm::vec2 &get_advance() const
			{
				return m_advance;
			}

			inline const glm::uvec4 &get_uv() const
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

}

#endif	/* UUID_63546ba5_5ad4_4a8c_b9c8_f617cff66a2c */
