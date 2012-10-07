/****************************************************************************
 *            effectdescription.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "effectdescription.hpp"
#include "xmlwriter.hpp"
#include "xmlreader.hpp"
#include "xmlutil.hpp"

namespace eternal_lands
{

	EffectDescription::EffectDescription(): m_description(edt_default),
		m_output(sot_float), m_receives_shadows(true),
		m_transparent(false)
	{
	}

	EffectDescription::~EffectDescription() noexcept
	{
	}

	String EffectDescription::get_xml_id()
	{
		return String(UTF8("effect"));
	}

	void EffectDescription::load_xml(const FileSystemSharedPtr &file_system,
		const String &file_name)
	{
		XmlReaderSharedPtr xml_reader;

		xml_reader = XmlReaderSharedPtr(new XmlReader(file_system,
			file_name));

		load_xml(xml_reader->get_root_node());
	}

	void EffectDescription::load_xml(const xmlNodePtr node)
	{
		xmlNodePtr it;

		if (xmlStrcmp(node->name, BAD_CAST get_xml_id().get().c_str())
			!= 0)
		{
			return;
		}

		it = XmlUtil::children(node, true);

		do
		{
			if (xmlStrcmp(it->name, BAD_CAST UTF8("name")) == 0)
			{
				set_name(XmlUtil::get_string_value(it));
			}

			if (xmlStrcmp(it->name,
				BAD_CAST UTF8("world_transformation")) == 0)
			{
				set_world_transformation(
					XmlUtil::get_string_value(it));
			}

			if (xmlStrcmp(it->name,
				BAD_CAST UTF8("texture_coodrinates")) == 0)
			{
				set_texture_coodrinates(
					XmlUtil::get_string_value(it));
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("main")) == 0)
			{
				set_main(XmlUtil::get_string_value(it));
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("lighting")) == 0)
			{
				set_lighting(XmlUtil::get_string_value(it));
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("description"))
				== 0)
			{
				set_description(EffectDescriptionUtil::
					get_effect_description(
						XmlUtil::get_string_value(it)));
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("output")) == 0)
			{
				set_output(ShaderOutputUtil::get_shader_output(
					XmlUtil::get_string_value(it)));
			}

			if (xmlStrcmp(it->name,
				BAD_CAST UTF8("receives_shadows")) == 0)
			{
				set_receives_shadows(
					XmlUtil::get_bool_value(it));
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("transparent"))
				== 0)
			{
				set_transparent(XmlUtil::get_bool_value(it));
			}
		}
		while (XmlUtil::next(it, true));
	}

	void EffectDescription::save_xml(
		const XmlWriterSharedPtr &writer) const
	{
		writer->start_element(get_xml_id());
		writer->write_element(String(UTF8("name")), get_name());
		writer->write_element(String(UTF8("world_transformation")),
			get_world_transformation());
		writer->write_element(String(UTF8("texture_coodrinates")),
			get_texture_coodrinates());
		writer->write_element(String(UTF8("main")), get_main());
		writer->write_element(String(UTF8("lighting")), get_lighting());
		writer->write_element(String(UTF8("description")),
			EffectDescriptionUtil::get_str(get_description()));
		writer->write_element(String(UTF8("output")),
			ShaderOutputUtil::get_str(get_output()));
		writer->write_bool_element(String(UTF8("receives_shadows")),
			get_receives_shadows());
		writer->write_bool_element(String(UTF8("transparent")),
			get_transparent());
		writer->end_element();
	}

	bool EffectDescription::operator==(const EffectDescription &effect)
		const
	{
		if (get_world_transformation() !=
			effect.get_world_transformation())
		{
			return false;
		}

		if (get_texture_coodrinates() !=
			effect.get_texture_coodrinates())
		{
			return false;
		}

		if (get_main() != effect.get_main())
		{
			return false;
		}

		if (get_lighting() != effect.get_lighting())
		{
			return false;
		}

		if (get_receives_shadows() != effect.get_receives_shadows())
		{
			return false;
		}

		return get_transparent() == effect.get_transparent();
	}

	bool EffectDescription::operator!=(const EffectDescription &effect)
		const
	{
		return !operator==(effect);
	}

	bool EffectDescription::operator<(const EffectDescription &effect)
		const
	{
		if (get_world_transformation() !=
			effect.get_world_transformation())
		{
			return get_world_transformation() <
				effect.get_world_transformation();
		}

		if (get_texture_coodrinates() !=
			effect.get_texture_coodrinates())
		{
			return get_texture_coodrinates() <
				effect.get_texture_coodrinates();
		}

		if (get_main() != effect.get_main())
		{
			return get_main() < effect.get_main();
		}

		if (get_lighting() != effect.get_lighting())
		{
			return get_lighting() < effect.get_lighting();
		}

		if (get_receives_shadows() != effect.get_receives_shadows())
		{
			return get_receives_shadows() <
				effect.get_receives_shadows();
		}

		return get_transparent() < effect.get_transparent();
	}

	OutStream &operator<<(OutStream &str, const EffectDescription &value)
	{
		str << "name: " << value.get_name();
		str << " world_transformation: ";
		str << value.get_world_transformation();
		str << " texture_coodrinates: ";
		str << value.get_texture_coodrinates();
		str << " main: " << value.get_main();
		str << " lighting: " << value.get_lighting();
		str << " receives_shadows: " << value.get_receives_shadows();
		str << " transparent: " << value.get_transparent();

		return str;
	}

}
