/****************************************************************************
 *            lightvisitor.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "lightvisitor.hpp"
#include "light.hpp"

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

				inline ~LightSort() throw()
				{
				}

				inline bool operator()(const LightSharedPtr &l0,
					const LightSharedPtr &l1)
				{
					float dist0, dist1;

					dist0 = glm::distance2(
						l0->get_position(), m_position);
					dist1 = glm::distance2(
						l1->get_position(), m_position);

					return dist0 < dist1;
				}

		};

	}

	LightVisitor::LightVisitor()
	{
	}

	LightVisitor::~LightVisitor() throw()
	{
	}

	void LightVisitor::operator()(
		const BoundedObjectSharedPtr &bounded_object,
		const SubFrustumsMask mask)
	{
		LightSharedPtr light;

		light = boost::dynamic_pointer_cast<Light>(bounded_object);

		assert(light.get() != nullptr);

		m_lights.push_back(light);
	}

	void LightVisitor::sort(const glm::vec3 &position)
	{
		std::sort(m_lights.begin(), m_lights.end(),
			LightSort(position));
	}

}
