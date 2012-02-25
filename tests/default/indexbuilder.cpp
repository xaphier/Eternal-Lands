/****************************************************************************
 *            indexbuilder.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "prerequisites.hpp"
#include "indexbuilder.hpp"
#define BOOST_TEST_MODULE index_builder
#include <boost/test/unit_test.hpp>

namespace el = eternal_lands;

BOOST_AUTO_TEST_CASE(index_builder_no_restart_split)
{
	el::Uint32Vector indices;
	el::Uint32 vertex_count, max_vertex, index_count, tile_size, i;
	std::bitset<65536> vertices;
	bool use_restart_index, split;

	split = true;
	use_restart_index = false;

	for (i = 0; i < 7; i++)
	{
		indices.clear();

		tile_size = 2 << i;

		BOOST_CHECK_LT(tile_size, 256);

		vertex_count = tile_size + 1;
		vertex_count *= tile_size + 1;

		BOOST_CHECK_LT(vertex_count, 65536);
		BOOST_CHECK_EQUAL(vertex_count % 2, 1);

		max_vertex = vertex_count - 1;

		BOOST_CHECK_LT(max_vertex, 65536);

		index_count = tile_size * tile_size * 24 / 4;

		el::IndexBuilder::build_plane_indices(indices, tile_size,
			use_restart_index, 0, split, 0);

		BOOST_FOREACH(const el::Uint32 index, indices)
		{
			BOOST_CHECK_LE(index, max_vertex);

			vertices[index] = true;
		}

		BOOST_CHECK_EQUAL(indices.size(), index_count);
		BOOST_CHECK_EQUAL(vertices.count(), vertex_count);
	}
}

BOOST_AUTO_TEST_CASE(index_builder_no_restart_no_split)
{
	el::Uint32Vector indices;
	el::Uint32 vertex_count, max_vertex, index_count, tile_size, i;
	std::bitset<65536> vertices;
	bool use_restart_index, split;

	split = false;
	use_restart_index = false;

	for (i = 0; i < 7; i++)
	{
		indices.clear();

		tile_size = 2 << i;

		BOOST_CHECK_LT(tile_size, 256);

		vertex_count = tile_size + 1;
		vertex_count *= tile_size + 1;

		BOOST_CHECK_LT(vertex_count, 65536);
		BOOST_CHECK_EQUAL(vertex_count % 2, 1);

		max_vertex = vertex_count * 2 - 1;

		BOOST_CHECK_LT(max_vertex, 65536);

		vertex_count = vertex_count / 2 + 1;

		index_count = tile_size * tile_size * 12 / 4;

		el::IndexBuilder::build_plane_indices(indices, tile_size,
			use_restart_index, 0, split, 0);

		BOOST_FOREACH(const el::Uint32 index, indices)
		{
			BOOST_CHECK_LE(index, max_vertex);

			vertices[index] = true;
		}

		BOOST_CHECK_EQUAL(indices.size(), index_count);
		BOOST_CHECK_EQUAL(vertices.count(), vertex_count);
	}
}

BOOST_AUTO_TEST_CASE(index_builder_restart_split)
{
	el::Uint32Vector indices;
	el::Uint32 vertex_count, index_count, tile_size;
	el::Uint32 restart_index, max_vertex, i;
	std::bitset<65536> vertices;
	bool use_restart_index, split;

	split = true;
	use_restart_index = true;

	for (i = 0; i < 7; i++)
	{
		indices.clear();

		tile_size = 2 << i;

		BOOST_CHECK_LT(tile_size, 256);

		vertex_count = tile_size + 1;
		vertex_count *= tile_size + 1;

		BOOST_CHECK_LT(vertex_count, 65536);
		BOOST_CHECK_EQUAL(vertex_count % 2, 1);

		max_vertex = vertex_count - 1;

		BOOST_CHECK_LT(max_vertex, 65536);

		index_count = tile_size * tile_size * 11 / 4;

		restart_index = el::IndexBuilder::build_plane_indices(
			indices, tile_size, use_restart_index, 0, split, 0);

		BOOST_CHECK_EQUAL(restart_index, 65535);

		BOOST_FOREACH(const el::Uint32 index, indices)
		{
			if (index == restart_index)
			{
				continue;
			}

			BOOST_CHECK_LE(index, max_vertex);

			vertices[index] = true;
		}

		BOOST_CHECK_EQUAL(indices.size(), index_count);
		BOOST_CHECK_EQUAL(vertices.count(), vertex_count);
	}
}

BOOST_AUTO_TEST_CASE(index_builder_restart_no_split)
{
	el::Uint32Vector indices;
	el::Uint32 vertex_count, index_count, tile_size;
	el::Uint32 restart_index, max_vertex, i;
	std::bitset<65536> vertices;
	bool use_restart_index, split;

	split = false;
	use_restart_index = true;

	for (i = 0; i < 7; i++)
	{
		indices.clear();

		tile_size = 2 << i;

		BOOST_CHECK_LT(tile_size, 256);

		vertex_count = tile_size + 1;
		vertex_count *= tile_size + 1;

		BOOST_CHECK_LT(vertex_count, 65536);
		BOOST_CHECK_EQUAL(vertex_count % 2, 1);

		max_vertex = vertex_count - 1;

		BOOST_CHECK_LT(max_vertex, 65536);

		vertex_count = vertex_count / 2 + 1;

		index_count = tile_size * tile_size * 7 / 4;

		restart_index = el::IndexBuilder::build_plane_indices(
			indices, tile_size, use_restart_index, 0, split, 0);

		BOOST_CHECK_EQUAL(restart_index, 65535);

		BOOST_FOREACH(const el::Uint32 index, indices)
		{
			if (index == restart_index)
			{
				continue;
			}

			BOOST_CHECK_LE(index, max_vertex);

			vertices[index] = true;
		}

		BOOST_CHECK_EQUAL(indices.size(), index_count);
		BOOST_CHECK_EQUAL(vertices.count(), vertex_count);
	}
}
