/****************************************************************************
 *            shadertexture.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "prerequisites.hpp"
#include "exceptions.hpp"
#include "shader/shadertextureutil.hpp"
#include "utf.hpp"
#define BOOST_TEST_MODULE shader_texture
#include <boost/test/unit_test.hpp>

namespace el = eternal_lands;

BOOST_AUTO_TEST_CASE(get_str)
{
	Uint32 i;

	for (i = 0; i < el::ShaderTextureUtil::get_shader_texture_count(); i++)
	{
		BOOST_CHECK_NO_THROW(el::ShaderTextureUtil::get_str(
			static_cast<el::ShaderTextureType>(i)));
	}
}

BOOST_AUTO_TEST_CASE(convert)
{
	Uint32 i;
	el::ShaderTextureType type;

	for (i = 0; i < el::ShaderTextureUtil::get_shader_texture_count(); i++)
	{
		type = static_cast<el::ShaderTextureType>(i);

		BOOST_CHECK_EQUAL(el::ShaderTextureUtil::get_shader_texture(
			el::ShaderTextureUtil::get_str(type)), type);
	}
}

BOOST_AUTO_TEST_CASE(get_str_value)
{
	BOOST_CHECK_EQUAL(el::ShaderTextureUtil::get_str(
		el::stt_diffuse_0).get(), "diffuse_sampler_0");
	BOOST_CHECK_EQUAL(el::ShaderTextureUtil::get_str(
		el::stt_diffuse_3).get(), "diffuse_sampler_3");
	BOOST_CHECK_EQUAL(el::ShaderTextureUtil::get_str(
		el::stt_emission_0).get(), "emission_sampler_0");
	BOOST_CHECK_EQUAL(el::ShaderTextureUtil::get_str(
		el::stt_blend_1).get(), "blend_sampler_1");
	BOOST_CHECK_EQUAL(el::ShaderTextureUtil::get_str(
		el::stt_shadow).get(), "shadow_sampler");
}
