/****************************************************************************
 *            shadersource.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "shadersource.hpp"
#include "exceptions.hpp"
#include "logging.hpp"
#include "samplerparameterutil.hpp"
#include "shadersourceparameterbuilder.hpp"
#include "shadersourceparameter.hpp"
#include "shadersourcedata.hpp"
#include "xmlwriter.hpp"
#include "xmlreader.hpp"
#include "xmlutil.hpp"

namespace eternal_lands
{

	namespace
	{

		inline bool sort_shader_source_data(
			const ShaderSourceData &shader_source_data_0,
			const ShaderSourceData &shader_source_data_1)
		{
			return  shader_source_data_0.get_version() >
				shader_source_data_1.get_version();
		}

	}

	ShaderSource::ShaderSource()
	{
	}

	ShaderSource::~ShaderSource() noexcept
	{
	}

	void ShaderSource::load_datas_xml(const String &source,
		const xmlNodePtr node)
	{
		xmlNodePtr it;

		if (xmlStrcmp(node->name, BAD_CAST UTF8("shader_source_datas"))
			!= 0)
		{
			return;
		}

		m_datas.clear();

		if (!XmlUtil::has_children(node, true))
		{
			return;
		}

		it = XmlUtil::children(node, true);

		do
		{
			if (xmlStrcmp(it->name,
				BAD_CAST UTF8("shader_source_data")) == 0)
			{
				m_datas.push_back(ShaderSourceData(source, it));
			}
		}
		while (XmlUtil::next(it, true));

		std::sort(m_datas.begin(), m_datas.end(),
			sort_shader_source_data);
	}

	void ShaderSource::do_load_xml(const xmlNodePtr node)
	{
		xmlNodePtr it;
		String source;

		if (node == 0)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_message(UTF8("parameter is zero"))
				<< errinfo_parameter_name(UTF8("node")));
		}

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

			if (xmlStrcmp(it->name, BAD_CAST UTF8("type")) == 0)
			{
				set_type(ShaderSourceUtil::get_shader_source(
					XmlUtil::get_string_value(it)));
			}
		}
		while (XmlUtil::next(it, true));

		source = get_typed_name();

		it = XmlUtil::children(node, true);

		do
		{
			if (xmlStrcmp(it->name,
				BAD_CAST UTF8("shader_source_datas")) == 0)
			{
				load_datas_xml(source, it);
			}
		}
		while (XmlUtil::next(it, true));
	}

	void ShaderSource::do_save_xml(const XmlWriterSharedPtr &xml_writer)
		const
	{
		xml_writer->start_element(get_xml_id());

		xml_writer->write_element(String(UTF8("name")), get_name());

		xml_writer->write_element(String(UTF8("type")),
			ShaderSourceUtil::get_str(get_type()));

		xml_writer->start_element(String(UTF8("shader_source_datas")));

		BOOST_FOREACH(const ShaderSourceData &data, get_datas())
		{
			data.save_xml(xml_writer);
		}

		xml_writer->end_element();

		xml_writer->end_element();
	}

	ShaderSourceParameterVector ShaderSource::get_parameters(
		const ShaderVersionType version) const
	{
		return get_data(version).get_parameters();
	}

	String ShaderSource::get_source(const ShaderVersionType version) const
	{
		return get_data(version).get_source();
	}

	bool ShaderSource::get_has_data(const ShaderVersionType version)
		const
	{
		BOOST_FOREACH(const ShaderSourceData &data, get_datas())
		{
			if (data.get_version() <= version)
			{
				return true;
			}
		}

		return false;
	}

	const ShaderSourceData &ShaderSource::get_data(
		const ShaderVersionType version) const
	{
		BOOST_FOREACH(const ShaderSourceData &data, get_datas())
		{
			if (data.get_version() <= version)
			{
				return data;
			}
		}

		EL_THROW_MESSAGE_EXCEPTION(UTF8("No shader source data with "
			"shader version %1% in '%2%' found."),
			version % get_typed_name(), ItemNotFoundException());
	}

	void ShaderSource::set_datas(const ShaderSourceDataVector &datas)
	{
		m_datas = datas;

		std::sort(m_datas.begin(), m_datas.end(),
			sort_shader_source_data);
	}

	String ShaderSource::get_xml_id()
	{
		return String(UTF8("shader_source"));
	}

	ShaderSourceType ShaderSource::get_type() const
	{
		return m_type;
	}

}
