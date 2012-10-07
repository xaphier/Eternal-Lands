/****************************************************************************
 *            abstractterrain.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "abstractterrain.hpp"
#include "shader/shadersourceterrain.hpp"
#include "material.hpp"
#include "materialdescription.hpp"
#include "materialbuilder.hpp"
#include "texturecache.hpp"
#include "globalvars.hpp"
#include "materialcache.hpp"
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

	AbstractTerrain::AbstractTerrain(
		const GlobalVarsSharedPtr &global_vars,
		const EffectCacheSharedPtr &effect_cache,
		const MaterialBuilderSharedPtr &material_builder,
		const MaterialCacheSharedPtr &material_cache,
		const String &material, const String &effect):
		m_global_vars(global_vars),
		m_low_quality_terrain(global_vars->get_low_quality_terrain())
	{
		MaterialDescription material_description;

		m_terrain_material = material_cache->get_material(material);

		material_description.set_name(
			String(UTF8("clipmap-terrain-material")));
		material_description.set_effect(effect);

		m_clipmap_terrain_material = material_builder->get_material(
			material_description);
	}

	AbstractTerrain::~AbstractTerrain() noexcept
	{
	}

	const glm::vec3 &AbstractTerrain::get_vector_scale() noexcept
	{
		return vector_scale;
	}

	const glm::vec3 &AbstractTerrain::get_vector_min() noexcept
	{
		return vector_min;
	}

	const glm::vec3 &AbstractTerrain::get_vector_max() noexcept
	{
		return vector_max;
	}

	const glm::vec4 &AbstractTerrain::get_vector_scale_rgb10_a2()
		noexcept
	{
		return vector_scale_rgb10_a2;
	}

	const glm::vec2 &AbstractTerrain::get_vector_offset_rgb10_a2()
		noexcept
	{
		return vector_offset_rgb10_a2;
	}

	const glm::vec4 &AbstractTerrain::get_vector_rgb10_a2() noexcept
	{
		return vector_rgb10_a2;
	}

	void AbstractTerrain::set_clipmap_terrain_texture(
		const TextureSharedPtr &texture)
	{
		m_terrain_material->set_texture(texture, spt_effect_0);
	}

	void AbstractTerrain::update_clipmap_terrain_material()
	{
		TextureSharedPtr dudv_texture, displacement_texture;
		TextureSharedPtr normal_texture;

		dudv_texture = get_dudv_texture();
		normal_texture = get_normal_texture();
		displacement_texture = get_displacement_texture();

		if (dudv_texture.get() != nullptr)
		{
			m_clipmap_terrain_material->set_texture(dudv_texture,
				ShaderSourceTerrain::get_dudv_sampler());
		}

		if (displacement_texture.get() != nullptr)
		{
			m_clipmap_terrain_material->set_texture(
				displacement_texture,
				ShaderSourceTerrain::get_displacement_sampler(
					));
		}

		if (normal_texture.get() != nullptr)
		{
			m_clipmap_terrain_material->set_texture(normal_texture,
				ShaderSourceTerrain::get_normal_sampler());
		}
	}

	void AbstractTerrain::set_geometry_maps(
		const ImageSharedPtr &displacement_map,
		const ImageSharedPtr &normal_map,
		const ImageSharedPtr &dudv_map)
	{
		do_set_geometry_maps(displacement_map, normal_map, dudv_map);

		update_clipmap_terrain_material();
	}

	void AbstractTerrain::set_blend_map(
		const ImageSharedPtr &blend_map,
		const TextureCacheSharedPtr &texture_cache)
	{
		TextureSharedPtr texture;

		if (get_global_vars()->get_opengl_3_0())
		{
			texture = texture_cache->get_texture_array(
				blend_map, String(UTF8("terrain-blend")));
		}
		else
		{
			texture = texture_cache->get_texture(blend_map);
		}

		texture->set_wrap_s(twt_clamp);
		texture->set_wrap_t(twt_clamp);

		texture->set_image(blend_map);

		m_clipmap_terrain_material->set_texture(texture,
			ShaderSourceTerrain::get_blend_sampler());
	}

	void AbstractTerrain::update_geometry_maps(
		const ImageUpdate &displacement_map,
		const ImageUpdate &normal_map, const ImageUpdate &dudv_map)
	{
		do_update_geometry_maps(displacement_map, normal_map, dudv_map);

		update_clipmap_terrain_material();
	}

	void AbstractTerrain::update_blend_map(
		const ImageUpdate &blend_image)
	{
		m_clipmap_terrain_material->get_texture(
			ShaderSourceTerrain::get_blend_sampler())->sub_texture(
			blend_image);
	}

	void AbstractTerrain::set_texture_maps(
		const StringVector &albedo_maps,
		const StringVector &specular_maps,
		const TextureCacheSharedPtr &texture_cache)
	{
		set_albedo_maps(albedo_maps, texture_cache);
		set_specular_maps(specular_maps, texture_cache);
	}

	void AbstractTerrain::set_albedo_maps(const StringVector &albedo_maps,
		const TextureCacheSharedPtr &texture_cache)
	{
		TextureSharedPtr texture;
		Uint32 i, count;

		count = ShaderSourceTerrain::get_non_array_sampler_count();

		for (i = 0; i < count; ++i)
		{
			m_clipmap_terrain_material->set_texture(texture,
				ShaderSourceTerrain::get_albedo_sampler(i));
		}

		if (albedo_maps.size() == 0)
		{
			return;
		}

		if (get_global_vars()->get_opengl_3_0())
		{
			texture = texture_cache->get_texture_array(
				albedo_maps,
				String(UTF8("terrain-albedo")));

			m_clipmap_terrain_material->set_texture(texture,
				ShaderSourceTerrain::get_albedo_sampler(0));

			return;
		}

		count = albedo_maps.size();
		count = std::min(count,
			ShaderSourceTerrain::get_non_array_sampler_count());

		for (i = 0; i < count; ++i)
		{
			texture = texture_cache->get_texture(albedo_maps[i]);

			m_clipmap_terrain_material->set_texture(texture,
				ShaderSourceTerrain::get_albedo_sampler(i));
		}
	}

	void AbstractTerrain::set_specular_maps(
		const StringVector &specular_maps,
		const TextureCacheSharedPtr &texture_cache)
	{
		TextureSharedPtr texture;
		Uint32 i, count;

		count = ShaderSourceTerrain::get_non_array_sampler_count();

		for (i = 0; i < count; ++i)
		{
			m_clipmap_terrain_material->set_texture(texture,
				ShaderSourceTerrain::get_specular_sampler(i));
		}

		if (specular_maps.size() == 0)
		{
			return;
		}

		if (get_global_vars()->get_opengl_3_0())
		{
			texture = texture_cache->get_texture_array(
				specular_maps,
				String(UTF8("terrain-specular")));

			m_clipmap_terrain_material->set_texture(texture,
				ShaderSourceTerrain::get_specular_sampler(0));

			return;
		}

		count = specular_maps.size();
		count = std::min(count,
			ShaderSourceTerrain::get_non_array_sampler_count());

		for (i = 0; i < count; ++i)
		{
			texture = texture_cache->get_texture(specular_maps[i]);

			m_clipmap_terrain_material->set_texture(texture,
				ShaderSourceTerrain::get_specular_sampler(i));
		}
	}

	void AbstractTerrain::set_effect(const EffectSharedPtr &effect)
	{
		m_clipmap_terrain_material->set_effect(effect);
	}

}
