/****************************************************************************
 *            effectoutput.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "effectoutput.hpp"
#include "effectnodeport.hpp"
#include "shader/commonparameterutil.hpp"
#include "xmlutil.hpp"
#include "xmlwriter.hpp"

namespace eternal_lands
{

	EffectOutput::EffectOutput()
	{
	}

	EffectOutput::EffectOutput(const String &name, const Uint32 id,
		Mt19937RandomUuidGenerator &uuid_generator):
		EffectNode(name, id, uuid_generator())
	{
		add_input_port(String(UTF8("albedo/diffuse color")),
			String(UTF8("rgb")), uuid_generator(), ect_fragment);
		add_input_port(String(UTF8("specular color")),
			String(UTF8("xyz")), uuid_generator(), ect_fragment);
		add_input_port(String(UTF8("alpha")), String(UTF8("?")),
			uuid_generator(), ect_fragment);
		add_input_port(String(UTF8("reflectance at normal incidence")),
			String(UTF8("?")), uuid_generator(), ect_fragment);
		add_input_port(String(UTF8("roughness")), String(UTF8("?")),
			uuid_generator(), ect_fragment);
		add_input_port(String(UTF8("emission")), String(UTF8("rgb")),
			uuid_generator(), ect_fragment);
		add_input_port(String(UTF8("normal")), String(UTF8("xyz")),
			uuid_generator(), ect_fragment);
	}

	EffectOutput::~EffectOutput() noexcept
	{
	}

	Uint16 EffectOutput::get_initial_value_count() const
	{
		return 4;
	}

	void EffectOutput::write(const Uint16StringMap &array_layers,
		const ShaderVersionType version,
		const EffectQualityType quality, const EffectChangeType change,
		StringUint16Map &parameters,
		ShaderSourceParameterVector &vertex_parameters,
		ShaderSourceParameterVector &fragment_parameters,
		OutStream &vertex_str, OutStream &fragment_str,
		EffectNodePtrSet &vertex_written,
		EffectNodePtrSet &fragment_written)
	{
		String default_value;

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

			port.write(array_layers, version, quality,
				ect_fragment, parameters, vertex_parameters,
				fragment_parameters, vertex_str, fragment_str,
				vertex_written, fragment_written);

			if (port.get_name() == UTF8("albedo/diffuse color"))
			{
				fragment_str << CommonParameterUtil::get_str(
					cpt_albedo);
				fragment_str << UTF8(".rgb");

				default_value = UTF8("vec3(1.0)");
			}

			if (port.get_name() == UTF8("alpha"))
			{
				fragment_str << CommonParameterUtil::get_str(
					cpt_albedo);
				fragment_str << UTF8(".a");

				default_value = UTF8("1.0");
			}

			if (port.get_name() == UTF8("specular color"))
			{
				fragment_str << CommonParameterUtil::get_str(
					cpt_specular);
				fragment_str << UTF8(".rgb");

				default_value = UTF8("vec3(1.0)");
			}

			if (port.get_name() ==
				UTF8("reflectance at normal incidence"))
			{
				fragment_str << CommonParameterUtil::get_str(
					cpt_reflectance_at_normal_incidence);

				default_value = UTF8("0.0");
			}

			if (port.get_name() == UTF8("roughness"))
			{
				fragment_str << CommonParameterUtil::get_str(
					cpt_roughness);

				default_value = UTF8("1.0");
			}

			if (port.get_name() == UTF8("emission"))
			{
				fragment_str << CommonParameterUtil::get_str(
					cpt_emission);

				default_value = UTF8("vec3(0.0)");
			}

			if (port.get_name() == UTF8("normal"))
			{
				fragment_str << CommonParameterUtil::get_str(
					cpt_fragment_normal);

				default_value = UTF8("vec3(0.0, 0.0, 1.0)");
			}

			fragment_str << UTF8(" = ");

			if (port.get_connected())
			{
				fragment_str <<
					port.get_connected_var_swizzled();
			}
			else
			{
				fragment_str << default_value;
			}

			fragment_str << UTF8(";\n");
		}
	}

	String EffectOutput::get_description() const
	{
		return String();
	}

	void EffectOutput::save_xml(const XmlWriterSharedPtr &writer)
	{
		writer->start_element(UTF8("effect_output"));

		EffectNode::save_xml(writer);

		writer->end_element();
	}

	void EffectOutput::load_xml(const xmlNodePtr node)
	{
		if (xmlStrcmp(node->name, BAD_CAST UTF8("effect_output"))
			!= 0)
		{
			return;
		}

		EffectNode::load_xml(node);
	}

}
