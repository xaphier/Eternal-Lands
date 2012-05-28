/****************************************************************************
 *            cdlodquadtree.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "prerequisites.hpp"
#include "terrain/cdlodquadtree.hpp"
#include <boost/random.hpp>
#define BOOST_TEST_MODULE cd_lod_quad_tree
#include <boost/test/unit_test.hpp>

namespace el = eternal_lands;

BOOST_AUTO_TEST_CASE(get_quad_order)
{
	glm::uvec2 sum;
	glm::vec2 dir;
	glm::vec4 dist;
	Uint32 i;

	for (i = 0; i <= std::numeric_limits<Uint16>::max(); ++i)
	{
		dir.x = (i * 2.0f) / std::numeric_limits<Uint16>::max() - 1.0f;
		dir.y = std::sqrt(1.0f - dir.x * dir.x);

		BOOST_CHECK_NO_THROW(el::CdLodQuadTree::get_quad_order(dir));

		const el::Uvec2Array4 quad_order =
			el::CdLodQuadTree::get_quad_order(dir);

		BOOST_CHECK_LE(quad_order[0].x, 1);
		BOOST_CHECK_LE(quad_order[0].y, 1);
		BOOST_CHECK_LE(quad_order[1].x, 1);
		BOOST_CHECK_LE(quad_order[1].y, 1);
		BOOST_CHECK_LE(quad_order[2].x, 1);
		BOOST_CHECK_LE(quad_order[2].y, 1);
		BOOST_CHECK_LE(quad_order[3].x, 1);
		BOOST_CHECK_LE(quad_order[3].y, 1);

		sum = quad_order[0] + quad_order[1] + quad_order[2] +
			quad_order[3];

		BOOST_CHECK_EQUAL(sum.x, 2);
		BOOST_CHECK_EQUAL(sum.y, 2);

		dist[0] = glm::distance(glm::vec2(quad_order[0]), -dir * 10.0f);
		dist[1] = glm::distance(glm::vec2(quad_order[1]), -dir * 10.0f);
		dist[2] = glm::distance(glm::vec2(quad_order[2]), -dir * 10.0f);
		dist[3] = glm::distance(glm::vec2(quad_order[3]), -dir * 10.0f);

		BOOST_CHECK_LE(dist[0], dist[1] + 0.1f);
		BOOST_CHECK_LE(dist[1], dist[2] + 0.1f);
		BOOST_CHECK_LE(dist[2], dist[3] + 0.1f);
	}

	for (i = 0; i <= std::numeric_limits<Uint16>::max(); ++i)
	{
		dir.x = (i * 2.0f) / std::numeric_limits<Uint16>::max() - 1.0f;
		dir.y = -std::sqrt(1.0f - dir.x * dir.x);

		BOOST_CHECK_NO_THROW(el::CdLodQuadTree::get_quad_order(dir));

		const el::Uvec2Array4 quad_order =
			el::CdLodQuadTree::get_quad_order(dir);

		BOOST_CHECK_LE(quad_order[0].x, 1);
		BOOST_CHECK_LE(quad_order[0].y, 1);
		BOOST_CHECK_LE(quad_order[1].x, 1);
		BOOST_CHECK_LE(quad_order[1].y, 1);
		BOOST_CHECK_LE(quad_order[2].x, 1);
		BOOST_CHECK_LE(quad_order[2].y, 1);
		BOOST_CHECK_LE(quad_order[3].x, 1);
		BOOST_CHECK_LE(quad_order[3].y, 1);

		sum = quad_order[0] + quad_order[1] + quad_order[2] +
			quad_order[3];

		BOOST_CHECK_EQUAL(sum.x, 2);
		BOOST_CHECK_EQUAL(sum.y, 2);

		dist[0] = glm::distance(glm::vec2(quad_order[0]), -dir * 10.0f);
		dist[1] = glm::distance(glm::vec2(quad_order[1]), -dir * 10.0f);
		dist[2] = glm::distance(glm::vec2(quad_order[2]), -dir * 10.0f);
		dist[3] = glm::distance(glm::vec2(quad_order[3]), -dir * 10.0f);

		BOOST_CHECK_LE(dist[0], dist[1] + 0.1f);
		BOOST_CHECK_LE(dist[1], dist[2] + 0.1f);
		BOOST_CHECK_LE(dist[2], dist[3] + 0.1f);
	}
}
