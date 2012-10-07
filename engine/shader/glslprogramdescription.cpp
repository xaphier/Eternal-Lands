/****************************************************************************
 *            glslprogramdescription.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "glslprogramdescription.hpp"
#include "xmlwriter.hpp"
#include "xmlreader.hpp"
#include "xmlutil.hpp"

namespace eternal_lands
{

	GlslProgramDescription::GlslProgramDescription()
	{
	}

	GlslProgramDescription::GlslProgramDescription(
		const String &vertex_shader, const String &tess_control_shader,
		const String &tess_evaluation_shader,
		const String &geometry_shader, const String &fragment_shader):
		m_vertex_shader(vertex_shader),
		m_tess_control_shader(tess_control_shader),
		m_tess_evaluation_shader(tess_evaluation_shader),
		m_geometry_shader(geometry_shader),
		m_fragment_shader(fragment_shader)
	{
	}

	GlslProgramDescription::~GlslProgramDescription() noexcept
	{
	}

	void GlslProgramDescription::load_xml(
		const FileSystemSharedPtr &file_system, const String &file_name)
	{
		XmlReaderSharedPtr xml_reader;

		xml_reader = XmlReaderSharedPtr(new XmlReader(file_system,
			file_name));

		load_xml(xml_reader->get_root_node());
	}

	void GlslProgramDescription::load_xml(const xmlNodePtr node)
	{
		xmlNodePtr it;

		if (xmlStrcmp(node->name,
			BAD_CAST UTF8("glslprogram")) != 0)
		{
			return;
		}

		it = XmlUtil::children(node, true);

		do
		{
			if (xmlStrcmp(it->name, BAD_CAST UTF8("vertex_shader"))
				== 0)
			{
				set_vertex_shader(
					XmlUtil::get_string_value(it));
			}

			if (xmlStrcmp(it->name,
				BAD_CAST UTF8("tess_control_shader")) == 0)
			{
				set_tess_control_shader(
					XmlUtil::get_string_value(it));
			}

			if (xmlStrcmp(it->name,
				BAD_CAST UTF8("tess_evaluation_shader")) == 0)
			{
				set_tess_evaluation_shader(
					XmlUtil::get_string_value(it));
			}

			if (xmlStrcmp(it->name,
				BAD_CAST UTF8("geometry_shader")) == 0)
			{
				set_geometry_shader(
					XmlUtil::get_string_value(it));
			}

			if (xmlStrcmp(it->name,
				BAD_CAST UTF8("fragment_shader")) == 0)
			{
				set_fragment_shader(
					XmlUtil::get_string_value(it));
			}
		}
		while (XmlUtil::next(it, true));
	}

	void GlslProgramDescription::save_xml(
		const XmlWriterSharedPtr &writer) const
	{
		writer->start_element(String(UTF8("glslprogram")));
		writer->write_element(String(UTF8("vertex_shader")),
			get_vertex_shader());
		writer->write_element(String(UTF8("tess_control_shader")),
			get_tess_control_shader());
		writer->write_element(String(UTF8("tess_evaluation_shader")),
			get_tess_evaluation_shader());
		writer->write_element(String(UTF8("geometry_shader")),
			get_geometry_shader());
		writer->write_element(String(UTF8("fragment_shader")),
			get_fragment_shader());
		writer->end_element();
	}

	bool GlslProgramDescription::operator==(
		const GlslProgramDescription &description) const
	{
		if (get_vertex_shader() !=
			description.get_vertex_shader())
		{
			return false;
		}

		if (get_tess_control_shader() !=
			description.get_tess_control_shader())
		{
			return false;
		}

		if (get_tess_evaluation_shader() !=
			description.get_tess_evaluation_shader())
		{
			return false;
		}

		if (get_geometry_shader() != description.get_geometry_shader())
		{
			return false;
		}

		return get_fragment_shader() ==
			description.get_fragment_shader();
	}

	bool GlslProgramDescription::operator!=(
		const GlslProgramDescription &description) const
	{
		return !operator==(description);
	}

	bool GlslProgramDescription::operator<(
		const GlslProgramDescription &description) const
	{
		if (get_vertex_shader() != description.get_vertex_shader())
		{
			return get_vertex_shader() <
				description.get_vertex_shader();
		}

		if (get_tess_control_shader() !=
			description.get_tess_control_shader())
		{
			return get_tess_control_shader() <
				description.get_tess_control_shader();
		}

		if (get_tess_evaluation_shader() !=
			description.get_tess_evaluation_shader())
		{
			return get_tess_evaluation_shader() <
				description.get_tess_evaluation_shader();
		}

		if (get_geometry_shader() != description.get_geometry_shader())
		{
			return get_geometry_shader() <
				description.get_geometry_shader();
		}

		return get_fragment_shader() <
			description.get_fragment_shader();
	}

	OutStream &operator<<(OutStream &str,
		const GlslProgramDescription &value)
	{
		str << UTF8("vertex_shader") << value.get_vertex_shader();
		str << UTF8(" tess_control_shader") <<
			value.get_tess_control_shader();
		str << UTF8(" tess_evaluation_shader") <<
			value.get_tess_evaluation_shader();
		str << UTF8(" geometry_shader") << value.get_geometry_shader();
		str << UTF8(" fragment_shader") << value.get_fragment_shader();

		return str;
	}

}
