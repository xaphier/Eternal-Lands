/****************************************************************************
 *            shadersourcedata.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "shadersourcedata.hpp"
#include "shadertextureutil.hpp"
#include "shadersourceparameterbuilder.hpp"
#include "shadersourceparameter.hpp"
#include "xmlwriter.hpp"
#include "xmlutil.hpp"

namespace eternal_lands
{

	ShaderSourceData::ShaderSourceData(): m_glsl_120(false),
		m_glsl_150(false), m_material_default(false),
		m_material_merged(false)
	{
	}

	ShaderSourceData::ShaderSourceData(const xmlNodePtr node):
		m_glsl_120(false), m_glsl_150(false), m_material_default(false),
		m_material_merged(false)
	{
		load_xml(node);
	}

	ShaderSourceData::~ShaderSourceData() throw()
	{
	}

	void ShaderSourceData::build_source(
		const ShaderSourceParameterVector &locals, 
		OutStream &stream, ShaderSourceParameterVector &globals) const
	{
		stream << get_source() << UTF8("\n");

		BOOST_FOREACH(const ShaderSourceParameter &parameter,
			get_parameters())
		{
			ShaderSourceParameterBuilder::add_parameter(parameter,
				locals, globals);
		}
	}

	bool ShaderSourceData::check_source_parameter(const String &name) const
	{
		BOOST_FOREACH(const ShaderSourceParameter &parameter,
			get_parameters())
		{
			if ((parameter.get_name() == name) &&
				(parameter.get_qualifier() != pqt_out))
			{
				return true;
			}
		}

		return false;
	}

	void ShaderSourceData::load_parameters_xml(const xmlNodePtr node)
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
					it));
			}
		}
		while (XmlUtil::next(it, true));
	}

	void ShaderSourceData::load_xml(const xmlNodePtr node)
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
				load_parameters_xml(it);
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("source")) == 0)
			{
				set_source(XmlUtil::get_string_value(it));
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("glsl_120")) == 0)
			{
				set_glsl_120(XmlUtil::get_bool_value(it));
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("glsl_150")) == 0)
			{
				set_glsl_150(XmlUtil::get_bool_value(it));
			}

			if (xmlStrcmp(it->name,
				BAD_CAST UTF8("material_default")) == 0)
			{
				set_material_default(
					XmlUtil::get_bool_value(it));
			}

			if (xmlStrcmp(it->name,
				BAD_CAST UTF8("material_merged")) == 0)
			{
				set_material_merged(
					XmlUtil::get_bool_value(it));
			}
		}
		while (XmlUtil::next(it, true));
	}

	void ShaderSourceData::save_xml(const XmlWriterSharedPtr &writer) const
	{
		StringVariantMap::const_iterator it, end;

		writer->start_element(UTF8("shader_source_data"));

		writer->start_element(UTF8("parameters"));

		BOOST_FOREACH(const ShaderSourceParameter &parameter,
			get_parameters())
		{
			parameter.save_xml(writer);
		}

		writer->end_element();

		writer->write_element(UTF8("source"), get_source());
		writer->write_bool_element(UTF8("glsl_120"), get_glsl_120());
		writer->write_bool_element(UTF8("glsl_150"), get_glsl_150());
		writer->write_bool_element(UTF8("material_default"),
			get_material_default());
		writer->write_bool_element(UTF8("material_merged"),
			get_material_merged());

		writer->end_element();
	}

	void ShaderSourceData::set_parameters(
		const ShaderSourceParameterVector &parameters)
	{
		m_parameters = parameters;
	}

}
