/****************************************************************************
 *            light.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "light.hpp"

namespace eternal_lands
{

	Light::Light(const LightData &light_data): m_light_data(light_data),
		m_inv_sqr_radius(0.0f)
	{
		set_inv_sqr_radius(get_radius());
		update_bounding_box();
	}

	Light::~Light() noexcept
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
