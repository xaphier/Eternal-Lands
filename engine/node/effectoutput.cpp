/****************************************************************************
 *            effectoutput.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "effectoutput.hpp"
#include "effectnodeport.hpp"
#include "shader/commonparameterutil.hpp"

namespace eternal_lands
{

	EffectOutput::EffectOutput(const String &name): EffectNode(name)
	{
		add_input_port(String(UTF8("albedo")), String(UTF8("rgb")),
			ect_fragment);
		add_input_port(String(UTF8("alpha")), String(UTF8("?")),
			ect_fragment);
		add_input_port(String(UTF8("roughness")), String(UTF8("?")),
			ect_fragment);
		add_input_port(String(UTF8("emission")), String(UTF8("rgb")),
			ect_fragment);
		add_input_port(String(UTF8("normal")), String(UTF8("xyz")),
			ect_fragment);
		add_input_port(String(UTF8("position")), String(UTF8("xyz")),
			ect_fragment);
	}

	EffectOutput::~EffectOutput() noexcept
	{
	}

	Uint16 EffectOutput::get_initial_value_count() const
	{
		return 4;
	}

	void EffectOutput::write(const bool glsl_120,
		const EffectChangeType change,
		StringUint16Map &parameters,
		ShaderSourceParameterVector &vertex_parameters,
		ShaderSourceParameterVector &fragment_parameters,
		OutStream &vertex_str, OutStream &fragment_str,
		EffectNodePtrSet &vertex_written,
		EffectNodePtrSet &fragment_written)
	{
		if (fragment_written.count(this) > 0)
		{
			return;
		}

		fragment_written.insert(this);

		BOOST_FOREACH(EffectNodePort &port, get_ports())
		{
			assert(port.get_input());

			if (!port.get_input())
			{
				continue;
			}

			port.write(glsl_120, ect_fragment, parameters,
				vertex_parameters, fragment_parameters,
				vertex_str, fragment_str, vertex_written,
				fragment_written);

			if (port.get_description() == UTF8("albedo"))
			{
				fragment_str << CommonParameterUtil::get_str(
					cpt_albedo);
				fragment_str << UTF8(".rgb");
			}

			if (port.get_description() == UTF8("alpha"))
			{
				fragment_str << CommonParameterUtil::get_str(
					cpt_albedo);
				fragment_str << UTF8(".a");
			}

			if (port.get_description() == UTF8("roughness"))
			{
				fragment_str << CommonParameterUtil::get_str(
					cpt_specular);
			}

			if (port.get_description() == UTF8("emission"))
			{
				fragment_str << CommonParameterUtil::get_str(
					cpt_emission);
			}

			if (port.get_description() == UTF8("normal"))
			{
				fragment_str << CommonParameterUtil::get_str(
					cpt_fragment_normal);
			}

			if (port.get_description() == UTF8("position"))
			{
				fragment_str << CommonParameterUtil::get_str(
					cpt_fragment_position);
			}

			fragment_str << UTF8(" = ");

			if (port.get_connected())
			{
				fragment_str <<
					port.get_connected_var_swizzled();
			}
			else
			{
				fragment_str << get_value_count_type_str();
				fragment_str << UTF8("(0.0)");
			}

			fragment_str << UTF8(";\n");
		}
	}

}
