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

	EffectConstant::EffectConstant(const String &name,
		const EffectConstantType type,
		Mt19937RandomUuidGenerator &generator): EffectNode(name),
		m_type(type)
	{
		String var_name;
		StringStream str;

		set_value_count(EffectConstantUtil::get_count(get_type()));

		str << generator();

		var_name = String(str.str());

		switch (get_type())
		{
			case ect_float:
				add_output_port(var_name, enpt_single,
					ect_constant);
				break;
			case ect_direction_xy:
				add_output_port(var_name, enpt_xy,
					ect_constant);
				break;
			case ect_color_rgb:
				add_output_port(var_name, enpt_rgb,
					ect_constant);
				break;
			case ect_direction_xyz:
				add_output_port(var_name, enpt_xyz,
					ect_constant);
				break;
			case ect_color_rgba:
				add_output_port(var_name, enpt_rgb,
					ect_constant);
				add_output_port(var_name, enpt_a,
					ect_constant);
				add_output_port(var_name, enpt_rgba,
					ect_constant);
				break;
			case ect_vec2:
				add_output_port(var_name, enpt_x,
					ect_constant);
				add_output_port(var_name, enpt_y,
					ect_constant);
				add_output_port(var_name, enpt_xy,
					ect_constant);
				break;
			case ect_vec3:
				add_output_port(var_name, enpt_x,
					ect_constant);
				add_output_port(var_name, enpt_y,
					ect_constant);
				add_output_port(var_name, enpt_z,
					ect_constant);
				add_output_port(var_name, enpt_xyz,
					ect_constant);
				break;
			case ect_vec4:
				add_output_port(var_name, enpt_x,
					ect_constant);
				add_output_port(var_name, enpt_y,
					ect_constant);
				add_output_port(var_name, enpt_z,
					ect_constant);
				add_output_port(var_name, enpt_w,
					ect_constant);
				add_output_port(var_name, enpt_xyzw,
					ect_constant);
				break;
		}
	}

	EffectConstant::~EffectConstant() noexcept
	{
	}

	void EffectConstant::write(OutStream &str) const
	{
		String name;

		name = get_ports().begin()->get_var_name();

		str << UTF8("constant ");

		switch (get_type())
		{
			case ect_float:
				str << UTF8("float ") << name << UTF8(" = ");
				str << get_value().x;
				break;
			case ect_direction_xy:
			case ect_vec2:
				str << UTF8("vec2 ") << name << UTF8(" = ");
				str << UTF8("vec2(") << get_value().x;
				str << UTF8(", ") << get_value().y;
				str << UTF8(")");
				break;
			case ect_color_rgb:
			case ect_direction_xyz:
			case ect_vec3:
				str << UTF8("vec3 ") << name << UTF8(" = ");
				str << UTF8("vec3(") << get_value().x;
				str << UTF8(", ") << get_value().y;
				str << UTF8(", ") << get_value().z;
				str << UTF8(")");
				break;
			case ect_color_rgba:
			case ect_vec4:
				str << UTF8("vec4 ") << name << UTF8(" = ");
				str << UTF8("vec4(") << get_value().x;
				str << UTF8(", ") << get_value().y;
				str << UTF8(", ") << get_value().z;
				str << UTF8(", ") << get_value().w;
				str << UTF8(")");
				break;
		}

		str << UTF8(";\n");
	}

}
