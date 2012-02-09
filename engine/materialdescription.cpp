/****************************************************************************
 *            materialdescription.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "materialdescription.hpp"
#include "exceptions.hpp"
#include "xmlutil.hpp"
#include "xmlwriter.hpp"
#include "filesystem.hpp"

namespace eternal_lands
{

	MaterialDescription::MaterialDescription(): m_receives_shadows(true),
		m_lighting(true)
	{
		Uint16 i;

		for (i = 0; i < 4; ++i)
		{
			m_albedo_scale_offsets[i][0] = glm::vec4(1.0f);
			m_albedo_scale_offsets[i][1] = glm::vec4(0.0f);
			m_texture_matrices[i][0] = glm::vec3(1.0f, 0.0f, 0.0f);
			m_texture_matrices[i][1] = glm::vec3(0.0f, 1.0f, 0.0f);
		}

		m_emission_scale_offset[0] = glm::vec3(1.0f);
		m_emission_scale_offset[1] = glm::vec3(0.0f);
		m_specular_scale_offset = glm::vec4(1.0f, 1.0f, 0.0f, 0.0f);
	}

	MaterialDescription::~MaterialDescription() throw()
	{
	}

	void MaterialDescription::load_xml(const xmlNodePtr node)
	{
		xmlNodePtr it;

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
			if (xmlStrcmp(it->name, BAD_CAST UTF8("albedo_0"))
				== 0)
			{
				set_texture(XmlUtil::get_string_value(it),
					stt_albedo_0);
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("albedo_1"))
				== 0)
			{
				set_texture(XmlUtil::get_string_value(it),
					stt_albedo_1);
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("albedo_2"))
				== 0)
			{
				set_texture(XmlUtil::get_string_value(it),
					stt_albedo_2);
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("albedo_3"))
				== 0)
			{
				set_texture(XmlUtil::get_string_value(it),
					stt_albedo_3);
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("normal"))
				== 0)
			{
				set_texture(XmlUtil::get_string_value(it),
					stt_normal_0);
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("specular"))
				== 0)
			{
				set_texture(XmlUtil::get_string_value(it),
					stt_specular_0);
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("emission")) == 0)
			{
				set_texture(XmlUtil::get_string_value(it),
					stt_emission_0);
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("blend")) == 0)
			{
				set_texture(XmlUtil::get_string_value(it),
					stt_blend_0);
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("height")) == 0)
			{
				set_texture(XmlUtil::get_string_value(it),
					stt_height);
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("name")) == 0)
			{
				set_material_name(
					XmlUtil::get_string_value(it));
			}

			if (xmlStrcmp(it->name,
				BAD_CAST UTF8("albedo_scale_offset_0")) == 0)
			{
				set_albedo_scale_offset(
					XmlUtil::get_mat2x4_value(it), 0);
			}

			if (xmlStrcmp(it->name,
				BAD_CAST UTF8("albedo_scale_offset_1")) == 0)
			{
				set_albedo_scale_offset(
					XmlUtil::get_mat2x4_value(it), 1);
			}

			if (xmlStrcmp(it->name,
				BAD_CAST UTF8("albedo_scale_offset_2")) == 0)
			{
				set_albedo_scale_offset(
					XmlUtil::get_mat2x4_value(it), 2);
			}

			if (xmlStrcmp(it->name,
				BAD_CAST UTF8("albedo_scale_offset_3")) == 0)
			{
				set_albedo_scale_offset(
					XmlUtil::get_mat2x4_value(it), 3);
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

			if (xmlStrcmp(it->name,
				BAD_CAST UTF8("texture_matrix_2")) == 0)
			{
				set_texture_matrix(
					XmlUtil::get_mat2x3_value(it), 2);
			}

			if (xmlStrcmp(it->name,
				BAD_CAST UTF8("texture_matrix_3")) == 0)
			{
				set_texture_matrix(
					XmlUtil::get_mat2x3_value(it), 3);
			}

			if (xmlStrcmp(it->name,
				BAD_CAST UTF8("emission_scale_offset")) == 0)
			{
				set_emission_scale_offset(
					XmlUtil::get_mat2x3_value(it));
			}

			if (xmlStrcmp(it->name,
				BAD_CAST UTF8("specular_scale_offset")) == 0)
			{
				set_specular_scale_offset(
					XmlUtil::get_vec4_value(it));
			}

			if (xmlStrcmp(it->name,
				BAD_CAST UTF8("texture_coodrinates")) == 0)
			{
				set_texture_coodrinates(
					XmlUtil::get_string_value(it));
			}

			if (xmlStrcmp(it->name,
				BAD_CAST UTF8("albedo_mapping")) == 0)
			{
				set_albedo_mapping(
					XmlUtil::get_string_value(it));
			}

			if (xmlStrcmp(it->name,
				BAD_CAST UTF8("normal_mapping")) == 0)
			{
				set_normal_mapping(
					XmlUtil::get_string_value(it));
			}

			if (xmlStrcmp(it->name,
				BAD_CAST UTF8("specular_mapping")) == 0)
			{
				set_specular_mapping(
					XmlUtil::get_string_value(it));
			}

			if (xmlStrcmp(it->name,
				BAD_CAST UTF8("emission_mapping")) == 0)
			{
				set_emission_mapping(
					XmlUtil::get_string_value(it));
			}

			if (xmlStrcmp(it->name,
				BAD_CAST UTF8("receives_shadows")) == 0)
			{
				set_receives_shadows(
					XmlUtil::get_bool_value(it));
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("lighting")) == 0)
			{
				set_lighting(XmlUtil::get_bool_value(it));
			}
		}
		while (XmlUtil::next(it, true));
	}

	void MaterialDescription::save_xml(
		const XmlWriterSharedPtr &writer) const
	{
		writer->start_element(UTF8("material"));
		writer->write_element(UTF8("name"), get_material_name());
		writer->write_element(UTF8("albedo_0"),
			get_texture(stt_albedo_0));
		writer->write_element(UTF8("albedo_1"),
			get_texture(stt_albedo_1));
		writer->write_element(UTF8("albedo_2"),
			get_texture(stt_albedo_2));
		writer->write_element(UTF8("albedo_3"),
			get_texture(stt_albedo_3));
		writer->write_element(UTF8("normal"),
			get_texture(stt_normal_0));
		writer->write_element(UTF8("specular"),
			get_texture(stt_specular_0));
		writer->write_element(UTF8("emission"),
			get_texture(stt_emission_0));
		writer->write_element(UTF8("blend"), get_texture(stt_blend_0));
		writer->write_element(UTF8("height"), get_texture(stt_height));
		writer->write_mat2x4_element(UTF8("albedo_scale_offset_0"),
			get_albedo_scale_offset(0));
		writer->write_mat2x4_element(UTF8("albedo_scale_offset_1"),
			get_albedo_scale_offset(1));
		writer->write_mat2x4_element(UTF8("albedo_scale_offset_2"),
			get_albedo_scale_offset(2));
		writer->write_mat2x4_element(UTF8("albedo_scale_offset_3"),
			get_albedo_scale_offset(3));
		writer->write_mat2x3_element(UTF8("texture_matrix_0"),
			get_texture_matrix(0));
		writer->write_mat2x3_element(UTF8("texture_matrix_1"),
			get_texture_matrix(1));
		writer->write_mat2x3_element(UTF8("texture_matrix_2"),
			get_texture_matrix(2));
		writer->write_mat2x3_element(UTF8("texture_matrix_3"),
			get_texture_matrix(3));
		writer->write_mat2x3_element(UTF8("emission_scale_offset"),
			get_emission_scale_offset());
		writer->write_vec4_element(UTF8("specular_scale_offset"),
			get_specular_scale_offset());
		writer->write_element(UTF8("texture_coodrinates"),
			get_texture_coodrinates());
		writer->write_element(UTF8("albedo_mapping"),
			get_albedo_mapping());
		writer->write_element(UTF8("normal_mapping"),
			get_normal_mapping());
		writer->write_element(UTF8("specular_mapping"),
			get_specular_mapping());
		writer->write_element(UTF8("emission_mapping"),
			get_emission_mapping());
		writer->write_bool_element(UTF8("receives_shadows"),
			get_receives_shadows());
		writer->write_bool_element(UTF8("lighting"), get_lighting());
		writer->end_element();
	}

}
