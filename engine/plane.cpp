/****************************************************************************
 *            plane.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "plane.hpp"
#include "exceptions.hpp"

namespace eternal_lands
{

	namespace
	{

		const String intersection_type_names[] =
		{
			String(UTF8("inside")),
			String(UTF8("outside")),
			String(UTF8("intersect"))
		};

		const Uint32 intersection_type_names_count =
			sizeof(intersection_type_names) / sizeof(String);

	}

	Plane::Plane()
	{
		m_normal_and_distance = glm::vec4(0.0f);
	}

	Plane::Plane(const glm::vec3 &point, const glm::vec3 &dir)
	{
		set_normal(dir);
		recalculate_distance(point);
	}

	Plane::Plane(const glm::vec3 &dir, const float distance)
	{
		set_normal(dir);
		set_distance(distance);
	}

	Plane::Plane(const glm::vec4 &data)
	{
		set_data(data);
	}

	Plane::~Plane() throw()
	{
	}

	Plane Plane::transform(const glm::mat4x3 &matrix) const
	{
		Plane result;

		result.set_normal(glm::mat3(matrix) * get_normal());

		result.recalculate_distance(matrix *
			glm::vec4(get_member_point(), 1.0f));

		return result;
	}

	bool Plane::intersection_point(const Plane p0, const Plane p1,
		const Plane p2, glm::vec3 &point)
	{
		glm::vec3 temp;
		float det;

		det = glm::dot(p0.get_normal(), glm::cross(p1.get_normal(),
			p2.get_normal()));

		if (std::abs(det) <= 0.0001f)
		{
			return false;
		}

		temp = -p0.get_distance() * glm::cross(p1.get_normal(),
			p2.get_normal());

		temp += -p1.get_distance() * glm::cross(p2.get_normal(),
			p0.get_normal());

		temp += -p2.get_distance() * glm::cross(p0.get_normal(),
			p1.get_normal());

		point = temp / det;

		return true;
	}
#ifdef	USE_SSE2_
	IntersectionType Plane::intersect_sse2(const BoundingBox &box) const
	{
		__m128 c_xy, h_xy, c_z, h_z, min_max_xy, p, p_xy, p_z, p_w;
		__m128 min_z, max_z, tmp_xy, tmp0, tmp1;
		Uint16 result;

		c_xy = (__m128)_mm_load_sd((double*)glm::value_ptr(
			box.get_center()));
		h_xy = (__m128)_mm_load_sd((double*)glm::value_ptr(
			box.get_half_size()));
		c_z = _mm_load_ss(&(box.get_center()[2]));
		h_z = _mm_load_ss(&(box.get_half_size()[2]));
		p = _mm_loadu_ps(glm::value_ptr(m_normal_and_distance));
		c_xy = _mm_shuffle_ps(c_xy, c_xy, _MM_SHUFFLE(1, 0, 1, 0));
		h_xy = _mm_shuffle_ps(h_xy, h_xy, _MM_SHUFFLE(1, 0, 1, 0));
		c_z = _mm_shuffle_ps(c_z, c_z, _MM_SHUFFLE(0, 0, 0, 0));
		h_z = _mm_shuffle_ps(h_z, h_z, _MM_SHUFFLE(0, 0, 0, 0));
		p_xy = (__m128)_mm_shuffle_epi32((__m128i)p, _MM_SHUFFLE(1, 0, 1, 0));
		p_z = (__m128)_mm_shuffle_epi32((__m128i)p, _MM_SHUFFLE(2, 2, 2, 2));
		p_w = (__m128)_mm_shuffle_epi32((__m128i)p, _MM_SHUFFLE(3, 3, 3, 3));
		h_xy = _mm_xor_ps(h_xy, _mm_set_ps(-0.0f, -0.0f, 0.0f, 0.0f));
		min_max_xy = _mm_add_ps(c_xy, h_xy);
		min_max_xy = _mm_mul_ps(min_max_xy, p_xy);
		tmp_xy = (__m128)_mm_shuffle_epi32((__m128i)min_max_xy,
			_MM_SHUFFLE(0, 3, 2, 1));
		min_max_xy = _mm_add_ps(min_max_xy, tmp_xy);
		min_max_xy = _mm_add_ps(min_max_xy, p_w);
		min_z = _mm_sub_ps(c_z, h_z);
		max_z = _mm_add_ps(c_z, h_z);
		min_z = _mm_mul_ps(min_z, p_z);
		max_z = _mm_mul_ps(max_z, p_z);
		tmp0 = _mm_add_ps(min_max_xy, min_z);
		tmp1 = _mm_add_ps(min_max_xy, max_z);
		tmp0 = _mm_cmpge_ps(tmp0, _mm_setzero_ps());
		tmp1 = _mm_cmpge_ps(tmp1, _mm_setzero_ps());
		tmp0 = (__m128)_mm_packs_epi32((__m128i)tmp0, (__m128i)tmp1);
		result = _mm_movemask_epi8((__m128i)tmp0);

		if (result == 0x0000)
		{
			return it_outside;
		}

		if (result == 0xFFFF)
		{
			return it_inside;
		}

		return it_intersect;
	}
#endif	/* USE_SSE2 */
	const String &Plane::get_str(const IntersectionType intersection)
	{
		if (intersection_type_names_count <= intersection)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					intersection_type_names_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					intersection))
				<< boost::errinfo_type_info_name(
					UTF8("IntersectionType")));
		}

		return intersection_type_names[intersection];
	}

	IntersectionType Plane::get_intersection_type(const String &str)
	{
		Uint32 i;
		IntersectionType intersection_type;

		for (i = 0; i < intersection_type_names_count; ++i)
		{
			intersection_type = static_cast<IntersectionType>(i);

			if (str == get_str(intersection_type))
			{
				return intersection_type;
			}
		}

		EL_THROW_EXCEPTION(InvalidParameterException()
			<< errinfo_string_value(str)
			<< boost::errinfo_type_info_name(UTF8(
				"IntersectionType")));
	}

	OutStream& operator<<(OutStream &str, const IntersectionType value)
	{
		str << Plane::get_str(value);

		return str;
	}

	InStream& operator>>(InStream &str, IntersectionType &value)
	{
		StringType string;

		str >> string;

		value = Plane::get_intersection_type(String(string));

		return str;
	}

}
