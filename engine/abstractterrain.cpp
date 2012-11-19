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
#include "image.hpp"
#include "imageupdate.hpp"

namespace eternal_lands
{

	namespace
	{

		const glm::vec3 vector_scale = glm::vec3(32.0f, 32.0f, 64.0f);
		const glm::vec3 vector_min = glm::vec3(-vector_scale.x,
			-vector_scale.y, 0.0f);
		const glm::vec3 vector_max = glm::vec3(vector_scale);

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

	const glm::vec3 &AbstractTerrain::get_vector_min() noexcept
	{
		return vector_min;
	}

	const glm::vec3 &AbstractTerrain::get_vector_max() noexcept
	{
		return vector_max;
	}

	const glm::vec3 &AbstractTerrain::get_vector_scale() noexcept
	{
		return vector_scale;
	}

	void AbstractTerrain::set_clipmap_texture(
		const TextureSharedPtr &texture)
	{
		m_terrain_material->set_texture(texture, spt_effect_0);
	}

	void AbstractTerrain::set_clipmap_normal_texture(
		const TextureSharedPtr &texture)
	{
		m_terrain_material->set_texture(texture, spt_effect_1);
	}

	void AbstractTerrain::set_geometry_maps(
		const ImageSharedPtr &displacement_map,
		const ImageSharedPtr &normal_map,
		const ImageSharedPtr &dudv_map)
	{
		String texture_format_str, rgb10_a2_str;

		if (displacement_map->get_texture_format() != tft_rgb10_a2)
		{
			rgb10_a2_str = TextureFormatUtil::get_str(tft_rgb10_a2);

			texture_format_str = TextureFormatUtil::get_str(
				displacement_map->get_texture_format());

			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_item_name(
					displacement_map->get_name())
				<< errinfo_string_value(texture_format_str)
				<< errinfo_expected_string_value(rgb10_a2_str));
		}

		do_set_geometry_maps(displacement_map, normal_map, dudv_map);
	}

	void AbstractTerrain::set_blend_map(
		const ImageSharedPtr &blend_map,
		const TextureCacheSharedPtr &texture_cache)
	{
		TextureSharedPtr texture;
		ImageUpdate blend_image;
		Uint32 i, j, mipmaps, count, width, height;

		if (get_global_vars()->get_opengl_3_0())
		{
			texture = texture_cache->get_texture_array(
				blend_map, String(UTF8("terrain-blend")));

			texture->set_wrap_s(twt_clamp);
			texture->set_wrap_t(twt_clamp);

			m_clipmap_terrain_material->set_texture(texture,
				ShaderSourceTerrain::get_blend_sampler(0));

			return;
		}

		count = glm::clamp(blend_map->get_layer(), 1u,
			ShaderSourceTerrain::get_non_array_blend_sampler_count(
				));

		mipmaps = blend_map->get_mipmap_count();

		for (i = 0; i < count; ++i)
		{
			StringStream str;

			str << blend_map->get_name() << UTF8("-layer-") << i;

			texture = boost::make_shared<Texture>(String(str.str()),
				blend_map->get_width(), blend_map->get_height(),
				0, 0xFFFF, 0, blend_map->get_texture_format(),
				ttt_texture_2d);

			width = blend_map->get_width();
			height = blend_map->get_height();

			texture->set_wrap_s(twt_clamp);
			texture->set_wrap_t(twt_clamp);
			texture->init(width, height, 0, mipmaps, 0);

			for (j = 0; j <= mipmaps; ++j)
			{
				texture->sub_texture(j, j, blend_map,
					glm::uvec3(0), glm::uvec3(0, 0, i),
					glm::uvec3(width, height, 0));

				if (width > 1)
				{
					width = width / 2;
				}

				if (height > 1)
				{
					height = height / 2;
				}
			}

			m_clipmap_terrain_material->set_texture(texture,
				ShaderSourceTerrain::get_blend_sampler(i));
		}
	}

	void AbstractTerrain::update_geometry_maps(
		const ImageUpdate &displacement_map,
		const ImageUpdate &normal_map, const ImageUpdate &dudv_map)
	{
		String texture_format_str, rgb10_a2_str;

		if (displacement_map.get_image()->get_texture_format() !=
			tft_rgb10_a2)
		{
			rgb10_a2_str = TextureFormatUtil::get_str(tft_rgb10_a2);

			texture_format_str = TextureFormatUtil::get_str(
				displacement_map.get_image(
					)->get_texture_format());

			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_item_name(
					displacement_map.get_image(
						)->get_name())
				<< errinfo_string_value(texture_format_str)
				<< errinfo_expected_string_value(rgb10_a2_str));
		}

