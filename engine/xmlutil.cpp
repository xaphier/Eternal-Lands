/****************************************************************************
 *            xmlutil.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "xmlutil.hpp"
#include "exceptions.hpp"
#include "variantutil.hpp"
#include "glmutil.hpp"

namespace eternal_lands
{

	glm::bvec2 XmlUtil::get_bvec2_value(const xmlNodePtr node)
	{
		StringStream str;
		glm::bvec2 result;

		if (node == 0)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_message(UTF8("parameter is zero"))
				<< errinfo_parameter_name(UTF8("node")));
		}

		if (node->children == 0)
		{
			return glm::bvec2();
		}

		str << node->children->content;
		str >> result;

		return result;
	}

	glm::bvec3 XmlUtil::get_bvec3_value(const xmlNodePtr node)
	{
		StringStream str;
		glm::bvec3 result;

		if (node == 0)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_message(UTF8("parameter is zero"))
				<< errinfo_parameter_name(UTF8("node")));
		}

		if (node->children == 0)
		{
			return glm::bvec3();
		}

		str << node->children->content;
		str >> result;

		return result;
	}

	glm::bvec4 XmlUtil::get_bvec4_value(const xmlNodePtr node)
	{
		StringStream str;
		glm::bvec4 result;

		if (node == 0)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_message(UTF8("parameter is zero"))
				<< errinfo_parameter_name(UTF8("node")));
		}

		if (node->children == 0)
		{
			return glm::bvec4();
		}

		str << node->children->content;
		str >> result;

		return result;
	}

	glm::uvec2 XmlUtil::get_uvec2_value(const xmlNodePtr node)
	{
		StringStream str;
		glm::uvec2 result;

		if (node == 0)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_message(UTF8("parameter is zero"))
				<< errinfo_parameter_name(UTF8("node")));
		}

		if (node->children == 0)
		{
			return glm::uvec2();
		}

		str << node->children->content;
		str >> result;

		return result;
	}

	glm::uvec3 XmlUtil::get_uvec3_value(const xmlNodePtr node)
	{
		StringStream str;
		glm::uvec3 result;

		if (node == 0)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_message(UTF8("parameter is zero"))
				<< errinfo_parameter_name(UTF8("node")));
		}

		if (node->children == 0)
		{
			return glm::uvec3();
		}

		str << node->children->content;
		str >> result;

		return result;
	}

	glm::uvec4 XmlUtil::get_uvec4_value(const xmlNodePtr node)
	{
		StringStream str;
		glm::uvec4 result;

		if (node == 0)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_message(UTF8("parameter is zero"))
				<< errinfo_parameter_name(UTF8("node")));
		}

		if (node->children == 0)
		{
			return glm::uvec4();
		}

		str << node->children->content;
		str >> result;

		return result;
	}

	glm::ivec2 XmlUtil::get_ivec2_value(const xmlNodePtr node)
	{
		StringStream str;
		glm::ivec2 result;

		if (node == 0)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_message(UTF8("parameter is zero"))
				<< errinfo_parameter_name(UTF8("node")));
		}

		if (node->children == 0)
		{
			return glm::ivec2();
		}

		str << node->children->content;
		str >> result;

		return result;
	}

	glm::ivec3 XmlUtil::get_ivec3_value(const xmlNodePtr node)
	{
		StringStream str;
		glm::ivec3 result;

		if (node == 0)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_message(UTF8("parameter is zero"))
				<< errinfo_parameter_name(UTF8("node")));
		}

		if (node->children == 0)
		{
			return glm::ivec3();
		}

		str << node->children->content;
		str >> result;

		return result;
	}

	glm::ivec4 XmlUtil::get_ivec4_value(const xmlNodePtr node)
	{
		StringStream str;
		glm::ivec4 result;

		if (node == 0)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_message(UTF8("parameter is zero"))
				<< errinfo_parameter_name(UTF8("node")));
		}

		if (node->children == 0)
		{
			return glm::ivec4();
		}

		str << node->children->content;
		str >> result;

		return result;
	}

	glm::vec2 XmlUtil::get_vec2_value(const xmlNodePtr node)
	{
		StringStream str;
		glm::vec2 result;

		if (node == 0)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_message(UTF8("parameter is zero"))
				<< errinfo_parameter_name(UTF8("node")));
		}

		if (node->children == 0)
		{
			return glm::vec2();
		}

		str << node->children->content;
		str >> result;

		return result;
	}

	glm::vec3 XmlUtil::get_vec3_value(const xmlNodePtr node)
	{
		StringStream str;
		glm::vec3 result;

		if (node == 0)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_message(UTF8("parameter is zero"))
				<< errinfo_parameter_name(UTF8("node")));
		}

		if (node->children == 0)
		{
			return glm::vec3();
		}

		str << node->children->content;
		str >> result;

		return result;
	}

	glm::vec4 XmlUtil::get_vec4_value(const xmlNodePtr node)
	{
		StringStream str;
		glm::vec4 result;

		if (node == 0)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_message(UTF8("parameter is zero"))
				<< errinfo_parameter_name(UTF8("node")));
		}

		if (node->children == 0)
		{
			return glm::vec4();
		}

		str << node->children->content;
		str >> result;

		return result;
	}

	glm::mat2 XmlUtil::get_mat2_value(const xmlNodePtr node)
	{
		StringStream str;
		glm::mat2 result;

		if (node == 0)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_message(UTF8("parameter is zero"))
				<< errinfo_parameter_name(UTF8("node")));
		}

		if (node->children == 0)
		{
			return glm::mat2();
		}

		str << node->children->content;
		str >> result;

		return result;
	}

	glm::mat2x3 XmlUtil::get_mat2x3_value(const xmlNodePtr node)
	{
		StringStream str;
		glm::mat2x3 result;

		if (node == 0)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_message(UTF8("parameter is zero"))
				<< errinfo_parameter_name(UTF8("node")));
		}

		if (node->children == 0)
		{
			return glm::mat2x3();
		}

		str << node->children->content;
		str >> result;

		return result;
	}

	glm::mat2x4 XmlUtil::get_mat2x4_value(const xmlNodePtr node)
	{
		StringStream str;
		glm::mat2x4 result;

		if (node == 0)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_message(UTF8("parameter is zero"))
				<< errinfo_parameter_name(UTF8("node")));
		}

		if (node->children == 0)
		{
			return glm::mat2x4();
		}

		str << node->children->content;
		str >> result;

		return result;
	}

	glm::mat3x2 XmlUtil::get_mat3x2_value(const xmlNodePtr node)
	{
		StringStream str;
		glm::mat3x2 result;

		if (node == 0)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_message(UTF8("parameter is zero"))
				<< errinfo_parameter_name(UTF8("node")));
		}

		if (node->children == 0)
		{
			return glm::mat3x2();
		}

		str << node->children->content;
		str >> result;

		return result;
	}

	glm::mat3 XmlUtil::get_mat3_value(const xmlNodePtr node)
	{
		StringStream str;
		glm::mat3 result;

		if (node == 0)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_message(UTF8("parameter is zero"))
				<< errinfo_parameter_name(UTF8("node")));
		}

		if (node->children == 0)
		{
			return glm::mat3();
		}

		str << node->children->content;
		str >> result;

		return result;
	}

	glm::mat3x4 XmlUtil::get_mat3x4_value(const xmlNodePtr node)
	{
		StringStream str;
		glm::mat3x4 result;

		if (node == 0)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_message(UTF8("parameter is zero"))
				<< errinfo_parameter_name(UTF8("node")));
		}

		if (node->children == 0)
		{
			return glm::mat3x4();
		}

		str << node->children->content;
		str >> result;

		return result;
	}

	glm::mat4x2 XmlUtil::get_mat4x2_value(const xmlNodePtr node)
	{
		StringStream str;
		glm::mat4x2 result;

		if (node == 0)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_message(UTF8("parameter is zero"))
				<< errinfo_parameter_name(UTF8("node")));
		}

		if (node->children == 0)
		{
			return glm::mat4x2();
		}

		str << node->children->content;
		str >> result;

		return result;
	}

	glm::mat4x3 XmlUtil::get_mat4x3_value(const xmlNodePtr node)
	{
		StringStream str;
		glm::mat4x3 result;

		if (node == 0)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_message(UTF8("parameter is zero"))
				<< errinfo_parameter_name(UTF8("node")));
		}

		if (node->children == 0)
		{
			return glm::mat4x3();
		}

		str << node->children->content;
		str >> result;

		return result;
	}

	glm::mat4 XmlUtil::get_mat4_value(const xmlNodePtr node)
	{
		StringStream str;
		glm::mat4 result;

		if (node == 0)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_message(UTF8("parameter is zero"))
				<< errinfo_parameter_name(UTF8("node")));
		}

		if (node->children == 0)
		{
			return glm::mat4();
		}

		str << node->children->content;
		str >> result;

		return result;
	}

	glm::quat XmlUtil::get_quaternion_value(const xmlNodePtr node)
	{
		StringStream str;
		glm::quat result;

		if (node == 0)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_message(UTF8("parameter is zero"))
				<< errinfo_parameter_name(UTF8("node")));
		}

		if (node->children == 0)
		{
			return glm::quat();
		}

		str << node->children->content;
		str >> result;

		return result;
	}

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

	boost::uuids::uuid XmlUtil::get_uuid_value(const xmlNodePtr node)
	{
		if (node == 0)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_message(UTF8("parameter is zero"))
				<< errinfo_parameter_name(UTF8("node")));
		}

		if (node->children == 0)
		{
			return boost::uuids::uuid();
		}

		return boost::lexical_cast<boost::uuids::uuid>(
			node->children->content);
	}

	Sint32 XmlUtil::get_s32_value(const xmlNodePtr node)
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

	Uint32 XmlUtil::get_u32_value(const xmlNodePtr node)
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

	Sint16 XmlUtil::get_s16_value(const xmlNodePtr node)
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

	Uint16 XmlUtil::get_u16_value(const xmlNodePtr node)
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
			return String();
		}

		return String((char*)node->children->content);
	}

	BitSet64 XmlUtil::get_bitset64_value(const xmlNodePtr node)
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

		return boost::lexical_cast<BitSet64>(node->children->content);
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
		const String &element)
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
			if (xmlStrcmp(it->name, BAD_CAST element.get().c_str())
				== 0)
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

	bool XmlUtil::get_bool_property(const xmlNodePtr node,
		const String &property)
	{
		const xmlAttr *attr;

		if (node == 0)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_message(UTF8("parameter is zero"))
				<< errinfo_parameter_name(UTF8("node")));
		}

		for (attr = node->properties; attr; attr = attr->next)
		{
			if ((attr->type == XML_ATTRIBUTE_NODE) &&
				(xmlStrcasecmp(attr->name,
					BAD_CAST property.get().c_str()) == 0))
			{
				if (attr->children == 0)
				{
					return false;
				}

				if (xmlStrcmp(attr->children->content,
					BAD_CAST UTF8("true")) == 0)
				{
					return true;
				}

				if (xmlStrcmp(attr->children->content,
					BAD_CAST UTF8("yes")) == 0)
				{
					return true;
				}

				if (xmlStrcmp(attr->children->content,
					BAD_CAST UTF8("false")) == 0)
				{
					return false;
				}

				if (xmlStrcmp(attr->children->content,
					BAD_CAST UTF8("no")) == 0)
				{
					return false;
				}

				return boost::lexical_cast<bool>(
					attr->children->content);
			}
		}

		EL_THROW_EXCEPTION(ItemNotFoundException()
			<< errinfo_message(UTF8("Property not found"))
			<< errinfo_item_name(property)
			<< errinfo_parameter_name((char*)node->name));
	}

	bool XmlUtil::get_bool_property(const xmlNodePtr node,
		const String &property, const bool default_value)
	{
		const xmlAttr *attr;

		if (node == 0)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_message(UTF8("parameter is zero"))
				<< errinfo_parameter_name(UTF8("node")));
		}

		for (attr = node->properties; attr; attr = attr->next)
		{
			if ((attr->type == XML_ATTRIBUTE_NODE) &&
				(xmlStrcasecmp(attr->name,
					BAD_CAST property.get().c_str()) == 0))
			{
				if (attr->children == 0)
				{
					return default_value;
				}

				if (xmlStrcmp(attr->children->content,
					BAD_CAST UTF8("true")) == 0)
				{
					return true;
				}

				if (xmlStrcmp(attr->children->content,
					BAD_CAST UTF8("yes")) == 0)
				{
					return true;
				}

				if (xmlStrcmp(attr->children->content,
					BAD_CAST UTF8("false")) == 0)
				{
					return false;
				}

				if (xmlStrcmp(attr->children->content,
					BAD_CAST UTF8("no")) == 0)
				{
					return false;
				}

				return boost::lexical_cast<bool>(
					attr->children->content);
			}
		}

		return default_value;
	}

	FloatVector XmlUtil::get_float_vector(const xmlNodePtr node)
	{
		FloatVector result;
		StringStream values_str;
		String string;
		std::vector<std::string> values;

		if (node == 0)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_message(UTF8("parameter is zero"))
				<< errinfo_parameter_name(UTF8("node")));
		}

		if (node->children == 0)
		{
			return result;
		}

		values_str << node->children->content;
		string = String(values_str.str());

		boost::split(values, string.get(),
			boost::is_any_of(UTF8("\n\t ")),
			boost::token_compress_on);

		BOOST_FOREACH(const std::string &value, values)
		{
			StringStream str;
			float tmp;

			if (value.empty())
			{
				continue;
			}

			str << value;
			str >> tmp;

			result.push_back(tmp);
		}

		return result;

	}

	Sint32Vector XmlUtil::get_s32_vector(const xmlNodePtr node)
	{
		Sint32Vector result;
		StringStream values_str;
		String string;
		std::vector<std::string> values;

		if (node == 0)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_message(UTF8("parameter is zero"))
				<< errinfo_parameter_name(UTF8("node")));
		}

		if (node->children == 0)
		{
			return result;
		}

		values_str << node->children->content;
		string = String(values_str.str());

		boost::split(values, string.get(),
			boost::is_any_of(UTF8("\n\t ")),
			boost::token_compress_on);

		BOOST_FOREACH(const std::string &value, values)
		{
			StringStream str;
			Sint32 tmp;

			if (value.empty())
			{
				continue;
			}

			str << value;
			str >> tmp;

			result.push_back(tmp);
		}

		return result;

	}

	Sint16Vector XmlUtil::get_s16_vector(const xmlNodePtr node)
	{
		Sint16Vector result;
		StringStream values_str;
		String string;
		std::vector<std::string> values;

		if (node == 0)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_message(UTF8("parameter is zero"))
				<< errinfo_parameter_name(UTF8("node")));
		}

		if (node->children == 0)
		{
			return result;
		}

		values_str << node->children->content;
		string = String(values_str.str());

		boost::split(values, string.get(),
			boost::is_any_of(UTF8("\n\t ")),
			boost::token_compress_on);

		BOOST_FOREACH(const std::string &value, values)
		{
			StringStream str;
			Sint16 tmp;

			if (value.empty())
			{
				continue;
			}

			str << value;
			str >> tmp;

			result.push_back(tmp);
		}

		return result;

	}

	Uint32Vector XmlUtil::get_u32_vector(const xmlNodePtr node)
	{
		Uint32Vector result;
		StringStream values_str;
		String string;
		std::vector<std::string> values;

		if (node == 0)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_message(UTF8("parameter is zero"))
				<< errinfo_parameter_name(UTF8("node")));
		}

		if (node->children == 0)
		{
			return result;
		}

		values_str << node->children->content;
		string = String(values_str.str());

		boost::split(values, string.get(),
			boost::is_any_of(UTF8("\n\t ")),
			boost::token_compress_on);

		BOOST_FOREACH(const std::string &value, values)
		{
			StringStream str;
			Uint32 tmp;

			if (value.empty())
			{
				continue;
			}

			str << value;
			str >> tmp;

			result.push_back(tmp);
		}

		return result;

	}

	Uint16Vector XmlUtil::get_u16_vector(const xmlNodePtr node)
	{
		Uint16Vector result;
		StringStream values_str;
		String string;
		std::vector<std::string> values;

		if (node == 0)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_message(UTF8("parameter is zero"))
				<< errinfo_parameter_name(UTF8("node")));
		}

		if (node->children == 0)
		{
			return result;
		}

		values_str << node->children->content;
		string = String(values_str.str());

		boost::split(values, string.get(),
			boost::is_any_of(UTF8("\n\t ")),
			boost::token_compress_on);

		BOOST_FOREACH(const std::string &value, values)
		{
			StringStream str;
			Uint16 tmp;

			if (value.empty())
			{
				continue;
			}

			str << value;
			str >> tmp;

			result.push_back(tmp);
		}

		return result;

	}

}
