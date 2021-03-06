/****************************************************************************
 *            indexbuilder.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "prerequisites.hpp"
#include "indexbuilder.hpp"
#define BOOST_TEST_MODULE index_builder
#include <boost/test/unit_test.hpp>

namespace el = eternal_lands;

namespace
{

	class Triangle
	{
		private:
			glm::uvec3 m_indices;

		public:
			Triangle();
			Triangle(const Uint32 idx0, const Uint32 idx1,
				const Uint32 idx2);
			bool operator==(const Triangle &triangle) const;
			bool operator<(const Triangle &triangle) const;

			inline const glm::uvec3 &get_indices() const
			{
				return m_indices;
			}

	};

	Triangle::Triangle()
	{
		m_indices = glm::uvec3(std::numeric_limits<Uint32>::max());
	}

	Triangle::Triangle(const Uint32 idx0, const Uint32 idx1,
		const Uint32 idx2)
	{
		m_indices = glm::uvec3(idx0, idx1, idx2);

		if (m_indices.y < m_indices.x)
		{
			std::swap(m_indices.x, m_indices.y);
		}

		if (m_indices.z < m_indices.x)
		{
			std::swap(m_indices.x, m_indices.z);
		}

		if (m_indices.z < m_indices.y)
		{
			std::swap(m_indices.y, m_indices.z);
		}

		assert(m_indices.x <= m_indices.y);
		assert(m_indices.x <= m_indices.z);
		assert(m_indices.y <= m_indices.z);
	}

	bool Triangle::operator==(const Triangle &triangle) const
	{
		return (m_indices.x == triangle.m_indices.x) &&
			(m_indices.y == triangle.m_indices.y) &&
			(m_indices.z == triangle.m_indices.z);
	}

	bool Triangle::operator<(const Triangle &triangle) const
	{
		if (m_indices.x != triangle.m_indices.x)
		{
			return m_indices.x < triangle.m_indices.x;
		}

		if (m_indices.y != triangle.m_indices.y)
		{
			return m_indices.y < triangle.m_indices.y;
		}

		return m_indices.z < triangle.m_indices.z;
	}

	bool check_front_facing(const glm::vec3 &normal,
		const glm::uvec2 &grid_size, const Uint32 idx0,
		const Uint32 idx1, const Uint32 idx2)
	{
		glm::vec3 p0, p1, p2, d0, d1, d2;

		p0.x = idx0 % grid_size.x;
		p0.y = idx0 / grid_size.x;
		p0.z = 0.0f;

		p1.x = idx1 % grid_size.x;
		p1.y = idx1 / grid_size.x;
		p1.z = 0.0f;

		p2.x = idx2 % grid_size.x;
		p2.y = idx2 / grid_size.x;
		p2.z = 0.0f;

		d0 = p1 - p0;
		d1 = p2 - p0;

		if ((glm::dot(d0, d0) < el::epsilon) ||
			(glm::dot(d1, d1) < el::epsilon))
		{
			return false;
		}

		d0 = glm::normalize(d0);
		d1 = glm::normalize(d1);

		d2 = glm::cross(d0, d1);

		return glm::dot(normal, d2) > 0.0f;
	}

}

BOOST_AUTO_TEST_CASE(plane_no_restart_split)
{
	el::Uint32Vector indices;
	Uint32 vertex_count, max_vertex, index_count, tile_size, i, j;
	std::set<Triangle> triangles;
	Triangle triangle;
	std::bitset<65536> vertices;
	bool use_restart_index, split;

	split = true;
	use_restart_index = false;

	for (i = 0; i < 7; i++)
	{
		indices.clear();
		triangles.clear();

		tile_size = 2 << i;

		BOOST_CHECK_LT(tile_size, 256);

		vertex_count = tile_size + 1;
		vertex_count *= tile_size + 1;

		BOOST_CHECK_LE(vertex_count,
			std::numeric_limits<Uint16>::max());
		BOOST_CHECK_EQUAL(vertex_count % 2, 1);

		max_vertex = vertex_count - 1;

		BOOST_CHECK_LT(max_vertex, std::numeric_limits<Uint16>::max());

		index_count = tile_size * tile_size * 24 / 4;

		el::IndexBuilder::build_plane_indices(tile_size,
			use_restart_index, 0, split, indices);

		BOOST_FOREACH(const Uint32 index, indices)
		{
			BOOST_CHECK_LE(index, max_vertex);

			vertices[index] = true;
		}

		BOOST_CHECK_EQUAL(indices.size(), index_count);
		BOOST_CHECK_EQUAL(vertices.count(), vertex_count);

		for (j = 0; j < (indices.size() / 3); ++j)
		{
			BOOST_CHECK(check_front_facing(
				glm::vec3(0.0f, 0.0f, 1.0f),
				glm::uvec2(tile_size + 1),
				indices[j * 3 + 0],
				indices[j * 3 + 1], indices[j * 3 + 2]));

			triangle = Triangle(indices[j * 3 + 0],
				indices[j * 3 + 1], indices[j * 3 + 2]);

			BOOST_CHECK_EQUAL(triangles.count(triangle), 0);

			triangles.insert(triangle);
		}
	}
}

BOOST_AUTO_TEST_CASE(plane_no_restart_no_split)
{
	el::Uint32Vector indices;
	Uint32 vertex_count, max_vertex, index_count, tile_size, i, j;
	std::set<Triangle> triangles;
	Triangle triangle;
	std::bitset<65536> vertices;
	bool use_restart_index, split;

	split = false;
	use_restart_index = false;

	for (i = 0; i < 7; i++)
	{
		indices.clear();
		triangles.clear();

		tile_size = 2 << i;

		BOOST_CHECK_LT(tile_size, 256);

		vertex_count = tile_size + 1;
		vertex_count *= tile_size + 1;

		BOOST_CHECK_LE(vertex_count,
			std::numeric_limits<Uint16>::max());
		BOOST_CHECK_EQUAL(vertex_count % 2, 1);

		max_vertex = vertex_count * 2 - 1;

		BOOST_CHECK_LT(max_vertex, std::numeric_limits<Uint16>::max());

		vertex_count = vertex_count / 2 + 1;

		index_count = tile_size * tile_size * 12 / 4;

		el::IndexBuilder::build_plane_indices(tile_size,
			use_restart_index, 0, split, indices);

		BOOST_FOREACH(const Uint32 index, indices)
		{
			BOOST_CHECK_LE(index, max_vertex);

			vertices[index] = true;
		}

		BOOST_CHECK_EQUAL(indices.size(), index_count);
		BOOST_CHECK_EQUAL(vertices.count(), vertex_count);

		for (j = 0; j < (indices.size() / 3); ++j)
		{
			BOOST_CHECK(check_front_facing(
				glm::vec3(0.0f, 0.0f, 1.0f),
				glm::uvec2(tile_size + 1),
				indices[j * 3 + 0],
				indices[j * 3 + 1], indices[j * 3 + 2]));

			triangle = Triangle(indices[j * 3 + 0],
				indices[j * 3 + 1], indices[j * 3 + 2]);

			BOOST_CHECK_EQUAL(triangles.count(triangle), 0);

			triangles.insert(triangle);
		}
	}
}

BOOST_AUTO_TEST_CASE(plane_restart_split)
{
	el::Uint32Vector indices, restart_indices;
	std::bitset<65536> vertices;
	std::set<Triangle> triangles, restart_triangles;
	Triangle triangle;
	Uint32 vertex_count, index_count, tile_size, max_vertex, i, j;
	Uint32 center, last;
	bool split;

	split = true;

	for (i = 0; i < 7; i++)
	{
		indices.clear();
		triangles.clear();
		restart_indices.clear();
		restart_triangles.clear();

		tile_size = 2 << i;

		BOOST_CHECK_LT(tile_size, 256);

		vertex_count = tile_size + 1;
		vertex_count *= tile_size + 1;

		BOOST_CHECK_LE(vertex_count,
			std::numeric_limits<Uint16>::max());
		BOOST_CHECK_EQUAL(vertex_count % 2, 1);

		max_vertex = vertex_count - 1;

		BOOST_CHECK_LE(max_vertex, std::numeric_limits<Uint16>::max());

		index_count = tile_size * tile_size * 11 / 4;

		el::IndexBuilder::build_plane_indices(tile_size, true, 0, split,
			restart_indices);

		BOOST_CHECK_EQUAL(restart_indices.size(), index_count);

		center = std::numeric_limits<Uint32>::max();
		last = std::numeric_limits<Uint32>::max();

		BOOST_FOREACH(const Uint32 index, restart_indices)
		{
			if (index == std::numeric_limits<Uint32>::max())
			{
				center = std::numeric_limits<Uint32>::max();
				last = std::numeric_limits<Uint32>::max();
				continue;
			}

			BOOST_CHECK_LE(index, max_vertex);

			vertices[index] = true;

			if (center == std::numeric_limits<Uint32>::max())
			{
				center = index;
				continue;
			}

			if (last == std::numeric_limits<Uint32>::max())
			{
				last = index;
				continue;
			}

			BOOST_CHECK(check_front_facing(
				glm::vec3(0.0f, 0.0f, 1.0f),
				glm::uvec2(tile_size + 1),
				center, last, index));

			triangle = Triangle(center, last, index);

			last = index;

			BOOST_CHECK_EQUAL(restart_triangles.count(triangle), 0);

			restart_triangles.insert(triangle);
		}

		BOOST_CHECK_EQUAL(vertices.count(), vertex_count);

		el::IndexBuilder::build_plane_indices(tile_size, false, 0,
			split, indices);

		for (j = 0; j < (indices.size() / 3); ++j)
		{
			BOOST_CHECK(check_front_facing(
				glm::vec3(0.0f, 0.0f, 1.0f),
				glm::uvec2(tile_size + 1),
				indices[j * 3 + 0],
				indices[j * 3 + 1], indices[j * 3 + 2]));

			triangle = Triangle(indices[j * 3 + 0],
				indices[j * 3 + 1], indices[j * 3 + 2]);

			BOOST_CHECK_EQUAL(triangles.count(triangle), 0);

			triangles.insert(triangle);
		}

		BOOST_CHECK_EQUAL(restart_triangles.size(), triangles.size());

		BOOST_FOREACH(const Triangle &triangle, triangles)
		{
			BOOST_CHECK_EQUAL(restart_triangles.count(triangle), 1);
		}

		BOOST_FOREACH(const Triangle &triangle, restart_triangles)
		{
			BOOST_CHECK_EQUAL(triangles.count(triangle), 1);
		}
	}
}

BOOST_AUTO_TEST_CASE(plane_restart_no_split)
{
	el::Uint32Vector indices, restart_indices;
	std::bitset<65536> vertices;
	std::set<Triangle> triangles, restart_triangles;
	Triangle triangle;
	Uint32 vertex_count, index_count, tile_size, max_vertex, i, j;
	Uint32 center, last;
	bool split;

	split = false;

	for (i = 0; i < 7; i++)
	{
		indices.clear();
		triangles.clear();
		restart_indices.clear();
		restart_triangles.clear();

		tile_size = 2 << i;

		BOOST_CHECK_LT(tile_size, 256);

		vertex_count = tile_size + 1;
		vertex_count *= tile_size + 1;

		BOOST_CHECK_LE(vertex_count,
			std::numeric_limits<Uint16>::max());
		BOOST_CHECK_EQUAL(vertex_count % 2, 1);

		max_vertex = vertex_count - 1;

		BOOST_CHECK_LE(max_vertex, std::numeric_limits<Uint16>::max());

		vertex_count = vertex_count / 2 + 1;

		index_count = tile_size * tile_size * 7 / 4;

		el::IndexBuilder::build_plane_indices(tile_size, true, 0, split,
			restart_indices);

		BOOST_CHECK_EQUAL(restart_indices.size(), index_count);

		center = std::numeric_limits<Uint32>::max();
		last = std::numeric_limits<Uint32>::max();

		BOOST_FOREACH(const Uint32 index, restart_indices)
		{
			if (index == std::numeric_limits<Uint32>::max())
			{
				center = std::numeric_limits<Uint32>::max();
				last = std::numeric_limits<Uint32>::max();
				continue;
			}

			BOOST_CHECK_LE(index, max_vertex);

			vertices[index] = true;

			if (center == std::numeric_limits<Uint32>::max())
			{
				center = index;
				continue;
			}

			if (last == std::numeric_limits<Uint32>::max())
			{
				last = index;
				continue;
			}

			BOOST_CHECK(check_front_facing(
				glm::vec3(0.0f, 0.0f, 1.0f),
				glm::uvec2(tile_size + 1),
				center, last, index));

			triangle = Triangle(center, last, index);

			last = index;

			BOOST_CHECK_EQUAL(restart_triangles.count(triangle), 0);

			restart_triangles.insert(triangle);
		}

		BOOST_CHECK_EQUAL(vertices.count(), vertex_count);

		el::IndexBuilder::build_plane_indices(tile_size, false, 0,
			split, indices);

		for (j = 0; j < (indices.size() / 3); ++j)
		{
			BOOST_CHECK(check_front_facing(
				glm::vec3(0.0f, 0.0f, 1.0f),
				glm::uvec2(tile_size + 1),
				indices[j * 3 + 0],
				indices[j * 3 + 1], indices[j * 3 + 2]));

			triangle = Triangle(indices[j * 3 + 0],
				indices[j * 3 + 1], indices[j * 3 + 2]);

			BOOST_CHECK_EQUAL(triangles.count(triangle), 0);

			triangles.insert(triangle);
		}

		BOOST_CHECK_EQUAL(restart_triangles.size(), triangles.size());

		BOOST_FOREACH(const Triangle &triangle, triangles)
		{
			BOOST_CHECK_EQUAL(restart_triangles.count(triangle), 1);
		}

		BOOST_FOREACH(const Triangle &triangle, restart_triangles)
		{
			BOOST_CHECK_EQUAL(triangles.count(triangle), 1);
		}
	}
}

BOOST_AUTO_TEST_CASE(terrain_no_restart)
{
	el::Uint32Vector indices;
	Uint32 vertex_count, max_vertex, index_count, tile_size, i, j;
	std::set<Triangle> triangles;
	Triangle triangle;
	std::bitset<65536> vertices;
	bool use_restart_index;

	use_restart_index = false;

	for (i = 0; i < 7; i++)
	{
		indices.clear();
		triangles.clear();

		tile_size = 2 << i;

		BOOST_CHECK_LT(tile_size, 256);

		vertex_count = tile_size + 1;
		vertex_count *= tile_size + 1;

		BOOST_CHECK_LE(vertex_count,
			std::numeric_limits<Uint16>::max());
		BOOST_CHECK_EQUAL(vertex_count % 2, 1);

		max_vertex = vertex_count - 1;

		BOOST_CHECK_LT(max_vertex, std::numeric_limits<Uint16>::max());

		index_count = tile_size * tile_size * 6;

		el::IndexBuilder::build_terrain_indices(tile_size,
			use_restart_index, indices);

		BOOST_FOREACH(const Uint32 index, indices)
		{
			BOOST_CHECK_LE(index, max_vertex);

			vertices[index] = true;
		}

		BOOST_CHECK_EQUAL(indices.size(), index_count);
		BOOST_CHECK_EQUAL(vertices.count(), vertex_count);

		for (j = 0; j < (indices.size() / 3); ++j)
		{
			BOOST_CHECK(check_front_facing(
				glm::vec3(0.0f, 0.0f, 1.0f),
				glm::uvec2(tile_size + 1),
				indices[j * 3 + 0],
				indices[j * 3 + 1], indices[j * 3 + 2]));

			triangle = Triangle(indices[j * 3 + 0],
				indices[j * 3 + 1], indices[j * 3 + 2]);

			BOOST_CHECK_EQUAL(triangles.count(triangle), 0);

			triangles.insert(triangle);
		}
	}
}

BOOST_AUTO_TEST_CASE(terrain_restart)
{
	el::Uint32Vector indices, restart_indices;
	std::bitset<65536> vertices;
	std::set<Triangle> triangles, restart_triangles;
	Triangle triangle;
	Uint32 vertex_count, index_count, tile_size, max_vertex, i, j;
	glm::uvec2 last;
	bool flip_triangle;

	for (i = 0; i < 7; i++)
	{
		indices.clear();
		triangles.clear();
		restart_indices.clear();
		restart_triangles.clear();

		tile_size = 2 << i;

		BOOST_CHECK_LT(tile_size, 256);

		vertex_count = tile_size + 1;
		vertex_count *= tile_size + 1;

		BOOST_CHECK_LE(vertex_count,
			std::numeric_limits<Uint16>::max());
		BOOST_CHECK_EQUAL(vertex_count % 2, 1);

		max_vertex = vertex_count - 1;

		BOOST_CHECK_LE(max_vertex, std::numeric_limits<Uint16>::max());

		index_count = tile_size * (tile_size * 2 + 3);

		el::IndexBuilder::build_terrain_indices(tile_size, true,
			restart_indices);

		BOOST_CHECK_EQUAL(restart_indices.size(), index_count);

		last.x = std::numeric_limits<Uint32>::max();
		last.y = std::numeric_limits<Uint32>::max();
		flip_triangle = false;

		BOOST_FOREACH(const Uint32 index, restart_indices)
		{
			if (index == std::numeric_limits<Uint32>::max())
			{
				last.x = std::numeric_limits<Uint32>::max();
				last.y = std::numeric_limits<Uint32>::max();
				flip_triangle = false;
				continue;
			}

			BOOST_CHECK_LE(index, max_vertex);

			vertices[index] = true;

			if (last.x == std::numeric_limits<Uint32>::max())
			{
				last.x = index;
				continue;
			}

			if (last.y == std::numeric_limits<Uint32>::max())
			{
				last.y = index;
				continue;
			}

			if (flip_triangle)
			{
				BOOST_CHECK(check_front_facing(
					glm::vec3(0.0f, 0.0f, 1.0f),
					glm::uvec2(tile_size + 1),
					last.y, last.x, index));
			}
			else
			{
				BOOST_CHECK(check_front_facing(
					glm::vec3(0.0f, 0.0f, 1.0f),
					glm::uvec2(tile_size + 1),
					last.x, last.y, index));
			}

			triangle = Triangle(last.x, last.y, index);

			last.x = last.y;
			last.y = index;

			BOOST_CHECK_EQUAL(restart_triangles.count(triangle), 0);

			restart_triangles.insert(triangle);

			flip_triangle = !flip_triangle;
		}

		BOOST_CHECK_EQUAL(vertices.count(), vertex_count);

		el::IndexBuilder::build_terrain_indices(tile_size, false,
			indices);

		for (j = 0; j < (indices.size() / 3); ++j)
		{
			BOOST_CHECK(check_front_facing(
				glm::vec3(0.0f, 0.0f, 1.0f),
				glm::uvec2(tile_size + 1),
				indices[j * 3 + 0],
				indices[j * 3 + 1], indices[j * 3 + 2]));

			triangle = Triangle(indices[j * 3 + 0],
				indices[j * 3 + 1], indices[j * 3 + 2]);

			BOOST_CHECK_EQUAL(triangles.count(triangle), 0);

			triangles.insert(triangle);
		}

		BOOST_CHECK_EQUAL(restart_triangles.size(), triangles.size());

		BOOST_FOREACH(const Triangle &triangle, triangles)
		{
			BOOST_CHECK_EQUAL(restart_triangles.count(triangle), 1);
		}

		BOOST_FOREACH(const Triangle &triangle, restart_triangles)
		{
			BOOST_CHECK_EQUAL(triangles.count(triangle), 1);
		}
	}
}
