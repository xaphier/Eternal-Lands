/****************************************************************************
 *            xmlutil.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "xmlutil.hpp"
#include "exceptions.hpp"
#include "variantutil.hpp"

namespace eternal_lands
{

	float XmlUtil::get_float_value(const xmlNodePtr node)
	{
		if (node == 0)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_message(UTF8("parameter is zero"))
				<< errinfo_parameter_name(UTF8("node")));
		}

		if (node->children == 0)
		{
			return 0.0f;
		}

		return boost::lexical_cast<float>(node->children->content);
	}

	Sint32 XmlUtil::get_sint32_value(const xmlNodePtr node)
	{
		if (node == 0)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_message(UTF8("parameter is zero"))
				<< errinfo_parameter_name(UTF8("node")));
		}

		if (node->children == 0)
		{
			return 0;
		}

		return boost::lexical_cast<Sint32>(node->children->content);
	}

	Uint32 XmlUtil::get_uint32_value(const xmlNodePtr node)
	{
		if (node == 0)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_message(UTF8("parameter is zero"))
				<< errinfo_parameter_name(UTF8("node")));
		}

		if (node->children == 0)
		{
			return 0;
		}

		return boost::lexical_cast<Uint32>(node->children->content);
	}

	Sint16 XmlUtil::get_sint16_value(const xmlNodePtr node)
	{
		if (node == 0)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_message(UTF8("parameter is zero"))
				<< errinfo_parameter_name(UTF8("node")));
		}

		if (node->children == 0)
		{
			return 0;
		}

		return boost::lexical_cast<Sint16>(node->children->content);
	}

	Uint16 XmlUtil::get_uint16_value(const xmlNodePtr node)
	{
		if (node == 0)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_message(UTF8("parameter is zero"))
				<< errinfo_parameter_name(UTF8("node")));
		}

		if (node->children == 0)
		{
			return 0;
		}

		return boost::lexical_cast<Uint16>(node->children->content);
	}

	String XmlUtil::get_string_value(const xmlNodePtr node)
	{
		if (node == 0)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_message(UTF8("parameter is zero"))
				<< errinfo_parameter_name(UTF8("node")));
		}

		if (node->children == 0)
		{
			return String(UTF8(""));
		}

		return String((char*)node->children->content);
	}

	Variant XmlUtil::get_variant_value(const xmlNodePtr node)
	{
		String type, value;
		xmlNodePtr it;

		if (node == 0)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_message(UTF8("parameter is zero"))
				<< errinfo_parameter_name(UTF8("node")));
		}

		it = XmlUtil::children(node, true);

		do
		{
			if (xmlStrcmp(it->name, BAD_CAST UTF8("type")) == 0)
			{
				type = XmlUtil::get_string_value(it);
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("value")) == 0)
			{
				value = XmlUtil::get_string_value(it);
			}
		}
		while (XmlUtil::next(it, true));

		return VariantUtil::get_variant(type, value);
	}

	StringVariantPair XmlUtil::get_string_variant(const xmlNodePtr node)
	{
		String type, value, name;
		xmlNodePtr it;

		if (node == 0)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_message(UTF8("parameter is zero"))
				<< errinfo_parameter_name(UTF8("node")));
		}

		it = XmlUtil::children(node, true);

		do
		{
			if (xmlStrcmp(it->name, BAD_CAST UTF8("name")) == 0)
			{
				name = XmlUtil::get_string_value(it);
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("type")) == 0)
			{
				type = XmlUtil::get_string_value(it);
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("value")) == 0)
			{
				value = XmlUtil::get_string_value(it);
			}
		}
		while (XmlUtil::next(it, true));

		return StringVariantPair(name, VariantUtil::get_variant(type,
			value));
	}

	StringVariantMap XmlUtil::get_string_variant_map(const xmlNodePtr node,
		const StringType &element)
	{
		StringVariantMap result;
		xmlNodePtr it;

		if (node == 0)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_message(UTF8("parameter is zero"))
				<< errinfo_parameter_name(UTF8("node")));
		}

		it = XmlUtil::children(node, true);

		do
		{
			if (xmlStrcmp(it->name, BAD_CAST element.c_str()) == 0)
			{
				result.insert(get_string_variant(it));
			}
		}
		while (XmlUtil::next(it, true));

		return result;
	}

	bool XmlUtil::get_bool_value(const xmlNodePtr node)
	{
		if (node == 0)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_message(UTF8("parameter is zero"))
				<< errinfo_parameter_name(UTF8("node")));
		}

		if (node->children == 0)
		{
			return false;
		}

		if (xmlStrcmp(node->children->content, BAD_CAST UTF8("true"))
			== 0)
		{
			return true;
		}

		if (xmlStrcmp(node->children->content, BAD_CAST UTF8("yes"))
			== 0)
		{
			return true;
		}

		if (xmlStrcmp(node->children->content, BAD_CAST UTF8("false"))
			== 0)
		{
			return false;
		}

		if (xmlStrcmp(node->children->content, BAD_CAST UTF8("no"))
			== 0)
		{
			return false;
		}

		return boost::lexical_cast<bool>(node->children->content);
	}

	bool XmlUtil::skip_non_elements(xmlNodePtr &node)
	{
		if (node == 0)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_message(UTF8("parameter is zero"))
				<< errinfo_parameter_name(UTF8("it")));
		}

		while (node->type != XML_ELEMENT_NODE)
		{
			node = node->next;

			if (node == 0)
			{
				return false;
			}
		}

		return true;
	}

	bool XmlUtil::next(xmlNodePtr &node, const bool elements_only)
	{
		if (node == 0)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_message(UTF8("parameter is zero"))
				<< errinfo_parameter_name(UTF8("it")));
		}

		node = node->next;

		if (node == 0)
		{
			return false;
		}

		if (elements_only)
		{
			return skip_non_elements(node);
		}

		return true;
	}

	void XmlUtil::forece_next(xmlNodePtr &node, const bool elements_only)
	{
		if (!next(node, elements_only))
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_message(UTF8("no next xml node")));
		}
	}

	xmlNodePtr XmlUtil::children(const xmlNodePtr node,
		const bool elements_only)
	{
		xmlNodePtr result;

		if (node == 0)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_message(UTF8("parameter is zero"))
				<< errinfo_parameter_name(UTF8("node")));
		}

		result = node->children;

		if (result == 0)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_message(UTF8("no child xml node")));
		}

		if (elements_only)
		{
			skip_non_elements(result);
		}

		return result;
	}

	bool XmlUtil::has_children(const xmlNodePtr node,
		const bool elements_only)
	{
		xmlNodePtr it;

		if (node == 0)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_message(UTF8("parameter is zero"))
				<< errinfo_parameter_name(UTF8("node")));
		}

		it = node->children;

		if (it == 0)
		{
			return false;
		}

		if (elements_only)
		{
			return skip_non_elements(it);
		}

		return true;
	}

}
