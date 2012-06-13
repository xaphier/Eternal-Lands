/****************************************************************************
 *            jpegimage.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "jpegimage.hpp"
#include "exceptions.hpp"
#include "logging.hpp"
#include "readwritememory.hpp"
#include "reader.hpp"
#include "writer.hpp"
#include "image.hpp"

extern "C"
{
	#include <jpeglib.h>
	#include <setjmp.h>
}

namespace eternal_lands
{

	namespace
	{

		class JpegErrorException: public virtual IoErrorException {};
		class JpegUnkownFormatException:
			public virtual UnkownFormatException {};
		class JpegFormatNotSupportedException:
			public virtual NotImplementedException {};

		struct el_error_mgr
		{
			struct jpeg_error_mgr m_errmgr;	/* "public" fields */
			jmp_buf m_jmp_buffer;		/* for return to caller */
			char m_buffer[JMSG_LENGTH_MAX];
		};

#if	JPEG_LIB_VERSION < 80
		void init_source(j_decompress_ptr cinfo)
		{
		}

		boolean fill_input_buffer(j_decompress_ptr cinfo)
		{
			return TRUE;
		}

		void skip_input_data(j_decompress_ptr cinfo, long num_bytes)
		{
			struct jpeg_source_mgr* src =
				(struct jpeg_source_mgr*)cinfo->src;

			if (num_bytes > 0)
			{
				src->next_input_byte += (size_t) num_bytes;
				src->bytes_in_buffer -= (size_t) num_bytes;
			}
		}

		void term_source(j_decompress_ptr cinfo)
		{
		}

		void jpeg_mem_src(j_decompress_ptr cinfo, void* buffer,
			long nbytes)
		{
			struct jpeg_source_mgr* src;

			if (cinfo->src == NULL)
			{
				/* first time for this JPEG object? */

				cinfo->src = (struct jpeg_source_mgr *)
					(*cinfo->mem->alloc_small)
					((j_common_ptr)cinfo, JPOOL_PERMANENT,
					sizeof(jpeg_source_mgr));
			}

			src = (jpeg_source_mgr*) cinfo->src;
			src->init_source = init_source;
			src->fill_input_buffer = fill_input_buffer;
			src->skip_input_data = skip_input_data;
			/* use default method */
			src->resync_to_restart = jpeg_resync_to_restart;
			src->term_source = term_source;
			src->bytes_in_buffer = nbytes;
			src->next_input_byte = (JOCTET*)buffer;
		}
