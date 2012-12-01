/****************************************************************************
 *            terrainmaterialdata.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "prerequisites.hpp"
#include "terrain/terrainmaterialdata.hpp"
#include "xmlbuffer.hpp"
#include "xmlwriter.hpp"
#include "xmlreader.hpp"
#include "tools/randomutil.hpp"
#define BOOST_TEST_MODULE terrain_material_data
#include <boost/test/unit_test.hpp>

namespace el = eternal_lands;

BOOST_FIXTURE_TEST_SUITE(random_tool, el::RandomUtil)

BOOST_AUTO_TEST_CASE(default_creation)
{
	el::TerrainMaterialData terrain_material_data;

	BOOST_CHECK_EQUAL(terrain_material_data.get_layer_count(), 0);
	BOOST_CHECK_EQUAL(terrain_material_data.get_use_specular_map(0),
		false);
	BOOST_CHECK_EQUAL(terrain_material_data.get_use_gloss_map(0), false);
	BOOST_CHECK_EQUAL(terrain_material_data.get_use_height_map(0), false);
	BOOST_CHECK_EQUAL(terrain_material_data.get_use_blend_size_texture(0),
		false);
	BOOST_CHECK_EQUAL(terrain_material_data.get_write_height(), true);
	BOOST_CHECK_EQUAL(terrain_material_data.get_write_specular_gloss(),
		true);
}

BOOST_AUTO_TEST_CASE(all)
{
	el::TerrainMaterialData terrain_material_data;
	el::Vec3Vector default_speculars;
	el::FloatVector default_glosses;
	el::FloatVector default_heights;
	el::FloatVector blend_sizes;
	el::BitSet64 use_blend_size_textures;
	el::BitSet64 use_specular_maps;
	el::BitSet64 use_gloss_maps;
	el::BitSet64 use_height_maps;
	glm::vec3 default_specular;
	bool write_height;
	bool write_specular_gloss;
	Uint32 i, j, count;

	for (i = 0; i < 1024; ++i)
	{
		default_speculars.clear();
		default_glosses.clear();
		default_heights.clear();
		blend_sizes.clear();
		use_blend_size_textures.reset();
		use_specular_maps.reset();
		use_gloss_maps.reset();
		use_height_maps.reset();

		count = std::abs(get_random_int()) % 64;

		for (j = 0; j < count; ++j)
		{
			default_specular.r = get_random_float();
			default_specular.g = get_random_float();
			default_specular.b = get_random_float();
			default_speculars.push_back(default_specular);

			default_glosses.push_back(get_random_float());
			default_heights.push_back(get_random_float());
			blend_sizes.push_back(get_random_float());

			use_blend_size_textures[j] = get_random_bool();
			use_specular_maps[j] = get_random_bool();
			use_gloss_maps[j] = get_random_bool();
			use_height_maps[j] = get_random_bool();
		}

		write_height = get_random_bool();
		write_specular_gloss = get_random_bool();

		terrain_material_data.resize(count);

		BOOST_CHECK_EQUAL(terrain_material_data.get_layer_count(),
			count);

		terrain_material_data.set_default_speculars(default_speculars);
		terrain_material_data.set_default_glosses(default_glosses);
		terrain_material_data.set_default_heights(default_heights);
		terrain_material_data.set_blend_sizes(blend_sizes);
		terrain_material_data.set_use_blend_size_textures(
			use_blend_size_textures);
		terrain_material_data.set_use_specular_maps(use_specular_maps);
		terrain_material_data.set_use_gloss_maps(use_gloss_maps);
		terrain_material_data.set_use_height_maps(use_height_maps);
		terrain_material_data.set_write_height(write_height);
		terrain_material_data.set_write_specular_gloss(
			write_specular_gloss);

		for (j = 0; j < count; ++j)
		{
			BOOST_CHECK_CLOSE(
				terrain_material_data.get_default_specular(j).r,
				default_speculars[j].r, 1.0);
			BOOST_CHECK_CLOSE(
				terrain_material_data.get_default_specular(j).g,
				default_speculars[j].g, 1.0);
			BOOST_CHECK_CLOSE(
				terrain_material_data.get_default_specular(j).b,
				default_speculars[j].b, 1.0);
			BOOST_CHECK_CLOSE(
				terrain_material_data.get_default_gloss(j),
				default_glosses[j], 1.0);
			BOOST_CHECK_CLOSE(
				terrain_material_data.get_default_height(j),
				default_heights[j], 1.0);
			BOOST_CHECK_CLOSE(
				terrain_material_data.get_blend_size(j),
				blend_sizes[j], 1.0);
			BOOST_CHECK_EQUAL(terrain_material_data.
					get_use_blend_size_texture(j),
				use_blend_size_textures[j]);
			BOOST_CHECK_EQUAL(
				terrain_material_data.get_use_specular_map(j),
				use_specular_maps[j]);
			BOOST_CHECK_EQUAL(
				terrain_material_data.get_use_gloss_map(j),
				use_gloss_maps[j]);
			BOOST_CHECK_EQUAL(
				terrain_material_data.get_use_height_map(j),
				use_height_maps[j]);
		}

		BOOST_CHECK_EQUAL(terrain_material_data.get_use_blend_size_textures(),
			use_blend_size_textures);
		BOOST_CHECK_EQUAL(terrain_material_data.get_use_specular_maps(),
			use_specular_maps);
		BOOST_CHECK_EQUAL(terrain_material_data.get_use_gloss_maps(),
			use_gloss_maps);
		BOOST_CHECK_EQUAL(terrain_material_data.get_use_height_maps(),
			use_height_maps);
		BOOST_CHECK_EQUAL(terrain_material_data.get_write_height(),
			write_height);
		BOOST_CHECK_EQUAL(
			terrain_material_data.get_write_specular_gloss(),
			write_specular_gloss);
	}
}

BOOST_AUTO_TEST_CASE(xml)
{
	el::XmlBufferSharedPtr buffer;
	el::XmlWriterSharedPtr writer;
	el::XmlReaderSharedPtr reader;
	el::TerrainMaterialData tmp_terrain_material_data;
	el::TerrainMaterialData terrain_material_data;
	el::Vec3Vector default_speculars;
	el::FloatVector default_glosses;
	el::FloatVector default_heights;
	el::FloatVector blend_sizes;
	el::BitSet64 use_blend_size_textures;
	el::BitSet64 use_specular_maps;
	el::BitSet64 use_gloss_maps;
	el::BitSet64 use_height_maps;
	glm::vec3 default_specular;
	bool write_height;
	bool write_specular_gloss;
	Uint32 i, j, count;

	for (i = 0; i < 1024; ++i)
	{
		default_speculars.clear();
		default_glosses.clear();
		default_heights.clear();
		blend_sizes.clear();
		use_blend_size_textures.reset();
		use_specular_maps.reset();
		use_gloss_maps.reset();
		use_height_maps.reset();

		count = std::abs(get_random_int()) % 64;

		for (j = 0; j < count; ++j)
		{
			default_specular.r = get_random_float();
			default_specular.g = get_random_float();
			default_specular.b = get_random_float();
			default_speculars.push_back(default_specular);

			default_glosses.push_back(get_random_float());
			default_heights.push_back(get_random_float());
			blend_sizes.push_back(get_random_float());

			use_blend_size_textures[j] = get_random_bool();
			use_specular_maps[j] = get_random_bool();
			use_gloss_maps[j] = get_random_bool();
			use_height_maps[j] = get_random_bool();
		}

		write_height = get_random_bool();
		write_specular_gloss = get_random_bool();

		tmp_terrain_material_data.resize(count);

		tmp_terrain_material_data.set_default_speculars(default_speculars);
		tmp_terrain_material_data.set_default_glosses(default_glosses);
		tmp_terrain_material_data.set_default_heights(default_heights);
		tmp_terrain_material_data.set_blend_sizes(blend_sizes);
		tmp_terrain_material_data.set_use_blend_size_textures(
			use_blend_size_textures);
		tmp_terrain_material_data.set_use_specular_maps(use_specular_maps);
		tmp_terrain_material_data.set_use_gloss_maps(use_gloss_maps);
		tmp_terrain_material_data.set_use_height_maps(use_height_maps);
		tmp_terrain_material_data.set_write_height(write_height);
		tmp_terrain_material_data.set_write_specular_gloss(
			write_specular_gloss);

		buffer = boost::make_shared<el::XmlBuffer>();
		writer = boost::make_shared<el::XmlWriter>(buffer);

		BOOST_CHECK_NO_THROW(tmp_terrain_material_data.save_xml(
			writer));

		writer.reset();

		reader = boost::make_shared<el::XmlReader>(buffer);

		BOOST_CHECK_NO_THROW(terrain_material_data.load_xml(
			reader->get_root_node()));

		reader.reset();

		BOOST_CHECK_EQUAL(terrain_material_data.get_layer_count(),
			count);

		for (j = 0; j < count; ++j)
		{
			BOOST_CHECK_CLOSE(
				terrain_material_data.get_default_specular(j).r,
				default_speculars[j].r, 1.0);
			BOOST_CHECK_CLOSE(
				terrain_material_data.get_default_specular(j).g,
				default_speculars[j].g, 1.0);
			BOOST_CHECK_CLOSE(
				terrain_material_data.get_default_specular(j).b,
				default_speculars[j].b, 1.0);
			BOOST_CHECK_CLOSE(
				terrain_material_data.get_default_gloss(j),
				default_glosses[j], 1.0);
			BOOST_CHECK_CLOSE(
				terrain_material_data.get_default_height(j),
				default_heights[j], 1.0);
			BOOST_CHECK_CLOSE(
				terrain_material_data.get_blend_size(j),
				blend_sizes[j], 1.0);
			BOOST_CHECK_EQUAL(terrain_material_data.
					get_use_blend_size_texture(j),
				use_blend_size_textures[j]);
			BOOST_CHECK_EQUAL(
				terrain_material_data.get_use_specular_map(j),
				use_specular_maps[j]);
			BOOST_CHECK_EQUAL(
				terrain_material_data.get_use_gloss_map(j),
				use_gloss_maps[j]);
			BOOST_CHECK_EQUAL(
				terrain_material_data.get_use_height_map(j),
				use_height_maps[j]);
		}

		BOOST_CHECK_EQUAL(terrain_material_data.get_use_blend_size_textures(),
			use_blend_size_textures);
		BOOST_CHECK_EQUAL(terrain_material_data.get_use_specular_maps(),
			use_specular_maps);
		BOOST_CHECK_EQUAL(terrain_material_data.get_use_gloss_maps(),
			use_gloss_maps);
		BOOST_CHECK_EQUAL(terrain_material_data.get_use_height_maps(),
			use_height_maps);
		BOOST_CHECK_EQUAL(terrain_material_data.get_write_height(),
			write_height);
		BOOST_CHECK_EQUAL(
			terrain_material_data.get_write_specular_gloss(),
			write_specular_gloss);
	}
}

BOOST_AUTO_TEST_SUITE_END()
