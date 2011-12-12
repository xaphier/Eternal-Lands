/****************************************************************************
 *            texturefontcache.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_5cadd685_7171_4ff0_abaa_01a916cdd6e4
#define	UUID_5cadd685_7171_4ff0_abaa_01a916cdd6e4

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
	class TextureFontCache
	{
		private:
			class TextureFontCacheItem;

			Uint16 m_width;
			Uint16 m_height;

			String get_font_file_name(const String &family,
				const float size, const bool bold,
				const bool italic);
		public:
			/**
			 * Default constructor.
			 */
			TextureFontCache();

			/**
			 * Default destructor.
			 */
			~TextureFontCache() throw();

			void add_font(const String &family, const float size,
				const bool bold, const bool italic);
			void add_font(const String &file_name);

	};

}

#endif	/* UUID_5cadd685_7171_4ff0_abaa_01a916cdd6e4 */
