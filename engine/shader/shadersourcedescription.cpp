/****************************************************************************
 *            shadersourcedescription.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "shadersourcedescription.hpp"
#include "xmlutil.hpp"
#include "xmlwriter.hpp"

namespace eternal_lands
{

	ShaderSourceDescription::ShaderSourceDescription():
		m_receives_shadows(true), m_transparent(false)
	{
	}

	ShaderSourceDescription::~ShaderSourceDescription() throw()
	{
	}

	void ShaderSourceDescription::load_xml(const xmlNodePtr node)
	{
		xmlNodePtr it;

		if (xmlStrcmp(node->name,
			BAD_CAST UTF8("shader_source_description")) != 0)
		{
			return;
		}

		it = XmlUtil::children(node, true);

		do
		{
			if (xmlStrcmp(it->name,
				BAD_CAST UTF8("world_transform")) == 0)
			{
				set_world_transform(
					XmlUtil::get_string_value(it));
			}

			if (xmlStrcmp(it->name,
				BAD_CAST UTF8("texture_coodrinates")) == 0)
			{
				set_texture_coodrinates(
					XmlUtil::get_string_value(it));
			}

			if (xmlStrcmp(it->name,
				BAD_CAST UTF8("normal_mapping")) == 0)
			{
				set_normal_mapping(
					XmlUtil::get_string_value(it));
			}

			if (xmlStrcmp(it->name,
				BAD_CAST UTF8("diffuse_mapping")) == 0)
			{
				set_diffuse_mapping(
					XmlUtil::get_string_value(it));
			}

			if (xmlStrcmp(it->name,
				BAD_CAST UTF8("specular_mapping")) == 0)
			{
				set_specular_mapping(
					XmlUtil::get_string_value(it));
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("light")) == 0)
			{
				set_light(XmlUtil::get_string_value(it));
			}

			if (xmlStrcmp(it->name,
				BAD_CAST UTF8("receives_shadows")) == 0)
			{
				set_receives_shadows(
					XmlUtil::get_bool_value(it));
			}

			if (xmlStrcmp(it->name,
				BAD_CAST UTF8("transparent")) == 0)
			{
				set_transparent(XmlUtil::get_bool_value(it));
			}
		}
		while (XmlUtil::next(it, true));
	}

	void ShaderSourceDescription::save_xml(
		const XmlWriterSharedPtr &writer) const
	{
		writer->start_element(UTF8("shader_source_description"));
		writer->write_element(UTF8("world_transform"),
			get_world_transform());
		writer->write_element(UTF8("texture_coodrinates"),
			get_texture_coodrinates());
		writer->write_element(UTF8("diffuse_mapping"),
			get_diffuse_mapping());
		writer->write_element(UTF8("normal_mapping"),
			get_normal_mapping());
		writer->write_element(UTF8("specular_mapping"),
			get_specular_mapping());
		writer->write_element(UTF8("light"), get_light());
		writer->write_bool_element(UTF8("receives_shadows"),
			get_receives_shadows());
		writer->write_bool_element(UTF8("transparent"),
			get_transparent());
		writer->end_element();
	}

}
