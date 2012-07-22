/****************************************************************************
 *            effectnodes.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "effectnodes.hpp"
#include "effectnode.hpp"
#include "effectnodeport.hpp"
#include "effectchangeutil.hpp"
#include "effectconstant.hpp"
#include "effectfunction.hpp"
#include "effectparameter.hpp"
#include "effecttexture.hpp"
#include "effectoutput.hpp"

namespace eternal_lands
{

	EffectNodes::EffectNodes(const String &name): m_name(name), m_ids(0)
	{
	}

	EffectNodes::~EffectNodes() noexcept
	{
	}

	EffectNodePtr EffectNodes::add_color(const String &name)
	{
		m_nodes.push_back(new EffectConstant(name, m_ids++,
			ect_color_rgb));

		return &(*m_nodes.rbegin());
	}

	EffectNodePtr EffectNodes::add_direction(const String &name)
	{
		m_nodes.push_back(new EffectConstant(name, m_ids++,
			ect_direction_xy));

		return &(*m_nodes.rbegin());
	}

	EffectNodePtr EffectNodes::add_constant(const String &name,
		const Uint16 count)
	{
		switch (count)
		{
			case 1:
				m_nodes.push_back(new EffectConstant(name,
					m_ids++, ect_float));
				break;
			case 2:
				m_nodes.push_back(new EffectConstant(name,
					m_ids++, ect_vec2));
				break;
			case 3:
				m_nodes.push_back(new EffectConstant(name,
					m_ids++, ect_vec3));
				break;
			case 4:
			default:
				m_nodes.push_back(new EffectConstant(name,
					m_ids++, ect_vec4));
				break;
		}

		return &(*m_nodes.rbegin());
	}

	EffectNodePtr EffectNodes::add_function(const String &name,
		const EffectFunctionType function)
	{
		m_nodes.push_back(new EffectFunction(name, m_ids++,
			function));

		return &(*m_nodes.rbegin());
	}

	EffectNodePtr EffectNodes::add_parameter(const String &name,
		const EffectParameterType parameter)
	{
		m_nodes.push_back(new EffectParameter(name, m_ids++,
			parameter));

		return &(*m_nodes.rbegin());
	}

	EffectNodePtr EffectNodes::add_texture(const String &name,
		const EffectSamplerType sampler,
		const EffectTextureType texture, const Uint16 texture_unit)
	{
		m_nodes.push_back(new EffectTexture(name, m_ids++,
			sampler, texture, texture_unit));

		return &(*m_nodes.rbegin());
	}

	EffectNodePtr EffectNodes::add_output(const String &name)
	{
		m_nodes.push_back(new EffectOutput(name, m_ids++));

		return &(*m_nodes.rbegin());
	}

	void EffectNodes::write(const Uint16StringMap &array_layers,
		const ShaderVersionType version, const bool low_quality,
		ShaderSourceParameterVector &vertex_parameters,
		ShaderSourceParameterVector &fragment_parameters,
		OutStream &vertex_str, OutStream &fragment_str)
	{
		EffectNodePtrSet vertex_written, fragment_written;
		StringUint16Map parameters;

		m_nodes.begin()->write(array_layers, version, low_quality,
			ect_constant, parameters, vertex_parameters,
			fragment_parameters, vertex_str, fragment_str,
			vertex_written, fragment_written);
	}

}
