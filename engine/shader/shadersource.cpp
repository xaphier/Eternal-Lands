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
			String(L"world_depth_transform"),
			String(L"world_normal_transform"),
			String(L"world_tangent_transform"),
			String(L"view_direction"),
			String(L"view_transform"),
			String(L"fog"),
			String(L"light"),
			String(L"uv"),
			String(L"shadow_uv"),
			String(L"shadow_mapping"),
			String(L"normal_mapping"),
			String(L"normal_depth_mapping"),
			String(L"diffuse_mapping"),
			String(L"specular_mapping"),
			String(L"transparent"),
			String(L"depth_color")
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

		m_datas.clear();

		if (!XmlUtil::has_children(node, true))
		{
			return;
		}

		it = XmlUtil::children(node, true);

		do
		{
			if (xmlStrcmp(it->name, BAD_CAST "shader_source_data")
				== 0)
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
				<< errinfo_message(L"parameter is zero")
				<< errinfo_parameter_name(L"node"));
		}

		if (xmlStrcmp(node->name, BAD_CAST "shader_source") != 0)
		{
			return;
		}

		it = XmlUtil::children(node, true);

		do
		{
			if (xmlStrcmp(it->name, BAD_CAST "shader_source_datas")
				== 0)
			{
				load_datas_xml(it);
			}

			if (xmlStrcmp(it->name, BAD_CAST "name") == 0)
			{
				set_name(XmlUtil::get_string_value(it));
			}

			if (xmlStrcmp(it->name, BAD_CAST "type") == 0)
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
		writer->start_element("shader_source");

		writer->write_element("name", get_name());
		writer->write_element("type", ShaderSourceUtil::get_str(
			get_type()));

		writer->start_element("shader_source_datas");

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

	void ShaderSource::write_function_use(const ShaderSourceDataType &type,
		OutStream &str) const
	{
		get_data(type).write_function_use(get_typed_name(), str);
	}

	void ShaderSource::write_function(const ShaderSourceDataType &type,
		const ParameterSizeTypeUint16Map &array_sizes, OutStream &str)
		const
	{
		get_data(type).write_function(get_typed_name(), array_sizes,
			str);
	}

	void ShaderSource::build_function(const ShaderSourceDataType &type,
		const ParameterSizeTypeUint16Map &array_sizes,
		const ShaderSourceParameterVector &locals,
		OutStream &functions, OutStream &local,
		ShaderSourceParameterVector &globals) const
	{
		get_data(type).build_function(get_typed_name(), array_sizes,
			locals, functions, local, globals);
	}

	bool ShaderSource::check_source_parameter(
		const ShaderSourceDataType &type, const String &name) const
	{
		return get_data(type).check_source_parameter(name);
	}

	bool ShaderSource::get_has_data(const ShaderSourceDataType type) const
	{
		BOOST_FOREACH(const ShaderSourceData &data, get_datas())
		{
			switch (type)
			{
				case ssdt_glsl_120:
					if (data.get_glsl_120())
					{
						return true;
					}
					break;
				case ssdt_glsl_150:
					if (data.get_glsl_150() &&
						data.get_material_default())
					{
						return true;
					}
					break;
				case ssdt_glsl_150_texture_arrays:
					if (data.get_material_texture_arrays()
						&& data.get_glsl_150())
					{
						return true;
					}
					break;
			}
		}

		return false;
	}

	const ShaderSourceData &ShaderSource::get_data(
		const ShaderSourceDataType type) const
	{
		BOOST_FOREACH(const ShaderSourceData &data, get_datas())
		{
			switch (type)
			{
				case ssdt_glsl_120:
					if (data.get_glsl_120())
					{
						return data;
					}
					break;
				case ssdt_glsl_150:
					if (data.get_glsl_150() &&
						data.get_material_default())
					{
						return data;
					}
					break;
				case ssdt_glsl_150_texture_arrays:
					if (data.get_material_texture_arrays()
						&& data.get_glsl_150())
					{
						return data;
					}
					break;
			}
		}

		EL_THROW_MESSAGE_EXCEPTION(L"No shader source data with type "
			"%1% in %2% found.", type % get_name(),
			ItemNotFoundException());
	}

	void ShaderSource::set_datas(const ShaderSourceDataVector &datas)
	{
		m_datas = datas;
	}

	String ShaderSource::get_typed_name() const
	{
		return String(ShaderSourceUtil::get_str(get_type()).get() +
			L"_" + get_name().get());
	}

}
