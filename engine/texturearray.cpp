/****************************************************************************
 *            texturearray.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "texturearray.hpp"
#include "filesystem.hpp"
#include "xmlutil.hpp"

namespace eternal_lands
{

	namespace
	{

		StringVector load_file_names_xml(const xmlNodePtr node)
		{
			StringVector file_names;
			xmlNodePtr it;

			if (xmlStrcmp(node->name, BAD_CAST UTF8("file_names"))
				!= 0)
			{
				return file_names;
			}

			if (!XmlUtil::has_children(node, true))
			{
				return file_names;
			}

			it = XmlUtil::children(node, true);

			do
			{
				if (xmlStrcmp(it->name,
					BAD_CAST UTF8("file_name")) == 0)
				{
					file_names.push_back(
						XmlUtil::get_string_value(it));
				}
			}
			while (XmlUtil::next(it, true));

			return file_names;
		}

	}

	TextureArray::TextureArray(): m_width(0), m_height(0), m_mipmaps(0),
		m_format(tft_rgba8)
	{
	}

	TextureArray::~TextureArray() throw()
	{
	}

	void TextureArray::load_xml(const xmlNodePtr node)
	{
		xmlNodePtr it;

		if (xmlStrcmp(node->name, BAD_CAST UTF8("texture_array")) != 0)
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
			if (xmlStrcmp(it->name, BAD_CAST UTF8("name")) == 0)
			{
				m_name = XmlUtil::get_string_value(it);
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("width")) == 0)
			{
				m_width = XmlUtil::get_uint32_value(it);
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("height")) == 0)
			{
				m_height = XmlUtil::get_uint32_value(it);
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("mipmaps")) == 0)
			{
				m_mipmaps = XmlUtil::get_uint16_value(it);
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("format")) == 0)
			{
				m_format = TextureFormatUtil::get_texture_format(
					XmlUtil::get_string_value(it));
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("file_names"))
				== 0)
			{
				m_file_names = load_file_names_xml(it);
			}
		}
		while (XmlUtil::next(it, true));
	}

}
