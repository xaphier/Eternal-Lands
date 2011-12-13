/****************************************************************************
 *            material.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "material.hpp"
#include "effectcache.hpp"
#include "effect.hpp"
#include "texture.hpp"
#include "texturecache.hpp"
#include "shader/shadertextureutil.hpp"
#include "statemanager.hpp"
#include "materialdescription.hpp"

namespace eternal_lands
{

	namespace
	{

		const String empty_str = String(UTF8(""));

	}

	Material::Material(const EffectCacheWeakPtr &effect_cache,
		const TextureCacheWeakPtr &texture_cache):
		m_effect_cache(effect_cache), m_texture_cache(texture_cache),
		m_layer_index(0.0f), m_shadow(true), m_culling(true)
	{
		assert(!m_effect_cache.expired());
		assert(!m_texture_cache.expired());
	}

	Material::Material(const EffectCacheWeakPtr &effect_cache,
		const TextureCacheWeakPtr &texture_cache,
		const MaterialDescription &material):
		m_effect_cache(effect_cache), m_texture_cache(texture_cache),
		m_layer_index(0.0f), m_shadow(true), m_culling(true)
	{
		assert(!m_effect_cache.expired());
		assert(!m_texture_cache.expired());

		set_layer_index(material.get_layer_index());

		set_shadow(material.get_shadow());
		set_culling(material.get_culling());

		set_effect(material.get_effect());

		set_texture(material, stt_diffuse_0);
		set_texture(material, stt_diffuse_1);
		set_texture(material, stt_diffuse_2);
		set_texture(material, stt_diffuse_3);
		set_texture(material, stt_specular_0);
		set_texture(material, stt_specular_1);
		set_texture(material, stt_normal_0);
		set_texture(material, stt_normal_1);
		set_texture(material, stt_glow_0);
		set_texture(material, stt_glow_1);
		set_texture(material, stt_blend_0);
		set_texture(material, stt_blend_1);
	}

	Material::~Material() throw()
	{
	}

	void Material::set_texture(const MaterialDescription &material,
		const ShaderTextureType texture_type)
	{
		set_texture(material.get_texture(texture_type), texture_type);
	}

	void Material::set_texture(const String &name,
		const ShaderTextureType texture_type)
	{
		TextureSharedPtr texture;
		float layer;
		Uint16 index;

		assert(texture_type < m_textures.size());

		if (name.get().empty())
		{
			m_textures[texture_type].reset();

			return;
		}

		if (ShaderTextureUtil::get_use_layer_index(texture_type) &&
			get_texture_cache()->get_texture_array(name, texture,
				layer))
		{
			index = ShaderTextureUtil::get_layer_index(
				texture_type);

			assert(m_layer_index[index] == layer);

			m_layer_index[index] = layer;
			m_textures[texture_type] = texture;

			return;
		}

		m_textures[texture_type] =
			get_texture_cache()->get_texture(name);
	}

	const String &Material::get_texture_name(
		const ShaderTextureType texture_type) const
	{
		assert(texture_type < m_textures.size());

		if (get_texture(texture_type).get() == 0)
		{
			return empty_str;
		}

		return get_texture(texture_type)->get_name();
	}

	void Material::set_texture(const TextureSharedPtr &texture,
		const ShaderTextureType texture_type)
	{
		assert(texture_type < m_textures.size());

		m_textures[texture_type] = texture;
	}

	const TextureSharedPtr &Material::get_texture(
		const ShaderTextureType texture_type) const
	{
		assert(texture_type < m_textures.size());

		return m_textures[texture_type];
	}

	void Material::set_effect(const String &effect)
	{
		m_effect = get_effect_cache()->get_effect(effect);
	}

	void Material::bind(StateManager &state_manager) const
	{
		Uint16 i, count;

		count = m_textures.size();

		for (i = 0; i < count; i++)
		{
			if (m_textures[i].get() != 0)
			{
				state_manager.switch_texture(i, m_textures[i]);
			}
		}

		state_manager.switch_culling(get_culling());
		state_manager.switch_layer_index(get_layer_index());
	}

	const String &Material::get_effect_name() const
	{
		if (get_effect().get() == 0)
		{
			return empty_str;
		}

		return get_effect()->get_name();
	}

}
