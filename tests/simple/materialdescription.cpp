/****************************************************************************
 *            materialdescription.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "prerequisites.hpp"
#include "materialdescription.hpp"
#include "xmlbuffer.hpp"
#include "xmlwriter.hpp"
#include "xmlreader.hpp"
#define BOOST_TEST_MODULE material_description
#include <boost/test/unit_test.hpp>

namespace el = eternal_lands;

BOOST_AUTO_TEST_CASE(default_creation)
{
	el::MaterialDescription material_description;

	BOOST_CHECK_EQUAL(material_description.get_texture(el::stt_albedo_0),
		"");
	BOOST_CHECK_EQUAL(material_description.get_texture(el::stt_albedo_1),
		"");
	BOOST_CHECK_EQUAL(material_description.get_texture(el::stt_albedo_2),
		"");
	BOOST_CHECK_EQUAL(material_description.get_texture(el::stt_albedo_3),
		"");
	BOOST_CHECK_EQUAL(material_description.get_texture(el::stt_normal_0),
		"");
	BOOST_CHECK_EQUAL(material_description.get_texture(el::stt_normal_1),
		"");
	BOOST_CHECK_EQUAL(material_description.get_texture(el::stt_specular_0),
		"");
	BOOST_CHECK_EQUAL(material_description.get_texture(el::stt_specular_1),
		"");
	BOOST_CHECK_EQUAL(material_description.get_texture(el::stt_emission_0),
		"");
	BOOST_CHECK_EQUAL(material_description.get_texture(el::stt_emission_1),
		"");
	BOOST_CHECK_EQUAL(material_description.get_texture(el::stt_blend_0),
		"");
	BOOST_CHECK_EQUAL(material_description.get_texture(el::stt_blend_1),
		"");
	BOOST_CHECK_EQUAL(material_description.get_texture(el::stt_height),
		"");
	BOOST_CHECK_EQUAL(material_description.get_texture(el::stt_dudv),
		"");
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(0
		)[0][0], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(0
		)[0][1], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(0
		)[0][2], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(0
		)[0][3], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(0
		)[1][0], 0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(0
		)[1][1], 0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(0
		)[1][2], 0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(0
		)[1][3], 0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(1
		)[0][0], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(1
		)[0][1], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(1
		)[0][2], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(1
		)[0][3], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(1
		)[1][0], 0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(1
		)[1][1], 0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(1
		)[1][2], 0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(1
		)[1][3], 0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(2
		)[0][0], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(2
		)[0][1], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(2
		)[0][2], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(2
		)[0][3], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(2
		)[1][0], 0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(2
		)[1][1], 0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(2
		)[1][2], 0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(2
		)[1][3], 0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(3
		)[0][0], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(3
		)[0][1], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(3
		)[0][2], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(3
		)[0][3], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(3
		)[1][0], 0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(3
		)[1][1], 0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(3
		)[1][2], 0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(3
		)[1][3], 0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_texture_matrix(0)[0][0],
		1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_texture_matrix(0)[0][1],
		0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_texture_matrix(0)[0][2],
		0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_texture_matrix(0)[1][0],
		0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_texture_matrix(0)[1][1],
		1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_texture_matrix(0)[1][2],
		0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_texture_matrix(1)[0][0],
		1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_texture_matrix(1)[0][1],
		0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_texture_matrix(1)[0][2],
		0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_texture_matrix(1)[1][0],
		0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_texture_matrix(1)[1][1],
		1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_texture_matrix(1)[1][2],
		0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_emission_scale_offset(
		)[0][0], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_emission_scale_offset(
		)[0][1], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_emission_scale_offset(
		)[0][2], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_emission_scale_offset(
		)[1][0], 0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_emission_scale_offset(
		)[1][1], 0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_emission_scale_offset(
		)[1][2], 0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_specular_scale_offset(
		)[0], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_specular_scale_offset(
		)[1], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_specular_scale_offset(
		)[2], 0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_specular_scale_offset(
		)[3], 0.0f, 1.0);
	BOOST_CHECK_EQUAL(material_description.get_name(), "");
	BOOST_CHECK_EQUAL(material_description.get_effect(), "");
	BOOST_CHECK_EQUAL(material_description.get_script(), "");
	BOOST_CHECK_EQUAL(material_description.get_cast_shadows(), true);
	BOOST_CHECK_EQUAL(material_description.get_culling(), true);
}

BOOST_AUTO_TEST_CASE(all)
{
	el::MaterialDescription material_description;

	BOOST_CHECK_NO_THROW(material_description.set_texture(
		el::String(UTF8("df")), el::stt_albedo_0));
	BOOST_CHECK_NO_THROW(material_description.set_texture(
		el::String(UTF8("agaghrt")), el::stt_albedo_1));
	BOOST_CHECK_NO_THROW(material_description.set_texture(
		el::String(UTF8("5z5")), el::stt_albedo_2));
	BOOST_CHECK_NO_THROW(material_description.set_texture(
		el::String(UTF8("wj")), el::stt_albedo_3));
	BOOST_CHECK_NO_THROW(material_description.set_texture(
		el::String(UTF8("m120")), el::stt_normal_0));
	BOOST_CHECK_NO_THROW(material_description.set_texture(
		el::String(UTF8("aw79dz")), el::stt_normal_1));
	BOOST_CHECK_NO_THROW(material_description.set_texture(
		el::String(UTF8("y<bnj3e")), el::stt_specular_0));
	BOOST_CHECK_NO_THROW(material_description.set_texture(
		el::String(UTF8("48fh")), el::stt_specular_1));
	BOOST_CHECK_NO_THROW(material_description.set_texture(
		el::String(UTF8("ymbnjf!2r2!A")), el::stt_emission_0));
	BOOST_CHECK_NO_THROW(material_description.set_texture(
		el::String(UTF8("SDFGSGDF")), el::stt_emission_1));
	BOOST_CHECK_NO_THROW(material_description.set_texture(
		el::String(UTF8("345w")), el::stt_blend_0));
	BOOST_CHECK_NO_THROW(material_description.set_texture(
		el::String(UTF8("fs46wAFED§&%")), el::stt_blend_1));
	BOOST_CHECK_NO_THROW(material_description.set_texture(
		el::String(UTF8("2556yvc23")), el::stt_height));
	BOOST_CHECK_NO_THROW(material_description.set_texture(
		el::String(UTF8(".,welrf43")), el::stt_dudv));

	BOOST_CHECK_EQUAL(material_description.get_texture(el::stt_albedo_0),
		"df");
	BOOST_CHECK_EQUAL(material_description.get_texture(el::stt_albedo_1),
		"agaghrt");
	BOOST_CHECK_EQUAL(material_description.get_texture(el::stt_albedo_2),
		"5z5");
	BOOST_CHECK_EQUAL(material_description.get_texture(el::stt_albedo_3),
		"wj");
	BOOST_CHECK_EQUAL(material_description.get_texture(el::stt_normal_0),
		"m120");
	BOOST_CHECK_EQUAL(material_description.get_texture(el::stt_normal_1),	
		"aw79dz");
	BOOST_CHECK_EQUAL(material_description.get_texture(el::stt_specular_0),
		"y<bnj3e");
	BOOST_CHECK_EQUAL(material_description.get_texture(el::stt_specular_1),
		"48fh");
	BOOST_CHECK_EQUAL(material_description.get_texture(el::stt_emission_0),
		"ymbnjf!2r2!A");
	BOOST_CHECK_EQUAL(material_description.get_texture(el::stt_emission_1),
		"SDFGSGDF");
	BOOST_CHECK_EQUAL(material_description.get_texture(el::stt_blend_0),
		"345w");
	BOOST_CHECK_EQUAL(material_description.get_texture(el::stt_blend_1),
		"fs46wAFED§&%");
	BOOST_CHECK_EQUAL(material_description.get_texture(el::stt_height),
		"2556yvc23");
	BOOST_CHECK_EQUAL(material_description.get_texture(el::stt_dudv),
		".,welrf43");
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(0
		)[0][0], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(0
		)[0][1], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(0
		)[0][2], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(0
		)[0][3], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(0
		)[1][0], 0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(0
		)[1][1], 0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(0
		)[1][2], 0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(0
		)[1][3], 0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(1
		)[0][0], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(1
		)[0][1], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(1
		)[0][2], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(1
		)[0][3], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(1
		)[1][0], 0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(1
		)[1][1], 0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(1
		)[1][2], 0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(1
		)[1][3], 0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(2
		)[0][0], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(2
		)[0][1], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(2
		)[0][2], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(2
		)[0][3], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(2
		)[1][0], 0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(2
		)[1][1], 0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(2
		)[1][2], 0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(2
		)[1][3], 0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(3
		)[0][0], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(3
		)[0][1], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(3
		)[0][2], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(3
		)[0][3], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(3
		)[1][0], 0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(3
		)[1][1], 0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(3
		)[1][2], 0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(3
		)[1][3], 0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_texture_matrix(0)[0][0],
		1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_texture_matrix(0)[0][1],
		0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_texture_matrix(0)[0][2],
		0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_texture_matrix(0)[1][0],
		0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_texture_matrix(0)[1][1],
		1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_texture_matrix(0)[1][2],
		0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_texture_matrix(1)[0][0],
		1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_texture_matrix(1)[0][1],
		0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_texture_matrix(1)[0][2],
		0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_texture_matrix(1)[1][0],
		0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_texture_matrix(1)[1][1],
		1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_texture_matrix(1)[1][2],
		0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_emission_scale_offset(
		)[0][0], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_emission_scale_offset(
		)[0][1], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_emission_scale_offset(
		)[0][2], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_emission_scale_offset(
		)[1][0], 0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_emission_scale_offset(
		)[1][1], 0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_emission_scale_offset(
		)[1][2], 0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_specular_scale_offset(
		)[0], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_specular_scale_offset(
		)[1], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_specular_scale_offset(
		)[2], 0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_specular_scale_offset(
		)[3], 0.0f, 1.0);
	BOOST_CHECK_EQUAL(material_description.get_name(), "");
	BOOST_CHECK_EQUAL(material_description.get_effect(), "");
	BOOST_CHECK_EQUAL(material_description.get_script(), "");
	BOOST_CHECK_EQUAL(material_description.get_cast_shadows(), true);
	BOOST_CHECK_EQUAL(material_description.get_culling(), true);

	BOOST_CHECK_NO_THROW(material_description.set_name(
		el::String(UTF8("afdgsr!§$AFD54"))));
	BOOST_CHECK_NO_THROW(material_description.set_effect(
		el::String(UTF8("428rfu0sr4v"))));
	BOOST_CHECK_NO_THROW(material_description.set_script(
		el::String(UTF8("345078rgt34tewr32"))));
	BOOST_CHECK_NO_THROW(material_description.set_cast_shadows(false));
	BOOST_CHECK_NO_THROW(material_description.set_culling(false));

	BOOST_CHECK_EQUAL(material_description.get_texture(el::stt_albedo_0),
		"df");
	BOOST_CHECK_EQUAL(material_description.get_texture(el::stt_albedo_1),
		"agaghrt");
	BOOST_CHECK_EQUAL(material_description.get_texture(el::stt_albedo_2),
		"5z5");
	BOOST_CHECK_EQUAL(material_description.get_texture(el::stt_albedo_3),
		"wj");
	BOOST_CHECK_EQUAL(material_description.get_texture(el::stt_normal_0),
		"m120");
	BOOST_CHECK_EQUAL(material_description.get_texture(el::stt_normal_1),	
		"aw79dz");
	BOOST_CHECK_EQUAL(material_description.get_texture(el::stt_specular_0),
		"y<bnj3e");
	BOOST_CHECK_EQUAL(material_description.get_texture(el::stt_specular_1),
		"48fh");
	BOOST_CHECK_EQUAL(material_description.get_texture(el::stt_emission_0),
		"ymbnjf!2r2!A");
	BOOST_CHECK_EQUAL(material_description.get_texture(el::stt_emission_1),
		"SDFGSGDF");
	BOOST_CHECK_EQUAL(material_description.get_texture(el::stt_blend_0),
		"345w");
	BOOST_CHECK_EQUAL(material_description.get_texture(el::stt_blend_1),
		"fs46wAFED§&%");
	BOOST_CHECK_EQUAL(material_description.get_texture(el::stt_height),
		"2556yvc23");
	BOOST_CHECK_EQUAL(material_description.get_texture(el::stt_dudv),
		".,welrf43");
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(0
		)[0][0], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(0
		)[0][1], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(0
		)[0][2], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(0
		)[0][3], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(0
		)[1][0], 0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(0
		)[1][1], 0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(0
		)[1][2], 0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(0
		)[1][3], 0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(1
		)[0][0], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(1
		)[0][1], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(1
		)[0][2], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(1
		)[0][3], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(1
		)[1][0], 0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(1
		)[1][1], 0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(1
		)[1][2], 0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(1
		)[1][3], 0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(2
		)[0][0], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(2
		)[0][1], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(2
		)[0][2], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(2
		)[0][3], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(2
		)[1][0], 0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(2
		)[1][1], 0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(2
		)[1][2], 0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(2
		)[1][3], 0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(3
		)[0][0], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(3
		)[0][1], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(3
		)[0][2], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(3
		)[0][3], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(3
		)[1][0], 0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(3
		)[1][1], 0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(3
		)[1][2], 0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(3
		)[1][3], 0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_texture_matrix(0)[0][0],
		1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_texture_matrix(0)[0][1],
		0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_texture_matrix(0)[0][2],
		0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_texture_matrix(0)[0][0],
		1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_texture_matrix(0)[0][1],
		0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_texture_matrix(0)[0][2],
		0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_texture_matrix(0)[1][0],
		0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_texture_matrix(0)[1][1],
		1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_texture_matrix(0)[1][2],
		0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_texture_matrix(1)[0][0],
		1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_texture_matrix(1)[0][1],
		0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_texture_matrix(1)[0][2],
		0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_texture_matrix(1)[1][0],
		0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_texture_matrix(1)[1][1],
		1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_texture_matrix(1)[1][2],
		0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_emission_scale_offset(
		)[0][0], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_emission_scale_offset(
		)[0][1], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_emission_scale_offset(
		)[0][2], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_emission_scale_offset(
		)[1][0], 0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_emission_scale_offset(
		)[1][1], 0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_emission_scale_offset(
		)[1][2], 0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_specular_scale_offset(
		)[0], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_specular_scale_offset(
		)[1], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_specular_scale_offset(
		)[2], 0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_specular_scale_offset(
		)[3], 0.0f, 1.0);
	BOOST_CHECK_EQUAL(material_description.get_name(),
		"afdgsr!§$AFD54");
	BOOST_CHECK_EQUAL(material_description.get_effect(),
		"428rfu0sr4v");
	BOOST_CHECK_EQUAL(material_description.get_script(),
		"345078rgt34tewr32");
	BOOST_CHECK_EQUAL(material_description.get_cast_shadows(), false);
	BOOST_CHECK_EQUAL(material_description.get_culling(), false);

	BOOST_CHECK_NO_THROW(material_description.set_texture(
		el::String(UTF8("df")), el::stt_albedo_0));
	BOOST_CHECK_NO_THROW(material_description.set_texture(
		el::String(UTF8("agaghrt")), el::stt_albedo_1));
	BOOST_CHECK_NO_THROW(material_description.set_texture(
		el::String(UTF8("5z5")), el::stt_albedo_2));
	BOOST_CHECK_NO_THROW(material_description.set_texture(
		el::String(UTF8("wj")), el::stt_albedo_3));
	BOOST_CHECK_NO_THROW(material_description.set_texture(
		el::String(UTF8("m120")), el::stt_normal_0));
	BOOST_CHECK_NO_THROW(material_description.set_texture(
		el::String(UTF8("aw79dz")), el::stt_normal_1));
	BOOST_CHECK_NO_THROW(material_description.set_texture(
		el::String(UTF8("y<bnj3e")), el::stt_specular_0));
	BOOST_CHECK_NO_THROW(material_description.set_texture(
		el::String(UTF8("48fh")), el::stt_specular_1));
	BOOST_CHECK_NO_THROW(material_description.set_texture(
		el::String(UTF8("ymbnjf!2r2!A")), el::stt_emission_0));
	BOOST_CHECK_NO_THROW(material_description.set_texture(
		el::String(UTF8("SDFGSGDF")), el::stt_emission_1));
	BOOST_CHECK_NO_THROW(material_description.set_texture(
		el::String(UTF8("345w")), el::stt_blend_0));
	BOOST_CHECK_NO_THROW(material_description.set_texture(
		el::String(UTF8("fs46wAFED§&%")), el::stt_blend_1));
	BOOST_CHECK_NO_THROW(material_description.set_texture(
		el::String(UTF8("2556yvc23")), el::stt_height));
	BOOST_CHECK_NO_THROW(material_description.set_texture(
		el::String(UTF8(".,welrf43")), el::stt_dudv));
	BOOST_CHECK_NO_THROW(material_description.set_texture_matrix(
		glm::mat2x3(0.6787f, 4.342f, 345.0f, 76.0f, 3124.0f, 123.0f),
			0));
	BOOST_CHECK_NO_THROW(material_description.set_texture_matrix(
		glm::mat2x3(0.356f, 43.3f, 5.0f, 176.0f, -24.0f, 13.0f), 1));
	BOOST_CHECK_NO_THROW(material_description.set_specular_scale_offset(
		glm::vec4(-1.7f, -4.0f, -15.0f, -21.0f)));
	BOOST_CHECK_NO_THROW(material_description.set_name(
		el::String(UTF8("9230fsn45,.e.,"))));
	BOOST_CHECK_NO_THROW(material_description.set_effect(
		el::String(UTF8("afdgsr!§$AFD54"))));
	BOOST_CHECK_NO_THROW(material_description.set_script(
		el::String(UTF8("3453=)/&vrf"))));
	BOOST_CHECK_NO_THROW(material_description.set_cast_shadows(false));
	BOOST_CHECK_NO_THROW(material_description.set_culling(false));

	BOOST_CHECK_EQUAL(material_description.get_texture(el::stt_albedo_0),
		"df");
	BOOST_CHECK_EQUAL(material_description.get_texture(el::stt_albedo_1),
		"agaghrt");
	BOOST_CHECK_EQUAL(material_description.get_texture(el::stt_albedo_2),
		"5z5");
	BOOST_CHECK_EQUAL(material_description.get_texture(el::stt_albedo_3),
		"wj");
	BOOST_CHECK_EQUAL(material_description.get_texture(el::stt_normal_0),
		"m120");
	BOOST_CHECK_EQUAL(material_description.get_texture(el::stt_normal_1),	
		"aw79dz");
	BOOST_CHECK_EQUAL(material_description.get_texture(el::stt_specular_0),
		"y<bnj3e");
	BOOST_CHECK_EQUAL(material_description.get_texture(el::stt_specular_1),
		"48fh");
	BOOST_CHECK_EQUAL(material_description.get_texture(el::stt_emission_0),
		"ymbnjf!2r2!A");
	BOOST_CHECK_EQUAL(material_description.get_texture(el::stt_emission_1),
		"SDFGSGDF");
	BOOST_CHECK_EQUAL(material_description.get_texture(el::stt_blend_0),
		"345w");
	BOOST_CHECK_EQUAL(material_description.get_texture(el::stt_blend_1),
		"fs46wAFED§&%");
	BOOST_CHECK_EQUAL(material_description.get_texture(el::stt_height),
		"2556yvc23");
	BOOST_CHECK_EQUAL(material_description.get_texture(el::stt_dudv),
		".,welrf43");
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(0
		)[0][0], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(0
		)[0][1], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(0
		)[0][2], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(0
		)[0][3], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(0
		)[1][0], 0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(0
		)[1][1], 0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(0
		)[1][2], 0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(0
		)[1][3], 0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(1
		)[0][0], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(1
		)[0][1], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(1
		)[0][2], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(1
		)[0][3], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(1
		)[1][0], 0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(1
		)[1][1], 0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(1
		)[1][2], 0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(1
		)[1][3], 0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(2
		)[0][0], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(2
		)[0][1], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(2
		)[0][2], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(2
		)[0][3], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(2
		)[1][0], 0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(2
		)[1][1], 0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(2
		)[1][2], 0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(2
		)[1][3], 0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(3
		)[0][0], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(3
		)[0][1], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(3
		)[0][2], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(3
		)[0][3], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(3
		)[1][0], 0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(3
		)[1][1], 0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(3
		)[1][2], 0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(3
		)[1][3], 0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_texture_matrix(0)[0][0],
		0.6787f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_texture_matrix(0)[0][1],
		4.342f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_texture_matrix(0)[0][2],
		345.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_texture_matrix(0)[1][0],
		76.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_texture_matrix(0)[1][1],
		3124.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_texture_matrix(0)[1][2],
		123.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_texture_matrix(1)[0][0],
		0.356f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_texture_matrix(1)[0][1],
		43.3f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_texture_matrix(1)[0][2],
		5.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_texture_matrix(1)[1][0],
		176.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_texture_matrix(1)[1][1],
		-24.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_texture_matrix(1)[1][2],
		13.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_emission_scale_offset(
		)[0][0], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_emission_scale_offset(
		)[0][1], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_emission_scale_offset(
		)[0][2], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_emission_scale_offset(
		)[1][0], 0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_emission_scale_offset(
		)[1][1], 0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_emission_scale_offset(
		)[1][2], 0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_specular_scale_offset(
		)[0], -1.7f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_specular_scale_offset(
		)[1], -4.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_specular_scale_offset(
		)[2], -15.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_specular_scale_offset(
		)[3], -21.0f, 1.0);
	BOOST_CHECK_EQUAL(material_description.get_name(), "9230fsn45,.e.,");
	BOOST_CHECK_EQUAL(material_description.get_effect(), "afdgsr!§$AFD54");
	BOOST_CHECK_EQUAL(material_description.get_script(), "3453=)/&vrf");
	BOOST_CHECK_EQUAL(material_description.get_cast_shadows(), false);
	BOOST_CHECK_EQUAL(material_description.get_culling(), false);
}

BOOST_AUTO_TEST_CASE(xml)
{
	el::XmlBuffer buffer(UTF8("<?xml version=\"1.0\" encoding=\"utf8\"?>"
		"<material><name>tada</name><albedo_0>df</albedo_0>"
		"<albedo_1>agaghrt</albedo_1><albedo_2>5z5</albedo_2>"
		"<albedo_3>wj</albedo_3><normal>m120</normal>"
		"<specular>y&lt;bnj3e</specular>"
		"<emission>ymbnjf!2r2!A</emission><blend>345w</blend>"
		"<height>2556yvc23</height>"
		"<texture_matrix_0>0.6787 4.342 345.0 76.0 3124.0 123.0</texture_matrix_0>"
		"<texture_matrix_1>0.356 43.3 5.0 176.0 -24.0 13.0</texture_matrix_1>"
		"<specular_scale_offset>-1.7 -4.0 -15.0 -21.0</specular_scale_offset>"
		"<dudv_scale>-1.7 -4.0</dudv_scale>"
		"<effect>afdgsr!§$AFD54</effect>"
		"<script>efw5463t4%§</script>"
		"<cast_shadows>false</cast_shadows>"
		"<culling>false</culling></material>"));
	el::MaterialDescription material_description;
	el::XmlReaderSharedPtr xml_reader;

	xml_reader = el::XmlReaderSharedPtr(new el::XmlReader(
		buffer.get_buffer()));

	BOOST_CHECK_NO_THROW(material_description.load_xml(
 		xml_reader->get_root_node()));

	xml_reader.reset();

	BOOST_CHECK_EQUAL(material_description.get_texture(el::stt_albedo_0),
		"df");
	BOOST_CHECK_EQUAL(material_description.get_texture(el::stt_albedo_1),
		"agaghrt");
	BOOST_CHECK_EQUAL(material_description.get_texture(el::stt_albedo_2),
		"5z5");
	BOOST_CHECK_EQUAL(material_description.get_texture(el::stt_albedo_3),
		"wj");
	BOOST_CHECK_EQUAL(material_description.get_texture(el::stt_normal_0),
		"m120");
	BOOST_CHECK_EQUAL(material_description.get_texture(el::stt_normal_1),	
		"");
	BOOST_CHECK_EQUAL(material_description.get_texture(el::stt_specular_0),
		"y<bnj3e");
	BOOST_CHECK_EQUAL(material_description.get_texture(el::stt_specular_1),
		"");
	BOOST_CHECK_EQUAL(material_description.get_texture(el::stt_emission_0),
		"ymbnjf!2r2!A");
	BOOST_CHECK_EQUAL(material_description.get_texture(el::stt_emission_1),
		"");
	BOOST_CHECK_EQUAL(material_description.get_texture(el::stt_blend_0),
		"345w");
	BOOST_CHECK_EQUAL(material_description.get_texture(el::stt_blend_1),
		"");
	BOOST_CHECK_EQUAL(material_description.get_texture(el::stt_height),
		"2556yvc23");
	BOOST_CHECK_EQUAL(material_description.get_texture(el::stt_dudv),
		"");
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(0
		)[0][0], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(0
		)[0][1], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(0
		)[0][2], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(0
		)[0][3], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(0
		)[1][0], 0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(0
		)[1][1], 0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(0
		)[1][2], 0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(0
		)[1][3], 0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(1
		)[0][0], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(1
		)[0][1], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(1
		)[0][2], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(1
		)[0][3], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(1
		)[1][0], 0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(1
		)[1][1], 0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(1
		)[1][2], 0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(1
		)[1][3], 0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(2
		)[0][0], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(2
		)[0][1], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(2
		)[0][2], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(2
		)[0][3], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(2
		)[1][0], 0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(2
		)[1][1], 0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(2
		)[1][2], 0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(2
		)[1][3], 0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(3
		)[0][0], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(3
		)[0][1], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(3
		)[0][2], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(3
		)[0][3], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(3
		)[1][0], 0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(3
		)[1][1], 0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(3
		)[1][2], 0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_albedo_scale_offset(3
		)[1][3], 0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_texture_matrix(0)[0][0],
		0.6787f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_texture_matrix(0)[0][1],
		4.342f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_texture_matrix(0)[0][2],
		345.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_texture_matrix(0)[1][0],
		76.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_texture_matrix(0)[1][1],
		3124.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_texture_matrix(0)[1][2],
		123.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_texture_matrix(1)[0][0],
		0.356f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_texture_matrix(1)[0][1],
		43.3f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_texture_matrix(1)[0][2],
		5.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_texture_matrix(1)[1][0],
		176.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_texture_matrix(1)[1][1],
		-24.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_texture_matrix(1)[1][2],
		13.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_emission_scale_offset(
		)[0][0], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_emission_scale_offset(
		)[0][1], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_emission_scale_offset(
		)[0][2], 1.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_emission_scale_offset(
		)[1][0], 0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_emission_scale_offset(
		)[1][1], 0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_emission_scale_offset(
		)[1][2], 0.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_specular_scale_offset(
		)[0], -1.7f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_specular_scale_offset(
		)[1], -4.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_specular_scale_offset(
		)[2], -15.0f, 1.0);
	BOOST_CHECK_CLOSE(material_description.get_specular_scale_offset(
		)[3], -21.0f, 1.0);
	BOOST_CHECK_EQUAL(material_description.get_name(), "tada");
	BOOST_CHECK_EQUAL(material_description.get_effect(), "afdgsr!§$AFD54");
	BOOST_CHECK_EQUAL(material_description.get_script(), "efw5463t4%§");
	BOOST_CHECK_EQUAL(material_description.get_cast_shadows(), false);
	BOOST_CHECK_EQUAL(material_description.get_culling(), false);
}
