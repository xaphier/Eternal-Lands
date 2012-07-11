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
		const String &swizzle)
	{
		m_ports.push_back(new EffectNodePort(this, String(),
			description, swizzle, ect_undefined, true));
	}

	void EffectNode::add_input_port(const String &swizzle)
	{
		m_ports.push_back(new EffectNodePort(this, String(),
			swizzle, ect_undefined, true));
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
		EffectChangeType change;
		Uint16 value_count, connected_value_count;

		if (updated.count(this) > 0)
		{
			return;
		}

		updated.insert(this);

		change = ect_undefined;
		value_count = get_initial_value_count();
		connected_value_count = get_initial_value_count();

		set_value_count(value_count);

		BOOST_FOREACH(EffectNodePort &port, m_ports)
		{
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
					value_count = connected_value_count;
				}
			}

			if (port.get_undefined_change())
			{
				change = std::max(change,
					port.get_connected_change());
			}
		}

		set_change(change);
		set_value_count(value_count);
	}

	void EffectNode::write(const bool glsl_120,
		const EffectChangeType change,
		StringBitSet16Map &parameters_indices,
		ShaderSourceParameterVector &vertex_parameters,
		ShaderSourceParameterVector &fragment_parameters,
		OutStream &vertex_str, OutStream &fragment_str,
		EffectNodePtrSet &written)
	{
		if (written.count(this) > 0)
		{
			return;
		}

		written.insert(this);

		do_write(glsl_120, change, parameters_indices,
			vertex_parameters, fragment_parameters, vertex_str,
			fragment_str, written);
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
