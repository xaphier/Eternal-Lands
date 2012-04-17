/****************************************************************************
 *            texturefont.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "texturefont.hpp"
#include "image.hpp"
#include "textureglyphe.hpp"
#include "atlas.hpp"
#include "exceptions.hpp"
#ifdef OSX
#include <FreeType/ft2build.h>
#else
#include <ft2build.h>
#endif
#include FT_FREETYPE_H 
#include FT_LCD_FILTER_H

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

		const Uint32 padd = 1;

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
					<< boost::errinfo_errno(
						FT_Errors[error].code)
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
					<< boost::errinfo_errno(
						FT_Errors[error].code)
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
					<< boost::errinfo_errno(
						FT_Errors[error].code)
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
					<< boost::errinfo_errno(
						FT_Errors[error].code)
					<< errinfo_message(
						FT_Errors[error].message));
			}

			/* Set transform matrix */
			FT_Set_Transform(face->get_face(), &matrix, 0);
		}

	}

	TextureFont::TextureFont(const AtlasSharedPtr &atlas,
		const DoubleSharedArray &data,
		const FileSystemSharedPtr &file_system,
		const Utf32String &char_codes, const String &file_name,
		const float size)
	{
		m_file_name = file_name;
		m_size = size;

		init(atlas, data, char_codes);
	}

	TextureFont::~TextureFont() throw()
	{
	}

	void TextureFont::init(const AtlasSharedPtr &atlas,
		const DoubleSharedArray &data, const Utf32String &char_codes)
	{
		FtLibrarySharedPtr library;
		FtFaceSharedPtr face;
		FT_Size_Metrics metrics;

		m_hinting = false;

		/* Get font metrics at high resolution */
		load_face(get_file_name(), get_size() * 100.0f, library, face);

		metrics = face->get_face()->size->metrics; 
		m_ascender = (metrics.ascender >> 6) / 100.0f;
		m_descender = (metrics.descender >> 6) / 100.0f;
		m_height = (metrics.height >> 6) / 100.0f;
		m_line_gap = get_height() - get_ascender() + get_descender();

		cache_glyphs(atlas, data, char_codes);
	}

	void TextureFont::cache_glyphs(const AtlasSharedPtr &atlas,
		const DoubleSharedArray &data, const Utf32String &char_codes)
	{
		Utf32CharTextureGlypheMap::const_iterator kerning_it;
		Utf32CharTextureGlypheMap::const_iterator kerning_end;
		Utf32CharTextureGlypheMap::iterator it, end;
		Utf32CharFloatMap kernings;
		Utf32Char char_code;
		FtLibrarySharedPtr library;
		FtFaceSharedPtr face;
		FT_Error error;
		FT_GlyphSlot slot;
		FT_UInt glyph_index, kerning_index;
		FT_Vector kerning;
		glm::vec4 uv;
		glm::uvec2 offset;
		glm::vec2 advance;
		Uint32 x, y, width, height, w, h;

		width  = atlas->get_width();
		height = atlas->get_height();

		load_face(get_file_name(), get_size(), library, face);

		/**
		 * Load each glyph
		 */
		BOOST_FOREACH(const Utf32Char &char_code, char_codes)
		{
			glyph_index = FT_Get_Char_Index(face->get_face(),
				char_code);

			FT_Int32 flags = FT_LOAD_RENDER;

			if (!get_hinting())
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
					<< boost::errinfo_errno(
						FT_Errors[error].code)
					<< errinfo_message(
						FT_Errors[error].message));
			}

			slot = face->get_face()->glyph;

			w = slot->bitmap.width;
			h = slot->bitmap.rows;

			if (!atlas->get_region(w + padd * 2, h + padd * 2,
				offset))
			{
				return;
			}

			offset += padd;

			for (x = 0; x < w; ++x)
			{
				for (y = 0; y < h; ++y)
				{
					data[x + offset.x +
						(y + offset.y) * width] =
						*(slot->bitmap.buffer + y * slot->bitmap.pitch + x);
				}
			}

			x = offset.x;
			y = offset.y;

			uv.x = static_cast<float>(x + 0.5f) / width;
			uv.y = static_cast<float>(y + 0.5f) / height;
			uv.z = static_cast<float>(x + w - 0.5f) / width;
			uv.w = static_cast<float>(y + h - 0.5f) / height;

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

	Uint32 TextureFont::write_to_stream(const TextAttribute &attribute,
		const VertexStreamsSharedPtr &streams,
		const Utf32Char last_char_code, const Utf32Char char_code,
		glm::vec2 &position) const
	{
		Utf32CharTextureGlypheMap::const_iterator found, end;
		float kerning;

		end = m_glyphs.end();
		found = m_glyphs.find(char_code);

		if (found == end)
		{
			found = m_glyphs.begin();
		}

		if (found == end)
		{
			return 0;
		}

		kerning = found->second.get_kerning(last_char_code);

		found->second.write_to_stream(attribute,
			get_height() + get_line_gap(), kerning, streams,
			position);

		return 1;
	}

	Uint32 TextureFont::advance(const TextAttribute &attribute,
		const Utf32Char last_char_code, const Utf32Char char_code,
		glm::vec2 &position) const
	{
		Utf32CharTextureGlypheMap::const_iterator found, end;
		float kerning;

		end = m_glyphs.end();
		found = m_glyphs.find(char_code);

		if (found == end)
		{
			found = m_glyphs.begin();
		}

		if (found == end)
		{
			return 0;
		}

		kerning = found->second.get_kerning(last_char_code);

		found->second.advance(attribute, kerning, position);

		return 1;
	}


	float TextureFont::get_width(const TextAttribute &attribute,
		const Utf32Char last_char_code, const Utf32Char char_code)
		const
	{
		Utf32CharTextureGlypheMap::const_iterator found;
		float kerning;

		found = m_glyphs.find(char_code);

		if (found == m_glyphs.end())
		{
			return 0.0f;
		}

		kerning = found->second.get_kerning(last_char_code);

		return found->second.get_size(attribute, kerning).x;
	}

}
