/****************************************************************************
 *            parameterutil.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "parameterutil.hpp"
#include "exceptions.hpp"

namespace eternal_lands
{

	namespace
	{

		enum ParameterValueType
		{
			pvt_int,
			pvt_unsigned_int,
			pvt_float,
			pvt_bool,
			pvt_shadow
		};

		enum ParameterFormatType
		{
			pft_array,
			pft_rect,
			pft_buffer,
			pft_none,
			pft_matrix
		};

		class ParameterTypeData
		{
			private:
				const String m_name;
				const Uint32 m_count;
				const GLenum m_gl_type;
				const ParameterValueType m_value_type;
				const ParameterFormatType m_format_type;
				const bool m_sampler;

			public:
				inline ParameterTypeData(
					const String &name,
					const Uint32 count,
					const GLenum gl_type,
					const ParameterValueType value_type,
					const ParameterFormatType format_type):
					m_name(name), m_count(count),
					m_gl_type(gl_type),
					m_value_type(value_type),
					m_format_type(format_type),
					m_sampler(false)
				{
					assert(get_count() > 0);
					assert(get_value_type() != pvt_shadow);
					assert(get_format_type() != pft_array);
					assert(get_format_type() != pft_rect);
					assert(get_format_type() != pft_buffer);
				}

				inline ParameterTypeData(
					const String &name,
					const GLenum gl_type,
					const ParameterValueType value_type,
					const ParameterFormatType format_type):
					m_name(name), m_count(1),
					m_gl_type(gl_type),
					m_value_type(value_type),
					m_format_type(format_type),
					m_sampler(true)
				{
					assert(get_count() > 0);
					assert(get_value_type() != pvt_bool);
					assert(get_format_type() != pft_matrix);
				}

				inline ~ParameterTypeData() noexcept
				{
				}

				inline const String &get_name() const noexcept
				{
					return m_name;
				}

				inline Uint32 get_count() const noexcept
				{
					return m_count;
				}

				inline GLenum get_gl_type() const noexcept
				{
					return m_gl_type;
				}

				inline bool get_sampler() const noexcept
				{
					return m_sampler;
				}

				inline ParameterValueType get_value_type()
					const noexcept
				{
					return m_value_type;
				}

				inline ParameterFormatType get_format_type()
					const noexcept
				{
					return m_format_type;
				}

		};

		const ParameterTypeData parameter_datas[] =
		{
			ParameterTypeData(String(UTF8("bool")), 1,
				GL_BOOL, pvt_bool, pft_none),
			ParameterTypeData(String(UTF8("bvec2")), 2,
				GL_BOOL_VEC2, pvt_bool, pft_none),
			ParameterTypeData(String(UTF8("bvec3")), 3,
				GL_BOOL_VEC3, pvt_bool, pft_none),
			ParameterTypeData(String(UTF8("bvec4")), 4,
				GL_BOOL_VEC4, pvt_bool, pft_none),
			ParameterTypeData(String(UTF8("int")), 1,
				GL_INT, pvt_int, pft_none),
			ParameterTypeData(String(UTF8("ivec2")), 2,
				GL_INT_VEC2, pvt_int, pft_none),
			ParameterTypeData(String(UTF8("ivec3")), 3,
				GL_INT_VEC3, pvt_int, pft_none),
			ParameterTypeData(String(UTF8("ivec4")), 4,
				GL_INT_VEC4, pvt_int, pft_none),
			ParameterTypeData(String(UTF8("unsigned int")), 1,
				GL_UNSIGNED_INT, pvt_unsigned_int, pft_none),
			ParameterTypeData(String(UTF8("uvec2")), 2,
				GL_UNSIGNED_INT_VEC2, pvt_unsigned_int,
				pft_none),
			ParameterTypeData(String(UTF8("uvec3")), 3,
				GL_UNSIGNED_INT_VEC3, pvt_unsigned_int,
				pft_none),
			ParameterTypeData(String(UTF8("uvec4")), 4,
				GL_UNSIGNED_INT_VEC4, pvt_unsigned_int,
				pft_none),
			ParameterTypeData(String(UTF8("float")), 1,
				GL_FLOAT, pvt_float, pft_none),
			ParameterTypeData(String(UTF8("vec2")), 2,
				GL_FLOAT_VEC2, pvt_float, pft_none),
			ParameterTypeData(String(UTF8("vec3")), 3,
				GL_FLOAT_VEC3, pvt_float, pft_none),
			ParameterTypeData(String(UTF8("vec4")), 4,
				GL_FLOAT_VEC4, pvt_float, pft_none),
			ParameterTypeData(String(UTF8("mat2")), 4,
				GL_FLOAT_MAT2, pvt_float, pft_matrix),
			ParameterTypeData(String(UTF8("mat2x3")), 6,
				GL_FLOAT_MAT2x3, pvt_float, pft_matrix),
			ParameterTypeData(String(UTF8("mat2x4")), 8,
				GL_FLOAT_MAT2x4, pvt_float, pft_matrix),
			ParameterTypeData(String(UTF8("mat3x2")), 6,
				GL_FLOAT_MAT3x2, pvt_float, pft_matrix),
			ParameterTypeData(String(UTF8("mat3")), 9,
				GL_FLOAT_MAT3, pvt_float, pft_matrix),
			ParameterTypeData(String(UTF8("mat3x4")), 12,
				GL_FLOAT_MAT3x4, pvt_float, pft_matrix),
			ParameterTypeData(String(UTF8("mat4x2")), 8,
				GL_FLOAT_MAT4x2, pvt_float, pft_matrix),
			ParameterTypeData(String(UTF8("mat4x3")), 12,
				GL_FLOAT_MAT4x3, pvt_float, pft_matrix),
			ParameterTypeData(String(UTF8("mat4")), 16,
				GL_FLOAT_MAT4, pvt_float, pft_matrix),
			ParameterTypeData(String(UTF8("sampler1D")),
				GL_SAMPLER_1D, pvt_float, pft_none),
			ParameterTypeData(String(UTF8("sampler2D")),
				GL_SAMPLER_2D, pvt_float, pft_none),
			ParameterTypeData(String(UTF8("samplerCube")),
				GL_SAMPLER_CUBE, pvt_float, pft_none),
			ParameterTypeData(String(UTF8("sampler3D")),
				GL_SAMPLER_3D, pvt_float, pft_none),
			ParameterTypeData(String(UTF8("sampler1DShadow")),
				GL_SAMPLER_1D_SHADOW, pvt_shadow, pft_none),
			ParameterTypeData(String(UTF8("sampler2DShadow")),
				GL_SAMPLER_2D_SHADOW, pvt_shadow, pft_none),
			ParameterTypeData(String(UTF8("sampler2DRect")),
				GL_SAMPLER_2D_RECT, pvt_float, pft_rect),
			ParameterTypeData(String(UTF8("sampler2DRectShadow")),
				GL_SAMPLER_2D_RECT_SHADOW, pvt_shadow,
				pft_rect),
			ParameterTypeData(String(UTF8("sampler1DArray")),
				GL_SAMPLER_1D_ARRAY, pvt_float, pft_array),
			ParameterTypeData(String(UTF8("sampler2DArray")),
				GL_SAMPLER_2D_ARRAY, pvt_float, pft_array),
			ParameterTypeData(String(UTF8("sampler1DArrayShadow")),
				GL_SAMPLER_1D_ARRAY_SHADOW, pvt_shadow,
				pft_array),
			ParameterTypeData(String(UTF8("sampler2DArrayShadow")),
				GL_SAMPLER_2D_ARRAY_SHADOW, pvt_shadow,
				pft_array),
			ParameterTypeData(String(UTF8("samplerCubeShadow")),
				GL_SAMPLER_CUBE_SHADOW, pvt_shadow,
				pft_none),
			ParameterTypeData(String(UTF8("samplerBuffer")),
				GL_SAMPLER_BUFFER, pvt_float, pft_buffer),
			ParameterTypeData(String(UTF8("isampler1D")),
				GL_INT_SAMPLER_1D, pvt_int, pft_none),
			ParameterTypeData(String(UTF8("isampler2D")),
				GL_INT_SAMPLER_2D, pvt_int, pft_none),
			ParameterTypeData(String(UTF8("isamplerCube")),
				GL_INT_SAMPLER_CUBE, pvt_int, pft_none),
			ParameterTypeData(String(UTF8("isampler3D")),
				GL_INT_SAMPLER_3D, pvt_int, pft_none),
			ParameterTypeData(String(UTF8("isampler1DArray")),
				GL_INT_SAMPLER_1D_ARRAY, pvt_int, pft_array),
			ParameterTypeData(String(UTF8("isampler2DArray")),
				GL_INT_SAMPLER_2D_ARRAY, pvt_int, pft_array),
			ParameterTypeData(String(UTF8("isampler2DRect")),
				GL_INT_SAMPLER_2D_RECT, pvt_int, pft_rect),
			ParameterTypeData(String(UTF8("isamplerBuffer")),
				GL_INT_SAMPLER_BUFFER, pvt_int, pft_buffer),
			ParameterTypeData(String(UTF8("usampler1D")),
				GL_UNSIGNED_INT_SAMPLER_1D, pvt_unsigned_int,
				pft_none),
			ParameterTypeData(String(UTF8("usampler2D")),
				GL_UNSIGNED_INT_SAMPLER_2D, pvt_unsigned_int,
				pft_none),
			ParameterTypeData(String(UTF8("usamplerCube")),
				GL_UNSIGNED_INT_SAMPLER_CUBE, pvt_unsigned_int,
				pft_none),
			ParameterTypeData(String(UTF8("usampler3D")),
				GL_UNSIGNED_INT_SAMPLER_3D, pvt_unsigned_int,
				pft_none),
			ParameterTypeData(String(UTF8("usampler1DArray")),
				GL_UNSIGNED_INT_SAMPLER_1D_ARRAY,
				pvt_unsigned_int, pft_array),
			ParameterTypeData(String(UTF8("usampler2DArray")),
				GL_UNSIGNED_INT_SAMPLER_2D_ARRAY,
				pvt_unsigned_int, pft_array),
			ParameterTypeData(String(UTF8("usampler2DRect")),
				GL_UNSIGNED_INT_SAMPLER_2D_RECT,
				pvt_unsigned_int, pft_rect),
			ParameterTypeData(String(UTF8("usamplerBuffer")),
				GL_UNSIGNED_INT_SAMPLER_BUFFER,
				pvt_unsigned_int, pft_buffer)
		};

		const Uint32 parameter_datas_count =
			sizeof(parameter_datas) /
			sizeof(ParameterTypeData);

	}

	Uint32 ParameterUtil::get_count(const ParameterType parameter)
	{
		if (parameter_datas_count <= parameter)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					parameter_datas_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					parameter))
				<< boost::errinfo_type_info_name(UTF8(
					"ParameterType")));
		}

		return parameter_datas[parameter].get_count();
	}

	bool ParameterUtil::get_sampler(const ParameterType parameter)
	{
		if (parameter_datas_count <= parameter)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					parameter_datas_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					parameter))
				<< boost::errinfo_type_info_name(UTF8(
					"ParameterType")));
		}

		return parameter_datas[parameter].get_sampler();
	}

	bool ParameterUtil::get_int(const ParameterType parameter)
	{
		if (parameter_datas_count <= parameter)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					parameter_datas_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					parameter))
				<< boost::errinfo_type_info_name(UTF8(
					"ParameterType")));
		}

		return parameter_datas[parameter].get_value_type() ==
			pvt_int;
	}

	bool ParameterUtil::get_uint(const ParameterType parameter)
	{
		if (parameter_datas_count <= parameter)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					parameter_datas_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					parameter))
				<< boost::errinfo_type_info_name(UTF8(
					"ParameterType")));
		}

		return parameter_datas[parameter].get_value_type() ==
			pvt_unsigned_int;
	}

	bool ParameterUtil::get_bool(const ParameterType parameter)
	{
		if (parameter_datas_count <= parameter)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					parameter_datas_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					parameter))
				<< boost::errinfo_type_info_name(UTF8(
					"ParameterType")));
		}

		return parameter_datas[parameter].get_value_type() ==
			pvt_bool;
	}

	bool ParameterUtil::get_float(const ParameterType parameter)
	{
		if (parameter_datas_count <= parameter)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					parameter_datas_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					parameter))
				<< boost::errinfo_type_info_name(UTF8(
					"ParameterType")));
		}

		return parameter_datas[parameter].get_value_type() ==
			pvt_float;
	}

	bool ParameterUtil::get_matrix(const ParameterType parameter)
	{
		if (parameter_datas_count <= parameter)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					parameter_datas_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					parameter))
				<< boost::errinfo_type_info_name(UTF8(
					"ParameterType")));
		}

		return parameter_datas[parameter].get_format_type() ==
			pft_matrix;
	}

	GLenum ParameterUtil::get_gl(const ParameterType parameter)
	{
		if (parameter_datas_count <= parameter)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					parameter_datas_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					parameter))
				<< boost::errinfo_type_info_name(UTF8(
					"ParameterType")));
		}

		return parameter_datas[parameter].get_gl_type();
	}

	const String &ParameterUtil::get_str(const ParameterType parameter)
	{
		if (parameter_datas_count <= parameter)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					parameter_datas_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					parameter))
				<< boost::errinfo_type_info_name(UTF8(
					"ParameterType")));
		}

		return parameter_datas[parameter].get_name();
	}

	ParameterType ParameterUtil::get_parameter(const GLenum parameter)
	{
		Uint32 i;
		ParameterType parameter_type;

		for (i = 0; i < parameter_datas_count; ++i)
		{
			parameter_type = static_cast<ParameterType>(i);

			if (parameter == get_gl(parameter_type))
			{
				return parameter_type;
			}
		}

		EL_THROW_EXCEPTION(InvalidParameterException()
			<< errinfo_value(parameter)
			<< boost::errinfo_type_info_name(UTF8(
				"ParameterType")));
	}

	ParameterType ParameterUtil::get_parameter(const String &str)
	{
		Uint32 i;
		ParameterType parameter;

		for (i = 0; i < parameter_datas_count; ++i)
		{
			parameter = static_cast<ParameterType>(i);

			if (str == get_str(parameter))
			{
				return parameter;
			}
		}

		EL_THROW_EXCEPTION(InvalidParameterException()
			<< errinfo_string_value(str)
			<< boost::errinfo_type_info_name(UTF8(
				"ParameterType")));
	}

	bool ParameterUtil::get_parameter(const String &str,
		ParameterType &parameter) noexcept
	{
		Uint32 i;
		ParameterType tmp;

		for (i = 0; i < parameter_datas_count; ++i)
		{
			tmp = static_cast<ParameterType>(i);

			if (str == get_str(tmp))
			{
				parameter = tmp;

				return true;
			}
		}

		return false;
	}

	Uint32 ParameterUtil::get_parameter_count() noexcept
	{
		return parameter_datas_count;
	}

	OutStream& operator<<(OutStream &str, const ParameterType value)
	{
		str << ParameterUtil::get_str(value);

		return str;
	}

	InStream& operator>>(InStream &str, ParameterType &value)
	{
		StringType string;

		str >> string;

		value = ParameterUtil::get_parameter(String(string));

		return str;
	}

}