#endif

		void el_error_exit(j_common_ptr cinfo)
		{
			/* cinfo->err really points to a el_error_mgr struct */
			el_error_mgr* el_err = (el_error_mgr*)cinfo->err;

			/* create the message */
			(*cinfo->err->format_message)(cinfo, el_err->m_buffer);

			longjmp(el_err->m_jmp_buffer, 1);
		}

		void el_output_message(j_common_ptr cinfo)
		{
			/* cinfo->err really points to a el_error_mgr struct */
			el_error_mgr* el_err = (el_error_mgr*)cinfo->err;

			/* create the message */
			(*cinfo->err->format_message)(cinfo, el_err->m_buffer);

			// send it to user's message proc
			LOG_WARNING(lt_jpeg_image, UTF8("JPEG: %1%"),
				el_err->m_buffer);
		}

		class JpegDecompress
		{
			private:
				jpeg_decompress_struct m_cinfo;
				el_error_mgr m_jerr;

				static TextureFormatType get_texture_format(
					const J_COLOR_SPACE color_space,
					const bool rg_formats);
				static J_COLOR_SPACE get_color_space(
					const J_COLOR_SPACE color_space);

			public:
				JpegDecompress();
				~JpegDecompress();
				ImageSharedPtr get_image(
					const ReaderSharedPtr &reader,
					const bool rg_formats);
				void get_image_information(
					const ReaderSharedPtr &reader,
					const bool rg_formats,
					TextureFormatType &texture_format,
					glm::uvec3 &sizes, Uint16 &mipmaps);

		};

		JpegDecompress::JpegDecompress()
		{
			memset(m_jerr.m_buffer, 0, sizeof(m_jerr.m_buffer));

			m_cinfo.err = jpeg_std_error(&m_jerr.m_errmgr);
			m_jerr.m_errmgr.error_exit = el_error_exit;
			m_jerr.m_errmgr.output_message = el_output_message;

			jpeg_create_decompress(&m_cinfo);
		}

		JpegDecompress::~JpegDecompress()
		{
			jpeg_destroy_decompress(&m_cinfo);
		}

		ImageSharedPtr JpegDecompress::get_image(
			const ReaderSharedPtr &reader, const bool rg_formats)
		{
			ImageSharedPtr image;
			JSAMPROW rowptr[1];
			ReadWriteMemory buffer;
			glm::uvec3 sizes;
			TextureFormatType texture_format;

			reader->read(buffer);

			if (setjmp(m_jerr.m_jmp_buffer))
			{
				EL_THROW_EXCEPTION(JpegErrorException()
					<< boost::errinfo_file_name(
						reader->get_name())
					<< errinfo_message(
						m_jerr.m_buffer));
			}

			jpeg_mem_src(&m_cinfo,
				static_cast<Uint8*>(buffer.get_ptr()),
				buffer.get_size());

			jpeg_read_header(&m_cinfo, true);

			m_cinfo.out_color_space = get_color_space(
				m_cinfo.jpeg_color_space);
			m_cinfo.quantize_colors = FALSE;

			jpeg_calc_output_dimensions(&m_cinfo);

			texture_format = get_texture_format(
				m_cinfo.jpeg_color_space, rg_formats);

			sizes[0] = m_cinfo.output_width;
			sizes[1] = m_cinfo.output_height;
			sizes[2] = 1;

			image = boost::make_shared<Image>(reader->get_name(),
				false, texture_format, sizes, 0);

			jpeg_start_decompress(&m_cinfo);

			while (m_cinfo.output_scanline < m_cinfo.output_height)
			{
				rowptr[0] = static_cast<JSAMPROW>(
					image->get_buffer()->get_ptr()) +
					m_cinfo.output_scanline *
					m_cinfo.output_width *
					m_cinfo.out_color_components;
				jpeg_read_scanlines(&m_cinfo, rowptr, 1);
			}

			jpeg_finish_decompress(&m_cinfo);

			return image;
		}

		TextureFormatType JpegDecompress::get_texture_format(
			const J_COLOR_SPACE color_space,
			const bool rg_formats)
		{
			switch (color_space)
			{
				case JCS_GRAYSCALE:
					if (rg_formats)
					{
						return tft_r8;
					}

					return tft_l8;
				case JCS_RGB:
				case JCS_YCbCr:
				case JCS_CMYK:
				case JCS_YCCK:
					return tft_rgb8;
				case JCS_UNKNOWN:
				default:
					EL_THROW_EXCEPTION(
						JpegUnkownFormatException());
			}
		}

		J_COLOR_SPACE JpegDecompress::get_color_space(
			const J_COLOR_SPACE color_space)
		{
			switch (color_space)
			{
				case JCS_GRAYSCALE:
					return JCS_GRAYSCALE;
				case JCS_RGB:
				case JCS_YCbCr:
				case JCS_CMYK:
				case JCS_YCCK:
					return JCS_RGB;
				case JCS_UNKNOWN:
				default:
					EL_THROW_EXCEPTION(
						JpegUnkownFormatException());
			}
		}

		void JpegDecompress::get_image_information(
			const ReaderSharedPtr &reader, const bool rg_formats,
			TextureFormatType &texture_format, glm::uvec3 &sizes,
			Uint16 &mipmaps)
		{
			ReadWriteMemory buffer;

			reader->read(buffer);

			if (setjmp(m_jerr.m_jmp_buffer))
			{
				EL_THROW_EXCEPTION(JpegErrorException()
					<< boost::errinfo_file_name(
						reader->get_name())
					<< errinfo_message(m_jerr.m_buffer));
			}

			jpeg_mem_src(&m_cinfo, static_cast<Uint8*>(
				buffer.get_ptr()), buffer.get_size());

			jpeg_read_header(&m_cinfo, true);

			texture_format = get_texture_format(
				m_cinfo.jpeg_color_space, rg_formats);

			sizes[0] = m_cinfo.output_width;
			sizes[1] = m_cinfo.output_height;
			sizes[2] = 1;

			mipmaps = 0;
		}

		class JpegCompress
		{
			private:
				jpeg_compress_struct m_cinfo;
				el_error_mgr m_jerr;

			public:
				JpegCompress();
				~JpegCompress();
				void set_image(const ImageSharedPtr &image,
					const WriterSharedPtr &writer);
				static bool can_save(
					const ImageSharedPtr &image);

		};

		JpegCompress::JpegCompress()
		{
			memset(m_jerr.m_buffer, 0, sizeof(m_jerr.m_buffer));

			m_cinfo.err = jpeg_std_error(&m_jerr.m_errmgr);
			m_jerr.m_errmgr.error_exit = el_error_exit;
			m_jerr.m_errmgr.output_message = el_output_message;

			jpeg_create_compress(&m_cinfo);
		}

		JpegCompress::~JpegCompress()
		{
			jpeg_destroy_compress(&m_cinfo);
		}

		void JpegCompress::set_image(const ImageSharedPtr &image,
			const WriterSharedPtr &writer)
		{
			boost::scoped_array<JSAMPROW> row_pointers;
			unsigned char *buffer;
			unsigned long size;
			Uint32 i;

			if (!can_save(image))
			{
				EL_THROW_EXCEPTION(
					JpegFormatNotSupportedException());
			}

			buffer = 0;
			size = 0;

			jpeg_mem_dest(&m_cinfo, &buffer, &size);

			m_cinfo.image_width = image->get_width();
			m_cinfo.image_height = image->get_height();
			m_cinfo.input_components = image->get_channel_count();

			if (image->get_channel_count() == 1)
			{
				m_cinfo.in_color_space = JCS_GRAYSCALE;
			}
			else
			{
				m_cinfo.in_color_space = JCS_RGB;
			}

			jpeg_set_defaults(&m_cinfo);
			jpeg_start_compress(&m_cinfo, TRUE);

			/* write bytes */
			row_pointers.reset(new JSAMPROW[image->get_height()]);

			for (i = 0; i < image->get_height(); i++)
			{
				row_pointers[i] = static_cast<JSAMPROW>(
					const_cast<void*>(image->get_pixel_data(
						0, i, 0, 0, 0)));
			}

			jpeg_write_scanlines(&m_cinfo, row_pointers.get(),
				image->get_height());

			jpeg_finish_compress(&m_cinfo);

			writer->write(buffer, size);
		}

		bool JpegCompress::can_save(const ImageSharedPtr &image)
		{
			if (image->get_type() != GL_UNSIGNED_BYTE)
			{
				return false;
			}

			switch (image->get_format())
			{
				case GL_LUMINANCE:
				case GL_LUMINANCE8:
				case GL_RGB:
				case GL_RGB8:
					return true;
				default:
					return false;
			}
		}

	}

	ImageSharedPtr JpegImage::load_image(const ReaderSharedPtr &reader,
		const bool rg_formats)
	{
		JpegDecompress jpeg_decompress;

		return jpeg_decompress.get_image(reader, rg_formats);
	}

	void JpegImage::get_image_information(const ReaderSharedPtr &reader,
		const bool rg_formats, TextureFormatType &texture_format,
		glm::uvec3 &sizes, Uint16 &mipmaps)
	{
		JpegDecompress jpeg_decompress;

		jpeg_decompress.get_image_information(reader, rg_formats,
			texture_format, sizes, mipmaps);
	}

	bool JpegImage::check_load(const Uint8Array32 &id)
	{
		return (id[0] == 0xFF) && (id[1] == 0xD8);
	}

	String JpegImage::get_image_str()
	{
		return String(UTF8("jpeg-image"));
	}

	void JpegImage::save_image(const ImageSharedPtr &image,
		const WriterSharedPtr &writer)
	{
		try
		{
			JpegCompress jpeg_compress;

			jpeg_compress.set_image(image, writer);
		}
		catch (boost::exception &exception)
		{
			exception << errinfo_name(image->get_name());
			throw;
		}
	}

	bool JpegImage::can_save(const ImageSharedPtr &image)
	{
		return JpegCompress::can_save(image);
	}

}
