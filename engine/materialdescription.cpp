/****************************************************************************
 *            materialdescription.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "materialdescription.hpp"
#include "exceptions.hpp"
#include "texturearraycache.hpp"
#include "xmlutil.hpp"
#include "xmlreader.hpp"

namespace eternal_lands
{

	MaterialDescription::MaterialDescription(): m_layer_index(0.0f),
		m_shadow(true), m_culling(true)
	{
	}

	MaterialDescription::~MaterialDescription() throw()
	{
	}

	void MaterialDescription::load_xml(const xmlNodePtr node)
	{
		xmlNodePtr it;
		ShaderTextureType texture;
		Uint16 i, count;

		if (node == 0)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_message(UTF8("parameter is zero"))
				<< errinfo_parameter_name(UTF8("node")));
		}

		if (xmlStrcmp(node->name, BAD_CAST UTF8("material")) != 0)
		{
			return;
		}

		it = XmlUtil::children(node, true);

		count = m_textures.size();

		do
		{
			for (i = 0; i < count; ++i)
			{
				texture = static_cast<ShaderTextureType>(i);

				if (xmlStrcmp(it->name, BAD_CAST
					ShaderTextureUtil::get_str(texture
						).get().c_str()) == 0)
				{
					set_texture(
						XmlUtil::get_string_value(it),
						texture);
				}
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("effect")) == 0)
			{
				set_effect(XmlUtil::get_string_value(it));
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("shadow")) == 0)
			{
				set_shadow(XmlUtil::get_bool_value(it));
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("culling")) == 0)
			{
				set_culling(XmlUtil::get_bool_value(it));
			}
		}
		while (XmlUtil::next(it, true));
	}

	void MaterialDescription::load_xml(
		const FileSystemSharedPtr &file_system,
		const String &file_name)
	{
		XmlReaderSharedPtr xml_reader;

		xml_reader = XmlReaderSharedPtr(new XmlReader(file_system,
			file_name));

		load_xml(xml_reader->get_root_node());
	}

	void MaterialDescription::load_xml(const ReaderSharedPtr &reader)
	{
		XmlReaderSharedPtr xml_reader;

		xml_reader = XmlReaderSharedPtr(new XmlReader(reader));

		load_xml(xml_reader->get_root_node());
	}

	bool MaterialDescription::compare_textures(
		const MaterialDescription &material) const
	{
		Uint16 i, count;

		count = m_textures.size();

		for (i = 0; i < count; ++i)
		{
			if (m_textures[i] != material.m_textures[i])
			{
				return false;
			}
		}

		return true;
	}

	bool MaterialDescription::compare_non_textures(
		const MaterialDescription &material) const
	{
		return (get_effect() == material.get_effect()) &&
			(get_shadow() == material.get_shadow()) &&
			(get_culling() == material.get_culling());
	}

	bool MaterialDescription::operator==(
		const MaterialDescription &material) const
	{
		return compare_textures(material) &&
			compare_non_textures(material) &&
			glm::all(glm::equal(get_layer_index(),
				material.get_layer_index()));
	}

	bool MaterialDescription::operator!=(
		const MaterialDescription &material) const
	{
		return !(*this == material);
	}

	bool MaterialDescription::operator<(
		const MaterialDescription &material) const
	{
		Uint16 i, count;

		if (get_effect() != material.get_effect())
		{
			return get_effect() < material.get_effect();
		}

		count = m_textures.size();

		for (i = 0; i < count; ++i)
		{
			if (m_textures[i] != material.m_textures[i])
			{
				return m_textures[i] < material.m_textures[i];
			}
		}

		count = m_layer_index.length();

		for (i = 0; i < count; ++i)
		{
			if (m_layer_index[i] != material.m_layer_index[i])
			{
				return m_layer_index[i] <
					material.m_layer_index[i];
			}
		}

		if (get_shadow() != material.get_shadow())
		{
			return get_shadow() < material.get_shadow();
		}

		return get_culling() < material.get_culling();
	}

	bool MaterialDescription::can_merge(
		const MaterialDescription &material) const
	{
		return compare_textures(material) &&
			compare_non_textures(material);
	}

	void MaterialDescription::build_layer_index(
		const TextureArrayCacheSharedPtr &texture_array_cache,
		const ShaderTextureType texture_type)
	{
		String name;
		float layer;
		Uint16 index;

		if (ShaderTextureUtil::get_use_layer_index(texture_type)
			&& texture_array_cache->get_texture_array_name(
				get_texture(texture_type), name, layer))
		{
			index = ShaderTextureUtil::get_layer_index(
				texture_type);

			m_layer_index[index] = layer;
			m_textures[texture_type] = name;
		}
	}

	void MaterialDescription::build_layer_index(
		const TextureArrayCacheSharedPtr &texture_array_cache)
	{
		Uint16 i, count;

		count = m_textures.size();

		for (i = 0; i < count; ++i)
		{
			build_layer_index(texture_array_cache,
				static_cast<ShaderTextureType>(i));
		}
	}

}
