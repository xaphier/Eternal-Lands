/****************************************************************************
 *            materialdescription.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "materialdescription.hpp"
#include "exceptions.hpp"
#include "xmlutil.hpp"
#include "xmlreader.hpp"
#include "filesystem.hpp"

namespace eternal_lands
{

	MaterialDescription::MaterialDescription(): m_shadow(true),
		m_culling(true)
	{
	}

	MaterialDescription::~MaterialDescription() throw()
	{
	}

	void MaterialDescription::load_xml(const xmlNodePtr node)
	{
		xmlNodePtr it;

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

		do
		{
			if (xmlStrcmp(it->name, BAD_CAST UTF8("diffuse_0"))
				== 0)
			{
				set_texture(XmlUtil::get_string_value(it),
					stt_diffuse_0);
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("diffuse_1"))
				== 0)
			{
				set_texture(XmlUtil::get_string_value(it),
					stt_diffuse_1);
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("diffuse_2"))
				== 0)
			{
				set_texture(XmlUtil::get_string_value(it),
					stt_diffuse_2);
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("diffuse_3"))
				== 0)
			{
				set_texture(XmlUtil::get_string_value(it),
					stt_diffuse_3);
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("normal_0"))
				== 0)
			{
				set_texture(XmlUtil::get_string_value(it),
					stt_normal_0);
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("normal_1"))
				== 0)
			{
				set_texture(XmlUtil::get_string_value(it),
					stt_normal_1);
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("specular_0"))
				== 0)
			{
				set_texture(XmlUtil::get_string_value(it),
					stt_specular_0);
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("specular_1"))
				== 0)
			{
				set_texture(XmlUtil::get_string_value(it),
					stt_specular_1);
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("glow_0")) == 0)
			{
				set_texture(XmlUtil::get_string_value(it),
					stt_glow_0);
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("glow_1")) == 0)
			{
				set_texture(XmlUtil::get_string_value(it),
					stt_glow_1);
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("blend_0")) == 0)
			{
				set_texture(XmlUtil::get_string_value(it),
					stt_blend_0);
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("blend_1")) == 0)
			{
				set_texture(XmlUtil::get_string_value(it),
					stt_blend_1);
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

	void MaterialDescription::load_xml(const String &dir,
		const xmlNodePtr node)
	{
		xmlNodePtr it;

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

		do
		{
			if (xmlStrcmp(it->name, BAD_CAST UTF8("diffuse_0"))
				== 0)
			{
				set_texture(String(dir.get() + UTF8("/") +
					XmlUtil::get_string_value(it).get()),
					stt_diffuse_0);
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("diffuse_1"))
				== 0)
			{
				set_texture(String(dir.get() + UTF8("/") +
					XmlUtil::get_string_value(it).get()),
					stt_diffuse_1);
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("diffuse_2"))
				== 0)
			{
				set_texture(String(dir.get() + UTF8("/") +
					XmlUtil::get_string_value(it).get()),
					stt_diffuse_2);
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("diffuse_3"))
				== 0)
			{
				set_texture(String(dir.get() + UTF8("/") +
					XmlUtil::get_string_value(it).get()),
					stt_diffuse_3);
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("normal_0"))
				== 0)
			{
				set_texture(String(dir.get() + UTF8("/") +
					XmlUtil::get_string_value(it).get()),
					stt_normal_0);
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("normal_1"))
				== 0)
			{
				set_texture(String(dir.get() + UTF8("/") +
					XmlUtil::get_string_value(it).get()),
					stt_normal_1);
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("specular_0"))
				== 0)
			{
				set_texture(String(dir.get() + UTF8("/") +
					XmlUtil::get_string_value(it).get()),
					stt_specular_0);
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("specular_1"))
				== 0)
			{
				set_texture(String(dir.get() + UTF8("/") +
					XmlUtil::get_string_value(it).get()),
					stt_specular_1);
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("glow_0")) == 0)
			{
				set_texture(String(dir.get() + UTF8("/") +
					XmlUtil::get_string_value(it).get()),
					stt_glow_0);
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("glow_1")) == 0)
			{
				set_texture(String(dir.get() + UTF8("/") +
					XmlUtil::get_string_value(it).get()),
					stt_glow_1);
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("blend_0")) == 0)
			{
				set_texture(String(dir.get() + UTF8("/") +
					XmlUtil::get_string_value(it).get()),
					stt_blend_0);
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("blend_1")) == 0)
			{
				set_texture(String(dir.get() + UTF8("/") +
					XmlUtil::get_string_value(it).get()),
					stt_blend_1);
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

		load_xml(FileSystem::get_dir_name(file_name),
			xml_reader->get_root_node());
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
			compare_non_textures(material);
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

}
