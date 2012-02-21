/****************************************************************************
 *            shadersourceparameter.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "shadersourceparameter.hpp"
#include "exceptions.hpp"
#include "xmlutil.hpp"
#include "xmlwriter.hpp"

namespace eternal_lands
{

	ShaderSourceParameter::ShaderSourceParameter():
		m_type(pt_vec4), m_qualifier(pqt_in), m_size(pst_one),
		m_scale(0)
	{
	}

	ShaderSourceParameter::ShaderSourceParameter(const String &source,
		const xmlNodePtr node): m_source(source), m_type(pt_vec4),
		m_qualifier(pqt_in), m_size(pst_one), m_scale(0)
	{
		load_xml(source, node);
	}

	ShaderSourceParameter::ShaderSourceParameter(const String &source,
		const String &name, const ParameterType type,
		const ParameterQualifierType qualifier,
		const ParameterSizeType size, const Uint16 scale):
		m_source(source), m_name(name), m_type(type),
		m_qualifier(qualifier), m_size(size), m_scale(scale)
	{
		assert(!get_name().get().empty());
	}

	ShaderSourceParameter::~ShaderSourceParameter() throw()
	{
	}

	void ShaderSourceParameter::log(OutStream &str) const
	{
		assert(!get_name().get().empty());
		str << UTF8("ShaderSourceParameter");
		str << UTF8(" [source]: ") << get_source();
		str << UTF8(" [name]: ") << get_name();
		str << UTF8(" [type]: ") << get_type();
		str << UTF8(" [qualifier]: ") << get_qualifier();
		str << UTF8(" [scale]: ") << get_scale();
	}

	Uint16 ShaderSourceParameter::get_array_size(
		const ParameterSizeTypeUint16Map &sizes) const
	{
		ParameterSizeTypeUint16Map::const_iterator found;

		assert(!get_name().get().empty());

		found = sizes.find(get_size());

		if (found != sizes.end())
		{
			return found->second * get_scale();
		}

		return get_scale();
	}

	bool ShaderSourceParameter::get_use(
		const ShaderSourceParameter &parameter) const
	{
		assert(!get_name().get().empty());

		if (get_name() != parameter.get_name())
		{
			return false;
		}

		if (get_type() != parameter.get_type())
		{
			EL_THROW_MESSAGE_EXCEPTION(UTF8("Parameter '%1%' and "
				"'%2%' have the same name, but not equal "
				"types"), *this % parameter,
				InvalidParameterException());
		}

		if (get_size() != parameter.get_size())
		{
			EL_THROW_MESSAGE_EXCEPTION(UTF8("Parameter '%1%' and "
				"'%2%' have the same name, but not equal "
				"sizes"), *this % parameter,
				InvalidParameterException());
		}

		if (get_scale() != parameter.get_scale())
		{
			EL_THROW_MESSAGE_EXCEPTION(UTF8("Parameter '%1%' and "
				"'%2%' have the same name, but not equal "
				"scales"), *this % parameter,
				InvalidParameterException());
		}

		switch (get_qualifier())
		{
			case pqt_in:
				if (parameter.get_qualifier() == pqt_in)
				{
					return true;
				}
				break;
			case pqt_out:
				if ((parameter.get_qualifier() == pqt_inout) ||
					(parameter.get_qualifier() == pqt_in))
				{
					return true;
				}
				break;
			case pqt_inout:
				if ((parameter.get_qualifier() == pqt_inout) ||
					(parameter.get_qualifier() == pqt_in))
				{
					return true;
				}
				break;
		}

		EL_THROW_MESSAGE_EXCEPTION(UTF8("Parameter '%1%' and '%2%' "
			"have the same name, but not compatible qualifiers"),
			*this % parameter, InvalidParameterException());
	}

	void ShaderSourceParameter::write(const String &name_prefix,
		const ParameterSizeTypeUint16Map &sizes, OutStream &str) const
	{
		Uint16 array_size;

		assert(!get_name().get().empty());

		str << get_type() << UTF8(" ") << name_prefix << get_name();

		array_size = get_array_size(sizes);

		assert(array_size > 0);

		if ((array_size > 1) || (get_size() != pst_one))
		{
			str << UTF8("[") << array_size << UTF8("]");
		}
	}

	void ShaderSourceParameter::write(const String &name_prefix,
		const ParameterSizeTypeUint16Map &sizes, const String &prefix,
		OutStream &str, bool &first) const
	{
		assert(!get_name().get().empty());

		if (!first)
		{
			str << UTF8(", ");
		}

		str << prefix;

		write(name_prefix, sizes, str);

		first = false;
	}

	void ShaderSourceParameter::write_name(const String &name_prefix,
		OutStream &str, bool &first) const
	{
		assert(!get_name().get().empty());

		if (!first)
		{
			str << UTF8(", ");
		}

		str << name_prefix << get_name();

		first = false;
	}

	void ShaderSourceParameter::write_parameter(const String &name_prefix,
		const ParameterSizeTypeUint16Map &sizes, OutStream &str,
		bool &first) const
	{
		assert(!get_name().get().empty());

		if (!first)
		{
			str << UTF8(", ");
		}

		if (get_qualifier() == pqt_in)
		{
			str << UTF8("const ");
		}

		str << get_qualifier() << UTF8(" ");

		write(name_prefix, sizes, str);

		first = false;
	}

	void ShaderSourceParameter::load_xml(const String &source,
		const xmlNodePtr node)
	{
		xmlNodePtr it;

		if (xmlStrcmp(node->name, BAD_CAST UTF8("parameter")) != 0)
		{
			return;
		}

		it = XmlUtil::children(node, true);

		set_source(source);

		do
		{
			if (xmlStrcmp(it->name, BAD_CAST UTF8("name")) == 0)
			{
				set_name(XmlUtil::get_string_value(it));
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("type")) == 0)
			{
				set_type(ParameterUtil::get_parameter(
					XmlUtil::get_string_value(it)));
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("qualifier"))
				== 0)
			{
				set_qualifier(
					ParameterQualifierUtil::get_parameter_qualifier(
						XmlUtil::get_string_value(it)));
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("size")) == 0)
			{
				set_size(ParameterSizeUtil::get_parameter_size(
					XmlUtil::get_string_value(it)));
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("scale")) == 0)
			{
				set_scale(XmlUtil::get_uint16_value(it));
			}
		}
		while (XmlUtil::next(it, true));
	}

	void ShaderSourceParameter::save_xml(const XmlWriterSharedPtr &writer)
		const
	{
		writer->start_element(UTF8("parameter"));
		writer->write_element(UTF8("name"), get_name());
		writer->write_element(UTF8("type"), ParameterUtil::get_str(
			get_type()));
		writer->write_element(UTF8("qualifier"),
			ParameterQualifierUtil::get_str(get_qualifier()));
		writer->write_element(UTF8("size"),
			ParameterSizeUtil::get_str(get_size()));
		writer->write_int_element(UTF8("scale"), get_scale());
		writer->end_element();
	}

	OutStream& operator<<(OutStream &str,
		const ShaderSourceParameter &value)
	{
		value.log(str);

		return str;
	}

}
