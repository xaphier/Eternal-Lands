/****************************************************************************
 *            materialdescription.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "prerequisites.hpp"
#include "materialdescription.hpp"
#include "xmlbuffer.hpp"
#include "xmlwriter.hpp"
#include "xmlreader.hpp"
#include "tools/randomutil.hpp"
#define BOOST_TEST_MODULE material_description
#include <boost/test/unit_test.hpp>

namespace el = eternal_lands;

namespace
{

	glm::mat2x4 mat2x4_scale_offset(1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	glm::mat2x3 mat2x3_scale_offset(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f);
	glm::mat2x3 texture_matrix;
	glm::vec4 vec4_scale_offset(1.0f, 1.0f, 0.0f, 0.0f);
	glm::vec4 vec4_zero(0.0f);
	glm::vec4 vec4_one(1.0f);

}

BOOST_FIXTURE_TEST_SUITE(random_tool, el::RandomUtil)

BOOST_AUTO_TEST_CASE(default_creation)
{
	el::MaterialDescription material_description;

	BOOST_CHECK_EQUAL(material_description.get_texture(el::spt_albedo_0),
		"");
	BOOST_CHECK_EQUAL(material_description.get_texture(el::spt_albedo_1),
		"");
	BOOST_CHECK_EQUAL(material_description.get_texture(el::spt_albedo_2),
		"");
	BOOST_CHECK_EQUAL(material_description.get_texture(el::spt_albedo_3),
		"");
	BOOST_CHECK_EQUAL(material_description.get_texture(el::spt_normal),
		"");
	BOOST_CHECK_EQUAL(material_description.get_texture(el::spt_specular),
		"");
	BOOST_CHECK_EQUAL(material_description.get_texture(el::spt_emission),
		"");
	BOOST_CHECK_EQUAL(material_description.get_texture(el::spt_blend),
		"");
	BOOST_CHECK_EQUAL(material_description.get_texture(el::spt_vertex_vector),
		"");
	BOOST_CHECK_EQUAL(material_description.get_texture(el::spt_vertex_normal),
		"");
	BOOST_CHECK_EQUAL(material_description.get_texture(el::spt_vertex_dudv),
		"");
	EL_CHECK_EQUAL_VEC_MAT(material_description.get_blend_size(0), vec4_one);
	EL_CHECK_EQUAL_VEC_MAT(material_description.get_blend_size(1), vec4_one);
	EL_CHECK_EQUAL_VEC_MAT(material_description.get_blend_size(2), vec4_one);
	EL_CHECK_EQUAL_VEC_MAT(material_description.get_blend_size(3), vec4_one);
	EL_CHECK_EQUAL_VEC_MAT(material_description.get_albedo_scale_offset(0),
		mat2x4_scale_offset);
	EL_CHECK_EQUAL_VEC_MAT(material_description.get_albedo_scale_offset(1),
		mat2x4_scale_offset);
	EL_CHECK_EQUAL_VEC_MAT(material_description.get_albedo_scale_offset(2),
		mat2x4_scale_offset);
	EL_CHECK_EQUAL_VEC_MAT(material_description.get_albedo_scale_offset(3),
		mat2x4_scale_offset);
	EL_CHECK_EQUAL_VEC_MAT(material_description.get_texture_matrix(0),
		texture_matrix);
	EL_CHECK_EQUAL_VEC_MAT(material_description.get_texture_matrix(1),
		texture_matrix);
	EL_CHECK_EQUAL_VEC_MAT(material_description.get_emission_scale_offset(),
		mat2x3_scale_offset);
	EL_CHECK_EQUAL_VEC_MAT(material_description.get_specular_scale_offset(),
		vec4_scale_offset);
	EL_CHECK_EQUAL_VEC_MAT(material_description.get_color(), vec4_zero);
	BOOST_CHECK_EQUAL(material_description.get_name(), "");
	BOOST_CHECK_EQUAL(material_description.get_effect(), "");
	BOOST_CHECK_EQUAL(material_description.get_script(), "");
	BOOST_CHECK_EQUAL(material_description.get_cast_shadows(), true);
	BOOST_CHECK_EQUAL(material_description.get_culling(), true);
}

BOOST_AUTO_TEST_CASE(all)
{
	el::MaterialDescription material_description;
	el::StringArray4 albedos;
	el::String normal, specular, emission, blend, vertex_vector;
	el::String vertex_normal, vertex_dudv, name, effect, script;
	el::Vec4Array4 blend_sizes;
	el::Mat2x4Array4 albedo_scale_offsets;
	el::Mat2x3Array2 texture_matrices;
	glm::mat2x3 emission_scale_offset;
	glm::vec4 color, specular_scale_offset;

	BOOST_FOREACH(glm::vec4 &value, blend_sizes)
	{
		value[0] = get_random_float();
		value[1] = get_random_float();
		value[2] = get_random_float();
		value[3] = get_random_float();
	}

	BOOST_FOREACH(glm::mat2x3 &value, texture_matrices)
	{
		value[0][0] = get_random_float();
		value[0][1] = get_random_float();
		value[0][2] = get_random_float();
		value[1][0] = get_random_float();
		value[1][1] = get_random_float();
		value[1][2] = get_random_float();
	}

	BOOST_FOREACH(glm::mat2x4 &value, albedo_scale_offsets)
	{
		value[0][0] = get_random_float();
		value[0][1] = get_random_float();
		value[0][2] = get_random_float();
		value[0][3] = get_random_float();
		value[1][0] = get_random_float();
		value[1][1] = get_random_float();
		value[1][2] = get_random_float();
		value[1][3] = get_random_float();
	}

	emission_scale_offset[0][0] = get_random_float();
	emission_scale_offset[0][1] = get_random_float();
	emission_scale_offset[0][2] = get_random_float();
	emission_scale_offset[1][0] = get_random_float();
	emission_scale_offset[1][1] = get_random_float();
	emission_scale_offset[1][2] = get_random_float();

	color[0] = get_random_float();
	color[1] = get_random_float();
	color[2] = get_random_float();
	color[3] = get_random_float();

	specular_scale_offset[0] = get_random_float();
	specular_scale_offset[1] = get_random_float();
	specular_scale_offset[2] = get_random_float();
	specular_scale_offset[3] = get_random_float();

	BOOST_FOREACH(el::String &name, albedos)
	{
		name = get_random_name();
	}

	normal = get_random_name();
	specular = get_random_name();
	emission = get_random_name();
	blend = get_random_name();
	vertex_vector = get_random_name();
	vertex_normal = get_random_name();
	vertex_dudv = get_random_name();
	name = get_random_name();
	effect = get_random_name();
	script = get_random_name();

	BOOST_CHECK_NO_THROW(material_description.set_texture(albedos[0],
		el::spt_albedo_0));
	BOOST_CHECK_NO_THROW(material_description.set_texture(albedos[1],
		el::spt_albedo_1));
	BOOST_CHECK_NO_THROW(material_description.set_texture(albedos[2],
		el::spt_albedo_2));
	BOOST_CHECK_NO_THROW(material_description.set_texture(albedos[3],
		el::spt_albedo_3));
	BOOST_CHECK_NO_THROW(material_description.set_texture(normal,
		el::spt_normal));
	BOOST_CHECK_NO_THROW(material_description.set_texture(specular,
		el::spt_specular));
	BOOST_CHECK_NO_THROW(material_description.set_texture(emission,
		el::spt_emission));
	BOOST_CHECK_NO_THROW(material_description.set_texture(blend,
		el::spt_blend));
	BOOST_CHECK_NO_THROW(material_description.set_texture(vertex_vector,
		el::spt_vertex_vector));
	BOOST_CHECK_NO_THROW(material_description.set_texture(vertex_normal,
		el::spt_vertex_normal));
	BOOST_CHECK_NO_THROW(material_description.set_texture(vertex_dudv,
		el::spt_vertex_dudv));
	BOOST_CHECK_NO_THROW(material_description.set_blend_sizes(blend_sizes));
	BOOST_CHECK_NO_THROW(material_description.set_albedo_scale_offsets(
		albedo_scale_offsets));
	BOOST_CHECK_NO_THROW(material_description.set_texture_matrices(
		texture_matrices));
	BOOST_CHECK_NO_THROW(material_description.set_emission_scale_offset(
		emission_scale_offset));
	BOOST_CHECK_NO_THROW(material_description.set_specular_scale_offset(
		specular_scale_offset));
	BOOST_CHECK_NO_THROW(material_description.set_color(color));
	BOOST_CHECK_NO_THROW(material_description.set_name(name));
	BOOST_CHECK_NO_THROW(material_description.set_effect(effect));
	BOOST_CHECK_NO_THROW(material_description.set_script(script));
	BOOST_CHECK_NO_THROW(material_description.set_cast_shadows(false));
	BOOST_CHECK_NO_THROW(material_description.set_culling(false));

	BOOST_CHECK_EQUAL(material_description.get_texture(el::spt_albedo_0),
		albedos[0]);
	BOOST_CHECK_EQUAL(material_description.get_texture(el::spt_albedo_1),
		albedos[1]);
	BOOST_CHECK_EQUAL(material_description.get_texture(el::spt_albedo_2),
		albedos[2]);
	BOOST_CHECK_EQUAL(material_description.get_texture(el::spt_albedo_3),
		albedos[3]);
	BOOST_CHECK_EQUAL(material_description.get_texture(el::spt_normal),
		normal);
	BOOST_CHECK_EQUAL(material_description.get_texture(el::spt_specular),	
		specular);
	BOOST_CHECK_EQUAL(material_description.get_texture(el::spt_emission),
		emission);
	BOOST_CHECK_EQUAL(material_description.get_texture(el::spt_blend),
		blend);
	BOOST_CHECK_EQUAL(material_description.get_texture(
		el::spt_vertex_vector), vertex_vector);
	BOOST_CHECK_EQUAL(material_description.get_texture(
		el::spt_vertex_normal), vertex_normal);
	BOOST_CHECK_EQUAL(material_description.get_texture(
		el::spt_vertex_dudv), vertex_dudv);
	EL_CHECK_EQUAL_VEC_MAT(material_description.get_blend_size(0),
		blend_sizes[0]);
	EL_CHECK_EQUAL_VEC_MAT(material_description.get_blend_size(1),
		blend_sizes[1]);
	EL_CHECK_EQUAL_VEC_MAT(material_description.get_blend_size(2),
		blend_sizes[2]);
	EL_CHECK_EQUAL_VEC_MAT(material_description.get_blend_size(3),
		blend_sizes[3]);
	EL_CHECK_EQUAL_VEC_MAT(material_description.get_albedo_scale_offset(0),
		albedo_scale_offsets[0]);
	EL_CHECK_EQUAL_VEC_MAT(material_description.get_albedo_scale_offset(1),
		albedo_scale_offsets[1]);
	EL_CHECK_EQUAL_VEC_MAT(material_description.get_albedo_scale_offset(2),
		albedo_scale_offsets[2]);
	EL_CHECK_EQUAL_VEC_MAT(material_description.get_albedo_scale_offset(3),
		albedo_scale_offsets[3]);
	EL_CHECK_EQUAL_VEC_MAT(material_description.get_texture_matrix(0),
		texture_matrices[0]);
	EL_CHECK_EQUAL_VEC_MAT(material_description.get_texture_matrix(1),
		texture_matrices[1]);
	EL_CHECK_EQUAL_VEC_MAT(material_description.get_emission_scale_offset(),
		emission_scale_offset);
	EL_CHECK_EQUAL_VEC_MAT(material_description.get_specular_scale_offset(),
		specular_scale_offset);
	EL_CHECK_EQUAL_VEC_MAT(material_description.get_color(), color);
	BOOST_CHECK_EQUAL(material_description.get_name(), name);
	BOOST_CHECK_EQUAL(material_description.get_effect(), effect);
	BOOST_CHECK_EQUAL(material_description.get_script(), script);
	BOOST_CHECK_EQUAL(material_description.get_cast_shadows(), false);
	BOOST_CHECK_EQUAL(material_description.get_culling(), false);
}

BOOST_AUTO_TEST_CASE(xml)
{
	el::XmlBuffer buffer;
	el::XmlWriterSharedPtr writer;
	el::XmlReaderSharedPtr reader;
	el::MaterialDescription tmp_material_description;
	el::MaterialDescription material_description;
	el::StringArray4 albedos;
	el::String normal, specular, emission, blend, vertex_vector;
	el::String vertex_normal, vertex_dudv, name, effect, script;
	el::Vec4Array4 blend_sizes;
	el::Mat2x4Array4 albedo_scale_offsets;
	el::Mat2x3Array2 texture_matrices;
	glm::mat2x3 emission_scale_offset;
	glm::vec4 color, specular_scale_offset;

	BOOST_FOREACH(glm::vec4 &value, blend_sizes)
	{
		value[0] = get_random_int_float();
		value[1] = get_random_int_float();
		value[2] = get_random_int_float();
		value[3] = get_random_int_float();
	}

	BOOST_FOREACH(glm::mat2x3 &value, texture_matrices)
	{
		value[0][0] = get_random_int_float();
		value[0][1] = get_random_int_float();
		value[0][2] = get_random_int_float();
		value[1][0] = get_random_int_float();
		value[1][1] = get_random_int_float();
		value[1][2] = get_random_int_float();
	}

	BOOST_FOREACH(glm::mat2x4 &value, albedo_scale_offsets)
	{
		value[0][0] = get_random_int_float();
		value[0][1] = get_random_int_float();
		value[0][2] = get_random_int_float();
		value[0][3] = get_random_int_float();
		value[1][0] = get_random_int_float();
		value[1][1] = get_random_int_float();
		value[1][2] = get_random_int_float();
		value[1][3] = get_random_int_float();
	}

	emission_scale_offset[0][0] = get_random_int_float();
	emission_scale_offset[0][1] = get_random_int_float();
	emission_scale_offset[0][2] = get_random_int_float();
	emission_scale_offset[1][0] = get_random_int_float();
	emission_scale_offset[1][1] = get_random_int_float();
	emission_scale_offset[1][2] = get_random_int_float();

	color[0] = get_random_int_float();
	color[1] = get_random_int_float();
	color[2] = get_random_int_float();
	color[3] = get_random_int_float();

	specular_scale_offset[0] = get_random_int_float();
	specular_scale_offset[1] = get_random_int_float();
	specular_scale_offset[2] = get_random_int_float();
	specular_scale_offset[3] = get_random_int_float();

	BOOST_FOREACH(el::String &name, albedos)
	{
		name = get_random_name();
	}

	normal = get_random_name();
	specular = get_random_name();
	emission = get_random_name();
	blend = get_random_name();
	vertex_vector = get_random_name();
	vertex_normal = get_random_name();
	vertex_dudv = get_random_name();
	name = get_random_name();
	effect = get_random_name();
	script = get_random_name();

	BOOST_CHECK_NO_THROW(tmp_material_description.set_texture(albedos[0],
		el::spt_albedo_0));
	BOOST_CHECK_NO_THROW(tmp_material_description.set_texture(albedos[1],
		el::spt_albedo_1));
	BOOST_CHECK_NO_THROW(tmp_material_description.set_texture(albedos[2],
		el::spt_albedo_2));
	BOOST_CHECK_NO_THROW(tmp_material_description.set_texture(albedos[3],
		el::spt_albedo_3));
	BOOST_CHECK_NO_THROW(tmp_material_description.set_texture(normal,
		el::spt_normal));
	BOOST_CHECK_NO_THROW(tmp_material_description.set_texture(specular,
		el::spt_specular));
	BOOST_CHECK_NO_THROW(tmp_material_description.set_texture(emission,
		el::spt_emission));
	BOOST_CHECK_NO_THROW(tmp_material_description.set_texture(blend,
		el::spt_blend));
	BOOST_CHECK_NO_THROW(tmp_material_description.set_texture(vertex_vector,
		el::spt_vertex_vector));
	BOOST_CHECK_NO_THROW(tmp_material_description.set_texture(vertex_normal,
		el::spt_vertex_normal));
	BOOST_CHECK_NO_THROW(tmp_material_description.set_texture(vertex_dudv,
		el::spt_vertex_dudv));
	BOOST_CHECK_NO_THROW(tmp_material_description.set_blend_sizes(
		blend_sizes));
	BOOST_CHECK_NO_THROW(tmp_material_description.set_albedo_scale_offsets(
		albedo_scale_offsets));
	BOOST_CHECK_NO_THROW(tmp_material_description.set_texture_matrices(
		texture_matrices));
	BOOST_CHECK_NO_THROW(tmp_material_description.set_emission_scale_offset(
		emission_scale_offset));
	BOOST_CHECK_NO_THROW(tmp_material_description.set_specular_scale_offset(
		specular_scale_offset));
	BOOST_CHECK_NO_THROW(tmp_material_description.set_color(color));
	BOOST_CHECK_NO_THROW(tmp_material_description.set_name(name));
	BOOST_CHECK_NO_THROW(tmp_material_description.set_effect(effect));
	BOOST_CHECK_NO_THROW(tmp_material_description.set_script(script));
	BOOST_CHECK_NO_THROW(tmp_material_description.set_cast_shadows(false));
	BOOST_CHECK_NO_THROW(tmp_material_description.set_culling(false));

	writer = el::XmlWriterSharedPtr(new el::XmlWriter(buffer.get_buffer()));

	BOOST_CHECK_NO_THROW(tmp_material_description.save_xml(writer));

	writer.reset();

	reader = el::XmlReaderSharedPtr(new el::XmlReader(buffer.get_buffer()));

	BOOST_CHECK_NO_THROW(material_description.load_xml(
		reader->get_root_node()));

	reader.reset();

	BOOST_CHECK_EQUAL(material_description.get_texture(el::spt_albedo_0),
		albedos[0]);
	BOOST_CHECK_EQUAL(material_description.get_texture(el::spt_albedo_1),
		albedos[1]);
	BOOST_CHECK_EQUAL(material_description.get_texture(el::spt_albedo_2),
		albedos[2]);
	BOOST_CHECK_EQUAL(material_description.get_texture(el::spt_albedo_3),
		albedos[3]);
	BOOST_CHECK_EQUAL(material_description.get_texture(el::spt_normal),
		normal);
	BOOST_CHECK_EQUAL(material_description.get_texture(el::spt_specular),	
		specular);
	BOOST_CHECK_EQUAL(material_description.get_texture(el::spt_emission),
		emission);
	BOOST_CHECK_EQUAL(material_description.get_texture(el::spt_blend),
		blend);
	BOOST_CHECK_EQUAL(material_description.get_texture(
		el::spt_vertex_vector), vertex_vector);
	BOOST_CHECK_EQUAL(material_description.get_texture(
		el::spt_vertex_normal), vertex_normal);
	BOOST_CHECK_EQUAL(material_description.get_texture(
		el::spt_vertex_dudv), vertex_dudv);
	EL_CHECK_EQUAL_VEC_MAT(material_description.get_blend_size(0),
		blend_sizes[0]);
	EL_CHECK_EQUAL_VEC_MAT(material_description.get_blend_size(1),
		blend_sizes[1]);
	EL_CHECK_EQUAL_VEC_MAT(material_description.get_blend_size(2),
		blend_sizes[2]);
	EL_CHECK_EQUAL_VEC_MAT(material_description.get_blend_size(3),
		blend_sizes[3]);
	EL_CHECK_EQUAL_VEC_MAT(material_description.get_albedo_scale_offset(0),
		albedo_scale_offsets[0]);
	EL_CHECK_EQUAL_VEC_MAT(material_description.get_albedo_scale_offset(1),
		albedo_scale_offsets[1]);
	EL_CHECK_EQUAL_VEC_MAT(material_description.get_albedo_scale_offset(2),
		albedo_scale_offsets[2]);
	EL_CHECK_EQUAL_VEC_MAT(material_description.get_albedo_scale_offset(3),
		albedo_scale_offsets[3]);
	EL_CHECK_EQUAL_VEC_MAT(material_description.get_texture_matrix(0),
		texture_matrices[0]);
	EL_CHECK_EQUAL_VEC_MAT(material_description.get_texture_matrix(1),
		texture_matrices[1]);
	EL_CHECK_EQUAL_VEC_MAT(material_description.get_emission_scale_offset(),
		emission_scale_offset);
	EL_CHECK_EQUAL_VEC_MAT(material_description.get_specular_scale_offset(),
		specular_scale_offset);
	EL_CHECK_EQUAL_VEC_MAT(material_description.get_color(), color);
	BOOST_CHECK_EQUAL(material_description.get_name(), name);
	BOOST_CHECK_EQUAL(material_description.get_effect(), effect);
	BOOST_CHECK_EQUAL(material_description.get_script(), script);
	BOOST_CHECK_EQUAL(material_description.get_cast_shadows(), false);
	BOOST_CHECK_EQUAL(material_description.get_culling(), false);
}

BOOST_AUTO_TEST_SUITE_END()
