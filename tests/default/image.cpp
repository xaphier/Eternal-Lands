/****************************************************************************
 *            image.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "prerequisites.hpp"
#include "image.hpp"
#include "packtool.hpp"
#include <boost/random.hpp>
#define BOOST_TEST_MODULE image
#include <boost/test/unit_test.hpp>

namespace el = eternal_lands;

namespace
{

	typedef	std::set<el::TextureFormatType> TextureFormatTypeSet;
	typedef boost::array<glm::uvec4, 2> Uvec4Array2;

	class TextureFormats
	{
		private:
			TextureFormatTypeSet m_8_bit;
			TextureFormatTypeSet m_16_bit;
			TextureFormatTypeSet m_32_bit;
			TextureFormatTypeSet m_all;
			TextureFormatTypeSet m_1_channel;
			TextureFormatTypeSet m_2_channels;
			TextureFormatTypeSet m_3_channels;
			TextureFormatTypeSet m_4_channels;

		public:
			TextureFormats();

			inline const TextureFormatTypeSet &get_8_bit() const
				noexcept
			{
				return m_8_bit;
			}

			inline const TextureFormatTypeSet &get_16_bit() const
				noexcept
			{
				return m_16_bit;
			}

			inline const TextureFormatTypeSet &get_32_bit() const
				noexcept
			{
				return m_32_bit;
			}


			inline const TextureFormatTypeSet &get_all() const
				noexcept
			{
				return m_all;
			}

			inline const TextureFormatTypeSet &get_1_channel()
				const
			{
				return m_1_channel;
			}

			inline const TextureFormatTypeSet &get_2_channels()
				const noexcept
			{
				return m_2_channels;
			}

			inline const TextureFormatTypeSet &get_3_channels()
				const noexcept
			{
				return m_3_channels;
			}

			inline const TextureFormatTypeSet &get_4_channels()
				const noexcept
			{
				return m_4_channels;
			}

	};

	TextureFormats::TextureFormats()
	{
		el::TextureFormatType texture_format;
		Uint32 i, count;

		m_8_bit.insert(el::tft_r3g3b2);
		m_16_bit.insert(el::tft_rgba4);
		m_16_bit.insert(el::tft_r5g6b5);
		m_16_bit.insert(el::tft_rgb5_a1);
		m_32_bit.insert(el::tft_rgb10_a2);
		m_32_bit.insert(el::tft_r10f_b11f_g10f);
		m_32_bit.insert(el::tft_rgb9_e5);

		count = el::TextureFormatUtil::get_texture_format_count();

		for (i = 0; i < count; ++i)
		{
			texture_format = static_cast<el::TextureFormatType>(i);

			if (el::TextureFormatUtil::get_compressed(
				texture_format))
			{
				continue;
			}

			m_all.insert(texture_format);

			switch (el::TextureFormatUtil::get_count(
				texture_format))
			{
				case 1:
					m_1_channel.insert(texture_format);
					break;
				case 2:
					m_2_channels.insert(texture_format);
					break;
				case 3:
					m_3_channels.insert(texture_format);
					break;
				case 4:
					m_4_channels.insert(texture_format);
					break;
			};
		}
	}

}

BOOST_FIXTURE_TEST_SUITE(texture_formats, TextureFormats)

BOOST_AUTO_TEST_CASE(default_creation)
{
	boost::mt19937 rng;
	boost::uniform_int<Uint16> range(1, 2048);
	boost::variate_generator<boost::mt19937&,
		boost::uniform_int<Uint16> > random_int(rng, range);
	el::ImageSharedPtr image;
	glm::uvec3 size;
	Uint32 mipmap_count;
	bool cube_map;

	BOOST_FOREACH(const el::TextureFormatType texture_format, get_all())
	{
		size.x = random_int();
		size.y = random_int();
		size.z = random_int();
		cube_map = (random_int() % 2) == 0;

		mipmap_count = std::max(std::max(size.x, size.y), size.z);
		mipmap_count = std::log(mipmap_count) / std::log(2);

		image = boost::make_shared<el::Image>(el::String("image"),
			cube_map, texture_format, size, mipmap_count, false);

		BOOST_CHECK_EQUAL(image->get_cube_map(), cube_map);
		BOOST_CHECK_EQUAL(image->get_texture_format(), texture_format);
		BOOST_CHECK_EQUAL(image->get_size().x, size.x);
		BOOST_CHECK_EQUAL(image->get_size().y, size.y);
		BOOST_CHECK_EQUAL(image->get_size().z, size.z);
		BOOST_CHECK_EQUAL(image->get_width(), size.x);
		BOOST_CHECK_EQUAL(image->get_height(), size.y);
		BOOST_CHECK_EQUAL(image->get_depth(), size.z);
		BOOST_CHECK_EQUAL(image->get_mipmap_count(), mipmap_count);
	}
}
/*
BOOST_AUTO_TEST_CASE(get_value)
{
	boost::mt19937 rng;
	boost::uniform_int<Uint16> range(1, 2048);
	boost::variate_generator<boost::mt19937&,
		boost::uniform_int<Uint16> > random_int(rng, range);
	el::ImageSharedPtr image;
	glm::uvec3 size;
	Uint32 mipmap_count, x, y, z, m, w, h, d;
	bool cube_map;

	BOOST_FOREACH(const el::TextureFormatType texture_format, get_all())
	{
		size.x = random_int();
		size.y = random_int();
		size.z = random_int();

		mipmap_count = std::max(std::max(size.x, size.y), size.z);
		mipmap_count = std::log(mipmap_count) / std::log(2);

		image = boost::make_shared<el::Image>(el::String("image"),
			cube_map, texture_format, size, mipmap_count);

		BOOST_CHECK_EQUAL(image->get_cube_map(), cube_map);
		BOOST_CHECK_EQUAL(image->get_texture_format(), texture_format);
		BOOST_CHECK_EQUAL(image->get_size().x, size.x);
		BOOST_CHECK_EQUAL(image->get_size().y, size.y);
		BOOST_CHECK_EQUAL(image->get_size().z, size.z);
		BOOST_CHECK_EQUAL(image->get_mipmap_count(), mipmap_count);

		w = size.x;
		h = size.y;
		d = size.z;

		for (m = 0; m < mipmap_count; ++m)
		{	
			for (z = 0; z < d; ++z)
			{
				for (y = 0; y < h; ++y)
				{
					for (x = 0; x < w; ++x)
					{
						for (f = 0; f < faces; ++f)
						{
							data = image->get_pixel_data(x, y, z, f, m);

							BOOST_CHECK_SMALL(data.x, epsilon);
							BOOST_CHECK_SMALL(data.y, epsilon);
							BOOST_CHECK_SMALL(data.z, epsilon);
							BOOST_CHECK_SMALL(data.w, epsilon);
						}
					}
				}
			}
		}
	}
}
*/
BOOST_AUTO_TEST_CASE(set_get_8_bit_values)
{
	boost::mt19937 rng;
	boost::uniform_int<Uint16> range(0, 255);
	boost::variate_generator<boost::mt19937&,
		boost::uniform_int<Uint16> > random_int(rng, range);
	el::ImageSharedPtr image;
	glm::uvec3 size;
	Uint32 mipmap_count, x, y, z, f, m, data, i;
	bool cube_map;

	BOOST_FOREACH(const el::TextureFormatType texture_format, get_8_bit())
	{
		size.x = 64;
		size.y = 64;
		size.z = 64;
		cube_map = (random_int() % 2) == 0;

		mipmap_count = 6;

		image = boost::make_shared<el::Image>(el::String("image"),
			cube_map, texture_format, size, mipmap_count, false);

		BOOST_CHECK_EQUAL(image->get_cube_map(), cube_map);
		BOOST_CHECK_EQUAL(image->get_texture_format(), texture_format);
		BOOST_CHECK_EQUAL(image->get_size().x, size.x);
		BOOST_CHECK_EQUAL(image->get_size().y, size.y);
		BOOST_CHECK_EQUAL(image->get_size().z, size.z);
		BOOST_CHECK_EQUAL(image->get_width(), size.x);
		BOOST_CHECK_EQUAL(image->get_height(), size.y);
		BOOST_CHECK_EQUAL(image->get_depth(), size.z);
		BOOST_CHECK_EQUAL(image->get_mipmap_count(), mipmap_count);

		for (i = 0; i < 128; ++i)
		{
			x = random_int() % 64;
			y = random_int() % 64;
			z = random_int() % 64;
			m = random_int() % 6;

			x = x >> m;
			y = y >> m;
			z = z >> m;

			if (cube_map)
			{
				f = random_int() % 6;
			}
			else
			{
				f = 0;
			}

			data = random_int();

			image->set_pixel_packed_uint8(x, y, z, f, m, data);

			BOOST_CHECK_EQUAL(image->get_pixel_packed_uint8(x, y,
				z, f, m), data);
		}
	}
}

