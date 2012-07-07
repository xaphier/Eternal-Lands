/****************************************************************************
 *            effectfunction.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "effectfunction.hpp"
#include "effectnodeport.hpp"

namespace eternal_lands
{

	EffectFunction::EffectFunction(const String &name,
		const EffectFunctionType type,
		Mt19937RandomUuidGenerator &generator): EffectNode(name),
		m_type(type)
	{
		StringStream str;

		str << UTF8("_") << generator();

		m_var = String(str.str());

		switch (get_type())
		{
			case ent_length:
				add_input_port(String(), String(UTF8("*")));
				add_output_port(m_var, String(),
					String(UTF8("?")));
				break;
			case ent_distance:
				add_input_port(String(UTF8("p0")),
					String(UTF8("*")));
				add_input_port(String(UTF8("p1")),
					String(UTF8("*")));
				add_output_port(m_var, String(),
					String(UTF8("?")));
				break;
			case ent_dot:
				add_input_port(String(UTF8("x")),
					String(UTF8("*")));
				add_input_port(String(UTF8("y")),
					String(UTF8("*")));
				add_output_port(m_var, String(),
					String(UTF8("?")));
				break;
			case ent_add:
			case ent_sub:
			case ent_mul:
			case ent_div:
			case ent_pow:
			case ent_mod:
			case ent_min:
			case ent_max:
			case ent_atan2:
				add_input_port(String(UTF8("x")),
					String(UTF8("*")));
				add_input_port(String(UTF8("y")),
					String(UTF8("*")));
				add_output_port(m_var, String(),
					String(UTF8("*")));
				break;
			case ent_reflect:
				add_input_port(String(UTF8("I")),
					String(UTF8("*")));
				add_input_port(String(UTF8("N")),
					String(UTF8("*")));
				add_output_port(m_var, String(),
					String(UTF8("*")));
				break;
			case ent_mad:
				add_input_port(String(UTF8("a")),
					String(UTF8("*")));
				add_input_port(String(UTF8("b")),
					String(UTF8("*")));
				add_input_port(String(UTF8("c")),
					String(UTF8("*")));
				add_output_port(m_var, String(),
					String(UTF8("*")));
				break;
			case ent_clamp:
				add_input_port(String(UTF8("x")),
					String(UTF8("*")));
				add_input_port(String(UTF8("minVal")),
					String(UTF8("*")));
				add_input_port(String(UTF8("maxVal")),
					String(UTF8("*")));
				add_output_port(m_var, String(),
					String(UTF8("*")));
				break;
			case ent_mix:
				add_input_port(String(UTF8("x")),
					String(UTF8("*")));
				add_input_port(String(UTF8("y")),
					String(UTF8("*")));
				add_input_port(String(UTF8("a")),
					String(UTF8("*")));
				add_output_port(m_var, String(),
					String(UTF8("*")));
				break;
			case ent_step:
				add_input_port(String(UTF8("edge")),
					String(UTF8("*")));
				add_input_port(String(UTF8("x")),
					String(UTF8("*")));
				add_output_port(m_var, String(),
					String(UTF8("*")));
				break;
			case ent_smoothstep:
				add_input_port(String(UTF8("edge0")),
					String(UTF8("*")));
				add_input_port(String(UTF8("edge1")),
					String(UTF8("*")));
				add_input_port(String(UTF8("x")),
					String(UTF8("*")));
				add_output_port(m_var, String(),
					String(UTF8("*")));
				break;
			case ent_radians:
			case ent_degrees:
			case ent_sin:
			case ent_cos:
			case ent_tan:
			case ent_asin:
			case ent_acos:
			case ent_atan:
			case ent_sinh:
			case ent_cosh:
			case ent_tanh:
			case ent_asinh:
			case ent_acosh:
			case ent_atanh:
			case ent_exp:
			case ent_log:
			case ent_exp2:
			case ent_log2:
			case ent_sqrt:
			case ent_inversesqrt:
			case ent_abs:
			case ent_sign:
			case ent_round:
			case ent_trunc:
			case ent_floor:
			case ent_ceil:
			case ent_fract:
			case ent_normalize:
			case ent_saturate:
				add_input_port(String(), String(UTF8("*")));
				add_output_port(m_var, String(),
					String(UTF8("*")));
				break;
			case ent_dFdx:
			case ent_dFdy:
				add_input_port(String(), String(UTF8("*")));
				add_output_port(m_var, String(),
					String(UTF8("*")), ect_fragment);
				break;
			case ent_refract:
				add_input_port(String(UTF8("I")),
					String(UTF8("*")));
				add_input_port(String(UTF8("N")),
					String(UTF8("*")));
				add_input_port(String(UTF8("eta")),
					String(UTF8("?")));
				add_output_port(m_var, String(),
					String(UTF8("*")));
				break;
			case ent_fresnel:
				add_input_port(String(UTF8("view")),
					String(UTF8("xyz")));
				add_input_port(String(UTF8("normal")),
					String(UTF8("xyz")));
				add_output_port(m_var, String(),
					String(UTF8("*")));
				break;
		}
	}

	EffectFunction::~EffectFunction() noexcept
	{
	}

	void EffectFunction::do_write(const bool glsl_120,
		const EffectChangeType change,
		StringBitSet16Map &parameters_indices,
		ShaderSourceParameterVector &vertex_parameters,
		ShaderSourceParameterVector &fragment_parameters,
		OutStream &vertex_str, OutStream &fragment_str,
		EffectNodePtrSet &written)
	{
		StringVector inputs;
		String output;
		EffectChangeType input_change;

		input_change = ect_constant;

		BOOST_FOREACH(EffectNodePort &port, get_ports())
		{
			if (port.get_input())
			{
				inputs.push_back(
					port.get_connected_var_swizzled());

				port.write(glsl_120, change,
					parameters_indices, vertex_parameters,
					fragment_parameters, vertex_str,
					fragment_str, written);

				input_change = std::max(input_change,
					port.get_change());
			}
		}

		if (input_change == ect_constant)
		{
			input_change = change;
		}

		OutStream &str = change == ect_fragment ? fragment_str :
			vertex_str;

		str << get_value_count_type_str() << UTF8(" ");
		str << m_var << UTF8(" = ");

		switch (get_type())
		{
			case ent_add:
				str << inputs[0] << UTF8(" + ") << inputs[1];
				break;
			case ent_sub:
				str << inputs[0] << UTF8(" - ") << inputs[1];
				break;
			case ent_mul:
				str << inputs[0] << UTF8(" * ") << inputs[1];
				break;
			case ent_div:
				str << inputs[0] << UTF8(" / ") << inputs[1];
				break;
			case ent_mad:
				str << inputs[0] << UTF8(" * ") << inputs[1];
				str << UTF8(" + ") << inputs[2];
				break;
			case ent_length:
				str << UTF8("length(") << inputs[0];
				str << UTF8(")");
				break;
			case ent_distance:
				str << UTF8("distance(") << inputs[0];
				str << UTF8(", ") << inputs[1] << UTF8(")");
				break;
			case ent_dot:
				str << UTF8("dot(") << inputs[0] << UTF8(", ");
				str << inputs[1] << UTF8(")");
				break;
			case ent_pow:
				str << UTF8("pow(") << inputs[0] << UTF8(", ");
				str << inputs[1] << UTF8(")");
				break;
			case ent_mod:
				str << UTF8("mod(") << inputs[0] << UTF8(", ");
				str << inputs[1] << UTF8(")");
				break;
			case ent_min:
				str << UTF8("min(") << inputs[0] << UTF8(", ");
				str << inputs[1] << UTF8(")");
				break;
			case ent_max:
				str << UTF8("max(") << inputs[0] << UTF8(", ");
				str << inputs[1] << UTF8(")");
				break;
			case ent_atan2:
				str << UTF8("atan2(") << inputs[0];
				str << UTF8(", ") << inputs[1] << UTF8(")");
				break;
			case ent_reflect:
				str << UTF8("reflect(") << inputs[0];
				str << UTF8(", ") << inputs[1] << UTF8(")");
				break;
			case ent_clamp:
				str << UTF8("clamp(") << inputs[0];
				str << UTF8(", ") << inputs[1] << UTF8(", ");
				str << inputs[2] << UTF8(")");
				break;
			case ent_mix:
				str << UTF8("mix(") << inputs[0] << UTF8(", ");
				str << inputs[1] << UTF8(", ") << inputs[2];
				str << UTF8(")");
				break;
			case ent_step:
				str << UTF8("step(") << inputs[0] << UTF8(", ");
				str << inputs[1] << UTF8(", ") << inputs[2];
				str << UTF8(")");
				break;
			case ent_smoothstep:
				str << UTF8("smoothstep(") << inputs[0];
				str << UTF8(", ") << inputs[1] << UTF8(", ");
				str << inputs[2] << UTF8(")");
				break;
			case ent_radians:
				str << UTF8("radians(") << inputs[0];
				str << UTF8(")");
				break;
			case ent_degrees:
				str << UTF8("degrees(") << inputs[0];
				str << UTF8(")");
				break;
			case ent_sin:
				str << UTF8("sin(") << inputs[0] << UTF8(")");
				break;
			case ent_cos:
				str << UTF8("cos(") << inputs[0] << UTF8(")");
				break;
			case ent_tan:
				str << UTF8("tan(") << inputs[0] << UTF8(")");
				break;
			case ent_asin:
				str << UTF8("asin(") << inputs[0] << UTF8(")");
				break;
			case ent_acos:
				str << UTF8("acos(") << inputs[0] << UTF8(")");
				break;
			case ent_atan:
				str << UTF8("atan(") << inputs[0] << UTF8(")");
				break;
			case ent_sinh:
				str << UTF8("sinh(") << inputs[0] << UTF8(")");
				break;
			case ent_cosh:
				str << UTF8("cosh(") << inputs[0] << UTF8(")");
				break;
			case ent_tanh:
				str << UTF8("tanh(") << inputs[0] << UTF8(")");
				break;
			case ent_asinh:
				str << UTF8("asinh(") << inputs[0] << UTF8(")");
				break;
			case ent_acosh:
				str << UTF8("acosh(") << inputs[0] << UTF8(")");
				break;
			case ent_atanh:
				str << UTF8("atanh(") << inputs[0] << UTF8(")");
				break;
			case ent_exp:
				str << UTF8("exp(") << inputs[0] << UTF8(")");
				break;
			case ent_log:
				str << UTF8("log(") << inputs[0] << UTF8(")");
				break;
			case ent_exp2:
				str << UTF8("exp2(") << inputs[0] << UTF8(")");
				break;
			case ent_log2:
				str << UTF8("log2(") << inputs[0] << UTF8(")");
				break;
			case ent_sqrt:
				str << UTF8("sqrt(") << inputs[0] << UTF8(")");
				break;
			case ent_inversesqrt:
				str << UTF8("inversesqrt(") << inputs[0];
				str << UTF8(")");
				break;
			case ent_abs:
				str << UTF8("abs(") << inputs[0] << UTF8(")");
				break;
			case ent_sign:
				str << UTF8("sign(") << inputs[0] << UTF8(")");
				break;
			case ent_round:
				str << UTF8("round(") << inputs[0] << UTF8(")");
				break;
			case ent_trunc:
				str << UTF8("trunc(") << inputs[0] << UTF8(")");
				break;
			case ent_floor:
				str << UTF8("floor(") << inputs[0] << UTF8(")");
				break;
			case ent_ceil:
				str << UTF8("ceil(") << inputs[0] << UTF8(")");
				break;
			case ent_fract:
				str << UTF8("fract(") << inputs[0] << UTF8(")");
				break;
			case ent_normalize:
				str << UTF8("normalize(") << inputs[0];
				str << UTF8(")");
				break;
			case ent_saturate:
				str << UTF8("saturate(") << inputs[0];
				str << UTF8(", 0.0, 1.0)");
				break;
			case ent_refract:
				str << UTF8("refract(") << inputs[0];
				str << UTF8(", ") << inputs[1] << UTF8(", ");
				str << inputs[2] << UTF8(")");
				break;
			case ent_dFdx:
				str << UTF8("dFdx(") << inputs[0];
				str << UTF8(")");
				break;
			case ent_dFdy:
				str << UTF8("dFdy(") << inputs[0];
				str << UTF8(")");
				break;
			case ent_fresnel:
				str << UTF8("dot(") << inputs[0] << UTF8(", ");
				str << inputs[1] << UTF8(")");
				break;
		}

		str << UTF8(";\n");
	}

}
