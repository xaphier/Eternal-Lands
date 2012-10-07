/****************************************************************************
 *            shadersourceloadsave.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "prerequisites.hpp"
#include "xmlbuffer.hpp"
#include "xmlwriter.hpp"
#include "shader/shadersource.hpp"
#define BOOST_TEST_MODULE shader_source_load_save
#include <boost/test/unit_test.hpp>
#include <boost/uuid/uuid_generators.hpp>

namespace el = eternal_lands;

BOOST_AUTO_TEST_CASE(shader_source)
{
	boost::scoped_ptr<el::ShaderSource> shader_source;

	BOOST_CHECK_NO_THROW(shader_source.reset(new el::ShaderSource()));
}

BOOST_AUTO_TEST_CASE(load)
{
	boost::shared_ptr<el::ShaderSource> shader_source;
	el::String name;
	std::ifstream str;

	BOOST_CHECK_NO_THROW(shader_source.reset(new el::ShaderSource()));

	str.open("shader_sources.txt", std::ios::in);

	BOOST_CHECK(str.is_open());

	while (!str.eof())
	{
		str >> name;

		BOOST_CHECK_NO_THROW(shader_source->load_xml(name));
	}
}
/*
BOOST_AUTO_TEST_CASE(load_save)
{
	boost::shared_ptr<el::ShaderSource> shader_source;
	el::String name;
	std::ifstream str;

	BOOST_CHECK_NO_THROW(shader_source.reset(new el::ShaderSource()));

	str.open("shader_sources.txt", std::ios::in);

	BOOST_CHECK(str.is_open());

	while (!str.eof())
	{
		str >> name;

		BOOST_CHECK_NO_THROW(shader_source->load_xml(name));
		BOOST_CHECK_NO_THROW(shader_source->save_xml_string());
	}

}
*/
BOOST_AUTO_TEST_CASE(load_save)
{
	boost::shared_ptr<el::ShaderSource> shader_source;
	boost::shared_ptr<el::ShaderSource> shader_source_tmp;
	el::String name, data, data_tmp;
	std::ifstream str;

	BOOST_CHECK_NO_THROW(shader_source.reset(new el::ShaderSource()));
	BOOST_CHECK_NO_THROW(shader_source_tmp.reset(new el::ShaderSource()));

	str.open("shader_sources.txt", std::ios::in);

	BOOST_CHECK(str.is_open());

	while (!str.eof())
	{
		str >> name;

		BOOST_CHECK_NO_THROW(shader_source->load_xml(name));
		BOOST_CHECK_NO_THROW(data = shader_source->save_xml_string());
		BOOST_CHECK_NO_THROW(shader_source_tmp->load_xml_string(data));
		BOOST_CHECK_NO_THROW(data_tmp =
			shader_source_tmp->save_xml_string());
//		BOOST_CHECK_EQUAL(shader_source, shader_source_tmp);
		BOOST_CHECK_EQUAL(data.get(), data_tmp.get());
	}

}
