/****************************************************************************
 *            shadersourceparameter.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "prerequisites.hpp"
#include "shader/shadersourcedata.hpp"
#include "utf.hpp"
#include "xmlbuffer.hpp"
#include "xmlwriter.hpp"
#include "xmlreader.hpp"
#include <glm/gtx/epsilon.hpp>
#include <boost/random.hpp>
#include <boost/exception/diagnostic_information.hpp>
#define BOOST_TEST_MODULE objectdata
#include <boost/test/unit_test.hpp>

namespace el = eternal_lands;

BOOST_AUTO_TEST_CASE(shader_source_data_default_creation)
{
	el::ShaderSourceData shader_source_data;

	BOOST_CHECK_EQUAL(el::string_to_utf8(
		shader_source_data.get_source()), "");

	BOOST_CHECK_EQUAL(shader_source_data.get_glsl_120(), false);
	BOOST_CHECK_EQUAL(shader_source_data.get_glsl_120(), false);
	BOOST_CHECK_EQUAL(shader_source_data.get_glsl_140(), false);
	BOOST_CHECK_EQUAL(shader_source_data.get_glsl_150(), false);
	BOOST_CHECK_EQUAL(shader_source_data.get_glsl_330(), false);
}

BOOST_AUTO_TEST_CASE(shader_source_data_source)
{
	el::ShaderSourceData shader_source_data;

	shader_source_data.set_source(el::String(UTF8("3sfd23")));
	BOOST_CHECK_EQUAL(shader_source_data.get_source(), UTF8("3sfd23"));

	shader_source_data.set_source(el::String(UTF8("w4tgi3")));
	BOOST_CHECK_EQUAL(shader_source_data.get_source(), UTF8("w4tgi3"));
}

BOOST_AUTO_TEST_CASE(shader_source_data_options)
{
	el::ShaderSourceData shader_source_data;

	shader_source_data.set_glsl_120(false);
	BOOST_CHECK_EQUAL(shader_source_data.get_glsl_120(), false);

	shader_source_data.set_glsl_120(true);
	BOOST_CHECK_EQUAL(shader_source_data.get_glsl_120(), true);

	shader_source_data.set_glsl_130(false);
	BOOST_CHECK_EQUAL(shader_source_data.get_glsl_130(), false);

	shader_source_data.set_glsl_130(true);
	BOOST_CHECK_EQUAL(shader_source_data.get_glsl_130(), true);

	shader_source_data.set_glsl_140(false);
	BOOST_CHECK_EQUAL(shader_source_data.get_glsl_140(), false);

	shader_source_data.set_glsl_140(true);
	BOOST_CHECK_EQUAL(shader_source_data.get_glsl_140(), true);

	shader_source_data.set_glsl_150(false);
	BOOST_CHECK_EQUAL(shader_source_data.get_glsl_150(), false);

	shader_source_data.set_glsl_150(true);
	BOOST_CHECK_EQUAL(shader_source_data.get_glsl_150(), true);

	shader_source_data.set_glsl_330(false);
	BOOST_CHECK_EQUAL(shader_source_data.get_glsl_330(), false);

	shader_source_data.set_glsl_330(true);
	BOOST_CHECK_EQUAL(shader_source_data.get_glsl_330(), true);
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
	shader_source_data_save.set_glsl_120(false);
	shader_source_data_save.set_glsl_130(true);
	shader_source_data_save.set_glsl_140(false);
	shader_source_data_save.set_glsl_150(true);
	shader_source_data_save.set_glsl_330(false);

	BOOST_CHECK_NO_THROW(shader_source_data_save.save_xml(writer));

	writer.reset();

	reader = el::XmlReaderSharedPtr(new el::XmlReader(buffer.get_buffer()));

	BOOST_CHECK_NO_THROW(shader_source_data_load.load_xml(
		reader->get_root_node()));

	reader.reset();

	BOOST_CHECK_EQUAL(shader_source_data_load.get_source(),
		UTF8("3sfd23"));
	BOOST_CHECK_EQUAL(shader_source_data_load.get_glsl_120(), false);
	BOOST_CHECK_EQUAL(shader_source_data_load.get_glsl_130(), true);
	BOOST_CHECK_EQUAL(shader_source_data_load.get_glsl_140(), false);
	BOOST_CHECK_EQUAL(shader_source_data_load.get_glsl_150(), true);
	BOOST_CHECK_EQUAL(shader_source_data_load.get_glsl_330(), false);
}