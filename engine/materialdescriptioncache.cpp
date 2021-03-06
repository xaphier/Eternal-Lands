/****************************************************************************
 *            materialdescriptioncache.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "materialdescriptioncache.hpp"
#include "materialdescription.hpp"
#include "exceptions.hpp"
#include "logging.hpp"
#include "filesystem.hpp"
#include "xmlreader.hpp"
#include "xmlutil.hpp"
#include "xmlwriter.hpp"

namespace eternal_lands
{

	MaterialDescriptionCache::MaterialDescriptionCache()
	{
	}

	MaterialDescriptionCache::~MaterialDescriptionCache() noexcept
	{
	}

	const MaterialDescription
		&MaterialDescriptionCache::get_material_description(
			const String &name) const
	{
		StringMaterialDescriptionMap::const_iterator found;

		found = m_material_description_cache.find(name);

		if (found != m_material_description_cache.end())
		{
			return found->second;
		}

		EL_THROW_EXCEPTION(ItemNotFoundException()
			<< errinfo_message(UTF8("Material not found"))
			<< errinfo_item_name(name));
	}

	bool MaterialDescriptionCache::get_has_material_description(
		const String &name) const
	{
		return m_material_description_cache.find(name) !=
			m_material_description_cache.end();
	}

	void MaterialDescriptionCache::load_xml(const xmlNodePtr node)
	{
		String name;
		xmlNodePtr it;

		if (xmlStrcmp(node->name, BAD_CAST UTF8("materials")) != 0)
		{
			return;
		}

		if (!XmlUtil::has_children(node, true))
		{
			return;
		}

		it = XmlUtil::children(node, true);

		do
		{
			if (xmlStrcmp(it->name, BAD_CAST UTF8("material"))
				== 0)
			{
				MaterialDescription material_description;

				material_description.load_xml(it);

				name = material_description.get_name();

				m_material_description_cache[name] =
					material_description;
			}
		}
		while (XmlUtil::next(it, true));
	}

	void MaterialDescriptionCache::load_xml(
		const FileSystemConstSharedPtr &file_system,
		const String &file_name)
	{
		XmlReaderSharedPtr xml_reader;

		xml_reader = XmlReaderSharedPtr(new XmlReader(file_system,
			file_name));

		load_xml(xml_reader->get_root_node());
	}

	StringVector MaterialDescriptionCache::get_material_description_names()
		const
	{
		StringMaterialDescriptionMap::const_iterator it, end;
		StringVector result;

		end = m_material_description_cache.end();

		for (it = m_material_description_cache.begin(); it != end; ++it)
		{
			result.push_back(it->first);
		}

		return result;
	}

	void MaterialDescriptionCache::save_xml(const String &file_name) const
	{
		StringMaterialDescriptionMap::const_iterator it, end;
		XmlWriterSharedPtr writer;

		writer = XmlWriterSharedPtr(new XmlWriter(file_name));

		writer->start_element(String(UTF8("materials")));

		end = m_material_description_cache.end();

		for (it = m_material_description_cache.begin(); it != end; ++it)
		{
			it->second.save_xml(writer);
		}

		writer->end_element();
	}

}
