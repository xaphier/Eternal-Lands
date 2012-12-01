/****************************************************************************
 *            terrainmaterialdata.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "terrainmaterialdata.hpp"
#include "xmlbuffer.hpp"
#include "xmlwriter.hpp"
#include "xmlreader.hpp"
#include "xmlutil.hpp"

namespace eternal_lands
{

	TerrainMaterialData::TerrainMaterialData(): m_write_height(true),
		m_write_specular_gloss(true)
	{
		resize(0);
	}

	TerrainMaterialData::~TerrainMaterialData() noexcept
	{
	}

	void TerrainMaterialData::load_layer_xml(const xmlNodePtr node,
		const Uint32 index)
	{
		xmlNodePtr it;
		glm::vec3 default_specular;
		float default_height, default_gloss, blend_size;

		if (xmlStrcmp(node->name, BAD_CAST UTF8("layer")) != 0)
		{
			return;
		}

		default_height = 0.0f;
		default_gloss = 0.0f;
		blend_size = 0.0f;

		it = XmlUtil::children(node, true);

		do
		{
			if (xmlStrcmp(it->name, BAD_CAST UTF8(
				"default_height")) == 0)
			{
				default_height = XmlUtil::get_float_value(it);
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8(
				"default_specular")) == 0)
			{
				default_specular = XmlUtil::get_vec3_value(it);
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8(
				"default_gloss")) == 0)
			{
				default_gloss = XmlUtil::get_float_value(it);
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8(
				"blend_size")) == 0)
			{
				blend_size = XmlUtil::get_float_value(it);
			}
		}
		while (XmlUtil::next(it, true));

		set_default_height(default_height, index);
		set_default_specular(default_specular, index);
		set_default_gloss(default_gloss, index);
		set_blend_size(blend_size, index);
	}

	void TerrainMaterialData::load_xml(const xmlNodePtr node)
	{
		xmlNodePtr it;
		Uint32 index;

		if (xmlStrcmp(node->name,
			BAD_CAST UTF8("terrain_material_data")) != 0)
		{
			return;
		}

		it = XmlUtil::children(node, true);

		clear();

		resize(64);
		index = 0;

		do
		{
			if (xmlStrcmp(it->name,
				BAD_CAST UTF8("use_height_maps")) == 0)
			{
				set_use_height_maps(
					XmlUtil::get_bitset64_value(it));
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8(
				"use_specular_maps")) == 0)
			{
				set_use_specular_maps(
					XmlUtil::get_bitset64_value(it));
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8(
				"use_gloss_maps")) == 0)
			{
				set_use_gloss_maps(
					XmlUtil::get_bitset64_value(it));
			}

			if (xmlStrcmp(it->name,
				BAD_CAST UTF8("use_blend_size_textures")) == 0)
			{
				set_use_blend_size_textures(
					XmlUtil::get_bitset64_value(it));
			}

			if (xmlStrcmp(it->name,
				BAD_CAST UTF8("write_height")) == 0)
			{
				set_write_height(XmlUtil::get_bool_value(it));
			}

			if (xmlStrcmp(it->name,
				BAD_CAST UTF8("write_specular_gloss")) == 0)
			{
				set_write_specular_gloss(
					XmlUtil::get_bool_value(it));
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("layer")) == 0)
			{
				if (index < 64)
				{
					load_layer_xml(it, index);
					index++;
				}
			}
		}
		while (XmlUtil::next(it, true));

		resize(index);
	}

	void TerrainMaterialData::load_xml_string(const String &str)
	{
		XmlReaderSharedPtr xml_reader;

		xml_reader = XmlReader::get_xml_reader_from_string(str);

		load_xml(xml_reader->get_root_node());
	}

	void TerrainMaterialData::save_xml(const XmlWriterSharedPtr &writer)
		const
	{
		Uint32 i, count;

		writer->start_element(String(UTF8("terrain_material_data")));
		writer->write_bitset64_element(String(UTF8("use_height_maps")),
			get_use_height_maps());
		writer->write_bitset64_element(
			String(UTF8("use_specular_maps")),
			get_use_specular_maps());
		writer->write_bitset64_element(String(UTF8("use_gloss_maps")),
			get_use_gloss_maps());
		writer->write_bitset64_element(
			String(UTF8("use_blend_size_textures")),
			get_use_blend_size_textures());
		writer->write_bool_element(String(UTF8("write_height")),
			get_write_height());
		writer->write_bool_element(String(UTF8("write_specular_gloss")),
			get_write_specular_gloss());

		count = get_layer_count();

		for (i = 0; i < count; ++i)
		{
			writer->start_element(String(UTF8("layer")));
			writer->write_float_element(
				String(UTF8("default_height")),
				get_default_height(i));
			writer->write_vec3_element(
				String(UTF8("default_specular")),
				get_default_specular(i));
			writer->write_float_element(
				String(UTF8("default_gloss")),
				get_default_gloss(i));
			writer->write_float_element(String(UTF8("blend_size")),
				get_blend_size(i));
			writer->end_element();
		}

		writer->end_element();
	}

	String TerrainMaterialData::save_xml_string() const
	{
		XmlBufferSharedPtr xml_buffer;

		xml_buffer = boost::make_shared<XmlBuffer>();

		{
			XmlWriterSharedPtr xml_writer;

			xml_writer = boost::make_shared<XmlWriter>(xml_buffer);

			save_xml(xml_writer);
		}

		return xml_buffer->get_string();
	}

}
