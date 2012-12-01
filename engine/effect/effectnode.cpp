/****************************************************************************
 *            effectnode.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "effectnode.hpp"
#include "effectnodeport.hpp"
#include "xmlutil.hpp"
#include "xmlwriter.hpp"

namespace eternal_lands
{

	EffectNode::EffectNode(): m_id(0), m_change(ect_undefined),
		m_value_count(0)
	{
	}

	EffectNode::EffectNode(const String &name, const Uint32 id,
		const boost::uuids::uuid &uuid): m_uuid(uuid), m_name(name),
		m_id(id), m_change(ect_undefined), m_value_count(0)
	{
	}

	EffectNode::~EffectNode() noexcept
	{
	}

	void EffectNode::save_xml(const XmlWriterSharedPtr &writer) const
	{
		Uint32 i, count;

		writer->write_uuid_element(String(UTF8("uuid")), get_uuid());
		writer->write_element(String(UTF8("name")), get_name());
		writer->write_int_element(String(UTF8("id")), get_id());
		writer->write_ivec2_element(String(UTF8("position")),
			get_position());

		writer->start_element(String(UTF8("ports")));

		count = get_ports().size();

		for (i = 0; i < count; ++i)
		{
			get_ports()[i].save_xml(writer);
		}

		writer->end_element();
	}

	void EffectNode::save_connections_xml(const XmlWriterSharedPtr &writer)
		const
	{
		Uint32 i, count;

		count = get_ports().size();

		for (i = 0; i < count; ++i)
		{
			get_ports()[i].save_connections_xml(writer);
		}
	}

	void EffectNode::load_ports_xml(const xmlNodePtr node)
	{
		xmlNodePtr it;

		if (xmlStrcmp(node->name, BAD_CAST UTF8("ports"))
			!= 0)
		{
			return;
		}

		clear_ports();

		it = XmlUtil::children(node, true);

		do
		{
			if (xmlStrcmp(it->name,
				BAD_CAST UTF8("effect_node_port")) == 0)
			{
				m_ports.push_back(new EffectNodePort(this, it));
			}
		}
		while (XmlUtil::next(it, true));
	}

	void EffectNode::load_xml(const xmlNodePtr node)
	{
		xmlNodePtr it;

		it = XmlUtil::children(node, true);

		do
		{
			if (xmlStrcmp(it->name, BAD_CAST UTF8("uuid")) == 0)
			{
				m_uuid = XmlUtil::get_uuid_value(it);
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("name")) == 0)
			{
				m_name = XmlUtil::get_string_value(it);
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("id")) == 0)
			{
				m_id = XmlUtil::get_u32_value(it);
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("position")) == 0)
			{
				m_position = XmlUtil::get_ivec2_value(it);
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("ports")) == 0)
			{
				load_ports_xml(it);
			}
		}
		while (XmlUtil::next(it, true));
	}

	void EffectNode::add_input_port(const String &description,
		const String &swizzle, const boost::uuids::uuid &uuid,
		const EffectChangeType change)
	{
		m_ports.push_back(new EffectNodePort(this, String(),
			description, swizzle, uuid, change, true));
	}

	void EffectNode::add_input_port(const String &swizzle,
		const boost::uuids::uuid &uuid, const EffectChangeType change)
	{
		m_ports.push_back(new EffectNodePort(this, String(), swizzle,
			uuid, change, true));
	}

	void EffectNode::add_output_port(const String &var,
		const String &description, const String &swizzle,
		const boost::uuids::uuid &uuid, const EffectChangeType change)
	{
		m_ports.push_back(new EffectNodePort(this, var, description,
			swizzle, uuid, change, false));
	}

	void EffectNode::add_output_port(const String &var,
		const String &swizzle, const boost::uuids::uuid &uuid,
		const EffectChangeType change)
	{
		m_ports.push_back(new EffectNodePort(this, var, swizzle, uuid,
			change, false));
	}

	bool EffectNode::check_connections(EffectNodePtrVector &checking)
	{
		BOOST_FOREACH(const EffectNodePtr node, checking)
		{
			if (node == this)
			{
				checking.push_back(this);

				return false;
			}
		}

		checking.push_back(this);

		BOOST_FOREACH(const EffectNodePort &port, m_ports)
		{
			if (port.get_output())
			{
				if (!port.check_connection(checking))
				{
					return false;
				}
			}
		}

		checking.pop_back();

		return true;
	}

	void EffectNode::update(EffectNodePtrSet &updated)
	{
		Uint16 connected_value_count;

		if (updated.count(this) > 0)
		{
			return;
		}

		updated.insert(this);

		m_change = ect_undefined;
		m_value_count = get_initial_value_count();
		connected_value_count = get_initial_value_count();

		BOOST_FOREACH(EffectNodePort &port, get_ports())
		{
			if (port.get_output())
			{
				continue;
			}

			port.update(updated);

			if (port.get_general_type())
			{
				connected_value_count =
					port.get_connected_value_count();

				if (((connected_value_count > 1) &&
					port.get_input()) ||
					((connected_value_count > 0) &&
						port.get_output()))
				{
					m_value_count = connected_value_count;
				}
			}

			if (port.get_undefined_change())
			{
				m_change = std::max(m_change,
					port.get_connected_change());
			}
			else
			{
				m_change = std::max(m_change,
					port.get_change());
			}
		}

		BOOST_FOREACH(EffectNodePort &port, get_ports())
		{
			if (port.get_input())
			{
				continue;
			}

			port.update(updated);

			if (port.get_general_type())
			{
				connected_value_count =
					port.get_connected_value_count();

				if (((connected_value_count > 1) &&
					port.get_input()) ||
					((connected_value_count > 0) &&
						port.get_output()))
				{
					m_value_count = connected_value_count;
				}
			}
		}
	}

	String EffectNode::get_value_count_type_str() const
	{
//		assert(get_value_count() > 0);
		assert(get_value_count() < 5);

		switch (get_value_count())
		{
			case 1:
				return String(UTF8("float"));
			case 2:
				return String(UTF8("vec2"));
			case 3:
				return String(UTF8("vec3"));
			case 4:
				return String(UTF8("vec4"));
			default:
				return String();
		}
	}

	void EffectNode::update()
	{
		EffectNodePtrSet updated;

		update(updated);
	}

	String EffectNode::get_var_name(const Uint32 id, const Uint16 index)
	{
		StringStream str;

		str << UTF8("effect_var_") << id << UTF8("_") << index;

		return String(str.str());
	}

	void EffectNode::get_ports(UuidEffectNodePortPtrMap &ports)
	{
		EffectNodePortVector::iterator it, end;

		end = m_ports.end();

		for (it = m_ports.begin(); it != end; ++it)
		{
			ports[it->get_uuid()] = &(*it);
		}
	}

}
