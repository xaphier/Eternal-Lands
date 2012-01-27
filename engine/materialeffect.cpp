/****************************************************************************
 *            materialeffect.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "materialeffect.hpp"
#include "effectcache.hpp"
#include "effect.hpp"
#include "texture.hpp"
#include "texturecache.hpp"
#include "shader/shadertextureutil.hpp"
#include "statemanager.hpp"
#include "materialeffectdescription.hpp"

namespace eternal_lands
{

	namespace
	{

		const String empty_str = String(UTF8(""));
		const EffectDescription empty_material;

	}

	MaterialEffect::MaterialEffect(const EffectCacheWeakPtr &effect_cache,
		const TextureCacheWeakPtr &texture_cache):
		m_effect_cache(effect_cache), m_texture_cache(texture_cache),
		m_texture_scale_offset(1.0f, 1.0f, 0.0f, 0.0f),
		m_cast_shadows(true), m_culling(true)
	{
		assert(!m_effect_cache.expired());
		assert(!m_texture_cache.expired());
	}

	MaterialEffect::MaterialEffect(const EffectCacheWeakPtr &effect_cache,
		const TextureCacheWeakPtr &texture_cache,
		const MaterialEffectDescription &material):
		m_effect_cache(effect_cache), m_texture_cache(texture_cache),
		m_texture_scale_offset(1.0f, 1.0f, 0.0f, 0.0f),
		m_cast_shadows(true), m_culling(true)
	{
		assert(!m_effect_cache.expired());
		assert(!m_texture_cache.expired());

		set_texture_scale_offset(material.get_texture_scale_offset());
		set_cast_shadows(material.get_cast_shadows());
		set_culling(material.get_culling());

		set_effect(EffectDescription(material));

		set_texture(material, stt_diffuse_0);
		set_texture(material, stt_diffuse_1);
		set_texture(material, stt_diffuse_2);
		set_texture(material, stt_diffuse_3);
		set_texture(material, stt_specular_0);
		set_texture(material, stt_specular_1);
		set_texture(material, stt_normal_0);
		set_texture(material, stt_normal_1);
		set_texture(material, stt_emission_0);
		set_texture(material, stt_emission_1);
		set_texture(material, stt_blend_0);
		set_texture(material, stt_blend_1);
		set_texture(material, stt_height);
		set_texture(material, stt_dudv);
	}

	MaterialEffect::~MaterialEffect() throw()
	{
	}

	void MaterialEffect::set_effect(const EffectDescription &effect)
	{
		m_effect = get_effect_cache()->get_effect(effect);

		assert(m_effect.get() != 0);
	}

	void MaterialEffect::set_texture(
		const MaterialEffectDescription &material,
		const ShaderTextureType texture_type)
	{
		set_texture(material.get_texture(texture_type), texture_type);
	}

	void MaterialEffect::set_texture(const String &name,
		const ShaderTextureType texture_type)
	{
		assert(texture_type < m_textures.size());

		if (name.get().empty())
		{
			m_textures[texture_type].reset();

			return;
		}

		m_textures[texture_type] = get_texture_cache()->get_texture(
			name);
	}

	const String &MaterialEffect::get_texture_name(
		const ShaderTextureType texture_type) const
	{
		assert(texture_type < m_textures.size());

		if (get_texture(texture_type).get() == 0)
		{
			return empty_str;
		}

		return get_texture(texture_type)->get_name();
	}

	void MaterialEffect::set_texture(const TextureSharedPtr &texture,
		const ShaderTextureType texture_type)
	{
		assert(texture_type < m_textures.size());

		m_textures[texture_type] = texture;
	}

	const TextureSharedPtr &MaterialEffect::get_texture(
		const ShaderTextureType texture_type) const
	{
		assert(texture_type < m_textures.size());

		return m_textures[texture_type];
	}

	void MaterialEffect::bind(StateManager &state_manager) const
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

	bool MaterialEffect::operator==(const MaterialEffect &material) const
	{
		Uint16 i, count;

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

		for (i = 0; i < 4; ++i)
		{
			if (get_texture_scale_offset()[i] !=
				material.get_texture_scale_offset()[i])
			{
				return false;
			}
		}

		if (get_cast_shadows() != material.get_cast_shadows())
		{
			return false;
		}

		return get_culling() == material.get_culling();
	}

	bool MaterialEffect::operator!=(const MaterialEffect &material) const
	{
		return !operator==(material);
	}

	bool MaterialEffect::operator<(const MaterialEffect &material) const
	{
		Uint16 i, count;

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

		for (i = 0; i < 4; ++i)
		{
			if (get_texture_scale_offset()[i] !=
				material.get_texture_scale_offset()[i])
			{
				return get_texture_scale_offset()[i] <
					material.get_texture_scale_offset()[i];
			}
		}

		if (get_cast_shadows() != material.get_cast_shadows())
		{
			return get_cast_shadows() < material.get_cast_shadows();
		}

		return get_culling() < material.get_culling();
	}

	const EffectDescription &MaterialEffect::get_effect_description() const
	{
		if (m_effect.get() != 0)
		{
			return m_effect->get_description();
		}

		return empty_material;
	}

}
