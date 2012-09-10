/****************************************************************************
 *            abstractterrainmanager.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "abstractterrainmanager.hpp"
#include "shader/shadersourceterrain.hpp"
#include "material.hpp"
#include "materialdescription.hpp"
#include "materialbuilder.hpp"
#include "texturecache.hpp"


#include "texture.hpp"

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

	MaterialSharedPtr AbstractTerrainManager::build_clipmap_material(
		const MaterialBuilderSharedPtr &material_builder,
		const EffectCacheSharedPtr &effect_cache,
		const TextureCacheSharedPtr &texture_cache,
		const StringVector &albedo_maps,
		const StringVector &specular_maps,
		const ImageSharedPtrVector &blend_images,
		const bool texture_arrays) const
	{
		MaterialDescription material_description;
		MaterialSharedPtr result;
		TextureSharedPtr texture, dudv_texture, displacement_texture;
		TextureSharedPtr normal_texture;
		Uint32 i, count;

		material_description.set_name(String(UTF8("terrain")));
		material_description.set_effect(String(UTF8("terrain")));

		count = albedo_maps.size();
		count = std::min(count,
			ShaderSourceTerrain::get_non_array_sampler_count());

		if (texture_arrays)
		{
			count = 0;
		}

		for (i = 0; i < count; ++i)
		{
			material_description.set_texture(albedo_maps[i],
				ShaderSourceTerrain::get_albedo_sampler(i));
		}

		if (texture_arrays)
		{
			count = 0;
		}

		count = specular_maps.size();
		count = std::min(count,
			ShaderSourceTerrain::get_non_array_sampler_count());

		for (i = 0; i < count; ++i)
		{
			material_description.set_texture(specular_maps[i],
				ShaderSourceTerrain::get_specular_sampler(i));
		}

		result = material_builder->get_material(material_description);

		if ((albedo_maps.size() > 0) && texture_arrays)
		{
			texture = texture_cache->get_texture_array(
				albedo_maps,
				String(UTF8("terrain-albedo")));

			result->set_texture(texture,
				ShaderSourceTerrain::get_albedo_sampler(0));
		}

		if ((specular_maps.size() > 0) && texture_arrays)
		{
			texture = texture_cache->get_texture_array(
				specular_maps,
				String(UTF8("terrain-specular")));

			result->set_texture(texture,
				ShaderSourceTerrain::get_specular_sampler(0));
		}

		if ((blend_images.size() > 0) && texture_arrays)
		{
			texture = texture_cache->get_texture_array(
				blend_images,
				String(UTF8("terrain-blend")));

			result->set_texture(texture,
				ShaderSourceTerrain::get_blend_sampler());
		}

		if ((blend_images.size() > 0) && !texture_arrays)
		{
			texture = texture_cache->get_texture(
				blend_images[0]);

			result->set_texture(texture,
				ShaderSourceTerrain::get_blend_sampler());
		}

		dudv_texture = get_dudv_texture();
		normal_texture = get_normal_texture();
		displacement_texture = get_displacement_texture();

		if (dudv_texture.get() != nullptr)
		{
			result->set_texture(dudv_texture,
				ShaderSourceTerrain::get_dudv_sampler());
		}

		if (displacement_texture.get() != nullptr)
		{
			result->set_texture(displacement_texture,
				ShaderSourceTerrain::get_displacement_sampler(
					));
		}

		if (normal_texture.get() != nullptr)
		{
			result->set_texture(normal_texture,
				ShaderSourceTerrain::get_normal_sampler());
		}

		return result;
	}

}
