/****************************************************************************
 *            dds.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

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
		tft_l8,
		tft_l8,
		tft_la8,
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
		tft_rgba8,
		tft_r5g6b5,
		tft_rgb5_a1,
		tft_rgb10_a2,
		tft_r3g3b2,
		tft_l8,
		tft_l8,
		tft_la8,
		tft_l8,
		tft_la8
	};

	const Uint32 image_width = 32;
	const Uint32 image_height = 32;
	const Uint32 image_depth = 1;
	const Uint32 image_mipmap_count = 5;

}

BOOST_AUTO_TEST_CASE(fourcc)
{
	CodecManager codec_manager;

	BOOST_CHECK_NO_THROW(DdsImage::check_all_fourcc_support(codec_manager,
		false));
	BOOST_CHECK_NO_THROW(DdsImage::check_all_fourcc_support(codec_manager,
		true));
}

BOOST_AUTO_TEST_CASE(read_image)
{
	FileSystem file_system;
	CodecManager codec_manager;
	ImageCompressionTypeSet compressions;
	ReaderSharedPtr reader;
	ImageSharedPtr image;
	Uint16 i;

	file_system.add_dir(String(UTF8("images")));

	compressions.insert(ict_s3tc);
	compressions.insert(ict_rgtc);

	for (i = 0; i < 15; i++)
	{
		std::stringstream str;

		str << UTF8("test") << i << UTF8(".dds");

		reader = file_system.get_file(String(str.str()));

		BOOST_CHECK_NO_THROW(image = DdsImage::load_image(codec_manager,
			reader, compressions, false, false));

		BOOST_CHECK_EQUAL(image->get_texture_format(),
			texture_formats[i]);
		BOOST_CHECK_EQUAL(image->get_width(), image_width);
		BOOST_CHECK_EQUAL(image->get_height(), image_height);
		BOOST_CHECK_EQUAL(image->get_depth(), image_depth);
		BOOST_CHECK_EQUAL(image->get_mipmap_count(),
			image_mipmap_count);
	}
}

BOOST_AUTO_TEST_CASE(read_image_uncompressed)
{
	FileSystem file_system;
	CodecManager codec_manager;
	ImageCompressionTypeSet compressions;
	ReaderSharedPtr reader;
	ImageSharedPtr image;
	Uint16 i;

	file_system.add_dir(String(UTF8("images")));

	for (i = 0; i < 15; i++)
	{
		std::stringstream str;

		str << UTF8("test") << i << UTF8(".dds");

		reader = file_system.get_file(String(str.str()));

		BOOST_CHECK_NO_THROW(image = DdsImage::load_image(codec_manager,
			reader, compressions, false, false));

		BOOST_CHECK_EQUAL(image->get_texture_format(),
			uncompressed_texture_formats[i]);
		BOOST_CHECK_EQUAL(image->get_width(), image_width);
		BOOST_CHECK_EQUAL(image->get_height(), image_height);
		BOOST_CHECK_EQUAL(image->get_depth(), image_depth);
		BOOST_CHECK_EQUAL(image->get_mipmap_count(),
			image_mipmap_count);
	}
}
#if	0
BOOST_AUTO_TEST_CASE(read_write_read)
{
	FileSystem file_system;
	CodecManager codec_manager;
	ImageCompressionTypeSet compressions;
	ReaderSharedPtr reader;
	WriterSharedPtr writer;
	glm::uvec4 d0, d1;
	ImageSharedPtr image0, image1;
	Uint32 width, height, depth;
	Uint16 i, x, y, z, m;

	file_system.add_dir(String(UTF8("images")));

	for (i = 0; i < 15; i++)
	{
		std::stringstream str;

		str << UTF8("test") << i << UTF8(".dds");

		reader = file_system.get_file(String(str.str()));

		image0 = DdsImage::load_image(codec_manager, reader,
			compressions, false, false);

		BOOST_CHECK_EQUAL(image0->get_texture_format(),
			uncompressed_texture_formats[i]);
		BOOST_CHECK_EQUAL(image0->get_width(), image_width);
		BOOST_CHECK_EQUAL(image0->get_height(), image_height);
		BOOST_CHECK_EQUAL(image0->get_depth(), image_depth);
		BOOST_CHECK_EQUAL(image0->get_mipmap_count(),
			image_mipmap_count);

		BOOST_CHECK_NO_THROW(DdsImage::save_image(image0, writer));

		image1 = DdsImage::load_image(codec_manager, reader,
			compressions, false, false);

		BOOST_CHECK_EQUAL(image1->get_texture_format(),
			uncompressed_texture_formats[i]);
		BOOST_CHECK_EQUAL(image1->get_width(), image_width);
		BOOST_CHECK_EQUAL(image1->get_height(), image_height);
		BOOST_CHECK_EQUAL(image1->get_depth(), image_depth);
		BOOST_CHECK_EQUAL(image1->get_mipmap_count(),
			image_mipmap_count);

		width = image_width;
		height = image_height;
		depth = image_depth;

		for (m = 0; m < image_mipmap_count; ++m)
		{
			for (z = 0; z < depth; ++z)
			{
				for (y = 0; y < height; ++y)
				{
					for (x = 0; x < width; ++x)
					{
						d0 = image0->get_pixel_uint(
							x, y, z, 0, m);
						d1 = image1->get_pixel_uint(
							x, y, z, 0, m);

						BOOST_CHECK_EQUAL(d0.x, d1.x);
						BOOST_CHECK_EQUAL(d0.y, d1.y);
						BOOST_CHECK_EQUAL(d0.z, d1.z);
						BOOST_CHECK_EQUAL(d0.w, d1.w);
					}
				}
			}

			width = std::max((width + 1u) / 2u, 1u);
			height = std::max((height + 1u) / 2u, 1u);
			depth = std::max((depth + 1u) / 2u, 1u);
		}
	}
}
#endif
