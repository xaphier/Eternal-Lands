/****************************************************************************
 *            dds.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "prerequisites.hpp"
#include "exceptions.hpp"
#include "codec/ddsimage.hpp"
#include "codec/codecmanager.hpp"
#include "image.hpp"
#include "reader.hpp"
#include "writer.hpp"
#include "filesystem.hpp"
#include <boost/random.hpp>
#define BOOST_TEST_MODULE dds
#include <boost/test/unit_test.hpp>

using namespace eternal_lands;

namespace
{

	const TextureFormatType texture_formats[15] =
	{
		tft_rgba_dxt1,
		tft_rgba_dxt3,
		tft_rgba_dxt5,
		tft_rgb8,
		tft_rgba8,
		tft_rgba4,
		tft_r5g6b5,
		tft_rgb5_a1,
		tft_rgb10_a2,
		tft_r3g3b2,
		tft_r8,
		tft_r8,
		tft_rg8,
		tft_r_rgtc1,
		tft_rg_rgtc2
	};

	const TextureFormatType uncompressed_texture_formats[15] =
	{
		tft_rgba8,
		tft_rgba8,
		tft_rgba8,
		tft_rgb8,
		tft_rgba8,
		tft_rgba4,
		tft_r5g6b5,
		tft_rgb5_a1,
		tft_rgb10_a2,
		tft_r3g3b2,
		tft_r8,
		tft_r8,
		tft_rg8,
		tft_r8,
		tft_rg8
	};

	const TextureFormatType texture_formats_color[10] =
	{
		tft_rgba8,
		tft_rgba8,
		tft_rgba8,
		tft_rgb8,
		tft_rgba8,
		tft_rgba4,
		tft_r5g6b5,
		tft_rgb5_a1,
		tft_rgb10_a2,
		tft_r3g3b2
	};

	const bool color_has_alpha[10] =
	{
		false,
		true,
		true,
		false,
		true,
		true,
		false,
		true,
		true,
		false,
	};

	const TextureFormatType texture_formats_color_fourcc[10] =
	{
		tft_r16,
		tft_rg16,
		tft_rgba16,
		tft_r16f,
		tft_rg16f,
		tft_rgba16f,
		tft_rgba16_snorm,
		tft_r32f,
		tft_rg32f,
		tft_rgba32f
	};

	const Uint32 image_width = 32;
	const Uint32 image_height = 32;
	const Uint32 image_depth = 0;
	const Uint32 image_mipmap_count = 5;

}

BOOST_AUTO_TEST_CASE(fourcc)
{
	BOOST_CHECK_NO_THROW(DdsImage::check_all_fourcc_support(false));
	BOOST_CHECK_NO_THROW(DdsImage::check_all_fourcc_support(true));
}

BOOST_AUTO_TEST_CASE(read_image)
{
	FileSystem file_system;
	ImageCompressionTypeSet compressions;
	ReaderSharedPtr reader;
	ImageSharedPtr image;
	String name;
	Uint32 i;

	file_system.add_dir(String(UTF8("images")));

	compressions.insert(ict_s3tc);
	compressions.insert(ict_rgtc);

	for (i = 0; i < 15; i++)
	{
		std::stringstream str;

		str << UTF8("test") << i << UTF8(".dds");

		name = String(str.str());

		BOOST_CHECK_NO_THROW(reader = file_system.get_file(name));

		BOOST_CHECK_NO_THROW(image = DdsImage::load_image(reader,
			compressions, true, false, false));

		BOOST_CHECK_EQUAL(image->get_texture_format(),
			texture_formats[i]);
		BOOST_CHECK_EQUAL(image->get_width(), image_width);
		BOOST_CHECK_EQUAL(image->get_height(), image_height);
		BOOST_CHECK_EQUAL(image->get_depth(), image_depth);
		BOOST_CHECK_EQUAL(image->get_mipmap_count(),
			image_mipmap_count);
		BOOST_CHECK_EQUAL(image->get_array(), false);
		BOOST_CHECK_EQUAL(image->get_cube_map(), false);
	}
}

BOOST_AUTO_TEST_CASE(read_image_uncompressed)
{
	FileSystem file_system;
	ImageCompressionTypeSet compressions;
	ReaderSharedPtr reader;
	ImageSharedPtr image;
	String name;
	Uint32 i;

	file_system.add_dir(String(UTF8("images")));

	for (i = 0; i < 15; i++)
	{
		std::stringstream str;

		str << UTF8("test") << i << UTF8(".dds");

		name = String(str.str());

		BOOST_CHECK_NO_THROW(reader = file_system.get_file(name));

		BOOST_CHECK_NO_THROW(image = DdsImage::load_image(reader,
			compressions, true, false, false));

		BOOST_CHECK_EQUAL(image->get_texture_format(),
			uncompressed_texture_formats[i]);
		BOOST_CHECK_EQUAL(image->get_width(), image_width);
		BOOST_CHECK_EQUAL(image->get_height(), image_height);
		BOOST_CHECK_EQUAL(image->get_depth(), image_depth);
		BOOST_CHECK_EQUAL(image->get_mipmap_count(),
			image_mipmap_count);
		BOOST_CHECK_EQUAL(image->get_array(), false);
		BOOST_CHECK_EQUAL(image->get_cube_map(), false);
	}
}

BOOST_AUTO_TEST_CASE(read_image_color)
{
	FileSystem file_system;
	ImageCompressionTypeSet compressions;
	ReaderSharedPtr reader;
	ImageSharedPtr image;
	glm::vec4 c0, c1;
	String name;
	Uint32 width, height, depth;
	Uint32 i, x, y, z;

	file_system.add_dir(String(UTF8("images")));

	for (i = 0; i < 10; i++)
	{
		std::stringstream str;

		str << UTF8("test") << i << UTF8("_color.dds");

		name = String(str.str());

		BOOST_CHECK_NO_THROW(reader = file_system.get_file(name));

		BOOST_CHECK_NO_THROW(image = DdsImage::load_image(reader,
			compressions, true, false, false));

		BOOST_CHECK_EQUAL(image->get_texture_format(),
			texture_formats_color[i]);
		BOOST_CHECK_EQUAL(image->get_width(), image_width);
		BOOST_CHECK_EQUAL(image->get_height(), image_height);
		BOOST_CHECK_EQUAL(image->get_depth(), image_depth);
		BOOST_CHECK_EQUAL(image->get_mipmap_count(),
			image_mipmap_count);
		BOOST_CHECK_EQUAL(image->get_array(), false);
		BOOST_CHECK_EQUAL(image->get_cube_map(), false);

		width = std::max(1u, image_width);
		height = std::max(1u, image_height);
		depth = std::max(1u, image_depth);

		for (z = 0; z < depth; ++z)
		{
			for (y = 0; y < height; ++y)
			{
				for (x = 0; x < width; ++x)
				{
					c0 = image->get_pixel(x, y, z, 0, 0);

					c1.r = (((x / 4) % 4) % 2);
					c1.g = (((x / 4) % 4) / 2);
					c1.b = (((y / 4) % 4) % 2);
					c1.a = (((y / 4) % 4) / 2);

					BOOST_CHECK_CLOSE(c0.r, c1.r, 1.0);
					BOOST_CHECK_CLOSE(c0.g, c1.g, 1.0);
					BOOST_CHECK_CLOSE(c0.b, c1.b, 1.0);

					if (color_has_alpha[i])
					{
						BOOST_CHECK_CLOSE(c0.a, c1.a,
							1.0);
					}
				}
			}
		}
	}
}

BOOST_AUTO_TEST_CASE(read_write_read)
{
	FileSystem file_system;
	ImageCompressionTypeSet compressions;
	ReaderSharedPtr reader;
	WriterSharedPtr writer;
	ReadWriteMemorySharedPtr buffer;
	boost::shared_ptr<StringStream> stream;
	glm::uvec4 c0, c1;
	ImageSharedPtr image0, image1;
	String name;
	Uint32 width, height, depth;
	Uint32 i, x, y, z, m;

	file_system.add_dir(String(UTF8("images")));

	for (i = 0; i < 15; i++)
	{
		std::stringstream str;

		str << UTF8("test") << i << UTF8(".dds");

		name = String(str.str());

		BOOST_CHECK_NO_THROW(reader = file_system.get_file(name));

		image0 = DdsImage::load_image(reader, compressions, true,
			false, false);

		BOOST_CHECK_EQUAL(image0->get_texture_format(),
			uncompressed_texture_formats[i]);
		BOOST_CHECK_EQUAL(image0->get_width(), image_width);
		BOOST_CHECK_EQUAL(image0->get_height(), image_height);
		BOOST_CHECK_EQUAL(image0->get_depth(), image_depth);
		BOOST_CHECK_EQUAL(image0->get_mipmap_count(),
			image_mipmap_count);
		BOOST_CHECK_EQUAL(image0->get_array(), false);
		BOOST_CHECK_EQUAL(image0->get_cube_map(), false);

		stream = boost::make_shared<StringStream>();

		writer = boost::make_shared<Writer>(stream, name);

		BOOST_CHECK_NO_THROW(DdsImage::save_image(image0, writer));

		buffer = boost::make_shared<ReadWriteMemory>(
			writer->get_position());

		stream->read(static_cast<char*>(buffer->get_ptr()),
			buffer->get_size());

		reader = boost::make_shared<Reader>(buffer, name);	

		image1 = DdsImage::load_image(reader, compressions, true,
			false, false);

		BOOST_CHECK_EQUAL(image1->get_texture_format(),
			uncompressed_texture_formats[i]);
		BOOST_CHECK_EQUAL(image1->get_width(), image_width);
		BOOST_CHECK_EQUAL(image1->get_height(), image_height);
		BOOST_CHECK_EQUAL(image1->get_depth(), image_depth);
		BOOST_CHECK_EQUAL(image1->get_mipmap_count(),
			image_mipmap_count);
		BOOST_CHECK_EQUAL(image1->get_array(), false);
		BOOST_CHECK_EQUAL(image1->get_cube_map(), false);

		width = std::max(1u, image_width);
		height = std::max(1u, image_height);
		depth = std::max(1u, image_depth);

		for (m = 0; m < image_mipmap_count; ++m)
		{
			for (z = 0; z < depth; ++z)
			{
				for (y = 0; y < height; ++y)
				{
					for (x = 0; x < width; ++x)
					{
						c0 = image0->get_pixel_uint(
							x, y, z, 0, m);
						c1 = image1->get_pixel_uint(
							x, y, z, 0, m);

						BOOST_CHECK_EQUAL(c0.x, c1.x);
						BOOST_CHECK_EQUAL(c0.y, c1.y);
						BOOST_CHECK_EQUAL(c0.z, c1.z);
						BOOST_CHECK_EQUAL(c0.w, c1.w);
					}
				}
			}

			width = std::max((width + 1u) / 2u, 1u);
			height = std::max((height + 1u) / 2u, 1u);
			depth = std::max((depth + 1u) / 2u, 1u);
		}
	}
}

BOOST_AUTO_TEST_CASE(write_read_color)
{
	ImageCompressionTypeSet compressions;
	ReaderSharedPtr reader;
	WriterSharedPtr writer;
	ReadWriteMemorySharedPtr buffer;
	boost::shared_ptr<StringStream> stream;
	glm::vec4 c0, c1;
	ImageSharedPtr image0, image1;
	String name;
	Uint32 width, height, depth, count;
	Uint32 i, j, x, y, z;

	for (i = 0; i < 10; i++)
	{
		std::stringstream str;

		str << UTF8("test") << i << UTF8(".dds");

		name = String(str.str());

		stream = boost::make_shared<StringStream>();

		writer = boost::make_shared<Writer>(stream, name);

		image0 = boost::make_shared<Image>(name, false,
			texture_formats_color_fourcc[i],
			glm::uvec3(image_width, image_height, image_depth), 0,
			false);

		width = std::max(1u, image_width);
		height = std::max(1u, image_height);
		depth = std::max(1u, image_depth);

		for (z = 0; z < depth; ++z)
		{
			for (y = 0; y < height; ++y)
			{
				for (x = 0; x < width; ++x)
				{
					c0.r = (((x / 4) % 4) % 2);
					c0.g = (((x / 4) % 4) / 2);
					c0.b = (((y / 4) % 4) % 2);
					c0.a = (((y / 4) % 4) / 2);

					image0->set_pixel(x, y, z, 0, 0, c0);
				}
			}
		}

		BOOST_CHECK_NO_THROW(DdsImage::save_image(image0, writer));

		buffer = boost::make_shared<ReadWriteMemory>(
			writer->get_position());

		stream->read(static_cast<char*>(buffer->get_ptr()),
			buffer->get_size());

		reader = boost::make_shared<Reader>(buffer, name);	

		image1 = DdsImage::load_image(reader, compressions, true,
			false, false);

		BOOST_CHECK_EQUAL(image1->get_texture_format(),
			texture_formats_color_fourcc[i]);
		BOOST_CHECK_EQUAL(image1->get_width(), image_width);
		BOOST_CHECK_EQUAL(image1->get_height(), image_height);
		BOOST_CHECK_EQUAL(image1->get_depth(), image_depth);
		BOOST_CHECK_EQUAL(image1->get_mipmap_count(), 0);
		BOOST_CHECK_EQUAL(image1->get_array(), false);
		BOOST_CHECK_EQUAL(image1->get_cube_map(), false);

		count = TextureFormatUtil::get_count(
			texture_formats_color_fourcc[i]);

		for (z = 0; z < depth; ++z)
		{
			for (y = 0; y < height; ++y)
			{
				for (x = 0; x < width; ++x)
				{
					c0 = image0->get_pixel(x, y, z, 0, 0);
					c1 = image1->get_pixel(x, y, z, 0, 0);

					for (j = 0; j < count; ++j)
					{
						BOOST_CHECK_CLOSE(c0[j], c1[j],
							1.0);
					}
				}
			}
		}
	}
}

BOOST_AUTO_TEST_CASE(write_read_dxt10)
{
	ImageCompressionTypeSet compressions;
	ReaderSharedPtr reader;
	WriterSharedPtr writer;
	ReadWriteMemorySharedPtr buffer;
	boost::shared_ptr<StringStream> stream;
	std::vector<TextureFormatType> texture_formats;
	glm::vec4 c0, c1;
	ImageSharedPtr image0, image1;
	String name;
	Uint32 width, height, depth, count;
	Uint32 i, j, x, y, z;

	DdsImage::get_dxt10_formats(texture_formats);

	for (i = 0; i < texture_formats.size(); i++)
	{
		std::stringstream str;

		count = TextureFormatUtil::get_count(texture_formats[i]);

		if (count == 0)
		{
			continue;
		}

		if (TextureFormatUtil::get_compressed(texture_formats[i]))
		{
			continue;
		}

		str << UTF8("test-dds ") << texture_formats[i];

		name = String(str.str());

		stream = boost::make_shared<StringStream>();

		writer = boost::make_shared<Writer>(stream, name);

		image0 = boost::make_shared<Image>(name, false,
			texture_formats[i], glm::uvec3(image_width,
				image_height, image_depth), 0, false);

		width = std::max(1u, image_width);
		height = std::max(1u, image_height);
		depth = std::max(1u, image_depth);

		for (z = 0; z < depth; ++z)
		{
			for (y = 0; y < height; ++y)
			{
				for (x = 0; x < width; ++x)
				{
					c0.r = (((x / 4) % 4) % 2);
					c0.g = (((x / 4) % 4) / 2);
					c0.b = (((y / 4) % 4) % 2);
					c0.a = (((y / 4) % 4) / 2);

					image0->set_pixel(x, y, z, 0, 0, c0);
				}
			}
		}

		BOOST_CHECK_NO_THROW(DdsImage::save_image_dxt10(image0,
			writer));

		buffer = boost::make_shared<ReadWriteMemory>(
			writer->get_position());

		stream->read(static_cast<char*>(buffer->get_ptr()),
			buffer->get_size());

		reader = boost::make_shared<Reader>(buffer, name);	

		image1 = DdsImage::load_image(reader, compressions, true,
			false, false);

		BOOST_CHECK_EQUAL(image1->get_texture_format(),
			texture_formats[i]);
		BOOST_CHECK_EQUAL(image1->get_width(), image_width);
		BOOST_CHECK_EQUAL(image1->get_height(), image_height);
		BOOST_CHECK_EQUAL(image1->get_depth(), image_depth);
		BOOST_CHECK_EQUAL(image1->get_mipmap_count(), 0);
		BOOST_CHECK_EQUAL(image1->get_array(), false);
		BOOST_CHECK_EQUAL(image1->get_cube_map(), false);

		for (z = 0; z < depth; ++z)
		{
			for (y = 0; y < height; ++y)
			{
				for (x = 0; x < width; ++x)
				{
					c0 = image0->get_pixel(x, y, z, 0, 0);
					c1 = image1->get_pixel(x, y, z, 0, 0);

					for (j = 0; j < count; ++j)
					{
						BOOST_CHECK_CLOSE(c0[j], c1[j],
							1.0);
					}
				}
			}
		}
	}
}
