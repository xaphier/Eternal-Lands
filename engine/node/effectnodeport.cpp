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

	EffectNodePort::EffectNodePort(const EffectNode &node,
		const String &description, const EffectNodePortType type,
		const bool input): m_node(node), m_description(description),
		m_type(type), m_input(input)
	{
	}

	EffectNodePort::EffectNodePort(const EffectNode &node,
		const EffectNodePortType type, const bool input): m_node(node),
		m_description(EffectNodePortUtil::get_description(type)),
		m_type(type), m_input(input)
	{
	}

	EffectNodePort::~EffectNodePort() noexcept
	{
		BOOST_FOREACH(const EffectNodePortPtr port, m_connections)
		{
			disconnect(port);
		}
	}

	void EffectNodePort::do_connect(const EffectNodePortPtr port)
	{
		NULL_PTR_CHECK(port, UTF8("port"));

		if (get_input_port())
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
		Uint16 count;

		count = EffectNodePortUtil::get_count(get_type());;

		if (count == 0)
		{
			return get_node_value_count();
		}

		return count;
	}

	Uint16 EffectNodePort::get_node_value_count() const noexcept
	{
		return m_node.get_value_count();
	}

	bool EffectNodePort::get_convertable(const EffectNodePortPtr port)
		const
	{
		if (port == nullptr)
		{
			return false;
		}

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
