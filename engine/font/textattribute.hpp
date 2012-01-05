/****************************************************************************
 *            textattribute.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_96885776_85b5_4c27_9aa7_9b1343c88271
#define	UUID_96885776_85b5_4c27_9aa7_9b1343c88271

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class TextAttribute.
 * This file contains the @c class TextAttribute.
 */
namespace eternal_lands
{

	/**
	 * @brief @c class for text attribute.
	 *
	 * @c class for text attribute.
	 */
	class TextAttribute
	{
		private:
			glm::vec4 m_color;
			String m_font;
			float m_size;
			float m_spacing;
			float m_rise;

		public:
			/**
			 * Default constructor.
			 */
			TextAttribute();

			/**
			 * Default destructor.
			 */
			~TextAttribute() throw();

			inline bool operator==(const TextAttribute &attribute)
				const
			{
				return get_color() == attribute.get_color() &&
					get_size() == attribute.get_size() &&
					get_spacing() == attribute.get_spacing()
					&& get_rise() == attribute.get_rise()
					&& get_font() == attribute.get_font();
			}

			inline bool operator!=(const TextAttribute &attribute)
				const
			{
				return !operator==(attribute);
			}

			inline void set_color(const glm::vec4 &color)
			{
				m_color = color;
			}

			inline void set_size(const float size)
			{
				m_size = size;
			}

			inline void set_spacing(const float spacing)
			{
				m_spacing = spacing;
			}

			inline void set_rise(const float rise)
			{
				m_rise = rise;
			}

			inline void set_font(const String font)
			{
				m_font = font;
			}

			inline const glm::vec4 &get_color() const
			{
				return m_color;
			}

			inline float get_size() const
			{
				return m_size;
			}

			inline float get_spacing() const
			{
				return m_spacing;
			}

			inline float get_rise() const
			{
				return m_rise;
			}

			inline const String &get_font() const
			{
				return m_font;
			}

	};

}

#endif	/* UUID_96885776_85b5_4c27_9aa7_9b1343c88271 */
