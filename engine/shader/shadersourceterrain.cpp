/****************************************************************************
 *            shadersourceterrain.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "shadersourceterrain.hpp"
#include "commonparameterutil.hpp"
#include "shadersourceparameter.hpp"
#include "shadersourceparameterbuilder.hpp"
#include "xmlwriter.hpp"
#include "xmlreader.hpp"
#include "xmlutil.hpp"

namespace eternal_lands
{

	namespace
	{

		boost::array<SamplerParameterType, 16> samplers =
			boost::assign::list_of(spt_effect_0)(spt_effect_1)
			(spt_effect_2)(spt_effect_3)(spt_effect_4)
			(spt_effect_5)(spt_effect_6)(spt_effect_7)
			(spt_effect_8)(spt_effect_9)(spt_effect_10)
			(spt_effect_11)(spt_effect_12)(spt_effect_13)
			(spt_effect_14)(spt_effect_15);

	}

	ShaderSourceTerrain::ShaderSourceTerrain()
	{
	}

	ShaderSourceTerrain::~ShaderSourceTerrain() noexcept
	{
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

		clear();

		do
		{
			if (xmlStrcmp(it->name, BAD_CAST UTF8("name")) == 0)
			{
				set_name(XmlUtil::get_string_value(it));
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("use_extra_maps"))
				== 0)
			{
				set_use_extra_maps(XmlUtil::get_bitset64_value(
					it));
			}

			if (xmlStrcmp(it->name,
				BAD_CAST UTF8("use_blend_size_samplers")) == 0)
			{
				set_use_blend_size_samplers(
					XmlUtil::get_bitset64_value(it));
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("blend_data"))
				== 0)
			{
				append_blend_data(BlendData(it));
			}
		}
		while (XmlUtil::next(it, true));
	}

	void ShaderSourceTerrain::do_save_xml(const XmlWriterSharedPtr &writer)
		const
	{
		writer->start_element(get_xml_id());
		writer->write_element(String(UTF8("name")), get_name());
		writer->write_bitset64_element(String(UTF8("use_extra_maps")),
			get_use_extra_maps());
		writer->write_bitset64_element(
			String(UTF8("use_blend_size_samplers")),
			get_use_blend_size_samplers());

		BOOST_FOREACH(const BlendData &blend_data, get_blend_datas())
		{
			blend_data.save_xml(writer);
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
		bool use_glsl_130;

		source = get_name();

		use_glsl_130 = version >= svt_130;

		if (use_glsl_130)
		{
			sampler = pt_sampler2DArray;
		}
		else
		{
			sampler = pt_sampler2D;
		}

		ShaderSourceParameterBuilder::add_parameter(source,
			cpt_world_uv, pqt_in, result);
		ShaderSourceParameterBuilder::add_parameter(source,
			cpt_world_extra_uv, pqt_in, result);
		ShaderSourceParameterBuilder::add_parameter(source,
			cpt_output_data_float, pqt_out, result);

		if (use_glsl_130)
		{
			if (get_use_any_extra_map())
			{
				ShaderSourceParameterBuilder::add_parameter(
					source, get_extra_sampler(),
					sampler, result);
			}

			if (get_use_any_none_blend_size_sampler())
			{
				ShaderSourceParameterBuilder::add_parameter(
					source, get_albedo_sampler(0), sampler,
					result);
			}

			if (get_use_any_blend_size_sampler())
			{
				ShaderSourceParameterBuilder::add_parameter(
					source, get_albedo_sampler(1), sampler,
					result);
			}

			ShaderSourceParameterBuilder::add_parameter(source,
				get_blend_sampler(0), sampler, result);

			return result;
		}

		count = std::min((get_blend_datas_size() + 3) / 4,
			get_non_array_blend_sampler_count());

		for (i = 0; i < count; ++i)
		{
			ShaderSourceParameterBuilder::add_parameter(source,
				get_blend_sampler(i), sampler, result);
		}

		count = std::min(get_blend_datas_size() + 1,
			get_non_array_albedo_sampler_count());

		for (i = 0; i < count; ++i)
		{
			ShaderSourceParameterBuilder::add_parameter(source,
				get_albedo_sampler(i), sampler, result);
		}

		return result;
	}

	void ShaderSourceTerrain::write_blend_fetch(const String &indent,
		const Uint16 index, const bool use_glsl_130, OutStream &str)
		const
	{
		if (!use_glsl_130)
		{
			str << indent << UTF8("blends[") << index;
			str << UTF8("] = texture2D(");
			str << get_blend_sampler(index) << UTF8(", ");
			str << cpt_world_uv << UTF8(");\n");

			return;
		}

#ifdef	USE_COMPRESSED_BLEND
		str << indent << UTF8("blends[") << index << UTF8("].rg = ");
		str << UTF8("texture(") << get_blend_sampler(0) << UTF8(", ");
		str << UTF8("vec3(") << cpt_world_uv << UTF8(", ");
		str << (index * 2) << UTF8(")).rg;\n");

		str << indent << UTF8("blends[") << index << UTF8("].ba = ");
		str << UTF8("texture(") << get_blend_sampler(0) << UTF8(", ");
		str << UTF8("vec3(") << cpt_world_uv << UTF8(", ");
		str << (index * 2 + 1) << UTF8(")).rg;\n");
#else	/* USE_COMPRESSED_BLEND */
		str << indent << UTF8("blends[") << index;
		str << UTF8("] = texture(") << get_blend_sampler(0);
		str << UTF8(", vec3(") << cpt_world_uv << UTF8(", ");
		str << index << UTF8("));\n");
