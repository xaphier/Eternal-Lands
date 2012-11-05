/****************************************************************************
 *            j2kimage.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "j2kimage.hpp"
#include "exceptions.hpp"
#include "logging.hpp"
#include "readwritememory.hpp"
#include "reader.hpp"
#include "image.hpp"
#include <openjpeg.h>

namespace eternal_lands
{

	namespace
	{

		/**
		 * OpenJPEG Error callback 
		 */
		void j2k_error_callback(const char *msg, void *client_data)
		{
			LOG_ERROR(lt_j2k_image, UTF8("J2K: %1%"), msg);
		}

		/**
		 * OpenJPEG Warning callback 
		 */
		void j2k_warning_callback(const char *msg, void *client_data)
		{
			LOG_WARNING(lt_j2k_image, UTF8("J2K: %1%"), msg);
		}

		/**
		 * OpenJPEG Debugging callback 
		 */
		void j2k_debug_callback(const char *msg, void *client_data)
		{
			LOG_DEBUG(lt_j2k_image, UTF8("J2K: %1%"), msg);
		}

		class J2kDecompress
		{
			private:
				opj_dparameters_t m_parameters;	// decompression parameters 
				opj_event_mgr_t m_event_mgr;		// event manager 
				opj_image_t* m_image;		// decoded image 
				opj_dinfo_t* m_dinfo;	// handle to a decompressor 
				opj_cio_t* m_cio;

				TextureFormatType get_texture_format(
					const Uint32 color_type,
					const Uint32 bit_depth,
					const bool sRGB, const bool rg_formats)
					const;

			public:
				J2kDecompress();
				~J2kDecompress() noexcept;

				ImageSharedPtr get_image(
					const ReaderSharedPtr &reader,
					const bool rg_formats);

				void get_image_information(
					const ReaderSharedPtr &reader,
					const bool rg_formats,
					TextureFormatType &texture_format,
					glm::uvec3 &size, Uint16 &mipmaps);

		};

		J2kDecompress::J2kDecompress()
		{
			// configure the event callbacks
			memset(&m_event_mgr, 0, sizeof(opj_event_mgr_t));
			m_event_mgr.error_handler = j2k_error_callback;
			m_event_mgr.warning_handler = j2k_warning_callback;
			m_event_mgr.info_handler = j2k_debug_callback;

			// set decoding parameters to default values 
			opj_set_default_decoder_parameters(&m_parameters);

			// get a decoder handle 
			m_dinfo = opj_create_decompress(CODEC_J2K);
			
			// catch events using our callbacks
			opj_set_event_mgr((opj_common_ptr)m_dinfo, &m_event_mgr,
				nullptr);			

			// setup the decoder decoding parameters using user parameters 
			opj_setup_decoder(m_dinfo, &m_parameters);
		}

		J2kDecompress::~J2kDecompress() noexcept
		{
			if (m_cio != nullptr)
			{
				opj_cio_close(m_cio);
				m_cio = nullptr;
			}

			if (m_dinfo != nullptr)
			{
				opj_destroy_decompress(m_dinfo);
				m_dinfo = nullptr;
			}

			if (m_image != nullptr)
			{
				opj_image_destroy(m_image);
				m_image = nullptr;
			}
		}

		ImageSharedPtr J2kDecompress::get_image(
			const ReaderSharedPtr &reader, const bool rg_formats)
		{
			ImageSharedPtr image;

			m_cio = opj_cio_open((opj_common_ptr)m_dinfo,
				const_cast<Uint8*>(static_cast<const Uint8*>(
					reader->get_buffer()->get_ptr())) +
				reader->get_position(),
				reader->get_bytes_left());

			// decode the stream and fill the image structure 
			m_image = opj_decode(m_dinfo, m_cio);

			return image;
		}

		void J2kDecompress::get_image_information(
			const ReaderSharedPtr &reader, const bool rg_formats,
			TextureFormatType &texture_format, glm::uvec3 &size,
			Uint16 &mipmaps)
		{
		}

	}

	ImageSharedPtr J2kImage::load_image(const ReaderSharedPtr &reader,
		const bool rg_formats)
	{
		J2kDecompress j2k_decompress;

		return j2k_decompress.get_image(reader, rg_formats);
	}

	void J2kImage::get_image_information(const ReaderSharedPtr &reader,
		const bool rg_formats, TextureFormatType &texture_format,
		glm::uvec3 &size, Uint16 &mipmaps)
	{
/*		JpegDecompress jpeg_decompress;

		jpeg_decompress.get_image_information(reader, rg_formats,
			texture_format, size, mipmaps);
*/	}

	bool J2kImage::check_load(const Uint8Array32 &id)
	{
		return (id[0] == 0xFF) && (id[1] == 0x4F);
	}

	String J2kImage::get_image_str()
	{
		return String(UTF8("j2k-image"));
	}

	void J2kImage::save_image(const ImageSharedPtr &image,
		OutStream &saver)
	{
/*		try
		{
			JpegCompress jpeg_compress;

			jpeg_compress.set_image(image, saver);
		}
		catch (boost::exception &exception)
		{
			exception << errinfo_name(image->get_name());
			throw;
		}
*/	}

	bool J2kImage::can_save(const ImageSharedPtr &image)
	{
		return false;//JpegCompress::can_save(image);
	}

}
