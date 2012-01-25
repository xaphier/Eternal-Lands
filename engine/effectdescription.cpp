/****************************************************************************
 *            effectdescription.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "effectdescription.hpp"
#include "materialeffectdescription.hpp"
#include "xmlutil.hpp"
#include "xmlwriter.hpp"

namespace eternal_lands
{

	EffectDescription::EffectDescription(): m_receives_shadows(true),
		m_transparent(false), m_lighting(true)
	{
	}

	EffectDescription::EffectDescription(
		const MaterialEffectDescription &material):
		m_receives_shadows(true), m_transparent(false), m_lighting(true)
	{
		set_world_transform(material.get_world_transform());
		set_texture_coodrinates(material.get_texture_coodrinates());
		set_diffuse_mapping(material.get_diffuse_mapping());
		set_normal_mapping(material.get_normal_mapping());
		set_specular_mapping(material.get_specular_mapping());
		set_emission_mapping(material.get_emission_mapping());
		set_receives_shadows(material.get_receives_shadows());
		set_transparent(material.get_transparent());
		set_lighting(material.get_lighting());
	}

	EffectDescription::~EffectDescription() throw()
	{
	}

	void EffectDescription::load_xml(const xmlNodePtr node)
	{
		xmlNodePtr it;

		if (xmlStrcmp(node->name,
			BAD_CAST UTF8("effect")) != 0)
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

			if (xmlStrcmp(it->name,
				BAD_CAST UTF8("transparent")) == 0)
			{
				set_transparent(XmlUtil::get_bool_value(it));
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("lighting")) == 0)
			{
				set_lighting(XmlUtil::get_bool_value(it));
			}
		}
		while (XmlUtil::next(it, true));
	}

	void EffectDescription::save_xml(
		const XmlWriterSharedPtr &writer) const
	{
		writer->start_element(UTF8("effect"));
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
		writer->write_element(UTF8("emission_mapping"),
			get_emission_mapping());
		writer->write_bool_element(UTF8("receives_shadows"),
			get_receives_shadows());
		writer->write_bool_element(UTF8("transparent"),
			get_transparent());
		writer->write_bool_element(UTF8("lighting"), get_lighting());
		writer->end_element();
	}

	bool EffectDescription::operator==(const EffectDescription &effect)
		const
	{
		if (get_world_transform() != effect.get_world_transform())
		{
			return false;
		}

		if (get_texture_coodrinates() !=
			effect.get_texture_coodrinates())
		{
			return false;
		}

		if (get_diffuse_mapping() != effect.get_diffuse_mapping())
		{
			return false;
		}

		if (get_normal_mapping() != effect.get_normal_mapping())
		{
			return false;
		}

		if (get_specular_mapping() != effect.get_specular_mapping())
		{
			return false;
		}

		if (get_emission_mapping() != effect.get_emission_mapping())
		{
			return false;
		}

		if (get_receives_shadows() != effect.get_receives_shadows())
		{
			return false;
		}

		if (get_transparent() != effect.get_transparent())
		{
			return false;
		}

		return get_lighting() == effect.get_lighting();
	}

	bool EffectDescription::operator!=(const EffectDescription &effect)
		const
	{
		return !operator==(effect);
	}

	bool EffectDescription::operator<(const EffectDescription &effect)
		const
	{
		if (get_world_transform() != effect.get_world_transform())
		{
			return get_world_transform() <
				effect.get_world_transform();
		}

		if (get_texture_coodrinates() !=
			effect.get_texture_coodrinates())
		{
			return get_texture_coodrinates() <
				effect.get_texture_coodrinates();
		}

		if (get_diffuse_mapping() != effect.get_diffuse_mapping())
		{
			return get_diffuse_mapping() <
				effect.get_diffuse_mapping();
		}

		if (get_normal_mapping() != effect.get_normal_mapping())
		{
			return get_normal_mapping() <
				effect.get_normal_mapping();
		}

		if (get_specular_mapping() != effect.get_specular_mapping())
		{
			return get_specular_mapping() <
				effect.get_specular_mapping();
		}

		if (get_emission_mapping() != effect.get_emission_mapping())
		{
			return get_emission_mapping() <
				effect.get_emission_mapping();
		}

		if (get_receives_shadows() != effect.get_receives_shadows())
		{
			return get_receives_shadows() <
				effect.get_receives_shadows();
		}

		if (get_transparent() != effect.get_transparent())
		{
			return get_transparent() < effect.get_transparent();
		}

		return get_lighting() < effect.get_lighting();
	}

	OutStream& operator<<(OutStream &str, const EffectDescription &value)
	{
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

		return str;
	}

}
