/****************************************************************************
 *            effectdescription.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "prerequisites.hpp"
#include "effectdescription.hpp"
#include "materialeffectdescription.hpp"
#include "xmlbuffer.hpp"
#include "xmlreader.hpp"
#define BOOST_TEST_MODULE effect_description
#include <boost/test/unit_test.hpp>

namespace el = eternal_lands;

BOOST_AUTO_TEST_CASE(default_creation)
{
	el::EffectDescription effect_description;

	BOOST_CHECK_EQUAL(effect_description.get_world_transform(), "");
	BOOST_CHECK_EQUAL(effect_description.get_texture_coodrinates(), "");
	BOOST_CHECK_EQUAL(effect_description.get_albedo_mapping(), "");
	BOOST_CHECK_EQUAL(effect_description.get_normal_mapping(), "");
	BOOST_CHECK_EQUAL(effect_description.get_specular_mapping(), "");
	BOOST_CHECK_EQUAL(effect_description.get_emission_mapping(), "");
	BOOST_CHECK_EQUAL(effect_description.get_receives_shadows(), true);
	BOOST_CHECK_EQUAL(effect_description.get_transparent(), false);
	BOOST_CHECK_EQUAL(effect_description.get_lighting(), true);
}

BOOST_AUTO_TEST_CASE(material_creation)
{
	el::EffectDescription effect_description;
	el::MaterialEffectDescription material_effect_description;

	BOOST_CHECK_NO_THROW(material_effect_description.set_texture(
		el::String(UTF8("df")), el::stt_albedo_0));
	BOOST_CHECK_NO_THROW(material_effect_description.set_texture(
		el::String(UTF8("agaghrt")), el::stt_albedo_1));
	BOOST_CHECK_NO_THROW(material_effect_description.set_texture(
		el::String(UTF8("5z5")), el::stt_albedo_2));
	BOOST_CHECK_NO_THROW(material_effect_description.set_texture(
		el::String(UTF8("wj")), el::stt_albedo_3));
	BOOST_CHECK_NO_THROW(material_effect_description.set_texture(
		el::String(UTF8("m120")), el::stt_normal_0));
	BOOST_CHECK_NO_THROW(material_effect_description.set_texture(
		el::String(UTF8("aw79dz")), el::stt_normal_1));
	BOOST_CHECK_NO_THROW(material_effect_description.set_texture(
		el::String(UTF8("y<bnj3e")), el::stt_specular_0));
	BOOST_CHECK_NO_THROW(material_effect_description.set_texture(
		el::String(UTF8("48fh")), el::stt_specular_1));
	BOOST_CHECK_NO_THROW(material_effect_description.set_texture(
		el::String(UTF8("ymbnjf!2r2!A")), el::stt_emission_0));
	BOOST_CHECK_NO_THROW(material_effect_description.set_texture(
		el::String(UTF8("SDFGSGDF")), el::stt_emission_1));
	BOOST_CHECK_NO_THROW(material_effect_description.set_texture(
		el::String(UTF8("345w")), el::stt_blend_0));
	BOOST_CHECK_NO_THROW(material_effect_description.set_texture(
		el::String(UTF8("fs46wAFED§&%")), el::stt_blend_1));
	BOOST_CHECK_NO_THROW(
		material_effect_description.set_texture_scale_offset(
			glm::vec4(0.6787f, 4.342f, 345.0f, 76.0f)));
	BOOST_CHECK_NO_THROW(material_effect_description.set_material_name(
		el::String(UTF8("hrtzn..,"))));
	BOOST_CHECK_NO_THROW(material_effect_description.set_world_transform(
		el::String(UTF8("afsd5sf"))));
	BOOST_CHECK_NO_THROW(
		material_effect_description.set_texture_coodrinates(
			el::String(UTF8("afdgsr!§$AFD54"))));
	BOOST_CHECK_NO_THROW(material_effect_description.set_albedo_mapping(
		el::String(UTF8("428rfu0sr4v"))));
	BOOST_CHECK_NO_THROW(material_effect_description.set_normal_mapping(
		el::String(UTF8("AST%&%"))));
	BOOST_CHECK_NO_THROW(material_effect_description.set_specular_mapping(
		el::String(UTF8("53456675cas"))));
	BOOST_CHECK_NO_THROW(material_effect_description.set_emission_mapping(
		el::String(UTF8("F%$&GRF3453rf"))));
	BOOST_CHECK_NO_THROW(material_effect_description.set_receives_shadows(
		false));
	BOOST_CHECK_NO_THROW(material_effect_description.set_lighting(false));
	BOOST_CHECK_NO_THROW(material_effect_description.set_cast_shadows(
		false));
	BOOST_CHECK_NO_THROW(material_effect_description.set_culling(false));
	BOOST_CHECK_NO_THROW(material_effect_description.set_transparent(
		true));

	effect_description = el::EffectDescription(material_effect_description);

	BOOST_CHECK_EQUAL(effect_description.get_world_transform(),
		"afsd5sf");
	BOOST_CHECK_EQUAL(effect_description.get_texture_coodrinates(),
		"afdgsr!§$AFD54");
	BOOST_CHECK_EQUAL(effect_description.get_albedo_mapping(),
		"428rfu0sr4v");
	BOOST_CHECK_EQUAL(effect_description.get_normal_mapping(), "AST%&%");
	BOOST_CHECK_EQUAL(effect_description.get_specular_mapping(),
		"53456675cas");
	BOOST_CHECK_EQUAL(effect_description.get_emission_mapping(),
		"F%$&GRF3453rf");
	BOOST_CHECK_EQUAL(effect_description.get_receives_shadows(), false);
	BOOST_CHECK_EQUAL(effect_description.get_transparent(), true);
	BOOST_CHECK_EQUAL(effect_description.get_lighting(), false);
}

BOOST_AUTO_TEST_CASE(get_set)
{
	el::EffectDescription effect_description;

	BOOST_CHECK_NO_THROW(effect_description.set_world_transform(
		el::String(UTF8("sfg345sfgv"))));
	BOOST_CHECK_NO_THROW(effect_description.set_texture_coodrinates(
		el::String(UTF8("afdgsr!§$AFD54"))));
	BOOST_CHECK_NO_THROW(effect_description.set_albedo_mapping(
		el::String(UTF8("428rfu0sr4v"))));
	BOOST_CHECK_NO_THROW(effect_description.set_normal_mapping(
		el::String(UTF8("AST%&%"))));
	BOOST_CHECK_NO_THROW(effect_description.set_specular_mapping(
		el::String(UTF8("53456675cas"))));
	BOOST_CHECK_NO_THROW(effect_description.set_emission_mapping(
		el::String(UTF8("F%$&GRF3453rf"))));
	BOOST_CHECK_NO_THROW(effect_description.set_receives_shadows(false));
	BOOST_CHECK_NO_THROW(effect_description.set_transparent(true));
	BOOST_CHECK_NO_THROW(effect_description.set_lighting(false));

	BOOST_CHECK_EQUAL(effect_description.get_world_transform(),
		"sfg345sfgv");
	BOOST_CHECK_EQUAL(effect_description.get_texture_coodrinates(),
		"afdgsr!§$AFD54");
	BOOST_CHECK_EQUAL(effect_description.get_albedo_mapping(),
		"428rfu0sr4v");
	BOOST_CHECK_EQUAL(effect_description.get_normal_mapping(), "AST%&%");
	BOOST_CHECK_EQUAL(effect_description.get_specular_mapping(),
		"53456675cas");
	BOOST_CHECK_EQUAL(effect_description.get_emission_mapping(),
		"F%$&GRF3453rf");
	BOOST_CHECK_EQUAL(effect_description.get_receives_shadows(), false);
	BOOST_CHECK_EQUAL(effect_description.get_transparent(), true);
	BOOST_CHECK_EQUAL(effect_description.get_lighting(), false);
}

BOOST_AUTO_TEST_CASE(xml)
{
	el::XmlBuffer buffer(UTF8("<?xml version=\"1.0\" encoding=\"utf8\"?>"
		"<effect><world_transform>sfg345sfgv</world_transform>"
		"<texture_coodrinates>afdgsr!§$AFD54</texture_coodrinates>"
		"<albedo_mapping>428rfu0sr4v</albedo_mapping>"
		"<normal_mapping>AST%&amp;%</normal_mapping>"
		"<specular_mapping>53456675cas</specular_mapping>"
		"<emission_mapping>F%$&amp;GRF3453rf</emission_mapping>"
		"<receives_shadows>false</receives_shadows>"
		"<lighting>false</lighting>"
		"<transparent>true</transparent></effect>"));
	el::EffectDescription effect_description;
	el::XmlReaderSharedPtr xml_reader;

	xml_reader = el::XmlReaderSharedPtr(new el::XmlReader(
		buffer.get_buffer()));

	BOOST_CHECK_NO_THROW(effect_description.load_xml(
 		xml_reader->get_root_node()));

	xml_reader.reset();

	BOOST_CHECK_EQUAL(effect_description.get_world_transform(),
		"sfg345sfgv");
	BOOST_CHECK_EQUAL(effect_description.get_texture_coodrinates(),
		"afdgsr!§$AFD54");
	BOOST_CHECK_EQUAL(effect_description.get_albedo_mapping(),
		"428rfu0sr4v");
	BOOST_CHECK_EQUAL(effect_description.get_normal_mapping(), "AST%&%");
	BOOST_CHECK_EQUAL(effect_description.get_specular_mapping(),
		"53456675cas");
	BOOST_CHECK_EQUAL(effect_description.get_emission_mapping(),
		"F%$&GRF3453rf");
	BOOST_CHECK_EQUAL(effect_description.get_receives_shadows(), false);
	BOOST_CHECK_EQUAL(effect_description.get_transparent(), true);
	BOOST_CHECK_EQUAL(effect_description.get_lighting(), false);
}

BOOST_AUTO_TEST_CASE(asign)
{
	el::EffectDescription effect_description_0;
	el::EffectDescription effect_description_1;

	BOOST_CHECK_NO_THROW(effect_description_0.set_world_transform(
		el::String(UTF8("afsd5sf"))));
	BOOST_CHECK_NO_THROW(
		effect_description_0.set_texture_coodrinates(
			el::String(UTF8("afdgsr!§$AFD54"))));
	BOOST_CHECK_NO_THROW(effect_description_0.set_albedo_mapping(
		el::String(UTF8("428rfu0sr4v"))));
	BOOST_CHECK_NO_THROW(effect_description_0.set_normal_mapping(
		el::String(UTF8("AST%&%"))));
	BOOST_CHECK_NO_THROW(effect_description_0.set_specular_mapping(
		el::String(UTF8("53456675cas"))));
	BOOST_CHECK_NO_THROW(effect_description_0.set_emission_mapping(
		el::String(UTF8("F%$&GRF3453rf"))));
	BOOST_CHECK_NO_THROW(effect_description_0.set_receives_shadows(
		false));
	BOOST_CHECK_NO_THROW(effect_description_0.set_lighting(false));
	BOOST_CHECK_NO_THROW(effect_description_0.set_transparent(
		true));

	effect_description_1 = effect_description_0;

	BOOST_CHECK_EQUAL(effect_description_0.get_world_transform(),
		"afsd5sf");
	BOOST_CHECK_EQUAL(effect_description_1.get_world_transform(),
		"afsd5sf");
	BOOST_CHECK_EQUAL(
		effect_description_0.get_texture_coodrinates(),
		"afdgsr!§$AFD54");
	BOOST_CHECK_EQUAL(
		effect_description_1.get_texture_coodrinates(),
		"afdgsr!§$AFD54");
	BOOST_CHECK_EQUAL(effect_description_0.get_albedo_mapping(),
		"428rfu0sr4v");
	BOOST_CHECK_EQUAL(effect_description_1.get_albedo_mapping(),
		"428rfu0sr4v");
	BOOST_CHECK_EQUAL(effect_description_0.get_normal_mapping(),
		"AST%&%");
	BOOST_CHECK_EQUAL(effect_description_1.get_normal_mapping(),
		"AST%&%");
	BOOST_CHECK_EQUAL(effect_description_0.get_specular_mapping(),
		"53456675cas");
	BOOST_CHECK_EQUAL(effect_description_1.get_specular_mapping(),
		"53456675cas");
	BOOST_CHECK_EQUAL(effect_description_0.get_emission_mapping(),
		"F%$&GRF3453rf");
	BOOST_CHECK_EQUAL(effect_description_1.get_emission_mapping(),
		"F%$&GRF3453rf");
	BOOST_CHECK_EQUAL(effect_description_0.get_receives_shadows(),
		false);
	BOOST_CHECK_EQUAL(effect_description_1.get_receives_shadows(),
		false);
	BOOST_CHECK_EQUAL(effect_description_0.get_lighting(), false);
	BOOST_CHECK_EQUAL(effect_description_1.get_lighting(), false);
	BOOST_CHECK_EQUAL(effect_description_0.get_transparent(),
		true);
	BOOST_CHECK_EQUAL(effect_description_1.get_transparent(),
		true);

	BOOST_CHECK_EQUAL(effect_description_0,
		effect_description_1);
}

BOOST_AUTO_TEST_CASE(equal)
{
	el::EffectDescription effect_description_0;
	el::EffectDescription effect_description_1;

	BOOST_CHECK_NO_THROW(effect_description_0.set_world_transform(
		el::String(UTF8("afsd5sf"))));
	BOOST_CHECK_NO_THROW(
		effect_description_0.set_texture_coodrinates(
			el::String(UTF8("afdgsr!§$AFD54"))));
	BOOST_CHECK_NO_THROW(effect_description_0.set_albedo_mapping(
		el::String(UTF8("428rfu0sr4v"))));
	BOOST_CHECK_NO_THROW(effect_description_0.set_normal_mapping(
		el::String(UTF8("AST%&%"))));
	BOOST_CHECK_NO_THROW(effect_description_0.set_specular_mapping(
		el::String(UTF8("53456675cas"))));
	BOOST_CHECK_NO_THROW(effect_description_0.set_emission_mapping(
		el::String(UTF8("F%$&GRF3453rf"))));
	BOOST_CHECK_NO_THROW(effect_description_0.set_receives_shadows(
		false));
	BOOST_CHECK_NO_THROW(effect_description_0.set_lighting(false));
	BOOST_CHECK_NO_THROW(effect_description_0.set_transparent(
		true));

	BOOST_CHECK_NO_THROW(effect_description_1.set_world_transform(
		el::String(UTF8("afsd5sf"))));
	BOOST_CHECK_NO_THROW(
		effect_description_1.set_texture_coodrinates(
			el::String(UTF8("afdgsr!§$AFD54"))));
	BOOST_CHECK_NO_THROW(effect_description_1.set_albedo_mapping(
		el::String(UTF8("428rfu0sr4v"))));
	BOOST_CHECK_NO_THROW(effect_description_1.set_normal_mapping(
		el::String(UTF8("AST%&%"))));
	BOOST_CHECK_NO_THROW(effect_description_1.set_specular_mapping(
		el::String(UTF8("53456675cas"))));
	BOOST_CHECK_NO_THROW(effect_description_1.set_emission_mapping(
		el::String(UTF8("F%$&GRF3453rf"))));
	BOOST_CHECK_NO_THROW(effect_description_1.set_receives_shadows(
		false));
	BOOST_CHECK_NO_THROW(effect_description_1.set_lighting(false));
	BOOST_CHECK_NO_THROW(effect_description_1.set_transparent(
		true));

	BOOST_CHECK_EQUAL(effect_description_0, effect_description_0);
	BOOST_CHECK_EQUAL(effect_description_0, effect_description_1);
	BOOST_CHECK_EQUAL(effect_description_1, effect_description_1);
	BOOST_CHECK(!(effect_description_0 != effect_description_0));
	BOOST_CHECK(!(effect_description_0 != effect_description_1));
	BOOST_CHECK(!(effect_description_1 != effect_description_1));
	BOOST_CHECK(effect_description_0 == effect_description_0);
	BOOST_CHECK(effect_description_0 == effect_description_1);
	BOOST_CHECK(effect_description_1 == effect_description_1);
}

BOOST_AUTO_TEST_CASE(not_equal)
{
	el::EffectDescription effect_description_0;
	el::EffectDescription effect_description_1;

	BOOST_CHECK_NO_THROW(effect_description_0.set_world_transform(
		el::String(UTF8("afsd5sf"))));
	BOOST_CHECK_NO_THROW(
		effect_description_0.set_texture_coodrinates(
			el::String(UTF8("afdgsr!§$AFD54"))));
	BOOST_CHECK_NO_THROW(effect_description_0.set_albedo_mapping(
		el::String(UTF8("428rfu0sr4v"))));
	BOOST_CHECK_NO_THROW(effect_description_0.set_normal_mapping(
		el::String(UTF8("AST%&%"))));
	BOOST_CHECK_NO_THROW(effect_description_0.set_specular_mapping(
		el::String(UTF8("53456675cas"))));
	BOOST_CHECK_NO_THROW(effect_description_0.set_emission_mapping(
		el::String(UTF8("F%$&GRF3453rf"))));
	BOOST_CHECK_NO_THROW(effect_description_0.set_receives_shadows(
		false));
	BOOST_CHECK_NO_THROW(effect_description_0.set_lighting(false));
	BOOST_CHECK_NO_THROW(effect_description_0.set_transparent(
		true));

	BOOST_CHECK_NE(effect_description_0, effect_description_1);
	BOOST_CHECK(!(effect_description_0 == effect_description_1));
	BOOST_CHECK(effect_description_0 != effect_description_1);

	effect_description_1 = effect_description_0;

	BOOST_CHECK_NO_THROW(effect_description_1.set_world_transform(
		el::String(UTF8(""))));

	BOOST_CHECK_NE(effect_description_0, effect_description_1);
	BOOST_CHECK(!(effect_description_0 == effect_description_1));
	BOOST_CHECK(effect_description_0 != effect_description_1);

	effect_description_1 = effect_description_0;

	BOOST_CHECK_NO_THROW(effect_description_1.set_texture_coodrinates(
		el::String(UTF8(""))));

	BOOST_CHECK_NE(effect_description_0, effect_description_1);
	BOOST_CHECK(!(effect_description_0 == effect_description_1));
	BOOST_CHECK(effect_description_0 != effect_description_1);

	effect_description_1 = effect_description_0;

	BOOST_CHECK_NO_THROW(effect_description_1.set_albedo_mapping(
		el::String(UTF8(""))));

	BOOST_CHECK_NE(effect_description_0, effect_description_1);
	BOOST_CHECK(!(effect_description_0 == effect_description_1));
	BOOST_CHECK(effect_description_0 != effect_description_1);

	effect_description_1 = effect_description_0;

	BOOST_CHECK_NO_THROW(effect_description_1.set_normal_mapping(
		el::String(UTF8(""))));

	BOOST_CHECK_NE(effect_description_0, effect_description_1);
	BOOST_CHECK(!(effect_description_0 == effect_description_1));
	BOOST_CHECK(effect_description_0 != effect_description_1);

	effect_description_1 = effect_description_0;

	BOOST_CHECK_NO_THROW(effect_description_1.set_specular_mapping(
		el::String(UTF8(""))));

	BOOST_CHECK_NE(effect_description_0, effect_description_1);
	BOOST_CHECK(!(effect_description_0 == effect_description_1));
	BOOST_CHECK(effect_description_0 != effect_description_1);

	effect_description_1 = effect_description_0;

	BOOST_CHECK_NO_THROW(effect_description_1.set_emission_mapping(
		el::String(UTF8(""))));

	BOOST_CHECK_NE(effect_description_0, effect_description_1);
	BOOST_CHECK(!(effect_description_0 == effect_description_1));
	BOOST_CHECK(effect_description_0 != effect_description_1);

	effect_description_1 = effect_description_0;

	BOOST_CHECK_NO_THROW(effect_description_1.set_receives_shadows(true));

	BOOST_CHECK_NE(effect_description_0, effect_description_1);
	BOOST_CHECK(!(effect_description_0 == effect_description_1));
	BOOST_CHECK(effect_description_0 != effect_description_1);

	effect_description_1 = effect_description_0;

	BOOST_CHECK_NO_THROW(effect_description_1.set_lighting(true));

	BOOST_CHECK_NE(effect_description_0, effect_description_1);
	BOOST_CHECK(!(effect_description_0 == effect_description_1));
	BOOST_CHECK(effect_description_0 != effect_description_1);

	effect_description_1 = effect_description_0;

	BOOST_CHECK_NO_THROW(effect_description_1.set_transparent(false));

	BOOST_CHECK_NE(effect_description_0, effect_description_1);
	BOOST_CHECK(!(effect_description_0 == effect_description_1));
	BOOST_CHECK(effect_description_0 != effect_description_1);
}

BOOST_AUTO_TEST_CASE(less)
{
	el::EffectDescription effect_description_0;
	el::EffectDescription effect_description_1;

	BOOST_CHECK_NO_THROW(effect_description_0.set_world_transform(
		el::String(UTF8("afsd5sf"))));
	BOOST_CHECK_NO_THROW(
		effect_description_0.set_texture_coodrinates(
			el::String(UTF8("afdgsr!§$AFD54"))));
	BOOST_CHECK_NO_THROW(effect_description_0.set_albedo_mapping(
		el::String(UTF8("428rfu0sr4v"))));
	BOOST_CHECK_NO_THROW(effect_description_0.set_normal_mapping(
		el::String(UTF8("AST%&%"))));
	BOOST_CHECK_NO_THROW(effect_description_0.set_specular_mapping(
		el::String(UTF8("53456675cas"))));
	BOOST_CHECK_NO_THROW(effect_description_0.set_emission_mapping(
		el::String(UTF8("F%$&GRF3453rf"))));
	BOOST_CHECK_NO_THROW(effect_description_0.set_receives_shadows(
		false));
	BOOST_CHECK_NO_THROW(effect_description_0.set_lighting(false));
	BOOST_CHECK_NO_THROW(effect_description_0.set_transparent(
		true));

	BOOST_CHECK_LT(effect_description_1, effect_description_0);
	BOOST_CHECK(!(effect_description_0 < effect_description_1));
	BOOST_CHECK(effect_description_1 < effect_description_0);

	effect_description_1 = effect_description_0;

	BOOST_CHECK_NO_THROW(effect_description_1.set_world_transform(
		el::String(UTF8(""))));

	BOOST_CHECK_LT(effect_description_1, effect_description_0);
	BOOST_CHECK(!(effect_description_0 < effect_description_1));
	BOOST_CHECK(effect_description_1 < effect_description_0);

	effect_description_1 = effect_description_0;

	BOOST_CHECK_NO_THROW(effect_description_1.set_texture_coodrinates(
		el::String(UTF8(""))));

	BOOST_CHECK_LT(effect_description_1, effect_description_0);
	BOOST_CHECK(!(effect_description_0 < effect_description_1));
	BOOST_CHECK(effect_description_1 < effect_description_0);

	effect_description_1 = effect_description_0;

	BOOST_CHECK_NO_THROW(effect_description_1.set_albedo_mapping(
		el::String(UTF8(""))));

	BOOST_CHECK_LT(effect_description_1, effect_description_0);
	BOOST_CHECK(!(effect_description_0 < effect_description_1));
	BOOST_CHECK(effect_description_1 < effect_description_0);

	effect_description_1 = effect_description_0;

	BOOST_CHECK_NO_THROW(effect_description_1.set_normal_mapping(
		el::String(UTF8(""))));

	BOOST_CHECK_LT(effect_description_1, effect_description_0);
	BOOST_CHECK(!(effect_description_0 < effect_description_1));
	BOOST_CHECK(effect_description_1 < effect_description_0);

	effect_description_1 = effect_description_0;

	BOOST_CHECK_NO_THROW(effect_description_1.set_specular_mapping(
		el::String(UTF8(""))));

	BOOST_CHECK_LT(effect_description_1, effect_description_0);
	BOOST_CHECK(!(effect_description_0 < effect_description_1));
	BOOST_CHECK(effect_description_1 < effect_description_0);

	effect_description_1 = effect_description_0;

	BOOST_CHECK_NO_THROW(effect_description_1.set_emission_mapping(
		el::String(UTF8(""))));

	BOOST_CHECK_LT(effect_description_1, effect_description_0);
	BOOST_CHECK(!(effect_description_0 < effect_description_1));
	BOOST_CHECK(effect_description_1 < effect_description_0);

	effect_description_1 = effect_description_0;

	BOOST_CHECK_NO_THROW(effect_description_1.set_receives_shadows(true));

	BOOST_CHECK_LT(effect_description_0, effect_description_1);
	BOOST_CHECK(!(effect_description_1 < effect_description_0));
	BOOST_CHECK(effect_description_0 < effect_description_1);

	effect_description_1 = effect_description_0;

	BOOST_CHECK_NO_THROW(effect_description_1.set_lighting(true));

	BOOST_CHECK_LT(effect_description_0, effect_description_1);
	BOOST_CHECK(!(effect_description_1 < effect_description_0));
	BOOST_CHECK(effect_description_0 < effect_description_1);

	effect_description_1 = effect_description_0;

	BOOST_CHECK_NO_THROW(effect_description_1.set_transparent(false));

	BOOST_CHECK_LT(effect_description_1, effect_description_0);
	BOOST_CHECK(!(effect_description_0 < effect_description_1));
	BOOST_CHECK(effect_description_1 < effect_description_0);
}
