/****************************************************************************
 *            meshdatatool.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "prerequisites.hpp"
#include "meshdatatool.hpp"
#include "indexbuilder.hpp"
#include "submesh.hpp"
#include <boost/random.hpp>
#define BOOST_TEST_MODULE mesh_data_tool
#include <boost/test/unit_test.hpp>

namespace el = eternal_lands;

BOOST_AUTO_TEST_CASE(create)
{
	el::MeshDataToolSharedPtr mesh_data_tool;
	el::VertexSemanticTypeSet semantics;
	glm::vec3 min, max;

	min = el::BoundingBox().get_min();
	max = el::BoundingBox().get_max();

	BOOST_CHECK_NO_THROW(mesh_data_tool.reset(new el::MeshDataTool(
		el::String("test"), 2353, 845, 1, semantics, el::pt_triangles,
		true, false)));

	BOOST_CHECK_EQUAL(mesh_data_tool->get_name(), "test");
	BOOST_CHECK_EQUAL(mesh_data_tool->get_vertex_count(), 2353);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_index_count(), 845);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_count(), 1);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_restart_index(),
		std::numeric_limits<Uint32>::max());
	BOOST_CHECK_EQUAL(mesh_data_tool->get_primitive(), el::pt_triangles);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_use_restart_index(), true);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_use_simd(), false);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_min()[0], min[0], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_min()[1], min[1], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_min()[2], min[2], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_max()[0], max[0], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_max()[1], max[1], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_max()[2], max[2], 0.001);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_data(0).get_offset(), 0);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_data(0).get_count(), 0);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_data(0).get_min_vertex(),
		0);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_data(0).get_max_vertex(),
		0);

	BOOST_CHECK_NO_THROW(mesh_data_tool.reset(new el::MeshDataTool(
		el::String("test"), 6572, 157, 674, semantics,
		el::pt_triangles, false, false)));

	BOOST_CHECK_EQUAL(mesh_data_tool->get_name(), "test");
	BOOST_CHECK_EQUAL(mesh_data_tool->get_vertex_count(), 6572);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_index_count(), 157);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_count(), 674);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_restart_index(),
		std::numeric_limits<Uint32>::max());
	BOOST_CHECK_EQUAL(mesh_data_tool->get_primitive(), el::pt_triangles);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_use_restart_index(), false);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_use_simd(), false);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_min()[0], min[0], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_min()[1], min[1], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_min()[2], min[2], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_max()[0], max[0], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_max()[1], max[1], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_max()[2], max[2], 0.001);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_data(0).get_offset(), 0);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_data(0).get_count(), 0);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_data(0).get_min_vertex(),
		0);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_data(0).get_max_vertex(),
		0);

	BOOST_CHECK_NO_THROW(mesh_data_tool.reset(new el::MeshDataTool(
		el::String("test"), 134, 93567, 32, semantics,
		el::pt_triangles, true, false)));

	BOOST_CHECK_EQUAL(mesh_data_tool->get_name(), "test");
	BOOST_CHECK_EQUAL(mesh_data_tool->get_vertex_count(), 134);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_index_count(), 93567);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_count(), 32);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_restart_index(),
		std::numeric_limits<Uint32>::max());
	BOOST_CHECK_EQUAL(mesh_data_tool->get_primitive(), el::pt_triangles);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_use_restart_index(), true);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_use_simd(), false);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_min()[0], min[0], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_min()[1], min[1], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_min()[2], min[2], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_max()[0], max[0], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_max()[1], max[1], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_max()[2], max[2], 0.001);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_data(0).get_offset(), 0);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_data(0).get_count(), 0);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_data(0).get_min_vertex(),
		0);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_data(0).get_max_vertex(),
		0);

	BOOST_CHECK_NO_THROW(mesh_data_tool.reset(new el::MeshDataTool(
		el::String("test"), 36546020, 230576086, 787553, semantics,
		el::pt_triangles, true, false)));

	BOOST_CHECK_EQUAL(mesh_data_tool->get_name(), "test");
	BOOST_CHECK_EQUAL(mesh_data_tool->get_vertex_count(), 36546020);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_index_count(), 230576086);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_count(), 787553);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_restart_index(),
		std::numeric_limits<Uint32>::max());
	BOOST_CHECK_EQUAL(mesh_data_tool->get_primitive(), el::pt_triangles);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_use_restart_index(), true);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_use_simd(), false);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_min()[0], min[0], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_min()[1], min[1], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_min()[2], min[2], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_max()[0], max[0], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_max()[1], max[1], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_max()[2], max[2], 0.001);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_data(0).get_offset(), 0);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_data(0).get_count(), 0);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_data(0).get_min_vertex(),
		0);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_data(0).get_max_vertex(),
		0);

	BOOST_CHECK_NO_THROW(mesh_data_tool.reset(new el::MeshDataTool(
		el::String("afs23"), 2353, 845, 1, semantics,
		el::pt_triangle_strip, true, true)));

	BOOST_CHECK_EQUAL(mesh_data_tool->get_name(), "afs23");
	BOOST_CHECK_EQUAL(mesh_data_tool->get_vertex_count(), 2353);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_index_count(), 845);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_count(), 1);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_restart_index(),
		std::numeric_limits<Uint32>::max());
	BOOST_CHECK_EQUAL(mesh_data_tool->get_primitive(),
		el::pt_triangle_strip);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_use_restart_index(), true);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_use_simd(), true);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_min()[0], min[0], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_min()[1], min[1], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_min()[2], min[2], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_max()[0], max[0], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_max()[1], max[1], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_max()[2], max[2], 0.001);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_data(0).get_offset(), 0);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_data(0).get_count(), 0);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_data(0).get_min_vertex(),
		0);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_data(0).get_max_vertex(),
		0);

	BOOST_CHECK_NO_THROW(mesh_data_tool.reset(new el::MeshDataTool(
		el::String("afs23"), 6572, 157, 674, semantics,
		el::pt_triangle_strip, false, true)));

	BOOST_CHECK_EQUAL(mesh_data_tool->get_name(), "afs23");
	BOOST_CHECK_EQUAL(mesh_data_tool->get_vertex_count(), 6572);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_index_count(), 157);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_count(), 674);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_restart_index(),
		std::numeric_limits<Uint32>::max());
	BOOST_CHECK_EQUAL(mesh_data_tool->get_primitive(),
		el::pt_triangle_strip);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_use_restart_index(), false);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_use_simd(), true);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_min()[0], min[0], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_min()[1], min[1], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_min()[2], min[2], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_max()[0], max[0], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_max()[1], max[1], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_max()[2], max[2], 0.001);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_data(0).get_offset(), 0);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_data(0).get_count(), 0);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_data(0).get_min_vertex(),
		0);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_data(0).get_max_vertex(),
		0);

	BOOST_CHECK_NO_THROW(mesh_data_tool.reset(new el::MeshDataTool(
		el::String("afs23"), 134, 93567, 32, semantics,
		el::pt_triangle_strip, true, true)));

	BOOST_CHECK_EQUAL(mesh_data_tool->get_name(), "afs23");
	BOOST_CHECK_EQUAL(mesh_data_tool->get_vertex_count(), 134);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_index_count(), 93567);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_count(), 32);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_restart_index(),
		std::numeric_limits<Uint32>::max());
	BOOST_CHECK_EQUAL(mesh_data_tool->get_primitive(),
		el::pt_triangle_strip);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_use_restart_index(), true);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_use_simd(), true);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_min()[0], min[0], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_min()[1], min[1], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_min()[2], min[2], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_max()[0], max[0], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_max()[1], max[1], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_max()[2], max[2], 0.001);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_data(0).get_offset(), 0);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_data(0).get_count(), 0);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_data(0).get_min_vertex(),
		0);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_data(0).get_max_vertex(),
		0);

	BOOST_CHECK_NO_THROW(mesh_data_tool.reset(new el::MeshDataTool(
		el::String("afs23"), 36546020, 230576086, 787553, semantics,
		el::pt_triangle_strip, true, true)));

	BOOST_CHECK_EQUAL(mesh_data_tool->get_name(), "afs23");
	BOOST_CHECK_EQUAL(mesh_data_tool->get_vertex_count(), 36546020);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_index_count(), 230576086);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_count(), 787553);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_restart_index(),
		std::numeric_limits<Uint32>::max());
	BOOST_CHECK_EQUAL(mesh_data_tool->get_primitive(),
		el::pt_triangle_strip);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_use_restart_index(), true);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_use_simd(), true);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_min()[0], min[0], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_min()[1], min[1], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_min()[2], min[2], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_max()[0], max[0], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_max()[1], max[1], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_max()[2], max[2], 0.001);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_data(0).get_offset(), 0);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_data(0).get_count(), 0);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_data(0).get_min_vertex(),
		0);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_data(0).get_max_vertex(),
		0);

	BOOST_CHECK_NO_THROW(mesh_data_tool.reset(new el::MeshDataTool(
		el::String("ge4it8eg"), 2353, 845, 1, semantics,
		el::pt_triangle_fan, true, true)));

	BOOST_CHECK_EQUAL(mesh_data_tool->get_name(), "ge4it8eg");
	BOOST_CHECK_EQUAL(mesh_data_tool->get_vertex_count(), 2353);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_index_count(), 845);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_count(), 1);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_restart_index(),
		std::numeric_limits<Uint32>::max());
	BOOST_CHECK_EQUAL(mesh_data_tool->get_primitive(),
		el::pt_triangle_fan);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_use_restart_index(), true);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_use_simd(), true);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_min()[0], min[0], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_min()[1], min[1], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_min()[2], min[2], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_max()[0], max[0], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_max()[1], max[1], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_max()[2], max[2], 0.001);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_data(0).get_offset(), 0);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_data(0).get_count(), 0);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_data(0).get_min_vertex(),
		0);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_data(0).get_max_vertex(),
		0);

	BOOST_CHECK_NO_THROW(mesh_data_tool.reset(new el::MeshDataTool(
		el::String("ge4it8eg"), 6572, 157, 674, semantics,
		el::pt_triangle_fan, false, false)));

	BOOST_CHECK_EQUAL(mesh_data_tool->get_name(), "ge4it8eg");
	BOOST_CHECK_EQUAL(mesh_data_tool->get_vertex_count(), 6572);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_index_count(), 157);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_count(), 674);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_restart_index(),
		std::numeric_limits<Uint32>::max());
	BOOST_CHECK_EQUAL(mesh_data_tool->get_primitive(),
		el::pt_triangle_fan);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_use_restart_index(), false);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_use_simd(), false);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_min()[0], min[0], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_min()[1], min[1], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_min()[2], min[2], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_max()[0], max[0], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_max()[1], max[1], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_max()[2], max[2], 0.001);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_data(0).get_offset(), 0);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_data(0).get_count(), 0);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_data(0).get_min_vertex(),
		0);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_data(0).get_max_vertex(),
		0);

	BOOST_CHECK_NO_THROW(mesh_data_tool.reset(new el::MeshDataTool(
		el::String("ge4it8eg"), 134, 93567, 32, semantics,
		el::pt_triangle_fan, true, false)));

	BOOST_CHECK_EQUAL(mesh_data_tool->get_name(), "ge4it8eg");
	BOOST_CHECK_EQUAL(mesh_data_tool->get_vertex_count(), 134);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_index_count(), 93567);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_count(), 32);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_restart_index(),
		std::numeric_limits<Uint32>::max());
	BOOST_CHECK_EQUAL(mesh_data_tool->get_primitive(),
		el::pt_triangle_fan);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_use_restart_index(), true);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_use_simd(), false);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_min()[0], min[0], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_min()[1], min[1], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_min()[2], min[2], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_max()[0], max[0], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_max()[1], max[1], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_max()[2], max[2], 0.001);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_data(0).get_offset(), 0);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_data(0).get_count(), 0);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_data(0).get_min_vertex(),
		0);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_data(0).get_max_vertex(),
		0);

	BOOST_CHECK_NO_THROW(mesh_data_tool.reset(new el::MeshDataTool(
		el::String("ge4it8eg"), 36546020, 230576086, 787553, semantics,
		el::pt_triangle_fan, true, false)));

	BOOST_CHECK_EQUAL(mesh_data_tool->get_name(), "ge4it8eg");
	BOOST_CHECK_EQUAL(mesh_data_tool->get_vertex_count(), 36546020);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_index_count(), 230576086);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_count(), 787553);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_restart_index(),
		std::numeric_limits<Uint32>::max());
	BOOST_CHECK_EQUAL(mesh_data_tool->get_primitive(),
		el::pt_triangle_fan);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_use_restart_index(), true);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_use_simd(), false);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_min()[0], min[0], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_min()[1], min[1], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_min()[2], min[2], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_max()[0], max[0], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_max()[1], max[1], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_max()[2], max[2], 0.001);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_data(0).get_offset(), 0);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_data(0).get_count(), 0);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_data(0).get_min_vertex(),
		0);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_data(0).get_max_vertex(),
		0);
}

BOOST_AUTO_TEST_CASE(add_plane)
{
	el::MeshDataToolSharedPtr mesh_data_tool;
	glm::vec4 normal, tangent, data;
	glm::vec3 min, max;
	glm::vec2 size, uv;
	el::Uint32Vector indices;
	el::VertexSemanticTypeSet semantics;
	el::Uint32 vertex_count, index_count, i, index, x, y, tile_size;
	el::PrimitiveType primitive;
	bool use_restart_index, split;

	split = true;
	tile_size = 16;
	use_restart_index = false;

	vertex_count = tile_size + 1;
	vertex_count *= tile_size + 1;

	if (use_restart_index)
	{
		primitive = el::pt_triangle_fan;
	}
	else
	{
		primitive = el::pt_triangles;
	}

	el::IndexBuilder::build_plane_indices(indices, tile_size,
		use_restart_index, 0, split);

	index_count = indices.size();

	semantics.insert(el::vst_position);
	semantics.insert(el::vst_texture_coordinate_0);
	semantics.insert(el::vst_normal);
	semantics.insert(el::vst_tangent);

	mesh_data_tool = boost::make_shared<el::MeshDataTool>(
		el::String("plane"), vertex_count, index_count, 1, semantics,
		primitive, use_restart_index, false);

	for (i = 0; i < index_count; ++i)
	{
		mesh_data_tool->set_index_data(i, indices[i]);
	}

	normal = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	tangent = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

	index = 0;

	for (y = 0; y <= tile_size; ++y)
	{
		for (x = 0; x <= tile_size; ++x)
		{
			uv = glm::vec2(x, y) / static_cast<float>(tile_size);

			data = glm::vec4(uv, 0.0f, 1.0f);

			mesh_data_tool->set_vertex_data(el::vst_position, index,
				data);
			mesh_data_tool->set_vertex_data(el::vst_normal, index,
				normal);
			mesh_data_tool->set_vertex_data(el::vst_tangent, index,
				tangent);
			mesh_data_tool->set_vertex_data(
				el::vst_texture_coordinate_0, index, data);

			++index;
		}
	}

	min = glm::vec3(0.0f, 0.0f, -0.01f);
	max = glm::vec3(1.0f, 1.0f, 0.0f);

	mesh_data_tool->set_sub_mesh_data(0, el::SubMesh(el::BoundingBox(min,
		max), 0, index_count, 0, vertex_count - 1));

	BOOST_CHECK_EQUAL(mesh_data_tool->get_name(), "plane");
	BOOST_CHECK_EQUAL(mesh_data_tool->get_vertex_count(), vertex_count);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_index_count(), index_count);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_count(), 1);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_restart_index(),
		std::numeric_limits<Uint32>::max());
	BOOST_CHECK_EQUAL(mesh_data_tool->get_primitive(), primitive);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_use_restart_index(),
		use_restart_index);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_use_simd(), false);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_min()[0], min[0], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_min()[1], min[1], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_min()[2], min[2], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_max()[0], max[0], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_max()[1], max[1], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_max()[2], max[2], 0.001);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_data(0).get_offset(), 0);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_data(0).get_count(),
		index_count);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_data(0).get_min_vertex(),
		0);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_data(0).get_max_vertex(),
		vertex_count - 1);

	for (i = 0; i < index_count; ++i)
	{
		BOOST_CHECK_EQUAL(mesh_data_tool->get_index_data(i),
			indices[i]);
	}

	index = 0;

	for (y = 0; y <= tile_size; ++y)
	{
		for (x = 0; x <= tile_size; ++x)
		{
			uv = glm::vec2(x, y) / static_cast<float>(tile_size);

			data = glm::vec4(uv, 0.0f, 1.0f);

			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_position, index).x, data.x, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_position, index).y, data.y, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_position, index).z, data.z, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_position, index).w, data.w, 0.001);

			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_normal, index).x, normal.x, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_normal, index).y, normal.y, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_normal, index).z, normal.z, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_normal, index).w, normal.w, 0.001);

			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_tangent, index).x, tangent.x, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_tangent, index).y, tangent.y, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_tangent, index).z, tangent.z, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_tangent, index).w, tangent.w, 0.001);

			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_texture_coordinate_0, index).x, data.x,
				0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_texture_coordinate_0, index).y, data.y,
				0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_texture_coordinate_0, index).z, data.z,
				0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_texture_coordinate_0, index).w, data.w,
				0.001);

			++index;
		}
	}
}

BOOST_AUTO_TEST_CASE(add_plane_build_normal)
{
	boost::mt19937 rng;
	boost::uniform_int<Sint32> range(-16777216, 16777216);
	boost::variate_generator<boost::mt19937&, boost::uniform_int<Sint32> >
		random_int(rng, range);
	el::MeshDataToolSharedPtr mesh_data_tool;
	glm::vec4 normal, tangent, data;
	glm::vec3 min, max;
	glm::vec2 size, uv;
	el::Uint32Vector indices;
	el::VertexSemanticTypeSet semantics;
	el::Uint32 vertex_count, index_count, i, index, x, y, tile_size;
	el::PrimitiveType primitive;
	bool use_restart_index, split;

	split = true;
	tile_size = 16;
	use_restart_index = false;

	vertex_count = tile_size + 1;
	vertex_count *= tile_size + 1;

	if (use_restart_index)
	{
		primitive = el::pt_triangle_fan;
	}
	else
	{
		primitive = el::pt_triangles;
	}

	el::IndexBuilder::build_plane_indices(indices, tile_size,
		use_restart_index, 0, split);

	index_count = indices.size();

	semantics.insert(el::vst_position);
	semantics.insert(el::vst_texture_coordinate_0);
	semantics.insert(el::vst_normal);
	semantics.insert(el::vst_tangent);

	mesh_data_tool = boost::make_shared<el::MeshDataTool>(
		el::String("normals"), vertex_count, index_count, 1, semantics,
		primitive, use_restart_index, false);

	for (i = 0; i < index_count; ++i)
	{
		mesh_data_tool->set_index_data(i, indices[i]);
	}

	normal = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	tangent = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

	index = 0;

	for (y = 0; y <= tile_size; ++y)
	{
		for (x = 0; x <= tile_size; ++x)
		{
			uv = glm::vec2(x, y) / static_cast<float>(tile_size);

			data = glm::vec4(uv, 0.0f, 1.0f);

			for (i = 0; i < 4; ++i)
			{
				normal[i] = random_int() * 0.001f;
			}

			mesh_data_tool->set_vertex_data(el::vst_position, index,
				data);
			mesh_data_tool->set_vertex_data(el::vst_normal, index,
				normal);
			mesh_data_tool->set_vertex_data(el::vst_tangent, index,
				tangent);
			mesh_data_tool->set_vertex_data(
				el::vst_texture_coordinate_0, index, data);

			++index;
		}
	}

	min = glm::vec3(0.0f, 0.0f, -0.01f);
	max = glm::vec3(1.0f, 1.0f, 0.0f);

	mesh_data_tool->set_sub_mesh_data(0, el::SubMesh(el::BoundingBox(min,
		max), 0, index_count, 0, vertex_count - 1));

	BOOST_CHECK_NO_THROW(mesh_data_tool->build_normal(false));

	normal = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);

	BOOST_CHECK_EQUAL(mesh_data_tool->get_name(), "normals");
	BOOST_CHECK_EQUAL(mesh_data_tool->get_vertex_count(), vertex_count);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_index_count(), index_count);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_count(), 1);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_restart_index(),
		std::numeric_limits<Uint32>::max());
	BOOST_CHECK_EQUAL(mesh_data_tool->get_primitive(), primitive);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_use_restart_index(),
		use_restart_index);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_use_simd(), false);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_min()[0], min[0], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_min()[1], min[1], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_min()[2], min[2], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_max()[0], max[0], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_max()[1], max[1], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_max()[2], max[2], 0.001);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_data(0).get_offset(), 0);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_data(0).get_count(),
		index_count);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_data(0).get_min_vertex(),
		0);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_data(0).get_max_vertex(),
		vertex_count - 1);

	for (i = 0; i < index_count; ++i)
	{
		BOOST_CHECK_EQUAL(mesh_data_tool->get_index_data(i),
			indices[i]);
	}

	index = 0;

	for (y = 0; y <= tile_size; ++y)
	{
		for (x = 0; x <= tile_size; ++x)
		{
			uv = glm::vec2(x, y) / static_cast<float>(tile_size);

			data = glm::vec4(uv, 0.0f, 1.0f);

			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_position, index).x, data.x, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_position, index).y, data.y, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_position, index).z, data.z, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_position, index).w, data.w, 0.001);

			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_normal, index).x, normal.x, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_normal, index).y, normal.y, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_normal, index).z, normal.z, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_normal, index).w, normal.w, 0.001);

			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_tangent, index).x, tangent.x, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_tangent, index).y, tangent.y, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_tangent, index).z, tangent.z, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_tangent, index).w, tangent.w, 0.001);

			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_texture_coordinate_0, index).x, data.x,
				0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_texture_coordinate_0, index).y, data.y,
				0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_texture_coordinate_0, index).z, data.z,
				0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_texture_coordinate_0, index).w, data.w,
				0.001);

			++index;
		}
	}
}

BOOST_AUTO_TEST_CASE(add_plane_build_tangent)
{
	boost::mt19937 rng;
	boost::uniform_int<Sint32> range(-16777216, 16777216);
	boost::variate_generator<boost::mt19937&, boost::uniform_int<Sint32> >
		random_int(rng, range);
	el::MeshDataToolSharedPtr mesh_data_tool;
	glm::vec4 normal, tangent, data;
	glm::vec3 min, max;
	glm::vec2 size, uv;
	el::Uint32Vector indices;
	el::VertexSemanticTypeSet semantics;
	el::Uint32 vertex_count, index_count, i, index, x, y, tile_size;
	el::PrimitiveType primitive;
	bool use_restart_index, split;

	split = true;
	tile_size = 16;
	use_restart_index = false;

	vertex_count = tile_size + 1;
	vertex_count *= tile_size + 1;

	if (use_restart_index)
	{
		primitive = el::pt_triangle_fan;
	}
	else
	{
		primitive = el::pt_triangles;
	}

	el::IndexBuilder::build_plane_indices(indices, tile_size,
		use_restart_index, 0, split);

	index_count = indices.size();

	semantics.insert(el::vst_position);
	semantics.insert(el::vst_texture_coordinate_0);
	semantics.insert(el::vst_normal);
	semantics.insert(el::vst_tangent);

	mesh_data_tool = boost::make_shared<el::MeshDataTool>(
		el::String("tangents"), vertex_count, index_count, 1,
		semantics, primitive, use_restart_index, false);

	for (i = 0; i < index_count; ++i)
	{
		mesh_data_tool->set_index_data(i, indices[i]);
	}

	normal = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	tangent = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

	index = 0;

	for (y = 0; y <= tile_size; ++y)
	{
		for (x = 0; x <= tile_size; ++x)
		{
			uv = glm::vec2(x, y) / static_cast<float>(tile_size);

			data = glm::vec4(uv, 0.0f, 1.0f);

			for (i = 0; i < 4; ++i)
			{
				tangent[i] = random_int() * 0.001f;
			}

			mesh_data_tool->set_vertex_data(el::vst_position, index,
				data);
			mesh_data_tool->set_vertex_data(el::vst_normal, index,
				normal);
			mesh_data_tool->set_vertex_data(el::vst_tangent, index,
				tangent);
			mesh_data_tool->set_vertex_data(
				el::vst_texture_coordinate_0, index, data);

			++index;
		}
	}

	min = glm::vec3(0.0f, 0.0f, -0.01f);
	max = glm::vec3(1.0f, 1.0f, 0.0f);

	mesh_data_tool->set_sub_mesh_data(0, el::SubMesh(el::BoundingBox(min,
		max), 0, index_count, 0, vertex_count - 1));

	BOOST_CHECK_NO_THROW(mesh_data_tool->build_tangent(false, false,
		false));

	tangent = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

	BOOST_CHECK_EQUAL(mesh_data_tool->get_name(), "tangents");
	BOOST_CHECK_EQUAL(mesh_data_tool->get_vertex_count(), vertex_count);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_index_count(), index_count);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_count(), 1);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_restart_index(),
		std::numeric_limits<Uint32>::max());
	BOOST_CHECK_EQUAL(mesh_data_tool->get_primitive(), primitive);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_use_restart_index(),
		use_restart_index);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_use_simd(), false);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_min()[0], min[0], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_min()[1], min[1], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_min()[2], min[2], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_max()[0], max[0], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_max()[1], max[1], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_max()[2], max[2], 0.001);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_data(0).get_offset(), 0);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_data(0).get_count(),
		index_count);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_data(0).get_min_vertex(),
		0);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_data(0).get_max_vertex(),
		vertex_count - 1);

	for (i = 0; i < index_count; ++i)
	{
		BOOST_CHECK_EQUAL(mesh_data_tool->get_index_data(i),
			indices[i]);
	}

	index = 0;

	for (y = 0; y <= tile_size; ++y)
	{
		for (x = 0; x <= tile_size; ++x)
		{
			uv = glm::vec2(x, y) / static_cast<float>(tile_size);

			data = glm::vec4(uv, 0.0f, 1.0f);

			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_position, index).x, data.x, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_position, index).y, data.y, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_position, index).z, data.z, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_position, index).w, data.w, 0.001);

			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_normal, index).x, normal.x, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_normal, index).y, normal.y, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_normal, index).z, normal.z, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_normal, index).w, normal.w, 0.001);

			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_tangent, index).x, tangent.x, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_tangent, index).y, tangent.y, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_tangent, index).z, tangent.z, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_tangent, index).w, tangent.w, 0.001);

			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_texture_coordinate_0, index).x, data.x,
				0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_texture_coordinate_0, index).y, data.y,
				0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_texture_coordinate_0, index).z, data.z,
				0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_texture_coordinate_0, index).w, data.w,
				0.001);

			++index;
		}
	}
}

BOOST_AUTO_TEST_CASE(add_plane_build_tangent_gram_schmidth_ortho)
{
	boost::mt19937 rng;
	boost::uniform_int<Sint32> range(-16777216, 16777216);
	boost::variate_generator<boost::mt19937&, boost::uniform_int<Sint32> >
		random_int(rng, range);
	el::MeshDataToolSharedPtr mesh_data_tool;
	glm::vec4 normal, tangent, data;
	glm::vec3 min, max;
	glm::vec2 size, uv;
	el::Uint32Vector indices;
	el::VertexSemanticTypeSet semantics;
	el::Uint32 vertex_count, index_count, i, index, x, y, tile_size;
	el::PrimitiveType primitive;
	bool use_restart_index, split;

	split = true;
	tile_size = 16;
	use_restart_index = false;

	vertex_count = tile_size + 1;
	vertex_count *= tile_size + 1;

	if (use_restart_index)
	{
		primitive = el::pt_triangle_fan;
	}
	else
	{
		primitive = el::pt_triangles;
	}

	el::IndexBuilder::build_plane_indices(indices, tile_size,
		use_restart_index, 0, split);

	index_count = indices.size();

	semantics.insert(el::vst_position);
	semantics.insert(el::vst_texture_coordinate_0);
	semantics.insert(el::vst_normal);
	semantics.insert(el::vst_tangent);

	mesh_data_tool = boost::make_shared<el::MeshDataTool>(
		el::String("tangents_gram_schmidth"), vertex_count,
		index_count, 1, semantics, primitive, use_restart_index, false);

	for (i = 0; i < index_count; ++i)
	{
		mesh_data_tool->set_index_data(i, indices[i]);
	}

	normal = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	tangent = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

	index = 0;

	for (y = 0; y <= tile_size; ++y)
	{
		for (x = 0; x <= tile_size; ++x)
		{
			uv = glm::vec2(x, y) / static_cast<float>(tile_size);

			data = glm::vec4(uv, 0.0f, 1.0f);

			for (i = 0; i < 4; ++i)
			{
				tangent[i] = random_int() * 0.001f;
			}

			mesh_data_tool->set_vertex_data(el::vst_position, index,
				data);
			mesh_data_tool->set_vertex_data(el::vst_normal, index,
				normal);
			mesh_data_tool->set_vertex_data(el::vst_tangent, index,
				tangent);
			mesh_data_tool->set_vertex_data(
				el::vst_texture_coordinate_0, index, data);

			++index;
		}
	}

	min = glm::vec3(0.0f, 0.0f, -0.01f);
	max = glm::vec3(1.0f, 1.0f, 0.0f);

	mesh_data_tool->set_sub_mesh_data(0, el::SubMesh(el::BoundingBox(min,
		max), 0, index_count, 0, vertex_count - 1));

	BOOST_CHECK_NO_THROW(mesh_data_tool->build_tangent(false, false,
		true));

	tangent = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

	BOOST_CHECK_EQUAL(mesh_data_tool->get_name(), "tangents_gram_schmidth");
	BOOST_CHECK_EQUAL(mesh_data_tool->get_vertex_count(), vertex_count);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_index_count(), index_count);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_count(), 1);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_restart_index(),
		std::numeric_limits<Uint32>::max());
	BOOST_CHECK_EQUAL(mesh_data_tool->get_primitive(), primitive);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_use_restart_index(),
		use_restart_index);

	BOOST_CHECK_EQUAL(mesh_data_tool->get_vertex_count(), vertex_count);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_index_count(), index_count);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_count(), 1);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_restart_index(),
		std::numeric_limits<Uint32>::max());
	BOOST_CHECK_EQUAL(mesh_data_tool->get_primitive(), primitive);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_use_restart_index(),
		use_restart_index);

	for (i = 0; i < index_count; ++i)
	{
		BOOST_CHECK_EQUAL(mesh_data_tool->get_index_data(i),
			indices[i]);
	}

	index = 0;

	for (y = 0; y <= tile_size; ++y)
	{
		for (x = 0; x <= tile_size; ++x)
		{
			uv = glm::vec2(x, y) / static_cast<float>(tile_size);

			data = glm::vec4(uv, 0.0f, 1.0f);

			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_position, index).x, data.x, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_position, index).y, data.y, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_position, index).z, data.z, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_position, index).w, data.w, 0.001);

			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_normal, index).x, normal.x, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_normal, index).y, normal.y, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_normal, index).z, normal.z, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_normal, index).w, normal.w, 0.001);

			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_tangent, index).x, tangent.x, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_tangent, index).y, tangent.y, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_tangent, index).z, tangent.z, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_tangent, index).w, tangent.w, 0.001);

			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_texture_coordinate_0, index).x, data.x,
				0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_texture_coordinate_0, index).y, data.y,
				0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_texture_coordinate_0, index).z, data.z,
				0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_texture_coordinate_0, index).w, data.w,
				0.001);

			++index;
		}
	}
}

BOOST_AUTO_TEST_CASE(add_plane_build_bounding_box)
{
	el::MeshDataToolSharedPtr mesh_data_tool;
	glm::vec4 normal, tangent, data;
	glm::vec3 min, max;
	glm::vec2 size, uv;
	el::Uint32Vector indices;
	el::VertexSemanticTypeSet semantics;
	el::Uint32 vertex_count, index_count, i, index, x, y, tile_size;
	el::PrimitiveType primitive;
	bool use_restart_index, split;

	split = true;
	tile_size = 16;
	use_restart_index = false;

	vertex_count = tile_size + 1;
	vertex_count *= tile_size + 1;

	if (use_restart_index)
	{
		primitive = el::pt_triangle_fan;
	}
	else
	{
		primitive = el::pt_triangles;
	}

	el::IndexBuilder::build_plane_indices(indices, tile_size,
		use_restart_index, 0, split);

	index_count = indices.size();

	semantics.insert(el::vst_position);
	semantics.insert(el::vst_texture_coordinate_0);
	semantics.insert(el::vst_normal);
	semantics.insert(el::vst_tangent);

	mesh_data_tool = boost::make_shared<el::MeshDataTool>(
		el::String("plane"), vertex_count, index_count, 1, semantics,
		primitive, use_restart_index, false);

	for (i = 0; i < index_count; ++i)
	{
		mesh_data_tool->set_index_data(i, indices[i]);
	}

	normal = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	tangent = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

	index = 0;

	for (y = 0; y <= tile_size; ++y)
	{
		for (x = 0; x <= tile_size; ++x)
		{
			uv = glm::vec2(x, y) / static_cast<float>(tile_size);

			data = glm::vec4(uv, 0.0f, 1.0f);

			mesh_data_tool->set_vertex_data(el::vst_position, index,
				data);
			mesh_data_tool->set_vertex_data(el::vst_normal, index,
				normal);
			mesh_data_tool->set_vertex_data(el::vst_tangent, index,
				tangent);
			mesh_data_tool->set_vertex_data(
				el::vst_texture_coordinate_0, index, data);

			++index;
		}
	}

	min = glm::vec3(235.0f, 12.0f, -0.541f);
	max = glm::vec3(189097.0f, 23541.0f, 230.0f);

	mesh_data_tool->set_sub_mesh_data(0, el::SubMesh(el::BoundingBox(min,
		max), 0, index_count, 0, vertex_count - 1));

	BOOST_CHECK_EQUAL(mesh_data_tool->get_name(), "plane");
	BOOST_CHECK_EQUAL(mesh_data_tool->get_vertex_count(), vertex_count);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_index_count(), index_count);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_count(), 1);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_restart_index(),
		std::numeric_limits<Uint32>::max());
	BOOST_CHECK_EQUAL(mesh_data_tool->get_primitive(), primitive);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_use_restart_index(),
		use_restart_index);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_use_simd(), false);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_min()[0], min[0], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_min()[1], min[1], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_min()[2], min[2], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_max()[0], max[0], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_max()[1], max[1], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_max()[2], max[2], 0.001);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_data(0).get_offset(), 0);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_data(0).get_count(),
		index_count);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_data(0).get_min_vertex(),
		0);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_data(0).get_max_vertex(),
		vertex_count - 1);

	for (i = 0; i < index_count; ++i)
	{
		BOOST_CHECK_EQUAL(mesh_data_tool->get_index_data(i),
			indices[i]);
	}

	index = 0;

	for (y = 0; y <= tile_size; ++y)
	{
		for (x = 0; x <= tile_size; ++x)
		{
			uv = glm::vec2(x, y) / static_cast<float>(tile_size);

			data = glm::vec4(uv, 0.0f, 1.0f);

			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_position, index).x, data.x, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_position, index).y, data.y, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_position, index).z, data.z, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_position, index).w, data.w, 0.001);

			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_normal, index).x, normal.x, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_normal, index).y, normal.y, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_normal, index).z, normal.z, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_normal, index).w, normal.w, 0.001);

			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_tangent, index).x, tangent.x, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_tangent, index).y, tangent.y, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_tangent, index).z, tangent.z, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_tangent, index).w, tangent.w, 0.001);

			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_texture_coordinate_0, index).x, data.x,
				0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_texture_coordinate_0, index).y, data.y,
				0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_texture_coordinate_0, index).z, data.z,
				0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_texture_coordinate_0, index).w, data.w,
				0.001);

			++index;
		}
	}

	min = glm::vec3(0.0f, 0.0f, 0.0f);
	max = glm::vec3(1.0f, 1.0f, 0.0f);

	BOOST_CHECK_NO_THROW(mesh_data_tool->update_sub_meshs_bounding_box());

	BOOST_CHECK_EQUAL(mesh_data_tool->get_name(), "plane");
	BOOST_CHECK_EQUAL(mesh_data_tool->get_vertex_count(), vertex_count);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_index_count(), index_count);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_count(), 1);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_restart_index(),
		std::numeric_limits<Uint32>::max());
	BOOST_CHECK_EQUAL(mesh_data_tool->get_primitive(), primitive);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_use_restart_index(),
		use_restart_index);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_use_simd(), false);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_min()[0], min[0], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_min()[1], min[1], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_min()[2], min[2], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_max()[0], max[0], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_max()[1], max[1], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_max()[2], max[2], 0.001);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_data(0).get_offset(), 0);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_data(0).get_count(),
		index_count);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_data(0).get_min_vertex(),
		0);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_data(0).get_max_vertex(),
		vertex_count - 1);

	for (i = 0; i < index_count; ++i)
	{
		BOOST_CHECK_EQUAL(mesh_data_tool->get_index_data(i),
			indices[i]);
	}

	index = 0;

	for (y = 0; y <= tile_size; ++y)
	{
		for (x = 0; x <= tile_size; ++x)
		{
			uv = glm::vec2(x, y) / static_cast<float>(tile_size);

			data = glm::vec4(uv, 0.0f, 1.0f);

			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_position, index).x, data.x, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_position, index).y, data.y, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_position, index).z, data.z, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_position, index).w, data.w, 0.001);

			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_normal, index).x, normal.x, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_normal, index).y, normal.y, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_normal, index).z, normal.z, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_normal, index).w, normal.w, 0.001);

			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_tangent, index).x, tangent.x, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_tangent, index).y, tangent.y, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_tangent, index).z, tangent.z, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_tangent, index).w, tangent.w, 0.001);

			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_texture_coordinate_0, index).x, data.x,
				0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_texture_coordinate_0, index).y, data.y,
				0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_texture_coordinate_0, index).z, data.z,
				0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_texture_coordinate_0, index).w, data.w,
				0.001);

			++index;
		}
	}
}

BOOST_AUTO_TEST_CASE(add_plane_use_restart)
{
	el::MeshDataToolSharedPtr mesh_data_tool;
	glm::vec4 normal, tangent, data;
	glm::vec3 min, max;
	glm::vec2 size, uv;
	el::Uint32Vector indices;
	el::VertexSemanticTypeSet semantics;
	el::Uint32 vertex_count, index_count, i, index, x, y, tile_size;
	el::PrimitiveType primitive;
	bool use_restart_index, split;

	split = true;
	tile_size = 16;
	use_restart_index = true;

	vertex_count = tile_size + 1;
	vertex_count *= tile_size + 1;

	if (use_restart_index)
	{
		primitive = el::pt_triangle_fan;
	}
	else
	{
		primitive = el::pt_triangles;
	}

	el::IndexBuilder::build_plane_indices(indices, tile_size,
		use_restart_index, 0, split);

	index_count = indices.size();

	semantics.insert(el::vst_position);
	semantics.insert(el::vst_texture_coordinate_0);
	semantics.insert(el::vst_normal);
	semantics.insert(el::vst_tangent);

	mesh_data_tool = boost::make_shared<el::MeshDataTool>(
		el::String("plane"), vertex_count, index_count, 1, semantics,
		primitive, use_restart_index, false);

	for (i = 0; i < index_count; ++i)
	{
		mesh_data_tool->set_index_data(i, indices[i]);
	}

	normal = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	tangent = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

	index = 0;

	for (y = 0; y <= tile_size; ++y)
	{
		for (x = 0; x <= tile_size; ++x)
		{
			uv = glm::vec2(x, y) / static_cast<float>(tile_size);

			data = glm::vec4(uv, 0.0f, 1.0f);

			mesh_data_tool->set_vertex_data(el::vst_position, index,
				data);
			mesh_data_tool->set_vertex_data(el::vst_normal, index,
				normal);
			mesh_data_tool->set_vertex_data(el::vst_tangent, index,
				tangent);
			mesh_data_tool->set_vertex_data(
				el::vst_texture_coordinate_0, index, data);

			++index;
		}
	}

	min = glm::vec3(0.0f, 0.0f, -0.01f);
	max = glm::vec3(1.0f, 1.0f, 0.0f);

	mesh_data_tool->set_sub_mesh_data(0, el::SubMesh(el::BoundingBox(min,
		max), 0, index_count, 0, vertex_count - 1));

	BOOST_CHECK_EQUAL(mesh_data_tool->get_name(), "plane");
	BOOST_CHECK_EQUAL(mesh_data_tool->get_vertex_count(), vertex_count);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_index_count(), index_count);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_count(), 1);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_restart_index(),
		std::numeric_limits<Uint32>::max());
	BOOST_CHECK_EQUAL(mesh_data_tool->get_primitive(), primitive);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_use_restart_index(),
		use_restart_index);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_use_simd(), false);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_min()[0], min[0], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_min()[1], min[1], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_min()[2], min[2], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_max()[0], max[0], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_max()[1], max[1], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_max()[2], max[2], 0.001);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_data(0).get_offset(), 0);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_data(0).get_count(),
		index_count);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_data(0).get_min_vertex(),
		0);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_data(0).get_max_vertex(),
		vertex_count - 1);

	for (i = 0; i < index_count; ++i)
	{
		BOOST_CHECK_EQUAL(mesh_data_tool->get_index_data(i),
			indices[i]);
	}

	index = 0;

	for (y = 0; y <= tile_size; ++y)
	{
		for (x = 0; x <= tile_size; ++x)
		{
			uv = glm::vec2(x, y) / static_cast<float>(tile_size);

			data = glm::vec4(uv, 0.0f, 1.0f);

			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_position, index).x, data.x, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_position, index).y, data.y, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_position, index).z, data.z, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_position, index).w, data.w, 0.001);

			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_normal, index).x, normal.x, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_normal, index).y, normal.y, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_normal, index).z, normal.z, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_normal, index).w, normal.w, 0.001);

			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_tangent, index).x, tangent.x, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_tangent, index).y, tangent.y, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_tangent, index).z, tangent.z, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_tangent, index).w, tangent.w, 0.001);

			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_texture_coordinate_0, index).x, data.x,
				0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_texture_coordinate_0, index).y, data.y,
				0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_texture_coordinate_0, index).z, data.z,
				0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_texture_coordinate_0, index).w, data.w,
				0.001);

			++index;
		}
	}
}

BOOST_AUTO_TEST_CASE(add_plane_build_normal_use_restart)
{
	boost::mt19937 rng;
	boost::uniform_int<Sint32> range(-16777216, 16777216);
	boost::variate_generator<boost::mt19937&, boost::uniform_int<Sint32> >
		random_int(rng, range);
	el::MeshDataToolSharedPtr mesh_data_tool;
	glm::vec4 normal, tangent, data;
	glm::vec3 min, max;
	glm::vec2 size, uv;
	el::Uint32Vector indices;
	el::VertexSemanticTypeSet semantics;
	el::Uint32 vertex_count, index_count, i, index, x, y, tile_size;
	el::PrimitiveType primitive;
	bool use_restart_index, split;

	split = true;
	tile_size = 16;
	use_restart_index = true;

	vertex_count = tile_size + 1;
	vertex_count *= tile_size + 1;

	if (use_restart_index)
	{
		primitive = el::pt_triangle_fan;
	}
	else
	{
		primitive = el::pt_triangles;
	}

	el::IndexBuilder::build_plane_indices(indices, tile_size,
		use_restart_index, 0, split);

	index_count = indices.size();

	semantics.insert(el::vst_position);
	semantics.insert(el::vst_texture_coordinate_0);
	semantics.insert(el::vst_normal);
	semantics.insert(el::vst_tangent);

	mesh_data_tool = boost::make_shared<el::MeshDataTool>(
		el::String("normals"), vertex_count, index_count, 1, semantics,
		primitive, use_restart_index, false);

	for (i = 0; i < index_count; ++i)
	{
		mesh_data_tool->set_index_data(i, indices[i]);
	}

	normal = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	tangent = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

	index = 0;

	for (y = 0; y <= tile_size; ++y)
	{
		for (x = 0; x <= tile_size; ++x)
		{
			uv = glm::vec2(x, y) / static_cast<float>(tile_size);

			data = glm::vec4(uv, 0.0f, 1.0f);

			for (i = 0; i < 4; ++i)
			{
				normal[i] = random_int() * 0.001f;
			}

			mesh_data_tool->set_vertex_data(el::vst_position, index,
				data);
			mesh_data_tool->set_vertex_data(el::vst_normal, index,
				normal);
			mesh_data_tool->set_vertex_data(el::vst_tangent, index,
				tangent);
			mesh_data_tool->set_vertex_data(
				el::vst_texture_coordinate_0, index, data);

			++index;
		}
	}

	min = glm::vec3(0.0f, 0.0f, -0.01f);
	max = glm::vec3(1.0f, 1.0f, 0.0f);

	mesh_data_tool->set_sub_mesh_data(0, el::SubMesh(el::BoundingBox(min,
		max), 0, index_count, 0, vertex_count - 1));

	BOOST_CHECK_NO_THROW(mesh_data_tool->build_normal(false));

	normal = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);

	BOOST_CHECK_EQUAL(mesh_data_tool->get_name(), "normals");
	BOOST_CHECK_EQUAL(mesh_data_tool->get_vertex_count(), vertex_count);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_index_count(), index_count);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_count(), 1);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_restart_index(),
		std::numeric_limits<Uint32>::max());
	BOOST_CHECK_EQUAL(mesh_data_tool->get_primitive(), primitive);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_use_restart_index(),
		use_restart_index);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_use_simd(), false);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_min()[0], min[0], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_min()[1], min[1], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_min()[2], min[2], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_max()[0], max[0], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_max()[1], max[1], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_max()[2], max[2], 0.001);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_data(0).get_offset(), 0);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_data(0).get_count(),
		index_count);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_data(0).get_min_vertex(),
		0);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_data(0).get_max_vertex(),
		vertex_count - 1);

	for (i = 0; i < index_count; ++i)
	{
		BOOST_CHECK_EQUAL(mesh_data_tool->get_index_data(i),
			indices[i]);
	}

	index = 0;

	for (y = 0; y <= tile_size; ++y)
	{
		for (x = 0; x <= tile_size; ++x)
		{
			uv = glm::vec2(x, y) / static_cast<float>(tile_size);

			data = glm::vec4(uv, 0.0f, 1.0f);

			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_position, index).x, data.x, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_position, index).y, data.y, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_position, index).z, data.z, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_position, index).w, data.w, 0.001);

			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_normal, index).x, normal.x, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_normal, index).y, normal.y, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_normal, index).z, normal.z, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_normal, index).w, normal.w, 0.001);

			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_tangent, index).x, tangent.x, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_tangent, index).y, tangent.y, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_tangent, index).z, tangent.z, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_tangent, index).w, tangent.w, 0.001);

			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_texture_coordinate_0, index).x, data.x,
				0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_texture_coordinate_0, index).y, data.y,
				0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_texture_coordinate_0, index).z, data.z,
				0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_texture_coordinate_0, index).w, data.w,
				0.001);

			++index;
		}
	}
}

BOOST_AUTO_TEST_CASE(add_plane_build_tangent_use_restart)
{
	boost::mt19937 rng;
	boost::uniform_int<Sint32> range(-16777216, 16777216);
	boost::variate_generator<boost::mt19937&, boost::uniform_int<Sint32> >
		random_int(rng, range);
	el::MeshDataToolSharedPtr mesh_data_tool;
	glm::vec4 normal, tangent, data;
	glm::vec3 min, max;
	glm::vec2 size, uv;
	el::Uint32Vector indices;
	el::VertexSemanticTypeSet semantics;
	el::Uint32 vertex_count, index_count, i, index, x, y, tile_size;
	el::PrimitiveType primitive;
	bool use_restart_index, split;

	split = true;
	tile_size = 16;
	use_restart_index = true;

	vertex_count = tile_size + 1;
	vertex_count *= tile_size + 1;

	if (use_restart_index)
	{
		primitive = el::pt_triangle_fan;
	}
	else
	{
		primitive = el::pt_triangles;
	}

	el::IndexBuilder::build_plane_indices(indices, tile_size,
		use_restart_index, 0, split);

	index_count = indices.size();

	semantics.insert(el::vst_position);
	semantics.insert(el::vst_texture_coordinate_0);
	semantics.insert(el::vst_normal);
	semantics.insert(el::vst_tangent);

	mesh_data_tool = boost::make_shared<el::MeshDataTool>(
		el::String("tangents"), vertex_count, index_count, 1,
		semantics, primitive, use_restart_index, false);

	for (i = 0; i < index_count; ++i)
	{
		mesh_data_tool->set_index_data(i, indices[i]);
	}

	normal = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	tangent = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

	index = 0;

	for (y = 0; y <= tile_size; ++y)
	{
		for (x = 0; x <= tile_size; ++x)
		{
			uv = glm::vec2(x, y) / static_cast<float>(tile_size);

			data = glm::vec4(uv, 0.0f, 1.0f);

			for (i = 0; i < 4; ++i)
			{
				tangent[i] = random_int() * 0.001f;
			}

			mesh_data_tool->set_vertex_data(el::vst_position, index,
				data);
			mesh_data_tool->set_vertex_data(el::vst_normal, index,
				normal);
			mesh_data_tool->set_vertex_data(el::vst_tangent, index,
				tangent);
			mesh_data_tool->set_vertex_data(
				el::vst_texture_coordinate_0, index, data);

			++index;
		}
	}

	min = glm::vec3(0.0f, 0.0f, -0.01f);
	max = glm::vec3(1.0f, 1.0f, 0.0f);

	mesh_data_tool->set_sub_mesh_data(0, el::SubMesh(el::BoundingBox(min,
		max), 0, index_count, 0, vertex_count - 1));

	BOOST_CHECK_NO_THROW(mesh_data_tool->build_tangent(false, false,
		false));

	tangent = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

	BOOST_CHECK_EQUAL(mesh_data_tool->get_name(), "tangents");
	BOOST_CHECK_EQUAL(mesh_data_tool->get_vertex_count(), vertex_count);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_index_count(), index_count);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_count(), 1);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_restart_index(),
		std::numeric_limits<Uint32>::max());
	BOOST_CHECK_EQUAL(mesh_data_tool->get_primitive(), primitive);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_use_restart_index(),
		use_restart_index);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_use_simd(), false);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_min()[0], min[0], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_min()[1], min[1], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_min()[2], min[2], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_max()[0], max[0], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_max()[1], max[1], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_max()[2], max[2], 0.001);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_data(0).get_offset(), 0);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_data(0).get_count(),
		index_count);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_data(0).get_min_vertex(),
		0);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_data(0).get_max_vertex(),
		vertex_count - 1);

	for (i = 0; i < index_count; ++i)
	{
		BOOST_CHECK_EQUAL(mesh_data_tool->get_index_data(i),
			indices[i]);
	}

	index = 0;

	for (y = 0; y <= tile_size; ++y)
	{
		for (x = 0; x <= tile_size; ++x)
		{
			uv = glm::vec2(x, y) / static_cast<float>(tile_size);

			data = glm::vec4(uv, 0.0f, 1.0f);

			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_position, index).x, data.x, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_position, index).y, data.y, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_position, index).z, data.z, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_position, index).w, data.w, 0.001);

			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_normal, index).x, normal.x, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_normal, index).y, normal.y, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_normal, index).z, normal.z, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_normal, index).w, normal.w, 0.001);

			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_tangent, index).x, tangent.x, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_tangent, index).y, tangent.y, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_tangent, index).z, tangent.z, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_tangent, index).w, tangent.w, 0.001);

			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_texture_coordinate_0, index).x, data.x,
				0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_texture_coordinate_0, index).y, data.y,
				0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_texture_coordinate_0, index).z, data.z,
				0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_texture_coordinate_0, index).w, data.w,
				0.001);

			++index;
		}
	}
}

BOOST_AUTO_TEST_CASE(add_plane_build_tangent_gram_schmidth_ortho_use_restart)
{
	boost::mt19937 rng;
	boost::uniform_int<Sint32> range(-16777216, 16777216);
	boost::variate_generator<boost::mt19937&, boost::uniform_int<Sint32> >
		random_int(rng, range);
	el::MeshDataToolSharedPtr mesh_data_tool;
	glm::vec4 normal, tangent, data;
	glm::vec3 min, max;
	glm::vec2 size, uv;
	el::Uint32Vector indices;
	el::VertexSemanticTypeSet semantics;
	el::Uint32 vertex_count, index_count, i, index, x, y, tile_size;
	el::PrimitiveType primitive;
	bool use_restart_index, split;

	split = true;
	tile_size = 16;
	use_restart_index = true;

	vertex_count = tile_size + 1;
	vertex_count *= tile_size + 1;

	if (use_restart_index)
	{
		primitive = el::pt_triangle_fan;
	}
	else
	{
		primitive = el::pt_triangles;
	}

	el::IndexBuilder::build_plane_indices(indices, tile_size,
		use_restart_index, 0, split);

	index_count = indices.size();

	semantics.insert(el::vst_position);
	semantics.insert(el::vst_texture_coordinate_0);
	semantics.insert(el::vst_normal);
	semantics.insert(el::vst_tangent);

	mesh_data_tool = boost::make_shared<el::MeshDataTool>(
		el::String("tangents_gram_schmidth"), vertex_count,
		index_count, 1, semantics, primitive, use_restart_index, false);

	for (i = 0; i < index_count; ++i)
	{
		mesh_data_tool->set_index_data(i, indices[i]);
	}

	normal = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	tangent = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

	index = 0;

	for (y = 0; y <= tile_size; ++y)
	{
		for (x = 0; x <= tile_size; ++x)
		{
			uv = glm::vec2(x, y) / static_cast<float>(tile_size);

			data = glm::vec4(uv, 0.0f, 1.0f);

			for (i = 0; i < 4; ++i)
			{
				tangent[i] = random_int() * 0.001f;
			}

			mesh_data_tool->set_vertex_data(el::vst_position, index,
				data);
			mesh_data_tool->set_vertex_data(el::vst_normal, index,
				normal);
			mesh_data_tool->set_vertex_data(el::vst_tangent, index,
				tangent);
			mesh_data_tool->set_vertex_data(
				el::vst_texture_coordinate_0, index, data);

			++index;
		}
	}

	min = glm::vec3(0.0f, 0.0f, -0.01f);
	max = glm::vec3(1.0f, 1.0f, 0.0f);

	mesh_data_tool->set_sub_mesh_data(0, el::SubMesh(el::BoundingBox(min,
		max), 0, index_count, 0, vertex_count - 1));

	BOOST_CHECK_NO_THROW(mesh_data_tool->build_tangent(false, false,
		true));

	tangent = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

	BOOST_CHECK_EQUAL(mesh_data_tool->get_name(), "tangents_gram_schmidth");
	BOOST_CHECK_EQUAL(mesh_data_tool->get_vertex_count(), vertex_count);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_index_count(), index_count);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_count(), 1);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_restart_index(),
		std::numeric_limits<Uint32>::max());
	BOOST_CHECK_EQUAL(mesh_data_tool->get_primitive(), primitive);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_use_restart_index(),
		use_restart_index);

	BOOST_CHECK_EQUAL(mesh_data_tool->get_vertex_count(), vertex_count);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_index_count(), index_count);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_count(), 1);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_restart_index(),
		std::numeric_limits<Uint32>::max());
	BOOST_CHECK_EQUAL(mesh_data_tool->get_primitive(), primitive);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_use_restart_index(),
		use_restart_index);

	for (i = 0; i < index_count; ++i)
	{
		BOOST_CHECK_EQUAL(mesh_data_tool->get_index_data(i),
			indices[i]);
	}

	index = 0;

	for (y = 0; y <= tile_size; ++y)
	{
		for (x = 0; x <= tile_size; ++x)
		{
			uv = glm::vec2(x, y) / static_cast<float>(tile_size);

			data = glm::vec4(uv, 0.0f, 1.0f);

			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_position, index).x, data.x, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_position, index).y, data.y, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_position, index).z, data.z, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_position, index).w, data.w, 0.001);

			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_normal, index).x, normal.x, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_normal, index).y, normal.y, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_normal, index).z, normal.z, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_normal, index).w, normal.w, 0.001);

			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_tangent, index).x, tangent.x, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_tangent, index).y, tangent.y, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_tangent, index).z, tangent.z, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_tangent, index).w, tangent.w, 0.001);

			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_texture_coordinate_0, index).x, data.x,
				0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_texture_coordinate_0, index).y, data.y,
				0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_texture_coordinate_0, index).z, data.z,
				0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_texture_coordinate_0, index).w, data.w,
				0.001);

			++index;
		}
	}
}

BOOST_AUTO_TEST_CASE(add_plane_disable_use_restart)
{
	el::MeshDataToolSharedPtr mesh_data_tool;
	glm::vec4 normal, tangent, data;
	glm::vec3 min, max;
	glm::vec2 size, uv;
	el::Uint32Vector indices;
	el::VertexSemanticTypeSet semantics;
	el::Uint32 vertex_count, index_count, i, index, x, y, tile_size;
	el::PrimitiveType primitive;
	bool use_restart_index, split;

	split = true;
	tile_size = 16;
	use_restart_index = true;

	vertex_count = tile_size + 1;
	vertex_count *= tile_size + 1;

	if (use_restart_index)
	{
		primitive = el::pt_triangle_fan;
	}
	else
	{
		primitive = el::pt_triangles;
	}

	el::IndexBuilder::build_plane_indices(indices, tile_size,
		use_restart_index, 0, split);

	index_count = indices.size();

	semantics.insert(el::vst_position);
	semantics.insert(el::vst_texture_coordinate_0);
	semantics.insert(el::vst_normal);
	semantics.insert(el::vst_tangent);

	mesh_data_tool = boost::make_shared<el::MeshDataTool>(
		el::String("plane"), vertex_count, index_count, 1, semantics,
		primitive, use_restart_index, false);

	for (i = 0; i < index_count; ++i)
	{
		mesh_data_tool->set_index_data(i, indices[i]);
	}

	normal = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	tangent = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

	index = 0;

	for (y = 0; y <= tile_size; ++y)
	{
		for (x = 0; x <= tile_size; ++x)
		{
			uv = glm::vec2(x, y) / static_cast<float>(tile_size);

			data = glm::vec4(uv, 0.0f, 1.0f);

			mesh_data_tool->set_vertex_data(el::vst_position, index,
				data);
			mesh_data_tool->set_vertex_data(el::vst_normal, index,
				normal);
			mesh_data_tool->set_vertex_data(el::vst_tangent, index,
				tangent);
			mesh_data_tool->set_vertex_data(
				el::vst_texture_coordinate_0, index, data);

			++index;
		}
	}

	min = glm::vec3(0.0f, 0.0f, -0.01f);
	max = glm::vec3(1.0f, 1.0f, 0.0f);

	mesh_data_tool->set_sub_mesh_data(0, el::SubMesh(el::BoundingBox(min,
		max), 0, index_count, 0, vertex_count - 1));

	BOOST_CHECK_EQUAL(mesh_data_tool->get_name(), "plane");
	BOOST_CHECK_EQUAL(mesh_data_tool->get_vertex_count(), vertex_count);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_index_count(), index_count);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_count(), 1);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_restart_index(),
		std::numeric_limits<Uint32>::max());
	BOOST_CHECK_EQUAL(mesh_data_tool->get_primitive(), primitive);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_use_restart_index(),
		use_restart_index);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_use_simd(), false);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_min()[0], min[0], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_min()[1], min[1], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_min()[2], min[2], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_max()[0], max[0], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_max()[1], max[1], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_max()[2], max[2], 0.001);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_data(0).get_offset(), 0);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_data(0).get_count(),
		index_count);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_data(0).get_min_vertex(),
		0);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_data(0).get_max_vertex(),
		vertex_count - 1);

	for (i = 0; i < index_count; ++i)
	{
		BOOST_CHECK_EQUAL(mesh_data_tool->get_index_data(i),
			indices[i]);
	}

	index = 0;

	for (y = 0; y <= tile_size; ++y)
	{
		for (x = 0; x <= tile_size; ++x)
		{
			uv = glm::vec2(x, y) / static_cast<float>(tile_size);

			data = glm::vec4(uv, 0.0f, 1.0f);

			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_position, index).x, data.x, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_position, index).y, data.y, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_position, index).z, data.z, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_position, index).w, data.w, 0.001);

			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_normal, index).x, normal.x, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_normal, index).y, normal.y, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_normal, index).z, normal.z, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_normal, index).w, normal.w, 0.001);

			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_tangent, index).x, tangent.x, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_tangent, index).y, tangent.y, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_tangent, index).z, tangent.z, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_tangent, index).w, tangent.w, 0.001);

			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_texture_coordinate_0, index).x, data.x,
				0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_texture_coordinate_0, index).y, data.y,
				0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_texture_coordinate_0, index).z, data.z,
				0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_texture_coordinate_0, index).w, data.w,
				0.001);

			++index;
		}
	}

	indices.clear();

	use_restart_index = false;

	if (use_restart_index)
	{
		primitive = el::pt_triangle_fan;
	}
	else
	{
		primitive = el::pt_triangles;
	}

	el::IndexBuilder::build_plane_indices(indices, tile_size,
		use_restart_index, 0, split);

	index_count = indices.size();

	BOOST_CHECK_NO_THROW(mesh_data_tool->disable_restart_index());

	BOOST_CHECK_EQUAL(mesh_data_tool->get_name(), "plane");
	BOOST_CHECK_EQUAL(mesh_data_tool->get_vertex_count(), vertex_count);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_index_count(), index_count);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_count(), 1);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_restart_index(),
		std::numeric_limits<Uint32>::max());
	BOOST_CHECK_EQUAL(mesh_data_tool->get_primitive(), primitive);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_use_restart_index(),
		use_restart_index);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_use_simd(), false);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_min()[0], min[0], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_min()[1], min[1], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_min()[2], min[2], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_max()[0], max[0], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_max()[1], max[1], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_max()[2], max[2], 0.001);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_data(0).get_offset(), 0);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_data(0).get_count(),
		index_count);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_data(0).get_min_vertex(),
		0);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_data(0).get_max_vertex(),
		vertex_count - 1);

	for (i = 0; i < index_count; ++i)
	{
		BOOST_CHECK_EQUAL(mesh_data_tool->get_index_data(i),
			indices[i]);
	}

	index = 0;

	for (y = 0; y <= tile_size; ++y)
	{
		for (x = 0; x <= tile_size; ++x)
		{
			uv = glm::vec2(x, y) / static_cast<float>(tile_size);

			data = glm::vec4(uv, 0.0f, 1.0f);

			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_position, index).x, data.x, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_position, index).y, data.y, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_position, index).z, data.z, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_position, index).w, data.w, 0.001);

			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_normal, index).x, normal.x, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_normal, index).y, normal.y, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_normal, index).z, normal.z, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_normal, index).w, normal.w, 0.001);

			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_tangent, index).x, tangent.x, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_tangent, index).y, tangent.y, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_tangent, index).z, tangent.z, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_tangent, index).w, tangent.w, 0.001);

			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_texture_coordinate_0, index).x, data.x,
				0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_texture_coordinate_0, index).y, data.y,
				0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_texture_coordinate_0, index).z, data.z,
				0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_texture_coordinate_0, index).w, data.w,
				0.001);

			++index;
		}
	}
}

BOOST_AUTO_TEST_CASE(add_plane_build_bounding_box_use_restart)
{
	el::MeshDataToolSharedPtr mesh_data_tool;
	glm::vec4 normal, tangent, data;
	glm::vec3 min, max;
	glm::vec2 size, uv;
	el::Uint32Vector indices;
	el::VertexSemanticTypeSet semantics;
	el::Uint32 vertex_count, index_count, i, index, x, y, tile_size;
	el::PrimitiveType primitive;
	bool use_restart_index, split;

	split = true;
	tile_size = 16;
	use_restart_index = true;

	vertex_count = tile_size + 1;
	vertex_count *= tile_size + 1;

	if (use_restart_index)
	{
		primitive = el::pt_triangle_fan;
	}
	else
	{
		primitive = el::pt_triangles;
	}

	el::IndexBuilder::build_plane_indices(indices, tile_size,
		use_restart_index, 0, split);

	index_count = indices.size();

	semantics.insert(el::vst_position);
	semantics.insert(el::vst_texture_coordinate_0);
	semantics.insert(el::vst_normal);
	semantics.insert(el::vst_tangent);

	mesh_data_tool = boost::make_shared<el::MeshDataTool>(
		el::String("plane"), vertex_count, index_count, 1, semantics,
		primitive, use_restart_index, false);

	for (i = 0; i < index_count; ++i)
	{
		mesh_data_tool->set_index_data(i, indices[i]);
	}

	normal = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	tangent = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

	index = 0;

	for (y = 0; y <= tile_size; ++y)
	{
		for (x = 0; x <= tile_size; ++x)
		{
			uv = glm::vec2(x, y) / static_cast<float>(tile_size);

			data = glm::vec4(uv, 0.0f, 1.0f);

			mesh_data_tool->set_vertex_data(el::vst_position, index,
				data);
			mesh_data_tool->set_vertex_data(el::vst_normal, index,
				normal);
			mesh_data_tool->set_vertex_data(el::vst_tangent, index,
				tangent);
			mesh_data_tool->set_vertex_data(
				el::vst_texture_coordinate_0, index, data);

			++index;
		}
	}

	min = glm::vec3(235.0f, 12.0f, -0.541f);
	max = glm::vec3(189097.0f, 23541.0f, 230.0f);

	mesh_data_tool->set_sub_mesh_data(0, el::SubMesh(el::BoundingBox(min,
		max), 0, index_count, 0, vertex_count - 1));

	BOOST_CHECK_EQUAL(mesh_data_tool->get_name(), "plane");
	BOOST_CHECK_EQUAL(mesh_data_tool->get_vertex_count(), vertex_count);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_index_count(), index_count);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_count(), 1);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_restart_index(),
		std::numeric_limits<Uint32>::max());
	BOOST_CHECK_EQUAL(mesh_data_tool->get_primitive(), primitive);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_use_restart_index(),
		use_restart_index);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_use_simd(), false);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_min()[0], min[0], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_min()[1], min[1], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_min()[2], min[2], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_max()[0], max[0], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_max()[1], max[1], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_max()[2], max[2], 0.001);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_data(0).get_offset(), 0);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_data(0).get_count(),
		index_count);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_data(0).get_min_vertex(),
		0);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_data(0).get_max_vertex(),
		vertex_count - 1);

	for (i = 0; i < index_count; ++i)
	{
		BOOST_CHECK_EQUAL(mesh_data_tool->get_index_data(i),
			indices[i]);
	}

	index = 0;

	for (y = 0; y <= tile_size; ++y)
	{
		for (x = 0; x <= tile_size; ++x)
		{
			uv = glm::vec2(x, y) / static_cast<float>(tile_size);

			data = glm::vec4(uv, 0.0f, 1.0f);

			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_position, index).x, data.x, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_position, index).y, data.y, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_position, index).z, data.z, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_position, index).w, data.w, 0.001);

			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_normal, index).x, normal.x, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_normal, index).y, normal.y, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_normal, index).z, normal.z, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_normal, index).w, normal.w, 0.001);

			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_tangent, index).x, tangent.x, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_tangent, index).y, tangent.y, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_tangent, index).z, tangent.z, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_tangent, index).w, tangent.w, 0.001);

			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_texture_coordinate_0, index).x, data.x,
				0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_texture_coordinate_0, index).y, data.y,
				0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_texture_coordinate_0, index).z, data.z,
				0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_texture_coordinate_0, index).w, data.w,
				0.001);

			++index;
		}
	}

	min = glm::vec3(0.0f, 0.0f, 0.0f);
	max = glm::vec3(1.0f, 1.0f, 0.0f);

	BOOST_CHECK_NO_THROW(mesh_data_tool->update_sub_meshs_bounding_box());

	BOOST_CHECK_EQUAL(mesh_data_tool->get_name(), "plane");
	BOOST_CHECK_EQUAL(mesh_data_tool->get_vertex_count(), vertex_count);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_index_count(), index_count);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_count(), 1);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_restart_index(),
		std::numeric_limits<Uint32>::max());
	BOOST_CHECK_EQUAL(mesh_data_tool->get_primitive(), primitive);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_use_restart_index(),
		use_restart_index);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_use_simd(), false);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_min()[0], min[0], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_min()[1], min[1], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_min()[2], min[2], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_max()[0], max[0], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_max()[1], max[1], 0.001);
	BOOST_CHECK_CLOSE(mesh_data_tool->get_sub_mesh_data(0).get_bounding_box(
		).get_max()[2], max[2], 0.001);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_data(0).get_offset(), 0);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_data(0).get_count(),
		index_count);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_data(0).get_min_vertex(),
		0);
	BOOST_CHECK_EQUAL(mesh_data_tool->get_sub_mesh_data(0).get_max_vertex(),
		vertex_count - 1);

	for (i = 0; i < index_count; ++i)
	{
		BOOST_CHECK_EQUAL(mesh_data_tool->get_index_data(i),
			indices[i]);
	}

	index = 0;

	for (y = 0; y <= tile_size; ++y)
	{
		for (x = 0; x <= tile_size; ++x)
		{
			uv = glm::vec2(x, y) / static_cast<float>(tile_size);

			data = glm::vec4(uv, 0.0f, 1.0f);

			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_position, index).x, data.x, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_position, index).y, data.y, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_position, index).z, data.z, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_position, index).w, data.w, 0.001);

			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_normal, index).x, normal.x, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_normal, index).y, normal.y, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_normal, index).z, normal.z, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_normal, index).w, normal.w, 0.001);

			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_tangent, index).x, tangent.x, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_tangent, index).y, tangent.y, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_tangent, index).z, tangent.z, 0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_tangent, index).w, tangent.w, 0.001);

			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_texture_coordinate_0, index).x, data.x,
				0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_texture_coordinate_0, index).y, data.y,
				0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_texture_coordinate_0, index).z, data.z,
				0.001);
			BOOST_CHECK_CLOSE(mesh_data_tool->get_vertex_data(
				el::vst_texture_coordinate_0, index).w, data.w,
				0.001);

			++index;
		}
	}
}
