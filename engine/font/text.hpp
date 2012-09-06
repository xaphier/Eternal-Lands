/****************************************************************************
 *            text.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_467ff34a_217c_45f3_b3f4_67694197588b
#define	UUID_467ff34a_217c_45f3_b3f4_67694197588b

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

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
			Uint32TextAttributeUint32PairMap m_text_attributes;
			Uint32Vector m_string;
			std::size_t m_hash;

		public:
			/**
			 * Default constructor.
			 */
			Text();

			/**
			 * Default destructor.
			 */
			~Text() noexcept;

			void add(const String &str,
				const TextAttribute &attribute);
			bool operator==(const Text &text) const;
			void clear();
			const TextAttribute &get_text_attribute(
				const Uint32 index, Uint32 &size) const;

			inline Utf32Char get_char(const Uint32 index) const
				noexcept
			{
				return m_string[index];
			}

			inline bool operator!=(const Text &text) const noexcept
			{
				return !operator==(text);
			}

			inline std::size_t get_hash() const noexcept
			{
				return m_hash;
			}

			inline Uint32 get_length() const noexcept
			{
				return m_string.size();
			}

	};

}

#endif	/* UUID_467ff34a_217c_45f3_b3f4_67694197588b */
