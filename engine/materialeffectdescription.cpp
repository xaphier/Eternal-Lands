/****************************************************************************
 *            materialeffectdescription.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "materialeffectdescription.hpp"

namespace eternal_lands
{

	MaterialEffectDescription::MaterialEffectDescription():
		m_cast_shadows(true), m_culling(true), m_transparent(false)
	{
	}

	MaterialEffectDescription::~MaterialEffectDescription() throw()
	{
	}

	bool MaterialEffectDescription::operator<(
		const MaterialEffectDescription &material) const
	{
		Uint16 i;

		if (get_world_transform() != material.get_world_transform())
		{
			return get_world_transform() <
				material.get_world_transform();
		}

		if (get_texture_coodrinates() !=
			material.get_texture_coodrinates())
		{
			return get_texture_coodrinates() <
				material.get_texture_coodrinates();
		}

		if (get_diffuse_mapping() != material.get_diffuse_mapping())
		{
			return get_diffuse_mapping() <
				material.get_diffuse_mapping();
		}

		if (get_normal_mapping() != material.get_normal_mapping())
		{
			return get_normal_mapping() <
				material.get_normal_mapping();
		}

		if (get_specular_mapping() != material.get_specular_mapping())
		{
			return get_specular_mapping() <
				material.get_specular_mapping();
		}

		if (get_emission_mapping() != material.get_emission_mapping())
		{
			return get_emission_mapping() <
				material.get_emission_mapping();
		}

		if (get_receives_shadows() != material.get_receives_shadows())
		{
			return get_receives_shadows() <
				material.get_receives_shadows();
		}

		if (get_transparent() != material.get_transparent())
		{
			return get_transparent() < material.get_transparent();
		}

		if (get_lighting() != material.get_lighting())
		{
			return get_lighting() < material.get_lighting();
		}

		for (i = 0; i < material_texture_count; ++i)
		{
			if (get_texture(i) != material.get_texture(i))
			{
				return get_texture(i) <
					material.get_texture(i);
			}
		}

		if (get_cast_shadows() != material.get_cast_shadows())
		{
			return get_cast_shadows() <
				material.get_cast_shadows();
		}

		return get_culling() < material.get_culling();
	}

	bool MaterialEffectDescription::operator==(
		const MaterialEffectDescription &material) const
	{
		Uint16 i;

		if (get_world_transform() != material.get_world_transform())
		{
			return false;
		}

		if (get_texture_coodrinates() !=
			material.get_texture_coodrinates())
		{
			return false;
		}

		if (get_diffuse_mapping() != material.get_diffuse_mapping())
		{
			return false;
		}

		if (get_normal_mapping() != material.get_normal_mapping())
		{
			return false;
		}

		if (get_specular_mapping() != material.get_specular_mapping())
		{
			return false;
		}

		if (get_emission_mapping() != material.get_emission_mapping())
		{
			return false;
		}

		if (get_receives_shadows() != material.get_receives_shadows())
		{
			return false;
		}

		if (get_transparent() != material.get_transparent())
		{
			return false;
		}

		if (get_lighting() != material.get_lighting())
		{
			return false;
		}

		for (i = 0; i < material_texture_count; ++i)
		{
			if (get_texture(i) != material.get_texture(i))
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

	bool MaterialEffectDescription::operator!=(
		const MaterialEffectDescription &material) const
	{
		return !operator==(material);
	}

	void MaterialEffectDescription::set_material_descriptiont(
		const MaterialDescription &material)
	{
		asign(material);
	}

	OutStream& operator<<(OutStream &str,
		const MaterialEffectDescription &value)
	{
		ShaderTextureType type;
		Uint16 i;

		str << "material_name: " << value.get_material_name();
		str << "world_transform: " << value.get_world_transform();
		str << " texture_coodrinates: ";
		str << value.get_texture_coodrinates() << std::endl;
		str << " diffuse_mapping: " << value.get_diffuse_mapping();
		str << " normal_mapping: " << value.get_normal_mapping();
		str << " specular_mapping: " << value.get_specular_mapping();
		str << " emission_mapping: " << value.get_emission_mapping();
		str << " receives_shadows: " << value.get_receives_shadows();
		str << " transparent: " << value.get_transparent();
		str << " lighting: " << value.get_lighting();

		for (i = 0; i < material_texture_count; ++i)
		{
			type = static_cast<ShaderTextureType>(i);
			str << " " << type << ": " << value.get_texture(type);
		}

		str << " cast_shadows: " << value.get_cast_shadows();
		str << " culling: " << value.get_culling();

		return str;
	}

}
