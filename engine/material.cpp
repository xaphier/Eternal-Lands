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
		m_shadow(true), m_culling(true)
	{
		assert(!m_effect_cache.expired());
		assert(!m_texture_cache.expired());
		build_hash();
	}

	Material::Material(const EffectCacheWeakPtr &effect_cache,
		const TextureCacheWeakPtr &texture_cache,
		const MaterialDescription &material):
		m_effect_cache(effect_cache), m_texture_cache(texture_cache),
		m_shadow(true), m_culling(true)
	{
		assert(!m_effect_cache.expired());
		assert(!m_texture_cache.expired());

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
		assert(texture_type < m_textures.size());

		if (name.get().empty())
		{
			m_textures[texture_type].reset();

			build_hash();

			return;
		}

		m_textures[texture_type] = get_texture_cache()->get_texture(
			name);

		build_hash();
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

		build_hash();
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

		build_hash();
	}

	void Material::bind(StateManager &state_manager) const
	{
		Uint16 i, count;

		count = m_textures.size();

		for (i = 0; i < count; ++i)
		{
			if (m_textures[i].get() != 0)
			{
				state_manager.switch_texture(i, m_textures[i]);
			}
		}

		state_manager.switch_culling(get_culling());
	}

	const String &Material::get_effect_name() const
	{
		if (get_effect().get() == 0)
		{
			return empty_str;
		}

		return get_effect()->get_name();
	}

	bool Material::operator==(const Material &material) const
	{
		Uint16 i, count;

		if (get_hash() != material.get_hash())
		{
			return false;
		}

		if (get_effect().get() != material.get_effect().get())
		{
			return false;
		}

		count = m_textures.size();

		for (i = 0; i < count; ++i)
		{
			if (m_textures[i].get() != material.m_textures[i].get())
			{
				return false;
			}
		}

		if (get_shadow() != material.get_shadow())
		{
			return get_shadow() == material.get_shadow();
		}

		return get_culling() == material.get_culling();
	}

	bool Material::operator!=(const Material &material) const
	{
		return !operator==(material);
	}

	bool Material::operator<(const Material &material) const
	{
		Uint16 i, count;

		if (get_hash() != material.get_hash())
		{
			return get_hash() < material.get_hash();
		}

		if (get_effect().get() != material.get_effect().get())
		{
			return get_effect().get() < material.get_effect().get();
		}

		count = m_textures.size();

		for (i = 0; i < count; ++i)
		{
			if (m_textures[i].get() != material.m_textures[i].get())
			{
				return m_textures[i].get() <
					material.m_textures[i].get();
			}
		}

		if (get_shadow() != material.get_shadow())
		{
			return get_shadow() < material.get_shadow();
		}

		return get_culling() < material.get_culling();
	}

	void Material::build_hash()
	{
		Uint16 i, count;

		m_hash = 0;

		boost::hash_combine(m_hash, get_effect().get());

		count = m_textures.size();

		for (i = 0; i < count; ++i)
		{
			boost::hash_combine(m_hash, m_textures[i].get());
		}

		boost::hash_combine(m_hash, get_shadow());
		boost::hash_combine(m_hash, get_culling());
	}

}
