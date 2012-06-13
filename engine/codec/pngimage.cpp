/****************************************************************************
 *            pngimage.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "pngimage.hpp"
#include "exceptions.hpp"
#include "logging.hpp"
#include "reader.hpp"
#include "writer.hpp"
#include "image.hpp"
#include <png.h>

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
			LOG_WARNING(lt_png_image, UTF8("PNG: %1%"),
				warning_message);
		}

		void png_read(png_structp png_ptr, png_bytep buffer,
			png_size_t size)
		{
			static_cast<Reader*>(png_get_io_ptr(png_ptr))->read(
				buffer, size);
		}

		void png_write(png_structp png_ptr, png_bytep buffer,
			png_size_t size)
		{
			static_cast<Writer*>(png_get_io_ptr(png_ptr))->write(
				buffer, size);
		}

		void png_flush(png_structp png_ptr)
		{
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
					const bool rg_formats) const;

			public:
				PngDecompress();
				~PngDecompress() noexcept;

				ImageSharedPtr get_image(
					const ReaderSharedPtr &reader,
					const bool rg_formats);

				void get_image_information(
					const ReaderSharedPtr &reader,
					const bool rg_formats,
					TextureFormatType &texture_format,
					glm::uvec3 &sizes, Uint16 &mipmaps);

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

		PngDecompress::~PngDecompress() noexcept
		{
			if (m_png_ptr)
			{
				png_destroy_read_struct(&m_png_ptr,
					&m_info_ptr, 0);
			}
		}

		ImageSharedPtr PngDecompress::get_image(
			const ReaderSharedPtr &reader, const bool rg_formats)
		{
			ImageSharedPtr image;
			boost::scoped_array<png_bytep> row_pointers;
			glm::uvec3 sizes;
			Uint32 i, color_type, bit_depth;
			TextureFormatType texture_format;

			/* for proper error handling */
			if (setjmp(png_jmpbuf(m_png_ptr)))
			{
				EL_THROW_EXCEPTION(PngErrorException()
					<< boost::errinfo_file_name(
						reader->get_name())
					<< errinfo_message(m_error_buffer));
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
				bit_depth, rg_formats);

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

			for (i = 0; i < image->get_height(); ++i)
			{
				row_pointers[i] = static_cast<png_byte*>(
					image->get_buffer()->get_ptr()) +
					png_get_rowbytes(m_png_ptr, m_info_ptr)
					* i;
			}

			png_read_image(m_png_ptr, row_pointers.get());

			return image;
		}

		void PngDecompress::get_image_information(
			const ReaderSharedPtr &reader, const bool rg_formats,
			TextureFormatType &texture_format, glm::uvec3 &sizes,
			Uint16 &mipmaps)
		{
			Uint32 color_type, bit_depth;

			/* for proper error handling */
			if (setjmp(png_jmpbuf(m_png_ptr)))
			{
				EL_THROW_EXCEPTION(PngErrorException()
					<< boost::errinfo_file_name(
						reader->get_name())
					<< errinfo_message(m_error_buffer));
			}

			png_set_read_fn(m_png_ptr, reader.get(), png_read);

			png_read_info(m_png_ptr, m_info_ptr);

			png_set_expand(m_png_ptr);

			png_read_update_info(m_png_ptr, m_info_ptr);

			color_type = png_get_color_type(m_png_ptr, m_info_ptr);
			bit_depth = png_get_bit_depth(m_png_ptr, m_info_ptr);

			texture_format = get_texture_format(color_type,
				bit_depth, rg_formats);

			sizes[0] = png_get_image_width(m_png_ptr, m_info_ptr);
			sizes[1] = png_get_image_height(m_png_ptr, m_info_ptr);
			sizes[2] = 1;

			mipmaps = 0;
		}

		TextureFormatType PngDecompress::get_texture_format(
			const Uint32 color_type, const Uint32 bit_depth,
			const bool rg_formats) const
		{
			if ((bit_depth != 8) && (bit_depth != 16))
			{
				EL_THROW_EXCEPTION(PngUnkownFormatException());
			}

			switch (color_type)
			{
				case PNG_COLOR_TYPE_GRAY:
					if (bit_depth <= 8)
					{
						if (rg_formats)
						{
							return tft_r8;
						}

						return tft_l8;
					}

					if (rg_formats)
					{
						return tft_r16;
					}

					return tft_l16;
				case PNG_COLOR_TYPE_GRAY_ALPHA:
					if (bit_depth <= 8)
					{
						if (rg_formats)
						{
							return tft_rg8;
						}

						return tft_la8;
					}

					if (rg_formats)
					{
						return tft_rg16;
					}

					return tft_la16;
				case PNG_COLOR_TYPE_RGB:
					if (bit_depth <= 8)
					{
						return tft_rgb8;
					}

					return tft_rgb16;
				case PNG_COLOR_TYPE_RGB_ALPHA:
					if (bit_depth <= 8)
					{
						return tft_rgba8;
					}

					return tft_rgba16;
			}

			EL_THROW_EXCEPTION(PngUnkownFormatException());
		}


		class PngCompress
		{
			private:
				WriterSharedPtr m_writer;
				png_structp m_png_ptr;
				png_infop m_info_ptr;

				static Uint32 get_png_color_type(
					const ImageSharedPtr &image);
				static Uint32 get_png_bit_depth(
					const ImageSharedPtr &image);

			public:
				PngCompress(const WriterSharedPtr &writer);
				~PngCompress();
				void set_image(const ImageSharedPtr &image);
				static bool can_save(
					const ImageSharedPtr &image);

		};

		PngCompress::PngCompress(const WriterSharedPtr &writer):
			m_writer(writer)
		{
			/* initialize stuff */
			m_png_ptr = png_create_write_struct(
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

			png_set_write_fn(m_png_ptr, m_writer.get(), png_write,
				png_flush);
		}

		PngCompress::~PngCompress()
		{
			if (m_png_ptr)
			{
				png_destroy_write_struct(&m_png_ptr,
					&m_info_ptr);
			}
		}

		void PngCompress::set_image(const ImageSharedPtr &image)
		{
			boost::scoped_array<png_bytep> row_pointers;
			Uint32 bit_depth, color_type, i;

			if (!can_save(image))
			{
				EL_THROW_EXCEPTION(
					PngFormatNotSupportedException()
					<< boost::errinfo_file_name(
						image->get_name()));
			}

			color_type = get_png_color_type(image);
			bit_depth = get_png_bit_depth(image);

			/* write header */
			png_set_IHDR(m_png_ptr, m_info_ptr, image->get_width(),
				image->get_height(), bit_depth, color_type,
				PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE,
				PNG_FILTER_TYPE_BASE);

			if (image->get_sRGB())
			{
				png_set_sRGB_gAMA_and_cHRM(m_png_ptr,
					m_info_ptr, PNG_sRGB_INTENT_ABSOLUTE);
			}

			png_write_info(m_png_ptr, m_info_ptr);

#ifndef BOOST_BIG_ENDIAN
			if (bit_depth == 16)
			{
				png_set_swap(m_png_ptr);
			}
#endif

			/* write bytes */
			row_pointers.reset(new png_bytep[image->get_height()]);

			for (i = 0; i < image->get_height(); i++)
			{
				row_pointers[i] = static_cast<png_byte*>(
					const_cast<void*>(image->get_pixel_data(
						0, i, 0, 0, 0)));
			}

			png_write_image(m_png_ptr, row_pointers.get());

			/* end write */
			png_write_end(m_png_ptr, m_info_ptr);
		}

		bool PngCompress::can_save(const ImageSharedPtr &image)
		{
			if ((image->get_type() != GL_UNSIGNED_BYTE) &&
				(image->get_type() != GL_UNSIGNED_SHORT))
			{
				return false;
			}

			switch (image->get_format())
			{
				case GL_LUMINANCE:
				case GL_LUMINANCE8:
				case GL_SLUMINANCE:
				case GL_SLUMINANCE8:
				case GL_LUMINANCE_ALPHA:
				case GL_LUMINANCE8_ALPHA8:
				case GL_SLUMINANCE_ALPHA:
				case GL_SLUMINANCE8_ALPHA8:
				case GL_RGB:
				case GL_RGB8:
				case GL_SRGB:
				case GL_SRGB8:
				case GL_RGBA:
				case GL_RGBA8:
				case GL_SRGB_ALPHA:
				case GL_SRGB8_ALPHA8:
					return true;
				default:
					return false;
			}
		}

		Uint32 PngCompress::get_png_color_type(
			const ImageSharedPtr &image)
		{
			switch (image->get_format())
			{
				case GL_LUMINANCE:
				case GL_LUMINANCE8:
				case GL_SLUMINANCE:
				case GL_SLUMINANCE8:
					return PNG_COLOR_TYPE_GRAY;
				case GL_LUMINANCE_ALPHA:
				case GL_LUMINANCE8_ALPHA8:
				case GL_SLUMINANCE_ALPHA:
				case GL_SLUMINANCE8_ALPHA8:
					return PNG_COLOR_TYPE_GRAY_ALPHA;
				case GL_RGB:
				case GL_RGB8:
				case GL_SRGB:
				case GL_SRGB8:
					return PNG_COLOR_TYPE_RGB;
				case GL_RGBA:
				case GL_RGBA8:
				case GL_SRGB_ALPHA:
				case GL_SRGB8_ALPHA8:
					return PNG_COLOR_TYPE_RGB_ALPHA;
			}

			EL_THROW_EXCEPTION(PngFormatNotSupportedException());
		}

		Uint32 PngCompress::get_png_bit_depth(
			const ImageSharedPtr &image)
		{
			switch (image->get_type())
			{
				case GL_UNSIGNED_BYTE:
					return 8;
				case GL_UNSIGNED_SHORT:
					return 16;
			}

			EL_THROW_EXCEPTION(PngFormatNotSupportedException());
		}

	}

	ImageSharedPtr PngImage::load_image(const ReaderSharedPtr &reader,
		const bool rg_formats)
	{
		PngDecompress png_decompress;

		return png_decompress.get_image(reader, rg_formats);
	}

	void PngImage::get_image_information(const ReaderSharedPtr &reader,
		const bool rg_formats, TextureFormatType &texture_format,
		glm::uvec3 &sizes, Uint16 &mipmaps)
	{
		PngDecompress png_decompress;

		png_decompress.get_image_information(reader, rg_formats,
			texture_format, sizes, mipmaps);
	}

	bool PngImage::check_load(const Uint8Array32 &id)
	{
		return (id[0] == 0x89) && (id[1] == 0x50) && (id[2] == 0x4E) &&
			(id[3] == 0x47) && (id[4] == 0x0D) && (id[5] == 0x0A) &&
			(id[6] == 0x1A) && (id[7] == 0x0A);
	}

	String PngImage::get_image_str()
	{
		return String(UTF8("png-image"));
	}

	void PngImage::save_image(const ImageSharedPtr &image,
		const WriterSharedPtr &writer)
	{
		try
		{
			PngCompress png_compress(writer);

			png_compress.set_image(image);
		}
		catch (boost::exception &exception)
		{
			exception << errinfo_name(image->get_name());
			throw;
		}
	}

	bool PngImage::can_save(const ImageSharedPtr &image)
	{
		return PngCompress::can_save(image);
	}

}
