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
		const EffectSamplerType sampler,
		const EffectTextureType texture, const Uint16 texture_unit,
		Uint32 &var_ids): EffectNode(name), m_sampler(sampler),
		m_texture(texture), m_texture_unit(texture_unit)
	{
		BOOST_FOREACH(String &var, m_vars)
		{
			StringStream str;

			str << UTF8("effect_var_") << std::hex << var_ids;
			var_ids++;

			var = String(str.str());
		}

		switch (get_texture())
		{
			case ett_default:
				add_output_port(m_vars[0], String(UTF8("rgba")),
					ect_fragment);
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
				add_output_port(m_vars[0], String(UTF8("uv")),
					ect_fragment);
				add_output_port(m_vars[1],
					String(UTF8("normal")),
					String(UTF8("xyz")), ect_fragment);
				add_output_port(m_vars[2], String(UTF8("?")),
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
		StringStream str;
		Uint16StringMap::const_iterator found;
		String uv, dPdx, dPdy, layer;
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
				if (port.get_description() == UTF8("dPdx"))
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

				if (port.get_description() == UTF8("dPdy"))
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

				if (port.get_description() == UTF8("layer"))
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
					str << UTF8(",") << found->second;
					str << UTF8(")");
				}
				else
				{
					str << uv;
				}

				if (use_grad)
				{
					str << UTF8(",") << dPdx;
					str << UTF8(",") << dPdy;
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
					str << UTF8(",") << found->second;
					str << UTF8(")");
				}
				else
				{
					str << uv;
				}

				if (use_grad)
				{
					str << UTF8(",") << dPdx;
					str << UTF8(",") << dPdy;
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
					str << UTF8(",") << dPdx;
					str << UTF8(",") << dPdy;
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
					str << UTF8(",") << dPdx;
					str << UTF8(",") << dPdy;
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
					str << UTF8(",") << dPdx;
					str << UTF8(",") << dPdy;
				}
				break;
			case est_sampler_2d_array:
				if (use_grad)
				{
					str << UTF8("Grad");
				}

				str << UTF8("(") << get_sampler_name();
				str << UTF8(", vec3(") << uv;
				str << UTF8(",") << layer << UTF8(")");

				if (use_grad)
				{
					str << UTF8(",") << dPdx;
					str << UTF8(",") << dPdy;
				}
				break;
			case est_sampler_cube_map_array:
				if (use_grad)
				{
					str << UTF8("Grad");
				}

				str << UTF8("(") << get_sampler_name();
				str << UTF8(", vec4(") << uv;
				str << UTF8(",") << layer << UTF8(")");

				if (use_grad)
				{
					str << UTF8(",") << dPdx;
					str << UTF8(",") << dPdy;
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
					str << UTF8(",") << dPdx;
					str << UTF8(",") << dPdy;
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
					str << UTF8(",") << dPdx;
					str << UTF8(",") << dPdy;
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
					str << UTF8(",") << dPdx;
					str << UTF8(",") << dPdy;
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
					str << UTF8(",") << dPdx;
					str << UTF8(",") << dPdy;
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
					str << UTF8(",") << dPdx;
					str << UTF8(",") << dPdy;
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
			case ett_parallax:
				BoostFormat format(UTF8("vec2 %1% = %3%.xy * "
					"2.0 - 1.0;\nvec3 %2% = vec3(%1%, "
					"sqrt(1.0 - dot(%1%, %1%)));"));

				format % m_vars[0] % m_vars[1] % str.str();

				fragment_str << format.str();
				break;
		}

	}

	String EffectTexture::get_sampler_name(const Uint16 texture_unit)
	{
		StringStream str;

		str << UTF8("effect_sampler_") << texture_unit;

		return String(str.str());
	}

}
