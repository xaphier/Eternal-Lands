/****************************************************************************
 *            materialdescription.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "materialdescription.hpp"
#include "exceptions.hpp"
#include "texturecache.hpp"

namespace eternal_lands
{

	MaterialDescription::MaterialDescription(): m_layer_index(0.0f),
		m_shadow(true), m_culling(true)
	{
	}

	MaterialDescription::MaterialDescription(const String &effect):
		m_layer_index(0.0f), m_shadow(true), m_culling(true)
	{
		set_effect(effect);
	}

	MaterialDescription::MaterialDescription(const String &diffuse_0,
		const String &effect): m_layer_index(0.0f), m_shadow(true),
		m_culling(true)
	{
		set_texture(diffuse_0, stt_diffuse_0);
		set_effect(effect);
	}

	MaterialDescription::MaterialDescription(const String &diffuse_0,
		const String &diffuse_1, const String &effect):
		m_layer_index(0.0f), m_shadow(true), m_culling(true)
	{
		set_texture(diffuse_0, stt_diffuse_0);
		set_texture(diffuse_1, stt_diffuse_1);
		set_effect(effect);
	}

	MaterialDescription::MaterialDescription(const String &diffuse_0,
		const String &normal, const String &specular,
		const String &effect): m_layer_index(0.0f), m_shadow(true),
		m_culling(true)
	{
		set_texture(diffuse_0, stt_diffuse_0);
		set_texture(normal, stt_normal_0);
		set_texture(specular, stt_specular_0);
		set_effect(effect);
	}

	MaterialDescription::MaterialDescription(const String &diffuse_0,
		const String &diffuse_1, const String &normal,
		const String &specular, const String &effect):
		m_layer_index(0.0f), m_shadow(true), m_culling(true)
	{
		set_texture(diffuse_0, stt_diffuse_0);
		set_texture(diffuse_1, stt_diffuse_1);
		set_texture(normal, stt_normal_0);
		set_texture(specular, stt_specular_0);
		set_effect(effect);
	}

	MaterialDescription::~MaterialDescription() throw()
	{
	}

	bool MaterialDescription::compare_textures(
		const MaterialDescription &material) const
	{
		Uint16 i, count;

		count = m_textures.size();

		for (i = 0; i < count; i++)
		{
			if (m_textures[i] != material.m_textures[i])
			{
				return false;
			}
		}

		return true;
	}

	bool MaterialDescription::compare_non_textures(
		const MaterialDescription &material) const
	{
		return (get_effect() == material.get_effect()) &&
			(get_shadow() == material.get_shadow()) &&
			(get_culling() == material.get_culling());
	}

	bool MaterialDescription::operator==(
		const MaterialDescription &material) const
	{
		return compare_textures(material) &&
			compare_non_textures(material) &&
			glm::all(glm::equal(get_layer_index(),
				material.get_layer_index()));
	}

	bool MaterialDescription::operator!=(
		const MaterialDescription &material) const
	{
		return !(*this == material);
	}

	bool MaterialDescription::operator<(
		const MaterialDescription &material) const
	{
		Uint16 i, count;

		if (get_effect() != material.get_effect())
		{
			return get_effect() < material.get_effect();
		}

		count = m_textures.size();

		for (i = 0; i < count; i++)
		{
			if (m_textures[i] != material.m_textures[i])
			{
				return m_textures[i] < material.m_textures[i];
			}
		}

		count = m_layer_index.length();

		for (i = 0; i < count; i++)
		{
			if (m_layer_index[i] != material.m_layer_index[i])
			{
				return m_layer_index[i] <
					material.m_layer_index[i];
			}
		}

		if (get_shadow() != material.get_shadow())
		{
			return get_shadow() < material.get_shadow();
		}

		return get_culling() < material.get_culling();
	}

	bool MaterialDescription::can_merge(
		const MaterialDescription &material) const
	{
		return compare_textures(material) &&
			compare_non_textures(material);
	}

	void MaterialDescription::build_layer_index(
		const TextureCacheSharedPtr &texture_cache,
		const ShaderTextureType texture_type)
	{
		String name;
		float layer;
		Uint16 index;

		if (ShaderTextureUtil::get_use_layer_index(texture_type)
			&& texture_cache->get_texture_array_name(
				get_texture(texture_type), name, layer))
		{
			index = ShaderTextureUtil::get_layer_index(
				texture_type);

			m_layer_index[index] = layer;
			m_textures[texture_type] = name;
		}
	}

	void MaterialDescription::build_layer_index(
		const TextureCacheSharedPtr &texture_cache)
	{
		Uint16 i, count;

		count = m_textures.size();

		for (i = 0; i < count; i++)
		{
			build_layer_index(texture_cache,
				static_cast<ShaderTextureType>(i));
		}
	}

}
