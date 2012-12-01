/****************************************************************************
 *            material.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "material.hpp"
#include "effect/effectcache.hpp"
#include "effect/effect.hpp"
#include "texture.hpp"
#include "texturecache.hpp"
#include "shader/samplerparameterutil.hpp"
#include "statemanager.hpp"
#include "materialdescription.hpp"
#include "script/materialscript.hpp"
#include "script/materialscriptcache.hpp"
#include "script/materialscriptmanager.hpp"
#include "shader/glslprogram.hpp"

namespace eternal_lands
{

	namespace
	{

		const String empty_str = String();
		const EffectDescription empty_material;

	}

	Material::Material(const EffectCacheSharedPtr &effect_cache,
		const TextureCacheSharedPtr &texture_cache,
		const MaterialScriptCacheSharedPtr &material_script_cache,
		const MaterialScriptManagerSharedPtr &material_script_manager,
		const MaterialDescription &material):
		m_name(material.get_name()), m_effect_cache(effect_cache),
		m_texture_cache(texture_cache),
		m_material_script_cache(material_script_cache),
		m_material_script_manager(material_script_manager)
	{
		assert(!m_effect_cache.expired());
		assert(!m_texture_cache.expired());
		assert(!m_material_script_cache.expired());
		assert(!m_material_script_manager.expired());

		set_effect(material.get_effect());
		set_material_script(material.get_script());

		m_data = material;

		set_texture(material, spt_effect_0);
		set_texture(material, spt_effect_1);
		set_texture(material, spt_effect_2);
		set_texture(material, spt_effect_3);
		set_texture(material, spt_effect_4);
		set_texture(material, spt_effect_5);
		set_texture(material, spt_effect_6);
		set_texture(material, spt_effect_7);
		set_texture(material, spt_effect_8);
		set_texture(material, spt_effect_9);
		set_texture(material, spt_effect_10);
		set_texture(material, spt_effect_11);
		set_texture(material, spt_effect_12);
		set_texture(material, spt_effect_13);
		set_texture(material, spt_effect_14);
		set_texture(material, spt_effect_15);
	}

	Material::~Material() noexcept
	{
		if (!m_material_script_manager.expired() &&
			(m_material_script.get() != nullptr))
		{
			m_material_script.reset();

			get_material_script_manager()->remove_material(this);
		}
	}

	void Material::set_material_script(const String &material_script)
	{
		if (material_script.get().empty())
		{
			m_material_script.reset();

			get_material_script_manager()->remove_material(this);

			return;
		}

		m_material_script = get_material_script_cache(
			)->get_material_script(material_script);

		get_material_script_manager()->add_material(this);

		assert(m_material_script.get() != nullptr);
	}

	void Material::set_effect(const String &effect)
	{
		m_effect = get_effect_cache()->get_effect(effect);

		assert(m_effect.get() != nullptr);
	}

	void Material::set_texture(const MaterialDescription &material,
		const SamplerParameterType sampler)
	{
		set_texture(material.get_texture(sampler), sampler,
			material.get_sRGB(sampler));
	}

	void Material::set_texture(const String &name,
		const SamplerParameterType sampler, const bool sRGB)
	{
		assert(sampler < m_textures.size());

		if (name.get().empty())
		{
			m_textures[sampler].reset();

			return;
		}

		m_textures[sampler] = get_texture_cache()->get_texture(name,
			sRGB);
	}

	const String &Material::get_texture_name(
		const SamplerParameterType sampler) const
	{
		assert(sampler < m_textures.size());

		if (get_texture(sampler).get() == nullptr)
		{
			return empty_str;
		}

		return get_texture(sampler)->get_name();
	}

	void Material::set_texture(const TextureSharedPtr &texture,
		const SamplerParameterType sampler)
	{
		assert(sampler < m_textures.size());

		m_textures[sampler] = texture;
	}

	const TextureSharedPtr &Material::get_texture(
		const SamplerParameterType sampler) const
	{
		assert(sampler < m_textures.size());

		return m_textures[sampler];
	}

	void Material::bind(StateManager &state_manager) const
	{
		Uint16 i, count;

		count = m_textures.size();

		for (i = 0; i < count; ++i)
		{
			if (m_textures[i].get() != nullptr)
			{
				state_manager.switch_texture(i, m_textures[i]);
			}
		}

		state_manager.switch_culling(get_culling());

		state_manager.get_program()->set_parameter(apt_texture_matrices,
			get_texture_matrices());
		state_manager.get_program()->set_parameter(apt_material_color,
			get_color());
		state_manager.get_program()->set_parameter(
			apt_dudv_scale_offset, get_dudv_scale_offset());
	}

	const EffectDescription &Material::get_effect_description() const
		noexcept
	{
		if (m_effect.get() != nullptr)
		{
			return m_effect->get_description();
		}

		return empty_material;
	}

	const String &Material::get_material_script_name() const noexcept
	{
		if (get_material_script().get() == nullptr)
		{
			return empty_str;
		}

		return get_material_script()->get_name();
	}

	bool Material::execute_script(const glm::vec4 &time,
		asIScriptContext* context)
	{
		if (get_material_script().get() != nullptr)
		{
			return get_material_script()->execute(time, m_data,
				context);
		}

		return true;
	}

	void Material::lock() noexcept
	{
		if (get_material_script().get() != nullptr)
		{
			get_material_script()->lock();
		}
	}

	void Material::unlock() noexcept
	{
		if (get_material_script().get() != nullptr)
		{
			get_material_script()->unlock();
		}
	}

}
