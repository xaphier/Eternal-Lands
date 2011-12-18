/****************************************************************************
 *            texturearray.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_d27acf4f_88c7_4420_8e14_7a869d66da94
#define	UUID_d27acf4f_88c7_4420_8e14_7a869d66da94

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "textureformatutil.hpp"

/**
 * @file
 * @brief The @c class TextureArray.
 * This file contains the @c class TextureArray.
 */
namespace eternal_lands
{

	class TextureArray
	{
		private:
			StringVector m_file_names;
			String m_name;
			Uint32 m_width;
			Uint32 m_height;
			Uint16 m_mipmaps;
			TextureFormatType m_format;

		public:
			TextureArray();
			~TextureArray() throw();
			void load_xml(const xmlNodePtr node);

			inline const StringVector &get_file_names() const
			{
				return m_file_names;
			}

			inline const String &get_name() const
			{
				return m_name;
			}

			inline Uint32 get_width() const
			{
				return m_width;
			}

			inline Uint32 get_height() const
			{
				return m_height;
			}

			inline Uint16 get_mipmaps() const
			{
				return m_mipmaps;
			}

			inline TextureFormatType get_format() const
			{
				return m_format;
			}

	};

}

#endif	/* UUID_d27acf4f_88c7_4420_8e14_7a869d66da94 */
