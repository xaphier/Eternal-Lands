/****************************************************************************
 *            shadersource.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "shadersource.hpp"
#include "exceptions.hpp"
#include "lua.hpp"
#include "logging.hpp"
#include "shadertextureutil.hpp"
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

		const String shader_source_type_datas[] =
		{
			String(UTF8("world_depth_transform")),
			String(UTF8("world_normal_transform")),
			String(UTF8("world_tangent_transform")),
			String(UTF8("view_direction")),
			String(UTF8("view_transform")),
			String(UTF8("fog")),
			String(UTF8("light")),
			String(UTF8("uv")),
			String(UTF8("shadow_uv")),
			String(UTF8("shadow_mapping")),
			String(UTF8("normal_mapping")),
			String(UTF8("normal_depth_mapping")),
			String(UTF8("diffuse_mapping")),
			String(UTF8("specular_mapping")),
			String(UTF8("transparent")),
			String(UTF8("depth_color"))
		};

		const Uint32 shader_source_type_datas_count =
			sizeof(shader_source_type_datas) / sizeof(String);

	}

	ShaderSource::ShaderSource(): m_type(sst_world_depth_transform)
	{
	}

	ShaderSource::~ShaderSource() throw()
	{
	}

	void ShaderSource::load_datas_xml(const xmlNodePtr node)
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
				m_datas.push_back(ShaderSourceData(it));
			}
		}
		while (XmlUtil::next(it, true));
	}

	void ShaderSource::load_xml(const xmlNodePtr node)
	{
		xmlNodePtr it;

		if (node == 0)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_message(UTF8("parameter is zero"))
				<< errinfo_parameter_name(UTF8("node")));
		}

		if (xmlStrcmp(node->name, BAD_CAST UTF8("shader_source")) != 0)
		{
			return;
		}

		it = XmlUtil::children(node, true);

		do
		{
			if (xmlStrcmp(it->name,
				BAD_CAST UTF8("shader_source_datas")) == 0)
			{
				load_datas_xml(it);
			}

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
	}

	void ShaderSource::load_xml(const String &file_name)
	{
		XmlReaderSharedPtr reader;

		reader = XmlReaderSharedPtr(new XmlReader(file_name));

		load_xml(reader->get_root_node());
	}

	void ShaderSource::save_xml(const XmlWriterSharedPtr &writer) const
	{
		writer->start_element(UTF8("shader_source"));

		writer->write_element(UTF8("name"), get_name());
		writer->write_element(UTF8("type"), ShaderSourceUtil::get_str(
			get_type()));

		writer->start_element(UTF8("shader_source_datas"));

		BOOST_FOREACH(const ShaderSourceData &data, get_datas())
		{
			data.save_xml(writer);
		}

		writer->end_element();

		writer->end_element();
	}

	void ShaderSource::save_xml(const String &file_name) const
	{
		XmlWriterSharedPtr writer;

		writer = XmlWriterSharedPtr(new XmlWriter(file_name));

		save_xml(writer);
	}

	void ShaderSource::build_source(const ShaderVersionType &type,
		const ShaderSourceParameterVector &locals,
		OutStream &stream, ShaderSourceParameterVector &globals) const
	{
		get_data(type).build_source(locals, stream, globals);
	}

	bool ShaderSource::check_source_parameter(
		const ShaderVersionType &type, const String &name) const
	{
		return get_data(type).check_source_parameter(name);
	}

	bool ShaderSource::get_has_data(const ShaderVersionType shader_version)
		const
	{
		BOOST_FOREACH(const ShaderSourceData &data, get_datas())
		{
			if (data.get_version(shader_version))
			{
				return true;
			}
		}

		return false;
	}

	const ShaderSourceData &ShaderSource::get_data(
		const ShaderVersionType shader_version) const
	{
		BOOST_FOREACH(const ShaderSourceData &data, get_datas())
		{
			if (data.get_version(shader_version))
			{
				return data;
			}
		}

		EL_THROW_MESSAGE_EXCEPTION(UTF8("No shader source data with "
			"shader version %1% in '[%2%]: %3%' found."),
			shader_version % get_type() % get_name(),
			ItemNotFoundException());
	}

	void ShaderSource::set_datas(const ShaderSourceDataVector &datas)
	{
		m_datas = datas;
	}

	String ShaderSource::get_typed_name() const
	{
		return String(ShaderSourceUtil::get_str(get_type()).get() +
			UTF8("_") + get_name().get());
	}

}