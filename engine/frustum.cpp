/****************************************************************************
 *            frustum.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "frustum.hpp"
#include "exceptions.hpp"

namespace eternal_lands
{

	namespace
	{

		const String plane_type_names[] =
		{
			String(UTF8("right")),
			String(UTF8("left")),
			String(UTF8("top")),
			String(UTF8("bottom")),
			String(UTF8("near")),
			String(UTF8("far"))
		};

		const Uint16 plane_type_names_count =
			sizeof(plane_type_names) / sizeof(String);

	}

	Frustum::Frustum()
	{
		m_planes_mask.reset();
	}

	Frustum::Frustum(const glm::mat4 &matrix)
	{
		m_planes_mask.reset();

		set_plane(matrix, pt_left);
		set_plane(matrix, pt_right);
		set_plane(matrix, pt_top);
		set_plane(matrix, pt_bottom);
		set_plane(matrix, pt_near);
		set_plane(matrix, pt_far);
	}

	Frustum::Frustum(const BoundingBox &box)
	{
		build_frustum(box);
	}

	Frustum::~Frustum() noexcept
	{
	}

	Plane Frustum::get_plane(const glm::mat4 &matrix,
		const PlaneType plane)
	{
		switch (plane)
		{
			case pt_left:
				return Plane(glm::vec4(
					matrix[0][3] + matrix[0][0],
					matrix[1][3] + matrix[1][0],
					matrix[2][3] + matrix[2][0],
					matrix[3][3] + matrix[3][0]));
			case pt_right:
				return Plane(glm::vec4(
					matrix[0][3] - matrix[0][0],
					matrix[1][3] - matrix[1][0],
					matrix[2][3] - matrix[2][0],
					matrix[3][3] - matrix[3][0]));
			case pt_top:
				return Plane(glm::vec4(
					matrix[0][3] - matrix[0][1],
					matrix[1][3] - matrix[1][1],
					matrix[2][3] - matrix[2][1],
					matrix[3][3] - matrix[3][1]));
			case pt_bottom:
				return Plane(glm::vec4(
					matrix[0][3] + matrix[0][1],
					matrix[1][3] + matrix[1][1],
					matrix[2][3] + matrix[2][1],
					matrix[3][3] + matrix[3][1]));
			case pt_near:
				return Plane(glm::vec4(
					matrix[0][3] + matrix[0][2],
					matrix[1][3] + matrix[1][2],
					matrix[2][3] + matrix[2][2],
					matrix[3][3] + matrix[3][2]));
			case pt_far:
				return Plane(glm::vec4(
					matrix[0][3] - matrix[0][2],
					matrix[1][3] - matrix[1][2],
					matrix[2][3] - matrix[2][2],
					matrix[3][3] - matrix[3][2]));
		}
		VALUE_NOT_IN_SWITCH(plane, UTF8("PlaneType"));
	}

	void Frustum::set_plane(const glm::mat4 &matrix, const PlaneType plane)
	{
		m_planes[plane] = get_plane(matrix, plane);

		m_planes_mask[plane] = true;
	}

	void Frustum::build_frustum(const BoundingBox &box)
	{
		glm::vec3 point, dir;

		point = box.get_point(0);

		dir = box.get_point(1) - point;
		m_planes[pt_right] = Plane(point, dir);

		dir = box.get_point(2) - point;
		m_planes[pt_top] = Plane(point, dir);

		dir = box.get_point(4) - point;
		m_planes[pt_near] = Plane(point, dir);

		point = box.get_point(7);

		dir = box.get_point(6) - point;
		m_planes[pt_left] = Plane(point, dir);

		dir = box.get_point(5) - point;
		m_planes[pt_bottom] = Plane(point, dir);

		dir = box.get_point(3) - point;
		m_planes[pt_far] = Plane(point, dir);

		m_planes_mask.reset();
		m_planes_mask[pt_right] = true;
		m_planes_mask[pt_left] = true;
		m_planes_mask[pt_top] = true;
		m_planes_mask[pt_bottom] = true;
		m_planes_mask[pt_near] = true;
		m_planes_mask[pt_far] = true;
	}

	IntersectionType Frustum::intersect(const BoundingBox &box,
		const BitSet64 in_mask, BitSet64 &out_mask) const
	{
		IntersectionType result;
		Uint16 i, count;

		count = m_planes.size();

		result = it_inside;

		for (i = 0; i < count; ++i)
		{
			if (in_mask[i])
			{
				switch (get_plane(i).intersect(box))
				{
					case it_outside:
						return it_outside;
					case it_intersect:
						out_mask[i] = true;
						result = it_intersect;
						break;
					case it_inside:
						break;
				}
			}
		}

		return result;
	}

	IntersectionType Frustum::intersect(const glm::vec3 &point,
		const BitSet64 in_mask, BitSet64 &out_mask) const
	{
		IntersectionType result;
		Uint16 i, count;

		count = m_planes.size();

		result = it_inside;

		for (i = 0; i < count; ++i)
		{
			if (in_mask[i])
			{
				switch (get_plane(i).intersect(point))
				{
					case it_outside:
						return it_outside;
					case it_intersect:
						out_mask[i] = true;
						result = it_intersect;
						break;
					case it_inside:
						break;
				}
			}
		}

		return result;
	}

	Vec3Array8 Frustum::get_corner_points() const
	{
		Vec3Array8 result;
		Uint16Array3 offsets;
		Uint16 i, j;

		for (i = 0; i < 8; ++i)
		{
			for (j = 0; j < 3; ++j)
			{
				offsets[j] = j * 2;

				if (std::bitset<3>(i)[j])
				{
					offsets[j]++;
				}
			}

			Plane::intersection_point(m_planes[offsets[0]],
				m_planes[offsets[1]], m_planes[offsets[2]],
				result[i]);
		}

		return result;
	}

	BoundingBox Frustum::get_bounding_box() const
	{
		return BoundingBox(get_corner_points());;
	}

	PlaneVector Frustum::get_planes(const BoundingBox &box)
	{
		PlaneVector result;
		glm::vec3 point, dir;

		point = box.get_point(0);

		dir = box.get_point(1) - point;
		result.push_back(Plane(point, dir));

		dir = box.get_point(2) - point;
		result.push_back(Plane(point, dir));

		dir = box.get_point(4) - point;
		result.push_back(Plane(point, dir));

		point = box.get_point(7);

		dir = box.get_point(6) - point;
		result.push_back(Plane(point, dir));

		dir = box.get_point(5) - point;
		result.push_back(Plane(point, dir));

		dir = box.get_point(3) - point;
		result.push_back(Plane(point, dir));

		return result;
	}

	PlaneVector Frustum::get_planes() const
	{
		PlaneVector result;
		Uint16 i, count;

		count = m_planes.size();

		for (i = 0; i < count; ++i)
		{
			if (get_planes_mask()[i])
			{
				result.push_back(get_plane(i));
			}
		}

		return result;
	}

	void Frustum::transform(const glm::mat4x3 &matrix)
	{
		Uint16 i;

		for (i = 0; i < get_planes_mask().size(); ++i)
		{
			if (get_planes_mask()[i])
			{
				m_planes[i].transform(matrix);
			}
		}
	}

	Frustum Frustum::get_transformed(const glm::mat4x3 &matrix) const
	{
		Frustum result;

		result.transform(matrix);

		return result;
	}

	const String Frustum::get_str(const PlaneType plane_type)
	{
		if (plane_type_names_count <= plane_type)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					plane_type_names_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					plane_type))
				<< boost::errinfo_type_info_name(UTF8(
					"PlaneType")));
		}

		return plane_type_names[plane_type];
	}

	OutStream &operator<<(OutStream &str, const PlaneType value)
	{
		str << Frustum::get_str(value);

		return str;
	}

}
