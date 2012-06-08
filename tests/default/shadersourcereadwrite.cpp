/****************************************************************************
 *            shadersourcereadwrite.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "prerequisites.hpp"
#include "shader/shadersource.hpp"
#define BOOST_TEST_MODULE shader_source_read_write
#include <boost/test/unit_test.hpp>

namespace el = eternal_lands;

BOOST_AUTO_TEST_CASE(shader_source)
{
	el::ShaderSource shader_source;

	BOOST_CHECK_EQUAL(shader_source.get_name().get(), "");
}

BOOST_AUTO_TEST_CASE(load)
{
	el::ShaderSource shader_source;
	el::String name;
	std::ifstream str;

	str.open("shader_sources.txt", std::ios::in);

	BOOST_CHECK(str.is_open());

	while (!str.eof())
	{
		str >> name;

		BOOST_CHECK_NO_THROW(shader_source.load_xml(name));
		BOOST_CHECK_NO_THROW(shader_source.save_xml(name));
	}
}
