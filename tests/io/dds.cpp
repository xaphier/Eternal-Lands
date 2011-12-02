/****************************************************************************
 *            main.cpp
 *
 * Author: 2010  Daniel Jungmann <dsj@gmx.net>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "exceptions.hpp"
#include "codec/dds/ddsimage.hpp"
#include "loader.hpp"
#include "saver.hpp"
#include "file/file.hpp"
#include <boost/random.hpp>
#include <boost/exception/diagnostic_information.hpp>

namespace eternal_lands
{

	namespace
	{

		const TextureFormatType texture_formats[15] =
		{
			tft_rgb_dxt1,
			tft_rgba_dxt3,
			tft_rgba_dxt5,
			tft_rgb8,
			tft_rgba8,
			tft_rgba8,
			tft_r5g6b5,
			tft_rgb5_a1,
			tft_rgb10_a2,
			tft_r3g3b2,
			tft_a8,
			tft_l8,
			tft_la8,
			tft_l_latc1,
			tft_la_latc2
		};

		const TextureFormatType uncompressed_texture_formats[15] =
		{
			tft_rgba8,
			tft_rgba8,
			tft_rgba8,
			tft_rgb8,
			tft_rgba8,
			tft_rgba8,
			tft_r5g6b5,
			tft_rgb5_a1,
			tft_rgb10_a2,
			tft_r3g3b2,
			tft_a8,
			tft_l8,
			tft_la8,
			tft_l8,
			tft_la8
		};

		const Uint32 image_width = 32;
		const Uint32 image_height = 32;
		const Uint32 image_depth = 1;
		const Uint32 image_mipmap_count = 5;

		TextureFormatType detect_texture_format(const String &file_name)
		{
			LoaderSharedPtr loader;
			AbstractFileSharedPtr resource;

			resource = boost::make_shared<File>(file_name);
			loader = boost::make_shared<Loader>(resource);

			return DdsImage::detect_texture_format(loader);
		}

		ImageSharedPtr load_dds(const String &file_name, const TextureFormatType format)
		{
			LoaderSharedPtr loader;
			AbstractFileSharedPtr resource;
			ImageSharedPtr image;

			resource = boost::make_shared<File>(file_name);
			loader = boost::make_shared<Loader>(resource);

			DdsImage dds(loader, format);

			return dds.get_image();
		}

		void check_dds(const String &dir)
		{
			glm::vec4 data;
			ImageSharedPtr image;
			String file_name;
			TextureFormatType format;
			Uint16 i;

			DdsImage::check_fourcc_support();

			for (i = 0; i < 15; i++)
			{
				std::stringstream str;

				str << "test" << i << ".dds";

				file_name = AbstractFile::combine(dir, str.str());

				format = detect_texture_format(file_name);

				if (format != texture_formats[i])
				{
					EL_THROW_EXCEPTION(IoErrorException()
						<< ::boost::errinfo_file_name(file_name));
				}

				image = load_dds(file_name, format);

				if (image->get_width() != image_width)
				{
					EL_THROW_EXCEPTION(IoErrorException()
						<< ::boost::errinfo_file_name(file_name));
				}

				if (image->get_height() != image_height)
				{
					EL_THROW_EXCEPTION(IoErrorException()
						<< ::boost::errinfo_file_name(file_name));
				}

				if (image->get_depth() != image_depth)
				{
					EL_THROW_EXCEPTION(IoErrorException()
						<< ::boost::errinfo_file_name(file_name));
				}

				if (image->get_mipmap_count() != image_mipmap_count)
				{
					EL_THROW_EXCEPTION(IoErrorException()
						<< ::boost::errinfo_file_name(file_name));
				}

				if (image->get_texture_format() != texture_formats[i])
				{
					EL_THROW_EXCEPTION(IoErrorException()
						<< ::boost::errinfo_file_name(file_name));
				}
			}
		}

		void check_dds_umcompress(const String &dir)
		{
			glm::vec4 data;
			ImageSharedPtr image;
			String file_name;
			float scale_x, scale_y;
			Uint16 i;

			DdsImage::check_fourcc_support();

			scale_x = 1.0f / (image_width - 1);
			scale_y = 1.0f / (image_height - 1);

			for (i = 0; i < 15; i++)
			{
				std::stringstream str;

				str << "test" << i << ".dds";

				file_name = AbstractFile::combine(dir, str.str());

				image = load_dds(file_name, uncompressed_texture_formats[i]);

				if (image->get_width() != image_width)
				{
					EL_THROW_EXCEPTION(IoErrorException()
						<< ::boost::errinfo_file_name(file_name));
				}

				if (image->get_height() != image_height)
				{
					EL_THROW_EXCEPTION(IoErrorException()
						<< ::boost::errinfo_file_name(file_name));
				}

				if (image->get_depth() != image_depth)
				{
					EL_THROW_EXCEPTION(IoErrorException()
						<< ::boost::errinfo_file_name(file_name));
				}

				if (image->get_mipmap_count() != image_mipmap_count)
				{
					EL_THROW_EXCEPTION(IoErrorException()
						<< ::boost::errinfo_file_name(file_name));
				}

				if (image->get_texture_format() != uncompressed_texture_formats[i])
				{
					EL_THROW_EXCEPTION(IoErrorException()
						<< ::boost::errinfo_file_name(file_name));
				}

				if (image->get_compressed())
				{
					EL_THROW_EXCEPTION(IoErrorException()
						<< ::boost::errinfo_file_name(file_name));
				}

			}
		}

		void check_write_dds()
		{
			glm::vec4 data;
			ImageSharedPtr image;
			SaverSharedPtr saver;
			Uint32Array3 size;
			Uint32 x, y, z, i, index;

			size[0] = image_width;
			size[1] = image_height;
			size[2] = image_depth;

			image = boost::make_shared<Image>("rgba", false, tft_rgba8, size, 0);
			saver = boost::make_shared<Saver>("data.dds");

			index = 0;

			for (z = 0; z < size[2]; z++)
			{
				for (y = 0; y < size[1]; y++)
				{
					for (x = 0; x < size[0]; x++)
					{
						for (i = 0; i < 4; i++)
						{
							data[i] = ((index * 37) % 257) / 256.0f;
							index += 17;
						}

						image->set_pixel(x, y, z, 0, 0, data);
					}
				}
			}

			DdsImage::save_image(*image, saver);
		}

	}
}

int main(int argc, char *argv[])
{
	std::string str;
	try
	{
		if (argc >= 2)
		{
			str = argv[1];
		}

		eternal_lands::check_dds(str);
		eternal_lands::check_dds_umcompress(str);
		eternal_lands::check_write_dds();

		return 0;
	}
	catch (const boost::exception &exception)
	{
		std::cerr << boost::diagnostic_information(exception) << std::endl;
		throw;
	}
}
