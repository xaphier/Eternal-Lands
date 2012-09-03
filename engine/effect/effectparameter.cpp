/****************************************************************************
 *            effectparameter.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "effectparameter.hpp"
#include "effectnodeport.hpp"
#include "shader/autoparameterutil.hpp"
#include "shader/commonparameterutil.hpp"
#include "shader/shadersourceparameterbuilder.hpp"
#include "xmlutil.hpp"
#include "xmlwriter.hpp"

namespace eternal_lands
{

	EffectParameter::EffectParameter(): m_type(ept_position)
	{
	}

	EffectParameter::EffectParameter(const String &name, const Uint32 id,
		const EffectParameterType type,
		Mt19937RandomUuidGenerator &uuid_generator):
		EffectNode(name, id, uuid_generator()), m_type(type)
	{
		switch (get_type())
		{
			case ept_position:
				add_output_port(CommonParameterUtil::get_str(
					cpt_world_position),
					String(UTF8("position")),
					String(UTF8("xyz")), uuid_generator(),
					ect_vertex);
				break;
			case ept_normal:
				add_output_port(CommonParameterUtil::get_str(
					cpt_world_normal),
					String(UTF8("normal")),
					String(UTF8("xyz")), uuid_generator(),
					ect_vertex);
				break;
			case ept_tangent:
				add_output_port(CommonParameterUtil::get_str(
					cpt_world_tangent),
					String(UTF8("tangent")),
					String(UTF8("xyz")), uuid_generator(),
					ect_vertex);
				break;
			case ept_view_direction:
				add_output_port(CommonParameterUtil::get_str(
					cpt_world_view_direction),
					String(UTF8("view direction")),
					String(UTF8("xyz")), uuid_generator(),
					ect_fragment);
				break;
			case ept_uv:
				add_output_port(CommonParameterUtil::get_str(
					cpt_world_uv),
					String(UTF8("texture coordinate")),
					String(UTF8("uv")), uuid_generator(),
					ect_vertex);
				break;
			case ept_fragment_coordinate:
				add_output_port(String(UTF8("gl_FragCoord")),
					String(UTF8("uv")), uuid_generator(),
					ect_fragment);
				add_output_port(String(UTF8("gl_FragCoord")),
					String(UTF8("z")), uuid_generator(),
					ect_fragment);
				add_output_port(String(UTF8("gl_FragCoord")),
					String(UTF8("w")), uuid_generator(),
					ect_fragment);
				break;
			case ept_time:
				add_output_port(AutoParameterUtil::get_str(
					apt_time), String(UTF8("time")),
					String(UTF8("?")), uuid_generator(),
					ect_constant);
				break;
			case ept_camera:
				add_output_port(AutoParameterUtil::get_str(
					apt_camera), String(UTF8("camera")),
					String(UTF8("xyz")), uuid_generator(),
					ect_constant);
				break;
		}
	}

	EffectParameter::~EffectParameter() noexcept
	{
	}

	Uint16 EffectParameter::get_initial_value_count() const
	{
		switch (get_type())
		{
			case ept_position:
			case ept_normal:
			case ept_tangent:
			case ept_view_direction:
			case ept_camera:
				return 3;
			case ept_uv:
				return 2;
			case ept_fragment_coordinate:
				return 4;
			case ept_time:
				return 1;
		}

		return 0;
	}

	void EffectParameter::write(const Uint16StringMap &array_layers,
		const ShaderVersionType version,
		const EffectQualityType quality, const EffectChangeType change,
		StringUint16Map &parameters,
		ShaderSourceParameterVector &vertex_parameters,
		ShaderSourceParameterVector &fragment_parameters,
		OutStream &vertex_str, OutStream &fragment_str,
		EffectNodePtrSet &vertex_written,
		EffectNodePtrSet &fragment_written)
	{
		ShaderSourceParameterVector &shader_parameters =
			change == ect_fragment ? fragment_parameters :
			vertex_parameters;

		switch (get_type())
		{
			case ept_position:
				ShaderSourceParameterBuilder::add_parameter(
					String(UTF8("EffectParameter")),
					cpt_world_position, pqt_in,
					shader_parameters);
				break;
			case ept_normal:
				ShaderSourceParameterBuilder::add_parameter(
					String(UTF8("EffectParameter")),
					cpt_world_normal, pqt_in,
					shader_parameters);
				break;
			case ept_tangent:
				ShaderSourceParameterBuilder::add_parameter(
					String(UTF8("EffectParameter")),
					cpt_world_tangent, pqt_in,
					shader_parameters);
				break;
			case ept_view_direction:
				ShaderSourceParameterBuilder::add_parameter(
					String(UTF8("EffectParameter")),
					cpt_world_view_direction, pqt_in,
					fragment_parameters);
				break;
			case ept_uv:
				ShaderSourceParameterBuilder::add_parameter(
					String(UTF8("EffectParameter")),
					cpt_world_uv, pqt_in,
					shader_parameters);
				break;
			case ept_fragment_coordinate:
				break;
			case ept_time:
				ShaderSourceParameterBuilder::add_parameter(
					String(UTF8("EffectParameter")),
					apt_time, shader_parameters);
				break;
			case ept_camera:
				ShaderSourceParameterBuilder::add_parameter(
					String(UTF8("EffectParameter")),
					apt_camera, shader_parameters);
				break;
		}
	}

	String EffectParameter::get_description() const
	{
		switch (get_type())
		{
			case ept_position:
				return String(UTF8("world position"));
			case ept_normal:
				return String(UTF8("world normal"));
			case ept_tangent:
				return String(UTF8("world tangent"));
			case ept_view_direction:
				return String(UTF8("view direction"));
			case ept_uv:
				return String(UTF8("texture coordinate"));
			case ept_fragment_coordinate:
				return String(UTF8("fragment coordinate"));
			case ept_time:
				return String(UTF8("time in seconds"));
			case ept_camera:
				return String(UTF8("camera position"));
		}

		return String();
	}

	void EffectParameter::save_xml(const XmlWriterSharedPtr &writer) const
	{
		writer->start_element(String(UTF8("effect_parameter")));

		EffectNode::save_xml(writer);

		writer->write_element(String(UTF8("type")),
			EffectParameterUtil::get_str(get_type()));

		writer->end_element();
	}

	void EffectParameter::load_xml(const xmlNodePtr node)
	{
		xmlNodePtr it;

		if (xmlStrcmp(node->name, BAD_CAST UTF8("effect_parameter"))
			!= 0)
		{
			return;
		}

		EffectNode::load_xml(node);

		it = XmlUtil::children(node, true);

		do
		{
			if (xmlStrcmp(it->name, BAD_CAST UTF8("type")) == 0)
			{
				m_type = EffectParameterUtil::
					get_effect_parameter(
						XmlUtil::get_string_value(it));
			}
		}
		while (XmlUtil::next(it, true));
	}

}