#endif	/* USE_COMPRESSED_BLEND */
	}

	void ShaderSourceTerrain::write_albedo_fetch(const String &indent,
		const Uint16 index, const bool use_glsl_130, OutStream &str) const
	{
		Uint16 i, sampler, layer;
		bool use_blend_size_sampler;

		str << indent << UTF8("albedo = texture");

		layer = 0;
		sampler = 0;

		use_blend_size_sampler = get_use_blend_size_sampler(index);

		if (use_blend_size_sampler)
		{
			sampler = 1;
		}

		for (i = 0; i < index; ++i)
		{
			if (get_use_blend_size_sampler(i) ==
				use_blend_size_sampler)
			{
				layer++;
			}
		}

		if (use_glsl_130)
		{
			str << UTF8("(") << get_albedo_sampler(sampler);
			str << UTF8(", vec3(") << cpt_world_extra_uv;
			str << UTF8(", ") << layer << UTF8("));\n");
		}
		else
		{
			str << UTF8("2D(") << get_albedo_sampler(index);
			str << UTF8(", ") << cpt_world_extra_uv;
			str << UTF8(");\n");
		}
	}

	void ShaderSourceTerrain::write_extra_fetch(const String &indent,
		const Uint16 index, OutStream &str) const
	{
		str << indent << UTF8("extra = texture(");
		str << get_extra_sampler() << UTF8(", vec3(");
		str << cpt_world_extra_uv << UTF8(", )).rg;\n");
	}

	void ShaderSourceTerrain::write_blend(const String &indent,
		const Uint16 index, OutStream &str) const
	{
		StringStream stream;

		stream << UTF8("blends[" ) << (index / 4) << UTF8("][");
		stream << (index % 4) << UTF8("]");

		str << indent << UTF8("blend = ");

		if (get_blend_data(index).get_use_blend_size())
		{
			str << UTF8("smoothstep(clamp(albedo.a - ");
			str << get_blend_data(index).get_blend_size();
			str << UTF8(", 0.0, 1.0), clamp(albedo.a + ");
			str << get_blend_data(index).get_blend_size();
			str << UTF8(", 0.0, 1.0), ") << stream.str();
			str << UTF8(");\n");
		}
		else
		{
			str << stream.str() << UTF8(";\n");
		}
	}

	void ShaderSourceTerrain::write_mix_result(const String &indent,
		const bool use_extra_map, OutStream &str) const
	{
		str << indent << UTF8("output_data_float[0].rgb = mix(");
		str << UTF8("output_data_float[0].rgb, albedo.rgb, blend);\n");

		if (use_extra_map)
		{
			str << indent << UTF8("output_data_float[0].a = mix(");
			str << UTF8("output_data_float[0].a, extra.r, ");
			str << UTF8("blend);\n");
		}
	}

	String ShaderSourceTerrain::get_source(const ShaderVersionType version)
		const
	{
		StringStream str;
		String indent;
		Uint32 i, index, count;
		bool use_glsl_130;

		use_glsl_130 = version >= svt_130;

		count = get_blend_datas_size();

		count = (count + 3) / 4;

		if (!use_glsl_130)
		{
			count = std::min(count,
				get_non_array_blend_sampler_count());
		}

		if (count > 0)
		{
			str << UTF8("vec4 blends[") << count << UTF8("];\n");
		}

		str << UTF8("vec4 albedo;\n");

		if (get_use_extra_maps().any())
		{
			str << UTF8("vec2 extra;\n");
		}

		str << UTF8("float blend;\n");

		count = get_blend_datas_size();

		count = (count + 3) / 4;

		if (!use_glsl_130)
		{
			count = std::min(count,
				get_non_array_blend_sampler_count());
		}

		for (i = 0; i < count; ++i)
		{
			write_blend_fetch(indent, i, use_glsl_130, str);
		}

		str << indent << UTF8("output_data_float[0] = vec4(1.0);\n");

		write_albedo_fetch(indent, 0, use_glsl_130, str);
		str << indent << UTF8("output_data_float[0].rgb = ");
		str << UTF8("albedo.rgb;\n");

		index = 0;

		if (get_use_extra_map(0) && use_glsl_130)
		{
			write_extra_fetch(indent, index, str);
			str << indent << UTF8("output_data_float[0].a = ");
			str << indent << UTF8("extra.r;\n");

			index++;
		}

		count = get_blend_datas_size();

		if (!use_glsl_130)
		{
			count = std::min(count,
				get_non_array_albedo_sampler_count() - 1);
		}

		for (i = 0; i < count; ++i)
		{
			write_albedo_fetch(indent, i + 1, use_glsl_130, str);

			if (get_use_extra_map(i + 1) && use_glsl_130)
			{
				write_extra_fetch(indent, index, str);

				index++;
			}

			write_blend(indent, i, str);
			write_mix_result(indent, get_use_extra_map(i + 1), str);
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

	Uint32 ShaderSourceTerrain::get_non_array_albedo_sampler_count()
	{
		return 13;
	}

	Uint32 ShaderSourceTerrain::get_non_array_blend_sampler_count()
	{
		return 3;
	}

	SamplerParameterType ShaderSourceTerrain::get_albedo_sampler(
		const Uint16 index)
	{
		RANGE_CECK_MAX(index, get_non_array_albedo_sampler_count(),
			UTF8("albedo sampler index too big"));

		return samplers[index];
	}

	SamplerParameterType ShaderSourceTerrain::get_extra_sampler()
	{
		return samplers[2];
	}

	SamplerParameterType ShaderSourceTerrain::get_blend_sampler(
		const Uint16 index)
	{
		RANGE_CECK_MAX(index, get_non_array_blend_sampler_count(),
			UTF8("albedo sampler index too big"));

		return samplers[index + get_non_array_albedo_sampler_count()];
	}

}
