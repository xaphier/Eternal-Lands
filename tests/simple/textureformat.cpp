/****************************************************************************
 *            textureformat.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "prerequisites.hpp"
#include "exceptions.hpp"
#include "textureformatutil.hpp"
#include "utf.hpp"
#define BOOST_TEST_MODULE texture_format
#include <boost/test/unit_test.hpp>

namespace el = eternal_lands;

BOOST_AUTO_TEST_CASE(get_str)
{
	Uint32 i, count;

	count = el::TextureFormatUtil::get_texture_format_count();

	for (i = 0; i < count; ++i)
	{
		BOOST_CHECK_NO_THROW(el::TextureFormatUtil::get_str(
			static_cast<el::TextureFormatType>(i)));
	}
}

BOOST_AUTO_TEST_CASE(convert)
{
	Uint32 i, count;
	el::TextureFormatType type;

	count = el::TextureFormatUtil::get_texture_format_count();

	for (i = 0; i < count; ++i)
	{
		type = static_cast<el::TextureFormatType>(i);

		BOOST_CHECK_EQUAL(el::TextureFormatUtil::get_texture_format(
			el::TextureFormatUtil::get_str(type)), type);
	}
}

BOOST_AUTO_TEST_CASE(source_format)
{
	Uint32 i, count;
	el::TextureFormatType type;
	std::set<GLenum> formats, types;
	GLenum gl_format, gl_type;

	count = el::TextureFormatUtil::get_texture_format_count();

	formats.insert(GL_NONE);
	formats.insert(GL_RED);
	formats.insert(GL_RG);
	formats.insert(GL_RGB);
	formats.insert(GL_BGR);
	formats.insert(GL_RGBA);
	formats.insert(GL_BGRA);
	formats.insert(GL_DEPTH_COMPONENT);
	formats.insert(GL_DEPTH_STENCIL);
	formats.insert(GL_ALPHA);
	formats.insert(GL_LUMINANCE);
	formats.insert(GL_LUMINANCE_ALPHA);
	formats.insert(GL_COMPRESSED_RGB_S3TC_DXT1_EXT);
	formats.insert(GL_COMPRESSED_RGBA_S3TC_DXT1_EXT);
	formats.insert(GL_COMPRESSED_RGBA_S3TC_DXT3_EXT);
	formats.insert(GL_COMPRESSED_RGBA_S3TC_DXT5_EXT);
	formats.insert(GL_COMPRESSED_SRGB_S3TC_DXT1_EXT);
	formats.insert(GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT1_EXT);
	formats.insert(GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT3_EXT);
	formats.insert(GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT);
	formats.insert(GL_COMPRESSED_RED_RGTC1);
	formats.insert(GL_COMPRESSED_SIGNED_RED_RGTC1);
	formats.insert(GL_COMPRESSED_RG_RGTC2);
	formats.insert(GL_COMPRESSED_SIGNED_RG_RGTC2);

	types.insert(GL_NONE);
	types.insert(GL_UNSIGNED_BYTE);
	types.insert(GL_UNSIGNED_SHORT);
	types.insert(GL_UNSIGNED_INT);
	types.insert(GL_BYTE);
	types.insert(GL_SHORT);
	types.insert(GL_INT);
	types.insert(GL_HALF_FLOAT);
	types.insert(GL_FLOAT);
	types.insert(GL_UNSIGNED_INT_24_8);
	types.insert(GL_FLOAT_32_UNSIGNED_INT_24_8_REV);
	types.insert(GL_UNSIGNED_BYTE_3_3_2);
	types.insert(GL_UNSIGNED_SHORT_4_4_4_4);
	types.insert(GL_UNSIGNED_SHORT_5_6_5);
	types.insert(GL_UNSIGNED_SHORT_5_5_5_1);
	types.insert(GL_UNSIGNED_INT_10_10_10_2);

	for (i = 0; i < count; ++i)
	{
		type = static_cast<el::TextureFormatType>(i);

		BOOST_CHECK_NO_THROW(el::TextureFormatUtil::get_source_format(
			type, gl_format, gl_type));

		BOOST_CHECK_EQUAL(formats.count(gl_format), 1);
		BOOST_CHECK_EQUAL(types.count(gl_type), 1);
	}
}

BOOST_AUTO_TEST_CASE(get_str_value)
{
	BOOST_CHECK_EQUAL(el::TextureFormatUtil::get_str(el::tft_rgba4).get(),
		"rgba4");
	BOOST_CHECK_EQUAL(el::TextureFormatUtil::get_str(el::tft_rg8).get(),
		"rg8");
	BOOST_CHECK_EQUAL(el::TextureFormatUtil::get_str(el::tft_rgb16).get(),
		"rgb16");
	BOOST_CHECK_EQUAL(el::TextureFormatUtil::get_str(el::tft_r32_ui).get(),
		"r32ui");
	BOOST_CHECK_EQUAL(el::TextureFormatUtil::get_str(el::tft_r16f).get(),
		"r16f");
	BOOST_CHECK_EQUAL(el::TextureFormatUtil::get_str(
		el::tft_rgba_dxt3).get(), "rgba_dxt3");
	BOOST_CHECK_EQUAL(el::TextureFormatUtil::get_str(el::tft_la16).get(),
		"la16");
}
