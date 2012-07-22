/****************************************************************************
 *            effecttexture.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "effecttexture.hpp"
#include "effectnodeport.hpp"
#include "../shader/commonparameterutil.hpp"

namespace eternal_lands
{

	EffectTexture::EffectTexture(const String &name, const Uint32 id,
		const EffectSamplerType sampler,
		const EffectTextureType texture, const Uint16 texture_unit):
		EffectNode(name, id), m_sampler(sampler), m_texture(texture),
		m_texture_unit(texture_unit)
	{
		Uint32 index;

		index = 0;

		BOOST_FOREACH(String &var, m_vars)
		{
			var = get_var_name(index);
			index++;
		}

		switch (get_texture())
		{
			case ett_default:
				add_output_port(m_vars[0], String(UTF8("r")),
					ect_fragment);
				add_output_port(m_vars[0], String(UTF8("g")),
					ect_fragment);
				add_output_port(m_vars[0], String(UTF8("b")),
					ect_fragment);
				add_output_port(m_vars[0], String(UTF8("a")),
					ect_fragment);
				break;
			case ett_albedo:
				add_output_port(m_vars[0], String(UTF8("rgba")),
					ect_fragment);
				add_output_port(m_vars[0], String(UTF8("rgb")),
					ect_fragment);
				add_output_port(m_vars[0], String(UTF8("a")),
					ect_fragment);
				break;
			case ett_normal:
				add_output_port(m_vars[1], String(UTF8("xyz")),
					ect_fragment);
				break;
			case ett_parallax:
				add_input_port(String(UTF8("parallax")),
					String(UTF8("?")), ect_fragment);
				add_output_port(m_vars[0], String(UTF8("uv")),
					ect_fragment);
				add_output_port(m_vars[1],
					String(UTF8("normal")),
					String(UTF8("xyz")), ect_fragment);
				add_output_port(m_vars[2],
					String(UTF8("extra")),
					String(UTF8("?")),
					ect_fragment);
				break;
		}

		switch (get_sampler())
		{
			case est_sampler_1d:
				add_input_port(String(UTF8("u")), ect_fragment);
				add_input_port(String("dPdx"),
					String(UTF8("x")), ect_fragment);
				add_input_port(String("dPdy"),
					String(UTF8("x")), ect_fragment);
				break;
			case est_sampler_2d:
				add_input_port(String(UTF8("uv")),
					ect_fragment);
				add_input_port(String("dPdx"),
					String(UTF8("xy")), ect_fragment);
				add_input_port(String("dPdy"),
					String(UTF8("xy")), ect_fragment);
				break;
			case est_sampler_3d:
				add_input_port(String(UTF8("xyz")),
					ect_fragment);
				add_input_port(String("dPdx"),
					String(UTF8("xyz")), ect_fragment);
				add_input_port(String("dPdy"),
					String(UTF8("xyz")), ect_fragment);
				break;
			case est_sampler_cube_map:
				add_input_port(String(UTF8("xyz")),
					ect_fragment);
				add_input_port(String("dPdx"),
					String(UTF8("xyz")), ect_fragment);
				add_input_port(String("dPdy"),
					String(UTF8("xyz")), ect_fragment);
				break;
			case est_sampler_1d_array:
				add_input_port(String(UTF8("u")),
					ect_fragment);
				add_input_port(String("layer"),
					String(UTF8("?")), ect_fragment);
				add_input_port(String("dPdx"),
					String(UTF8("x")), ect_fragment);
				add_input_port(String("dPdy"),
					String(UTF8("x")), ect_fragment);
				break;
			case est_sampler_2d_array:
				add_input_port(String(UTF8("uv")),
					ect_fragment);
				add_input_port(String("layer"),
					String(UTF8("?")), ect_fragment);
				add_input_port(String("dPdx"),
					String(UTF8("xy")), ect_fragment);
				add_input_port(String("dPdy"),
					String(UTF8("xy")), ect_fragment);
				break;
			case est_sampler_cube_map_array:
				add_input_port(String(UTF8("xyz")),
					ect_fragment);
				add_input_port(String("layer"),
					String(UTF8("?")), ect_fragment);
				add_input_port(String("dPdx"),
					String(UTF8("xyz")), ect_fragment);
				add_input_port(String("dPdy"),
					String(UTF8("xyz")), ect_fragment);
				break;
			case est_sampler_rectangle:
				add_input_port(String(UTF8("uv")),
					ect_fragment);
				add_input_port(String("dPdx"),
					String(UTF8("xy")), ect_fragment);
				add_input_port(String("dPdy"),
					String(UTF8("xy")), ect_fragment);
				break;
			case est_sampler_1d_project:
				add_input_port(String(UTF8("xyzw")),
					ect_fragment);
				add_input_port(String("dPdx"),
					String(UTF8("x")), ect_fragment);
				add_input_port(String("dPdy"),
					String(UTF8("x")), ect_fragment);
				break;
			case est_sampler_2d_project:
				add_input_port(String(UTF8("xyzw")),
					ect_fragment);
				add_input_port(String("dPdx"),
					String(UTF8("xy")), ect_fragment);
				add_input_port(String("dPdy"),
					String(UTF8("xy")), ect_fragment);
				break;
			case est_sampler_3d_project:
				add_input_port(String(UTF8("xyzw")),
					ect_fragment);
				add_input_port(String("dPdx"),
					String(UTF8("xyz")), ect_fragment);
				add_input_port(String("dPdy"),
					String(UTF8("xyz")), ect_fragment);
				break;
			case est_sampler_rectangle_project:
				add_input_port(String(UTF8("xyzw")),
					ect_fragment);
				add_input_port(String("dPdx"),
					String(UTF8("xy")), ect_fragment);
				add_input_port(String("dPdy"),
					String(UTF8("xy")), ect_fragment);
				break;
		}
	}

	EffectTexture::~EffectTexture() noexcept
	{
	}

	Uint16 EffectTexture::get_initial_value_count() const
	{
		return 4;
	}

	void EffectTexture::write(const Uint16StringMap &array_layers,
		 const ShaderVersionType version, const bool low_quality,
		const EffectChangeType change,
		StringUint16Map &parameters,
		ShaderSourceParameterVector &vertex_parameters,
		ShaderSourceParameterVector &fragment_parameters,
		OutStream &vertex_str, OutStream &fragment_str,
		EffectNodePtrSet &vertex_written,
		EffectNodePtrSet &fragment_written)
	{
		BoostFormat normal_format(UTF8(
			"vec2 %1% = %3%.xy * 2.0 - 1.0;\n"
			"vec3 %2% = vec3(%1%, sqrt(1.0 - dot(%1%, %1%)));"));
		BoostFormat parallax_format(UTF8(
			"vec4 %9%;\n"
			"vec3 %8%, %10%;\n"
			"float %11%, %12%;\n"
			"int %13%;\n"
			"\n"
			"%10% = %6%.xyz * %5%;\n"
			"%8% = vec3(%4%, 0.0);\n"
			"\n"
			"for (%13% = 0; %13% < 3; %13%++)\n"
			"{\n"
			"\t%9% = %3%;\n"
			"\t%9%.xy = %9%.xy * 2.0 - 1.0;\n"
			"\t%12% = sqrt(1.0 - dot(%9%.xy, %9%.xy));\n"
			"\t%11% = %9%.a * %7% - %7% * 0.5;\n"
			"\t%8% += (%11% - %8%.z) * %12% * %10%;\n"
			"}\n"
			"\n"
			"%1% = %8%.xy;\n"
			"%2% = %5% * vec3(%9%.xy, %12%);\n"));
		StringStream str;
		Uint16StringMap::const_iterator found;
		String world_uv, uv, scale, dPdx, dPdy, layer;
		bool use_grad;

		if (fragment_written.count(this) > 0)
		{
			return;
		}

		fragment_written.insert(this);

		found = array_layers.find(get_texture_unit());

		BOOST_FOREACH(EffectNodePort &port, get_ports())
		{
			if (port.get_input())
			{
				if (port.get_name() == UTF8("dPdx"))
				{
					dPdx = port.
						get_connected_var_swizzled();
					port.write(array_layers, version,
						low_quality, ect_fragment,
						parameters, vertex_parameters,
						fragment_parameters,
						vertex_str, fragment_str,
						vertex_written,
						fragment_written);

					continue;
				}

				if (port.get_name() == UTF8("dPdy"))
				{
					dPdy = port.
						get_connected_var_swizzled();
					port.write(array_layers, version,
						low_quality, ect_fragment,
						parameters, vertex_parameters,
						fragment_parameters,
						vertex_str, fragment_str,
						vertex_written,
						fragment_written);

					continue;
				}

				if (port.get_name() == UTF8("layer"))
				{
					layer = port.
						get_connected_var_swizzled();
					port.write(array_layers, version,
						low_quality, ect_fragment,
						parameters, vertex_parameters,
						fragment_parameters,
						vertex_str, fragment_str,
						vertex_written,
						fragment_written);

					continue;
				}

				if (port.get_name() == UTF8("parallax"))
				{
					scale = port.
						get_connected_var_swizzled();
					port.write(array_layers, version,
						low_quality, ect_fragment,
						parameters, vertex_parameters,
						fragment_parameters,
						vertex_str, fragment_str,
						vertex_written,
						fragment_written);

					continue;
				}
				/* Can only be the port used as input for
				 * the texture coordinates.
				 */
				uv = port.get_connected_var_swizzled();
				port.write(array_layers, version, low_quality,
					ect_fragment, parameters,
					vertex_parameters, fragment_parameters,
					vertex_str, fragment_str,
					vertex_written, fragment_written);
			}
		}

		world_uv = uv;

		if (get_texture() == ett_parallax)
		{
			uv = m_vars[2];
		}

		use_grad = (version > svt_120) && !dPdx.get().empty() &&
			!dPdy.get().empty();

		str << UTF8("texture");

		switch (get_sampler())
		{
			case est_sampler_1d:
				if (version == svt_120)
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

				str << UTF8("(") << get_sampler_name();
				str << UTF8(", ");

				if (found != array_layers.end())
				{
					str << UTF8("vec2(") << uv;
					str << UTF8(", ") << found->second;
					str << UTF8(")");
				}
				else
				{
					str << uv;
				}

				if (use_grad)
				{
					str << UTF8(", ") << dPdx;
					str << UTF8(", ") << dPdy;
				}
				break;
			case est_sampler_2d:
				if (version == svt_120)
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

				str << UTF8("(") << get_sampler_name();
				str << UTF8(", ");

				if (found != array_layers.end())
				{
					str << UTF8("vec3(") << uv;
					str << UTF8(", ") << found->second;
					str << UTF8(")");
				}
				else
				{
					str << uv;
				}

				if (use_grad)
				{
					str << UTF8(", ") << dPdx;
					str << UTF8(", ") << dPdy;
				}
				break;
			case est_sampler_3d:
				if (version == svt_120)
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

				str << UTF8("(") << get_sampler_name();
				str << UTF8(", ") << uv;

				if (use_grad)
				{
					str << UTF8(", ") << dPdx;
					str << UTF8(", ") << dPdy;
				}
				break;
			case est_sampler_cube_map:
				if (version == svt_120)
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

				str << UTF8("(") << get_sampler_name();
				str << UTF8(", ") << uv;

				if (use_grad)
				{
					str << UTF8(", ") << dPdx;
					str << UTF8(", ") << dPdy;
				}
				break;
			case est_sampler_1d_array:
				if (use_grad)
				{
					str << UTF8("Grad");
				}

				str << UTF8("(") << get_sampler_name();
				str << UTF8(", vec2(") << uv;
				str << UTF8(",") << layer << UTF8(")");

				if (use_grad)
				{
					str << UTF8(", ") << dPdx;
					str << UTF8(", ") << dPdy;
				}
				break;
			case est_sampler_2d_array:
				if (use_grad)
				{
					str << UTF8("Grad");
				}

				str << UTF8("(") << get_sampler_name();
				str << UTF8(", vec3(") << uv;
				str << UTF8(", ") << layer << UTF8(")");

				if (use_grad)
				{
					str << UTF8(", ") << dPdx;
					str << UTF8(", ") << dPdy;
				}
				break;
			case est_sampler_cube_map_array:
				if (use_grad)
				{
					str << UTF8("Grad");
				}

				str << UTF8("(") << get_sampler_name();
				str << UTF8(", vec4(") << uv;
				str << UTF8(", ") << layer << UTF8(")");

				if (use_grad)
				{
					str << UTF8(", ") << dPdx;
					str << UTF8(", ") << dPdy;
				}
				break;
			case est_sampler_rectangle:
				if (use_grad)
				{
					str << UTF8("Grad");
				}

				str << UTF8("(") << get_sampler_name();
				str << UTF8(", ") << uv;

				if (use_grad)
				{
					str << UTF8(", ") << dPdx;
					str << UTF8(", ") << dPdy;
				}
				break;
			case est_sampler_1d_project:
				if (version == svt_120)
				{
					str << UTF8("1D");
				}

				str << UTF8("Proj");

				if (use_grad)
				{
					str << UTF8("Grad");
				}

				str << UTF8("(") << get_sampler_name();
				str << UTF8(", ") << uv;

				if (use_grad)
				{
					str << UTF8(", ") << dPdx;
					str << UTF8(", ") << dPdy;
				}
				break;
			case est_sampler_2d_project:
				if (version == svt_120)
				{
					str << UTF8("2D");
				}

				str << UTF8("Proj");

				if (use_grad)
				{
					str << UTF8("Grad");
				}

				str << UTF8("(") << get_sampler_name();
				str << UTF8(", ") << uv;

				if (use_grad)
				{
					str << UTF8(", ") << dPdx;
					str << UTF8(", ") << dPdy;
				}
				break;
			case est_sampler_3d_project:
				if (version == svt_120)
				{
					str << UTF8("3D");
				}

				str << UTF8("Proj");

				if (use_grad)
				{
					str << UTF8("Grad");
				}

				str << UTF8("(") << get_sampler_name();
				str << UTF8(", ") << uv;

				if (use_grad)
				{
					str << UTF8(", ") << dPdx;
					str << UTF8(", ") << dPdy;
				}
				break;
			case est_sampler_rectangle_project:
				str << UTF8("Proj");

				if (use_grad)
				{
					str << UTF8("Grad");
				}

				str << UTF8("(") << get_sampler_name();
				str << UTF8(", ") << uv;

				if (use_grad)
				{
					str << UTF8(", ") << dPdx;
					str << UTF8(", ") << dPdy;
				}
				break;
		}

		str << UTF8(")");

		switch (get_texture())
		{
			case ett_default:
			case ett_albedo:
				fragment_str << UTF8("vec4 ") << m_vars[0];
				fragment_str << UTF8(" = ") << str.str();
				fragment_str << UTF8(";\n");
				break;
			case ett_normal:
				normal_format % m_vars[0] % m_vars[1];
				normal_format % str.str();

				fragment_str << normal_format.str();
				break;
			case ett_parallax:
				parallax_format % m_vars[0] % m_vars[1];
				parallax_format % str.str();
				parallax_format % world_uv % cpt_tbn_matrix;
				parallax_format % cpt_world_view_direction;
				parallax_format % scale % uv;
				parallax_format % m_vars[3] % m_vars[4];
				parallax_format % m_vars[5] % m_vars[6];
				parallax_format % m_vars[7];
				fragment_str << parallax_format.str();
				break;
		}

	}

	String EffectTexture::get_description() const
	{
		return EffectTextureUtil::get_description(get_texture());
	}

}
