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
		m_output(sot_float), m_node_based(true),
		m_receives_shadows(true), m_transparent(false),
		m_lighting(true)
	{
	}

	EffectDescription::~EffectDescription() noexcept
	{
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

		if (xmlStrcmp(node->name,
			BAD_CAST UTF8("effect")) != 0)
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

			if (xmlStrcmp(it->name,
				BAD_CAST UTF8("node_based")) == 0)
			{
				set_node_based(XmlUtil::get_bool_value(it));
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
		writer->write_element(UTF8("name"), get_name());
		writer->write_element(UTF8("world_transformation"),
			get_world_transformation());
		writer->write_element(UTF8("texture_coodrinates"),
			get_texture_coodrinates());
		writer->write_element(UTF8("main"), get_main());
		writer->write_bool_element(UTF8("node_based"),
			get_node_based());
		writer->write_element(UTF8("description"),
			EffectDescriptionUtil::get_str(get_description()));
		writer->write_element(UTF8("output"),
			ShaderOutputUtil::get_str(get_output()));
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

		if (get_node_based() != effect.get_node_based())
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

		if (get_node_based() != effect.get_node_based())
		{
			return get_node_based() < effect.get_node_based();
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
		str << "name: " << value.get_name();
		str << " world_transformation: ";
		str << value.get_world_transformation();
		str << " texture_coodrinates: ";
		str << value.get_texture_coodrinates();
		str << " main: " << value.get_main();
		str << " node_based: " << value.get_node_based();
		str << " receives_shadows: " << value.get_receives_shadows();
		str << " transparent: " << value.get_transparent();
		str << " lighting: " << value.get_lighting();

		return str;
	}

}
