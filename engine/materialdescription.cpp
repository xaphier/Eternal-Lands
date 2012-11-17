/****************************************************************************
 *            materialdescription.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "materialdescription.hpp"
#include "exceptions.hpp"
#include "xmlutil.hpp"
#include "xmlwriter.hpp"
#include "filesystem.hpp"

namespace eternal_lands
{

	MaterialDescription::MaterialDescription()
	{
	}

	MaterialDescription::~MaterialDescription() noexcept
	{
	}

	void MaterialDescription::load_xml(const xmlNodePtr node)
	{
		xmlNodePtr it;
		Uint32 i;
		SamplerParameterType sampler;

		if (node == 0)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_message(UTF8("parameter is zero"))
				<< errinfo_parameter_name(UTF8("node")));
		}

		if (xmlStrcmp(node->name, BAD_CAST UTF8("material")) != 0)
		{
			return;
		}

		it = XmlUtil::children(node, true);

		do
		{
			for (i = 0; i < m_textures.size(); ++i)
			{
				sampler = static_cast<SamplerParameterType>(i);

				if (xmlStrcmp(it->name, BAD_CAST
					SamplerParameterUtil::get_str(
						sampler).get().c_str()) == 0)
				{
					set_texture(XmlUtil::get_string_value(
						it), sampler);
					set_sRGB(XmlUtil::get_bool_property(it,
						String(UTF8("sRGB"))), sampler);
				}
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("name")) == 0)
			{
				set_name(XmlUtil::get_string_value(it));
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("effect")) == 0)
			{
				set_effect(XmlUtil::get_string_value(it));
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("script")) == 0)
			{
				set_script(XmlUtil::get_string_value(it));
			}

			if (xmlStrcmp(it->name,
				BAD_CAST UTF8("texture_matrix_0")) == 0)
			{
				set_texture_matrix(
					XmlUtil::get_mat2x3_value(it), 0);
			}

			if (xmlStrcmp(it->name,
				BAD_CAST UTF8("texture_matrix_1")) == 0)
			{
				set_texture_matrix(
					XmlUtil::get_mat2x3_value(it), 1);
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("color")) == 0)
			{
				set_color(XmlUtil::get_vec4_value(it));
			}

			if (xmlStrcmp(it->name,
				BAD_CAST UTF8("dudv_scale_offset")) == 0)
			{
				set_dudv_scale_offset(XmlUtil::get_vec4_value(
					it));
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("cast_shadows"))
				== 0)
			{
				set_cast_shadows(XmlUtil::get_bool_value(it));
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("culling")) == 0)
			{
				set_culling(XmlUtil::get_bool_value(it));
			}
		}
		while (XmlUtil::next(it, true));
	}

	void MaterialDescription::save_xml(
		const XmlWriterSharedPtr &writer) const
	{
		Uint32 i;
		SamplerParameterType sampler;

		writer->start_element(String(UTF8("material")));
		writer->write_element(String(UTF8("name")), get_name());
		writer->write_element(String(UTF8("effect")), get_effect());
		writer->write_element(String(UTF8("script")), get_script());

		for (i = 0; i < m_textures.size(); ++i)
		{
			sampler = static_cast<SamplerParameterType>(i);

			writer->start_element(SamplerParameterUtil::get_str(
				sampler));
			writer->write_bool_property(String(UTF8("sRGB")),
				get_sRGB(sampler));
			writer->write_string(get_texture(sampler));
			writer->end_element();
		}

		writer->write_mat2x3_element(String(UTF8("texture_matrix_0")),
			get_texture_matrix(0));
		writer->write_mat2x3_element(String(UTF8("texture_matrix_1")),
			get_texture_matrix(1));
		writer->write_vec4_element(String(UTF8("color")), get_color());
		writer->write_vec4_element(String(UTF8("dudv_scale_offset")),
			get_dudv_scale_offset());
		writer->write_bool_element(String(UTF8("cast_shadows")),
			get_cast_shadows());
		writer->write_bool_element(String(UTF8("culling")),
			get_culling());
		writer->end_element();
	}

	bool MaterialDescription::operator<(const MaterialDescription &material)
		const
	{
		glm::bvec4 cmp4;
		glm::bvec3 cmp3;
		Uint16 i, j;

		if (get_effect() != material.get_effect())
		{
			return get_effect() < material.get_effect();
		}

		for (i = 0; i < material_texture_count; ++i)
		{
			if (get_texture(i) != material.get_texture(i))
			{
				return get_texture(i) <
					material.get_texture(i);
			}
		}

		for (i = 0; i < 2; ++i)
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

		for (i = 0; i < 4; ++i)
		{
			if (get_color()[i] != material.get_color()[i])
			{
				
				return get_color()[i] < material.get_color()[i];
			}
		}

		if (get_cast_shadows() != material.get_cast_shadows())
		{
			return get_cast_shadows() < material.get_cast_shadows();
		}

		if (get_culling() != material.get_culling())
		{
			return get_culling() < material.get_culling();
		}

		if (get_script() != material.get_script())
		{
			return get_script() < material.get_script();
		}

		return get_name() < material.get_name();
	}

	bool MaterialDescription::operator==(
		const MaterialDescription &material) const
	{
		Uint16 i;

		if (get_effect() != material.get_effect())
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

		for (i = 0; i < 2; ++i)
		{
			if (get_texture_matrix(i) !=
				material.get_texture_matrix(i))
			{
				return false;
			}
		}

		if (get_color() != material.get_color())
		{
			return false;
		}

		if (get_cast_shadows() != material.get_cast_shadows())
		{
			return false;
		}

		if (get_culling() != material.get_culling())
		{
			return false;
		}

		if (get_script() != material.get_script())
		{
			return false;
		}

		return get_name() == material.get_name();
	}

	bool MaterialDescription::operator!=(
		const MaterialDescription &material) const
	{
		return !operator==(material);
	}

	OutStream &operator<<(OutStream &str, const MaterialDescription &value)
	{
		SamplerParameterType sampler;
		Uint16 i;

		str << "name: " << value.get_name();
		str << " effect: " << value.get_effect();
		str << " script: " << value.get_script();

		for (i = 0; i < material_texture_count; ++i)
		{
			sampler = static_cast<SamplerParameterType>(i);
			str << " " << sampler << ": ";
			str << value.get_texture(sampler);
		}

		for (i = 0; i < 2; ++i)
		{
			str << " texture_matrix: " << glm::to_string(
				value.get_texture_matrix(i));
		}

		str << " color: " << glm::to_string(value.get_color());
		str << " cast_shadows: " << value.get_cast_shadows();
		str << " culling: " << value.get_culling();

		return str;
	}

}