BOOST_AUTO_TEST_CASE(set_get_16_bit_values)
{
	boost::mt19937 rng;
	boost::uniform_int<Uint16> range(0, 65535);
	boost::variate_generator<boost::mt19937&,
		boost::uniform_int<Uint16> > random_int(rng, range);
	el::ImageSharedPtr image;
	glm::uvec3 size;
	Uint32 mipmap_count, x, y, z, f, m, data, i;
	bool cube_map;

	BOOST_FOREACH(const el::TextureFormatType texture_format, get_16_bit())
	{
		size.x = 64;
		size.y = 64;
		size.z = 64;
		cube_map = (random_int() % 2) == 0;

		mipmap_count = 6;

		image = boost::make_shared<el::Image>(el::String("image"),
			cube_map, texture_format, size, mipmap_count, false);

		BOOST_CHECK_EQUAL(image->get_cube_map(), cube_map);
		BOOST_CHECK_EQUAL(image->get_texture_format(), texture_format);
		BOOST_CHECK_EQUAL(image->get_size().x, size.x);
		BOOST_CHECK_EQUAL(image->get_size().y, size.y);
		BOOST_CHECK_EQUAL(image->get_size().z, size.z);
		BOOST_CHECK_EQUAL(image->get_width(), size.x);
		BOOST_CHECK_EQUAL(image->get_height(), size.y);
		BOOST_CHECK_EQUAL(image->get_depth(), size.z);
		BOOST_CHECK_EQUAL(image->get_mipmap_count(), mipmap_count);

		for (i = 0; i < 128; ++i)
		{
			x = random_int() % 64;
			y = random_int() % 64;
			z = random_int() % 64;
			m = random_int() % 6;

			x = x >> m;
			y = y >> m;
			z = z >> m;

			if (cube_map)
			{
				f = random_int() % 6;
			}
			else
			{
				f = 0;
			}

			data = random_int();

			image->set_pixel_packed_uint16(x, y, z, f, m, data);

			BOOST_CHECK_EQUAL(image->get_pixel_packed_uint16(x, y,
				z, f, m), data);
		}
	}
}

