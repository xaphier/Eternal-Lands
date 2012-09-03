/****************************************************************************
 *            effecttextureunit.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "effecttextureunit.hpp"
#include "xmlreader.hpp"
#include "xmlutil.hpp"
#include "xmlwriter.hpp"
#include "exceptions.hpp"

namespace eternal_lands
{

	EffectTextureUnit::EffectTextureUnit(): m_target(ttt_texture_2d)
	{
	}

	EffectTextureUnit::~EffectTextureUnit() noexcept
	{
	}

	void EffectTextureUnit::save_xml(const XmlWriterSharedPtr &writer)
		const
	{
		writer->start_element(String(UTF8("effect_texture_unit")));

		writer->write_element(String(UTF8("name")), get_name());

		writer->start_element(String(UTF8("default_texture")));

		BOOST_FOREACH(const String &file, get_default_texture())
		{
			writer->write_element(String(UTF8("file")), file);
		}

		writer->end_element();

		writer->write_element(String(UTF8("target")),
			TextureTargetUtil::get_str(get_target()));

		writer->end_element();
	}

	void EffectTextureUnit::load_default_texture_xml(const xmlNodePtr node)
	{
		xmlNodePtr it;

		if (node == 0)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_message(UTF8("parameter is zero"))
				<< errinfo_parameter_name(UTF8("node")));
		}

		if (xmlStrcmp(node->name, BAD_CAST UTF8("default_texture"))
			!= 0)
		{
			return;
		}

		it = XmlUtil::children(node, true);

		m_default_texture.clear();

		do
		{
			if (xmlStrcmp(it->name,
				BAD_CAST UTF8("file")) == 0)
			{
				m_default_texture.push_back(
					XmlUtil::get_string_value(it));
			}
		}
		while (XmlUtil::next(it, true));
	}

	void EffectTextureUnit::load_xml(const xmlNodePtr node)
	{
		xmlNodePtr it;

		if (node == 0)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_message(UTF8("parameter is zero"))
				<< errinfo_parameter_name(UTF8("node")));
		}

		if (xmlStrcmp(node->name, BAD_CAST UTF8("effect_texture_unit"))
			!= 0)
		{
			return;
		}

		it = XmlUtil::children(node, true);

		do
		{
			if (xmlStrcmp(it->name,
				BAD_CAST UTF8("name")) == 0)
			{
				set_name(XmlUtil::get_string_value(it));
			}

			if (xmlStrcmp(it->name,
				BAD_CAST UTF8("default_texture")) == 0)
			{
				load_default_texture_xml(it);
			}

			if (xmlStrcmp(it->name,
				BAD_CAST UTF8("target")) == 0)
			{
				set_target(
					TextureTargetUtil::get_texture_target(
						XmlUtil::get_string_value(it)));
			}
		}
		while (XmlUtil::next(it, true));
	}

}
