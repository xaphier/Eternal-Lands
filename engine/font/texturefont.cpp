/****************************************************************************
 *            texturefont.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "texturefont.hpp"
#include "image.hpp"
#include "textureglyphe.hpp"
#include "atlas.hpp"
#include "exceptions.hpp"
#include <ft2build.h>
#include FT_FREETYPE_H 
#include FT_LCD_FILTER_H
#ifdef	LINUX
#include <fontconfig/fontconfig.h>
#endif

#undef __FTERRORS_H__
#define FT_ERRORDEF( e, v, s )  { e, s },
#define FT_ERROR_START_LIST     {
#define FT_ERROR_END_LIST       { 0, 0 } };
const struct {
    int          code;
    const char*  message;
} FT_Errors[] =
#include FT_ERRORS_H

namespace eternal_lands
{

	namespace
	{

		class FtLibrary
		{
			private:
				FT_Library m_library;

			public:
				FtLibrary();
				~FtLibrary() throw();

				inline FT_Library &get_library()
				{
					return m_library;
				}

				inline const FT_Library &get_library() const
				{
					return m_library;
				}

		};

		FtLibrary::FtLibrary()
		{
			FT_Error error;

			error = FT_Init_FreeType(&m_library);

			if (error != 0)
			{
				EL_THROW_EXCEPTION(InternalErrorException()
					<< errinfo_code(FT_Errors[error].code)
					<< errinfo_message(
						FT_Errors[error].message));
			}
		}

		FtLibrary::~FtLibrary() throw()
		{
			FT_Done_FreeType(m_library);
		}

		typedef boost::shared_ptr<FtLibrary> FtLibrarySharedPtr;

		class FtFace
		{
			private:
				FT_Face m_face;
				FtLibrarySharedPtr m_library;

			public:
				FtFace(const FtLibrarySharedPtr &library,
					const String &file_name);
				~FtFace() throw();

				inline FT_Face &get_face()
				{
					return m_face;
				}

				inline const FT_Face &get_face() const
				{
					return m_face;
				}
		};

		FtFace::FtFace(const FtLibrarySharedPtr &library,
			const String &file_name): m_library(library)
		{
			FT_Error error;

			error = FT_New_Face(m_library->get_library(),
				file_name.get().c_str(), 0, &m_face);

			if (error != 0)
			{
				EL_THROW_EXCEPTION(InternalErrorException()
					<< errinfo_code(FT_Errors[error].code)
					<< errinfo_message(
						FT_Errors[error].message));
			}
		}

		FtFace::~FtFace() throw()
		{
			FT_Done_Face(m_face);
		}

		typedef boost::shared_ptr<FtFace> FtFaceSharedPtr;

		void load_face(const String &file_name, const float size,
			FtLibrarySharedPtr &library, FtFaceSharedPtr &face)
		{
			size_t hres = 64;
			FT_Error error;
			FT_Matrix matrix = { (int)((1.0/hres) * 0x10000L),
				(int)((0.0)      * 0x10000L),
				(int)((0.0)      * 0x10000L),
				(int)((1.0)      * 0x10000L) };

			/* Initialize library */
			library = boost::make_shared<FtLibrary>();

			/* Load face */
			face = boost::make_shared<FtFace>(library, file_name);

			/* Select charmap */
			error = FT_Select_Charmap(face->get_face(),
				FT_ENCODING_UNICODE);

			if (error != 0)
			{
				EL_THROW_EXCEPTION(InternalErrorException()
					<< errinfo_code(FT_Errors[error].code)
					<< errinfo_message(
						FT_Errors[error].message));
			}

			/* Set char size */
			error = FT_Set_Char_Size(face->get_face(),
				static_cast<Uint32>(size * 64), 0, 72 * hres,
				72);

			if (error != 0)
			{
				EL_THROW_EXCEPTION(InternalErrorException()
					<< errinfo_code(FT_Errors[error].code)
					<< errinfo_message(
						FT_Errors[error].message));
			}

			/* Set transform matrix */
			FT_Set_Transform(face->get_face(), &matrix, 0);
		}

	}

	TextureFont::TextureFont(const AtlasSharedPtr &atlas,
		const ImageSharedPtr &image, const String &family,
		const float size, const bool bold, const bool italic):
		m_atlas(atlas), m_image(image)
	{
		m_file_name = get_font_file_name(family, size, bold, italic);
		m_size = size;
		m_bold = bold;
		m_italic = italic;

		init();
	}

	TextureFont::TextureFont(const AtlasSharedPtr &atlas,
		const ImageSharedPtr &image, const String &file_name,
		const float size): m_atlas(atlas), m_image(image)
	{
		m_file_name = file_name;
		m_size = size;
		m_bold = false;
		m_italic = false;

		init();
	}

	TextureFont::~TextureFont() throw()
	{
	}

	void TextureFont::init()
	{
		FtLibrarySharedPtr library;
		FtFaceSharedPtr face;
		FT_Size_Metrics metrics;

		m_gamma = true;
		m_hinting = true;
		m_lcd_filter = false;
		m_lcd_weights[0] = 0;
		m_lcd_weights[1] = 0;
		m_lcd_weights[2] = 255;
		m_lcd_weights[3] = 0;
		m_lcd_weights[4] = 0;

		/* Get font metrics at high resolution */
		load_face(get_file_name(), get_size() * 100.0f, library, face);

		metrics = face->get_face()->size->metrics; 
		m_ascender = (metrics.ascender >> 6) / 100.0f;
		m_descender = (metrics.descender >> 6) / 100.0f;
		m_height = (metrics.height >> 6) / 100.0f;
		m_line_gap = get_height() - get_ascender() + get_descender();

		cache_glyphs(L"abcdefghijklmnopqrstuvwxyzäöüßABCDEFGHIJKLMNOPQRSTUVWXYZ!\"§$%&/()=?+-_.:,;<>|µ@^°{[]}\\1234567890#'~²³");
	}

	String TextureFont::get_font_file_name(const String &family,
		const float size, const bool bold, const bool italic)
	{
#if	0
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

		return String(file_name);
#else	/* LINUX */
		return String();
#endif	/* LINUX */
#endif
		return String();
	}

	void TextureFont::cache_glyphs(const Utf32String &char_codes)
	{
		Utf32CharTextureGlypheMap::const_iterator kerning_it;
		Utf32CharTextureGlypheMap::const_iterator kerning_end;
		Utf32CharTextureGlypheMap::iterator it, end;
		Utf32CharFloatMap kernings;
		Utf32Char char_code;
		FtLibrarySharedPtr library;
		FtFaceSharedPtr face;
		size_t i, x, y, width, height, depth, w, h;
		FT_Error error;
		FT_GlyphSlot slot;
		FT_UInt glyph_index, kerning_index;
		FT_Vector kerning;
		glm::uvec4 uv;
		glm::vec4 color;
		glm::uvec2 offset;
		glm::vec2 advance;

		width  = m_atlas->get_width();
		height = m_atlas->get_height();
		depth  = 3;//self->atlas->depth;

		load_face(get_file_name(), get_size(), library, face);

		/**
		 * Load each glyph
		 */
		BOOST_FOREACH(const Utf32Char &char_code, char_codes)
		{
			glyph_index = FT_Get_Char_Index(face->get_face(),
				char_code);

			// WARNING: We use texture-atlas depth to guess if user wants
			//          LCD subpixel rendering
			FT_Int32 flags = FT_LOAD_RENDER;

			if (get_hinting())
			{
				flags |= FT_LOAD_NO_HINTING | FT_LOAD_NO_AUTOHINT;
			}
			else
			{
				flags |= FT_LOAD_FORCE_AUTOHINT;
			}

			if (depth == 3)
			{
				FT_Library_SetLcdFilter(library->get_library(),
					FT_LCD_FILTER_LIGHT);
				flags |= FT_LOAD_TARGET_LCD;

				if (get_lcd_filter())
				{
					FT_Library_SetLcdFilterWeights(
						library->get_library(),
						m_lcd_weights.data());
				}
			}

			error = FT_Load_Glyph(face->get_face(), glyph_index,
				flags);

			if (error != 0)
			{
				EL_THROW_EXCEPTION(InternalErrorException()
					<< errinfo_code(FT_Errors[error].code)
					<< errinfo_message(
						FT_Errors[error].message));
			}

			slot = face->get_face()->glyph;

			// We want each glyph to be separated by at least one black pixel
			// (for example for shader used in demo-subpixel.c)
			w = slot->bitmap.width / depth + 1;
			h = slot->bitmap.rows + 1;

			if (!m_atlas->get_region(w, h, offset))
			{
				return;
			}

			w = w - 1;
			h = h - 1;

			/* Gamma correction (sort of) */
			for (x = 0; x < w; ++x)
			{
				for (y = 0; y < h; ++y)
				{
					for (i = 0; i < depth; ++i)
					{
						color[i] = *(slot->bitmap.buffer + y * slot->bitmap.pitch + x);
						color[i] /= 255.0f;
						color[i] = std::pow(color[i], get_gamma());
					}

					m_image->set_pixel(x + offset.x,
						y + offset.y, 0, 0, 0, color);
				}
			}

			x = offset.x;
			y = offset.y;

			offset = glm::uvec2(slot->bitmap_left,
				slot->bitmap_top);

			uv.x = x/(float)width;
			uv.y = y/(float)height;
			uv.z = (x + w)/(float)width;
			uv.w = (y + h)/(float)height;

			/* Discard hinting to get advance */
			FT_Load_Glyph(face->get_face(), glyph_index,
				FT_LOAD_RENDER | FT_LOAD_NO_HINTING);
			slot = face->get_face()->glyph;
			advance.x = slot->advance.x / 64.0f;
			advance.y = slot->advance.y / 64.0f;

			TextureGlyphe glyph(char_code, w, h,
				glm::uvec2(slot->bitmap_left, slot->bitmap_top),
				advance, uv);
		}

		/**
		 * For each glyph couple combination, check if kerning is
		 * necessary
		 */
		end = m_glyphs.end();
		kerning_end = m_glyphs.end();

		for (it = m_glyphs.begin(); it != end; ++it)
		{
			/**
			 * Count how many kerning pairs we need
			 */
			glyph_index = FT_Get_Char_Index(face->get_face(),
				it->second.get_char_code());

			kernings.clear();

			for (kerning_it = m_glyphs.begin();
				kerning_it != kerning_end; ++kerning_it)
			{
				char_code = kerning_it->second.get_char_code();

				kerning_index = FT_Get_Char_Index(
					face->get_face(), char_code);

				FT_Get_Kerning(face->get_face(), kerning_index,
					glyph_index, FT_KERNING_UNFITTED,
					&kerning);

				std::cout << "kerning: " << kerning.x << std::endl;

				if (kerning.x == 0.0f)
				{
					continue;
				}

				kernings[char_code] = kerning.x / 4096.0f;
			}

			it->second.set_kerning(kernings);
		}
	}

	void TextureFont::write_to_stream(const glm::vec4 &color,
		const glm::vec2 &position, const Utf32String &str,
		const float spacing, const float rise, VertexStreams &streams)
	{
		Utf32CharTextureGlypheMap::const_iterator found, end;
		Utf32Char last_char_code;
		glm::vec2 pos;
		float kerning;

		pos = position;

		end = m_glyphs.end();
		last_char_code = L'\0';

		BOOST_FOREACH(const Utf32Char char_code, str)
		{
			found = m_glyphs.find(char_code);

			if (found == end)
			{
				continue;
			}

			kerning = found->second.get_kerning(last_char_code);

			found->second.write_to_stream(color, kerning, spacing,
				rise, streams, pos);

			last_char_code = found->second.get_char_code();
		}
	}

	glm::vec2 TextureFont::get_size(const Utf32String &str,
		const float spacing, const float rise)
	{
		Utf32CharTextureGlypheMap::const_iterator found, end;
		Utf32Char last_char_code;
		glm::vec2 size;
		float kerning;

		end = m_glyphs.end();
		last_char_code = L'\0';

		BOOST_FOREACH(const Utf32Char char_code, str)
		{
			found = m_glyphs.find(char_code);

			if (found == end)
			{
				continue;
			}

			kerning = found->second.get_kerning(last_char_code);

			size += found->second.get_size(kerning, spacing, rise);

			last_char_code = found->second.get_char_code();
		}

		return size;
	}

}
