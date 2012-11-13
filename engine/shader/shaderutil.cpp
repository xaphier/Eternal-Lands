/****************************************************************************
 *            shaderutil.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "shaderutil.hpp"
#include "exceptions.hpp"

namespace eternal_lands
{

	namespace
	{

		const String shader_names[] =
		{
			String(UTF8("vertex")),
			String(UTF8("tess_control")),
			String(UTF8("tess_evaluation")),
			String(UTF8("geometry")),
			String(UTF8("fragment"))
		};

		const Uint32 shader_names_count =
			sizeof(shader_names) / sizeof(String);

	}

	const String &ShaderUtil::get_str(const ShaderType shader)
	{
		if (shader_names_count <= shader)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					shader_names_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					shader))
				<< boost::errinfo_type_info_name(UTF8(
					"ShaderType")));
		}

		return shader_names[shader];
	}

	ShaderType ShaderUtil::get_shader(const String &str)
	{
		Uint32 i;
		ShaderType shader_type;

		for (i = 0; i < shader_names_count; ++i)
		{
			shader_type =
				static_cast<ShaderType>(i);

			if (str == get_str(shader_type))
			{
				return shader_type;
			}
		}

		EL_THROW_EXCEPTION(InvalidParameterException()
			<< errinfo_string_value(str)
			<< boost::errinfo_type_info_name(UTF8(
				"ShaderType")));
	}

	Uint32 ShaderUtil::get_shader_count() noexcept
	{
		return shader_names_count;
	}

	String ShaderUtil::get_str(const ShaderTypeSet &shaders)
	{
		StringStream str;
		ShaderTypeSet::const_iterator it, end;
		bool first;

		end = shaders.end();
		first = true;

		for (it = shaders.begin(); it != end; ++it)
		{
			if (!first)
			{
				str << UTF8(" ");
			}

			str << *it;

			first = false;
		}

		return String(str.str());
	}

	ShaderTypeSet ShaderUtil::get_shaders(const String &str)
	{
		std::vector<std::string> tokens;
		std::vector<std::string>::iterator it;
		ShaderTypeSet result;
		std::string string;
		ShaderType shader_type;

		boost::split(tokens, str.get(), boost::is_any_of(UTF8(" ")),
			boost::token_compress_on);

		it = tokens.begin();

		while (it != tokens.end())
		{
			boost::algorithm::trim(*it);

			if (it->empty())
			{
				it = tokens.erase(it);
			}
			else
			{
				it++;
			}
		}

		BOOST_FOREACH(const std::string &string, tokens)
		{
			shader_type = ShaderUtil::get_shader(String(string));

			result.insert(shader_type);
		}

		return result;
	}

	OutStream &operator<<(OutStream &str, const ShaderType value)
	{
		str << ShaderUtil::get_str(value);

		return str;
	}

	OutStream &operator<<(OutStream &str, const ShaderTypeSet &value)
	{
		str << ShaderUtil::get_str(value);

		return str;
	}

	InStream &operator>>(InStream &str, ShaderType &value)
	{
		std::string string;

		str >> string;

		value = ShaderUtil::get_shader(String(string));

		return str;
	}

}
