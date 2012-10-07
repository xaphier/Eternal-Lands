/****************************************************************************
 *            shadersourceterrain.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "shadersourceterrain.hpp"
#include "commonparameterutil.hpp"
#include "shaderblenddata.hpp"
#include "shadersourceparameter.hpp"
#include "shadersourceparameterbuilder.hpp"
#include "xmlwriter.hpp"
#include "xmlreader.hpp"
#include "xmlutil.hpp"

namespace eternal_lands
{

	namespace
	{

		boost::array<SamplerParameterType, 10> samplers =
			boost::assign::list_of(spt_effect_0)(spt_effect_1)
			(spt_effect_2)(spt_effect_3)(spt_effect_4)
			(spt_effect_5)(spt_effect_6)(spt_effect_7)
			(spt_effect_8)(spt_effect_9);

	}

	ShaderSourceTerrain::ShaderSourceTerrain(): m_use_specular(false)
	{
	}

	ShaderSourceTerrain::~ShaderSourceTerrain() noexcept
	{
	}

	bool ShaderSourceTerrain::get_use_normal_map() const
	{
		BOOST_FOREACH(const ShaderBlendData &blend, m_data)
		{
			if (blend.get_blend() == sbt_slope)
			{
				return true;
			}
		}

		return false;
	}

	bool ShaderSourceTerrain::get_use_displacement_map() const
	{
		BOOST_FOREACH(const ShaderBlendData &blend, m_data)
		{
			if (blend.get_blend() == sbt_height)
			{
				return true;
			}
		}

		return false;
	}

	bool ShaderSourceTerrain::get_use_randomized_uv() const
	{
		return m_use_randomized_uvs.any();
	}

	void ShaderSourceTerrain::do_load_xml(const xmlNodePtr node)
	{
		xmlNodePtr it;

		if (xmlStrcmp(node->name,
			BAD_CAST get_xml_id().get().c_str()) != 0)
		{
			return;
		}

		it = XmlUtil::children(node, true);

		do
		{
			if (xmlStrcmp(it->name, BAD_CAST UTF8("name")) == 0)
			{
				set_name(XmlUtil::get_string_value(it));
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("use_specular"))
				== 0)
			{
				set_use_specular(XmlUtil::get_bool_value(it));
			}

			if (xmlStrcmp(it->name,
				BAD_CAST UTF8("use_randomized_uvs")) == 0)
			{
				m_use_randomized_uvs =
					XmlUtil::get_bitset64_value(it);
			}

			if (xmlStrcmp(it->name,
				BAD_CAST UTF8("shader_blend_data")) == 0)
			{
				m_data.push_back(ShaderBlendData(it));
			}
		}
		while (XmlUtil::next(it, true));
	}

	void ShaderSourceTerrain::do_save_xml(const XmlWriterSharedPtr &writer)
		const
	{
		writer->start_element(get_xml_id());
		writer->write_element(String(UTF8("name")), get_name());
		writer->write_bool_element(String(UTF8("use_specular")),
			get_use_specular());
		writer->write_bitset64_element(
			String(UTF8("use_randomized_uvs")),
			m_use_randomized_uvs);

		BOOST_FOREACH(const ShaderBlendData &data, m_data)
		{
			data.save_xml(writer);
		}

		writer->end_element();
	}

	ShaderSourceParameterVector ShaderSourceTerrain::get_parameters(
		const ShaderVersionType version) const
	{
		ShaderSourceParameterVector result;
		ParameterType sampler;
		String source;
		Uint32 i, count;
		bool use_normal_map, use_displacement_map, use_dudv_map;

		if (version >= svt_130)
		{
			sampler = pt_sampler2DArray;
		}
		else
		{
			sampler = pt_sampler2D;
		}

		use_normal_map = get_use_normal_map();
		use_displacement_map = get_use_displacement_map();
		use_dudv_map = use_normal_map || use_displacement_map;

		ShaderSourceParameterBuilder::add_parameter(source,
			cpt_world_uv, pqt_in, result);
		ShaderSourceParameterBuilder::add_parameter(source,
			cpt_world_extra_uv, pqt_in, result);
		ShaderSourceParameterBuilder::add_parameter(source,
			cpt_output_data_float, pqt_out, result);

		ShaderSourceParameterBuilder::add_parameter(source,
			get_blend_sampler(), sampler, result);

		if (use_dudv_map)
		{
			ShaderSourceParameterBuilder::add_parameter(source,
				get_dudv_sampler(), pt_sampler2D, result);

			ShaderSourceParameterBuilder::add_parameter(source,
				apt_dudv_scale, result);
		}

		if (use_normal_map)
		{
			ShaderSourceParameterBuilder::add_parameter(source,
				get_normal_sampler(), pt_sampler2D, result);
		}

		if (use_displacement_map)
		{
			ShaderSourceParameterBuilder::add_parameter(source,
				get_displacement_sampler(), pt_sampler2D,
				result);
		}

		count = m_data.size();

		if (version >= svt_130)
		{
			count = std::min(count, 1u);
		}
		else
		{
			count = std::min(count + 1,
				get_non_array_sampler_count());
		}

		for (i = 0; i < count; ++i)
		{
			ShaderSourceParameterBuilder::add_parameter(source,
				get_albedo_sampler(i), sampler, result);

			if (get_use_specular())
			{
				ShaderSourceParameterBuilder::add_parameter(
					source, get_specular_sampler(i),
					sampler, result);
			}
		}

		return result;
	}

	void ShaderSourceTerrain::write_blend_fetch(const String &indent,
		const Uint16 index, const bool use_glsl_130, OutStream &str)
		const
	{
		str << indent << UTF8("blends[") << index << UTF8("] = ");
		str << UTF8("texture");

		if (use_glsl_130)
		{
			str << UTF8("(") << get_blend_sampler();
			str << UTF8(", vec3(") << cpt_world_uv;
			str << UTF8(", ") << index << UTF8("));\n");
		}
		else
		{
			str << UTF8("2D(") << get_blend_sampler();
			str << UTF8(", ") << cpt_world_uv << UTF8(");\n");
		}
	}

	void ShaderSourceTerrain::write_albedo_fetch(const String &indent,
		const Uint16 index, const bool use_glsl_130, OutStream &str)
		const
	{
		String uv;

		if (get_use_randomized_uv(index))
		{
			uv = String(UTF8("uv"));
		}
		else
		{
			uv = CommonParameterUtil::get_str(cpt_world_extra_uv);
		}

		str << indent << UTF8("albedo = texture");

		if (use_glsl_130)
		{
			str << UTF8("(") << get_albedo_sampler(0);
			str << UTF8(", vec3(") << uv << UTF8(", ");
			str << index << UTF8("));\n");
		}
		else
		{
			str << UTF8("2D(") << get_albedo_sampler(index);
			str << UTF8(", ") << uv << UTF8(");\n");
		}
	}

	void ShaderSourceTerrain::write_specular_fetch(const String &indent,
		const Uint16 index, const bool use_glsl_130, OutStream &str)
		const
	{
		String uv;

		if (get_use_randomized_uv(index))
		{
			uv = String(UTF8("uv"));
		}
		else
		{
			uv = CommonParameterUtil::get_str(cpt_world_extra_uv);
		}

		str << indent << UTF8("specular = texture");

		if (use_glsl_130)
		{
			str << UTF8("(") << get_specular_sampler(0);
			str << UTF8(", vec3(") << uv << UTF8(", ");
			str << index << UTF8(")).rg;\n");
		}
		else
		{
			str << UTF8("2D(") << get_specular_sampler(index);
			str << UTF8(", ") << uv << UTF8(").rg;\n");
		}
	}

	void ShaderSourceTerrain::write_blend(const String &indent,
		const Uint16 index, OutStream &str) const
	{
		StringStream stream;

		stream << UTF8("blends[" ) << (index / 4) << UTF8("][");
		stream << (index % 4) << UTF8("]");

		switch (m_data[index].get_blend())
		{
			case sbt_texture:
				str << indent << UTF8("blend = albedo.a * ");
				str << stream.str() << UTF8(";\n");
				break;
			case sbt_slope:
				str << indent << UTF8("blend = clamp(slope");
				str << UTF8(" * ");
				str << m_data[index].get_scale_offset().x;
				str << UTF8(" + ");
				str << m_data[index].get_scale_offset().y;
				str << UTF8(", 0.0, 1.0) * ") << stream.str();
				str << UTF8(";\n");
				break;
			case sbt_height:
				str << indent << UTF8("blend = clamp(height");
				str << UTF8(" * ");
				str << m_data[index].get_scale_offset().x;
				str << UTF8(" + ");
				str << m_data[index].get_scale_offset().y;
				str << UTF8(", 0.0, 1.0) * ") << stream.str();
				str << UTF8(";\n");
				break;
			case sbt_blend:
				str << indent << UTF8("blend = ");
				str << stream.str() << UTF8(";\n");
				break;
			case sbt_size:
				str << indent << UTF8("blend = smoothstep(");
				str << UTF8("clamp(albedo.a - ");
				str << m_data[index].get_blend_size();
				str << UTF8(", 0.0, 1.0), clamp(albedo.a + ");
				str << m_data[index].get_blend_size();
				str << UTF8(", 0.0, 1.0), ") << stream.str();
				str << UTF8(");\n");
				break;
		};
	}

	void ShaderSourceTerrain::write_mix_result(const String &indent,
		OutStream &str) const
	{
		str << indent << UTF8("output_data_float[0].rgb = mix(");
		str << UTF8("output_data_float[0].rgb, albedo.rgb, blend);\n");

		if (get_use_specular())
		{
			str << indent << UTF8("output_data_float[0].a = mix(");
			str << UTF8("output_data_float[0].a, specular.r, ");
			str << UTF8("blend);\n");
		}
	}

	String ShaderSourceTerrain::get_source(const ShaderVersionType version)
		const
	{
		StringStream str;
		String indent;
		Uint32 i, count;
		bool use_normal_map, use_displacement_map, use_dudv_map;
		bool use_randomized_uv;

		count = m_data.size();

		if (count == 0)
		{
			return String();
		}

		count = (count + 3) / 4;

		use_normal_map = get_use_normal_map();
		use_displacement_map = get_use_displacement_map();
		use_dudv_map = use_normal_map || use_displacement_map;
		use_randomized_uv = get_use_randomized_uv();

		str << UTF8("vec4 blends[") << count << UTF8("];\n");
		str << UTF8("vec4 albedo;\n");

		if (use_normal_map || use_displacement_map)
		{
			str << UTF8("vec4 tmp;\n");
		}

		if (use_normal_map || use_displacement_map || use_randomized_uv)
		{
			str << UTF8("vec2 uv;\n");
		}

		if (use_randomized_uv)
		{
			if (version >= svt_130)
			{
				str << UTF8("ivec2 indices;\n");
				str << UTF8("int index;\n");
			}
			else
			{
				str << UTF8("vec3 indices;\n");
				str << UTF8("float index;\n");
			}
		}

		if (get_use_specular())
		{
			str << UTF8("vec2 specular;\n");
		}

		if (use_normal_map)
		{
			str << UTF8("float slope;\n");
		}

		if (use_displacement_map)
		{
			str << UTF8("float height;\n");
		}

		str << UTF8("float blend;\n");

		if (use_normal_map || use_displacement_map)
		{
			str << UTF8("uv = ") << cpt_world_uv << UTF8(";\n");
		}
/*
		if (use_dudv_map)
		{
			str << UTF8("uv += texture");

			if (version >= svt_130)
			{
				str << UTF8("(");
			}
			else
			{
				str << UTF8("2D(");
			}

			str << get_dudv_sampler() << UTF8(", uv).xy * ");

			str << apt_dudv_scale << UTF8(";\n");
		}
*/
		if (use_normal_map)
		{
			if (version >= svt_130)
			{
				str << UTF8("tmp = texture(");
			}
			else
			{
				str << UTF8("tmp = texture2D(");
			}

			str << get_normal_sampler() << UTF8(", uv);\n");

			str << UTF8("slope = decode_normal(tmp.xy).z;\n");
		}

		if (use_displacement_map)
		{
			if (version >= svt_130)
			{
				str << UTF8("tmp = texture(");
			}
			else
			{
				str << UTF8("tmp = texture2D(");
			}

			str << get_displacement_sampler() << UTF8(", uv);\n");

			str << UTF8("height = decode_terrain_displacement(");
			str << UTF8("tmp).z;\n");
		}

		count = m_data.size();

		count = (count + 3) / 4;

		for (i = 0; i < count; ++i)
		{
			write_blend_fetch(indent, i, version >= svt_130, str);
		}

		if (use_randomized_uv)
		{
			str << UTF8("uv = ") << cpt_world_extra_uv;
			str << UTF8(";\n");

			if (version >= svt_130)
			{
				str << UTF8("indices = ivec2(uv) * ");
				str << UTF8("ivec2(167, 331);\n");
				str << UTF8("index = indices.x + indices.y");
				str << UTF8(";\n");
				str << UTF8("uv.x *= (index & 0x1) * 2.0 - ");
				str << UTF8("1.0;\n");
				str << UTF8("uv.y *= (index & 0x2) - 1.0;\n");
				str << UTF8("uv = (index & 0x4) != 0 ? uv.yx");
				str << UTF8(" : uv;\n");
			}
			else
			{
				str << UTF8("index = dot(floor(uv), ");
				str << UTF8("vec2(167.0, 331.0));\n");
				str << UTF8("indices = mod(vec3(index), ");
				str << UTF8("vec3(2.0, 4.0, 8.0));\n");
				str << UTF8("indices.y = step(2.0, indices.y)");
				str << UTF8(";\n");
				str << UTF8("uv *= 1.0 - 2.0 * indices.xy;\n");
				str << UTF8("uv = indices.z >= 4.0 ? uv.yx");
				str << UTF8(" : uv;\n");
			}
		}

		str << indent << UTF8("output_data_float[0] = vec4(1.0);\n");

		write_albedo_fetch(indent, 0, version >= svt_130, str);
		str << indent << UTF8("output_data_float[0].rgb = ");
		str << UTF8("albedo.rgb;\n");

		if (get_use_specular())
		{
			write_specular_fetch(indent, 0, version >= svt_130,
				str);
			str << indent << UTF8("output_data_float[0].a = ");
			str << indent << UTF8("specular.r;\n");
		}

		count = m_data.size();

		for (i = 0; i < count; ++i)
		{
			write_albedo_fetch(indent, i + 1, version >= svt_130,
				str);

			if (get_use_specular())
			{
				write_specular_fetch(indent, i + 1,
					version >= svt_130, str);
			}

			write_blend(indent, i, str);
			write_mix_result(indent, str);
		}

		return String(str.str());
	}

	String ShaderSourceTerrain::get_xml_id()
	{
		return String(UTF8("shader_source_terrain"));
	}

	ShaderSourceType ShaderSourceTerrain::get_type() const
	{
		return sst_main_effect;
	}

	bool ShaderSourceTerrain::get_has_data(const ShaderVersionType version)
		const
	{
		return true;
	}

	Uint32 ShaderSourceTerrain::get_non_array_sampler_count()
	{
		return 5;
	}

	SamplerParameterType ShaderSourceTerrain::get_albedo_sampler(
		const Uint16 index)
	{
		RANGE_CECK_MAX(index, get_non_array_sampler_count(),
			UTF8("albedo sampler index too big"));

		return samplers[index];
	}

	SamplerParameterType ShaderSourceTerrain::get_specular_sampler(
		const Uint16 index)
	{
		RANGE_CECK_MAX(index, get_non_array_sampler_count(),
			UTF8("specular sampler index too big"));
		
		return samplers[index + get_non_array_sampler_count()];
	}

	SamplerParameterType ShaderSourceTerrain::get_blend_sampler()
	{
		return spt_effect_10;
	}

	SamplerParameterType ShaderSourceTerrain::get_dudv_sampler()
	{
		return spt_effect_11;
	}

	SamplerParameterType ShaderSourceTerrain::get_displacement_sampler()
	{
		return spt_effect_12;
	}

	SamplerParameterType ShaderSourceTerrain::get_normal_sampler()
	{
		return spt_effect_13;
	}

}
