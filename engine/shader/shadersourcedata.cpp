/****************************************************************************
 *            shadersourcedata.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "shadersourcedata.hpp"
#include "samplerparameterutil.hpp"
#include "shadersourceparameterbuilder.hpp"
#include "shadersourceparameter.hpp"
#include "xmlwriter.hpp"
#include "xmlutil.hpp"
#include "utf.hpp"

namespace eternal_lands
{

	ShaderSourceData::ShaderSourceData(): m_version(svt_120)
	{
	}

	ShaderSourceData::ShaderSourceData(const String &source,
		const xmlNodePtr node): m_version(svt_120)
	{
		load_xml(source, node);
	}

	ShaderSourceData::~ShaderSourceData() noexcept
	{
	}

	void ShaderSourceData::load_parameters_xml(const String &source,
		const xmlNodePtr node)
	{
		xmlNodePtr it;

		m_parameters.clear();

		if (!XmlUtil::has_children(node, true))
		{
			return;
		}

		it = XmlUtil::children(node, true);

		do
		{
			if (xmlStrcmp(it->name, BAD_CAST UTF8("parameter"))
				== 0)
			{
				m_parameters.push_back(ShaderSourceParameter(
					source, it));
			}
		}
		while (XmlUtil::next(it, true));
	}

	void ShaderSourceData::load_xml(const String &source,
		const xmlNodePtr node)
	{
		xmlNodePtr it;

		if (xmlStrcmp(node->name, BAD_CAST UTF8("shader_source_data"))
			!= 0)
		{
			return;
		}

		it = XmlUtil::children(node, true);

		do
		{
			if (xmlStrcmp(it->name, BAD_CAST UTF8("parameters"))
				== 0)
			{
				load_parameters_xml(source, it);
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("source")) == 0)
			{
				set_source(XmlUtil::get_string_value(it));
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("version")) == 0)
			{
				set_version(
					ShaderVersionUtil::get_shader_version(
						XmlUtil::get_string_value(it)));
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("shader_types"))
				== 0)
			{
				set_shader_types(ShaderUtil::get_shaders(
					XmlUtil::get_string_value(it)));
			}
		}
		while (XmlUtil::next(it, true));
	}

	void ShaderSourceData::save_xml(const XmlWriterSharedPtr &writer) const
	{
		writer->start_element(String(UTF8("shader_source_data")));

		writer->start_element(String(UTF8("parameters")));

		BOOST_FOREACH(const ShaderSourceParameter &parameter,
			get_parameters())
		{
			parameter.save_xml(writer);
		}

		writer->end_element();

		writer->write_element(String(UTF8("source")), get_source());
		writer->write_element(String(UTF8("version")),
			ShaderVersionUtil::get_str(get_version()));
		writer->write_element(String(UTF8("shader_types")),
			ShaderUtil::get_str(get_shader_types()));

		writer->end_element();
	}

	void ShaderSourceData::set_parameters(
		const ShaderSourceParameterVector &parameters)
	{
		m_parameters = parameters;
	}

}
