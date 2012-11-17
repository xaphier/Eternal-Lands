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

}

BOOST_FIXTURE_TEST_SUITE(random_tool, el::RandomUtil)

BOOST_AUTO_TEST_CASE(default_creation)
{
	el::MaterialDescription material_description;
	Uint32 i;

	for (i = 0; i < el::material_texture_count; ++i)
	{
		BOOST_CHECK_EQUAL(material_description.get_texture(i), "");
		BOOST_CHECK_EQUAL(material_description.get_sRGB(i), false);
	}

	EL_CHECK_EQUAL_VEC_MAT(material_description.get_texture_matrix(0),
		texture_matrix);
	EL_CHECK_EQUAL_VEC_MAT(material_description.get_texture_matrix(1),
		texture_matrix);
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
	el::MaterialStringArray textures;
	el::String name, effect, script;
	el::Mat2x3Array2 texture_matrices;
	el::BitSet64 sRGB;
	glm::vec4 color;
	Uint32 i;

	BOOST_FOREACH(glm::mat2x3 &value, texture_matrices)
	{
		value[0][0] = get_random_float();
		value[0][1] = get_random_float();
		value[0][2] = get_random_float();
		value[1][0] = get_random_float();
		value[1][1] = get_random_float();
		value[1][2] = get_random_float();
	}

	color[0] = get_random_float();
	color[1] = get_random_float();
	color[2] = get_random_float();
	color[3] = get_random_float();

	BOOST_FOREACH(el::String &name, textures)
	{
		name = get_random_name();
	}

	for (i = 0; i < el::material_texture_count; ++i)
	{
		sRGB[i] = get_random_bool();
	}

	name = get_random_name();
	effect = get_random_name();
	script = get_random_name();

	for (i = 0; i < el::material_texture_count; ++i)
	{
		BOOST_CHECK_NO_THROW(material_description.set_texture(
			textures[i], i));
		BOOST_CHECK_NO_THROW(material_description.set_sRGB(sRGB[i], i));
	}

	BOOST_CHECK_NO_THROW(material_description.set_texture_matrices(
		texture_matrices));
	BOOST_CHECK_NO_THROW(material_description.set_color(color));
	BOOST_CHECK_NO_THROW(material_description.set_name(name));
	BOOST_CHECK_NO_THROW(material_description.set_effect(effect));
	BOOST_CHECK_NO_THROW(material_description.set_script(script));
	BOOST_CHECK_NO_THROW(material_description.set_cast_shadows(false));
	BOOST_CHECK_NO_THROW(material_description.set_culling(false));

	for (i = 0; i < el::material_texture_count; ++i)
	{
		BOOST_CHECK_EQUAL(material_description.get_texture(i),
			textures[i]);
		BOOST_CHECK_EQUAL(material_description.get_sRGB(i), sRGB[i]);
	}

	EL_CHECK_EQUAL_VEC_MAT(material_description.get_texture_matrix(0),
		texture_matrices[0]);
	EL_CHECK_EQUAL_VEC_MAT(material_description.get_texture_matrix(1),
		texture_matrices[1]);
	EL_CHECK_EQUAL_VEC_MAT(material_description.get_color(), color);
	BOOST_CHECK_EQUAL(material_description.get_name(), name);
	BOOST_CHECK_EQUAL(material_description.get_effect(), effect);
	BOOST_CHECK_EQUAL(material_description.get_script(), script);
	BOOST_CHECK_EQUAL(material_description.get_cast_shadows(), false);
	BOOST_CHECK_EQUAL(material_description.get_culling(), false);
}

BOOST_AUTO_TEST_CASE(xml)
{
	el::XmlBufferSharedPtr buffer;
	el::XmlWriterSharedPtr writer;
	el::XmlReaderSharedPtr reader;
	el::MaterialDescription tmp_material_description;
	el::MaterialDescription material_description;
	el::MaterialStringArray textures;
	el::String name, effect, script;
	el::Mat2x3Array2 texture_matrices;
	el::BitSet64 sRGB;
	glm::vec4 color;
	Uint32 i;

	BOOST_FOREACH(glm::mat2x3 &value, texture_matrices)
	{
		value[0][0] = get_random_int_float();
		value[0][1] = get_random_int_float();
		value[0][2] = get_random_int_float();
		value[1][0] = get_random_int_float();
		value[1][1] = get_random_int_float();
		value[1][2] = get_random_int_float();
	}

	color[0] = get_random_int_float();
	color[1] = get_random_int_float();
	color[2] = get_random_int_float();
	color[3] = get_random_int_float();

	BOOST_FOREACH(el::String &name, textures)
	{
		name = get_random_name();
	}

	for (i = 0; i < el::material_texture_count; ++i)
	{
		sRGB[i] = get_random_bool();
	}

	name = get_random_name();
	effect = get_random_name();
	script = get_random_name();

	for (i = 0; i < el::material_texture_count; ++i)
	{
		BOOST_CHECK_NO_THROW(tmp_material_description.set_texture(
			textures[i], i));
		BOOST_CHECK_NO_THROW(tmp_material_description.set_sRGB(sRGB[i],
			i));
	}

	BOOST_CHECK_NO_THROW(tmp_material_description.set_texture_matrices(
		texture_matrices));
	BOOST_CHECK_NO_THROW(tmp_material_description.set_color(color));
	BOOST_CHECK_NO_THROW(tmp_material_description.set_name(name));
	BOOST_CHECK_NO_THROW(tmp_material_description.set_effect(effect));
	BOOST_CHECK_NO_THROW(tmp_material_description.set_script(script));
	BOOST_CHECK_NO_THROW(tmp_material_description.set_cast_shadows(false));
	BOOST_CHECK_NO_THROW(tmp_material_description.set_culling(false));

	buffer = boost::make_shared<el::XmlBuffer>();
	writer = boost::make_shared<el::XmlWriter>(buffer);

	BOOST_CHECK_NO_THROW(tmp_material_description.save_xml(writer));

	writer.reset();

	reader = boost::make_shared<el::XmlReader>(buffer);

	BOOST_CHECK_NO_THROW(material_description.load_xml(
		reader->get_root_node()));

	reader.reset();

	for (i = 0; i < el::material_texture_count; ++i)
	{
		BOOST_CHECK_EQUAL(material_description.get_texture(i),
			textures[i]);
		BOOST_CHECK_EQUAL(material_description.get_sRGB(i), sRGB[i]);
	}

	EL_CHECK_EQUAL_VEC_MAT(material_description.get_texture_matrix(0),
		texture_matrices[0]);
	EL_CHECK_EQUAL_VEC_MAT(material_description.get_texture_matrix(1),
		texture_matrices[1]);
	EL_CHECK_EQUAL_VEC_MAT(material_description.get_color(), color);
	BOOST_CHECK_EQUAL(material_description.get_name(), name);
	BOOST_CHECK_EQUAL(material_description.get_effect(), effect);
	BOOST_CHECK_EQUAL(material_description.get_script(), script);
	BOOST_CHECK_EQUAL(material_description.get_cast_shadows(), false);
	BOOST_CHECK_EQUAL(material_description.get_culling(), false);
}

BOOST_AUTO_TEST_SUITE_END()
