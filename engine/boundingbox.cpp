/****************************************************************************
 *            boundingbox.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "boundingbox.hpp"
#include "exceptions.hpp"
#include "transformation.hpp"

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

	BoundingBox::~BoundingBox() noexcept
	{
	}

	BoundingBox BoundingBox::transform(
		const Transformation &transformation) const noexcept
	{
		glm::vec3 min, max, point;
		Uint16 i;

		min = glm::vec3(std::numeric_limits<float>::max());
		max = glm::vec3(-std::numeric_limits<float>::max());

		for (i = 0; i < 8; ++i)
		{
			point = transformation.transform_point(
				get_point(std::bitset<3>(i)));

			min = glm::min(min, point);
			max = glm::max(max, point);
		}

		return BoundingBox(min, max);
	}

	BoundingBox BoundingBox::transform(const glm::mat4x4 &matrix) const
		noexcept
	{
		glm::vec4 point;
		glm::vec3 min, max, tmp;
		Uint16 i;

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
		const glm::vec2 &min, const glm::vec2 &max) const noexcept
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

	float BoundingBox::distance(const glm::vec3 &point) const noexcept
	{
		glm::vec3 tmp;

		tmp = point;
		tmp = glm::min(tmp, get_max());
		tmp = glm::max(tmp, get_min());

		return glm::distance(tmp, point);
	}

	OutStream &operator<<(OutStream &str, const BoundingBox &value)
	{
		str << "<" << value.get_min()[0] << ", " << value.get_min()[1];
		str << ", " << value.get_min()[2] << ">";
		str << "<" << value.get_max()[0] << ", " << value.get_max()[1];
		str << ", " << value.get_max()[2] << ">";

		return str;
	}

}
