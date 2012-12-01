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

		m_material_data.clear();

		do
		{
			if (xmlStrcmp(it->name, BAD_CAST UTF8("name")) == 0)
			{
				set_name(XmlUtil::get_string_value(it));
			}

			if (xmlStrcmp(it->name,
				BAD_CAST UTF8("terrain_material_data")) == 0)
			{
				m_material_data.load_xml(it);
			}
		}
		while (XmlUtil::next(it, true));

		assert(m_material_data.get_layer_count() > 0);
	}

	void ShaderSourceTerrain::do_save_xml(const XmlWriterSharedPtr &writer)
		const
	{
		writer->start_element(get_xml_id());
		writer->write_element(String(UTF8("name")), get_name());

		m_material_data.save_xml(writer);

		writer->end_element();
	}

	ShaderSourceParameterVector ShaderSourceTerrain::get_parameters(
		const ShaderType shader_type, const ShaderVersionType version)
		const
	{
		ShaderSourceParameterVector result;
		ParameterType sampler;
		String source;
		Uint32 i, count;
		bool use_glsl_130;

		assert(m_material_data.get_layer_count() > 0);

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
			if (m_material_data.get_use_any_height_map() &&
				m_material_data.get_write_height())
			{
				ShaderSourceParameterBuilder::add_parameter(
					source, get_height_sampler(),
					sampler, result);
			}

			if (m_material_data.get_use_any_specular_map() &&
				m_material_data.get_write_specular_gloss())
			{
				ShaderSourceParameterBuilder::add_parameter(
					source, get_specular_sampler(),
					sampler, result);
			}

			if (m_material_data.get_use_any_gloss_map() &&
				m_material_data.get_write_specular_gloss())
			{
				ShaderSourceParameterBuilder::add_parameter(
					source, get_gloss_sampler(),
					sampler, result);
			}

			if (m_material_data.get_use_any_none_blend_size_texture(
				))
			{
				ShaderSourceParameterBuilder::add_parameter(
					source, get_albedo_sampler(0), sampler,
					result);
			}

			if (m_material_data.get_use_any_blend_size_texture())
			{
				ShaderSourceParameterBuilder::add_parameter(
					source, get_albedo_sampler(1), sampler,
					result);
			}

			ShaderSourceParameterBuilder::add_parameter(source,
				get_blend_sampler(0), sampler, result);

			return result;
		}

		count = std::min(
			(m_material_data.get_layer_count() + 2) / 4,
			get_non_array_blend_sampler_count());

		for (i = 0; i < count; ++i)
		{
			ShaderSourceParameterBuilder::add_parameter(source,
				get_blend_sampler(i), sampler, result);
		}

		count = std::min(m_material_data.get_layer_count(),
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

		str << indent << UTF8("blends[") << index;
		str << UTF8("] = texture(") << get_blend_sampler(0);
		str << UTF8(", vec3(") << cpt_world_uv << UTF8(", ");
		str << index << UTF8("));\n");
	}

	void ShaderSourceTerrain::write_albedo_fetch(const String &indent,
		const Uint16 index, const bool use_glsl_130, OutStream &str)
		const
	{
		Uint16 i, layer;
		bool use_blend_size_texture;

		str << indent << UTF8("albedo = texture");

		layer = 0;

		use_blend_size_texture =
			m_material_data.get_use_blend_size_texture(index);

		for (i = 0; i < index; ++i)
		{
			if (m_material_data.get_use_blend_size_texture(i) ==
				use_blend_size_texture)
			{
				layer++;
			}
		}

		if (use_glsl_130)
		{
			str << UTF8("(");
			str << get_albedo_array_sampler(use_blend_size_texture);
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

	void ShaderSourceTerrain::write_height_fetch(const String &indent,
		const Uint16 index, OutStream &str) const
	{
		str << indent << UTF8("height = texture(");
		str << get_height_sampler() << UTF8(", vec3(");
		str << cpt_world_extra_uv << UTF8(", ") << index;
		str << UTF8(")).r;\n");
	}

	void ShaderSourceTerrain::write_specular_fetch(const String &indent,
		const Uint16 index, OutStream &str) const
	{
		str << indent << UTF8("specular = texture(");
		str << get_specular_sampler() << UTF8(", vec3(");
		str << cpt_world_extra_uv << UTF8(", ") << index;
		str << UTF8(")).rgb;\n");
	}

	void ShaderSourceTerrain::write_gloss_fetch(const String &indent,
		const Uint16 index, OutStream &str) const
	{
		str << indent << UTF8("gloss = texture(");
		str << get_gloss_sampler() << UTF8(", vec3(");
		str << cpt_world_extra_uv << UTF8(", ") << index;
		str << UTF8(")).r;\n");
	}

	void ShaderSourceTerrain::write_height_value(const String &indent,
		const Uint16 index, OutStream &str, Uint16 &texture_index)
		const
	{
		if (m_material_data.get_use_height_map(index))
		{
			write_height_fetch(indent, texture_index, str);

			texture_index++;
		}
		else
		{
			str << indent << UTF8("height = ");
			str << m_material_data.get_default_height(index);
			str << UTF8(";\n");
		}
	}

	void ShaderSourceTerrain::write_specular_value(const String &indent,
		const Uint16 index, OutStream &str, Uint16 &texture_index)
		const
	{
		if (m_material_data.get_use_specular_map(index))
		{
			write_specular_fetch(indent, texture_index, str);

			texture_index++;
		}
		else
		{
			str << indent << UTF8("specular = vec3(");
			str << m_material_data.get_default_specular(index).r;
			str << UTF8(", ");
			str << m_material_data.get_default_specular(index).g;
			str << UTF8(", ");
			str << m_material_data.get_default_specular(index).b;
			str << UTF8(");\n");
		}
	}

	void ShaderSourceTerrain::write_gloss_value(const String &indent,
		const Uint16 index, OutStream &str, Uint16 &texture_index)
		const
	{
		if (m_material_data.get_use_gloss_map(index))
		{
			write_gloss_fetch(indent, texture_index, str);

			texture_index++;
		}
		else
		{
			str << indent << UTF8("gloss = ");
			str << m_material_data.get_default_gloss(index);
			str << UTF8(";\n");
		}
	}

	void ShaderSourceTerrain::write_blend(const String &indent,
		const Uint16 index, OutStream &str) const
	{
		StringStream stream;

		stream << UTF8("blends[" ) << (index / 4) << UTF8("][");
		stream << (index % 4) << UTF8("]");

		str << indent << UTF8("blend = ");

		if (m_material_data.get_use_blend_size_texture(index + 1))
		{
			str << UTF8("smoothstep(clamp(albedo.a - ");
			str << m_material_data.get_blend_size(index + 1);
			str << UTF8(", 0.0, 1.0), clamp(albedo.a + ");
			str << m_material_data.get_blend_size(index + 1);
			str << UTF8(", 0.0, 1.0), ") << stream.str();
			str << UTF8(");\n");
		}
		else
		{
			str << stream.str() << UTF8(";\n");
		}
	}

	void ShaderSourceTerrain::write_mix_result(const String &indent,
		const bool write_height, const bool write_specular_gloss,
		OutStream &str) const
	{
		str << indent << UTF8("data_0.rgb = mix(data_0.rgb, albedo");
		str << UTF8(".rgb, blend);\n");

		if (write_height)
		{
			str << indent << UTF8("data_0.a = mix(data_0.a, ");
			str << UTF8("height, blend);\n");
		}

		if (write_specular_gloss)
		{
			str << indent << UTF8("data_1.rgb = mix(data_1.rgb, ");
			str << UTF8("specular, blend);\n");

			str << indent << UTF8("data_1.a = mix(data_1.a, ");
			str << UTF8("gloss, blend);\n");
		}
	}

	String ShaderSourceTerrain::get_source(const ShaderType shader_type,
		const ShaderVersionType version) const
	{
		StringStream str;
		String indent;
		Uint32 i, count;
		Uint16 height_index, specular_index, gloss_index;
		bool use_glsl_130, write_height, write_specular_gloss;

		use_glsl_130 = version >= svt_130;

		assert(m_material_data.get_layer_count() > 0);

		count = m_material_data.get_layer_count();

		count = (count + 2) / 4;

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
		str << UTF8("vec4 data_0;\n");

		if (m_material_data.get_write_height())
		{
			str << UTF8("float height;\n");
		}

		if (m_material_data.get_write_specular_gloss())
		{
			str << UTF8("vec4 data_1;\n");
			str << UTF8("vec3 specular;\n");
			str << UTF8("float gloss;\n");
		}

		str << UTF8("float blend;\n");

		for (i = 0; i < count; ++i)
		{
			write_blend_fetch(indent, i, use_glsl_130, str);
		}

		write_albedo_fetch(indent, 0, use_glsl_130, str);
		str << indent << UTF8("data_0.rgb = ");
		str << UTF8("albedo.rgb;\n");

		height_index = 0;
		specular_index = 0;
		gloss_index = 0;

		write_height = use_glsl_130 &&
			m_material_data.get_write_height();
		write_specular_gloss = use_glsl_130 &&
			m_material_data.get_write_specular_gloss();

		if (write_height)
		{
			write_height_value(indent, 0, str, height_index);

			str << indent << UTF8("data_0.a = height;\n");
		}
		else
		{
			str << indent << UTF8("data_0.a = 1.0;\n");
		}

		if (write_specular_gloss)
		{
			write_specular_value(indent, 0, str, specular_index);
			write_gloss_value(indent, 0, str, gloss_index);

			str << indent << UTF8("data_1.rgb = specular;\n");
			str << indent << UTF8("data_1.a = gloss;\n");
		}

		count = m_material_data.get_layer_count() - 1;

		if (!use_glsl_130)
		{
			count = std::min(count,
				get_non_array_albedo_sampler_count() - 1);
		}

		for (i = 0; i < count; ++i)
		{
			write_albedo_fetch(indent, i + 1, use_glsl_130, str);

			if (write_height)
			{
				write_height_value(indent, i + 1, str,
					height_index);
			}

			if (write_specular_gloss)
			{
				write_specular_value(indent, i + 1, str,
					specular_index);
				write_gloss_value(indent, i + 1, str,
					gloss_index);
			}

			write_blend(indent, i, str);
			write_mix_result(indent, write_height,
				write_specular_gloss, str);
		}

		str << indent << UTF8("output_data_float[0] = data_0;\n");

		if (write_specular_gloss)
		{
			str << indent << UTF8("output_data_float[1] = data_1");
			str << UTF8(";\n");
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

	bool ShaderSourceTerrain::get_has_data(const ShaderType shader_type,
		const ShaderVersionType version) const
	{
		return true;
	}

	Uint32 ShaderSourceTerrain::get_non_array_albedo_sampler_count()
		noexcept
	{
		return 13;
	}

	Uint32 ShaderSourceTerrain::get_non_array_blend_sampler_count()
		noexcept
	{
		return 3;
	}

	SamplerParameterType ShaderSourceTerrain::get_albedo_sampler(
		const Uint16 index)
	{
		RANGE_CECK_MAX(index, get_non_array_albedo_sampler_count() - 1,
			UTF8("albedo sampler index too big"));

		return samplers[index];
	}

	SamplerParameterType ShaderSourceTerrain::get_albedo_array_sampler(
		const bool use_blend_size_texture) noexcept
	{
		return samplers[use_blend_size_texture ? 1 : 0];
	}

	SamplerParameterType ShaderSourceTerrain::get_height_sampler() noexcept
	{
		return samplers[2];
	}

	SamplerParameterType ShaderSourceTerrain::get_specular_sampler()
		noexcept
	{
		return samplers[3];
	}

	SamplerParameterType ShaderSourceTerrain::get_gloss_sampler() noexcept
	{
		return samplers[4];
	}

	SamplerParameterType ShaderSourceTerrain::get_blend_sampler(
		const Uint16 index)
	{
		RANGE_CECK_MAX(index, get_non_array_blend_sampler_count() - 1,
			UTF8("albedo sampler index too big"));

		return samplers[index + get_non_array_albedo_sampler_count()];
	}

}
