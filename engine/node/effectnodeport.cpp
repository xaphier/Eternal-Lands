/****************************************************************************
 *            effectnodeport.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "effectnodeport.hpp"
#include "effectnode.hpp"
#include "exceptions.hpp"

namespace eternal_lands
{

	namespace
	{

		Uint8Array4 get_swizzle_from_str(const String &swizzle)
		{
			Uint8Array4 result;
			Uint16 i, set, tmp;

			set = 0;

			for (i = 0; i < 4; ++i)
			{
				if (i >= swizzle.get().size())
				{
					result[i] = UTF8(' ');

					continue;
				}

				result[i] = swizzle.get()[i];

				switch (result[i])
				{
					case UTF8('r'):
					case UTF8('g'):
					case UTF8('b'):
					case UTF8('a'):
						tmp = 1;
						break;
					case UTF8('x'):
					case UTF8('y'):
					case UTF8('z'):
					case UTF8('w'):
						tmp = 2;
						break;
					case UTF8('u'):
					case UTF8('v'):
						tmp = 3;
						break;
					case UTF8('s'):
					case UTF8('t'):
					case UTF8('p'):
					case UTF8('q'):
						tmp = 4;
						break;
					case UTF8('?'):
						tmp = 5;
						break;
					case UTF8('*'):
						tmp = 6;
						break;
					default:
						tmp = 0;
						break;
				}

				if (i == 0)
				{
					set = tmp;
				}

				if ((tmp != set) || (tmp == 0))
				{
					EL_THROW_EXCEPTION(
						InvalidParameterException()
						<< errinfo_parameter_name(
							UTF8("swizzle"))
						<< errinfo_string_value(
							swizzle));
				}
			}

			return result;
		}

	}

	EffectNodePort::EffectNodePort(const EffectNodePtr node,
		const String &var, const String &description,
		const String &swizzle, const EffectChangeType change,
		const bool input): m_node(node), m_var(var),
		m_description(description),
		m_swizzle(get_swizzle_from_str(swizzle)),
		m_change(change), m_input(input)
	{
		assert(m_input || !get_var().get().empty());
		assert(m_node != 0);
	}

	EffectNodePort::EffectNodePort(const EffectNodePtr node,
		const String &var, const String &swizzle,
		const EffectChangeType change, const bool input): m_node(node),
		m_var(var), m_description(swizzle),
		m_swizzle(get_swizzle_from_str(swizzle)),
		m_change(change), m_input(input)
	{
		assert(m_input || !get_var().get().empty());
		assert(m_node != 0);
	}

	EffectNodePort::~EffectNodePort() noexcept
	{
		BOOST_FOREACH(const EffectNodePortPtr port, m_connections)
		{
			disconnect(port);
		}
	}

	bool EffectNodePort::check_connection(const EffectNodePortPtr port)
	{
		EffectNodePtrSet checking;

		if (get_output())
		{
			return port->check_connection(this, checking);
		}
		else
		{
			return check_connection(port, checking);
		}
	}

	bool EffectNodePort::check_connection(const EffectNodePortPtr port,
		EffectNodePtrSet &checking) const
	{
		NULL_PTR_CHECK(port, UTF8("port"));

		checking.insert(port->get_node_ptr());

		if (get_input() == port->get_input())
		{
			return false;
		}

		if (!get_convertable(port))
		{
			return false;
		}

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
		get_node_ptr()->update();

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
		get_node_ptr()->update();
	}

	Uint16 EffectNodePort::get_value_count() const
	{
		if (get_general_type())
		{
			return get_node_value_count();
		}

		return get_var_count();
	}

	String EffectNodePort::get_connected_var_swizzled() const
	{
		StringStream str;

		if (get_output())
		{
			return get_var_swizzled();
		}

		if (!get_connected())
		{
			return String();
		}

		if (get_value_count() == get_connection()->get_value_count())
		{
			return get_connection()->get_var_swizzled();
		}

		str << UTF8("vec") << get_value_count() << UTF8("(");
		str << get_connection()->get_var_swizzled() << UTF8(")");

		return String(str.str());
	}

	String EffectNodePort::get_var_swizzled() const
	{
		StringStream str;
		Uint16 i;

		str << UTF8('.');

		for (i = 0; i < 4; ++i)
		{
			switch (get_swizzle()[i])
			{
				case UTF8('r'):
				case UTF8('x'):
				case UTF8('u'):
				case UTF8('s'):
					str << UTF8('r');
					break;
				case UTF8('g'):
				case UTF8('y'):
				case UTF8('v'):
				case UTF8('t'):
					str << UTF8('g');
					break;
				case UTF8('b'):
				case UTF8('z'):
				case UTF8('p'):
					str << UTF8('b');
					break;
				case UTF8('a'):
				case UTF8('w'):
				case UTF8('q'):
					str << UTF8('a');
					break;
			}
		}

		if (str.str().size() == 1)
		{
			return get_var();
		}

		return String(get_var().get() + str.str());
	}

	Uint16 EffectNodePort::get_node_value_count() const
	{
		return get_node().get_value_count();
	}

	Uint16 EffectNodePort::get_connected_value_count() const
	{
		if (!get_general_type())
		{
			return get_var_count();
		}

		BOOST_FOREACH(const EffectNodePortPtr port, m_connections)
		{
			return port->get_value_count();
		}

		return get_var_count();
	}

	EffectChangeType EffectNodePort::get_change() const
	{
		if (!get_undefined_change())
		{
			return get_node_change();
		}

		return m_change;
	}

	EffectChangeType EffectNodePort::get_node_change() const
	{
		return get_node().get_change();
	}

	EffectChangeType EffectNodePort::get_connected_change() const
	{
		if (!get_undefined_change())
		{
			return get_change();
		}

		BOOST_FOREACH(const EffectNodePortPtr port, m_connections)
		{
			return port->get_change();
		}

		return get_change();
	}

	void EffectNodePort::update(EffectNodePtrSet &updated)
	{
		BOOST_FOREACH(EffectNodePortPtr port, m_connections)
		{
			assert(port != 0);
			assert(port->m_node != 0);
			port->m_node->update(updated);
		}
	}

	void EffectNodePort::write(const bool glsl_120,
		const EffectChangeType change,
		StringBitSet16Map &parameters_indices,
		ShaderSourceParameterVector &vertex_parameters,
		ShaderSourceParameterVector &fragment_parameters,
		OutStream &vertex_str, OutStream &fragment_str,
		EffectNodePtrSet &written)
	{
		EffectChangeType new_change;

		if (get_output())
		{
			m_node->write(glsl_120, change, parameters_indices,
				vertex_parameters, fragment_parameters,
				vertex_str, fragment_str, written);

			return;
		}

		BOOST_FOREACH(EffectNodePortPtr port, m_connections)
		{
			new_change = change;

			if ((get_change() == ect_fragment) &&
				(port->get_change() == ect_vertex))
			{
				port->add_parameter(parameters_indices);

				new_change = ect_vertex;
			}

			port->write(glsl_120, new_change, parameters_indices,
				vertex_parameters, fragment_parameters,
				vertex_str, fragment_str, written);
		}
	}

	bool EffectNodePort::get_convertable(const EffectNodePortPtr port)
		const
	{
		NULL_PTR_CHECK(port, UTF8("port"));

		return (get_value_count() == port->get_value_count()) ||
			(get_value_count() == 0) ||
			(port->get_value_count() == 0) ||
			((get_value_count() == 1) && get_output()) ||
			((port->get_value_count() == 1) && port->get_output());
	}

	BitSet16 EffectNodePort::get_var_indices() const
	{
		BitSet16 result;
		Uint16 i;

		for (i = 0; i < 4; ++i)
		{
			switch (get_swizzle()[i])
			{
				case UTF8('r'):
				case UTF8('x'):
				case UTF8('u'):
				case UTF8('s'):
				case UTF8('?'):
					result[0] = true;
					break;
				case UTF8('g'):
				case UTF8('y'):
				case UTF8('v'):
				case UTF8('t'):
					result[1] = true;
					break;
				case UTF8('b'):
				case UTF8('z'):
				case UTF8('p'):
					result[2] = true;
					break;
				case UTF8('a'):
				case UTF8('w'):
				case UTF8('q'):
					result[3] = true;
					break;
			}
		}

		return result;
	}

	Uint16 EffectNodePort::get_var_count() const
	{
		Uint16 i, result;

		result = 0;

		for (i = 0; i < 4; ++i)
		{
			switch (get_swizzle()[i])
			{
				case UTF8('r'):
				case UTF8('x'):
				case UTF8('u'):
				case UTF8('s'):
				case UTF8('?'):
				case UTF8('g'):
				case UTF8('y'):
				case UTF8('v'):
				case UTF8('t'):
				case UTF8('b'):
				case UTF8('z'):
				case UTF8('p'):
				case UTF8('a'):
				case UTF8('w'):
				case UTF8('q'):
					result++;
					break;
			}
		}

		return result;
	}

	void EffectNodePort::add_parameter(
		StringBitSet16Map &parameters_indices)
	{
/*
		StringBitSet16Map::iterator found;

		found = parameters_indices.find(name);

		if (found == parameters_indices.end())
		{
			found->second |= used_indices;
			return;
		}

		parameters_indices[name] = used_indices;
*/
	}

}
