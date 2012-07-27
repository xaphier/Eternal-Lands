/****************************************************************************
 *            effectnodes.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "effectnodes.hpp"
#include "effectnode.hpp"
#include "effectnodeport.hpp"
#include "effectchangeutil.hpp"
#include "effectconstant.hpp"
#include "effectfunction.hpp"
#include "effectparameter.hpp"
#include "effecttexture.hpp"
#include "effectoutput.hpp"
#include "xmlreader.hpp"
#include "xmlutil.hpp"
#include "xmlwriter.hpp"
#include "exceptions.hpp"

namespace eternal_lands
{

	EffectNodes::EffectNodes(): m_uuid_generator(&m_ran)
	{
		m_ran.seed(time(0));
	}

	EffectNodes::EffectNodes(const String &name): m_uuid_generator(&m_ran),
		m_name(name), m_ids(0)
	{
		m_ran.seed(time(0));
	}

	EffectNodes::~EffectNodes() noexcept
	{
	}

	EffectNodePtr EffectNodes::add_color(const String &name)
	{
		m_nodes.push_back(new EffectConstant(name, m_ids++,
			ect_color_rgb, m_uuid_generator));

		return &(*m_nodes.rbegin());
	}

	EffectNodePtr EffectNodes::add_direction(const String &name)
	{
		m_nodes.push_back(new EffectConstant(name, m_ids++,
			ect_direction_xy, m_uuid_generator));

		return &(*m_nodes.rbegin());
	}

	EffectNodePtr EffectNodes::add_constant(const String &name,
		const Uint16 count)
	{
		switch (count)
		{
			case 1:
				m_nodes.push_back(new EffectConstant(name,
					m_ids++, ect_float, m_uuid_generator));
				break;
			case 2:
				m_nodes.push_back(new EffectConstant(name,
					m_ids++, ect_vec2, m_uuid_generator));
				break;
			case 3:
				m_nodes.push_back(new EffectConstant(name,
					m_ids++, ect_vec3, m_uuid_generator));
				break;
			case 4:
			default:
				m_nodes.push_back(new EffectConstant(name,
					m_ids++, ect_vec4, m_uuid_generator));
				break;
		}

		return &(*m_nodes.rbegin());
	}

	EffectNodePtr EffectNodes::add_function(const String &name,
		const EffectFunctionType function)
	{
		m_nodes.push_back(new EffectFunction(name, m_ids++,
			function, m_uuid_generator));

		return &(*m_nodes.rbegin());
	}

	EffectNodePtr EffectNodes::add_parameter(const String &name,
		const EffectParameterType parameter)
	{
		m_nodes.push_back(new EffectParameter(name, m_ids++,
			parameter, m_uuid_generator));

		return &(*m_nodes.rbegin());
	}

	EffectNodePtr EffectNodes::add_texture(const String &name,
		const EffectSamplerType sampler,
		const EffectTextureType texture, const Uint16 texture_unit)
	{
		m_nodes.push_back(new EffectTexture(name, m_ids++,
			sampler, texture, texture_unit, m_uuid_generator));

		return &(*m_nodes.rbegin());
	}

	EffectNodePtr EffectNodes::add_output(const String &name)
	{
		m_nodes.push_back(new EffectOutput(name, m_ids++,
			m_uuid_generator));

		return &(*m_nodes.rbegin());
	}

	void EffectNodes::write(const Uint16StringMap &array_layers,
		const ShaderVersionType version, const bool low_quality,
		ShaderSourceParameterVector &vertex_parameters,
		ShaderSourceParameterVector &fragment_parameters,
		OutStream &vertex_str, OutStream &fragment_str)
	{
		EffectNodePtrSet vertex_written, fragment_written;
		StringUint16Map parameters;

		m_nodes.begin()->write(array_layers, version, low_quality,
			ect_constant, parameters, vertex_parameters,
			fragment_parameters, vertex_str, fragment_str,
			vertex_written, fragment_written);
	}

	void EffectNodes::remove(const EffectNodePtr effect_node)
	{
		EffectNodeVector::iterator it, end;

		end = m_nodes.end();

		for (it = m_nodes.begin(); it != end; ++it)
		{
			if (&(*it) == effect_node)
			{
				m_nodes.erase(it);

				return;
			}
		}
	}

	void EffectNodes::get_ports(UuidEffectNodePortPtrMap &ports)
	{
		EffectNodeVector::iterator it, end;

		end = m_nodes.end();

		for (it = m_nodes.begin(); it != end; ++it)
		{
			it->get_ports(ports);
		}
	}

	void EffectNodes::save_xml(const XmlWriterSharedPtr &writer)
	{
		writer->start_element(UTF8("effect_nodes"));

		writer->write_element(UTF8("name"), get_name());

		writer->write_int_element(UTF8("ids"), get_ids());

		writer->start_element(UTF8("nodes"));

		BOOST_FOREACH(EffectNode &node, m_nodes)
		{
			node.save_xml(writer);
		}

		writer->end_element();

		writer->start_element(UTF8("connections"));

		BOOST_FOREACH(EffectNode &node, m_nodes)
		{
			node.save_connections_xml(writer);
		}

		writer->end_element();

		writer->end_element();
	}

	void EffectNodes::save_xml(const String &file_name)
	{
		XmlWriterSharedPtr writer;

		writer = XmlWriterSharedPtr(new XmlWriter(file_name));

		save_xml(writer);
	}

	void EffectNodes::load_nodes_xml(const xmlNodePtr node)
	{
		xmlNodePtr it;

		if (node == 0)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_message(UTF8("parameter is zero"))
				<< errinfo_parameter_name(UTF8("node")));
		}

		if (xmlStrcmp(node->name, BAD_CAST UTF8("nodes")) != 0)
		{
			return;
		}

		it = XmlUtil::children(node, true);

		do
		{
			if (xmlStrcmp(it->name,
				BAD_CAST UTF8("effect_constant")) == 0)
			{
				m_nodes.push_back(new EffectConstant());
				m_nodes.rbegin()->load_xml(it);
			}

			if (xmlStrcmp(it->name,
				BAD_CAST UTF8("effect_function")) == 0)
			{
				m_nodes.push_back(new EffectFunction());
				m_nodes.rbegin()->load_xml(it);
			}

			if (xmlStrcmp(it->name,
				BAD_CAST UTF8("effect_output")) == 0)
			{
				m_nodes.push_back(new EffectOutput());
				m_nodes.rbegin()->load_xml(it);
			}

			if (xmlStrcmp(it->name,
				BAD_CAST UTF8("effect_parameter")) == 0)
			{
				m_nodes.push_back(new EffectParameter());
				m_nodes.rbegin()->load_xml(it);
			}

			if (xmlStrcmp(it->name,
				BAD_CAST UTF8("effect_texture")) == 0)
			{
				m_nodes.push_back(new EffectTexture());
				m_nodes.rbegin()->load_xml(it);
			}
		}
		while (XmlUtil::next(it, true));
	}

	void EffectNodes::load_connection_xml(const xmlNodePtr node,
		UuidEffectNodePortPtrMap &ports)
	{
		xmlNodePtr it;
		UuidEffectNodePortPtrMap::iterator found;
		EffectNodePortPtr port;
		boost::array<boost::uuids::uuid, 2> uuids;
		Uint16 index;

		if (node == 0)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_message(UTF8("parameter is zero"))
				<< errinfo_parameter_name(UTF8("node")));
		}

		if (xmlStrcmp(node->name, BAD_CAST UTF8("connection")) != 0)
		{
			return;
		}

		it = XmlUtil::children(node, true);

		index = 0;

		do
		{
			if (xmlStrcmp(it->name,	BAD_CAST UTF8("uuid"))
				== 0)
			{
				if (index < uuids.size())
				{
					uuids[index] = XmlUtil::get_uuid_value(
						it);
				}

				index++;
			}
		}
		while (XmlUtil::next(it, true));

		if (index != uuids.size())
		{
			return;
		}

		found = ports.find(uuids[0]);

		if (found == ports.end())
		{
			return;
		}

		port = found->second;

		found = ports.find(uuids[1]);

		if (found == ports.end())
		{
			return;
		}

		port->connect(found->second);
	}

	void EffectNodes::load_connections_xml(const xmlNodePtr node)
	{
		xmlNodePtr it;
		UuidEffectNodePortPtrMap ports;

		if (node == 0)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_message(UTF8("parameter is zero"))
				<< errinfo_parameter_name(UTF8("node")));
		}

		if (xmlStrcmp(node->name, BAD_CAST UTF8("connections")) != 0)
		{
			return;
		}

		get_ports(ports);

		it = XmlUtil::children(node, true);

		do
		{
			if (xmlStrcmp(it->name,	BAD_CAST UTF8("connection"))
				== 0)
			{
				load_connection_xml(it, ports);
			}
		}
		while (XmlUtil::next(it, true));
	}

	void EffectNodes::load_xml(const xmlNodePtr node)
	{
		xmlNodePtr it;
		String source;

		if (node == 0)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_message(UTF8("parameter is zero"))
				<< errinfo_parameter_name(UTF8("node")));
		}

		if (xmlStrcmp(node->name, BAD_CAST UTF8("effect_nodes")) != 0)
		{
			return;
		}

		it = XmlUtil::children(node, true);

		do
		{
			if (xmlStrcmp(it->name, BAD_CAST UTF8("name")) == 0)
			{
				m_name = XmlUtil::get_string_value(it);
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("ids")) == 0)
			{
				m_ids = XmlUtil::get_uint32_value(it);
			}

			if (xmlStrcmp(it->name,	BAD_CAST UTF8("nodes")) == 0)
			{
				load_nodes_xml(it);
			}
		}
		while (XmlUtil::next(it, true));

		it = XmlUtil::children(node, true);

		do
		{
			if (xmlStrcmp(it->name,	BAD_CAST UTF8("connections"))
				== 0)
			{
				load_connections_xml(it);
			}
		}
		while (XmlUtil::next(it, true));
	}

	void EffectNodes::load_xml(const FileSystemSharedPtr &file_system,
		const String &file_name)
	{
		XmlReaderSharedPtr xml_reader;

		xml_reader = XmlReaderSharedPtr(new XmlReader(file_system,
			file_name));

		load_xml(xml_reader->get_root_node());
	}

	void EffectNodes::load_xml(const String &file_name)
	{
		XmlReaderSharedPtr reader;

		reader = XmlReaderSharedPtr(new XmlReader(file_name));

		load_xml(reader->get_root_node());
	}

}
