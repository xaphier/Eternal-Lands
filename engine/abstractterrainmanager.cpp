/****************************************************************************
 *            abstractterrainmanager.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "abstractterrainmanager.hpp"

namespace eternal_lands
{

	namespace
	{

		const glm::vec3 vector_scale = glm::vec3(8.0f, 8.0f, 32.0f);

		const glm::vec3 vector_min = glm::vec3(-vector_scale.x,
			-vector_scale.y, 0.0f);

		const glm::vec3 vector_max = vector_scale;

		//	2.0f, 2.0f, 4.0f * 1023.0f / 4095.0f, 3.0f / 4095.0f
		const glm::vec4 vector_scale_rgb10_a2 = glm::vec4(
			vector_scale.x * 2.0f, vector_scale.y * 2.0f,
			(vector_scale.z * 4.0f * 1023.0f) / 4095.0f,
			(vector_scale.z * 3.0f) / 4095.0f);

		//	-1.0f, -1.0f
		const glm::vec2 vector_offset_rgb10_a2 = glm::vec2(
			-vector_scale.x, -vector_scale.y);

		//	2.0f, 2.0f, 4.0f * 1023.0f / 4095.0f, 3.0f / 4095.0f
		const glm::vec4 vector_rgb10_a2 = glm::vec4(2.0f, 2.0f,
			(4.0f * 1023.0f) / 4095.0f, 3.0f / 4095.0f);

	}

	AbstractTerrainManager::AbstractTerrainManager()
	{
	}

	AbstractTerrainManager::~AbstractTerrainManager() noexcept
	{
	}

	const glm::vec3 &AbstractTerrainManager::get_vector_scale() noexcept
	{
		return vector_scale;
	}

	const glm::vec3 &AbstractTerrainManager::get_vector_min() noexcept
	{
		return vector_min;
	}

	const glm::vec3 &AbstractTerrainManager::get_vector_max() noexcept
	{
		return vector_max;
	}

	const glm::vec4 &AbstractTerrainManager::get_vector_scale_rgb10_a2()
		noexcept
	{
		return vector_scale_rgb10_a2;
	}

	const glm::vec2 &AbstractTerrainManager::get_vector_offset_rgb10_a2()
		noexcept
	{
		return vector_offset_rgb10_a2;
	}

	const glm::vec4 &AbstractTerrainManager::get_vector_rgb10_a2() noexcept
	{
		return vector_rgb10_a2;
	}

}
