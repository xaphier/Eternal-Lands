/****************************************************************************
 *            xmlutil.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "xmlutil.hpp"
#include "exceptions.hpp"

namespace eternal_lands
{

	float XmlUtil::get_float_value(const xmlNodePtr node)
	{
		if (node == 0)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_message(L"parameter is zero")
				<< errinfo_parameter_name(L"node"));
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
				<< errinfo_message(L"parameter is zero")
				<< errinfo_parameter_name(L"node"));
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
				<< errinfo_message(L"parameter is zero")
				<< errinfo_parameter_name(L"node"));
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
				<< errinfo_message(L"parameter is zero")
				<< errinfo_parameter_name(L"node"));
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
				<< errinfo_message(L"parameter is zero")
				<< errinfo_parameter_name(L"node"));
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
				<< errinfo_message(L"parameter is zero")
				<< errinfo_parameter_name(L"node"));
		}

		if (node->children == 0)
		{
			return String(L"");
		}

		return String(utf8_to_string((char*)node->children->content));
	}

	bool XmlUtil::get_bool_value(const xmlNodePtr node)
	{
		if (node == 0)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_message(L"parameter is zero")
				<< errinfo_parameter_name(L"node"));
		}

		if (node->children == 0)
		{
			return false;
		}

		if (xmlStrcmp(node->children->content, BAD_CAST "true") == 0)
		{
			return true;
		}

		if (xmlStrcmp(node->children->content, BAD_CAST "yes") == 0)
		{
			return true;
		}

		if (xmlStrcmp(node->children->content, BAD_CAST "false") == 0)
		{
			return false;
		}

		if (xmlStrcmp(node->children->content, BAD_CAST "no") == 0)
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
				<< errinfo_message(L"parameter is zero")
				<< errinfo_parameter_name(L"it"));
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
				<< errinfo_message(L"parameter is zero")
				<< errinfo_parameter_name(L"it"));
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
				<< errinfo_message(L"no next xml node"));
		}
	}

	xmlNodePtr XmlUtil::children(const xmlNodePtr node,
		const bool elements_only)
	{
		xmlNodePtr result;

		if (node == 0)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_message(L"parameter is zero")
				<< errinfo_parameter_name(L"node"));
		}

		result = node->children;

		if (result == 0)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_message(L"no child xml node"));
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
				<< errinfo_message(L"parameter is zero")
				<< errinfo_parameter_name(L"node"));
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
