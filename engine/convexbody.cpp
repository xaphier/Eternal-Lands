/****************************************************************************
 *            convexbody.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "convexbody.hpp"
#include "exceptions.hpp"
#include "frustum.hpp"
#include "boundingbox.hpp"
#include "ray.hpp"

namespace eternal_lands
{

	ConvexBody::ConvexBody()
	{
	}

	ConvexBody::ConvexBody(const Frustum &frustum, const Uint16 index)
	{
		// ordering of the points:
		// near (0-3), far (4-7); each (top-right, top-left, bottom-left, bottom-right)
		//	   5-----4
		//	  /|    /|
		//	 / |   / |
		//	1-----0  |
		//	|  6--|--7
		//	| /   | /
		//	|/    |/
		//	2-----3
		Vec3Array8 corner_points;
		Polygon polygon;

		corner_points = frustum.get_corner_points(index);
		/// update vertices: near, far, left, right, bottom, top; fill in ccw

		// near
		polygon.clear();
		polygon.add_vertex(corner_points[0]);
		polygon.add_vertex(corner_points[1]);
		polygon.add_vertex(corner_points[2]);
		polygon.add_vertex(corner_points[3]);
		add_polygon(polygon);

		// far
		polygon.clear();
		polygon.add_vertex(corner_points[5]);
		polygon.add_vertex(corner_points[4]);
		polygon.add_vertex(corner_points[7]);
		polygon.add_vertex(corner_points[6]);
		add_polygon(polygon);

		// left
		polygon.clear();
		polygon.add_vertex(corner_points[5]);
		polygon.add_vertex(corner_points[6]);
		polygon.add_vertex(corner_points[2]);
		polygon.add_vertex(corner_points[1]);
		add_polygon(polygon); 

		// right
		polygon.clear();
		polygon.add_vertex(corner_points[4]);
		polygon.add_vertex(corner_points[0]);
		polygon.add_vertex(corner_points[3]);
		polygon.add_vertex(corner_points[7]);
		add_polygon(polygon); 

		// bottom
		polygon.clear();
		polygon.add_vertex(corner_points[6]);
		polygon.add_vertex(corner_points[7]);
		polygon.add_vertex(corner_points[3]);
		polygon.add_vertex(corner_points[2]);
		add_polygon(polygon); 

		// top
		polygon.clear();
		polygon.add_vertex(corner_points[4]);
		polygon.add_vertex(corner_points[5]);
		polygon.add_vertex(corner_points[1]);
		polygon.add_vertex(corner_points[0]);
		add_polygon(polygon); 
	}

	ConvexBody::ConvexBody(const BoundingBox &box)
	{
		// ordering of the AAB points:
		//	1-----2
		//     /|    /|
		//    / |   / |
		//   5-----4  |
		//   |  0--|--3
		//   | /   | /
		//   |/    |/
		//   6-----7
		Polygon polygon;
		glm::vec3 min, max, vertex;

		min = box.get_min();
		max = box.get_max();

		vertex = min;

		// far
		polygon.clear();
		polygon.add_vertex(vertex); // 0 
		vertex.y = max.y;
		polygon.add_vertex(vertex); // 1
		vertex.x = max.x;
		polygon.add_vertex(vertex); // 2
		vertex.y = min.y;
		polygon.add_vertex(vertex); // 3
		add_polygon(polygon);

		// right
		polygon.clear();
		polygon.add_vertex(vertex); // 3
		vertex.y = max.y;
		polygon.add_vertex(vertex); // 2
		vertex.z = max.z;
		polygon.add_vertex(vertex); // 4
		vertex.y = min.y;
		polygon.add_vertex(vertex); // 7
		add_polygon(polygon); 

		// near
		polygon.clear();
		polygon.add_vertex(vertex); // 7
		vertex.y = max.y;
		polygon.add_vertex(vertex); // 4
		vertex.x = min.x;
		polygon.add_vertex(vertex); // 5
		vertex.y = min.y;
		polygon.add_vertex(vertex); // 6
		add_polygon(polygon);

		// left
		polygon.clear();
		polygon.add_vertex(vertex); // 6
		vertex.y = max.y;
		polygon.add_vertex(vertex); // 5
		vertex.z = min.z;
		polygon.add_vertex(vertex); // 1
		vertex.y = min.y;
		polygon.add_vertex(vertex); // 0
		add_polygon(polygon); 

		// bottom
		polygon.clear();
		polygon.add_vertex(vertex); // 0 
		vertex.x = max.x;
		polygon.add_vertex(vertex); // 3
		vertex.z = max.z;
		polygon.add_vertex(vertex); // 7 
		vertex.x = min.x;
		polygon.add_vertex(vertex); // 6
		add_polygon(polygon);

		// top
		polygon.clear();
		vertex = max;
		polygon.add_vertex(vertex); // 4
		vertex.z = min.z;
		polygon.add_vertex(vertex); // 2
		vertex.x = min.x;
		polygon.add_vertex(vertex); // 1
		vertex.z = max.z;
		polygon.add_vertex(vertex); // 5
		add_polygon(polygon);
	}

	ConvexBody::~ConvexBody() throw()
	{
	}

	void ConvexBody::clip(const Frustum &frustum, const Uint16 index)
	{
		PlaneVector planes;

		planes = frustum.get_planes(index);

		BOOST_FOREACH(const Plane &plane, planes)
		{
			clip(plane);
		}
	}

	void ConvexBody::clip(const BoundingBox &box)
	{
		PlaneVector planes;

		planes = Frustum::get_planes(box);

		BOOST_FOREACH(const Plane &plane, planes)
		{
			clip(plane);
		}
	}

	void ConvexBody::clip(const Plane &plane)
	{
		ConvexBody current;
		Polygon polygon, new_polygon, intersect_polygon, closing;
		Ray ray;
		std::vector<bool> insides;
		EdgeMap intersection_edges;
		EdgeMap::iterator it;
		glm::vec3 first_vertex, second_vertex, next_vertex, cross;
		glm::vec3 current_vertex, intersect_vertex, direction;
		float t;
		Uint32 i, j, k, count, vertex_count;
		bool frontside;

		count = current.get_polygon_count();

		for (i = 0; i < count; ++i)
		{
			vertex_count = current.get_polygon(i
				).get_vertex_count();

			if (vertex_count < 3)
			{
				continue;
			}

			insides.clear();
			insides.reserve(vertex_count);

			for (j = 0; j < vertex_count; ++j)
			{
				insides.push_back(plane.intersect(
					polygon.get_vertex(j)) == it_inside);
			}

			new_polygon.clear();
			intersect_polygon.clear();

			for (j = 0; j < vertex_count; ++j)
			{
				k = (j + 1) % vertex_count;

				current_vertex = current.get_polygon(i
					).get_vertex(j);
				next_vertex = current.get_polygon(i
					).get_vertex(k);

				if (insides[j] && insides[k])
				{
					new_polygon.add_vertex(next_vertex);

					continue;
				}

				if (insides[j] && !insides[k])
				{
					direction = glm::normalize(
						current_vertex - next_vertex);

					ray = Ray(next_vertex, direction);

					if (plane.intersect(ray, t))
					{
						intersect_vertex =
							ray.get_point(t);
						new_polygon.add_vertex(
							intersect_vertex);
						intersect_polygon.add_vertex(
							intersect_vertex);
					}

					continue;
				}

				if (!insides[j] && insides[k])
				{
					direction = glm::normalize(
						current_vertex - next_vertex);
					ray = Ray(current_vertex, direction);

					if (plane.intersect(ray, t))
					{
						intersect_vertex =
							ray.get_point(t);
						new_polygon.add_vertex(
							intersect_vertex);
						intersect_polygon.add_vertex(
							intersect_vertex);
					}

					new_polygon.add_vertex(next_vertex);

					continue;
				}				
			}

			if (new_polygon.get_vertex_count() >= 3)
			{
				new_polygon.remove_duplicates();

				if (new_polygon.get_vertex_count() >= 3)
				{
					add_polygon(new_polygon);
				}
			}

			if (intersect_polygon.get_vertex_count() == 2 )
			{
				intersection_edges.insert(Edge(
					intersect_polygon.get_vertex(0),
					intersect_polygon.get_vertex(1)));
			}
		}

		if (intersection_edges.size() >= 3 )
		{
			closing.clear();

			it = intersection_edges.begin();

			first_vertex  = it->first;
			second_vertex = it->second;

			intersection_edges.erase(it);

			if (find_and_erase_edge_pair(second_vertex,
				intersection_edges, next_vertex))
			{
				cross = glm::cross(first_vertex - second_vertex,
					next_vertex - second_vertex);
				frontside = glm::dot(plane.get_normal(), cross)
					> (1.0f - epsilon);

				if (frontside)
				{
					closing.add_vertex(next_vertex);
					closing.add_vertex(second_vertex);
					closing.add_vertex(first_vertex);
				}
				else
				{
					closing.add_vertex(first_vertex);
					closing.add_vertex(second_vertex);
					closing.add_vertex(next_vertex);
				}

				while (!intersection_edges.empty())
				{
					if (find_and_erase_edge_pair(
						current_vertex,
						intersection_edges,
						next_vertex))
					{
						if (!intersection_edges.empty(
							))
						{
							current_vertex =
								next_vertex;
							closing.add_vertex(
								next_vertex);
						}
					}
					else
					{
						break;
					}

				}

				add_polygon(closing);
			}
		}
	}

	bool ConvexBody::find_and_erase_edge_pair(const glm::vec3 &vertex,
		EdgeMap& intersection_edges, glm::vec3 &next)
	{
		EdgeMap::iterator it, end;

		end = intersection_edges.end();

		for (it = intersection_edges.begin(); it != end; ++it)
		{
			if (glm::all(glm::equalEpsilon(it->first, vertex,
				epsilon)))
			{
				next = it->second;

				intersection_edges.erase(it);

				return true;
			}

			if (glm::all(glm::equalEpsilon(it->second, vertex,
				epsilon)))
			{
				next = it->first;

				intersection_edges.erase(it);

				return true;
			}
		}

		return false;
	}

	void ConvexBody::transform(const glm::mat4x3 &matrix)
	{
		BOOST_FOREACH(Polygon &polygon, m_polygons)
		{
			polygon.transform(matrix);
		}
	}

	ConvexBody ConvexBody::get_transformed(const glm::mat4x3 &matrix) const
	{
		ConvexBody result;

		result = *this;

		result.transform(matrix);

		return result;
	}

	BoundingBox ConvexBody::get_bounding_box() const
	{
		glm::vec3 min, max;

		min = glm::vec3(std::numeric_limits<float>::max());
		max = glm::vec3(-std::numeric_limits<float>::max());

		BOOST_FOREACH(const Polygon &polygon, m_polygons)
		{
			BOOST_FOREACH(const glm::vec3 &point,
				polygon.get_vertices())
			{
				min = glm::min(min, point);
				max = glm::max(max, point);
			}
		}

		return BoundingBox(min, max);
	}

}
