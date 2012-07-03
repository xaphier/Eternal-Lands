/****************************************************************************
 *            effectnode.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "effectnode.hpp"
#include "effectnodeport.hpp"

namespace eternal_lands
{

	EffectNode::EffectNode(const String &name): m_name(name),
		m_value_count(0)
	{
	}

	EffectNode::~EffectNode() noexcept
	{
	}

	void EffectNode::add_input_port(const String &description,
		const EffectNodePortType type)
	{
		m_ports.push_back(new EffectNodePort(this, String(),
			description, type, ect_variable, true));
	}

	void EffectNode::add_input_port(const EffectNodePortType type)
	{
		m_ports.push_back(new EffectNodePort(this, String(),
			type, ect_variable, true));
	}

	void EffectNode::add_output_port(const String &var_name,
		const String &description, const EffectNodePortType type,
		const EffectChangeType change)
	{
		m_ports.push_back(new EffectNodePort(this, var_name,
			description, type, change, false));
	}

	void EffectNode::add_output_port(const String &var_name,
		const EffectNodePortType type, const EffectChangeType change)
	{
		m_ports.push_back(new EffectNodePort(this, var_name,
			type, change, false));
	}

	bool EffectNode::check_connections(EffectNodePtrSet &checking)
	{
		if (checking.count(this) > 0)
		{
			return false;
		}

		checking.insert(this);

		BOOST_FOREACH(EffectNodePort &port, m_ports)
		{
			if (port.get_output())
			{
				if (!port.check_connection(checking))
				{
					return false;
				}
			}
		}

		return true;
	}

	void EffectNode::update(EffectNodePtrSet &updated)
	{
		Uint16 value_count, connected_value_count;

		value_count = 0;

		if (updated.count(this) > 0)
		{
			return;
		}

		BOOST_FOREACH(EffectNodePort &port, m_ports)
		{
			port.update(updated);

			if (port.get_general_type())
			{
				connected_value_count =
					port.get_connected_value_count();

				if (value_count != connected_value_count)
				{
					assert(value_count == 0);
					value_count = connected_value_count;
				}
			}
		}

		updated.insert(this);

		set_value_count(value_count);
	}

}
