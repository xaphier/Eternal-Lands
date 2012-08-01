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

		const Uint16 sub_frustum_count = 4;
		const Uint16 sub_frustum_planes_count = 7;
		const Uint16 sub_frustum_intersect_offset = sub_frustum_count *
			sub_frustum_planes_count;

		const PlanesMask sub_frustum_masks[sub_frustum_count] =
		{
			0x1000007F,
			0x20003F80,
			0x401FC000,
			0x8FE00000
		};

		const PlanesMask sub_frustum_planes_masks[sub_frustum_count] =
		{
			0x0000007F,
			0x00003F80,
			0x001FC000,
			0x0FE00000
		};

		const PlanesMask sub_frustum_6_planes_masks[sub_frustum_count] =
		{
			0x0000003F,
			0x00001F80,
			0x000FC000,
			0x07E00000
		};

		const PlanesMask all_sub_frustum_planes_mask = 0x0FFFFFFF;

	}

	Frustum::Frustum()
	{
		assert(m_planes_mask.size() == (sub_frustum_count *
			(sub_frustum_planes_count + 1)));
		assert(m_planes_mask.size() == (m_planes.size() +
			sub_frustum_count));

		m_planes_mask.reset();
	}

	Frustum::Frustum(const Frustum &frustum, const Uint16 index)
	{
		Uint32 i, offset;

		assert(m_planes_mask.size() == (sub_frustum_count *
			(sub_frustum_planes_count + 1)));
		assert(m_planes_mask.size() == (m_planes.size() +
			sub_frustum_count));
		assert((sub_frustum_masks[0] & all_sub_frustum_planes_mask) ==
			sub_frustum_planes_masks[0]);
		assert((sub_frustum_masks[1] & all_sub_frustum_planes_mask) ==
			sub_frustum_planes_masks[1]);
		assert((sub_frustum_masks[2] & all_sub_frustum_planes_mask) ==
			sub_frustum_planes_masks[2]);
		assert((sub_frustum_masks[3] & all_sub_frustum_planes_mask) ==
			sub_frustum_planes_masks[3]);

		m_planes_mask.reset();

		RANGE_CECK_MAX(index, sub_frustum_count, UTF8("index too big"));

		offset = index * sub_frustum_planes_count;

		for (i = 0; i < sub_frustum_planes_count; ++i)
		{
			m_planes[i] = frustum.m_planes[i + offset];
			m_planes_mask[i] = frustum.m_planes_mask[i + offset];
		}
	}

	Frustum::Frustum(const glm::mat4 &matrix)
	{
		assert(m_planes_mask.size() == (sub_frustum_count *
			(sub_frustum_planes_count + 1)));
		assert(m_planes_mask.size() == (m_planes.size() +
			sub_frustum_count));
		assert((sub_frustum_masks[0] & all_sub_frustum_planes_mask) ==
			sub_frustum_planes_masks[0]);
		assert((sub_frustum_masks[1] & all_sub_frustum_planes_mask) ==
			sub_frustum_planes_masks[1]);
		assert((sub_frustum_masks[2] & all_sub_frustum_planes_mask) ==
			sub_frustum_planes_masks[2]);
		assert((sub_frustum_masks[3] & all_sub_frustum_planes_mask) ==
			sub_frustum_planes_masks[3]);

		m_planes_mask.reset();

		set_sub_frustum(matrix, 0);
	}

	Frustum::Frustum(const Mat4x4Vector &matrices)
	{
		Uint16 i, count;

		assert(m_planes_mask.size() == (sub_frustum_count *
			(sub_frustum_planes_count + 1)));
		assert(m_planes_mask.size() == (m_planes.size() +
			sub_frustum_count));
		assert((sub_frustum_masks[0] & all_sub_frustum_planes_mask) ==
			sub_frustum_planes_masks[0]);
		assert((sub_frustum_masks[1] & all_sub_frustum_planes_mask) ==
			sub_frustum_planes_masks[1]);
		assert((sub_frustum_masks[2] & all_sub_frustum_planes_mask) ==
			sub_frustum_planes_masks[2]);
		assert((sub_frustum_masks[3] & all_sub_frustum_planes_mask) ==
			sub_frustum_planes_masks[3]);

		m_planes_mask.reset();

		count = sub_frustum_count;

		if (count > matrices.size())
		{
			count = matrices.size();
		}

		for (i = 0; i < count; ++i)
		{
			set_sub_frustum(matrices[i], i);
		}
	}

	Frustum::Frustum(const BoundingBox &box)
	{
		assert(m_planes_mask.size() == (sub_frustum_count *
			(sub_frustum_planes_count + 1)));
		assert(m_planes_mask.size() == (m_planes.size() +
			sub_frustum_count));
		assert((sub_frustum_masks[0] & all_sub_frustum_planes_mask) ==
			sub_frustum_planes_masks[0]);
		assert((sub_frustum_masks[1] & all_sub_frustum_planes_mask) ==
			sub_frustum_planes_masks[1]);
		assert((sub_frustum_masks[2] & all_sub_frustum_planes_mask) ==
			sub_frustum_planes_masks[2]);
		assert((sub_frustum_masks[3] & all_sub_frustum_planes_mask) ==
			sub_frustum_planes_masks[3]);

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

	void Frustum::set_plane(const glm::mat4 &matrix, const PlaneType plane,
		const Uint16 sub_frustum_index)
	{
		Uint16 sub_frustum_offset;

		assert(sub_frustum_index < sub_frustum_count);
		assert(plane < sub_frustum_planes_count);

		sub_frustum_offset = sub_frustum_index *
			sub_frustum_planes_count;

		m_planes[plane + sub_frustum_offset] = get_plane(matrix, plane);

		m_planes_mask[plane + sub_frustum_offset] = true;
	}

	void Frustum::set_sub_frustum(const glm::mat4 &matrix,
		const Uint16 sub_frustum_index)
	{
		set_plane(matrix, pt_left, sub_frustum_index);
		set_plane(matrix, pt_right, sub_frustum_index);
		set_plane(matrix, pt_top, sub_frustum_index);
		set_plane(matrix, pt_bottom, sub_frustum_index);
		set_plane(matrix, pt_near, sub_frustum_index);
		set_plane(matrix, pt_far, sub_frustum_index);
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

	void Frustum::intersect(const BoundingBox &box,
		const PlanesMask in_mask, const Uint16 sub_frustum_index,
		PlanesMask &out_mask) const
	{
		PlanesMask mask;
		Uint16 i, index, offset;

		assert((get_planes_mask() & in_mask) ==
			(in_mask & all_sub_frustum_planes_mask));

		if ((in_mask &
			sub_frustum_planes_masks[sub_frustum_index]).none())
		{
			out_mask |=
				in_mask & sub_frustum_masks[sub_frustum_index];

			return;
		}

		offset = sub_frustum_index * sub_frustum_planes_count;

		for (i = 0; i < sub_frustum_planes_count; ++i)
		{
			index = i + offset;

			if (in_mask[index])
			{
#ifdef	USE_SSE2_
				switch (get_plane(index).intersect_sse2(box))
#else	/* USE_SSE2 */
				switch (get_plane(index).intersect(box))
#endif	/* USE_SSE2 */
				{
					case it_outside:
						return;
					case it_intersect:
						mask[index] = true;
						break;
					case it_inside:
						break;
				}
			}
		}

		index = sub_frustum_index + sub_frustum_intersect_offset;

		mask[index] = mask.none();

		out_mask |= mask;
	}

	IntersectionType Frustum::intersect(const BoundingBox &box,
		const PlanesMask in_mask, PlanesMask &out_mask) const
	{
		Uint16 i;

		out_mask.reset();

		for (i = 0; i < sub_frustum_count; ++i)
		{
			intersect(box, in_mask, i, out_mask);
		}

		if (out_mask.none())
		{
			return it_outside;
		}

		if ((out_mask & all_sub_frustum_planes_mask).none())
		{
			return it_inside;
		}

		return it_intersect;
	}

	SubFrustumsMask Frustum::intersect_sub_frustums(const BoundingBox &box,
		const PlanesMask in_mask) const
	{
		PlanesMask mask;
		Uint16 i;

		for (i = 0; i < sub_frustum_count; ++i)
		{
			intersect(box, in_mask, i, mask);
		}

		return get_sub_frustums_mask(mask);
	}

	SubFrustumsMask Frustum::get_sub_frustums_mask(const PlanesMask mask)
		const
	{
		SubFrustumsMask result;
		Uint16 i;

		for (i = 0; i < sub_frustum_count; ++i)
		{
			result[i] = (mask & sub_frustum_masks[i]).any();
		}

		return result;
	}

	Vec3Array8 Frustum::get_corner_points(const Uint16 sub_frustum_index)
		const
	{
		Vec3Array8 result;
		Uint16Array3 offsets;
		Uint16 i, j, offset, count;

		offset = sub_frustum_index * sub_frustum_planes_count;
		count = result.size();

		for (i = 0; i < count; ++i)
		{
			for (j = 0; j < 3; ++j)
			{
				offsets[j] = offset + j * 2;

				if (std::bitset<3>(i)[j])
				{
					offsets[j]++;
				}

				assert(offsets[j] >= offset);
				assert(offsets[j] < (offset +
					sub_frustum_planes_count));
			}

			Plane::intersection_point(m_planes[offsets[0]],
				m_planes[offsets[1]], m_planes[offsets[2]],
				result[i]);
		}

		return result;
	}

	SubFrustumsBoundingBoxes Frustum::get_bounding_boxes() const
	{
		SubFrustumsBoundingBoxes result;
		Uint16 i;

		for (i = 0; i < sub_frustum_count; ++i)
		{
			if ((get_planes_mask() & sub_frustum_6_planes_masks[i])
				== sub_frustum_6_planes_masks[i])
			{
				result[i] = BoundingBox(get_corner_points(i));
			}
		}

		return result;
	}

	SubFrustumsConerPoints Frustum::get_corner_points() const
	{
		SubFrustumsConerPoints result;
		Uint16 i;

		for (i = 0; i < sub_frustum_count; ++i)
		{
			if ((get_planes_mask() & sub_frustum_6_planes_masks[i])
				== sub_frustum_6_planes_masks[i])
			{
				result[i] = get_corner_points(i);
			}
		}

		return result;
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

	PlaneVector Frustum::get_planes(const Uint16 sub_frustum) const
	{
		PlaneVector result;
		Uint16 i, index;

		for (i = 0; i < sub_frustum_planes_count; ++i)
		{
			index = sub_frustum * sub_frustum_planes_count + i;

			if (get_planes_mask()[index])
			{
				result.push_back(get_plane(index));
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

	OutStream& operator<<(OutStream &str, const PlaneType value)
	{
		str << Frustum::get_str(value);

		return str;
	}

}
