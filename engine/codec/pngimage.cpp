/****************************************************************************
 *            pngimage.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "pngimage.hpp"
#include "exceptions.hpp"
#include "logging.hpp"
#include "memorybuffer.hpp"
#include "reader.hpp"
#include "image.hpp"
#include <png.h>
#include "utf.hpp"

namespace eternal_lands
{

	namespace
	{

		class PngErrorException: public virtual IoErrorException {};
		class PngUnkownFormatException:
			public virtual UnkownFormatException {};
		class PngFormatNotSupportedException:
			public virtual NotImplementedException {};

		void png_error(png_structp png_ptr,
			png_const_charp error_message)
		{
			png_voidp error_ptr;
			size_t size;

			error_ptr = png_get_error_ptr(png_ptr);

			if (error_message != 0)
			{
				size = strlen(error_message);

				memcpy(error_ptr, error_message, size);
			}
		}

		void png_warning(png_structp png_ptr,
			png_const_charp warning_message)
		{
			LOG_WARNING(L"PNG: %1%", warning_message);
		}

		void png_read(png_structp png_ptr, png_bytep buffer,
			png_size_t size)
		{
			static_cast<Reader*>(png_get_io_ptr(png_ptr))->read(
				buffer, size);
		}

		class PngDecompress
		{
			private:
				char m_error_buffer[256];
				png_structp m_png_ptr;
				png_infop m_info_ptr;

				TextureFormatType get_texture_format(
					const Uint32 color_type,
					const Uint32 bit_depth,
					const bool sRGB) const;

			public:
				PngDecompress();
				~PngDecompress() throw();

				ImageSharedPtr get_image(
					const ReaderSharedPtr &reader);

				void get_image_information(
					const ReaderSharedPtr &reader,
					TextureFormatType &texture_format,
					Uint32Array3 &sizes, Uint16 &mipmaps);

		};

		PngDecompress::PngDecompress()
		{
			memset(m_error_buffer, 0, sizeof(m_error_buffer));

			/* initialize stuff */
			m_png_ptr = png_create_read_struct(
				PNG_LIBPNG_VER_STRING, 0, png_error,
				png_warning);

			if (m_png_ptr == 0)
			{
				EL_THROW_EXCEPTION(PngErrorException());
			}

			m_info_ptr = png_create_info_struct(m_png_ptr);

			if (m_info_ptr == 0)
			{
				EL_THROW_EXCEPTION(PngErrorException());
			}
		}

		PngDecompress::~PngDecompress() throw()
		{
			if (m_png_ptr)
			{
				png_destroy_read_struct(&m_png_ptr,
					&m_info_ptr, 0);
			}
		}

		ImageSharedPtr PngDecompress::get_image(
			const ReaderSharedPtr &reader)
		{
			ImageSharedPtr image;
			boost::scoped_array<png_bytep> row_pointers;
			Uint32Array3 sizes;
			Uint32 i, color_type, bit_depth;
			TextureFormatType texture_format;

			/* for proper error handling */
			if (setjmp(png_jmpbuf(m_png_ptr)))
			{
				EL_THROW_EXCEPTION(PngErrorException()
					<< boost::errinfo_file_name(
						string_to_utf8(
							reader->get_name()))
					<< errinfo_message(utf8_to_string(
						m_error_buffer)));
			}

			png_set_read_fn(m_png_ptr, reader.get(), png_read);

			png_read_info(m_png_ptr, m_info_ptr);

			png_set_expand(m_png_ptr);

			png_read_update_info(m_png_ptr, m_info_ptr);

			color_type = png_get_color_type(m_png_ptr, m_info_ptr);
			bit_depth = png_get_bit_depth(m_png_ptr, m_info_ptr);
			sizes[0] = png_get_image_width(m_png_ptr, m_info_ptr);
			sizes[1] = png_get_image_height(m_png_ptr, m_info_ptr);
			sizes[2] = 1;

			texture_format = get_texture_format(color_type,
				bit_depth, png_get_valid(m_png_ptr, m_info_ptr,
					PNG_INFO_sRGB));

			image = boost::make_shared<Image>(reader->get_name(),
				false, texture_format, sizes, 0);

#ifndef BOOST_BIG_ENDIAN
			if (bit_depth == 16)
			{
				png_set_swap(m_png_ptr);
			}
#endif

			/* read file */
			row_pointers.reset(new png_bytep[image->get_height()]);

			for (i = 0; i < image->get_height(); i++)
			{
				row_pointers[i] = static_cast<png_byte*>(
					image->get_buffer().get_ptr()) +
					png_get_rowbytes(m_png_ptr, m_info_ptr)
					* i;
			}

			png_read_image(m_png_ptr, row_pointers.get());

			return image;
		}

		void PngDecompress::get_image_information(
			const ReaderSharedPtr &reader,
			TextureFormatType &texture_format, Uint32Array3 &sizes,
			Uint16 &mipmaps)
		{
			Uint32 color_type, bit_depth;

			/* for proper error handling */
			if (setjmp(png_jmpbuf(m_png_ptr)))
			{
				EL_THROW_EXCEPTION(PngErrorException()
					<< boost::errinfo_file_name(
						string_to_utf8(
							reader->get_name()))
					<< errinfo_message(utf8_to_string(
						m_error_buffer)));
			}

			png_set_read_fn(m_png_ptr, reader.get(), png_read);

			png_read_info(m_png_ptr, m_info_ptr);

			png_set_expand(m_png_ptr);

			png_read_update_info(m_png_ptr, m_info_ptr);

			color_type = png_get_color_type(m_png_ptr, m_info_ptr);
			bit_depth = png_get_bit_depth(m_png_ptr, m_info_ptr);

			texture_format = get_texture_format(color_type,
				bit_depth, png_get_valid(m_png_ptr, m_info_ptr,
					PNG_INFO_sRGB));

			sizes[0] = png_get_image_width(m_png_ptr, m_info_ptr);
			sizes[1] = png_get_image_height(m_png_ptr, m_info_ptr);
			sizes[2] = 1;

			mipmaps = 0;
		}

		TextureFormatType PngDecompress::get_texture_format(
			const Uint32 color_type, const Uint32 bit_depth,
			const bool sRGB) const
		{
			if ((bit_depth != 8) && (bit_depth != 16))
			{
				EL_THROW_EXCEPTION(PngUnkownFormatException());
			}

			switch (color_type)
			{
				case PNG_COLOR_TYPE_GRAY:
					if (bit_depth == 8)
					{
						return tft_r8;
					}
					else
					{
						return tft_r16;
					}
				case PNG_COLOR_TYPE_GRAY_ALPHA:
					if (bit_depth == 8)
					{
						return tft_rg8;
					}
					else
					{
						return tft_rg16;
					}
				case PNG_COLOR_TYPE_RGB:
					if (bit_depth == 8)
					{
						if (sRGB)
						{
							return tft_srgb8;
						}
						else
						{
							return tft_rgb8;
						}
					}
					else
					{
						return tft_rgb16;
					}
				case PNG_COLOR_TYPE_RGB_ALPHA:
					if (bit_depth == 8)
					{
						if (sRGB)
						{
							return tft_srgb8_a8;
						}
						else
						{
							return tft_rgba8;
						}
					}
					else
					{
						return tft_rgba16;
					}
			}

			EL_THROW_EXCEPTION(PngUnkownFormatException());
		}

	}

	ImageSharedPtr PngImage::load_image(const ReaderSharedPtr &reader)
	{
		PngDecompress png_decompress;

		return png_decompress.get_image(reader);
	}

	void PngImage::get_image_information(const ReaderSharedPtr &reader,
		TextureFormatType &texture_format, Uint32Array3 &sizes,
		Uint16 &mipmaps)
	{
		PngDecompress png_decompress;

		png_decompress.get_image_information(reader, texture_format,
			sizes, mipmaps);
	}

	bool PngImage::check_load(const Uint8Array32 &id)
	{
		return (id[0] == 0x89) && (id[1] == 0x50) && (id[2] == 0x4E) &&
			(id[3] == 0x47) && (id[4] == 0x0D) && (id[5] == 0x0A) &&
			(id[6] == 0x1A) && (id[7] == 0x0A);
	}

	String PngImage::get_image_str()
	{
		return String(L"png-image");
	}

}
