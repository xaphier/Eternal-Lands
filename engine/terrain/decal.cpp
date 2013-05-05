/****************************************************************************
 *            decal.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "decal.hpp"
#include "decaldescription.hpp"
#include "texturecache.hpp"
#include "texture.hpp"

namespace eternal_lands
{

	Decal::Decal(const TextureCacheSharedPtr &texture_cache,
		const DecalDescription &description):
		m_transform_matrix(description.get_transform_matrix())
	{
		m_texture = texture_cache->get_texture(
			description.get_texture(), true, false);
	}

	Decal::~Decal() noexcept
	{
	}

	glm::vec4 Decal::get_decal_rect() const
	{
		glm::vec2 min, max, tmp;

		tmp = glm::vec3(-1.0f, -1.0f, 1.0f) * get_transform_matrix();
		min = tmp;
		max = tmp;

		tmp = glm::vec3( 1.0f, -1.0f, 1.0f) * get_transform_matrix();
		min = glm::min(min, tmp);
		max = glm::max(max, tmp);

		tmp = glm::vec3(-1.0f,  1.0f, 1.0f) * get_transform_matrix();
		min = glm::min(min, tmp);
		max = glm::max(max, tmp);

		tmp = glm::vec3( 1.0f,  1.0f, 1.0f) * get_transform_matrix();
		min = glm::min(min, tmp);
		max = glm::max(max, tmp);

		return glm::vec4(min, max);
	}

}