		do_update_geometry_maps(displacement_map, normal_map, dudv_map);
	}

	void AbstractTerrain::update_blend_map(const ImageUpdate &blend_image)
	{
		glm::uvec3 offset, size;
		Uint32 i, count, mipmap;

		if (get_global_vars()->get_opengl_3_0())
		{
			m_clipmap_terrain_material->get_texture(
				ShaderSourceTerrain::get_blend_sampler(0)
					)->sub_texture(blend_image);

			return;
		}

		offset = blend_image.get_offset();
		mipmap = blend_image.get_mipmap();
		size = blend_image.get_size();

		count = size.z;

		size.z = 1;

		for (i = 0; i < count; ++i)
		{
			m_clipmap_terrain_material->get_texture(
				ShaderSourceTerrain::get_blend_sampler(
					i))->sub_texture(mipmap, mipmap,
						blend_image.get_image(),
						offset, offset, size);

			offset.z++;
		}
	}

	void AbstractTerrain::set_texture_maps(const StringVector &albedo_maps,
		const StringVector &extra_maps,
		const BitSet64 &use_blend_size_samplers,
		const BitSet64 &use_extra_maps,
		const TextureCacheSharedPtr &texture_cache)
	{
		set_albedo_maps(albedo_maps, use_blend_size_samplers,
			texture_cache);
		set_extra_maps(extra_maps, use_extra_maps, texture_cache);
	}

	void AbstractTerrain::set_albedo_maps(const StringVector &albedo_maps,
		const BitSet64 &use_blend_size_samplers,
		const TextureCacheSharedPtr &texture_cache)
	{
		TextureSharedPtr texture;
		StringVector albedo_blend_maps, albedo_no_blend_maps;
		Uint32 i, count;

		if (get_global_vars()->get_opengl_3_0())
		{
			count = std::min(albedo_maps.size(),
				use_blend_size_samplers.size());

			m_clipmap_terrain_material->set_texture(texture,
				ShaderSourceTerrain::get_albedo_sampler(0));

			m_clipmap_terrain_material->set_texture(texture,
				ShaderSourceTerrain::get_albedo_sampler(1));

			for (i = 0; i < count; ++i)
			{
				if (use_blend_size_samplers[i])
				{
					albedo_blend_maps.push_back(
						albedo_maps[i]);
				}
				else
				{
					albedo_no_blend_maps.push_back(
						albedo_maps[i]);
				}
			}

			if (albedo_no_blend_maps.size() > 0)
			{
				texture = texture_cache->get_texture_array(
					albedo_no_blend_maps,
					String(UTF8("terrain-albedo")), true);

				m_clipmap_terrain_material->set_texture(
					texture, ShaderSourceTerrain::
						get_albedo_sampler(0));
			}

			if (albedo_blend_maps.size() > 0)
			{
				texture = texture_cache->get_texture_array(
					albedo_blend_maps,
					String(UTF8("terrain-albedo-blend")),
					true);

				m_clipmap_terrain_material->set_texture(
					texture, ShaderSourceTerrain::
						get_albedo_sampler(1));
			}

			return;
		}

		count = ShaderSourceTerrain::get_non_array_albedo_sampler_count(
			);

		for (i = 0; i < count; ++i)
		{
			m_clipmap_terrain_material->set_texture(texture,
				ShaderSourceTerrain::get_albedo_sampler(i));
		}

		if (albedo_maps.size() == 0)
		{
			return;
		}

		count = albedo_maps.size();
		count = std::min(count,
			ShaderSourceTerrain::get_non_array_albedo_sampler_count(
				));

		for (i = 0; i < count; ++i)
		{
			texture = texture_cache->get_texture(albedo_maps[i],
				true);

			m_clipmap_terrain_material->set_texture(texture,
				ShaderSourceTerrain::get_albedo_sampler(i));
		}
	}

	void AbstractTerrain::set_extra_maps(const StringVector &extra_maps,
		const BitSet64 &use_extra_maps,
		const TextureCacheSharedPtr &texture_cache)
	{
		TextureSharedPtr texture;
		StringVector names;
		Uint32 i, count;

		if (!get_global_vars()->get_opengl_3_0())
		{
			return;
		}

		count = std::min(extra_maps.size(), use_extra_maps.size());

		for (i = 0; i < count; ++i)
		{
			if (use_extra_maps[i])
			{
				names.push_back(extra_maps[i]);
			}
		}

		m_clipmap_terrain_material->set_texture(texture,
			ShaderSourceTerrain::get_extra_sampler());

		if (names.size() == 0)
		{
			return;
		}

		texture = texture_cache->get_texture_array(names,
			String(UTF8("terrain-extra")), false);

		m_clipmap_terrain_material->set_texture(texture,
			ShaderSourceTerrain::get_extra_sampler());
	}

	void AbstractTerrain::set_effect(const EffectSharedPtr &effect)
	{
		m_clipmap_terrain_material->set_effect(effect);
	}

	void AbstractTerrain::set_dudv_scale_offset(
		const glm::vec4 &dudv_scale_offset)
	{
		m_terrain_material->set_dudv_scale_offset(dudv_scale_offset);
	}

	const glm::vec4 &AbstractTerrain::get_dudv_scale_offset() const
	{
		return m_terrain_material->get_dudv_scale_offset();
	}

}
