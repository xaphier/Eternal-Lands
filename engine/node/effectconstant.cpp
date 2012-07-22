/****************************************************************************
 *            effectconstant.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "effectconstant.hpp"
#include "effectnodeport.hpp"

namespace eternal_lands
{

	EffectConstant::EffectConstant(const String &name, const Uint32 id,
		const EffectConstantType type): EffectNode(name, id),
		m_type(type)
	{
		m_var_name = get_var_name();

		switch (get_type())
		{
			case ect_float:
				add_output_port(m_var_name, String(),
					String(UTF8("?")), ect_constant);
				break;
			case ect_direction_xy:
				add_output_port(m_var_name, String(UTF8("xy")),
					ect_constant);
				break;
			case ect_color_rgb:
				add_output_port(m_var_name,
					String(UTF8("rgb")), ect_constant);
				break;
			case ect_vec2:
				add_output_port(m_var_name, String(UTF8("xy")),
					ect_constant);
				break;
			case ect_vec3:
				add_output_port(m_var_name,
					String(UTF8("xyz")), ect_constant);
				break;
			case ect_vec4:
				add_output_port(m_var_name,
					String(UTF8("xyzw")), ect_constant);
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
		const ShaderVersionType version, const bool low_quality,
		const EffectChangeType change,
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

}
