/****************************************************************************
 *            effectfunction.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "effectfunction.hpp"
#include "effectnodeport.hpp"
#include "xmlutil.hpp"
#include "xmlwriter.hpp"

namespace eternal_lands
{

	EffectFunction::EffectFunction(): m_type(eft_add)
	{
	}

	EffectFunction::EffectFunction(const String &name, const Uint32 id,
		const EffectFunctionType type,
		Mt19937RandomUuidGenerator &uuid_generator):
		EffectNode(name, id, uuid_generator()), m_type(type)
	{
		Uint32 index;

		index = 0;

		BOOST_FOREACH(String &var, m_var_names)
		{
			var = get_var_name(index);
			index++;
		}

		switch (get_type())
		{
			case eft_length:
				add_input_port(String(), String(UTF8("*")),
					uuid_generator());
				add_output_port(m_var_names[0], String(),
					String(UTF8("?")), uuid_generator());
				break;
			case eft_distance:
				add_input_port(String(UTF8("p0")),
					String(UTF8("*")), uuid_generator());
				add_input_port(String(UTF8("p1")),
					String(UTF8("*")), uuid_generator());
				add_output_port(m_var_names[0], String(),
					String(UTF8("?")), uuid_generator());
				break;
			case eft_dot:
				add_input_port(String(UTF8("x")),
					String(UTF8("*")), uuid_generator());
				add_input_port(String(UTF8("y")),
					String(UTF8("*")), uuid_generator());
				add_output_port(m_var_names[0], String(),
					String(UTF8("?")), uuid_generator());
				break;
			case eft_add:
			case eft_sub:
			case eft_mul:
			case eft_div:
			case eft_pow:
			case eft_mod:
			case eft_min:
			case eft_max:
			case eft_atan2:
				add_input_port(String(UTF8("x")),
					String(UTF8("*")), uuid_generator());
				add_input_port(String(UTF8("y")),
					String(UTF8("*")), uuid_generator());
				add_output_port(m_var_names[0], String(),
					String(UTF8("*")), uuid_generator());
				break;
			case eft_reflect:
				add_input_port(String(UTF8("I")),
					String(UTF8("*")), uuid_generator());
				add_input_port(String(UTF8("N")),
					String(UTF8("*")), uuid_generator());
				add_output_port(m_var_names[0], String(),
					String(UTF8("*")), uuid_generator());
				break;
			case eft_mad:
				add_input_port(String(UTF8("x")),
					String(UTF8("*")), uuid_generator());
				add_input_port(String(UTF8("y")),
					String(UTF8("*")), uuid_generator());
				add_input_port(String(UTF8("z")),
					String(UTF8("*")), uuid_generator());
				add_output_port(m_var_names[0], String(),
					String(UTF8("*")), uuid_generator());
				break;
			case eft_clamp:
				add_input_port(String(UTF8("x")),
					String(UTF8("*")), uuid_generator());
				add_input_port(String(UTF8("minVal")),
					String(UTF8("*")), uuid_generator());
				add_input_port(String(UTF8("maxVal")),
					String(UTF8("*")), uuid_generator());
				add_output_port(m_var_names[0], String(),
					String(UTF8("*")), uuid_generator());
				break;
			case eft_mix:
				add_input_port(String(UTF8("x")),
					String(UTF8("*")), uuid_generator());
				add_input_port(String(UTF8("y")),
					String(UTF8("*")), uuid_generator());
				add_input_port(String(UTF8("a")),
					String(UTF8("*")), uuid_generator());
				add_output_port(m_var_names[0], String(),
					String(UTF8("*")), uuid_generator());
				break;
			case eft_step:
				add_input_port(String(UTF8("edge")),
					String(UTF8("*")), uuid_generator());
				add_input_port(String(UTF8("x")),
					String(UTF8("*")), uuid_generator());
				add_output_port(m_var_names[0], String(),
					String(UTF8("*")), uuid_generator());
				break;
			case eft_smoothstep:
				add_input_port(String(UTF8("edge0")),
					String(UTF8("*")), uuid_generator());
				add_input_port(String(UTF8("edge1")),
					String(UTF8("*")), uuid_generator());
				add_input_port(String(UTF8("x")),
					String(UTF8("*")), uuid_generator());
				add_output_port(m_var_names[0], String(),
					String(UTF8("*")), uuid_generator());
				break;
			case eft_radians:
				add_input_port(String(UTF8("degrees")),
					String(UTF8("*")), uuid_generator());
				add_output_port(m_var_names[0], String(),
					String(UTF8("*")), uuid_generator());
				break;
			case eft_degrees:
				add_input_port(String(UTF8("radians")),
					String(UTF8("*")), uuid_generator());
				add_output_port(m_var_names[0], String(),
					String(UTF8("*")), uuid_generator());
				break;
			case eft_sin:
			case eft_cos:
			case eft_tan:
			case eft_asin:
			case eft_acos:
			case eft_atan:
			case eft_sinh:
			case eft_cosh:
			case eft_tanh:
			case eft_asinh:
			case eft_acosh:
			case eft_atanh:
				add_input_port(String(UTF8("angle")),
					String(UTF8("*")), uuid_generator());
				add_output_port(m_var_names[0], String(),
					String(UTF8("*")), uuid_generator());
				break;
			case eft_exp:
			case eft_log:
			case eft_exp2:
			case eft_log2:
			case eft_sqrt:
			case eft_inversesqrt:
			case eft_abs:
			case eft_sign:
			case eft_round:
			case eft_roundEven:
			case eft_trunc:
			case eft_floor:
			case eft_ceil:
			case eft_fract:
			case eft_normalize:
			case eft_saturate:
				add_input_port(String("x"), String(UTF8("*")),
					uuid_generator());
				add_output_port(m_var_names[0], String(),
					String(UTF8("*")), uuid_generator());
				break;
			case eft_derivatives:
				add_input_port(String(UTF8("uv")),
					String(UTF8("*")), uuid_generator(),
					ect_fragment);
				add_output_port(m_var_names[0],
					String(UTF8("dFdx")),
					String(UTF8("*")), uuid_generator(),
					ect_fragment);
				add_output_port(m_var_names[1],
					String(UTF8("dFdy")),
					String(UTF8("*")), uuid_generator(),
					ect_fragment);
				break;
			case eft_fwidth:
				add_input_port(String(UTF8("p")),
					String(UTF8("*")), uuid_generator(),
					ect_fragment);
				add_output_port(m_var_names[0], String(),
					String(UTF8("*")), uuid_generator(),
					ect_fragment);
				break;
			case eft_refract:
				add_input_port(String(UTF8("I")),
					String(UTF8("*")), uuid_generator());
				add_input_port(String(UTF8("N")),
					String(UTF8("*")), uuid_generator());
				add_input_port(String(UTF8("eta")),
					String(UTF8("?")), uuid_generator());
				add_output_port(m_var_names[0], String(),
					String(UTF8("*")), uuid_generator());
				break;
		}
	}

	EffectFunction::~EffectFunction() noexcept
	{
	}

	Uint16 EffectFunction::get_initial_value_count() const
	{
		return 0;
	}

	void EffectFunction::write(const Uint16StringMap &array_layers,
		const ShaderVersionType version, const bool low_quality,
		const EffectChangeType change,
		StringUint16Map &parameters,
		ShaderSourceParameterVector &vertex_parameters,
		ShaderSourceParameterVector &fragment_parameters,
		OutStream &vertex_str, OutStream &fragment_str,
		EffectNodePtrSet &vertex_written,
		EffectNodePtrSet &fragment_written)
	{
		StringVector inputs;
		String output;
		EffectChangeType input_change;

		input_change = ect_constant;

		BOOST_FOREACH(const EffectNodePort &port, get_ports())
		{
			if (port.get_input())
			{
				inputs.push_back(
					port.get_connected_var_swizzled());

				input_change = std::max(input_change,
					port.get_change());
			}
		}

		if (input_change == ect_constant)
		{
			input_change = change;
		}

		BOOST_FOREACH(EffectNodePort &port, get_ports())
		{
			if (port.get_input())
			{
				port.write(array_layers, version, low_quality,
					input_change, parameters,
					vertex_parameters, fragment_parameters,
					vertex_str, fragment_str,
					vertex_written, fragment_written);
			}
		}

		OutStream &str = input_change == ect_fragment ? fragment_str :
			vertex_str;

		if (input_change == ect_fragment)
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

		str << get_value_count_type_str() << UTF8(" ");
		str << m_var_names[0] << UTF8(" = ");

		switch (get_type())
		{
			case eft_add:
				str << inputs[0] << UTF8(" + ") << inputs[1];
				break;
			case eft_sub:
				str << inputs[0] << UTF8(" - ") << inputs[1];
				break;
			case eft_mul:
				str << inputs[0] << UTF8(" * ") << inputs[1];
				break;
			case eft_div:
				str << inputs[0] << UTF8(" / ") << inputs[1];
				break;
			case eft_mad:
				str << inputs[0] << UTF8(" * ") << inputs[1];
				str << UTF8(" + ") << inputs[2];
				break;
			case eft_length:
				str << UTF8("length(") << inputs[0];
				str << UTF8(")");
				break;
			case eft_distance:
				str << UTF8("distance(") << inputs[0];
				str << UTF8(", ") << inputs[1] << UTF8(")");
				break;
			case eft_dot:
				str << UTF8("dot(") << inputs[0] << UTF8(", ");
				str << inputs[1] << UTF8(")");
				break;
			case eft_pow:
				str << UTF8("pow(") << inputs[0] << UTF8(", ");
				str << inputs[1] << UTF8(")");
				break;
			case eft_mod:
				str << UTF8("mod(") << inputs[0] << UTF8(", ");
				str << inputs[1] << UTF8(")");
				break;
			case eft_min:
				str << UTF8("min(") << inputs[0] << UTF8(", ");
				str << inputs[1] << UTF8(")");
				break;
			case eft_max:
				str << UTF8("max(") << inputs[0] << UTF8(", ");
				str << inputs[1] << UTF8(")");
				break;
			case eft_atan2:
				str << UTF8("atan2(") << inputs[0];
				str << UTF8(", ") << inputs[1] << UTF8(")");
				break;
			case eft_reflect:
				str << UTF8("reflect(") << inputs[0];
				str << UTF8(", ") << inputs[1] << UTF8(")");
				break;
			case eft_clamp:
				str << UTF8("clamp(") << inputs[0];
				str << UTF8(", ") << inputs[1] << UTF8(", ");
				str << inputs[2] << UTF8(")");
				break;
			case eft_mix:
				str << UTF8("mix(") << inputs[0] << UTF8(", ");
				str << inputs[1] << UTF8(", ") << inputs[2];
				str << UTF8(")");
				break;
			case eft_step:
				str << UTF8("step(") << inputs[0] << UTF8(", ");
				str << inputs[1] << UTF8(", ") << inputs[2];
				str << UTF8(")");
				break;
			case eft_smoothstep:
				str << UTF8("smoothstep(") << inputs[0];
				str << UTF8(", ") << inputs[1] << UTF8(", ");
				str << inputs[2] << UTF8(")");
				break;
			case eft_radians:
				str << UTF8("radians(") << inputs[0];
				str << UTF8(")");
				break;
			case eft_degrees:
				str << UTF8("degrees(") << inputs[0];
				str << UTF8(")");
				break;
			case eft_sin:
				str << UTF8("sin(") << inputs[0] << UTF8(")");
				break;
			case eft_cos:
				str << UTF8("cos(") << inputs[0] << UTF8(")");
				break;
			case eft_tan:
				str << UTF8("tan(") << inputs[0] << UTF8(")");
				break;
			case eft_asin:
				str << UTF8("asin(") << inputs[0] << UTF8(")");
				break;
			case eft_acos:
				str << UTF8("acos(") << inputs[0] << UTF8(")");
				break;
			case eft_atan:
				str << UTF8("atan(") << inputs[0] << UTF8(")");
				break;
			case eft_sinh:
				str << UTF8("sinh(") << inputs[0] << UTF8(")");
				break;
			case eft_cosh:
				str << UTF8("cosh(") << inputs[0] << UTF8(")");
				break;
			case eft_tanh:
				str << UTF8("tanh(") << inputs[0] << UTF8(")");
				break;
			case eft_asinh:
				str << UTF8("asinh(") << inputs[0] << UTF8(")");
				break;
			case eft_acosh:
				str << UTF8("acosh(") << inputs[0] << UTF8(")");
				break;
			case eft_atanh:
				str << UTF8("atanh(") << inputs[0] << UTF8(")");
				break;
			case eft_exp:
				str << UTF8("exp(") << inputs[0] << UTF8(")");
				break;
			case eft_log:
				str << UTF8("log(") << inputs[0] << UTF8(")");
				break;
			case eft_exp2:
				str << UTF8("exp2(") << inputs[0] << UTF8(")");
				break;
			case eft_log2:
				str << UTF8("log2(") << inputs[0] << UTF8(")");
				break;
			case eft_sqrt:
				str << UTF8("sqrt(") << inputs[0] << UTF8(")");
				break;
			case eft_inversesqrt:
				str << UTF8("inversesqrt(") << inputs[0];
				str << UTF8(")");
				break;
			case eft_abs:
				str << UTF8("abs(") << inputs[0] << UTF8(")");
				break;
			case eft_sign:
				str << UTF8("sign(") << inputs[0] << UTF8(")");
				break;
			case eft_round:
				str << UTF8("round(") << inputs[0] << UTF8(")");
				break;
			case eft_roundEven:
				str << UTF8("roundEven(") << inputs[0];
				str << UTF8(")");
				break;
			case eft_trunc:
				str << UTF8("trunc(") << inputs[0] << UTF8(")");
				break;
			case eft_floor:
				str << UTF8("floor(") << inputs[0] << UTF8(")");
				break;
			case eft_ceil:
				str << UTF8("ceil(") << inputs[0] << UTF8(")");
				break;
			case eft_fract:
				str << UTF8("fract(") << inputs[0] << UTF8(")");
				break;
			case eft_normalize:
				str << UTF8("normalize(") << inputs[0];
				str << UTF8(")");
				break;
			case eft_saturate:
				str << UTF8("saturate(") << inputs[0];
				str << UTF8(", 0.0, 1.0)");
				break;
			case eft_refract:
				str << UTF8("refract(") << inputs[0];
				str << UTF8(", ") << inputs[1] << UTF8(", ");
				str << inputs[2] << UTF8(")");
				break;
			case eft_derivatives:
				str << UTF8("dFdx(") << inputs[0];
				str << UTF8(");\n");
				str << get_value_count_type_str() << UTF8(" ");
				str << m_var_names[1] << UTF8(" = dFdy(");
				str << inputs[0] << UTF8(")");
				break;
			case eft_fwidth:
				str << UTF8("fwidth(") << inputs[0];
				str << UTF8(")");
				break;
		}

		str << UTF8(";\n");
	}

	String EffectFunction::get_description() const
	{
		return EffectFunctionUtil::get_description(get_type());
	}

	void EffectFunction::save_xml(const XmlWriterSharedPtr &writer)
	{
		writer->start_element(UTF8("effect_function"));

		EffectNode::save_xml(writer);

		writer->write_element(UTF8("var_name_0"), m_var_names[0]);
		writer->write_element(UTF8("var_name_1"), m_var_names[1]);
		writer->write_element(UTF8("type"),
			EffectFunctionUtil::get_str(get_type()));

		writer->end_element();
	}

	void EffectFunction::load_xml(const xmlNodePtr node)
	{
		xmlNodePtr it;

		if (xmlStrcmp(node->name, BAD_CAST UTF8("effect_function"))
			!= 0)
		{
			return;
		}

		EffectNode::load_xml(node);

		it = XmlUtil::children(node, true);

		do
		{
			if (xmlStrcmp(it->name, BAD_CAST UTF8("var_name_0"))
				== 0)
			{
				m_var_names[0] = XmlUtil::get_string_value(it);
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("var_name_1"))
				== 0)
			{
				m_var_names[1] = XmlUtil::get_string_value(it);
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("type")) == 0)
			{
				m_type =
					EffectFunctionUtil::get_effect_function(
						XmlUtil::get_string_value(it));
			}
		}
		while (XmlUtil::next(it, true));
	}

}
