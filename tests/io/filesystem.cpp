/****************************************************************************
 *            filesystem.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "prerequisites.hpp"
#include "filesystem.hpp"
#include <boost/random.hpp>
#define BOOST_TEST_MODULE file_system
#include <boost/test/unit_test.hpp>

using namespace eternal_lands;

BOOST_AUTO_TEST_CASE(create)
{
	boost::scoped_ptr<FileSystem> file_system;

	BOOST_CHECK_NO_THROW(file_system.reset(new FileSystem()));
}

BOOST_AUTO_TEST_CASE(list_files)
{
	boost::scoped_ptr<FileSystem> file_system;
	StringSet files, images;
	Uint16 i;

	BOOST_CHECK_NO_THROW(file_system.reset(new FileSystem()));

	BOOST_CHECK_NO_THROW(file_system->add_dir(String(".")));

	BOOST_CHECK_NO_THROW(files = file_system->get_files(String(),
		String("*.xml")));

	BOOST_CHECK_EQUAL(files.size(), 1);

	if (files.size() > 0)
	{
		BOOST_CHECK_EQUAL(*files.begin(), "materials.xml");
	}

	BOOST_CHECK_NO_THROW(files = file_system->get_files(String("images"),
		String("*.dds")));

	for (i = 0; i < 15; ++i)
	{
		StringStream str;

		str << "images/test" << i << ".dds";

		images.insert(String(str.str()));
	}

	for (i = 0; i < 10; ++i)
	{
		StringStream str;

		str << "images/test" << i << "_color.dds";

		images.insert(String(str.str()));
	}

	BOOST_CHECK_EQUAL_COLLECTIONS(files.begin(), files.end(),
		images.begin(), images.end());
}
