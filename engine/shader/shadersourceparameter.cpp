/****************************************************************************
 *            shadersourceparameter.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
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

		assert(!get_name().get().empty());
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

	ShaderSourceParameter::ShaderSourceParameter(const String &source,
		const AutoParameterType auto_parameter): m_source(source),
		m_type(pt_vec4), m_qualifier(pqt_in), m_size(pst_one),
		m_scale(0)
	{
		set_auto_parameter(auto_parameter);

		assert(!get_name().get().empty());
	}

	ShaderSourceParameter::ShaderSourceParameter(const String &source,
		const CommonParameterType common_parameter,
		const ParameterQualifierType qualifier): m_source(source),
		m_type(pt_vec4), m_qualifier(qualifier), m_size(pst_one),
		m_scale(0)
	{
		set_common_parameter(common_parameter);

		assert(!get_name().get().empty());
	}

	ShaderSourceParameter::ShaderSourceParameter(const String &source,
		const SamplerParameterType sampler_parameter,
		const ParameterType type): m_source(source),
		m_type(type), m_qualifier(pqt_in), m_size(pst_one), m_scale(0)
	{
		set_sampler_parameter(sampler_parameter);

		assert(!get_name().get().empty());
	}

	ShaderSourceParameter::~ShaderSourceParameter() noexcept
	{
	}

	void ShaderSourceParameter::set_auto_parameter(
		const AutoParameterType auto_parameter)
	{
		set_name(AutoParameterUtil::get_str(auto_parameter));
		set_type(AutoParameterUtil::get_type(auto_parameter));
		set_size(AutoParameterUtil::get_size(auto_parameter));
		set_scale(AutoParameterUtil::get_scale(auto_parameter));
		set_qualifier(pqt_in);
	}

	void ShaderSourceParameter::set_common_parameter(
		const CommonParameterType common_parameter)
	{
		set_name(CommonParameterUtil::get_str(common_parameter));
		set_type(CommonParameterUtil::get_type(common_parameter));
		set_size(CommonParameterUtil::get_size(common_parameter));
		set_scale(CommonParameterUtil::get_scale(common_parameter));
	}

	void ShaderSourceParameter::set_sampler_parameter(
		const SamplerParameterType sampler_parameter)
	{
		set_name(SamplerParameterUtil::get_str(sampler_parameter));
		set_size(pst_one);
		set_scale(1);
		set_qualifier(pqt_in);
	}

	bool ShaderSourceParameter::get_auto_parameter() const
	{
		AutoParameterType auto_parameter;

		return get_auto_parameter(auto_parameter);
	}

	bool ShaderSourceParameter::get_common_parameter() const
	{
		CommonParameterType common_parameter;

		return get_common_parameter(common_parameter);
	}

	bool ShaderSourceParameter::get_sampler_parameter() const
	{
		SamplerParameterType sampler_parameter;

		return get_sampler_parameter(sampler_parameter);
	}

	bool ShaderSourceParameter::get_auto_parameter(
		AutoParameterType &auto_parameter) const
	{
		return AutoParameterUtil::get_auto_parameter(get_name(),
			auto_parameter);
	}

	bool ShaderSourceParameter::get_common_parameter(
		CommonParameterType &common_parameter) const
	{
		return CommonParameterUtil::get_common_parameter(get_name(),
			common_parameter);
	}

	bool ShaderSourceParameter::get_sampler_parameter(
		SamplerParameterType &sampler_parameter) const
	{
		return SamplerParameterUtil::get_sampler_parameter(get_name(),
			sampler_parameter);
	}

	void ShaderSourceParameter::check() const
	{
		AutoParameterType auto_parameter;
		CommonParameterType common_parameter;
		ParameterType type;
		ParameterSizeType size;
		Uint16 scale;

		if (get_auto_parameter(auto_parameter))
		{
			type = AutoParameterUtil::get_type(auto_parameter);
			size = AutoParameterUtil::get_size(auto_parameter);
			scale = AutoParameterUtil::get_scale(auto_parameter);

			if (type != get_type())
			{
				EL_THROW_MESSAGE_EXCEPTION(UTF8("source '%1%' "
					"name '%2%' has wrong type %3% instead"
					" of %4%"), get_source() % get_name() %
					get_type() % type,
					DuplicateItemException()
					<< errinfo_parameter_name(get_name()));
			}

			if (pqt_in != get_qualifier())
			{
				EL_THROW_MESSAGE_EXCEPTION(UTF8("source '%1%' "
					"name '%2%' has wrong qualifier %3% "
					"instead of %4%"), get_source() %
					get_name() % get_qualifier() % pqt_in,
					DuplicateItemException()
					<< errinfo_parameter_name(get_name()));
			}

			if (size != get_size())
			{
				EL_THROW_MESSAGE_EXCEPTION(UTF8("source '%1%' "
					"name '%2%' has wrong size %3% instead"
					" of %4%"), get_source() % get_name() %
					get_size() % size,
					DuplicateItemException()
					<< errinfo_parameter_name(get_name()));
			}

			if (scale != get_scale())
			{
				EL_THROW_MESSAGE_EXCEPTION(UTF8("source '%1%' "
					"name '%2%' has wrong scale %3% instead"
					" of %4%"), get_source() % get_name() %
					get_scale() % scale,
					DuplicateItemException()
					<< errinfo_parameter_name(get_name()));
			}

			return;
		}

		if (get_common_parameter(common_parameter))
		{
			type = CommonParameterUtil::get_type(common_parameter);
			size = CommonParameterUtil::get_size(common_parameter);
			scale = CommonParameterUtil::get_scale(
				common_parameter);

			if (type != get_type())
			{
				EL_THROW_MESSAGE_EXCEPTION(UTF8("source '%1%' "
					"name '%2%' has wrong type %3% instead"
					" of %4%"), get_source() % get_name() %
					get_type() % type,
					DuplicateItemException()
					<< errinfo_parameter_name(get_name()));
			}

			if (size != get_size())
			{
				EL_THROW_MESSAGE_EXCEPTION(UTF8("source '%1%' "
					"name '%2%' has wrong size %3% instead"
					" of %4%"), get_source() % get_name() %
					get_size() % size,
					DuplicateItemException()
					<< errinfo_parameter_name(get_name()));
			}

			if (scale != get_scale())
			{
				EL_THROW_MESSAGE_EXCEPTION(UTF8("source '%1%' "
					"name '%2%' has wrong scale %3% instead"
					" of %4%"), get_source() % get_name() %
					get_scale() % scale,
					DuplicateItemException()
					<< errinfo_parameter_name(get_name()));
			}

			return;
		}

		if (get_sampler_parameter())
		{
			if (!ParameterUtil::get_sampler(get_type()))
			{
				EL_THROW_MESSAGE_EXCEPTION(UTF8("source '%1%' "
					"name '%2%' has non sampler type %3%"),
					get_source() % get_name() % get_type(),
					DuplicateItemException()
					<< errinfo_parameter_name(get_name()));
			}

			if (pqt_in != get_qualifier())
			{
				EL_THROW_MESSAGE_EXCEPTION(UTF8("source '%1%' "
					"name '%2%' has wrong qualifier %3% "
					"instead of %4%"), get_source() %
					get_name() % get_qualifier() % pqt_in,
					DuplicateItemException()
					<< errinfo_parameter_name(get_name()));
			}

			if (pst_one != get_size())
			{
				EL_THROW_MESSAGE_EXCEPTION(UTF8("source '%1%' "
					"name '%2%' has wrong size %3% instead"
					" of %4%"), get_source() % get_name() %
					get_size() % pst_one,
					DuplicateItemException()
					<< errinfo_parameter_name(get_name()));
			}

			if (1 != get_scale())
			{
				EL_THROW_MESSAGE_EXCEPTION(UTF8("source '%1%' "
					"name '%2%' has wrong scale %3% instead"
					" of %4%"), get_source() % get_name() %
					get_scale() % 1,
					DuplicateItemException()
					<< errinfo_parameter_name(get_name()));
			}

			return;
		}

		if (ParameterUtil::get_sampler(get_type()))
		{
			EL_THROW_MESSAGE_EXCEPTION(UTF8("source '%1%' "
				"name '%2%' has sampler type %3%"),
				get_source() % get_name() % get_type(),
				InvalidParameterException()
				<< errinfo_parameter_name(get_name()));
		}
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
			if (xmlStrcmp(it->name,
				BAD_CAST UTF8("auto_parameter")) == 0)
			{
				set_auto_parameter(
					AutoParameterUtil::get_auto_parameter(
						XmlUtil::get_string_value(it)));
			}

			if (xmlStrcmp(it->name,
				BAD_CAST UTF8("common_parameter")) == 0)
			{
				set_common_parameter(
					CommonParameterUtil::get_common_parameter(
						XmlUtil::get_string_value(it)));
			}

			if (xmlStrcmp(it->name,
				BAD_CAST UTF8("sampler_parameter")) == 0)
			{
				set_sampler_parameter(
					SamplerParameterUtil::get_sampler_parameter(
						XmlUtil::get_string_value(it)));
			}

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

//		check();
	}

	void ShaderSourceParameter::save_xml(const XmlWriterSharedPtr &writer)
		const
	{
		writer->start_element(UTF8("parameter"));

		if (get_auto_parameter())
		{
			writer->write_element(UTF8("auto_parameter"),
				get_name());
			writer->end_element();

			return;
		}

		if (get_common_parameter())
		{
			writer->write_element(UTF8("common_parameter"),
				get_name());
			writer->write_element(UTF8("qualifier"),
				ParameterQualifierUtil::get_str(
					get_qualifier()));
			writer->end_element();

			return;
		}

		if (get_sampler_parameter())
		{
			writer->write_element(UTF8("sampler_parameter"),
				get_name());
			writer->write_element(UTF8("type"),
				ParameterUtil::get_str(get_type()));
			writer->end_element();

			return;
		}

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
