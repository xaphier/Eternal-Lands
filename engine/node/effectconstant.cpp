/****************************************************************************
 *            effectconstant.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "effectconstant.hpp"
#include "effectnodeport.hpp"
#include "xmlutil.hpp"
#include "xmlwriter.hpp"

namespace eternal_lands
{

	EffectConstant::EffectConstant(): m_value(0.0f, 0.0f, 0.0f, 1.0f),
		m_type(ect_float)
	{
	}

	EffectConstant::EffectConstant(const String &name, const Uint32 id,
		const EffectConstantType type,
		Mt19937RandomUuidGenerator &uuid_generator):
		EffectNode(name, id, uuid_generator()),
		m_value(0.0f, 0.0f, 0.0f, 1.0f), m_type(type)
	{
		m_var_name = get_var_name();

		switch (get_type())
		{
			case ect_float:
				add_output_port(m_var_name, String(),
					String(UTF8("?")), uuid_generator(),
					ect_constant);
				break;
			case ect_direction_xy:
				add_output_port(m_var_name, String(UTF8("xy")),
					uuid_generator(),ect_constant);
				break;
			case ect_color_rgb:
				add_output_port(m_var_name,
					String(UTF8("rgb")), uuid_generator(),
					ect_constant);
				break;
			case ect_vec2:
				add_output_port(m_var_name, String(UTF8("xy")),
					uuid_generator(),ect_constant);
				break;
			case ect_vec3:
				add_output_port(m_var_name,
					String(UTF8("xyz")), uuid_generator(),
					ect_constant);
				break;
			case ect_vec4:
				add_output_port(m_var_name,
					String(UTF8("xyzw")), uuid_generator(),
					ect_constant);
				break;
		}
	}

	EffectConstant::~EffectConstant() noexcept
	{
	}

	Uint16 EffectConstant::get_initial_value_count() const
	{
		return EffectConstantUtil::get_count(get_type());
	}

	void EffectConstant::write(const Uint16StringMap &array_layers,
		const ShaderVersionType version,
		const EffectQualityType quality, const EffectChangeType change,
		StringUint16Map &parameters,
		ShaderSourceParameterVector &vertex_parameters,
		ShaderSourceParameterVector &fragment_parameters,
		OutStream &vertex_str, OutStream &fragment_str,
		EffectNodePtrSet &vertex_written,
		EffectNodePtrSet &fragment_written)
	{
		OutStream &str = change == ect_fragment ? fragment_str :
			vertex_str;

		if (change == ect_fragment)
		{
			if (fragment_written.count(this) > 0)
			{
				return;
			}

			fragment_written.insert(this);
		}
		else
		{
			if (vertex_written.count(this) > 0)
			{
				return;
			}

			vertex_written.insert(this);
		}

		str << UTF8("constant ");

		switch (get_type())
		{
			case ect_float:
				str << UTF8("float ") << m_var_name;;
				str << UTF8(" = ") << get_value().x;
				break;
			case ect_direction_xy:
			case ect_vec2:
				str << UTF8("vec2 ") << m_var_name;
				str << UTF8(" = ") << UTF8("vec2(");
				str << get_value().x << UTF8(", ");
				str << get_value().y << UTF8(")");
				break;
			case ect_color_rgb:
			case ect_vec3:
				str << UTF8("vec3 ") << m_var_name;
				str << UTF8(" = ") << UTF8("vec3(");
				str << get_value().x << UTF8(", ");
				str << get_value().y << UTF8(", ");
				str << get_value().z << UTF8(")");
				break;
			case ect_vec4:
				str << UTF8("vec4 ") << m_var_name;
				str << UTF8(" = ") << UTF8("vec4(");
				str << get_value().x << UTF8(", ");
				str << get_value().y << UTF8(", ");
				str << get_value().z << UTF8(", ");
				str << get_value().w << UTF8(")");
				break;
		}

		str << UTF8(";\n");
	}

	String EffectConstant::get_description() const
	{
		StringStream str;

		switch (get_type())
		{
			case ect_float:
				str << get_value().x;
				break;
			case ect_direction_xy:
			case ect_vec2:
				str << glm::to_string(glm::vec2(get_value()));
				break;
			case ect_color_rgb:
			case ect_vec3:
				str << glm::to_string(glm::vec3(get_value()));
				break;
			case ect_vec4:
				str << glm::to_string(glm::vec4(get_value()));
				break;
		}

		return String(str.str());
	}

	void EffectConstant::save_xml(const XmlWriterSharedPtr &writer)
	{
		writer->start_element(UTF8("effect_constant"));

		EffectNode::save_xml(writer);

		writer->write_vec4_element(UTF8("value"), get_value());
		writer->write_element(UTF8("var_name"), get_var_name());
		writer->write_element(UTF8("type"),
			EffectConstantUtil::get_str(get_type()));

		writer->end_element();
	}

	void EffectConstant::load_xml(const xmlNodePtr node)
	{
		xmlNodePtr it;

		if (xmlStrcmp(node->name, BAD_CAST UTF8("effect_constant"))
			!= 0)
		{
			return;
		}

		EffectNode::load_xml(node);

		it = XmlUtil::children(node, true);

		do
		{
			if (xmlStrcmp(it->name, BAD_CAST UTF8("value")) == 0)
			{
				m_value = XmlUtil::get_vec4_value(it);
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("var_name")) == 0)
			{
				m_var_name = XmlUtil::get_string_value(it);
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("type")) == 0)
			{
				m_type =
					EffectConstantUtil::get_effect_constant(
						XmlUtil::get_string_value(it));
			}
		}
		while (XmlUtil::next(it, true));
	}

}
