/****************************************************************************
 *            effecttexture.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "effecttexture.hpp"
#include "effectnodeport.hpp"

namespace eternal_lands
{

	EffectTexture::EffectTexture(const String &name,
		const EffectTextureType type,
		Mt19937RandomUuidGenerator &generator): EffectNode(name),
		m_type(type)
	{
		String var, var_rgb, var_a;
		StringStream str;

		set_value_count(4);

		str << UTF8("_") << generator();

		var = String(str.str());

		add_output_port(var, String(UTF8("rgba")), ect_fragment);
		add_output_port(var, String(UTF8("rgb")), ect_fragment);
		add_output_port(var, String(UTF8("a")), ect_fragment);

		switch (get_type())
		{
			case ett_texture_1d:
				add_input_port(String(UTF8("u")));
				add_input_port(String("dPdx"),
					String(UTF8("x")));
				add_input_port(String("dPdy"),
					String(UTF8("x")));
				break;
			case ett_texture_2d:
				add_input_port(String(UTF8("uv")));
				add_input_port(String("dPdx"),
					String(UTF8("xy")));
				add_input_port(String("dPdy"),
					String(UTF8("xy")));
				break;
			case ett_texture_3d:
				add_input_port(String(UTF8("xyz")));
				add_input_port(String("dPdx"),
					String(UTF8("xyz")));
				add_input_port(String("dPdy"),
					String(UTF8("xyz")));
				break;
			case ett_texture_cube_map:
				add_input_port(String(UTF8("xyz")));
				add_input_port(String("dPdx"),
					String(UTF8("xyz")));
				add_input_port(String("dPdy"),
					String(UTF8("xyz")));
				break;
			case ett_texture_1d_array:
				add_input_port(String(UTF8("u")));
				add_input_port(String("layer"),
					String(UTF8("?")));
				add_input_port(String("dPdx"),
					String(UTF8("x")));
				add_input_port(String("dPdy"),
					String(UTF8("x")));
				break;
			case ett_texture_2d_array:
				add_input_port(String(UTF8("uv")));
				add_input_port(String("layer"),
					String(UTF8("?")));
				add_input_port(String("dPdx"),
					String(UTF8("xy")));
				add_input_port(String("dPdy"),
					String(UTF8("xy")));
				break;
			case ett_texture_cube_map_array:
				add_input_port(String(UTF8("xyz")));
				add_input_port(String("layer"),
					String(UTF8("?")));
				add_input_port(String("dPdx"),
					String(UTF8("xyz")));
				add_input_port(String("dPdy"),
					String(UTF8("xyz")));
				break;
			case ett_texture_rectangle:
				add_input_port(String(UTF8("uv")));
				add_input_port(String("dPdx"),
					String(UTF8("xy")));
				add_input_port(String("dPdy"),
					String(UTF8("xy")));
				break;
			case ett_texture_1d_project:
				add_input_port(String(UTF8("xyzw")));
				add_input_port(String("dPdx"),
					String(UTF8("x")));
				add_input_port(String("dPdy"),
					String(UTF8("x")));
				break;
			case ett_texture_2d_project:
				add_input_port(String(UTF8("xyzw")));
				add_input_port(String("dPdx"),
					String(UTF8("xy")));
				add_input_port(String("dPdy"),
					String(UTF8("xy")));
				break;
			case ett_texture_3d_project:
				add_input_port(String(UTF8("xyzw")));
				add_input_port(String("dPdx"),
					String(UTF8("xyz")));
				add_input_port(String("dPdy"),
					String(UTF8("xyz")));
				break;
			case ett_texture_rectangle_project:
				add_input_port(String(UTF8("xyzw")));
				add_input_port(String("dPdx"),
					String(UTF8("xy")));
				add_input_port(String("dPdy"),
					String(UTF8("xy")));
				break;
		}
	}

	EffectTexture::~EffectTexture() noexcept
	{
	}

	void EffectTexture::do_write(const bool glsl_120,
		const EffectChangeType change,
		StringBitSet16Map &parameters_indices,
		ShaderSourceParameterVector &vertex_parameters,
		ShaderSourceParameterVector &fragment_parameters,
		OutStream &vertex_str, OutStream &fragment_str,
		EffectNodePtrSet &written)
	{
		String uv, dPdx, dPdy, layer;
		bool use_grad;

		OutStream &str = fragment_str;

		BOOST_FOREACH(EffectNodePort &port, get_ports())
		{
			if (port.get_input())
			{
				if (port.get_description() == UTF8("dPdx"))
				{
					dPdx = port.
						get_connected_var_swizzled();
					port.write(glsl_120, ect_fragment,
						parameters_indices,
						vertex_parameters,
						fragment_parameters,
						vertex_str, fragment_str,
						written);

					continue;
				}

				if (port.get_description() == UTF8("dPdy"))
				{
					dPdy = port.
						get_connected_var_swizzled();
					port.write(glsl_120, ect_fragment,
						parameters_indices,
						vertex_parameters,
						fragment_parameters,
						vertex_str, fragment_str,
						written);

					continue;
				}

				if (port.get_description() == UTF8("layer"))
				{
					layer = port.
						get_connected_var_swizzled();
					port.write(glsl_120, ect_fragment,
							parameters_indices,
					vertex_parameters,
						fragment_parameters,
						vertex_str, fragment_str,
						written);

					continue;
				}
				/* Can only be the port used as input for
				 * the texture coordinates.
				 */
				uv = port.get_connected_var_swizzled();
				port.write(glsl_120, ect_fragment,
					parameters_indices, vertex_parameters,
					fragment_parameters, vertex_str,
					fragment_str, written);
			}
		}

		use_grad = !glsl_120 && !dPdx.get().empty() &&
			!dPdy.get().empty();

		str << UTF8("vec4") << UTF8(" ") << m_var;
		str << UTF8(" = texture");

		switch (get_type())
		{
			case ett_texture_1d:
				if (glsl_120)
				{
					str << UTF8("1D");
				}
				else
				{
					if (use_grad)
					{
						str << UTF8("Grad");
					}
				}

				str << UTF8("(") << uv;

				if (use_grad)
				{
					str << UTF8(",") << dPdx;
					str << UTF8(",") << dPdy;
				}
				break;
			case ett_texture_2d:
				if (glsl_120)
				{
					str << UTF8("2D");
				}
				else
				{
					if (use_grad)
					{
						str << UTF8("Grad");
					}
				}

				str << UTF8("(") << uv;

				if (use_grad)
				{
					str << UTF8(",") << dPdx;
					str << UTF8(",") << dPdy;
				}
				break;
			case ett_texture_3d:
				if (glsl_120)
				{
					str << UTF8("3D");
				}
				else
				{
					if (use_grad)
					{
						str << UTF8("Grad");
					}
				}

				str << UTF8("(") << uv;

				if (use_grad)
				{
					str << UTF8(",") << dPdx;
					str << UTF8(",") << dPdy;
				}
				break;
			case ett_texture_cube_map:
				if (glsl_120)
				{
					str << UTF8("Cube");
				}
				else
				{
					if (use_grad)
					{
						str << UTF8("Grad");
					}
				}

				str << UTF8("(") << uv;

				if (use_grad)
				{
					str << UTF8(",") << dPdx;
					str << UTF8(",") << dPdy;
				}
				break;
			case ett_texture_1d_array:
				if (use_grad)
				{
					str << UTF8("Grad");
				}

				str << UTF8("(vec2(") << uv;
				str << UTF8(",") << layer << UTF8(")");

				if (use_grad)
				{
					str << UTF8(",") << dPdx;
					str << UTF8(",") << dPdy;
				}
				break;
			case ett_texture_2d_array:
				if (use_grad)
				{
					str << UTF8("Grad");
				}

				str << UTF8("(vec3(") << uv;
				str << UTF8(",") << layer << UTF8(")");

				if (use_grad)
				{
					str << UTF8(",") << dPdx;
					str << UTF8(",") << dPdy;
				}
				break;
			case ett_texture_cube_map_array:
				if (use_grad)
				{
					str << UTF8("Grad");
				}

				str << UTF8("(vec4(") << uv;
				str << UTF8(",") << layer << UTF8(")");

				if (use_grad)
				{
					str << UTF8(",") << dPdx;
					str << UTF8(",") << dPdy;
				}
				break;
			case ett_texture_rectangle:
				if (use_grad)
				{
					str << UTF8("Grad");
				}

				str << UTF8("(") << uv;

				if (use_grad)
				{
					str << UTF8(",") << dPdx;
					str << UTF8(",") << dPdy;
				}
				break;
			case ett_texture_1d_project:
				if (glsl_120)
				{
					str << UTF8("1D");
				}

				str << UTF8("Proj");

				if (use_grad)
				{
					str << UTF8("Grad");
				}

				str << UTF8("(") << uv;

				if (use_grad)
				{
					str << UTF8(",") << dPdx;
					str << UTF8(",") << dPdy;
				}
				break;
			case ett_texture_2d_project:
				if (glsl_120)
				{
					str << UTF8("2D");
				}

				str << UTF8("Proj");

				if (use_grad)
				{
					str << UTF8("Grad");
				}

				str << UTF8("(") << uv;

				if (use_grad)
				{
					str << UTF8(",") << dPdx;
					str << UTF8(",") << dPdy;
				}
				break;
			case ett_texture_3d_project:
				if (glsl_120)
				{
					str << UTF8("3D");
				}

				str << UTF8("Proj");

				if (use_grad)
				{
					str << UTF8("Grad");
				}

				str << UTF8("(") << uv;

				if (use_grad)
				{
					str << UTF8(",") << dPdx;
					str << UTF8(",") << dPdy;
				}
				break;
			case ett_texture_rectangle_project:
				str << UTF8("Proj");

				if (use_grad)
				{
					str << UTF8("Grad");
				}

				str << UTF8("(") << uv;

				if (use_grad)
				{
					str << UTF8(",") << dPdx;
					str << UTF8(",") << dPdy;
				}
				break;
		}

		str << UTF8(");\n");
	}

}
