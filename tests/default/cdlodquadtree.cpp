/****************************************************************************
 *            cdlodquadtree.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "prerequisites.hpp"
#include "image.hpp"
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

BOOST_AUTO_TEST_CASE(construct)
{
	boost::scoped_ptr<el::CdLodQuadTree> tree;
	el::ImageSharedPtr vector_map;

	vector_map = boost::make_shared<el::Image>(
		el::String(UTF8("vector_map")), false, el::tft_rgb32f,
		glm::uvec3(129, 129, 1), 0);

	BOOST_CHECK_NO_THROW(tree.reset(new el::CdLodQuadTree(vector_map,
		glm::vec3(1.0f), 0.25f)));

	BOOST_CHECK_EQUAL(tree->get_max_lod_count(), 8);
	BOOST_CHECK_EQUAL(tree->get_lod_count(), 4);
	BOOST_CHECK_EQUAL(tree->get_patch_size(), 16);
	BOOST_CHECK_CLOSE(tree->get_patch_scale(), 0.25f, 1.0f);
	BOOST_CHECK_CLOSE(tree->get_morph_zone_ratio(), 0.3f, 1.0f);
	BOOST_CHECK_EQUAL(tree->get_grid_size().x, 129);
	BOOST_CHECK_EQUAL(tree->get_grid_size().y, 129);
}
