/****************************************************************************
 *            materialeffectdescription.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "materialeffectdescription.hpp"
#include "effectdescription.hpp"

namespace eternal_lands
{

	MaterialEffectDescription::MaterialEffectDescription():
		m_cast_shadows(true), m_culling(true), m_transparent(false),
		m_billboard(false)
	{
	}

	MaterialEffectDescription::~MaterialEffectDescription() throw()
	{
	}

	bool MaterialEffectDescription::operator<(
		const MaterialEffectDescription &material) const
	{
		glm::bvec4 cmp4;
		glm::bvec3 cmp3;
		Uint16 i, j;

		if (get_world_transformation() !=
			material.get_world_transformation())
		{
			return get_world_transformation() <
				material.get_world_transformation();
		}

		if (get_texture_coodrinates() !=
			material.get_texture_coodrinates())
		{
			return get_texture_coodrinates() <
				material.get_texture_coodrinates();
		}

		if (get_albedo_mapping() != material.get_albedo_mapping())
		{
			return get_albedo_mapping() <
				material.get_albedo_mapping();
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

		if (get_billboard() != material.get_billboard())
		{
			return get_billboard() < material.get_billboard();
		}

		for (i = 0; i < material_texture_count; ++i)
		{
			if (get_texture(i) != material.get_texture(i))
			{
				return get_texture(i) <
					material.get_texture(i);
			}
		}

		for (i = 0; i < 4; ++i)
		{
			cmp4 = glm::equal(get_albedo_scale_offset(i)[0],
				material.get_albedo_scale_offset(i)[0]);
			
			if (glm::all(cmp4))
			{
				continue;
			}

			for (j = 0; j < 4; ++i)
			{
				if (cmp4[j])
				{
					continue;
				}

				return get_albedo_scale_offset(i)[0][j] <
					material.get_albedo_scale_offset(i)[0][j];
			}

			cmp4 = glm::equal(get_albedo_scale_offset(i)[1],
				material.get_albedo_scale_offset(i)[1]);
			
			if (glm::all(cmp4))
			{
				continue;
			}

			for (j = 0; j < 4; ++i)
			{
				if (cmp4[j])
				{
					continue;
				}

				return get_albedo_scale_offset(i)[1][j] <
					material.get_albedo_scale_offset(
						i)[1][j];
			}
		}

		for (i = 0; i < 4; ++i)
		{
			cmp3 = glm::equal(get_texture_matrix(i)[0],
				material.get_texture_matrix(i)[0]);
			
			if (glm::all(cmp3))
			{
				continue;
			}

			for (j = 0; j < 3; ++i)
			{
				if (cmp3[j])
				{
					continue;
				}

				return get_texture_matrix(i)[0][j] <
					material.get_texture_matrix(i)[0][j];
			}

			cmp3 = glm::equal(get_texture_matrix(i)[1],
				material.get_texture_matrix(i)[1]);
			
			if (glm::all(cmp3))
			{
				continue;
			}

			for (j = 0; j < 3; ++i)
			{
				if (cmp3[j])
				{
					continue;
				}

				return get_texture_matrix(i)[1][j] <
					material.get_texture_matrix(i)[1][j];
			}
		}

		for (i = 0; i < 3; ++i)
		{
			if (get_emission_scale_offset()[0][i] !=
				material.get_emission_scale_offset()[0][i])
			{
				
				return get_emission_scale_offset()[0][i] <
					material.get_emission_scale_offset(
						)[0][i];
			}
		}

		for (i = 0; i < 3; ++i)
		{
			if (get_emission_scale_offset()[1][i] !=
				material.get_emission_scale_offset()[1][i])
			{
				
				return get_emission_scale_offset()[1][i] <
					material.get_emission_scale_offset(
						)[1][i];
			}
		}

		for (i = 0; i < 4; ++i)
		{
			if (get_specular_scale_offset()[i] !=
				material.get_specular_scale_offset()[i])
			{
				
				return get_specular_scale_offset()[i] <
					material.get_specular_scale_offset()[i];
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

		if (get_world_transformation() !=
			material.get_world_transformation())
		{
			return false;
		}

		if (get_texture_coodrinates() !=
			material.get_texture_coodrinates())
		{
			return false;
		}

		if (get_albedo_mapping() != material.get_albedo_mapping())
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

		if (get_billboard() != material.get_billboard())
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

		for (i = 0; i < 4; ++i)
		{
			if (get_albedo_scale_offset(i) !=
				material.get_albedo_scale_offset(i))
			{
				return false;
			}
		}

		for (i = 0; i < 4; ++i)
		{
			if (get_texture_matrix(i) !=
				material.get_texture_matrix(i))
			{
				return false;
			}
		}

		if (get_emission_scale_offset() !=
			material.get_emission_scale_offset())
		{
			return false;
		}

		if (get_specular_scale_offset() !=
			material.get_specular_scale_offset())
		{
			return false;
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

	void MaterialEffectDescription::set_effect_description(
		const EffectDescription &effect)
	{
		set_world_transformation(effect.get_world_transformation());
		set_texture_coodrinates(effect.get_texture_coodrinates());
		set_albedo_mapping(effect.get_albedo_mapping());
		set_normal_mapping(effect.get_normal_mapping());
		set_specular_mapping(effect.get_specular_mapping());
		set_emission_mapping(effect.get_emission_mapping());
		set_receives_shadows(effect.get_receives_shadows());
		set_transparent(effect.get_transparent());
		set_lighting(effect.get_lighting());
		set_billboard(effect.get_billboard());
	}

	void MaterialEffectDescription::get_effect_description(
		EffectDescription &effect) const
	{
		effect.set_world_transformation(get_world_transformation());
		effect.set_texture_coodrinates(get_texture_coodrinates());
		effect.set_albedo_mapping(get_albedo_mapping());
		effect.set_normal_mapping(get_normal_mapping());
		effect.set_specular_mapping(get_specular_mapping());
		effect.set_emission_mapping(get_emission_mapping());
		effect.set_receives_shadows(get_receives_shadows());
		effect.set_transparent(get_transparent());
		effect.set_lighting(get_lighting());
		effect.set_billboard(get_billboard());
	}

	OutStream& operator<<(OutStream &str,
		const MaterialEffectDescription &value)
	{
		ShaderTextureType type;
		Uint16 i;

		str << "material_name: " << value.get_material_name();
		str << " world_transformation: ";
		str << value.get_world_transformation();
		str << " texture_coodrinates: ";
		str << value.get_texture_coodrinates() << std::endl;
		str << " albedo_mapping: " << value.get_albedo_mapping();
		str << " normal_mapping: " << value.get_normal_mapping();
		str << " specular_mapping: " << value.get_specular_mapping();
		str << " emission_mapping: " << value.get_emission_mapping();
		str << " receives_shadows: " << value.get_receives_shadows();
		str << " transparent: " << value.get_transparent();
		str << " lighting: " << value.get_lighting();
		str << " billboard: " << value.get_billboard();

		for (i = 0; i < material_texture_count; ++i)
		{
			type = static_cast<ShaderTextureType>(i);
			str << " " << type << ": " << value.get_texture(type);
		}

		for (i = 0; i < 4; ++i)
		{
			str << " albedo_scale: " << glm::to_string(
				value.get_albedo_scale_offset(i)[0]);
			str << " albedo_offset: " << glm::to_string(
				value.get_albedo_scale_offset(i)[1]);
		}

		for (i = 0; i < 4; ++i)
		{
			str << " texture_scale_offset: " << glm::to_string(
				value.get_texture_matrix(i));
		}

		str << " emission_scale: " << glm::to_string(
			value.get_emission_scale_offset()[0]);
		str << " emission_offset: " << glm::to_string(
			value.get_emission_scale_offset()[1]);
		str << " specular_scale_offset: " << glm::to_string(
			value.get_specular_scale_offset());

		str << " cast_shadows: " << value.get_cast_shadows();
		str << " culling: " << value.get_culling();

		return str;
	}

}
