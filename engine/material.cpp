/****************************************************************************
 *            material.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
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
#include "script/materialscript.hpp"

namespace eternal_lands
{

	namespace
	{

		const String empty_str = String(UTF8(""));
		const EffectDescription empty_material;

	}

	Material::Material(const EffectCacheWeakPtr &effect_cache,
		const TextureCacheWeakPtr &texture_cache):
		m_effect_cache(effect_cache), m_texture_cache(texture_cache)
	{
		assert(!m_effect_cache.expired());
		assert(!m_texture_cache.expired());
	}

	Material::Material(const EffectCacheWeakPtr &effect_cache,
		const TextureCacheWeakPtr &texture_cache,
		const MaterialDescription &material):
		m_effect_cache(effect_cache), m_texture_cache(texture_cache)
	{
		assert(!m_effect_cache.expired());
		assert(!m_texture_cache.expired());

		set_effect(material.get_effect());

		m_data = material;

		set_texture(material, stt_albedo_0);
		set_texture(material, stt_albedo_1);
		set_texture(material, stt_albedo_2);
		set_texture(material, stt_albedo_3);
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

	Material::~Material() throw()
	{
	}

	void Material::set_effect(const String &effect)
	{
		m_effect = get_effect_cache()->get_effect(effect);

		assert(m_effect.get() != 0);
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

			return;
		}

		m_textures[texture_type] = get_texture_cache()->get_texture(
			name);
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

	const EffectDescription &Material::get_effect_description() const
	{
		if (m_effect.get() != 0)
		{
			return m_effect->get_description();
		}

		return empty_material;
	}

	const String &Material::get_script_name() const
	{
		if (get_material_script().get() == 0)
		{
			return empty_str;
		}

		return get_material_script()->get_name();
	}

	bool Material::execute_script(asIScriptContext* context)
	{
		if (get_material_script().get() != 0)
		{
			return get_material_script()->execute(m_data, context);
		}

		return true;
	}

	void Material::lock()
	{
		if (get_material_script().get() != 0)
		{
			get_material_script()->lock();
		}
	}

	void Material::unlock()
	{
		if (get_material_script().get() != 0)
		{
			get_material_script()->unlock();
		}
	}

}
