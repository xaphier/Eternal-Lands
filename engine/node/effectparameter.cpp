/****************************************************************************
 *            effectparameter.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "effectparameter.hpp"
#include "effectnodeport.hpp"
#include "shader/autoparameterutil.hpp"
#include "shader/commonparameterutil.hpp"
#include "shader/shadersourceparameterbuilder.hpp"

namespace eternal_lands
{

	EffectParameter::EffectParameter(const String &name,
		const EffectParameterType type): EffectNode(name),
		m_type(type)
	{
		switch (get_type())
		{
			case ept_position:
				add_output_port(CommonParameterUtil::get_str(
					cpt_world_position), String(),
					String(UTF8("xyz")), ect_vertex);
				break;
			case ept_normal:
				add_output_port(CommonParameterUtil::get_str(
					cpt_world_normal), String(),
					String(UTF8("xyz")), ect_vertex);
				break;
			case ept_tangent:
				add_output_port(CommonParameterUtil::get_str(
					cpt_world_tangent), String(),
					String(UTF8("xyz")), ect_vertex);
				break;
			case ept_view_direction:
				add_output_port(CommonParameterUtil::get_str(
					cpt_world_view_direction), String(),
					String(UTF8("xyz")), ect_fragment);
				break;
			case ept_uv:
				add_output_port(CommonParameterUtil::get_str(
					cpt_world_uv), String(),
					String(UTF8("uv")), ect_vertex);
				break;
			case ept_fragment_coordinate:
				add_output_port(String("gl_FragCoord"),
					String(), String(UTF8("uv")),
					ect_fragment);
				add_output_port(String("gl_FragCoord"),
					String(), String(UTF8("z")),
					ect_fragment);
				add_output_port(String("gl_FragCoord"),
					String(), String(UTF8("w")),
					ect_fragment);
				break;
			case ept_time:
				add_output_port(AutoParameterUtil::get_str(
					apt_time), String(),
					String(UTF8("?")), ect_constant);
				break;
		}
	}

	EffectParameter::~EffectParameter() noexcept
	{
	}

	void EffectParameter::do_write(const bool glsl_120,
		const EffectChangeType change,
		StringBitSet16Map &parameters_indices,
		ShaderSourceParameterVector &vertex_parameters,
		ShaderSourceParameterVector &fragment_parameters,
		OutStream &vertex_str, OutStream &fragment_str,
		EffectNodePtrSet &written)
	{
		switch (get_type())
		{
			case ept_position:
				ShaderSourceParameterBuilder::add_parameter(
					String(UTF8("EffectParameter")),
					cpt_world_position, pqt_in,
					vertex_parameters);
				break;
			case ept_normal:
				ShaderSourceParameterBuilder::add_parameter(
					String(UTF8("EffectParameter")),
					cpt_world_normal, pqt_in,
					vertex_parameters);
				break;
			case ept_tangent:
				ShaderSourceParameterBuilder::add_parameter(
					String(UTF8("EffectParameter")),
					cpt_world_tangent, pqt_in,
					vertex_parameters);
				break;
			case ept_view_direction:
				ShaderSourceParameterBuilder::add_parameter(
					String(UTF8("EffectParameter")),
					cpt_world_view_direction, pqt_in,
					vertex_parameters);
				break;
			case ept_uv:
				ShaderSourceParameterBuilder::add_parameter(
					String(UTF8("EffectParameter")),
					cpt_world_uv, pqt_in,
					vertex_parameters);
				break;
			case ept_fragment_coordinate:
				break;
			case ept_time:
				if (change == ect_fragment)
				{
					ShaderSourceParameterBuilder::
						add_parameter(String(UTF8(
							"EffectParameter")),
							apt_time,
							fragment_parameters);
				}
				else
				{
					ShaderSourceParameterBuilder::
						add_parameter(String(UTF8(
							"EffectParameter")),
							apt_time,
							vertex_parameters);
				}
				break;
		}
	}

}
