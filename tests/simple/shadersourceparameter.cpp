/****************************************************************************
 *            shadersourceparameter.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "prerequisites.hpp"
#include "shader/shadersourceparameter.hpp"
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

BOOST_AUTO_TEST_CASE(shader_source_parameter_default_creation)
{
	el::ShaderSourceParameter shader_source_parameter(el::String(L"3sfd23"),
		el::pt_uvec3, el::pqt_inout, el::pst_bone_count, 14536u);

	BOOST_CHECK_EQUAL(el::string_to_utf8(
		shader_source_parameter.get_name()), "3sfd23");

	BOOST_CHECK_EQUAL(shader_source_parameter.get_type(), el::pt_uvec3);

	BOOST_CHECK_EQUAL(shader_source_parameter.get_qualifier(),
		el::pqt_inout);

	BOOST_CHECK_EQUAL(shader_source_parameter.get_size(),
		el::pst_bone_count);

	BOOST_CHECK_EQUAL(shader_source_parameter.get_scale(), 14536u);
}

BOOST_AUTO_TEST_CASE(shader_source_parameter_name)
{
	el::ShaderSourceParameter shader_source_parameter;

	shader_source_parameter.set_name(el::String(L"3sfd23"));
	BOOST_CHECK_EQUAL(el::string_to_utf8(
		shader_source_parameter.get_name()), "3sfd23");

	shader_source_parameter.set_name(el::String(L"w4tgi3"));
	BOOST_CHECK_EQUAL(el::string_to_utf8(
		shader_source_parameter.get_name()), "w4tgi3");
}

BOOST_AUTO_TEST_CASE(shader_source_parameter_type)
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

BOOST_AUTO_TEST_CASE(shader_source_parameter_qualifier)
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

BOOST_AUTO_TEST_CASE(shader_source_parameter_size)
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

BOOST_AUTO_TEST_CASE(shader_source_parameter_scale)
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

BOOST_AUTO_TEST_CASE(shader_source_parameter_save_xml)
{
	el::XmlBuffer buffer;
	el::XmlWriterSharedPtr writer;
	el::ShaderSourceParameter shader_source_parameter(el::String(L"3sfd23"),
		el::pt_uvec3, el::pqt_inout, el::pst_bone_count, 14536u);

	writer = el::XmlWriterSharedPtr(new el::XmlWriter(buffer.get_buffer()));

	BOOST_CHECK_NO_THROW(shader_source_parameter.save_xml(writer));
	writer.reset();
}

BOOST_AUTO_TEST_CASE(shader_source_parameter_load_xml)
{
	el::XmlBuffer buffer("<?xml version=\"1.0\" encoding=\"utf8\"?>"
		"<parameter><name>3sfd23</name><source></source>"
		"<type>uvec3</type><qualifier>inout</qualifier>"
		"<size>bone_count</size><scale>14536</scale></parameter>");
	el::XmlReaderSharedPtr reader;
	el::ShaderSourceParameter shader_source_parameter;

	reader = el::XmlReaderSharedPtr(new el::XmlReader(buffer.get_buffer()));

	BOOST_CHECK_NO_THROW(shader_source_parameter.load_xml(
		reader->get_root_node()));

	reader.reset();

	BOOST_CHECK_EQUAL(el::string_to_utf8(
		shader_source_parameter.get_name()), "3sfd23");

	BOOST_CHECK_EQUAL(shader_source_parameter.get_type(), el::pt_uvec3);

	BOOST_CHECK_EQUAL(shader_source_parameter.get_qualifier(),
		el::pqt_inout);

	BOOST_CHECK_EQUAL(shader_source_parameter.get_size(),
		el::pst_bone_count);

	BOOST_CHECK_EQUAL(shader_source_parameter.get_scale(), 14536u);
}

BOOST_AUTO_TEST_CASE(shader_source_parameter_save_load_xml)
{
	el::XmlBuffer buffer;
	el::XmlWriterSharedPtr writer;
	el::XmlReaderSharedPtr reader;
	el::ShaderSourceParameter shader_source_parameter_load;
	el::ShaderSourceParameter shader_source_parameter_save(
		el::String(L"3sfd23"), el::pt_uvec3, el::pqt_inout,
		el::pst_bone_count, 14536u);

	writer = el::XmlWriterSharedPtr(new el::XmlWriter(buffer.get_buffer()));

	BOOST_CHECK_NO_THROW(shader_source_parameter_save.save_xml(writer));

	writer.reset();

	reader = el::XmlReaderSharedPtr(new el::XmlReader(buffer.get_buffer()));

	BOOST_CHECK_NO_THROW(shader_source_parameter_load.load_xml(
		reader->get_root_node()));

	reader.reset();

	BOOST_CHECK_EQUAL(el::string_to_utf8(
		shader_source_parameter_load.get_name()), "3sfd23");

	BOOST_CHECK_EQUAL(shader_source_parameter_load.get_type(),
		el::pt_uvec3);

	BOOST_CHECK_EQUAL(shader_source_parameter_load.get_qualifier(),
		el::pqt_inout);

	BOOST_CHECK_EQUAL(shader_source_parameter_load.get_size(),
		el::pst_bone_count);

	BOOST_CHECK_EQUAL(shader_source_parameter_load.get_scale(), 14536u);
}