BOOST_AUTO_TEST_CASE(set_get_32_bit_values)
{
	boost::mt19937 rng;
	boost::uniform_int<Uint32> range(0, 4294967295);
	boost::variate_generator<boost::mt19937&,
		boost::uniform_int<Uint32> > random_int(rng, range);
	el::ImageSharedPtr image;
	glm::uvec3 size;
	Uint32 mipmap_count, x, y, z, f, m, data, i;
	bool cube_map;

	BOOST_FOREACH(const el::TextureFormatType texture_format, get_32_bit())
	{
		size.x = 64;
		size.y = 64;
		size.z = 64;
		cube_map = (random_int() % 2) == 0;

		mipmap_count = 6;

		image = boost::make_shared<el::Image>(el::String("image"),
			cube_map, texture_format, size, mipmap_count, false);

		BOOST_CHECK_EQUAL(image->get_cube_map(), cube_map);
		BOOST_CHECK_EQUAL(image->get_texture_format(), texture_format);
		BOOST_CHECK_EQUAL(image->get_size().x, size.x);
		BOOST_CHECK_EQUAL(image->get_size().y, size.y);
		BOOST_CHECK_EQUAL(image->get_size().z, size.z);
		BOOST_CHECK_EQUAL(image->get_width(), size.x);
		BOOST_CHECK_EQUAL(image->get_height(), size.y);
		BOOST_CHECK_EQUAL(image->get_depth(), size.z);
		BOOST_CHECK_EQUAL(image->get_mipmap_count(), mipmap_count);

		for (i = 0; i < 128; ++i)
		{
			x = random_int() % 64;
			y = random_int() % 64;
			z = random_int() % 64;
			m = random_int() % 6;

			x = x >> m;
			y = y >> m;
			z = z >> m;

			if (cube_map)
			{
				f = random_int() % 6;
			}
			else
			{
				f = 0;
			}

			data = random_int();

			image->set_pixel_packed_uint32(x, y, z, f, m, data);

			BOOST_CHECK_EQUAL(image->get_pixel_packed_uint32(x, y,
				z, f, m), data);
		}
	}
}

