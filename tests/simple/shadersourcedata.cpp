/****************************************************************************
 *            shadersourcedata.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "prerequisites.hpp"
#include "shader/shadersourcedata.hpp"
#include "xmlbuffer.hpp"
#include "xmlwriter.hpp"
#include "xmlreader.hpp"
#define BOOST_TEST_MODULE shader_source_data
#include <boost/test/unit_test.hpp>

namespace el = eternal_lands;

BOOST_AUTO_TEST_CASE(default_creation)
{
	el::ShaderSourceData shader_source_data;

	BOOST_CHECK_EQUAL(shader_source_data.get_source(), "");

	BOOST_CHECK_EQUAL(shader_source_data.get_version(), el::svt_120);
}

BOOST_AUTO_TEST_CASE(source)
{
	el::ShaderSourceData shader_source_data;

	shader_source_data.set_source(el::String(UTF8("3sfd23")));
	BOOST_CHECK_EQUAL(shader_source_data.get_source(), UTF8("3sfd23"));

	shader_source_data.set_source(el::String(UTF8("w4tgi3")));
	BOOST_CHECK_EQUAL(shader_source_data.get_source(), UTF8("w4tgi3"));
}

BOOST_AUTO_TEST_CASE(options)
{
	el::ShaderSourceData shader_source_data;

	shader_source_data.set_version(el::svt_120);
	BOOST_CHECK_EQUAL(shader_source_data.get_version(), el::svt_120);

	shader_source_data.set_version(el::svt_330);
	BOOST_CHECK_EQUAL(shader_source_data.get_version(), el::svt_330);

	shader_source_data.set_version(el::svt_420);
	BOOST_CHECK_EQUAL(shader_source_data.get_version(), el::svt_420);
}

BOOST_AUTO_TEST_CASE(shader_source_parameter_save_xml)
{
	el::XmlBuffer buffer;
	el::XmlWriterSharedPtr writer;
	el::ShaderSourceData shader_source_data;

	writer = el::XmlWriterSharedPtr(new el::XmlWriter(buffer.get_buffer()));

	BOOST_CHECK_NO_THROW(shader_source_data.save_xml(writer));
	writer.reset();
}

BOOST_AUTO_TEST_CASE(shader_source_parameter_save_load_xml)
{
	el::XmlBuffer buffer;
	el::XmlWriterSharedPtr writer;
	el::XmlReaderSharedPtr reader;
	el::ShaderSourceData shader_source_data_load;
	el::ShaderSourceData shader_source_data_save;

	writer = el::XmlWriterSharedPtr(new el::XmlWriter(buffer.get_buffer()));

	shader_source_data_save.set_source(el::String(UTF8("3sfd23")));
	shader_source_data_save.set_version(el::svt_150);
	BOOST_CHECK_EQUAL(shader_source_data_save.get_version(), el::svt_150);

	BOOST_CHECK_NO_THROW(shader_source_data_save.save_xml(writer));

	writer.reset();

	reader = el::XmlReaderSharedPtr(new el::XmlReader(buffer.get_buffer()));

	BOOST_CHECK_NO_THROW(shader_source_data_load.load_xml(
		el::String(UTF8("3sfd23")), reader->get_root_node()));

	reader.reset();

	BOOST_CHECK_EQUAL(shader_source_data_load.get_source(),
		UTF8("3sfd23"));
	BOOST_CHECK_EQUAL(shader_source_data_load.get_version(), el::svt_150);
}
