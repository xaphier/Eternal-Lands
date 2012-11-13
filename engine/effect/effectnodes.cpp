/****************************************************************************
 *            effectnodes.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
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
#include "../shader/shadersourceparameterbuilder.hpp"
#include "../shader/shadersourceparameter.hpp"

namespace eternal_lands
{

	EffectNodes::EffectNodes(): m_uuid_generator(&m_ran)
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

	ShaderSourceParameterVector EffectNodes::get_parameters(
		const ShaderType shader_type,
		const ShaderVersionType version) const
	{
		ShaderSourceParameterVector vertex_parameters;
		ShaderSourceParameterVector fragment_parameters;
		StringStream vertex_str, fragment_str;
		Uint16StringMap array_layers;

		write(array_layers, version, eqt_high, vertex_parameters,
			fragment_parameters, vertex_str, fragment_str);

		if (shader_type == st_vertex)
		{
			return vertex_parameters;
		}

		if (shader_type == st_fragment)
		{
			return fragment_parameters;
		}

		return ShaderSourceParameterVector();
	}

	String EffectNodes::get_source(const ShaderType shader_type,
		const ShaderVersionType version) const
	{
		ShaderSourceParameterVector vertex_parameters;
		ShaderSourceParameterVector fragment_parameters;
		StringStream vertex_str, fragment_str;
		Uint16StringMap array_layers;

		write(array_layers, version, eqt_high, vertex_parameters,
			fragment_parameters, vertex_str, fragment_str);

		if (shader_type == st_vertex)
		{
			return String(vertex_str.str());
		}

		if (shader_type == st_fragment)
		{
			return String(fragment_str.str());
		}

		return String();
	}

	void EffectNodes::write(const Uint16StringMap &array_layers,
		const ShaderVersionType version,
		const EffectQualityType quality,
		ShaderSourceParameterVector &vertex_parameters,
		ShaderSourceParameterVector &fragment_parameters,
		OutStream &vertex_str, OutStream &fragment_str) const
	{
		UuidSet vertex_written, fragment_written;
		StringUint16Map parameters;
		StringUint16Map::const_iterator it, end;
		ParameterType type;
		Uint32 i, count;

		count = m_nodes.size();

		for (i = 0; i < count; ++i)
		{
			if (dynamic_cast<const EffectOutput*>(&m_nodes[i]) == 0)
			{
				continue;
			}

			m_nodes[i].write(array_layers, version, quality,
				ect_constant, parameters, vertex_parameters,
				fragment_parameters, vertex_str, fragment_str,
				vertex_written, fragment_written);

			break;
		}

		end = parameters.end();

		for (it = parameters.begin(); it != end; ++it)
		{
			assert(it->second > 0);
			assert(it->second < 5);
		
			type = pt_float;

			switch (it->second)
			{			
				case 1:
					type = pt_float;
					break;
				case 2:
					type = pt_vec2;
					break;
				case 3:
					type = pt_vec3;
					break;
				case 4:
					type = pt_vec4;
					break;
				default:
					continue;
			}

			vertex_parameters.push_back(
				ShaderSourceParameterBuilder::build(
					String(UTF8("effect nodes")),
					it->first, type, pqt_out, pst_one, 1));

			fragment_parameters.push_back(
				ShaderSourceParameterBuilder::build(
					String(UTF8("effect nodes")), it->first,
					type, pqt_in, pst_one, 1));
		}
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

	void EffectNodes::do_save_xml(const XmlWriterSharedPtr &xml_writer)
		const
	{
		Uint32 i, count;

		xml_writer->start_element(get_xml_id());

		xml_writer->write_element(String(UTF8("name")), get_name());

		xml_writer->write_int_element(String(UTF8("ids")), get_ids());


		xml_writer->start_element(String(UTF8("texture_units")));

		BOOST_FOREACH(const EffectTextureUnit &texture_units,
			m_texture_units)
		{
			texture_units.save_xml(xml_writer);
		}

		xml_writer->end_element();

		xml_writer->start_element(String(UTF8("nodes")));

		count = m_nodes.size();

		for (i = 0; i < count; ++i)
		{
			m_nodes[i].save_xml(xml_writer);
		}

		xml_writer->end_element();

		xml_writer->start_element(String(UTF8("connections")));

		count = m_nodes.size();

		for (i = 0; i < count; ++i)
		{
			m_nodes[i].save_connections_xml(xml_writer);
		}

		xml_writer->end_element();

		xml_writer->end_element();
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

	void EffectNodes::do_load_xml(const xmlNodePtr node)
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

	void EffectNodes::set_texture_unit(const String &name,
		const TextureTargetType target, const Uint16 index)
	{
		m_texture_units[index].set_name(name);
		m_texture_units[index].set_target(target);
		m_texture_units[index].set_default_texture(String());
	}

	void EffectNodes::set_texture_unit(const String &name,
		const String &default_texture, const TextureTargetType target,
		const Uint16 index)
	{
		m_texture_units[index].set_name(name);
		m_texture_units[index].set_target(target);
		m_texture_units[index].set_default_texture(default_texture);
	}

	void EffectNodes::set_texture_unit(const String &name,
		const StringVector &default_texture,
		const TextureTargetType target, const Uint16 index)
	{
		m_texture_units[index].set_name(name);
		m_texture_units[index].set_target(target);
		m_texture_units[index].set_default_texture(default_texture);
	}

	String EffectNodes::get_xml_id()
	{
		return String(UTF8("effect_nodes"));
	}

	ShaderSourceType EffectNodes::get_type() const
	{
		return sst_main_effect;
	}

	bool EffectNodes::get_has_data(const ShaderType shader_type,
		const ShaderVersionType version) const
	{
		return (shader_type == st_vertex) ||
			(shader_type == st_fragment);
	}

}
