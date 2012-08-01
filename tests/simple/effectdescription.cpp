/****************************************************************************
 *            effectdescription.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "prerequisites.hpp"
#include "effectdescription.hpp"
#include "xmlbuffer.hpp"
#include "xmlreader.hpp"
#define BOOST_TEST_MODULE effect_description
#include <boost/test/unit_test.hpp>

namespace el = eternal_lands;

BOOST_AUTO_TEST_CASE(default_creation)
{
	el::EffectDescription effect_description;

	BOOST_CHECK_EQUAL(effect_description.get_world_transformation(), "");
	BOOST_CHECK_EQUAL(effect_description.get_texture_coodrinates(), "");
	BOOST_CHECK_EQUAL(effect_description.get_albedo_mapping(), "");
	BOOST_CHECK_EQUAL(effect_description.get_normal_mapping(), "");
	BOOST_CHECK_EQUAL(effect_description.get_specular_mapping(), "");
	BOOST_CHECK_EQUAL(effect_description.get_emission_mapping(), "");
	BOOST_CHECK_EQUAL(effect_description.get_receives_shadows(), true);
	BOOST_CHECK_EQUAL(effect_description.get_transparent(), false);
	BOOST_CHECK_EQUAL(effect_description.get_lighting(), true);
}

BOOST_AUTO_TEST_CASE(get_set)
{
	el::EffectDescription effect_description;

	BOOST_CHECK_NO_THROW(effect_description.set_world_transformation(
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

	BOOST_CHECK_EQUAL(effect_description.get_world_transformation(),
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
		"<effect>"
		"<world_transformation>sfg345sfgv</world_transformation>"
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

	BOOST_CHECK_EQUAL(effect_description.get_world_transformation(),
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

	BOOST_CHECK_NO_THROW(effect_description_0.set_world_transformation(
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

	BOOST_CHECK_EQUAL(effect_description_0.get_world_transformation(),
		"afsd5sf");
	BOOST_CHECK_EQUAL(effect_description_1.get_world_transformation(),
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

	BOOST_CHECK_NO_THROW(effect_description_0.set_world_transformation(
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

	BOOST_CHECK_NO_THROW(effect_description_1.set_world_transformation(
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

	BOOST_CHECK_NO_THROW(effect_description_0.set_world_transformation(
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

	BOOST_CHECK_NO_THROW(effect_description_1.set_world_transformation(
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

	BOOST_CHECK_NO_THROW(effect_description_0.set_world_transformation(
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

	BOOST_CHECK_NO_THROW(effect_description_1.set_world_transformation(
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
