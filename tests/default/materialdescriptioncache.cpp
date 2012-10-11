/****************************************************************************
 *            materialdescriptioncache.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "prerequisites.hpp"
#include "materialdescription.hpp"
#include "materialdescriptioncache.hpp"
#include "filesystem.hpp"
#define BOOST_TEST_MODULE material_description_cache
#include <boost/test/unit_test.hpp>

namespace el = eternal_lands;

BOOST_AUTO_TEST_CASE(load_xml)
{
	el::MaterialDescriptionCache material_description_cache;
	el::FileSystemSharedPtr file_system;

	file_system = el::FileSystemSharedPtr(new el::FileSystem());
	file_system->add_dir(el::String("."));

	BOOST_CHECK_NO_THROW(material_description_cache.load_xml(file_system,
		el::String("materials.xml")));

	BOOST_CHECK_NO_THROW(
		material_description_cache.get_material_description(
			el::String("ground/grass1")));

	BOOST_CHECK_NO_THROW(
		material_description_cache.get_material_description(
			el::String("tile42")));

	BOOST_CHECK_NO_THROW(
		material_description_cache.get_material_description(
			el::String("giant1")));

	BOOST_CHECK_NO_THROW(
		material_description_cache.get_material_description(
			el::String("target1")));

	BOOST_CHECK_EQUAL(material_description_cache.get_material_description(
		el::String("ground/grass1")).get_texture(el::spt_effect_0),
		"2dobjects/ground/grass1.dds");

	BOOST_CHECK_EQUAL(material_description_cache.get_material_description(
		el::String("tile42")).get_texture(el::spt_effect_0),
		"3dobjects/tile42.dds");

	BOOST_CHECK_EQUAL(material_description_cache.get_material_description(
		el::String("giant1")).get_texture(el::spt_effect_0),
		"meshes/giant1.dds");

	BOOST_CHECK_EQUAL(material_description_cache.get_material_description(
		el::String("target1")).get_texture(el::spt_effect_0),
		"meshes/target1.dds");
}
