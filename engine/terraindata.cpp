/****************************************************************************
 *            terraindata.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "terraindata.hpp"
#include "exceptions.hpp"
#include "logging.hpp"
#include "xmlreader.hpp"
#include "xmlutil.hpp"
#include "xmlwriter.hpp"

namespace eternal_lands
{

	TerrainData::TerrainData(): m_dudv_scale(1.0f)
	{
	}

	TerrainData::TerrainData(const StringArray4 &albedo_maps,
		const String &blend_map, const String &height_map,
		const String &dudv_map, const glm::vec3 &translation,
		const glm::vec2 &dudv_scale): m_albedo_maps(albedo_maps),
		m_blend_map(blend_map), m_height_map(height_map),
		m_dudv_map(dudv_map), m_translation(translation),
		m_dudv_scale(dudv_scale)
	{
	}

	TerrainData::~TerrainData() throw()
	{
	}

	void TerrainData::load_xml(const FileSystemSharedPtr &file_system,
		const String &file_name)
	{
		XmlReaderSharedPtr xml_reader;

		xml_reader = XmlReaderSharedPtr(new XmlReader(file_system,
			file_name));

		load_xml(xml_reader->get_root_node());
	}

	void TerrainData::load_xml(const xmlNodePtr node)
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

			if (xmlStrcmp(it->name, BAD_CAST UTF8("dudv")) == 0)
			{
				set_dudv_map(XmlUtil::get_string_value(it));
			}

			if (xmlStrcmp(it->name,	BAD_CAST UTF8("translation"))
				== 0)
			{
				m_translation = XmlUtil::get_vec3_value(it);
			}

			if (xmlStrcmp(it->name,	BAD_CAST UTF8("dudv_scale"))
				== 0)
			{
				m_dudv_scale = XmlUtil::get_vec2_value(it);
			}
		}
		while (XmlUtil::next(it, true));
	}

	void TerrainData::save_xml(const XmlWriterSharedPtr &writer)
		const
	{
		writer->start_element(UTF8("terrain"));
		writer->write_element(UTF8("albedo_0"), get_albedo_map(0));
		writer->write_element(UTF8("albedo_1"), get_albedo_map(1));
		writer->write_element(UTF8("albedo_2"), get_albedo_map(2));
		writer->write_element(UTF8("albedo_3"), get_albedo_map(3));
		writer->write_element(UTF8("blend"), get_blend_map());
		writer->write_element(UTF8("height"), get_height_map());
		writer->write_element(UTF8("dudv"), get_dudv_map());
		writer->write_vec3_element(UTF8("translation"),
			get_translation());
		writer->write_vec2_element(UTF8("dudv_scale"),
			get_dudv_scale());
		writer->end_element();
	}

}
