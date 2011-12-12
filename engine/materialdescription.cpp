/****************************************************************************
 *            materialdescription.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "materialdescription.hpp"
#include "exceptions.hpp"

namespace eternal_lands
{

	namespace
	{

		bool find(const StringVector &names, const String &name,
			Sint32 &index)
		{
			Uint32 i, count;

			if (name.get().empty())
			{
				index = 0;
				return true;
			}

			count = names.size();

			for (i = 0; i < count; i++)
			{
				if (names[i] == name)
				{
					index = i;
					return true;
				}
			}

			return false;
		}

	}

	MaterialDescription::MaterialDescription(): m_shadow(true),
		m_culling(true)
	{
	}

	MaterialDescription::MaterialDescription(const String &effect):
		m_shadow(true), m_culling(true)
	{
		set_effect(effect);
	}

	MaterialDescription::MaterialDescription(const String &diffuse_0,
		const String &effect): m_shadow(true), m_culling(true)
	{
		set_texture(diffuse_0, stt_diffuse_0);
		set_effect(effect);
	}

	MaterialDescription::MaterialDescription(const String &diffuse_0,
		const String &diffuse_1, const String &effect): m_shadow(true),
		m_culling(true)
	{
		set_texture(diffuse_0, stt_diffuse_0);
		set_texture(diffuse_1, stt_diffuse_1);
		set_effect(effect);
	}

	MaterialDescription::MaterialDescription(const String &diffuse_0,
		const String &normal, const String &specular,
		const String &effect): m_shadow(true), m_culling(true)
	{
		set_texture(diffuse_0, stt_diffuse_0);
		set_texture(normal, stt_normal_0);
		set_texture(specular, stt_specular_0);
		set_effect(effect);
	}

	MaterialDescription::MaterialDescription(const String &diffuse_0,
		const String &diffuse_1, const String &normal,
		const String &specular, const String &effect): m_shadow(true),
		m_culling(true)
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
			compare_non_textures(material);
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

		if (get_shadow() != material.get_shadow())
		{
			return get_shadow() < material.get_shadow();
		}

		return get_culling() < material.get_culling();
	}

	bool MaterialDescription::can_merge(
		const MaterialDescription &material) const
	{
		return (get_effect() == material.get_effect()) &&
			(get_shadow() == material.get_shadow()) &&
			(get_culling() == material.get_culling()) &&
			(get_texture(stt_diffuse_2) ==
				material.get_texture(stt_diffuse_2)) &&
			(get_texture(stt_diffuse_3) ==
				material.get_texture(stt_diffuse_3)) &&
			(get_texture(stt_normal_1) ==
				material.get_texture(stt_normal_1)) &&
			(get_texture(stt_specular_1) ==
				material.get_texture(stt_specular_1)) &&
			(get_texture(stt_glow_0) ==
				material.get_texture(stt_glow_0)) &&
			(get_texture(stt_glow_1) ==
				material.get_texture(stt_glow_1)) &&
			(get_texture(stt_blend_0) ==
				material.get_texture(stt_blend_0)) &&
			(get_texture(stt_blend_1) ==
				material.get_texture(stt_blend_1));
	}
#if	0	
		String texture;
		Uint32 i, count;

		texture = get_texture(texture_type);

		if (texture.get().empty())
		{
			layer = 0;
			return true;
		}

		count = textures.size();

		for (i = 0; i < count; i++)
		{
			if (textures[i] == texture)
			{
				layer = i;
				return true;
			}
		}

		return false;
	}
#endif
}
