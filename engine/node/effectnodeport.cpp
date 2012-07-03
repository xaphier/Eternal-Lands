/****************************************************************************
 *            abstractnode.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "effectnodeport.hpp"
#include "effectnode.hpp"
#include "exceptions.hpp"

namespace eternal_lands
{

	EffectNodePort::EffectNodePort(const EffectNodePtr node,
		const String &var_name, const String &description,
		const EffectNodePortType type, const EffectChangeType change,
		const bool input): m_node(node), m_var_name(var_name),
		m_description(description), m_type(type), m_change(change),
		m_input(input)
	{
		assert(m_input || !get_var_name().get().empty());
	}

	EffectNodePort::EffectNodePort(const EffectNodePtr node,
		const String &var_name, const EffectNodePortType type,
		const EffectChangeType change, const bool input): m_node(node),
		m_var_name(var_name),
		m_description(EffectNodePortUtil::get_description(type)),
		m_type(type), m_change(change), m_input(input)
	{
		assert(m_input || !get_var_name().get().empty());
	}

	EffectNodePort::~EffectNodePort() noexcept
	{
		BOOST_FOREACH(const EffectNodePortPtr port, m_connections)
		{
			disconnect(port);
		}
	}

	bool EffectNodePort::check_connection(const EffectNodePortPtr port,
		EffectNodePtrSet &checking) const
	{
		checking.insert(port->get_node_ptr());

		return m_node->check_connections(checking);
	}

	bool EffectNodePort::check_connection(EffectNodePtrSet &checking) const
	{
		BOOST_FOREACH(EffectNodePortPtr connected_port, m_connections)
		{
			if (!connected_port->m_node->check_connections(
				checking))
			{
				return false;
			}
		}

		return true;
	}

	void EffectNodePort::do_connect(const EffectNodePortPtr port)
	{
		NULL_PTR_CHECK(port, UTF8("port"));

		if (get_input())
		{
			m_connections.clear();
		}

		m_connections.insert(port);
	}

	bool EffectNodePort::connect(const EffectNodePortPtr port)
	{
		NULL_PTR_CHECK(port, UTF8("port"));

		if (!get_convertable(port))
		{
			return false;
		}

		port->do_connect(this);
		do_connect(port);

		return true;
	}

	void EffectNodePort::do_disconnect(const EffectNodePortPtr port)
	{
		NULL_PTR_CHECK(port, UTF8("port"));

		m_connections.erase(port);
	}

	void EffectNodePort::disconnect(const EffectNodePortPtr port)
	{
		NULL_PTR_CHECK(port, UTF8("port"));

		port->do_disconnect(this);
		do_disconnect(port);
	}

	Uint16 EffectNodePort::get_value_count() const noexcept
	{
		if (get_general_type())
		{
			return get_node_value_count();
		}

		return EffectNodePortUtil::get_count(get_type());
	}

	String EffectNodePort::get_connected_var_name() const
	{
		StringStream str;

		if (get_output())
		{
			return get_var_name();
		}

		if (!get_connected())
		{
			return String();
		}

		if (get_value_count() == get_connection()->get_value_count())
		{
			return get_connection()->get_var_name();
		}

		str << UTF8("vec") << get_value_count() << UTF8("(");
		str << get_connection()->get_var_name() << UTF8(")");

		return String(str.str());
	}

	Uint16 EffectNodePort::get_node_value_count() const noexcept
	{
		return get_node().get_value_count();
	}

	Uint16 EffectNodePort::get_connected_value_count() const
	{
		if (!get_general_type())
		{
			return EffectNodePortUtil::get_count(get_type());
		}

		BOOST_FOREACH(const EffectNodePortPtr port, m_connections)
		{
			return port->get_value_count();
		}

		return 0;
	}

	void EffectNodePort::update(EffectNodePtrSet &updated)
	{
		BOOST_FOREACH(EffectNodePortPtr port, m_connections)
		{
			port->m_node->update(updated);
		}
	}

	bool EffectNodePort::get_convertable(const EffectNodePortPtr port)
		const
	{
		NULL_PTR_CHECK(port, UTF8("port"));

		return get_convertable(get_type(), port->get_type(),
			get_value_count(), port->get_value_count());
	}

	bool EffectNodePort::get_convertable(
		const EffectNodePortType effect_node_port_0,
		const EffectNodePortType effect_node_port_1,
		const Uint16 count_0, const Uint16 count_1)
	{
		Uint16 tmp_count_0, tmp_count_1;

		tmp_count_0 = EffectNodePortUtil::get_count(
			effect_node_port_0);
		tmp_count_1 = EffectNodePortUtil::get_count(
			effect_node_port_1);

		if (tmp_count_0 == 0)
		{
			tmp_count_0 = count_0;
		}

		if (tmp_count_1 == 0)
		{
			tmp_count_1 = count_1;
		}

		return (tmp_count_0 == tmp_count_1) ||
			EffectNodePortUtil::get_convertable(
				effect_node_port_0) ||
			EffectNodePortUtil::get_convertable(
				effect_node_port_1);
	}

}