BOOST_AUTO_TEST_CASE(set_get_r3g3b2_value)
{
	boost::mt19937 rng;
	boost::uniform_int<Uint16> range(0, 255);
	boost::variate_generator<boost::mt19937&,
		boost::uniform_int<Uint16> > random_int(rng, range);
	el::ImageSharedPtr image;
	std::vector<Uvec4Array2> values;
	Uvec4Array2 value;
	glm::uvec3 color;
	glm::uvec3 size;
	Uint32 mipmap_count, x, y, z, f, m, data, i;
	bool cube_map;

	size.x = 64;
	size.y = 64;
	size.z = 64;
	cube_map = (random_int() % 2) == 0;

	mipmap_count = 6;

	image = boost::make_shared<el::Image>(el::String("image"),
		cube_map, el::tft_r3g3b2, size, mipmap_count, false);

	BOOST_CHECK_EQUAL(image->get_cube_map(), cube_map);
	BOOST_CHECK_EQUAL(image->get_texture_format(), el::tft_r3g3b2);
	BOOST_CHECK_EQUAL(image->get_size().x, size.x);
	BOOST_CHECK_EQUAL(image->get_size().y, size.y);
	BOOST_CHECK_EQUAL(image->get_size().z, size.z);
	BOOST_CHECK_EQUAL(image->get_width(), size.x);
	BOOST_CHECK_EQUAL(image->get_height(), size.y);
	BOOST_CHECK_EQUAL(image->get_depth(), size.z);
	BOOST_CHECK_EQUAL(image->get_mipmap_count(), mipmap_count);

	for (i = 0; i < 128; ++i)
	{
		x = random_int() % 64;
		y = random_int() % 64;
		z = random_int() % 64;
		m = random_int() % 6;

		x = x >> m;
		y = y >> m;
		z = z >> m;

		if (cube_map)
		{
			f = random_int() % 6;
		}
		else
		{
			f = 0;
		}

		color.r = random_int() % 8;
		color.g = random_int() % 8;
		color.b = random_int() % 4;

		data = el::PackTool::pack_uint_3_3_2(false, glm::vec3(color));

		image->set_pixel_uint(x, y, z, f, m, glm::uvec4(color, 0));

		BOOST_CHECK_EQUAL(image->get_pixel_uint(x, y, z, f, m).r,
			color.r);
		BOOST_CHECK_EQUAL(image->get_pixel_uint(x, y, z, f, m).g,
			color.g);
		BOOST_CHECK_EQUAL(image->get_pixel_uint(x, y, z, f, m).b,
			color.b);

		BOOST_CHECK_CLOSE(image->get_pixel(x, y, z, f, m).r,
			color.r / 7.0f, 1.0);
		BOOST_CHECK_CLOSE(image->get_pixel(x, y, z, f, m).g,
			color.g / 7.0f, 1.0);
		BOOST_CHECK_CLOSE(image->get_pixel(x, y, z, f, m).b,
			color.b / 3.0f, 1.0);

		BOOST_CHECK_EQUAL(image->get_pixel_packed_uint8(x, y, z, f, m),
			data);
	}

	for (i = 0; i < 128; ++i)
	{
		x = random_int() % 64;
		y = random_int() % 64;
		z = random_int() % 64;
		m = random_int() % 6;

		x = x >> m;
		y = y >> m;
		z = z >> m;

		if (cube_map)
		{
			f = random_int() % 6;
		}
		else
		{
			f = 0;
		}

		color.r = random_int() % 8;
		color.g = random_int() % 8;
		color.b = random_int() % 4;

		data = el::PackTool::pack_uint_3_3_2(false, glm::vec3(color));

		image->set_pixel_packed_uint8(x, y, z, f, m, data);

		BOOST_CHECK_EQUAL(image->get_pixel_uint(x, y, z, f, m).r,
			color.r);
		BOOST_CHECK_EQUAL(image->get_pixel_uint(x, y, z, f, m).g,
			color.g);
		BOOST_CHECK_EQUAL(image->get_pixel_uint(x, y, z, f, m).b,
			color.b);

		BOOST_CHECK_CLOSE(image->get_pixel(x, y, z, f, m).r,
			color.r / 7.0f, 1.0);
		BOOST_CHECK_CLOSE(image->get_pixel(x, y, z, f, m).g,
			color.g / 7.0f, 1.0);
		BOOST_CHECK_CLOSE(image->get_pixel(x, y, z, f, m).b,
			color.b / 3.0f, 1.0);

		BOOST_CHECK_EQUAL(image->get_pixel_packed_uint8(x, y, z, f, m),
			data);
	}
}

