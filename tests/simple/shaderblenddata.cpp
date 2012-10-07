/****************************************************************************
 *            shadersourcedata.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "prerequisites.hpp"
#include "shader/shaderblenddata.hpp"
#include "xmlbuffer.hpp"
#include "xmlwriter.hpp"
#include "xmlreader.hpp"
#define BOOST_TEST_MODULE shader_blend_data
#include <boost/test/unit_test.hpp>

namespace el = eternal_lands;

BOOST_AUTO_TEST_CASE(default_creation)
{
	el::ShaderBlendData shader_blend_data;

	BOOST_CHECK_SMALL(shader_blend_data.get_scale_offset().x, el::epsilon);
	BOOST_CHECK_SMALL(shader_blend_data.get_scale_offset().y, el::epsilon);
	BOOST_CHECK_EQUAL(shader_blend_data.get_blend(), el::sbt_texture);
}

BOOST_AUTO_TEST_CASE(blend)
{
	el::ShaderBlendData shader_blend_data;

	shader_blend_data.set_blend(el::sbt_height);
	BOOST_CHECK_EQUAL(shader_blend_data.get_blend(), el::sbt_height);

	shader_blend_data.set_blend(el::sbt_slope);
	BOOST_CHECK_EQUAL(shader_blend_data.get_blend(), el::sbt_slope);

	shader_blend_data.set_blend(el::sbt_size);
	BOOST_CHECK_EQUAL(shader_blend_data.get_blend(), el::sbt_size);

	shader_blend_data.set_blend(el::sbt_texture);
	BOOST_CHECK_EQUAL(shader_blend_data.get_blend(), el::sbt_texture);
}

BOOST_AUTO_TEST_CASE(blend_size)
{
	el::ShaderBlendData shader_blend_data;

	shader_blend_data.set_blend_size(0.3f);
	BOOST_CHECK_CLOSE(shader_blend_data.get_blend_size(), 0.3f, 1.0);

	shader_blend_data.set_blend_size(0.7f);
	BOOST_CHECK_CLOSE(shader_blend_data.get_blend_size(), 0.7f, 1.0);

	shader_blend_data.set_blend_size(0.1f);
	BOOST_CHECK_CLOSE(shader_blend_data.get_blend_size(), 0.1f, 1.0);

	shader_blend_data.set_blend_size(0.5f);
	BOOST_CHECK_CLOSE(shader_blend_data.get_blend_size(), 0.5f, 1.0);
}

BOOST_AUTO_TEST_CASE(scale_offset)
{
	el::ShaderBlendData shader_blend_data;

	shader_blend_data.set_scale_offset(glm::vec2(0.3f, 0.7f));
	BOOST_CHECK_CLOSE(shader_blend_data.get_scale_offset().x, 0.3f, 1.0);
	BOOST_CHECK_CLOSE(shader_blend_data.get_scale_offset().y, 0.7f, 1.0);

	shader_blend_data.set_scale_offset(glm::vec2(0.9f, 0.1f));
	BOOST_CHECK_CLOSE(shader_blend_data.get_scale_offset().x, 0.9f, 1.0);
	BOOST_CHECK_CLOSE(shader_blend_data.get_scale_offset().y, 0.1f, 1.0);

	shader_blend_data.set_scale_offset(glm::vec2(0.5f, 0.5f));
	BOOST_CHECK_CLOSE(shader_blend_data.get_scale_offset().x, 0.5f, 1.0);
	BOOST_CHECK_CLOSE(shader_blend_data.get_scale_offset().y, 0.5f, 1.0);

	shader_blend_data.set_scale_offset(glm::vec2(2.5f, -1.7f));
	BOOST_CHECK_CLOSE(shader_blend_data.get_scale_offset().x, 2.5f, 1.0);
	BOOST_CHECK_CLOSE(shader_blend_data.get_scale_offset().y, -1.7f, 1.0);
}

BOOST_AUTO_TEST_CASE(shader_blend_data_save_xml)
{
	el::XmlBufferSharedPtr buffer;
	el::XmlWriterSharedPtr writer;
	el::ShaderBlendData shader_blend_data;

	buffer = boost::make_shared<el::XmlBuffer>();
	writer = boost::make_shared<el::XmlWriter>(buffer);

	BOOST_CHECK_NO_THROW(shader_blend_data.save_xml(writer));
	writer.reset();
}

BOOST_AUTO_TEST_CASE(shader_blend_data_save_load_xml)
{
	el::XmlBufferSharedPtr buffer;
	el::XmlWriterSharedPtr writer;
	el::XmlReaderSharedPtr reader;
	el::ShaderBlendData shader_blend_data_load;
	el::ShaderBlendData shader_blend_data_save;

	buffer = boost::make_shared<el::XmlBuffer>();
	writer = boost::make_shared<el::XmlWriter>(buffer);

	shader_blend_data_save.set_blend(el::sbt_size);
	shader_blend_data_save.set_scale_offset(glm::vec2(2.5f, -1.7f));

	BOOST_CHECK_NO_THROW(shader_blend_data_save.save_xml(writer));

	writer.reset();

	reader = boost::make_shared<el::XmlReader>(buffer);

	BOOST_CHECK_NO_THROW(shader_blend_data_load.load_xml(
		reader->get_root_node()));

	reader.reset();

	BOOST_CHECK_EQUAL(shader_blend_data_load.get_blend(), el::sbt_size);
	BOOST_CHECK_CLOSE(shader_blend_data_load.get_scale_offset().x, 2.5f,
		1.0);
	BOOST_CHECK_CLOSE(shader_blend_data_load.get_scale_offset().y, -1.7f,
		1.0);
}
