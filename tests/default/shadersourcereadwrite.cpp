/****************************************************************************
 *            shadersourcereadwrite.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "prerequisites.hpp"
#include "shader/shadersource.hpp"
#define BOOST_TEST_MODULE shader_source_read_write
#include <boost/test/unit_test.hpp>
#include <boost/uuid/uuid_generators.hpp>

namespace el = eternal_lands;

BOOST_AUTO_TEST_CASE(shader_source)
{
	boost::scoped_ptr<el::ShaderSource> shader_source;

	BOOST_CHECK_NO_THROW(shader_source.reset(new el::ShaderSource(
		boost::uuids::random_generator()())));
}

BOOST_AUTO_TEST_CASE(load)
{
	boost::shared_ptr<el::ShaderSource> shader_source;
	el::String name;
	std::ifstream str;

	BOOST_CHECK_NO_THROW(shader_source.reset(new el::ShaderSource(
		boost::uuids::random_generator()())));

	str.open("shader_sources.txt", std::ios::in);

	BOOST_CHECK(str.is_open());

	while (!str.eof())
	{
		str >> name;

		BOOST_CHECK_NO_THROW(shader_source->load_xml(name));
//		BOOST_CHECK_NO_THROW(shader_source->save_xml(name));
	}

}
