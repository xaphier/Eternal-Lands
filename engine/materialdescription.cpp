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

	ShaderTextureType MaterialDescription::get_shader_texture(
		const Uint16 index, const bool use_low_index)
	{
		switch (index)
		{
			case 0:
				if (use_low_index)
				{
					return stt_diffuse_0;
				}
				return stt_diffuse_2;
			case 1:
				if (use_low_index)
				{
					return stt_diffuse_1;
				}
				return stt_diffuse_3;
			case 2:
				if (use_low_index)
				{
					return stt_specular_0;
				}
				return stt_specular_1;
			case 3:
				if (use_low_index)
				{
					return stt_normal_0;
				}
				return stt_normal_1;
			case 4:
				if (use_low_index)
				{
					return stt_glow_0;
				}
				return stt_glow_1;
			case 5:
				if (use_low_index)
				{
					return stt_blend_0;
				}
				return stt_blend_1;
		}

		EL_THROW_EXCEPTION(RangeErrorException()
			<< errinfo_range_min(0)
			<< errinfo_range_max(5)
			<< errinfo_range_index(index)
			<< errinfo_parameter_name(UTF8("index")));
	}

	bool MaterialDescription::empty(const bool use_low_index) const
	{
		Uint16 i, count;

		count = m_textures.size() / 2;

		for (i = 0; i < count; i++)
		{
			if (get_texture(get_shader_texture(i,
				use_low_index)).get().empty())
			{
				return false;
			}
		}

		return true;
	}

	bool MaterialDescription::compare_textures(
		const MaterialDescription &material, const bool use_low_index,
		const bool use_low_source_index) const
	{
		Uint16 i, count;

		count = m_textures.size() / 2;

		for (i = 0; i < count; i++)
		{
			if (get_texture(get_shader_texture(i, use_low_index)) !=
				material.get_texture(get_shader_texture(i,
					use_low_index)))
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
		return compare_textures(material, true, true) &&
			compare_textures(material, false, false) &&
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

	bool MaterialDescription::can_merge(const MaterialDescription &material)
		const
	{
		return (get_effect() == material.get_effect()) &&
			(get_shadow() == material.get_shadow()) &&
			(get_culling() == material.get_culling()) &&
			get_texture(stt_diffuse_2).get().empty() &&
			get_texture(stt_diffuse_3).get().empty() &&
			get_texture(stt_normal_1).get().empty() &&
			get_texture(stt_specular_1).get().empty() &&
			get_texture(stt_glow_0).get().empty() &&
			get_texture(stt_glow_1).get().empty() &&
			get_texture(stt_blend_0).get().empty() &&
			get_texture(stt_blend_1).get().empty() &&
			material.get_texture(stt_diffuse_2).get().empty() &&
			material.get_texture(stt_diffuse_3).get().empty() &&
			material.get_texture(stt_normal_1).get().empty() &&
			material.get_texture(stt_specular_1).get().empty() &&
			material.get_texture(stt_glow_0).get().empty() &&
			material.get_texture(stt_glow_1).get().empty() &&
			material.get_texture(stt_blend_0).get().empty() &&
			material.get_texture(stt_blend_1).get().empty();
	}

	bool MaterialDescription::contains(const MaterialDescription &material,
		glm::vec4 &index) const
	{
		if ((get_effect() != material.get_effect()) ||
			(get_shadow() != material.get_shadow()) ||
			(get_culling() != material.get_culling()))
		{
			return false;
		}

		if (get_texture(stt_diffuse_0) ==
			material.get_texture(stt_diffuse_0))
		{
			index.x = 0.0f;
		}
		else
		{
			if (get_texture(stt_diffuse_2) ==
				material.get_texture(stt_diffuse_0))
			{
				index.x = 1.0f;
			}
			else
			{
				return false;
			}
		}

		if (get_texture(stt_diffuse_1) ==
			material.get_texture(stt_diffuse_1))
		{
			index.y = 0.0f;
		}
		else
		{
			if (get_texture(stt_diffuse_3) ==
				material.get_texture(stt_diffuse_1))
			{
				index.y = 1.0f;
			}
			else
			{
				return false;
			}
		}

		if (get_texture(stt_normal_0) ==
			material.get_texture(stt_normal_0))
		{
			index.z = 0.0f;
		}
		else
		{
			if (get_texture(stt_normal_1) ==
				material.get_texture(stt_normal_0))
			{
				index.z = 1.0f;
			}
			else
			{
				return false;
			}
		}

		if (get_texture(stt_specular_0) ==
			material.get_texture(stt_specular_0))
		{
			index.w = 0.0f;
		}
		else
		{
			if (get_texture(stt_specular_1) ==
				material.get_texture(stt_specular_0))
			{
				index.w = 1.0f;
			}
			else
			{
				return false;
			}
		}

		if (material.get_texture(stt_diffuse_2).get().empty() &&
			material.get_texture(stt_diffuse_3).get().empty() &&
			material.get_texture(stt_normal_1).get().empty() &&
			material.get_texture(stt_specular_1).get().empty() &&
			get_texture(stt_glow_0) ==
				material.get_texture(stt_glow_0) &&
			get_texture(stt_glow_1) ==
				material.get_texture(stt_glow_1) &&
			get_texture(stt_blend_0) ==
				material.get_texture(stt_blend_0) &&
			get_texture(stt_blend_1) ==
				material.get_texture(stt_blend_1))
		{
			return true;
		}

		return false;		
	}

}
