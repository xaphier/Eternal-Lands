/****************************************************************************
 *            blenddata.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "blenddata.hpp"
#include "exceptions.hpp"
#include "logging.hpp"
#include "xmlwriter.hpp"
#include "xmlreader.hpp"
#include "xmlutil.hpp"

namespace eternal_lands
{

	BlendData::BlendData(): m_blend_size(1.0f),
		m_use_blend_size(false)
	{
	}

	BlendData::BlendData(const float blend_size):
		m_blend_size(blend_size), m_use_blend_size(true)
	{
	}

	BlendData::BlendData(const float blend_size,
		const bool use_blend_size): m_blend_size(blend_size),
		m_use_blend_size(use_blend_size)
	{
	}

	BlendData::BlendData(const xmlNodePtr node):
		m_blend_size(1.0f), m_use_blend_size(false)
	{
		load_xml(node);
	}

	BlendData::~BlendData() noexcept
	{
	}

	bool BlendData::operator==(const BlendData &blend_data)
		const
	{
		return (blend_data.get_blend_size() == get_blend_size()) &&
			(blend_data.get_use_blend_size());
	}

	void BlendData::load_xml(const xmlNodePtr node)
	{
		xmlNodePtr it;

		if (xmlStrcmp(node->name, BAD_CAST UTF8("blend_data"))
			!= 0)
		{
			return;
		}

		it = XmlUtil::children(node, true);

		do
		{
			if (xmlStrcmp(it->name, BAD_CAST UTF8("blend_size"))
				== 0)
			{
				set_blend_size(XmlUtil::get_float_value(it));
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("use_blend_size"))
				== 0)
			{
				set_use_blend_size(XmlUtil::get_bool_value(it));
			}
		}
		while (XmlUtil::next(it, true));
	}

	void BlendData::save_xml(const XmlWriterSharedPtr &writer) const
	{
		writer->start_element(String(UTF8("blend_data")));

		writer->write_float_element(String(UTF8("blend_size")),
			get_blend_size());
		writer->write_bool_element(String(UTF8("use_blend_size")),
			get_use_blend_size());

		writer->end_element();
	}

}
