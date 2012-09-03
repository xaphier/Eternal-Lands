/****************************************************************************
 *            effecttexture.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "effecttexture.hpp"
#include "effectnodeport.hpp"
#include "effectconstant.hpp"
#include "../shader/shadersourceparameterbuilder.hpp"
#include "exceptions.hpp"
#include "xmlutil.hpp"
#include "xmlwriter.hpp"

namespace eternal_lands
{

	EffectTexture::EffectTexture(): m_sampler(est_sampler_2d),
		m_texture(ett_default), m_texture_unit(0)
	{
	}

	EffectTexture::EffectTexture(const String &name, const Uint32 id,
		const EffectSamplerType sampler,
		const EffectTextureType texture, const Uint16 texture_unit,
		Mt19937RandomUuidGenerator &uuid_generator):
		EffectNode(name, id, uuid_generator()), m_sampler(sampler),
		m_texture(texture), m_texture_unit(texture_unit)
	{
		Uint32 index;

		RANGE_CECK_MAX(m_texture_unit, material_texture_count,
			UTF8("texture unit value too big"));

		index = 0;

		BOOST_FOREACH(String &var, m_var_names)
		{
			var = get_var_name(index);
			index++;
		}

		switch (get_texture())
		{
			case ett_default:
				add_output_port(m_var_names[0],
					String(UTF8("r")), uuid_generator(),
					ect_fragment);
				add_output_port(m_var_names[0],
					String(UTF8("g")), uuid_generator(),
					ect_fragment);
				add_output_port(m_var_names[0],
					String(UTF8("b")), uuid_generator(),
					ect_fragment);
				add_output_port(m_var_names[0],
					String(UTF8("a")), uuid_generator(),
					ect_fragment);
				break;
			case ett_uv_offset:
				add_output_port(m_var_names[0],
					String(UTF8("rg")), uuid_generator(),
					ect_fragment);
				add_output_port(m_var_names[0],
					String(UTF8("ba")), uuid_generator(),
					ect_fragment);
				break;
			case ett_albedo:
				add_output_port(m_var_names[0],
					String(UTF8("rgba")), uuid_generator(),
					ect_fragment);
				add_output_port(m_var_names[0],
					String(UTF8("rgb")), uuid_generator(),
					ect_fragment);
				add_output_port(m_var_names[0],
					String(UTF8("a")), uuid_generator(),
					ect_fragment);
				break;
			case ett_normal:
				add_output_port(m_var_names[1],
					String(UTF8("xyz")), uuid_generator(),
					ect_fragment);
				add_output_port(m_var_names[0],
					String(UTF8("b")), uuid_generator(),
					ect_fragment);
				add_output_port(m_var_names[0],
					String(UTF8("a")), uuid_generator(),
					ect_fragment);
				break;
			case ett_parallax:
				add_input_port(String(UTF8("parallax")),
					String(UTF8("?")), uuid_generator(),
					ect_fragment);
				add_output_port(m_var_names[0],
					String(UTF8("uv")), uuid_generator(),
					ect_fragment);
				add_output_port(m_var_names[1],
					String(UTF8("normal")),
					String(UTF8("xyz")), uuid_generator(),
					ect_fragment);
				add_output_port(m_var_names[2],
					String(UTF8("extra")),
					String(UTF8("a")), uuid_generator(),
					ect_fragment);
				break;
		}

		switch (get_sampler())
		{
			case est_sampler_1d:
				add_input_port(String(UTF8("u")),
					uuid_generator(), ect_fragment);
				add_input_port(String("dPdx"),
					String(UTF8("x")), uuid_generator(),
					ect_fragment);
				add_input_port(String("dPdy"),
					String(UTF8("x")), uuid_generator(),
					ect_fragment);
				break;
			case est_sampler_2d:
				add_input_port(String(UTF8("uv")),
					uuid_generator(), ect_fragment);
				add_input_port(String("dPdx"),
					String(UTF8("xy")), uuid_generator(),
					ect_fragment);
				add_input_port(String("dPdy"),
					String(UTF8("xy")), uuid_generator(),
					ect_fragment);
				break;
			case est_sampler_3d:
				add_input_port(String(UTF8("xyz")),
					uuid_generator(), ect_fragment);
				add_input_port(String("dPdx"),
					String(UTF8("xyz")), uuid_generator(),
					ect_fragment);
				add_input_port(String("dPdy"),
					String(UTF8("xyz")), uuid_generator(),
					ect_fragment);
				break;
			case est_sampler_cube_map:
				add_input_port(String(UTF8("xyz")),
					uuid_generator(), ect_fragment);
				add_input_port(String("dPdx"),
					String(UTF8("xyz")), uuid_generator(),
					ect_fragment);
				add_input_port(String("dPdy"),
					String(UTF8("xyz")), uuid_generator(),
					ect_fragment);
				break;
			case est_sampler_1d_array:
				add_input_port(String(UTF8("u")),
					uuid_generator(), ect_fragment);
				add_input_port(String("layer"),
					String(UTF8("?")), uuid_generator(),
					ect_fragment);
				add_input_port(String("dPdx"),
					String(UTF8("x")), uuid_generator(),
					ect_fragment);
				add_input_port(String("dPdy"),
					String(UTF8("x")), uuid_generator(),
					ect_fragment);
				break;
			case est_sampler_2d_array:
				add_input_port(String(UTF8("uv")),
					uuid_generator(), ect_fragment);
				add_input_port(String("layer"),
					String(UTF8("?")), uuid_generator(),
					ect_fragment);
				add_input_port(String("dPdx"),
					String(UTF8("xy")), uuid_generator(),
					ect_fragment);
				add_input_port(String("dPdy"),
					String(UTF8("xy")), uuid_generator(),
					ect_fragment);
				break;
			case est_sampler_rectangle:
				add_input_port(String(UTF8("uv")),
					uuid_generator(), ect_fragment);
				add_input_port(String("dPdx"),
					String(UTF8("xy")), uuid_generator(),
					ect_fragment);
				add_input_port(String("dPdy"),
					String(UTF8("xy")), uuid_generator(),
					ect_fragment);
				break;
			case est_sampler_1d_project:
				add_input_port(String(UTF8("xyzw")),
					uuid_generator(), ect_fragment);
				add_input_port(String("dPdx"),
					String(UTF8("x")), uuid_generator(),
					ect_fragment);
				add_input_port(String("dPdy"),
					String(UTF8("x")), uuid_generator(),
					ect_fragment);
				break;
			case est_sampler_2d_project:
				add_input_port(String(UTF8("xyzw")),
					uuid_generator(), ect_fragment);
				add_input_port(String("dPdx"),
					String(UTF8("xy")), uuid_generator(),
					ect_fragment);
				add_input_port(String("dPdy"),
					String(UTF8("xy")), uuid_generator(),
					ect_fragment);
				break;
			case est_sampler_3d_project:
				add_input_port(String(UTF8("xyzw")),
					uuid_generator(), ect_fragment);
				add_input_port(String("dPdx"),
					String(UTF8("xyz")), uuid_generator(),
					ect_fragment);
				add_input_port(String("dPdy"),
					String(UTF8("xyz")), uuid_generator(),
					ect_fragment);
				break;
			case est_sampler_rectangle_project:
				add_input_port(String(UTF8("xyzw")),
					uuid_generator(), ect_fragment);
				add_input_port(String("dPdx"),
					String(UTF8("xy")), uuid_generator(),
					ect_fragment);
				add_input_port(String("dPdy"),
					String(UTF8("xy")), uuid_generator(),
					ect_fragment);
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

	Uint16 EffectTexture::get_layer()
	{
		EffectConstant* constant;
		BitSet16 indices;
		Uint32 i;

		BOOST_FOREACH(EffectNodePort &port, get_ports())
		{
			if (port.get_output())
			{
				continue;
			}

			if (port.get_name() != UTF8("layer"))
			{
				continue;
			}

			if (!port.get_connected())
			{
				continue;
			}

			indices = port.get_connection()->get_var_indices();

			if (indices.count() != 1)
			{
				continue;
			}

			constant = dynamic_cast<EffectConstant*>(
				port.get_connection()->get_node_ptr());		

			if (constant == 0)
			{
				continue;
			}

			for (i = 0; i < 4; ++i)
			{
				if (indices[i])
				{
					return constant->get_value()[i];
				}
			}
		}

		return std::numeric_limits<Uint16>::max();
	}

	void EffectTexture::write(const Uint16StringMap &array_layers,
		const ShaderVersionType version,
		const EffectQualityType quality, const EffectChangeType change,
		StringUint16Map &parameters,
		ShaderSourceParameterVector &vertex_parameters,
		ShaderSourceParameterVector &fragment_parameters,
		OutStream &vertex_str, OutStream &fragment_str,
		EffectNodePtrSet &vertex_written,
		EffectNodePtrSet &fragment_written)
	{
		BoostFormat normal_format(UTF8(
			"vec4 %1% = %4%;\n"
			"vec2 %3% = %1%.xy * 2.0 - 1.0;\n"
			"vec3 %2% = %5% * vec3(%3%, sqrt(1.0 - dot(%3%, "
			"%3%)));\n"));
		BoostFormat no_parallax_format(UTF8(
			"vec4 %4% = %5%;\n"
			"vec2 %1% = %6%;\n"
			"vec2 %3% = %4%.xy * 2.0 - 1.0;\n"
			"vec3 %2% = %7% * vec3(%3%, sqrt(1.0 - dot(%3%, "
			"%3%)));\n"));
		BoostFormat parallax_format(UTF8(
			"vec4 %3%;\n"
			"vec3 %9%, %10%;\n"
			"float %11%, %12%;\n"
			"int %13%;\n"
			"\n"
			"%10% = vec3(%7%.xyz * mat2x3(%6%), -1.0);\n"
			"%9% = vec3(%5%, 0.0);\n"
			"\n"
			"for (%13% = 0; %13% < %14%; %13%++)\n"
			"{\n"
			"\t%3% = %4%;\n"
			"\t%3%.xy = %3%.xy * 2.0 - 1.0;\n"
			"\t%12% = sqrt(1.0 - dot(%3%.xy, %3%.xy));\n"
			"\t%11% = %3%.b * %8% - %8% * 0.5;\n"
			"\t%9% += (%11% - %9%.z) * %12% * %10%;\n"
			"}\n"
			"\n"
			"%1% = %9%.xy;\n"
			"%2% = %6% * vec3(%3%.xy, %12%);\n"));
		StringStream str;
		Uint16StringMap::const_iterator found;
		String world_uv, uv, scale, dPdx, dPdy, layer;
		SamplerParameterType sampler;
		ParameterType type;
		bool use_grad;

		if (fragment_written.count(this) > 0)
		{
			return;
		}

		fragment_written.insert(this);

		sampler = static_cast<SamplerParameterType>(m_texture_unit);

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
						quality, ect_fragment,
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
						quality, ect_fragment,
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
						quality, ect_fragment,
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
						quality, ect_fragment,
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
				port.write(array_layers, version, quality,
					ect_fragment, parameters,
					vertex_parameters, fragment_parameters,
					vertex_str, fragment_str,
					vertex_written, fragment_written);
			}
		}

		world_uv = uv;

		if (get_texture() == ett_parallax)
		{
			uv = m_var_names[3];
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

				str << UTF8("(") << sampler << UTF8(", ");

				if (found != array_layers.end())
				{
					str << UTF8("vec2(") << uv;
					str << UTF8(", ") << found->second;
					str << UTF8(")");

					type = pt_sampler1DArray;
				}
				else
				{
					str << uv;

					type = pt_sampler1D;
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

				str << UTF8("(") << sampler << UTF8(", ");

				if (found != array_layers.end())
				{
					str << UTF8("vec3(") << uv;
					str << UTF8(", ") << found->second;
					str << UTF8(")");

					type = pt_sampler2DArray;
				}
				else
				{
					str << uv;

					type = pt_sampler2D;
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

				str << UTF8("(") << sampler << UTF8(", ") << uv;

				type = pt_sampler3D;

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

				str << UTF8("(") << sampler << UTF8(", ") << uv;

				type = pt_samplerCube;

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

				str << UTF8("(") << sampler << UTF8(", vec2(");
				str << uv << UTF8(",") << layer << UTF8(")");

				type = pt_sampler1DArray;

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

				str << UTF8("(") << sampler << UTF8(", vec3(");
				str << uv << UTF8(", ") << layer << UTF8(")");

				type = pt_sampler2DArray;

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

				str << UTF8("(") << sampler << UTF8(", ") << uv;

				type = pt_sampler2DRect;

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

				str << UTF8("(") << sampler << UTF8(", ") << uv;

				type = pt_sampler1D;

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

				str << UTF8("(") << sampler << UTF8(", ") << uv;

				type = pt_sampler2D;

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

				str << UTF8("(") << sampler << UTF8(", ") << uv;

				type = pt_sampler3D;

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

				str << UTF8("(") << sampler << UTF8(", ") << uv;

				type = pt_sampler2DRect;

				if (use_grad)
				{
					str << UTF8(", ") << dPdx;
					str << UTF8(", ") << dPdy;
				}
				break;
		}

		ShaderSourceParameterBuilder::add_parameter(
			String(UTF8("effect texture")), sampler, type,
			fragment_parameters);

		str << UTF8(")");

		switch (get_texture())
		{
			case ett_default:
			case ett_albedo:
			case ett_uv_offset:
				fragment_str << UTF8("vec4 ") << m_var_names[0];
				fragment_str << UTF8(" = ") << str.str();
				fragment_str << UTF8(";\n");
				break;
			case ett_normal:
				normal_format % m_var_names[0] % m_var_names[1];
				normal_format % m_var_names[2] % str.str();

				fragment_str << normal_format.str();
				break;
			case ett_parallax:
				if (quality == eqt_low)
				{
					no_parallax_format % m_var_names[0];
					no_parallax_format % m_var_names[1];
					no_parallax_format % m_var_names[2];
					no_parallax_format % m_var_names[3];
					no_parallax_format % str.str();
					no_parallax_format % world_uv;
					no_parallax_format % cpt_tbn_matrix;

					fragment_str <<
						no_parallax_format.str();
					break;
				}

				parallax_format % m_var_names[0];
				parallax_format % m_var_names[1];
				parallax_format % m_var_names[2] % str.str();
				parallax_format % world_uv % cpt_tbn_matrix;
				parallax_format % cpt_world_view_direction;
				parallax_format % scale % uv;
				parallax_format % m_var_names[4];
				parallax_format % m_var_names[5];
				parallax_format % m_var_names[6];
				parallax_format % m_var_names[7];

				if (quality == eqt_medium)
				{
					parallax_format % 1;
				}
				else
				{
					parallax_format % 4;
				}

				fragment_str << parallax_format.str();
				break;
		}

	}

	String EffectTexture::get_description() const
	{
		return EffectTextureUtil::get_description(get_texture());
	}

	void EffectTexture::save_xml(const XmlWriterSharedPtr &writer) const
	{
		writer->start_element(String(UTF8("effect_texture")));

		EffectNode::save_xml(writer);

		writer->write_element(String(UTF8("var_name_0")),
			m_var_names[0]);
		writer->write_element(String(UTF8("var_name_1")),
			m_var_names[1]);
		writer->write_element(String(UTF8("var_name_2")),
			m_var_names[2]);
		writer->write_element(String(UTF8("var_name_3")),
			m_var_names[3]);
		writer->write_element(String(UTF8("var_name_4")),
			m_var_names[4]);
		writer->write_element(String(UTF8("var_name_5")),
			m_var_names[5]);
		writer->write_element(String(UTF8("var_name_6")),
			m_var_names[6]);
		writer->write_element(String(UTF8("var_name_7")),
			m_var_names[7]);
		writer->write_element(String(UTF8("sampler")),
			EffectSamplerUtil::get_str(get_sampler()));
		writer->write_element(String(UTF8("texture")),
			EffectTextureUtil::get_str(get_texture()));
		writer->write_int_element(String(UTF8("texture_unit")),
			get_texture_unit());

		writer->end_element();
	}

	void EffectTexture::load_xml(const xmlNodePtr node)
	{
		xmlNodePtr it;

		if (xmlStrcmp(node->name, BAD_CAST UTF8("effect_texture"))
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

			if (xmlStrcmp(it->name, BAD_CAST UTF8("var_name_2"))
				== 0)
			{
				m_var_names[2] = XmlUtil::get_string_value(it);
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("var_name_3"))
				== 0)
			{
				m_var_names[3] = XmlUtil::get_string_value(it);
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("var_name_4"))
				== 0)
			{
				m_var_names[4] = XmlUtil::get_string_value(it);
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("var_name_5"))
				== 0)
			{
				m_var_names[5] = XmlUtil::get_string_value(it);
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("var_name_6"))
				== 0)
			{
				m_var_names[6] = XmlUtil::get_string_value(it);
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("var_name_7"))
				== 0)
			{
				m_var_names[7] = XmlUtil::get_string_value(it);
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("sampler")) == 0)
			{
				m_sampler =
					EffectSamplerUtil::get_effect_sampler(
						XmlUtil::get_string_value(it));
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("texture")) == 0)
			{
				m_texture =
					EffectTextureUtil::get_effect_texture(
						XmlUtil::get_string_value(it));
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("texture_unit"))
				== 0)
			{
				m_texture_unit = XmlUtil::get_uint32_value(it);

				RANGE_CECK_MAX(m_texture_unit,
					material_texture_count,
					UTF8("texture unit value too big"));
			}
		}
		while (XmlUtil::next(it, true));
	}

}
