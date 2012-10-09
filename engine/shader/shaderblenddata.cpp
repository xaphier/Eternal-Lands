/****************************************************************************
 *            shaderblenddata.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "shaderblenddata.hpp"
#include "exceptions.hpp"
#include "logging.hpp"
#include "xmlwriter.hpp"
#include "xmlreader.hpp"
#include "xmlutil.hpp"

namespace eternal_lands
{

	ShaderBlendData::ShaderBlendData(): m_blend(sbt_blend)
	{
	}

	ShaderBlendData::ShaderBlendData(const glm::vec2 &data,
		const ShaderBlendType blend): m_data(data), m_blend(blend)
	{
	}

	ShaderBlendData::ShaderBlendData(const ShaderBlendType blend):
		m_blend(blend)
	{
	}

	ShaderBlendData::ShaderBlendData(const xmlNodePtr node):
		m_blend(sbt_texture)
	{
		load_xml(node);
	}

	ShaderBlendData::~ShaderBlendData() noexcept
	{
	}

	void ShaderBlendData::load_xml(const xmlNodePtr node)
	{
		xmlNodePtr it;

		if (xmlStrcmp(node->name, BAD_CAST UTF8("shader_blend_data"))
			!= 0)
		{
			return;
		}

		it = XmlUtil::children(node, true);

		do
		{
			if (xmlStrcmp(it->name, BAD_CAST UTF8("blend")) == 0)
			{
				set_blend(ShaderBlendUtil::get_shader_blend(
					XmlUtil::get_string_value(it)));
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("data")) == 0)
			{
				set_data(XmlUtil::get_vec2_value(it));
			}
		}
		while (XmlUtil::next(it, true));
	}

	void ShaderBlendData::save_xml(const XmlWriterSharedPtr &writer) const
	{
		writer->start_element(String(UTF8("shader_blend_data")));

		writer->write_element(String(UTF8("blend")),
			ShaderBlendUtil::get_str(get_blend()));
		writer->write_vec2_element(String(UTF8("data")), get_data());

		writer->end_element();
	}

}