BOOST_AUTO_TEST_CASE(set_get_rgba4_value)
{
	boost::mt19937 rng;
	boost::uniform_int<Uint16> range(0, 255);
	boost::variate_generator<boost::mt19937&,
		boost::uniform_int<Uint16> > random_int(rng, range);
	el::ImageSharedPtr image;
	std::vector<Uvec4Array2> values;
	Uvec4Array2 value;
	glm::uvec4 color;
	glm::uvec3 size;
	Uint32 mipmap_count, x, y, z, f, m, data, i;
	bool cube_map;

	size.x = 64;
	size.y = 64;
	size.z = 64;
	cube_map = (random_int() % 2) == 0;

	mipmap_count = 6;

	image = boost::make_shared<el::Image>(el::String("image"),
		cube_map, el::tft_rgba4, size, mipmap_count, false);

	BOOST_CHECK_EQUAL(image->get_cube_map(), cube_map);
	BOOST_CHECK_EQUAL(image->get_texture_format(), el::tft_rgba4);
	BOOST_CHECK_EQUAL(image->get_size().x, size.x);
	BOOST_CHECK_EQUAL(image->get_size().y, size.y);
	BOOST_CHECK_EQUAL(image->get_size().z, size.z);
	BOOST_CHECK_EQUAL(image->get_width(), size.x);
	BOOST_CHECK_EQUAL(image->get_height(), size.y);
	BOOST_CHECK_EQUAL(image->get_depth(), size.z);
	BOOST_CHECK_EQUAL(image->get_mipmap_count(), mipmap_count);

	for (i = 0; i < 128; ++i)
	{
		x = random_int() % 64;
		y = random_int() % 64;
		z = random_int() % 64;
		m = random_int() % 6;

		x = x >> m;
		y = y >> m;
		z = z >> m;

		if (cube_map)
		{
			f = random_int() % 6;
		}
		else
		{
			f = 0;
		}

		color.r = random_int() % 16;
		color.g = random_int() % 16;
		color.b = random_int() % 16;
		color.a = random_int() % 16;

		data = el::PackTool::pack_uint_4_4_4_4(false, glm::vec4(color));

		image->set_pixel_uint(x, y, z, f, m, color);

		BOOST_CHECK_EQUAL(image->get_pixel_uint(x, y, z, f, m).r,
			color.r);
		BOOST_CHECK_EQUAL(image->get_pixel_uint(x, y, z, f, m).g,
			color.g);
		BOOST_CHECK_EQUAL(image->get_pixel_uint(x, y, z, f, m).b,
			color.b);
		BOOST_CHECK_EQUAL(image->get_pixel_uint(x, y, z, f, m).a,
			color.a);

		BOOST_CHECK_CLOSE(image->get_pixel(x, y, z, f, m).r,
			color.r / 15.0f, 1.0);
		BOOST_CHECK_CLOSE(image->get_pixel(x, y, z, f, m).g,
			color.g / 15.0f, 1.0);
		BOOST_CHECK_CLOSE(image->get_pixel(x, y, z, f, m).b,
			color.b / 15.0f, 1.0);
		BOOST_CHECK_CLOSE(image->get_pixel(x, y, z, f, m).a,
			color.a / 15.0f, 1.0);

		BOOST_CHECK_EQUAL(image->get_pixel_packed_uint16(x, y, z, f, m),
			data);
	}

	for (i = 0; i < 128; ++i)
	{
		x = random_int() % 64;
		y = random_int() % 64;
		z = random_int() % 64;
		m = random_int() % 6;

		x = x >> m;
		y = y >> m;
		z = z >> m;

		if (cube_map)
		{
			f = random_int() % 6;
		}
		else
		{
			f = 0;
		}

		color.r = random_int() % 16;
		color.g = random_int() % 16;
		color.b = random_int() % 16;
		color.a = random_int() % 16;

		data = el::PackTool::pack_uint_4_4_4_4(false, glm::vec4(color));

		image->set_pixel_packed_uint16(x, y, z, f, m, data);

		BOOST_CHECK_EQUAL(image->get_pixel_uint(x, y, z, f, m).r,
			color.r);
		BOOST_CHECK_EQUAL(image->get_pixel_uint(x, y, z, f, m).g,
			color.g);
		BOOST_CHECK_EQUAL(image->get_pixel_uint(x, y, z, f, m).b,
			color.b);
		BOOST_CHECK_EQUAL(image->get_pixel_uint(x, y, z, f, m).a,
			color.a);

		BOOST_CHECK_CLOSE(image->get_pixel(x, y, z, f, m).r,
			color.r / 15.0f, 1.0);
		BOOST_CHECK_CLOSE(image->get_pixel(x, y, z, f, m).g,
			color.g / 15.0f, 1.0);
		BOOST_CHECK_CLOSE(image->get_pixel(x, y, z, f, m).b,
			color.b / 15.0f, 1.0);
		BOOST_CHECK_CLOSE(image->get_pixel(x, y, z, f, m).a,
			color.a / 15.0f, 1.0);

		BOOST_CHECK_EQUAL(image->get_pixel_packed_uint16(x, y, z, f, m),
			data);
	}
}

