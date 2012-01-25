/****************************************************************************
 *            shadersourceparameter.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "prerequisites.hpp"
#include "shader/shadersourceparameter.hpp"
#include "xmlbuffer.hpp"
#include "xmlwriter.hpp"
#include "xmlreader.hpp"
#define BOOST_TEST_MODULE shader_source_parameter
#include <boost/test/unit_test.hpp>

namespace el = eternal_lands;

BOOST_AUTO_TEST_CASE(default_creation)
{
	el::ShaderSourceParameter shader_source_parameter(
		el::String(UTF8("346")), el::String(UTF8("3sfd23")),
		el::pt_uvec3, el::pqt_inout, el::pst_bone_count, 14536u);

	BOOST_CHECK_EQUAL(shader_source_parameter.get_source(), UTF8("346"));
	BOOST_CHECK_EQUAL(shader_source_parameter.get_name(), UTF8("3sfd23"));
	BOOST_CHECK_EQUAL(shader_source_parameter.get_type(), el::pt_uvec3);
	BOOST_CHECK_EQUAL(shader_source_parameter.get_qualifier(),
		el::pqt_inout);
	BOOST_CHECK_EQUAL(shader_source_parameter.get_size(),
		el::pst_bone_count);
	BOOST_CHECK_EQUAL(shader_source_parameter.get_scale(), 14536u);
}

BOOST_AUTO_TEST_CASE(source)
{
	el::ShaderSourceParameter shader_source_parameter;

	shader_source_parameter.set_source(el::String(UTF8("fasd")));
	BOOST_CHECK_EQUAL(shader_source_parameter.get_source(), UTF8("fasd"));

	shader_source_parameter.set_source(el::String(UTF8("znr6")));
	BOOST_CHECK_EQUAL(shader_source_parameter.get_source(), UTF8("znr6"));
}

BOOST_AUTO_TEST_CASE(name)
{
	el::ShaderSourceParameter shader_source_parameter;

	shader_source_parameter.set_name(el::String(UTF8("3sfd23")));
	BOOST_CHECK_EQUAL(shader_source_parameter.get_name(), UTF8("3sfd23"));

	shader_source_parameter.set_name(el::String(UTF8("w4tgi3")));
	BOOST_CHECK_EQUAL(shader_source_parameter.get_name(), UTF8("w4tgi3"));
}

BOOST_AUTO_TEST_CASE(type)
{
	el::ShaderSourceParameter shader_source_parameter;

	shader_source_parameter.set_type(el::pt_uvec3);
	BOOST_CHECK_EQUAL(shader_source_parameter.get_type(), el::pt_uvec3);

	shader_source_parameter.set_type(el::pt_bvec2);
	BOOST_CHECK_EQUAL(shader_source_parameter.get_type(), el::pt_bvec2);

	shader_source_parameter.set_type(el::pt_float);
	BOOST_CHECK_EQUAL(shader_source_parameter.get_type(), el::pt_float);

	shader_source_parameter.set_type(el::pt_mat3x2);
	BOOST_CHECK_EQUAL(shader_source_parameter.get_type(), el::pt_mat3x2);

	shader_source_parameter.set_type(el::pt_samplerCube);
	BOOST_CHECK_EQUAL(shader_source_parameter.get_type(),
		el::pt_samplerCube);

	shader_source_parameter.set_type(el::pt_sampler1DArray);
	BOOST_CHECK_EQUAL(shader_source_parameter.get_type(),
		el::pt_sampler1DArray);

	shader_source_parameter.set_type(el::pt_isampler2DArray);
	BOOST_CHECK_EQUAL(shader_source_parameter.get_type(),
		el::pt_isampler2DArray);

	shader_source_parameter.set_type(el::pt_usamplerBuffer);
	BOOST_CHECK_EQUAL(shader_source_parameter.get_type(),
		el::pt_usamplerBuffer);
}

BOOST_AUTO_TEST_CASE(qualifier)
{
	el::ShaderSourceParameter shader_source_parameter;

	shader_source_parameter.set_qualifier(el::pqt_in);
	BOOST_CHECK_EQUAL(shader_source_parameter.get_qualifier(),
		el::pqt_in);

	shader_source_parameter.set_qualifier(el::pqt_out);
	BOOST_CHECK_EQUAL(shader_source_parameter.get_qualifier(),
		el::pqt_out);

	shader_source_parameter.set_qualifier(el::pqt_inout);
	BOOST_CHECK_EQUAL(shader_source_parameter.get_qualifier(),
		el::pqt_inout);
}

BOOST_AUTO_TEST_CASE(size)
{
	el::ShaderSourceParameter shader_source_parameter;

	shader_source_parameter.set_size(el::pst_one);
	BOOST_CHECK_EQUAL(shader_source_parameter.get_size(),
		el::pst_one);

	shader_source_parameter.set_size(el::pst_light_count);
	BOOST_CHECK_EQUAL(shader_source_parameter.get_size(),
		el::pst_light_count);

	shader_source_parameter.set_size(el::pst_bone_count);
	BOOST_CHECK_EQUAL(shader_source_parameter.get_size(),
		el::pst_bone_count);

	shader_source_parameter.set_size(el::pst_shadow_map_count);
	BOOST_CHECK_EQUAL(shader_source_parameter.get_size(),
		el::pst_shadow_map_count);
}

BOOST_AUTO_TEST_CASE(scale)
{
	el::ShaderSourceParameter shader_source_parameter;

	shader_source_parameter.set_scale(345u);
	BOOST_CHECK_EQUAL(shader_source_parameter.get_scale(), 345u);

	shader_source_parameter.set_scale(14536u);
	BOOST_CHECK_EQUAL(shader_source_parameter.get_scale(), 14536u);

	shader_source_parameter.set_scale(1u);
	BOOST_CHECK_EQUAL(shader_source_parameter.get_scale(), 1u);

	shader_source_parameter.set_scale(65u);
	BOOST_CHECK_EQUAL(shader_source_parameter.get_scale(), 65u);
}

BOOST_AUTO_TEST_CASE(save_xml)
{
	el::XmlBuffer buffer;
	el::XmlWriterSharedPtr writer;
	el::ShaderSourceParameter shader_source_parameter(
		el::String(UTF8("fasd")), el::String(UTF8("3sfd23")),
		el::pt_uvec3, el::pqt_inout, el::pst_bone_count, 14536u);

	writer = el::XmlWriterSharedPtr(new el::XmlWriter(buffer.get_buffer()));

	BOOST_CHECK_NO_THROW(shader_source_parameter.save_xml(writer));
	writer.reset();
}

BOOST_AUTO_TEST_CASE(load_xml)
{
	el::XmlBuffer buffer(UTF8("<?xml version=\"1.0\" encoding=\"utf8\"?>"
		"<parameter><name>3sfd23</name><type>uvec3</type>"
		"<qualifier>inout</qualifier><size>bone_count</size>"
		"<scale>14536</scale></parameter>"));
	el::XmlReaderSharedPtr xml_reader;
	el::ShaderSourceParameter shader_source_parameter;

	xml_reader = el::XmlReaderSharedPtr(new el::XmlReader(
		buffer.get_buffer()));

	BOOST_CHECK_NO_THROW(shader_source_parameter.load_xml(
		el::String(UTF8("fasd")), xml_reader->get_root_node()));

	xml_reader.reset();

	BOOST_CHECK_EQUAL(shader_source_parameter.get_source(), UTF8("fasd"));
	BOOST_CHECK_EQUAL(shader_source_parameter.get_name(), UTF8("3sfd23"));
	BOOST_CHECK_EQUAL(shader_source_parameter.get_type(), el::pt_uvec3);
	BOOST_CHECK_EQUAL(shader_source_parameter.get_qualifier(),
		el::pqt_inout);
	BOOST_CHECK_EQUAL(shader_source_parameter.get_size(),
		el::pst_bone_count);
	BOOST_CHECK_EQUAL(shader_source_parameter.get_scale(), 14536u);
}

BOOST_AUTO_TEST_CASE(save_load_xml)
{
	el::XmlBuffer buffer;
	el::XmlWriterSharedPtr writer;
	el::XmlReaderSharedPtr reader;
	el::ShaderSourceParameter shader_source_parameter_load;
	el::ShaderSourceParameter shader_source_parameter_save(
		el::String(UTF8("fasd")), el::String(UTF8("3sfd23")),
		el::pt_uvec3, el::pqt_inout, el::pst_bone_count, 14536u);

	writer = el::XmlWriterSharedPtr(new el::XmlWriter(buffer.get_buffer()));

	BOOST_CHECK_NO_THROW(shader_source_parameter_save.save_xml(writer));

	writer.reset();

	reader = el::XmlReaderSharedPtr(new el::XmlReader(buffer.get_buffer()));

	BOOST_CHECK_NO_THROW(shader_source_parameter_load.load_xml(
		el::String(UTF8("asdggrfgrf34")), reader->get_root_node()));

	reader.reset();

	BOOST_CHECK_EQUAL(shader_source_parameter_load.get_source(),
		UTF8("asdggrfgrf34"));
	BOOST_CHECK_EQUAL(shader_source_parameter_load.get_name(),
		UTF8("3sfd23"));
	BOOST_CHECK_EQUAL(shader_source_parameter_load.get_type(),
		el::pt_uvec3);
	BOOST_CHECK_EQUAL(shader_source_parameter_load.get_qualifier(),
		el::pqt_inout);
	BOOST_CHECK_EQUAL(shader_source_parameter_load.get_size(),
		el::pst_bone_count);
	BOOST_CHECK_EQUAL(shader_source_parameter_load.get_scale(), 14536u);
}
