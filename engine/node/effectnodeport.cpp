/****************************************************************************
 *            effectnodeport.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "effectnodeport.hpp"
#include "effectnode.hpp"
#include "exceptions.hpp"
#include "xmlutil.hpp"
#include "xmlwriter.hpp"

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

	EffectNodePort::EffectNodePort()
	{
	}

	EffectNodePort::EffectNodePort(const EffectNodePtr node,
		const String &var, const String &name,
		const String &swizzle, const boost::uuids::uuid &uuid,
		const EffectChangeType change, const bool input): m_node(node),
		m_uuid(uuid), m_var(var), m_name(name), m_description(name),
		m_swizzle(get_swizzle_from_str(swizzle)),
		m_change(change), m_input(input)
	{
		assert(m_input || !get_var().get().empty());
		assert(m_node != nullptr);
	}

	EffectNodePort::EffectNodePort(const EffectNodePtr node,
		const String &var, const String &swizzle,
		const boost::uuids::uuid &uuid, const EffectChangeType change,
		const bool input): m_node(node), m_uuid(uuid), m_var(var),
		m_name(swizzle), m_description(swizzle),
		m_swizzle(get_swizzle_from_str(swizzle)),
		m_change(change), m_input(input)
	{
		assert(m_input || !get_var().get().empty());
		assert(m_node != nullptr);
	}

	EffectNodePort::~EffectNodePort() noexcept
	{
		BOOST_FOREACH(const EffectNodePortPtr port, get_connections())
		{
			port->do_disconnect(this);
			do_disconnect(port);
		}

		BOOST_FOREACH(const EffectNodePortPtr port, get_connections())
		{
			port->get_node_ptr()->update();
		}
	}

	bool EffectNodePort::check_connection(const EffectNodePortPtr port)
	{
		EffectNodePtrVector checking;

		if (get_output())
		{
			return port->do_check_connection(this, checking);
		}
		else
		{
			return do_check_connection(port, checking);
		}
	}

	bool EffectNodePort::do_check_connection(const EffectNodePortPtr port,
		EffectNodePtrVector &checking) const
	{
		NULL_PTR_CHECK(port, UTF8("port"));

		if (get_input() == port->get_input())
		{
			return false;
		}

		if (!get_convertable(port))
		{
			return false;
		}

		checking.push_back(port->get_node_ptr());

		return get_node_ptr()->check_connections(checking);
	}

	bool EffectNodePort::check_connection(EffectNodePtrVector &checking)
		const
	{
		BOOST_FOREACH(const EffectNodePortPtr port, get_connections())
		{
			if (!port->get_node_ptr()->check_connections(checking))
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
		return get_node_ptr()->get_value_count();
	}

	Uint16 EffectNodePort::get_connected_value_count() const
	{
		if (!get_general_type())
		{
			return get_var_count();
		}

		if (get_connected())
		{
			return get_connection()->get_value_count();
		}

		return get_var_count();
	}

	EffectChangeType EffectNodePort::get_change() const
	{
		if (!get_undefined_change())
		{
			return m_change;
		}

		return get_node_change();
	}

	EffectChangeType EffectNodePort::get_node_change() const
	{
		return get_node_ptr()->get_change();
	}

	EffectChangeType EffectNodePort::get_connected_change() const
	{
		if (!get_undefined_change())
		{
			return get_change();
		}

		if (get_connected())
		{
			return get_connection()->get_change();
		}

		return get_change();
	}

	void EffectNodePort::update(EffectNodePtrSet &updated)
	{
		BOOST_FOREACH(const EffectNodePortPtr port, get_connections())
		{
			port->get_node_ptr()->update(updated);
		}
	}

	void EffectNodePort::write(const Uint16StringMap &array_layers,
		const ShaderVersionType version, const bool low_quality,
		const EffectChangeType change,
		StringUint16Map &parameters,
		ShaderSourceParameterVector &vertex_parameters,
		ShaderSourceParameterVector &fragment_parameters,
		OutStream &vertex_str, OutStream &fragment_str,
		EffectNodePtrSet &vertex_written,
		EffectNodePtrSet &fragment_written)
	{
		EffectChangeType new_change;

		if (get_output())
		{
			get_node_ptr()->write(array_layers, version,
				low_quality, change, parameters,
				vertex_parameters, fragment_parameters,
				vertex_str, fragment_str, vertex_written,
				fragment_written);

			return;
		}

		BOOST_FOREACH(const EffectNodePortPtr port, get_connections())
		{
			new_change = change;

			if ((get_change() == ect_fragment) &&
				(port->get_change() == ect_vertex))
			{
				parameters[port->get_var()] =
					port->get_value_count();

				new_change = ect_vertex;
			}

			port->write(array_layers, version, low_quality,
				new_change, parameters, vertex_parameters,
				fragment_parameters, vertex_str, fragment_str,
				vertex_written, fragment_written);
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

	void EffectNodePort::save_xml(const XmlWriterSharedPtr &writer) const
	{
		StringType swizzle;
		Uint16 i;

		for (i = 0; i < 4; ++i)
		{
			switch (get_swizzle()[i])
			{
				case UTF8('r'):
				case UTF8('g'):
				case UTF8('b'):
				case UTF8('a'):
				case UTF8('x'):
				case UTF8('y'):
				case UTF8('z'):
				case UTF8('w'):
				case UTF8('u'):
				case UTF8('v'):
				case UTF8('s'):
				case UTF8('t'):
				case UTF8('p'):
				case UTF8('q'):
				case UTF8('?'):
				case UTF8('*'):
					swizzle += '\0' + get_swizzle()[i];
					break;
				default:
					break;
			}
		}

		writer->start_element(UTF8("effect_node_port"));

		writer->write_uuid_element(UTF8("uuid"), get_uuid());
		writer->write_element(UTF8("var"), get_var());
		writer->write_element(UTF8("name"), get_name());
		writer->write_element(UTF8("description"), get_description());
		writer->write_element(UTF8("swizzle"), swizzle);
		writer->write_element(UTF8("change"),
			EffectChangeUtil::get_str(get_change()));
		writer->write_bool_element(UTF8("input"), get_input());

		writer->end_element();
	}
		
	void EffectNodePort::save_connections_xml(
		const XmlWriterSharedPtr &writer) const
	{
		if (get_output())
		{
			return;
		}

		BOOST_FOREACH(const EffectNodePortPtr &connection,
			m_connections)
		{
			writer->start_element(UTF8("connection"));

			writer->write_uuid_element(UTF8("uuid"), get_uuid());
			writer->write_uuid_element(UTF8("uuid"),
				connection->get_uuid());

			writer->end_element();
		}
	}

	EffectNodePort::EffectNodePort(const EffectNodePtr effect_node,
		const xmlNodePtr node): m_node(effect_node)
	{
		xmlNodePtr it;

		if (xmlStrcmp(node->name, BAD_CAST UTF8("effect_node_port"))
			!= 0)
		{
			return;
		}

		it = XmlUtil::children(node, true);

		do
		{
			if (xmlStrcmp(it->name, BAD_CAST UTF8("uuid")) == 0)
			{
				m_uuid = XmlUtil::get_uuid_value(it);
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("var")) == 0)
			{
				m_var = XmlUtil::get_string_value(it);
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("name")) == 0)
			{
				m_name = XmlUtil::get_string_value(it);
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("description"))
				== 0)
			{
				m_description = XmlUtil::get_string_value(it);
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("swizzle")) == 0)
			{
				m_swizzle = get_swizzle_from_str(
					XmlUtil::get_string_value(it));
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("change")) == 0)
			{
				m_change = EffectChangeUtil::get_effect_change(
					XmlUtil::get_string_value(it));
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("input")) == 0)
			{
				m_input = XmlUtil::get_bool_value(it);
			}
		}
		while (XmlUtil::next(it, true));
	}

}
