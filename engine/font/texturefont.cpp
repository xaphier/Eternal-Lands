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
				EL_THROW_EXCEPTION(FileNotFoundException()
					<< errinfo_code(FT_Errors[error].code)
					<< errinfo_message(
						FT_Errors[error].message)
					<< boost::errinfo_file_name(file_name));
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
		const ImageSharedPtr &image,
		const FileSystemSharedPtr &file_system, const String &file_name,
		const float size): m_atlas(atlas), m_image(image)
	{
		m_file_name = file_name;
		m_size = size;

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

		m_hinting = true;

		/* Get font metrics at high resolution */
		load_face(get_file_name(), get_size() * 100.0f, library, face);

		metrics = face->get_face()->size->metrics; 
		m_ascender = (metrics.ascender >> 6) / 100.0f;
		m_descender = (metrics.descender >> 6) / 100.0f;
		m_height = (metrics.height >> 6) / 100.0f;
		m_line_gap = get_height() - get_ascender() + get_descender();

		cache_glyphs(L"abcdefghijklmnopqrstuvwxyzäöüßABCDEFGHIJKLMNOP"
			"QRSTUVWXYZ!\"§$%&/()=?+-_.:,;<>|µ@^°{[]}\\1234567890"
			"#'~²³");
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
		Uint32 x, y, width, height, w, h;
		FT_Error error;
		FT_GlyphSlot slot;
		FT_UInt glyph_index, kerning_index;
		FT_Vector kerning;
		glm::vec4 uv;
		glm::vec4 color;
		glm::uvec2 offset;
		glm::vec2 advance;

		width  = m_atlas->get_width();
		height = m_atlas->get_height();

		load_face(get_file_name(), get_size(), library, face);

		/**
		 * Load each glyph
		 */
		BOOST_FOREACH(const Utf32Char &char_code, char_codes)
		{
			glyph_index = FT_Get_Char_Index(face->get_face(),
				char_code);

			FT_Int32 flags = FT_LOAD_RENDER;

			if (get_hinting())
			{
				flags |= FT_LOAD_NO_HINTING | FT_LOAD_NO_AUTOHINT;
			}
			else
			{
				flags |= FT_LOAD_FORCE_AUTOHINT;
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

			w = slot->bitmap.width + 1;
			h = slot->bitmap.rows + 1;

			if (!m_atlas->get_region(w, h, offset))
			{
				return;
			}

			w = w - 1;
			h = h - 1;

			for (x = 0; x < w; ++x)
			{
				for (y = 0; y < h; ++y)
				{
					color.x = *(slot->bitmap.buffer + y * slot->bitmap.pitch + x);
					color.x /= 255.0f;

					m_image->set_pixel(x + offset.x,
						y + offset.y, 0, 0, 0, color);
				}
			}

			x = offset.x;
			y = offset.y;

			offset = glm::uvec2(slot->bitmap_left,
				slot->bitmap_top);

			uv.x = static_cast<float>(x) / width;
			uv.y = static_cast<float>(y) / height;
			uv.z = static_cast<float>(x + w) / width;
			uv.w = static_cast<float>(y + h) / height;

			/* Discard hinting to get advance */
			FT_Load_Glyph(face->get_face(), glyph_index,
				FT_LOAD_RENDER | FT_LOAD_NO_HINTING);
			slot = face->get_face()->glyph;
			advance.x = slot->advance.x / 64.0f;
			advance.y = slot->advance.y / 64.0f;

			m_glyphs.insert(Utf32CharTextureGlyphePair(char_code,
				TextureGlyphe(char_code, w, h,
				glm::vec2(slot->bitmap_left, slot->bitmap_top),
				advance, uv)));
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

				if (kerning.x == 0.0f)
				{
					continue;
				}

				kernings[char_code] = kerning.x / 4096.0f;
			}

			it->second.set_kerning(kernings);
		}
	}

	Uint32 TextureFont::write_to_stream(const Utf32String &str,
		const VertexStreamsSharedPtr &streams,
		const glm::vec2 &position, const glm::vec4 &color,
		const Uint32 max_lines,
		const float spacing, const float rise) const
	{
		Utf32CharTextureGlypheMap::const_iterator found, end;
		Utf32Char last_char_code;
		glm::vec2 pos;
		float kerning;
		Uint32 count, current_lines;

		pos = position;
		end = m_glyphs.end();
		last_char_code = L'\0';
		count = 0;
		current_lines = 0;

		BOOST_FOREACH(const Utf32Char char_code, str)
		{
			if ((char_code == L'\n') || (char_code == L'\r'))
			{
				pos.y += get_height();
				pos.x = position.x;

				current_lines++;

				if (current_lines > max_lines)
				{
					break;
				}

				continue;
			}

			found = m_glyphs.find(char_code);

			if (found == end)
			{
				continue;
			}

			kerning = found->second.get_kerning(last_char_code);
/*
			if (pos.x + displayed_font_x_size - position.x >= max_width)
			{
				pos.y += get_height();
				pos.x = position.x;

				current_lines++;

				if (current_lines > max_lines)
				{
					break;
				}
			}
*/
			found->second.write_to_stream(color, kerning, spacing,
				rise, streams, pos);

			last_char_code = found->second.get_char_code();
			count++;
		}

		return count;
	}

	glm::vec2 TextureFont::get_size(const Utf32String &str,
		const float spacing, const float rise) const
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