BOOST_AUTO_TEST_CASE(set_get_r5g6b5_value)
{
	boost::mt19937 rng;
	boost::uniform_int<Uint16> range(0, 255);
	boost::variate_generator<boost::mt19937&,
		boost::uniform_int<Uint16> > random_int(rng, range);
	el::ImageSharedPtr image;
	std::vector<Uvec4Array2> values;
	Uvec4Array2 value;
	glm::uvec3 color;
	glm::uvec3 size;
	Uint32 mipmap_count, x, y, z, f, m, data, i;
	bool cube_map;

	size.x = 64;
	size.y = 64;
	size.z = 64;
	cube_map = (random_int() % 2) == 0;

	mipmap_count = 6;

	image = boost::make_shared<el::Image>(el::String("image"),
		cube_map, el::tft_r5g6b5, size, mipmap_count, false);

	BOOST_CHECK_EQUAL(image->get_cube_map(), cube_map);
	BOOST_CHECK_EQUAL(image->get_texture_format(), el::tft_r5g6b5);
	BOOST_CHECK_EQUAL(image->get_size().x, size.x);
	BOOST_CHECK_EQUAL(image->get_size().y, size.y);
	BOOST_CHECK_EQUAL(image->get_size().z, size.z);
	BOOST_CHECK_EQUAL(image->get_width(), size.x);
	BOOST_CHECK_EQUAL(image->get_height(), size.y);
	BOOST_CHECK_EQUAL(image->get_depth(), size.z);
	BOOST_CHECK_EQUAL(image->get_mipmap_count(), mipmap_count);

	for (i = 0; i < 128; ++i)
	{
		x = random_int() % 64;
		y = random_int() % 64;
		z = random_int() % 64;
		m = random_int() % 6;

		x = x >> m;
		y = y >> m;
		z = z >> m;

		if (cube_map)
		{
			f = random_int() % 6;
		}
		else
		{
			f = 0;
		}

		color.r = random_int() % 32;
		color.g = random_int() % 64;
		color.b = random_int() % 32;

		data = el::PackTool::pack_uint_5_6_5(false, glm::vec3(color));

		image->set_pixel_uint(x, y, z, f, m, glm::uvec4(color, 0));

		BOOST_CHECK_EQUAL(image->get_pixel_uint(x, y, z, f, m).r,
			color.r);
		BOOST_CHECK_EQUAL(image->get_pixel_uint(x, y, z, f, m).g,
			color.g);
		BOOST_CHECK_EQUAL(image->get_pixel_uint(x, y, z, f, m).b,
			color.b);

		BOOST_CHECK_CLOSE(image->get_pixel(x, y, z, f, m).r,
			color.r / 31.0f, 1.0);
		BOOST_CHECK_CLOSE(image->get_pixel(x, y, z, f, m).g,
			color.g / 63.0f, 1.0);
		BOOST_CHECK_CLOSE(image->get_pixel(x, y, z, f, m).b,
			color.b / 31.0f, 1.0);

		BOOST_CHECK_EQUAL(image->get_pixel_packed_uint16(x, y, z, f, m),
			data);
	}

	for (i = 0; i < 128; ++i)
	{
		x = random_int() % 64;
		y = random_int() % 64;
		z = random_int() % 64;
		m = random_int() % 6;

		x = x >> m;
		y = y >> m;
		z = z >> m;

		if (cube_map)
		{
			f = random_int() % 6;
		}
		else
		{
			f = 0;
		}

		color.r = random_int() % 32;
		color.g = random_int() % 64;
		color.b = random_int() % 32;

		data = el::PackTool::pack_uint_5_6_5(false, glm::vec3(color));

		image->set_pixel_packed_uint16(x, y, z, f, m, data);

		BOOST_CHECK_EQUAL(image->get_pixel_uint(x, y, z, f, m).r,
			color.r);
		BOOST_CHECK_EQUAL(image->get_pixel_uint(x, y, z, f, m).g,
			color.g);
		BOOST_CHECK_EQUAL(image->get_pixel_uint(x, y, z, f, m).b,
			color.b);

		BOOST_CHECK_CLOSE(image->get_pixel(x, y, z, f, m).r,
			color.r / 31.0f, 1.0);
		BOOST_CHECK_CLOSE(image->get_pixel(x, y, z, f, m).g,
			color.g / 63.0f, 1.0);
		BOOST_CHECK_CLOSE(image->get_pixel(x, y, z, f, m).b,
			color.b / 31.0f, 1.0);

		BOOST_CHECK_EQUAL(image->get_pixel_packed_uint16(x, y, z, f, m),
			data);
	}
}

