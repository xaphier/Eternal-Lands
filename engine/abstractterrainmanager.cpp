/****************************************************************************
 *            abstractterrainmanager.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "abstractterrainmanager.hpp"
#include "exceptions.hpp"
#include "logging.hpp"
#include "xmlreader.hpp"
#include "xmlutil.hpp"
#include "xmlwriter.hpp"

namespace eternal_lands
{

	AbstractTerrainManager::AbstractTerrainManager()
	{
	}

	AbstractTerrainManager::~AbstractTerrainManager() throw()
	{
	}

	void AbstractTerrainManager::load_xml(
		const FileSystemSharedPtr &file_system,
		const String &file_name)
	{
		XmlReaderSharedPtr xml_reader;

		xml_reader = XmlReaderSharedPtr(new XmlReader(file_system,
			file_name));

		load_xml(xml_reader->get_root_node());
	}

	void AbstractTerrainManager::load_xml(const xmlNodePtr node)
	{
		xmlNodePtr it;

		if (node == 0)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_message(UTF8("parameter is zero"))
				<< errinfo_parameter_name(UTF8("node")));
		}

		if (xmlStrcmp(node->name, BAD_CAST UTF8("terrain")) != 0)
		{
			LOG_WARNING(UTF8("Can't read node '%1%', expected "
				"'%2%'."), ((char*)node->name) %
				UTF8("terrain"));
			return;
		}

		it = XmlUtil::children(node, true);

		do
		{
			if (xmlStrcmp(it->name, BAD_CAST UTF8("albedo_0"))
				== 0)
			{
				set_albedo_map(XmlUtil::get_string_value(it),
					0);
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("albedo_1"))
				== 0)
			{
				set_albedo_map(XmlUtil::get_string_value(it),
					1);
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("albedo_2"))
				== 0)
			{
				set_albedo_map(XmlUtil::get_string_value(it),
					2);
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("albedo_3"))
				== 0)
			{
				set_albedo_map(XmlUtil::get_string_value(it),
					3);
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("blend")) == 0)
			{
				set_blend_map(XmlUtil::get_string_value(it));
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("height")) == 0)
			{
				set_height_map(XmlUtil::get_string_value(it));
			}

			if (xmlStrcmp(it->name,	BAD_CAST UTF8("rotation")) == 0)
			{
				m_transformation.set_rotation(
					XmlUtil::get_quaternion_value(it));
			}

			if (xmlStrcmp(it->name,	BAD_CAST UTF8("translation")) == 0)
			{
				m_transformation.set_translation(
					XmlUtil::get_vec3_value(it));
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("scale")) == 0)
			{
				m_transformation.set_scale(
					XmlUtil::get_float_value(it));
			}
		}
		while (XmlUtil::next(it, true));
	}

	void AbstractTerrainManager::save_xml(const XmlWriterSharedPtr &writer)
		const
	{
		writer->start_element(UTF8("terrain"));
		writer->write_element(UTF8("albedo_0"), get_albedo_map(0));
		writer->write_element(UTF8("albedo_1"), get_albedo_map(1));
		writer->write_element(UTF8("albedo_2"), get_albedo_map(2));
		writer->write_element(UTF8("albedo_3"), get_albedo_map(3));
		writer->write_element(UTF8("blend"), get_blend_map());
		writer->write_element(UTF8("height"), get_height_map());
		writer->write_quaternion_element(UTF8("rotation"),
			get_transformation().get_rotation());
		writer->write_vec3_element(UTF8("translation"),
			get_transformation().get_translation());
		writer->write_float_element(UTF8("scale"),
			get_transformation().get_scale());
		writer->end_element();
	}

}
