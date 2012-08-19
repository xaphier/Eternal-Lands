/****************************************************************************
 *            lightvisitor.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "lightvisitor.hpp"
#include "light.hpp"
#include "frustum.hpp"
#include "renderlightdata.hpp"

namespace eternal_lands
{

	namespace
	{

		class LightSort
		{
			private:
				const glm::vec3 m_position;

			public:
				inline LightSort(const glm::vec3 &position):
					m_position(position)
				{
				}

				inline ~LightSort() noexcept
				{
				}

				inline bool operator()(
					const RenderLightData &l0,
					const RenderLightData &l1) noexcept
				{
					float dist0, dist1;

					dist0 = glm::distance2(
						l0.get_light()->get_position(),
						m_position);
					dist1 = glm::distance2(
						l1.get_light()->get_position(),
						m_position);

					return dist0 < dist1;
				}

		};

	}

	LightVisitor::LightVisitor()
	{
	}

	LightVisitor::~LightVisitor() noexcept
	{
	}

	void LightVisitor::operator()(
		const BoundedObjectSharedPtr &bounded_object)
	{
		LightSharedPtr light;

		light = boost::dynamic_pointer_cast<Light>(bounded_object);

		assert(light.get() != nullptr);

		m_lights.push_back(RenderLightData(light));
	}

	void LightVisitor::sort(const glm::vec3 &position)
	{
		std::sort(m_lights.begin(), m_lights.end(),
			LightSort(position));
	}

	void LightVisitor::update_camera(const glm::vec3 &camera)
	{
		BOOST_FOREACH(RenderLightData &light, m_lights)
		{
			light.set_camera_inside(glm::distance(camera,
				light.get_light()->get_position()) <
				(light.get_light()->get_radius() + 1.2f));
		}
	}

}
