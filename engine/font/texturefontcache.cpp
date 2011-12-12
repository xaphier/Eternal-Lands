/****************************************************************************
 *            texturefontcache.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "texturefontcache.hpp"
#include <ft2build.h>
#include FT_FREETYPE_H 
#ifdef	LINUX
#include <fontconfig/fontconfig.h>
#endif

namespace eternal_lands
{

	TextureFontCache::TextureFontCache()
	{
	}

	TextureFontCache::~TextureFontCache() throw()
	{
	}

	String TextureFontCache::get_font_file_name(const String &family,
		const float size, const bool bold, const bool italic)
	{
#ifdef	LINUX
		std::string file_name;
		Sint32 weight, slant;
		FcPattern *pattern;
		FcPattern *match;
		FcResult result;
		FcValue value;

		weight = FC_WEIGHT_REGULAR;
		slant = FC_SLANT_ROMAN;

		if (bold)
		{
			weight = FC_WEIGHT_BOLD;
		}

		if (italic)
		{
			slant = FC_SLANT_ITALIC;
		}

		FcInit();
		pattern = FcPatternCreate();
		FcPatternAddDouble(pattern, FC_SIZE, size);
		FcPatternAddInteger(pattern, FC_WEIGHT, weight);
		FcPatternAddInteger(pattern, FC_SLANT, slant);
		FcPatternAddString(pattern, FC_FAMILY, (FcChar8*)family.get().c_str());
		FcConfigSubstitute(0, pattern, FcMatchPattern );
		FcDefaultSubstitute(pattern);
		match = FcFontMatch( 0, pattern, &result);
		FcPatternDestroy(pattern);

		if (match == 0)
		{
			fprintf( stderr, "fontconfig error: could not match family '%s'", family.get().c_str());
			return String();
		}
		else
		{
			result = FcPatternGet(match, FC_FILE, 0, &value);

			if (result)
			{
				fprintf( stderr, "fontconfig error: could not match family '%s'", family.get().c_str());
			}
			else
			{
				file_name = (char *)(value.u.s);
			}
		}

		FcPatternDestroy(match);

//    font = texture_font_new( self->atlas, filename, size );
//    texture_font_cache_glyphs( font, self->cache );

		return String(file_name);
#else	/* LINUX */
		return String();
#endif	/* LINUX */
	}

	void TextureFontCache::add_font(const String &family, const float size,
		const bool bold, const bool italic)
	{
		add_font(get_font_file_name(family, size, bold, italic));
	}

	void TextureFontCache::add_font(const String &file_name)
	{
	}

}
