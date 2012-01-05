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
			Utf32StringTextAttributePairVector m_text;
			std::size_t m_hash;
			Uint32 m_length;

		public:
			/**
			 * Default constructor.
			 */
			Text();

			/**
			 * Default destructor.
			 */
			~Text() throw();

			void add(const Utf32String &str,
				const TextAttribute &attribute);

			Utf32String get_string() const;

			bool operator==(const Text &text) const;

			inline bool operator!=(const Text &text) const
			{
				return !operator==(text);
			}

			inline const Utf32StringTextAttributePairVector
				&get_text() const
			{
				return m_text;
			}

			inline std::size_t get_hash() const
			{
				return m_hash;
			}

			inline Uint32 get_length() const
			{
				return m_length;
			}

			inline void clear()
			{
				m_text.clear();
				m_length = 0;
				m_hash = 0;
			}

	};

}

#endif	/* UUID_467ff34a_217c_45f3_b3f4_67694197588b */
