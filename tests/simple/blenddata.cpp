/****************************************************************************
 *            blenddata.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "prerequisites.hpp"
#include "blenddata.hpp"
#include "xmlbuffer.hpp"
#include "xmlwriter.hpp"
#include "xmlreader.hpp"
#define BOOST_TEST_MODULE blend_data
#include <boost/test/unit_test.hpp>

namespace el = eternal_lands;

BOOST_AUTO_TEST_CASE(default_creation)
{
	el::BlendData blend_data;

	BOOST_CHECK_CLOSE(blend_data.get_blend_size(), 1.0f, 1.0);
	BOOST_CHECK_EQUAL(blend_data.get_use_blend_size(), false);
}

BOOST_AUTO_TEST_CASE(blend)
{
	el::BlendData blend_data;

	BOOST_CHECK_NO_THROW(blend_data.set_blend_size(0.5f));
	BOOST_CHECK_NO_THROW(blend_data.set_use_blend_size(true));

	BOOST_CHECK_CLOSE(blend_data.get_blend_size(), 0.5f, 1.0);
	BOOST_CHECK_EQUAL(blend_data.get_use_blend_size(), true);

	BOOST_CHECK_NO_THROW(blend_data.set_blend_size(0.75f));
	BOOST_CHECK_NO_THROW(blend_data.set_use_blend_size(false));

	BOOST_CHECK_CLOSE(blend_data.get_blend_size(), 0.75f, 1.0);
	BOOST_CHECK_EQUAL(blend_data.get_use_blend_size(), false);

	BOOST_CHECK_NO_THROW(blend_data.set_blend_size(0.01f));
	BOOST_CHECK_NO_THROW(blend_data.set_use_blend_size(false));

	BOOST_CHECK_CLOSE(blend_data.get_blend_size(), 0.01f, 1.0);
	BOOST_CHECK_EQUAL(blend_data.get_use_blend_size(), false);

	BOOST_CHECK_NO_THROW(blend_data.set_blend_size(0.99f));
	BOOST_CHECK_NO_THROW(blend_data.set_use_blend_size(true));

	BOOST_CHECK_CLOSE(blend_data.get_blend_size(), 0.99f, 1.0);
	BOOST_CHECK_EQUAL(blend_data.get_use_blend_size(), true);
}

BOOST_AUTO_TEST_CASE(blend_data_save_xml)
{
	el::XmlBufferSharedPtr buffer;
	el::XmlWriterSharedPtr writer;
	el::BlendData blend_data;

	buffer = boost::make_shared<el::XmlBuffer>();
	writer = boost::make_shared<el::XmlWriter>(buffer);

	BOOST_CHECK_NO_THROW(blend_data.save_xml(writer));
	writer.reset();
}

BOOST_AUTO_TEST_CASE(blend_data_save_load_xml)
{
	el::XmlBufferSharedPtr buffer;
	el::XmlWriterSharedPtr writer;
	el::XmlReaderSharedPtr reader;
	el::BlendData blend_data_load;
	el::BlendData blend_data_save;

	buffer = boost::make_shared<el::XmlBuffer>();
	writer = boost::make_shared<el::XmlWriter>(buffer);

	BOOST_CHECK_NO_THROW(blend_data_save.set_blend_size(0.5f));
	BOOST_CHECK_NO_THROW(blend_data_save.set_use_blend_size(true));

	BOOST_CHECK_NO_THROW(blend_data_save.save_xml(writer));

	writer.reset();

	reader = boost::make_shared<el::XmlReader>(buffer);

	BOOST_CHECK_NO_THROW(blend_data_load.load_xml(
		reader->get_root_node()));

	reader.reset();

	BOOST_CHECK_CLOSE(blend_data_load.get_blend_size(), 0.5f, 1.0);
	BOOST_CHECK_EQUAL(blend_data_load.get_use_blend_size(), true);
}
