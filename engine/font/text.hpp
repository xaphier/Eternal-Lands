/****************************************************************************
 *            text.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_467ff34a_217c_45f3_b3f4_67694197588b
#define	UUID_467ff34a_217c_45f3_b3f4_67694197588b

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "textattribute.hpp"

/**
 * @file
 * @brief The @c class Text.
 * This file contains the @c class Text.
 */
namespace eternal_lands
{

	/**
	 * @brief @c class for text.
	 *
	 * @c class for text.
	 */
	class Text
	{
		private:
			typedef std::pair<String, TextAttribute> StringTextAttributePair;

			std::vector<StringTextAttributePair> m_text;

		public:
			/**
			 * Default constructor.
			 */
			Text();

			/**
			 * Default destructor.
			 */
			~Text() throw();

			String get_string() const;

	};

}

#endif	/* UUID_467ff34a_217c_45f3_b3f4_67694197588b */
