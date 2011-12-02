/****************************************************************************
 *            light.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "light.hpp"

namespace eternal_lands
{

	Light::Light(const glm::vec3 &position, const glm::vec3 &color,
		const float ambient, const float radius, const Uint32 id):
		m_position(position), m_color(color), m_ambient(ambient),
		m_radius(radius), m_inv_sqr_radius(0.0f), m_id(id)
	{
		set_inv_sqr_radius(radius);
		update_bounding_box();
	}

	Light::~Light() throw()
	{
	}

	void Light::update_bounding_box(const float scale)
	{
		BoundingBox bounding_box;

		bounding_box.set_center(get_position());
		bounding_box.set_half_size(glm::vec3(get_radius()));
		bounding_box.scale(scale);

		set_bounding_box(bounding_box);
	}

	bool Light::intersect(const BoundingBox &box) const
	{
		glm::vec3 tmp;
		float total_distance;

		tmp = glm::abs(get_position() - box.get_center());
		tmp -= box.get_half_size();
		tmp = glm::max(tmp, 0.0f);

		total_distance = glm::length2(tmp);

		return total_distance <= (get_radius() * get_radius());
	}

}

