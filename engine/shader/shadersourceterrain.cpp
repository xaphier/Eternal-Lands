/****************************************************************************
 *            shadersourceterrain.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "shadersourceterrain.hpp"
#include "commonparameterutil.hpp"
#include "samplerparameterutil.hpp"
#include "shaderblenddata.hpp"
#include "xmlwriter.hpp"
#include "xmlreader.hpp"
#include "xmlutil.hpp"

namespace eternal_lands
{

	namespace
	{

		boost::array<SamplerParameterType, 5> diffuse_samplers =
			boost::assign::list_of(spt_effect_0)(spt_effect_1)
			(spt_effect_2)(spt_effect_3)(spt_effect_4);
		boost::array<SamplerParameterType, 5> gloss_samplers =
			boost::assign::list_of(spt_effect_5)(spt_effect_6)
			(spt_effect_7)(spt_effect_8)(spt_effect_9);
		SamplerParameterType blend_sampler = spt_effect_10;

	}

	ShaderSourceTerrain::ShaderSourceTerrain()
	{
	}

	ShaderSourceTerrain::~ShaderSourceTerrain() noexcept
	{
	}

	void ShaderSourceTerrain::load_xml(
		const FileSystemSharedPtr &file_system, const String &file_name)
	{
		XmlReaderSharedPtr xml_reader;

		xml_reader = XmlReaderSharedPtr(new XmlReader(file_system,
			file_name));

		load_xml(xml_reader->get_root_node());
	}

	void ShaderSourceTerrain::load_xml(const String &file_name)
	{
		XmlReaderSharedPtr reader;

		reader = XmlReaderSharedPtr(new XmlReader(file_name));

		load_xml(reader->get_root_node());
	}

	void ShaderSourceTerrain::load_xml(const xmlNodePtr node)
	{
		xmlNodePtr it;

		if (xmlStrcmp(node->name,
			BAD_CAST UTF8("shader_source_terrain")) != 0)
		{
			return;
		}

		it = XmlUtil::children(node, true);

		do
		{
			if (xmlStrcmp(it->name,
				BAD_CAST UTF8("shader_blend_data")) == 0)
			{
				m_data.push_back(ShaderBlendData(it));
			}
		}
		while (XmlUtil::next(it, true));
	}

	void ShaderSourceTerrain::save_xml(const String &file_name) const
	{
		XmlWriterSharedPtr writer;

		writer = XmlWriterSharedPtr(new XmlWriter(file_name));

		save_xml(writer);
	}

	void ShaderSourceTerrain::save_xml(const XmlWriterSharedPtr &writer)
		const
	{
		writer->start_element(String(UTF8("shader_source_terrain")));

		BOOST_FOREACH(const ShaderBlendData &data, m_data)
		{
			data.save_xml(writer);
		}

		writer->end_element();
	}

	void ShaderSourceTerrain::write_blend_fetch(const String &indent,
		const Uint16 index, const bool use_glsl_130, OutStream &str)
	{
		str << indent << UTF8("blends[") << index << UTF8("] = ");
		str << UTF8("texture");

		if (use_glsl_130)
		{
			str << UTF8("(") << blend_sampler;
			str << UTF8(", vec3(") << cpt_world_uv << UTF8(", ");
			str << index << UTF8("));\n");
		}
		else
		{
			str << UTF8("2D(") << blend_sampler;
			str << UTF8(", ") << cpt_world_uv << UTF8(");\n");
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
				str << indent << UTF8("blend = diffuse.a * ");
				str << stream.str() << UTF8(";\n");
				break;
			case sbt_slop:
				str << indent << UTF8("blend = slope * ");
				str << stream.str() << UTF8(";\n");
				break;
			case sbt_height:
				str << indent << UTF8("blend = (height * ");
				str << m_data[index].get_scale_offset().x;
				str << UTF8(" + ");
				str << m_data[index].get_scale_offset().y;
				str << UTF8(") * ") << stream.str();
				str << UTF8(";\n");
				break;
			case sbt_blend:
				break;
			case sbt_size:
				str << indent << UTF8("blend = smoothstep(");
				str << UTF8("clamp(texture.a - ");
				str << m_data[index].get_blend_size();
				str << UTF8(", 0.0, 1.0), clamp(texture.a + ");
				str << m_data[index].get_blend_size();
				str << UTF8(", 0.0, 1.0), ") << stream.str();
				str << UTF8(");\n");
				break;
		};
	}
/*
	void ShaderSourceTerrain::write_mix(const String &indent,
		OutStream &str)
	{
		str << indent << UTF8("result.rgb = mix(result.rgb, ");
		str << UTF8("diffuse.rgb, blend);\n");

		if (get_use_gloss())
		{
			str << indent << UTF8("result.a = mix(result.a, ");
			str << UTF8("gloss, blend);\n");
		}
	}
*/
	ShaderSourceType ShaderSourceTerrain::get_type() const
	{
		return sst_main_effect;
	}

	bool ShaderSourceTerrain::get_has_data(const ShaderVersionType version)
		const
	{
		return true;
	}

}
