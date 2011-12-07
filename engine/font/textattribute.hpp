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
			Uint16 m_size;
			bool m_bold;
			bool m_italic;

		public:
			/**
			 * Default constructor.
			 */
			TextAttribute();

			/**
			 * Default destructor.
			 */
			~TextAttribute() throw();

			inline void set_color(const glm::vec4 &color)
			{
				m_color = color;
			}

			inline void set_size(const Uint16 size)
			{
				m_size = size;
			}

			inline void set_bold(const bool bold)
			{
				m_bold = bold;
			}

			inline void set_italic(const bool italic)
			{
				m_italic = italic;
			}

			inline const glm::vec4 &get_color() const
			{
				return m_color;
			}

			inline Uint16 get_size() const
			{
				return m_size;
			}

			inline bool get_bold() const
			{
				return m_bold;
			}

			inline bool get_italic() const
			{
				return m_italic;
			}

	};

}

#endif	/* UUID_96885776_85b5_4c27_9aa7_9b1343c88271 */
