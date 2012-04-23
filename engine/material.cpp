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
#include "script/materialscriptcache.hpp"
#include "script/materialscriptmanager.hpp"
#include "glslprogram.hpp"

namespace eternal_lands
{

	namespace
	{

		const String empty_str = String(UTF8(""));
		const EffectDescription empty_material;

	}

	Material::Material(const EffectCacheWeakPtr &effect_cache,
		const TextureCacheWeakPtr &texture_cache,
		const MaterialScriptCacheWeakPtr &material_script_cache,
		const MaterialScriptManagerWeakPtr &material_script_manager):
		m_effect_cache(effect_cache), m_texture_cache(texture_cache),
		m_material_script_cache(material_script_cache),
		m_material_script_manager(material_script_manager)
	{
		assert(!m_effect_cache.expired());
		assert(!m_texture_cache.expired());
		assert(!m_material_script_cache.expired());
		assert(!m_material_script_manager.expired());
	}

	Material::~Material() throw()
	{
		if (!m_material_script_manager.expired() &&
			(m_material_script.get() != 0))
		{
			m_material_script.reset();

			get_material_script_manager()->remove_material(
				shared_from_this());
		}
	}

	void Material::init(const MaterialDescription &material)
	{
		set_effect(material.get_effect());
		set_material_script(material.get_script());

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

	void Material::set_material_script(const String &material_script)
	{
		if (material_script.get().empty())
		{
			m_material_script.reset();

			get_material_script_manager()->remove_material(
				shared_from_this());

			return;
		}

		m_material_script = get_material_script_cache(
			)->get_material_script(material_script);

		get_material_script_manager()->add_material(
			shared_from_this());

		assert(m_material_script.get() != 0);
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

		state_manager.get_program()->set_parameter(apt_texture_matrices,
			get_texture_matrices());
		state_manager.get_program()->set_parameter(
			apt_albedo_scale_offsets, get_albedo_scale_offsets());
		state_manager.get_program()->set_parameter(
			apt_emission_scale_offset, get_emission_scale_offset());
		state_manager.get_program()->set_parameter(
			apt_specular_scale_offset, get_specular_scale_offset());
		state_manager.get_program()->set_parameter(apt_dudv_scale,
			get_dudv_scale());
	}

	const EffectDescription &Material::get_effect_description() const
	{
		if (m_effect.get() != 0)
		{
			return m_effect->get_description();
		}

		return empty_material;
	}

	const String &Material::get_material_script_name() const
	{
		if (get_material_script().get() == 0)
		{
			return empty_str;
		}

		return get_material_script()->get_name();
	}

	bool Material::execute_script(const glm::vec4 &time,
		asIScriptContext* context)
	{
		if (get_material_script().get() != 0)
		{
			return get_material_script()->execute(time, m_data,
				context);
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
