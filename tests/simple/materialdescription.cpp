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

	BOOST_CHECK_EQUAL(material_description.get_texture(el::stt_diffuse_0),
		"");
	BOOST_CHECK_EQUAL(material_description.get_texture(el::stt_diffuse_1),
		"");
	BOOST_CHECK_EQUAL(material_description.get_texture(el::stt_diffuse_2),
		"");
	BOOST_CHECK_EQUAL(material_description.get_texture(el::stt_diffuse_3),
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
	BOOST_CHECK_EQUAL(material_description.get_texture_coodrinates(), "");
	BOOST_CHECK_EQUAL(material_description.get_diffuse_mapping(), "");
	BOOST_CHECK_EQUAL(material_description.get_normal_mapping(), "");
	BOOST_CHECK_EQUAL(material_description.get_specular_mapping(), "");
	BOOST_CHECK_EQUAL(material_description.get_emission_mapping(), "");
	BOOST_CHECK_EQUAL(material_description.get_receives_shadows(), true);
	BOOST_CHECK_EQUAL(material_description.get_lighting(), true);
}

BOOST_AUTO_TEST_CASE(textures)
{
	el::MaterialDescription material_description;

	BOOST_CHECK_NO_THROW(material_description.set_texture(
		el::String(UTF8("df")), el::stt_diffuse_0));
	BOOST_CHECK_NO_THROW(material_description.set_texture(
		el::String(UTF8("agaghrt")), el::stt_diffuse_1));
	BOOST_CHECK_NO_THROW(material_description.set_texture(
		el::String(UTF8("5z5")), el::stt_diffuse_2));
	BOOST_CHECK_NO_THROW(material_description.set_texture(
		el::String(UTF8("wj")), el::stt_diffuse_3));
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

	BOOST_CHECK_EQUAL(material_description.get_texture(el::stt_diffuse_0),
		"df");
	BOOST_CHECK_EQUAL(material_description.get_texture(el::stt_diffuse_1),
		"agaghrt");
	BOOST_CHECK_EQUAL(material_description.get_texture(el::stt_diffuse_2),
		"5z5");
	BOOST_CHECK_EQUAL(material_description.get_texture(el::stt_diffuse_3),
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
	BOOST_CHECK_EQUAL(material_description.get_texture_coodrinates(), "");
	BOOST_CHECK_EQUAL(material_description.get_diffuse_mapping(), "");
	BOOST_CHECK_EQUAL(material_description.get_normal_mapping(), "");
	BOOST_CHECK_EQUAL(material_description.get_specular_mapping(), "");
	BOOST_CHECK_EQUAL(material_description.get_emission_mapping(), "");
	BOOST_CHECK_EQUAL(material_description.get_receives_shadows(), true);
	BOOST_CHECK_EQUAL(material_description.get_lighting(), true);
}

BOOST_AUTO_TEST_CASE(effect)
{
	el::MaterialDescription material_description;

	BOOST_CHECK_NO_THROW(material_description.set_texture_coodrinates(
		el::String(UTF8("afdgsr!§$AFD54"))));
	BOOST_CHECK_NO_THROW(material_description.set_diffuse_mapping(
		el::String(UTF8("428rfu0sr4v"))));
	BOOST_CHECK_NO_THROW(material_description.set_normal_mapping(
		el::String(UTF8("AST%&%"))));
	BOOST_CHECK_NO_THROW(material_description.set_specular_mapping(
		el::String(UTF8("53456675cas"))));
	BOOST_CHECK_NO_THROW(material_description.set_emission_mapping(
		el::String(UTF8("F%$&GRF3453rf"))));
	BOOST_CHECK_NO_THROW(material_description.set_receives_shadows(false));
	BOOST_CHECK_NO_THROW(material_description.set_lighting(false));

	BOOST_CHECK_EQUAL(material_description.get_texture(el::stt_diffuse_0),
		"");
	BOOST_CHECK_EQUAL(material_description.get_texture(el::stt_diffuse_1),
		"");
	BOOST_CHECK_EQUAL(material_description.get_texture(el::stt_diffuse_2),
		"");
	BOOST_CHECK_EQUAL(material_description.get_texture(el::stt_diffuse_3),
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
	BOOST_CHECK_EQUAL(material_description.get_texture_coodrinates(),
		"afdgsr!§$AFD54");
	BOOST_CHECK_EQUAL(material_description.get_diffuse_mapping(),
		"428rfu0sr4v");
	BOOST_CHECK_EQUAL(material_description.get_normal_mapping(), "AST%&%");
	BOOST_CHECK_EQUAL(material_description.get_specular_mapping(),
		"53456675cas");
	BOOST_CHECK_EQUAL(material_description.get_emission_mapping(),
		"F%$&GRF3453rf");
	BOOST_CHECK_EQUAL(material_description.get_receives_shadows(), false);
	BOOST_CHECK_EQUAL(material_description.get_lighting(), false);
}

BOOST_AUTO_TEST_CASE(all)
{
	el::MaterialDescription material_description;

	BOOST_CHECK_NO_THROW(material_description.set_texture(
		el::String(UTF8("df")), el::stt_diffuse_0));
	BOOST_CHECK_NO_THROW(material_description.set_texture(
		el::String(UTF8("agaghrt")), el::stt_diffuse_1));
	BOOST_CHECK_NO_THROW(material_description.set_texture(
		el::String(UTF8("5z5")), el::stt_diffuse_2));
	BOOST_CHECK_NO_THROW(material_description.set_texture(
		el::String(UTF8("wj")), el::stt_diffuse_3));
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
	BOOST_CHECK_NO_THROW(material_description.set_texture_coodrinates(
		el::String(UTF8("afdgsr!§$AFD54"))));
	BOOST_CHECK_NO_THROW(material_description.set_diffuse_mapping(
		el::String(UTF8("428rfu0sr4v"))));
	BOOST_CHECK_NO_THROW(material_description.set_normal_mapping(
		el::String(UTF8("AST%&%"))));
	BOOST_CHECK_NO_THROW(material_description.set_specular_mapping(
		el::String(UTF8("53456675cas"))));
	BOOST_CHECK_NO_THROW(material_description.set_emission_mapping(
		el::String(UTF8("F%$&GRF3453rf"))));
	BOOST_CHECK_NO_THROW(material_description.set_receives_shadows(false));
	BOOST_CHECK_NO_THROW(material_description.set_lighting(false));

	BOOST_CHECK_EQUAL(material_description.get_texture(el::stt_diffuse_0),
		"df");
	BOOST_CHECK_EQUAL(material_description.get_texture(el::stt_diffuse_1),
		"agaghrt");
	BOOST_CHECK_EQUAL(material_description.get_texture(el::stt_diffuse_2),
		"5z5");
	BOOST_CHECK_EQUAL(material_description.get_texture(el::stt_diffuse_3),
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
	BOOST_CHECK_EQUAL(material_description.get_texture_coodrinates(),
		"afdgsr!§$AFD54");
	BOOST_CHECK_EQUAL(material_description.get_diffuse_mapping(),
		"428rfu0sr4v");
	BOOST_CHECK_EQUAL(material_description.get_normal_mapping(), "AST%&%");
	BOOST_CHECK_EQUAL(material_description.get_specular_mapping(),
		"53456675cas");
	BOOST_CHECK_EQUAL(material_description.get_emission_mapping(),
		"F%$&GRF3453rf");
	BOOST_CHECK_EQUAL(material_description.get_receives_shadows(), false);
	BOOST_CHECK_EQUAL(material_description.get_lighting(), false);
}

BOOST_AUTO_TEST_CASE(xml)
{
	el::XmlBuffer buffer(UTF8("<?xml version=\"1.0\" encoding=\"utf8\"?>"
		"<material><name>tada</name><diffuse>df</diffuse>"
		"<second_diffuse>agaghrt</second_diffuse>"
		"<normal>m120</normal><specular>y&lt;bnj3e</specular>"
		"<emission>ymbnjf!2r2!A</emission><blend>345w</blend>"
		"<texture_coodrinates>afdgsr!§$AFD54</texture_coodrinates>"
		"<diffuse_mapping>428rfu0sr4v</diffuse_mapping>"
		"<normal_mapping>AST%&amp;%</normal_mapping>"
		"<specular_mapping>53456675cas</specular_mapping>"
		"<emission_mapping>F%$&amp;GRF3453rf</emission_mapping>"
		"<receives_shadows>false</receives_shadows>"
		"<lighting>false</lighting></material>"));
	el::MaterialDescription material_description;
	el::XmlReaderSharedPtr xml_reader;

	xml_reader = el::XmlReaderSharedPtr(new el::XmlReader(
		buffer.get_buffer()));

	BOOST_CHECK_NO_THROW(material_description.load_xml(
 		xml_reader->get_root_node()));

	xml_reader.reset();

	BOOST_CHECK_EQUAL(material_description.get_texture(el::stt_diffuse_0),
		"df");
	BOOST_CHECK_EQUAL(material_description.get_texture(el::stt_diffuse_1),
		"agaghrt");
	BOOST_CHECK_EQUAL(material_description.get_texture(el::stt_diffuse_2),
		"");
	BOOST_CHECK_EQUAL(material_description.get_texture(el::stt_diffuse_3),
		"");
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
	BOOST_CHECK_EQUAL(material_description.get_texture_coodrinates(),
		"afdgsr!§$AFD54");
	BOOST_CHECK_EQUAL(material_description.get_diffuse_mapping(),
		"428rfu0sr4v");
	BOOST_CHECK_EQUAL(material_description.get_normal_mapping(), "AST%&%");
	BOOST_CHECK_EQUAL(material_description.get_specular_mapping(),
		"53456675cas");
	BOOST_CHECK_EQUAL(material_description.get_emission_mapping(),
		"F%$&GRF3453rf");
	BOOST_CHECK_EQUAL(material_description.get_receives_shadows(), false);
	BOOST_CHECK_EQUAL(material_description.get_lighting(), false);
}
