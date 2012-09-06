/****************************************************************************
 *            effectdescription.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "prerequisites.hpp"
#include "effect/effectdescription.hpp"
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
	BOOST_CHECK_EQUAL(effect_description.get_main(), "");
	BOOST_CHECK_EQUAL(effect_description.get_lighting(), "");
	BOOST_CHECK_EQUAL(effect_description.get_receives_shadows(), true);
	BOOST_CHECK_EQUAL(effect_description.get_transparent(), false);
}

BOOST_AUTO_TEST_CASE(get_set)
{
	el::EffectDescription effect_description;

	BOOST_CHECK_NO_THROW(effect_description.set_world_transformation(
		el::String(UTF8("sfg345sfgv"))));
	BOOST_CHECK_NO_THROW(effect_description.set_texture_coodrinates(
		el::String(UTF8("afdgsr!§$AFD54"))));
	BOOST_CHECK_NO_THROW(effect_description.set_main(
		el::String(UTF8("428rfu0sr4v"))));
	BOOST_CHECK_NO_THROW(effect_description.set_lighting(
		el::String(UTF8("3423ge56z4g2bv"))));
	BOOST_CHECK_NO_THROW(effect_description.set_receives_shadows(false));
	BOOST_CHECK_NO_THROW(effect_description.set_transparent(true));

	BOOST_CHECK_EQUAL(effect_description.get_world_transformation(),
		"sfg345sfgv");
	BOOST_CHECK_EQUAL(effect_description.get_texture_coodrinates(),
		"afdgsr!§$AFD54");
	BOOST_CHECK_EQUAL(effect_description.get_main(), "428rfu0sr4v");
	BOOST_CHECK_EQUAL(effect_description.get_lighting(), "3423ge56z4g2bv");
	BOOST_CHECK_EQUAL(effect_description.get_receives_shadows(), false);
	BOOST_CHECK_EQUAL(effect_description.get_transparent(), true);
}

BOOST_AUTO_TEST_CASE(xml)
{
	el::XmlBuffer buffer(UTF8("<?xml version=\"1.0\" encoding=\"utf8\"?>"
		"<effect>"
		"<world_transformation>sfg345sfgv</world_transformation>"
		"<texture_coodrinates>afdgsr!§$AFD54</texture_coodrinates>"
		"<main>428rfu0sr4v</main>"
		"<lighting>3423ge56z4g2bv</lighting>"
		"<node_based>false</node_based>"
		"<receives_shadows>false</receives_shadows>"
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
	BOOST_CHECK_EQUAL(effect_description.get_main(), "428rfu0sr4v");
	BOOST_CHECK_EQUAL(effect_description.get_lighting(), "3423ge56z4g2bv");
	BOOST_CHECK_EQUAL(effect_description.get_receives_shadows(), false);
	BOOST_CHECK_EQUAL(effect_description.get_transparent(), true);
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
	BOOST_CHECK_NO_THROW(effect_description_0.set_main(
		el::String(UTF8("428rfu0sr4v"))));
	BOOST_CHECK_NO_THROW(effect_description_0.set_lighting(
		el::String(UTF8("3423ge56z4g2bv"))));
	BOOST_CHECK_NO_THROW(effect_description_0.set_receives_shadows(
		false));
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
	BOOST_CHECK_EQUAL(effect_description_0.get_main(),
		"428rfu0sr4v");
	BOOST_CHECK_EQUAL(effect_description_1.get_main(),
		"428rfu0sr4v");
	BOOST_CHECK_EQUAL(effect_description_0.get_lighting(),
		"3423ge56z4g2bv");
	BOOST_CHECK_EQUAL(effect_description_1.get_lighting(),
		"3423ge56z4g2bv");
	BOOST_CHECK_EQUAL(effect_description_0.get_receives_shadows(),
		false);
	BOOST_CHECK_EQUAL(effect_description_1.get_receives_shadows(),
		false);
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
	BOOST_CHECK_NO_THROW(effect_description_0.set_main(
		el::String(UTF8("428rfu0sr4v"))));
	BOOST_CHECK_NO_THROW(effect_description_0.set_lighting(
		el::String(UTF8("3423ge56z4g2bv"))));
	BOOST_CHECK_NO_THROW(effect_description_0.set_receives_shadows(
		false));
	BOOST_CHECK_NO_THROW(effect_description_0.set_transparent(
		true));

	BOOST_CHECK_NO_THROW(effect_description_1.set_world_transformation(
		el::String(UTF8("afsd5sf"))));
	BOOST_CHECK_NO_THROW(
		effect_description_1.set_texture_coodrinates(
			el::String(UTF8("afdgsr!§$AFD54"))));
	BOOST_CHECK_NO_THROW(effect_description_1.set_main(
		el::String(UTF8("428rfu0sr4v"))));
	BOOST_CHECK_NO_THROW(effect_description_1.set_lighting(
		el::String(UTF8("3423ge56z4g2bv"))));
	BOOST_CHECK_NO_THROW(effect_description_1.set_receives_shadows(
		false));
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
	BOOST_CHECK_NO_THROW(effect_description_0.set_main(
		el::String(UTF8("428rfu0sr4v"))));
	BOOST_CHECK_NO_THROW(effect_description_0.set_lighting(
		el::String(UTF8("3423ge56z4g2bv"))));
	BOOST_CHECK_NO_THROW(effect_description_0.set_receives_shadows(
		false));
	BOOST_CHECK_NO_THROW(effect_description_0.set_transparent(
		true));

	BOOST_CHECK_NE(effect_description_0, effect_description_1);
	BOOST_CHECK(!(effect_description_0 == effect_description_1));
	BOOST_CHECK(effect_description_0 != effect_description_1);

	effect_description_1 = effect_description_0;

	BOOST_CHECK_NO_THROW(effect_description_1.set_world_transformation(
		el::String()));

	BOOST_CHECK_NE(effect_description_0, effect_description_1);
	BOOST_CHECK(!(effect_description_0 == effect_description_1));
	BOOST_CHECK(effect_description_0 != effect_description_1);

	effect_description_1 = effect_description_0;

	BOOST_CHECK_NO_THROW(effect_description_1.set_texture_coodrinates(
		el::String()));

	BOOST_CHECK_NE(effect_description_0, effect_description_1);
	BOOST_CHECK(!(effect_description_0 == effect_description_1));
	BOOST_CHECK(effect_description_0 != effect_description_1);

	effect_description_1 = effect_description_0;

	BOOST_CHECK_NO_THROW(effect_description_1.set_main(el::String()));

	BOOST_CHECK_NE(effect_description_0, effect_description_1);
	BOOST_CHECK(!(effect_description_0 == effect_description_1));
	BOOST_CHECK(effect_description_0 != effect_description_1);

	effect_description_1 = effect_description_0;

	BOOST_CHECK_NO_THROW(effect_description_1.set_lighting(el::String()));

	BOOST_CHECK_NE(effect_description_0, effect_description_1);
	BOOST_CHECK(!(effect_description_0 == effect_description_1));
	BOOST_CHECK(effect_description_0 != effect_description_1);

	effect_description_1 = effect_description_0;

	BOOST_CHECK_NO_THROW(effect_description_1.set_receives_shadows(true));

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
	BOOST_CHECK_NO_THROW(effect_description_0.set_main(
		el::String(UTF8("428rfu0sr4v"))));
	BOOST_CHECK_NO_THROW(effect_description_0.set_lighting(
		el::String(UTF8("3423ge56z4g2bv"))));
	BOOST_CHECK_NO_THROW(effect_description_0.set_receives_shadows(
		false));
	BOOST_CHECK_NO_THROW(effect_description_0.set_transparent(
		true));

	BOOST_CHECK_LT(effect_description_1, effect_description_0);
	BOOST_CHECK(!(effect_description_0 < effect_description_1));
	BOOST_CHECK(effect_description_1 < effect_description_0);

	effect_description_1 = effect_description_0;

	BOOST_CHECK_NO_THROW(effect_description_1.set_world_transformation(
		el::String()));

	BOOST_CHECK_LT(effect_description_1, effect_description_0);
	BOOST_CHECK(!(effect_description_0 < effect_description_1));
	BOOST_CHECK(effect_description_1 < effect_description_0);

	effect_description_1 = effect_description_0;

	BOOST_CHECK_NO_THROW(effect_description_1.set_texture_coodrinates(
		el::String()));

	BOOST_CHECK_LT(effect_description_1, effect_description_0);
	BOOST_CHECK(!(effect_description_0 < effect_description_1));
	BOOST_CHECK(effect_description_1 < effect_description_0);

	effect_description_1 = effect_description_0;

	BOOST_CHECK_NO_THROW(effect_description_1.set_main(el::String()));

	BOOST_CHECK_LT(effect_description_1, effect_description_0);
	BOOST_CHECK(!(effect_description_0 < effect_description_1));
	BOOST_CHECK(effect_description_1 < effect_description_0);

	effect_description_1 = effect_description_0;

	BOOST_CHECK_NO_THROW(effect_description_1.set_lighting(el::String()));

	BOOST_CHECK_LT(effect_description_1, effect_description_0);
	BOOST_CHECK(!(effect_description_0 < effect_description_1));
	BOOST_CHECK(effect_description_1 < effect_description_0);

	effect_description_1 = effect_description_0;

	BOOST_CHECK_NO_THROW(effect_description_1.set_receives_shadows(true));

	BOOST_CHECK_LT(effect_description_0, effect_description_1);
	BOOST_CHECK(!(effect_description_1 < effect_description_0));
	BOOST_CHECK(effect_description_0 < effect_description_1);

	effect_description_1 = effect_description_0;

	BOOST_CHECK_NO_THROW(effect_description_1.set_transparent(false));

	BOOST_CHECK_LT(effect_description_1, effect_description_0);
	BOOST_CHECK(!(effect_description_0 < effect_description_1));
	BOOST_CHECK(effect_description_1 < effect_description_0);
}
