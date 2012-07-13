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
		m_change(ect_undefined), m_value_count(0)
	{
	}

	EffectNode::~EffectNode() noexcept
	{
	}

	void EffectNode::add_input_port(const String &description,
		const String &swizzle, const EffectChangeType change)
	{
		m_ports.push_back(new EffectNodePort(this, String(),
			description, swizzle, change, true));
	}

	void EffectNode::add_input_port(const String &swizzle,
		const EffectChangeType change)
	{
		m_ports.push_back(new EffectNodePort(this, String(),
			swizzle, change, true));
	}

	void EffectNode::add_output_port(const String &var,
		const String &description, const String &swizzle,
		const EffectChangeType change)
	{
		m_ports.push_back(new EffectNodePort(this, var,
			description, swizzle, change, false));
	}

	void EffectNode::add_output_port(const String &var,
		const String &swizzle, const EffectChangeType change)
	{
		m_ports.push_back(new EffectNodePort(this, var,
			swizzle, change, false));
	}

	bool EffectNode::check_connections(EffectNodePtrVector &checking)
	{
		BOOST_FOREACH(EffectNodePtr node, checking)
		{
			if (node == this)
			{
				checking.push_back(this);

				return false;
			}
		}

		checking.push_back(this);

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

		BOOST_FOREACH(EffectNodePort &port, m_ports)
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

		BOOST_FOREACH(EffectNodePort &port, m_ports)
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
		assert(get_value_count() > 0);
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

}
