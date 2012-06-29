/****************************************************************************
 *            clipmapdata.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "prerequisites.hpp"
#include "clipmapdata.hpp"
#include "tools/randomutil.hpp"
#define BOOST_TEST_MODULE clipmap_data
#include <boost/test/unit_test.hpp>

namespace el = eternal_lands;

BOOST_FIXTURE_TEST_SUITE(random_tool, el::RandomUtil)

BOOST_AUTO_TEST_CASE(default_creation)
{
	el::ClipmapData clipmap_data;

	BOOST_CHECK_EQUAL(clipmap_data.get_albedo_map(0), "");
	BOOST_CHECK_EQUAL(clipmap_data.get_albedo_map(1), "");
	BOOST_CHECK_EQUAL(clipmap_data.get_albedo_map(2), "");
	BOOST_CHECK_EQUAL(clipmap_data.get_albedo_map(3), "");
	BOOST_CHECK_EQUAL(clipmap_data.get_albedo_map(4), "");
	BOOST_CHECK_EQUAL(clipmap_data.get_albedo_map(5), "");
	BOOST_CHECK_EQUAL(clipmap_data.get_albedo_map(6), "");
	BOOST_CHECK_EQUAL(clipmap_data.get_albedo_map(7), "");
	BOOST_CHECK_EQUAL(clipmap_data.get_albedo_map(8), "");
	BOOST_CHECK_EQUAL(clipmap_data.get_albedo_map(9), "");
	BOOST_CHECK_EQUAL(clipmap_data.get_albedo_map(10), "");
	BOOST_CHECK_EQUAL(clipmap_data.get_albedo_map(11), "");
	BOOST_CHECK_EQUAL(clipmap_data.get_albedo_map(12), "");
	BOOST_CHECK_EQUAL(clipmap_data.get_albedo_map(13), "");
	BOOST_CHECK_EQUAL(clipmap_data.get_albedo_map(14), "");
	BOOST_CHECK_EQUAL(clipmap_data.get_albedo_map(15), "");

	BOOST_CHECK_EQUAL(clipmap_data.get_albedo_maps()[0], "");
	BOOST_CHECK_EQUAL(clipmap_data.get_albedo_maps()[1], "");
	BOOST_CHECK_EQUAL(clipmap_data.get_albedo_maps()[2], "");
	BOOST_CHECK_EQUAL(clipmap_data.get_albedo_maps()[3], "");
	BOOST_CHECK_EQUAL(clipmap_data.get_albedo_maps()[4], "");
	BOOST_CHECK_EQUAL(clipmap_data.get_albedo_maps()[5], "");
	BOOST_CHECK_EQUAL(clipmap_data.get_albedo_maps()[6], "");
	BOOST_CHECK_EQUAL(clipmap_data.get_albedo_maps()[7], "");
	BOOST_CHECK_EQUAL(clipmap_data.get_albedo_maps()[8], "");
	BOOST_CHECK_EQUAL(clipmap_data.get_albedo_maps()[9], "");
	BOOST_CHECK_EQUAL(clipmap_data.get_albedo_maps()[10], "");
	BOOST_CHECK_EQUAL(clipmap_data.get_albedo_maps()[11], "");
	BOOST_CHECK_EQUAL(clipmap_data.get_albedo_maps()[12], "");
	BOOST_CHECK_EQUAL(clipmap_data.get_albedo_maps()[13], "");
	BOOST_CHECK_EQUAL(clipmap_data.get_albedo_maps()[14], "");
	BOOST_CHECK_EQUAL(clipmap_data.get_albedo_maps()[15], "");

	BOOST_CHECK_EQUAL(clipmap_data.get_normal_map(0), "");
	BOOST_CHECK_EQUAL(clipmap_data.get_normal_map(1), "");
	BOOST_CHECK_EQUAL(clipmap_data.get_normal_map(2), "");
	BOOST_CHECK_EQUAL(clipmap_data.get_normal_map(3), "");
	BOOST_CHECK_EQUAL(clipmap_data.get_normal_map(4), "");
	BOOST_CHECK_EQUAL(clipmap_data.get_normal_map(5), "");
	BOOST_CHECK_EQUAL(clipmap_data.get_normal_map(6), "");
	BOOST_CHECK_EQUAL(clipmap_data.get_normal_map(7), "");
	BOOST_CHECK_EQUAL(clipmap_data.get_normal_map(8), "");
	BOOST_CHECK_EQUAL(clipmap_data.get_normal_map(9), "");
	BOOST_CHECK_EQUAL(clipmap_data.get_normal_map(10), "");
	BOOST_CHECK_EQUAL(clipmap_data.get_normal_map(11), "");
	BOOST_CHECK_EQUAL(clipmap_data.get_normal_map(12), "");
	BOOST_CHECK_EQUAL(clipmap_data.get_normal_map(13), "");
	BOOST_CHECK_EQUAL(clipmap_data.get_normal_map(14), "");
	BOOST_CHECK_EQUAL(clipmap_data.get_normal_map(15), "");

	BOOST_CHECK_EQUAL(clipmap_data.get_normal_maps()[0], "");
	BOOST_CHECK_EQUAL(clipmap_data.get_normal_maps()[1], "");
	BOOST_CHECK_EQUAL(clipmap_data.get_normal_maps()[2], "");
	BOOST_CHECK_EQUAL(clipmap_data.get_normal_maps()[3], "");
	BOOST_CHECK_EQUAL(clipmap_data.get_normal_maps()[4], "");
	BOOST_CHECK_EQUAL(clipmap_data.get_normal_maps()[5], "");
	BOOST_CHECK_EQUAL(clipmap_data.get_normal_maps()[6], "");
	BOOST_CHECK_EQUAL(clipmap_data.get_normal_maps()[7], "");
	BOOST_CHECK_EQUAL(clipmap_data.get_normal_maps()[8], "");
	BOOST_CHECK_EQUAL(clipmap_data.get_normal_maps()[9], "");
	BOOST_CHECK_EQUAL(clipmap_data.get_normal_maps()[10], "");
	BOOST_CHECK_EQUAL(clipmap_data.get_normal_maps()[11], "");
	BOOST_CHECK_EQUAL(clipmap_data.get_normal_maps()[12], "");
	BOOST_CHECK_EQUAL(clipmap_data.get_normal_maps()[13], "");
	BOOST_CHECK_EQUAL(clipmap_data.get_normal_maps()[14], "");
	BOOST_CHECK_EQUAL(clipmap_data.get_normal_maps()[15], "");

	BOOST_CHECK_EQUAL(clipmap_data.get_specular_map(0), "");
	BOOST_CHECK_EQUAL(clipmap_data.get_specular_map(1), "");
	BOOST_CHECK_EQUAL(clipmap_data.get_specular_map(2), "");
	BOOST_CHECK_EQUAL(clipmap_data.get_specular_map(3), "");
	BOOST_CHECK_EQUAL(clipmap_data.get_specular_map(4), "");
	BOOST_CHECK_EQUAL(clipmap_data.get_specular_map(5), "");
	BOOST_CHECK_EQUAL(clipmap_data.get_specular_map(6), "");
	BOOST_CHECK_EQUAL(clipmap_data.get_specular_map(7), "");
	BOOST_CHECK_EQUAL(clipmap_data.get_specular_map(8), "");
	BOOST_CHECK_EQUAL(clipmap_data.get_specular_map(9), "");
	BOOST_CHECK_EQUAL(clipmap_data.get_specular_map(10), "");
	BOOST_CHECK_EQUAL(clipmap_data.get_specular_map(11), "");
	BOOST_CHECK_EQUAL(clipmap_data.get_specular_map(12), "");
	BOOST_CHECK_EQUAL(clipmap_data.get_specular_map(13), "");
	BOOST_CHECK_EQUAL(clipmap_data.get_specular_map(14), "");
	BOOST_CHECK_EQUAL(clipmap_data.get_specular_map(15), "");

	BOOST_CHECK_EQUAL(clipmap_data.get_specular_maps()[0], "");
	BOOST_CHECK_EQUAL(clipmap_data.get_specular_maps()[1], "");
	BOOST_CHECK_EQUAL(clipmap_data.get_specular_maps()[2], "");
	BOOST_CHECK_EQUAL(clipmap_data.get_specular_maps()[3], "");
	BOOST_CHECK_EQUAL(clipmap_data.get_specular_maps()[4], "");
	BOOST_CHECK_EQUAL(clipmap_data.get_specular_maps()[5], "");
	BOOST_CHECK_EQUAL(clipmap_data.get_specular_maps()[6], "");
	BOOST_CHECK_EQUAL(clipmap_data.get_specular_maps()[7], "");
	BOOST_CHECK_EQUAL(clipmap_data.get_specular_maps()[8], "");
	BOOST_CHECK_EQUAL(clipmap_data.get_specular_maps()[9], "");
	BOOST_CHECK_EQUAL(clipmap_data.get_specular_maps()[10], "");
	BOOST_CHECK_EQUAL(clipmap_data.get_specular_maps()[11], "");
	BOOST_CHECK_EQUAL(clipmap_data.get_specular_maps()[12], "");
	BOOST_CHECK_EQUAL(clipmap_data.get_specular_maps()[13], "");
	BOOST_CHECK_EQUAL(clipmap_data.get_specular_maps()[14], "");
	BOOST_CHECK_EQUAL(clipmap_data.get_specular_maps()[15], "");
}

BOOST_AUTO_TEST_CASE(all)
{
	el::ClipmapData clipmap_data;
	el::StringArray16 albedo_maps, normal_maps, specular_maps;
	el::Vec4Array4 blend_sizes;
	glm::vec4 color;

	color[0] = get_random_float();
	color[1] = get_random_float();
	color[2] = get_random_float();
	color[3] = get_random_float();

	BOOST_FOREACH(glm::vec4 &value, blend_sizes)
	{
		value[0] = get_random_float();
		value[1] = get_random_float();
		value[2] = get_random_float();
		value[3] = get_random_float();
	}

	BOOST_FOREACH(el::String &name, albedo_maps)
	{
		name = get_random_name();
	}

	BOOST_FOREACH(el::String &name, normal_maps)
	{
		name = get_random_name();
	}

	BOOST_FOREACH(el::String &name, specular_maps)
	{
		name = get_random_name();
	}

	BOOST_CHECK_NO_THROW(clipmap_data.set_albedo_maps(albedo_maps));
	BOOST_CHECK_NO_THROW(clipmap_data.set_normal_maps(normal_maps));
	BOOST_CHECK_NO_THROW(clipmap_data.set_specular_maps(specular_maps));
	BOOST_CHECK_NO_THROW(clipmap_data.set_blend_sizes(blend_sizes));
	BOOST_CHECK_NO_THROW(clipmap_data.set_color(color));

	BOOST_CHECK_EQUAL(clipmap_data.get_albedo_map(0), albedo_maps[0]);
	BOOST_CHECK_EQUAL(clipmap_data.get_albedo_map(1), albedo_maps[1]);
	BOOST_CHECK_EQUAL(clipmap_data.get_albedo_map(2), albedo_maps[2]);
	BOOST_CHECK_EQUAL(clipmap_data.get_albedo_map(3), albedo_maps[3]);
	BOOST_CHECK_EQUAL(clipmap_data.get_albedo_map(4), albedo_maps[4]);
	BOOST_CHECK_EQUAL(clipmap_data.get_albedo_map(5), albedo_maps[5]);
	BOOST_CHECK_EQUAL(clipmap_data.get_albedo_map(6), albedo_maps[6]);
	BOOST_CHECK_EQUAL(clipmap_data.get_albedo_map(7), albedo_maps[7]);
	BOOST_CHECK_EQUAL(clipmap_data.get_albedo_map(8), albedo_maps[8]);
	BOOST_CHECK_EQUAL(clipmap_data.get_albedo_map(9), albedo_maps[9]);
	BOOST_CHECK_EQUAL(clipmap_data.get_albedo_map(10), albedo_maps[10]);
	BOOST_CHECK_EQUAL(clipmap_data.get_albedo_map(11), albedo_maps[11]);
	BOOST_CHECK_EQUAL(clipmap_data.get_albedo_map(12), albedo_maps[12]);
	BOOST_CHECK_EQUAL(clipmap_data.get_albedo_map(13), albedo_maps[13]);
	BOOST_CHECK_EQUAL(clipmap_data.get_albedo_map(14), albedo_maps[14]);
	BOOST_CHECK_EQUAL(clipmap_data.get_albedo_map(15), albedo_maps[15]);

	BOOST_CHECK_EQUAL(clipmap_data.get_albedo_maps()[0], albedo_maps[0]);
	BOOST_CHECK_EQUAL(clipmap_data.get_albedo_maps()[1], albedo_maps[1]);
	BOOST_CHECK_EQUAL(clipmap_data.get_albedo_maps()[2], albedo_maps[2]);
	BOOST_CHECK_EQUAL(clipmap_data.get_albedo_maps()[3], albedo_maps[3]);
	BOOST_CHECK_EQUAL(clipmap_data.get_albedo_maps()[4], albedo_maps[4]);
	BOOST_CHECK_EQUAL(clipmap_data.get_albedo_maps()[5], albedo_maps[5]);
	BOOST_CHECK_EQUAL(clipmap_data.get_albedo_maps()[6], albedo_maps[6]);
	BOOST_CHECK_EQUAL(clipmap_data.get_albedo_maps()[7], albedo_maps[7]);
	BOOST_CHECK_EQUAL(clipmap_data.get_albedo_maps()[8], albedo_maps[8]);
	BOOST_CHECK_EQUAL(clipmap_data.get_albedo_maps()[9], albedo_maps[9]);
	BOOST_CHECK_EQUAL(clipmap_data.get_albedo_maps()[10], albedo_maps[10]);
	BOOST_CHECK_EQUAL(clipmap_data.get_albedo_maps()[11], albedo_maps[11]);
	BOOST_CHECK_EQUAL(clipmap_data.get_albedo_maps()[12], albedo_maps[12]);
	BOOST_CHECK_EQUAL(clipmap_data.get_albedo_maps()[13], albedo_maps[13]);
	BOOST_CHECK_EQUAL(clipmap_data.get_albedo_maps()[14], albedo_maps[14]);
	BOOST_CHECK_EQUAL(clipmap_data.get_albedo_maps()[15], albedo_maps[15]);

	BOOST_CHECK_EQUAL(clipmap_data.get_normal_map(0), normal_maps[0]);
	BOOST_CHECK_EQUAL(clipmap_data.get_normal_map(1), normal_maps[1]);
	BOOST_CHECK_EQUAL(clipmap_data.get_normal_map(2), normal_maps[2]);
	BOOST_CHECK_EQUAL(clipmap_data.get_normal_map(3), normal_maps[3]);
	BOOST_CHECK_EQUAL(clipmap_data.get_normal_map(4), normal_maps[4]);
	BOOST_CHECK_EQUAL(clipmap_data.get_normal_map(5), normal_maps[5]);
	BOOST_CHECK_EQUAL(clipmap_data.get_normal_map(6), normal_maps[6]);
	BOOST_CHECK_EQUAL(clipmap_data.get_normal_map(7), normal_maps[7]);
	BOOST_CHECK_EQUAL(clipmap_data.get_normal_map(8), normal_maps[8]);
	BOOST_CHECK_EQUAL(clipmap_data.get_normal_map(9), normal_maps[9]);
	BOOST_CHECK_EQUAL(clipmap_data.get_normal_map(10), normal_maps[10]);
	BOOST_CHECK_EQUAL(clipmap_data.get_normal_map(11), normal_maps[11]);
	BOOST_CHECK_EQUAL(clipmap_data.get_normal_map(12), normal_maps[12]);
	BOOST_CHECK_EQUAL(clipmap_data.get_normal_map(13), normal_maps[13]);
	BOOST_CHECK_EQUAL(clipmap_data.get_normal_map(14), normal_maps[14]);
	BOOST_CHECK_EQUAL(clipmap_data.get_normal_map(15), normal_maps[15]);

	BOOST_CHECK_EQUAL(clipmap_data.get_normal_maps()[0], normal_maps[0]);
	BOOST_CHECK_EQUAL(clipmap_data.get_normal_maps()[1], normal_maps[1]);
	BOOST_CHECK_EQUAL(clipmap_data.get_normal_maps()[2], normal_maps[2]);
	BOOST_CHECK_EQUAL(clipmap_data.get_normal_maps()[3], normal_maps[3]);
	BOOST_CHECK_EQUAL(clipmap_data.get_normal_maps()[4], normal_maps[4]);
	BOOST_CHECK_EQUAL(clipmap_data.get_normal_maps()[5], normal_maps[5]);
	BOOST_CHECK_EQUAL(clipmap_data.get_normal_maps()[6], normal_maps[6]);
	BOOST_CHECK_EQUAL(clipmap_data.get_normal_maps()[7], normal_maps[7]);
	BOOST_CHECK_EQUAL(clipmap_data.get_normal_maps()[8], normal_maps[8]);
	BOOST_CHECK_EQUAL(clipmap_data.get_normal_maps()[9], normal_maps[9]);
	BOOST_CHECK_EQUAL(clipmap_data.get_normal_maps()[10], normal_maps[10]);
	BOOST_CHECK_EQUAL(clipmap_data.get_normal_maps()[11], normal_maps[11]);
	BOOST_CHECK_EQUAL(clipmap_data.get_normal_maps()[12], normal_maps[12]);
	BOOST_CHECK_EQUAL(clipmap_data.get_normal_maps()[13], normal_maps[13]);
	BOOST_CHECK_EQUAL(clipmap_data.get_normal_maps()[14], normal_maps[14]);
	BOOST_CHECK_EQUAL(clipmap_data.get_normal_maps()[15], normal_maps[15]);

	BOOST_CHECK_EQUAL(clipmap_data.get_specular_map(0), specular_maps[0]);
	BOOST_CHECK_EQUAL(clipmap_data.get_specular_map(1), specular_maps[1]);
	BOOST_CHECK_EQUAL(clipmap_data.get_specular_map(2), specular_maps[2]);
	BOOST_CHECK_EQUAL(clipmap_data.get_specular_map(3), specular_maps[3]);
	BOOST_CHECK_EQUAL(clipmap_data.get_specular_map(4), specular_maps[4]);
	BOOST_CHECK_EQUAL(clipmap_data.get_specular_map(5), specular_maps[5]);
	BOOST_CHECK_EQUAL(clipmap_data.get_specular_map(6), specular_maps[6]);
	BOOST_CHECK_EQUAL(clipmap_data.get_specular_map(7), specular_maps[7]);
	BOOST_CHECK_EQUAL(clipmap_data.get_specular_map(8), specular_maps[8]);
	BOOST_CHECK_EQUAL(clipmap_data.get_specular_map(9), specular_maps[9]);
	BOOST_CHECK_EQUAL(clipmap_data.get_specular_map(10), specular_maps[10]);
	BOOST_CHECK_EQUAL(clipmap_data.get_specular_map(11), specular_maps[11]);
	BOOST_CHECK_EQUAL(clipmap_data.get_specular_map(12), specular_maps[12]);
	BOOST_CHECK_EQUAL(clipmap_data.get_specular_map(13), specular_maps[13]);
	BOOST_CHECK_EQUAL(clipmap_data.get_specular_map(14), specular_maps[14]);
	BOOST_CHECK_EQUAL(clipmap_data.get_specular_map(15), specular_maps[15]);

	BOOST_CHECK_EQUAL(clipmap_data.get_specular_maps()[0],
		specular_maps[0]);
	BOOST_CHECK_EQUAL(clipmap_data.get_specular_maps()[1],
		specular_maps[1]);
	BOOST_CHECK_EQUAL(clipmap_data.get_specular_maps()[2],
		specular_maps[2]);
	BOOST_CHECK_EQUAL(clipmap_data.get_specular_maps()[3],
		specular_maps[3]);
	BOOST_CHECK_EQUAL(clipmap_data.get_specular_maps()[4],
		specular_maps[4]);
	BOOST_CHECK_EQUAL(clipmap_data.get_specular_maps()[5],
		specular_maps[5]);
	BOOST_CHECK_EQUAL(clipmap_data.get_specular_maps()[6],
		specular_maps[6]);
	BOOST_CHECK_EQUAL(clipmap_data.get_specular_maps()[7],
		specular_maps[7]);
	BOOST_CHECK_EQUAL(clipmap_data.get_specular_maps()[8],
		specular_maps[8]);
	BOOST_CHECK_EQUAL(clipmap_data.get_specular_maps()[9],
		specular_maps[9]);
	BOOST_CHECK_EQUAL(clipmap_data.get_specular_maps()[10],
		specular_maps[10]);
	BOOST_CHECK_EQUAL(clipmap_data.get_specular_maps()[11],
		specular_maps[11]);
	BOOST_CHECK_EQUAL(clipmap_data.get_specular_maps()[12],
		specular_maps[12]);
	BOOST_CHECK_EQUAL(clipmap_data.get_specular_maps()[13],
		specular_maps[13]);
	BOOST_CHECK_EQUAL(clipmap_data.get_specular_maps()[14],
		specular_maps[14]);
	BOOST_CHECK_EQUAL(clipmap_data.get_specular_maps()[15],
		specular_maps[15]);

	EL_CHECK_EQUAL_VEC_MAT(clipmap_data.get_blend_size(0), blend_sizes[0]);
	EL_CHECK_EQUAL_VEC_MAT(clipmap_data.get_blend_size(1), blend_sizes[1]);
	EL_CHECK_EQUAL_VEC_MAT(clipmap_data.get_blend_size(2), blend_sizes[2]);
	EL_CHECK_EQUAL_VEC_MAT(clipmap_data.get_blend_size(3), blend_sizes[3]);

	EL_CHECK_EQUAL_VEC_MAT(clipmap_data.get_color(), color);
}

BOOST_AUTO_TEST_SUITE_END()
