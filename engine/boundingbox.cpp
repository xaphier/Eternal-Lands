/****************************************************************************
 *            boundingbox.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "boundingbox.hpp"
#include "exceptions.hpp"

namespace eternal_lands
{

	BoundingBox::BoundingBox()
	{
		set_empty();
	}

	BoundingBox::BoundingBox(const glm::vec3 &min, const glm::vec3 &max)
	{
		set_min_max(min, max);
	}

	BoundingBox::BoundingBox(const Vec3Array8 &points)
	{
		glm::vec3 min, max;
		Uint16 i;

		min = points[0];
		max = points[0];

		for (i = 1; i < 8; ++i)
		{
			min = glm::min(min, points[i]);
			max = glm::max(max, points[i]);
		}

		set_min_max(min, max);
	}

	BoundingBox::~BoundingBox() throw()
	{
	}

	void BoundingBox::set_min_max(const glm::vec3 &min,
		const glm::vec3 &max)
	{
		if (!glm::all(glm::lessThanEqual(min, max)))
		{
			set_empty();

			return;
		}

		set_center((min + max) * 0.5f);
		set_half_size((max - min) * 0.5f);
	}

	void BoundingBox::set_half_size(const glm::vec3 &half_size)
	{
		if (!glm::all(glm::greaterThanEqual(half_size,
			glm::vec3(0.0f))))
		{
			set_empty();

			return;
		}

		m_half_size = half_size;
	}

	void BoundingBox::merge(const BoundingBox &box)
	{
		glm::vec3 min, max;

		min = glm::min(get_min(), box.get_min());
		max = glm::max(get_max(), box.get_max());

		set_min_max(min, max);
	}

	void BoundingBox::clamp(const BoundingBox &box)
	{
		glm::vec3 min, max;

		min = glm::max(get_min(), box.get_min());
		max = glm::min(get_max(), box.get_max());

		set_min_max(min, max);
	}

	float BoundingBox::overlap(const BoundingBox &box) const
	{
		glm::vec3 t, t0, t1;

		t0 = glm::abs(get_center() - box.get_center());
		t1 = get_half_size() + box.get_half_size();

		t = glm::max(t1 - t0, glm::vec3(0.0f));

		return t[0] * t[1] * t[2];
	}

	bool BoundingBox::contains(const BoundingBox &box) const
	{
		glm::vec3 dist;

		dist = glm::abs(get_center() - box.get_center()) +
			box.get_half_size();

		return glm::all(glm::lessThanEqual(dist, get_half_size() *
			1.0001f));
	}

	float BoundingBox::enlargement(const BoundingBox &box) const
	{
		glm::vec3 min, max, size;

		min = glm::min(get_min(), box.get_min());
		max = glm::max(get_max(), box.get_max());

		size = max - min;

		return size[0] * size[1] * size[2] - get_half_size()[0] *
			get_half_size()[1] * get_half_size()[2] * 8.0f;
	}

	BoundingBox BoundingBox::transform(const glm::mat4x3 &matrix) const
	{
		glm::vec4 point;
		glm::vec3 min, max, tmp;
		Uint32 i;

		min = glm::vec3(std::numeric_limits<float>::max());
		max = glm::vec3(-std::numeric_limits<float>::max());

		for (i = 0; i < 8; ++i)
		{
			point = glm::vec4(get_point(std::bitset<3>(i)), 1.0f);
			tmp = matrix * point;

			min = glm::min(min, tmp);
			max = glm::max(max, tmp);
		}

		return BoundingBox(min, max);
	}

	BoundingBox BoundingBox::transform(const glm::mat4x4 &matrix) const
	{
		glm::vec4 point;
		glm::vec3 min, max, tmp;
		Uint32 i;

		min = glm::vec3(std::numeric_limits<float>::max());
		max = glm::vec3(-std::numeric_limits<float>::max());

		for (i = 0; i < 8; ++i)
		{
			point = glm::vec4(get_point(std::bitset<3>(i)), 1.0f);
			point = matrix * point;
			tmp = glm::vec3(point) / point.w;

			min = glm::min(min, tmp);
			max = glm::max(max, tmp);
		}

		return BoundingBox(min, max);
	}

	bool BoundingBox::intersect(const glm::mat4 &view_matrix,
		const glm::mat4 &projection_matrix, const glm::ivec4 &viewport,
		const glm::vec2 &min, const glm::vec2 &max) const
	{
		glm::vec2 point;
		Uint32 i;
		BitSet4 set;

		for (i = 0; i < 8; ++i)
		{
			point = glm::vec2(glm::project(get_point(
				std::bitset<3>(i)), view_matrix,
				projection_matrix, viewport));

			set[0] = set[0] || point.x < max.x;
			set[1] = set[1] || point.x > min.x;
			set[2] = set[2] || point.y < max.y;
			set[3] = set[3] || point.y > min.y;

			if (set.count() == 4)
			{
				return true;
			}
		}

		return false;
	}

	float BoundingBox::sqr_distance(const glm::vec3 &point) const
	{
		glm::vec3 dist;

		dist = glm::abs(get_center() - point);
		dist -= get_half_size();
		dist = glm::max(glm::vec3(0.0f), dist);

		return glm::dot(dist, dist);
	}

	float BoundingBox::distance(const glm::vec3 &point) const
	{
		return std::sqrt(sqr_distance(point));
	}

	OutStream& operator<<(OutStream &str, const BoundingBox &value)
	{
		str << "<" << value.get_min()[0] << ", " << value.get_min()[1];
		str << ", " << value.get_min()[2] << ">";
		str << "<" << value.get_max()[0] << ", " << value.get_max()[1];
		str << ", " << value.get_max()[2] << ">";

		return str;
	}

}