BOOST_AUTO_TEST_CASE(set_get_rgb5_a1_value)
{
	boost::mt19937 rng;
	boost::uniform_int<Uint16> range(0, 255);
	boost::variate_generator<boost::mt19937&,
		boost::uniform_int<Uint16> > random_int(rng, range);
	el::ImageSharedPtr image;
	std::vector<Uvec4Array2> values;
	Uvec4Array2 value;
	glm::uvec4 color;
	glm::uvec3 size;
	Uint32 mipmap_count, x, y, z, f, m, data, i;
	bool cube_map;

	size.x = 64;
	size.y = 64;
	size.z = 64;
	cube_map = (random_int() % 2) == 0;

	mipmap_count = 6;

	image = boost::make_shared<el::Image>(el::String("image"),
		cube_map, el::tft_rgb5_a1, size, mipmap_count, false);

	BOOST_CHECK_EQUAL(image->get_cube_map(), cube_map);
	BOOST_CHECK_EQUAL(image->get_texture_format(), el::tft_rgb5_a1);
	BOOST_CHECK_EQUAL(image->get_size().x, size.x);
	BOOST_CHECK_EQUAL(image->get_size().y, size.y);
	BOOST_CHECK_EQUAL(image->get_size().z, size.z);
	BOOST_CHECK_EQUAL(image->get_width(), size.x);
	BOOST_CHECK_EQUAL(image->get_height(), size.y);
	BOOST_CHECK_EQUAL(image->get_depth(), size.z);
	BOOST_CHECK_EQUAL(image->get_mipmap_count(), mipmap_count);

	for (i = 0; i < 128; ++i)
	{
		x = random_int() % 64;
		y = random_int() % 64;
		z = random_int() % 64;
		m = random_int() % 6;

		x = x >> m;
		y = y >> m;
		z = z >> m;

		if (cube_map)
		{
			f = random_int() % 6;
		}
		else
		{
			f = 0;
		}

		color.r = random_int() % 32;
		color.g = random_int() % 32;
		color.b = random_int() % 32;
		color.a = random_int() % 2;

		data = el::PackTool::pack_uint_5_5_5_1(false, glm::vec4(color));

		image->set_pixel_uint(x, y, z, f, m, color);

		BOOST_CHECK_EQUAL(image->get_pixel_uint(x, y, z, f, m).r,
			color.r);
		BOOST_CHECK_EQUAL(image->get_pixel_uint(x, y, z, f, m).g,
			color.g);
		BOOST_CHECK_EQUAL(image->get_pixel_uint(x, y, z, f, m).b,
			color.b);
		BOOST_CHECK_EQUAL(image->get_pixel_uint(x, y, z, f, m).a,
			color.a);

		BOOST_CHECK_CLOSE(image->get_pixel(x, y, z, f, m).r,
			color.r / 31.0f, 1.0);
		BOOST_CHECK_CLOSE(image->get_pixel(x, y, z, f, m).g,
			color.g / 31.0f, 1.0);
		BOOST_CHECK_CLOSE(image->get_pixel(x, y, z, f, m).b,
			color.b / 31.0f, 1.0);
		BOOST_CHECK_CLOSE(image->get_pixel(x, y, z, f, m).a,
			color.a / 1.0f, 1.0);

		BOOST_CHECK_EQUAL(image->get_pixel_packed_uint16(x, y, z, f, m),
			data);
	}

	for (i = 0; i < 128; ++i)
	{
		x = random_int() % 64;
		y = random_int() % 64;
		z = random_int() % 64;
		m = random_int() % 6;

		x = x >> m;
		y = y >> m;
		z = z >> m;

		if (cube_map)
		{
			f = random_int() % 6;
		}
		else
		{
			f = 0;
		}

		color.r = random_int() % 32;
		color.g = random_int() % 32;
		color.b = random_int() % 32;
		color.a = random_int() % 2;

		data = el::PackTool::pack_uint_5_5_5_1(false, glm::vec4(color));

		image->set_pixel_packed_uint16(x, y, z, f, m, data);

		BOOST_CHECK_EQUAL(image->get_pixel_uint(x, y, z, f, m).r,
			color.r);
		BOOST_CHECK_EQUAL(image->get_pixel_uint(x, y, z, f, m).g,
			color.g);
		BOOST_CHECK_EQUAL(image->get_pixel_uint(x, y, z, f, m).b,
			color.b);
		BOOST_CHECK_EQUAL(image->get_pixel_uint(x, y, z, f, m).a,
			color.a);

		BOOST_CHECK_CLOSE(image->get_pixel(x, y, z, f, m).r,
			color.r / 31.0f, 1.0);
		BOOST_CHECK_CLOSE(image->get_pixel(x, y, z, f, m).g,
			color.g / 31.0f, 1.0);
		BOOST_CHECK_CLOSE(image->get_pixel(x, y, z, f, m).b,
			color.b / 31.0f, 1.0);
		BOOST_CHECK_CLOSE(image->get_pixel(x, y, z, f, m).a,
			color.a / 1.0f, 1.0);

		BOOST_CHECK_EQUAL(image->get_pixel_packed_uint16(x, y, z, f, m),
			data);
	}
}

BOOST_AUTO_TEST_SUITE_END()
