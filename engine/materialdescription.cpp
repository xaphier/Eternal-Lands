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
			if (xmlStrcmp(it->name, BAD_CAST UTF8("name")) == 0)
			{
				set_material_name(
					XmlUtil::get_string_value(it));
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("diffuse"))
				== 0)
			{
				set_texture(XmlUtil::get_string_value(it),
					stt_diffuse_0);
			}

			if (xmlStrcmp(it->name,
				BAD_CAST UTF8("second_diffuse")) == 0)
			{
				set_texture(XmlUtil::get_string_value(it),
					stt_diffuse_1);
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

			if (xmlStrcmp(it->name,
				BAD_CAST UTF8("texture_coodrinates")) == 0)
			{
				set_texture_coodrinates(
					XmlUtil::get_string_value(it));
			}

			if (xmlStrcmp(it->name,
				BAD_CAST UTF8("diffuse_mapping")) == 0)
			{
				set_diffuse_mapping(
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
		writer->write_element(UTF8("diffuse"),
			get_texture(stt_diffuse_0));
		writer->write_element(UTF8("second_diffuse"),
			get_texture(stt_diffuse_1));
		writer->write_element(UTF8("normal"),
			get_texture(stt_normal_0));
		writer->write_element(UTF8("specular"),
			get_texture(stt_specular_0));
		writer->write_element(UTF8("emission"),
			get_texture(stt_emission_0));
		writer->write_element(UTF8("blend"), get_texture(stt_blend_0));
		writer->write_element(UTF8("texture_coodrinates"),
			get_texture_coodrinates());
		writer->write_element(UTF8("diffuse_mapping"),
			get_diffuse_mapping());
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
