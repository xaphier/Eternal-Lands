/****************************************************************************
 *            variantutil.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "variantutil.hpp"
#include "glmutil.hpp"
#include "xmlwriter.hpp"
#include "exceptions.hpp"

namespace eternal_lands
{

	namespace
	{

		class VariantParameterTypes: public boost::static_visitor<>
		{
			private:
				ParameterType &m_type;

			public:
				VariantParameterTypes(ParameterType &type):
					m_type(type)
				{
				}

				~VariantParameterTypes() noexcept
				{ 
				}

				void operator()(const bool value) const
					noexcept
				{
					m_type = pt_bool;
				}

				void operator()(const float value) const
					noexcept
				{
					m_type = pt_float;
				}

				void operator()(const Sint64 value) const
					noexcept
				{
					m_type = pt_int;
				}

				void operator()(const glm::vec2 &value) const
					noexcept
				{
					m_type = pt_vec2;
				}

				void operator()(const glm::vec3 &value) const
					noexcept
				{
					m_type = pt_vec3;
				}

				void operator()(const glm::vec4 &value) const
					noexcept
				{
					m_type = pt_vec4;
				}

				void operator()(const glm::ivec2 &value) const
					noexcept
				{
					m_type = pt_ivec2;
				}

				void operator()(const glm::ivec3 &value) const
					noexcept
				{
					m_type = pt_ivec3;
				}

				void operator()(const glm::ivec4 &value) const
					noexcept
				{
					m_type = pt_ivec4;
				}

				void operator()(const glm::uvec2 &value) const
					noexcept
				{
					m_type = pt_uvec2;
				}

				void operator()(const glm::uvec3 &value) const
					noexcept
				{
					m_type = pt_uvec3;
				}

				void operator()(const glm::uvec4 &value) const
					noexcept
				{
					m_type = pt_uvec4;
				}

				void operator()(const glm::bvec2 &value) const
					noexcept
				{
					m_type = pt_bvec2;
				}

				void operator()(const glm::bvec3 &value) const
					noexcept
				{
					m_type = pt_bvec3;
				}

				void operator()(const glm::bvec4 &value) const
					noexcept
				{
					m_type = pt_bvec4;
				}

				void operator()(const glm::mat2x2 &value) const
					noexcept
				{
					m_type = pt_mat2x2;
				}

				void operator()(const glm::mat2x3 &value) const
					noexcept
				{
					m_type = pt_mat2x3;
				}

				void operator()(const glm::mat2x4 &value) const
					noexcept
				{
					m_type = pt_mat2x4;
				}

				void operator()(const glm::mat3x2 &value) const
					noexcept
				{
					m_type = pt_mat3x2;
				}

				void operator()(const glm::mat3x3 &value) const
					noexcept
				{
					m_type = pt_mat3x3;
				}

				void operator()(const glm::mat3x4 &value) const
					noexcept
				{
					m_type = pt_mat3x4;
				}

				void operator()(const glm::mat4x2 &value) const
					noexcept
				{
					m_type = pt_mat4x2;
				}

				void operator()(const glm::mat4x3 &value) const
					noexcept
				{
					m_type = pt_mat4x3;
				}

				void operator()(const glm::mat4x4 &value) const
					noexcept
				{
					m_type = pt_mat4x4;
				}

				void operator()(const Ivec4Vector &value) const
					noexcept
				{
					m_type = pt_ivec4;
				}

				void operator()(const Uvec4Vector &value) const
					noexcept
				{
					m_type = pt_uvec4;
				}

				void operator()(const Vec4Vector &value) const
					noexcept
				{
					m_type = pt_vec4;
				}

				void operator()(const Mat2x4Vector &value) const
					noexcept
				{
					m_type = pt_mat2x4;
				}

				void operator()(const Mat3x4Vector &value) const
					noexcept
				{
					m_type = pt_mat3x4;
				}

				void operator()(const Mat4x4Vector &value) const
					noexcept
				{
					m_type = pt_mat4x4;
				}

		};

		class VariantVectorSize: public boost::static_visitor<>
		{
			private:
				Uint32 &m_size;

			public:
				VariantVectorSize(Uint32 &size): m_size(size)
				{
					m_size = 0;
				}

				~VariantVectorSize() noexcept
				{
				}

				void operator()(const bool value) const
					noexcept
				{
					m_size = 1;
				}

				void operator()(const float value) const
					noexcept
				{
					m_size = 1;
				}

				void operator()(const Sint64 value) const
					noexcept
				{
					m_size = 1;
				}

				void operator()(const glm::vec2 &value) const
					noexcept
				{
					m_size = 1;
				}

				void operator()(const glm::vec3 &value) const
					noexcept
				{
					m_size = 1;
				}

				void operator()(const glm::vec4 &value) const
					noexcept
				{
					m_size = 1;
				}

				void operator()(const glm::ivec2 &value) const
					noexcept
				{
					m_size = 1;
				}

				void operator()(const glm::ivec3 &value) const
					noexcept
				{
					m_size = 1;
				}

				void operator()(const glm::ivec4 &value) const
					noexcept
				{
					m_size = 1;
				}

				void operator()(const glm::uvec2 &value) const
					noexcept
				{
					m_size = 1;
				}

				void operator()(const glm::uvec3 &value) const
					noexcept
				{
					m_size = 1;
				}

				void operator()(const glm::uvec4 &value) const
					noexcept
				{
					m_size = 1;
				}

				void operator()(const glm::bvec2 &value) const
					noexcept
				{
					m_size = 1;
				}

				void operator()(const glm::bvec3 &value) const
					noexcept
				{
					m_size = 1;
				}

				void operator()(const glm::bvec4 &value) const
					noexcept
				{
					m_size = 1;
				}

				void operator()(const glm::mat2x2 &value) const
					noexcept
				{
					m_size = 1;
				}

				void operator()(const glm::mat2x3 &value) const
					noexcept
				{
					m_size = 1;
				}

				void operator()(const glm::mat2x4 &value) const
					noexcept
				{
					m_size = 1;
				}

				void operator()(const glm::mat3x2 &value) const
					noexcept
				{
					m_size = 1;
				}

				void operator()(const glm::mat3x3 &value) const
					noexcept
				{
					m_size = 1;
				}

				void operator()(const glm::mat3x4 &value) const
					noexcept
				{
					m_size = 1;
				}

				void operator()(const glm::mat4x2 &value) const
					noexcept
				{
					m_size = 1;
				}

				void operator()(const glm::mat4x3 &value) const
					noexcept
				{
					m_size = 1;
				}

				void operator()(const glm::mat4x4 &value) const
					noexcept
				{
					m_size = 1;
				}

				void operator()(const Ivec4Vector &value) const
					noexcept
				{
					m_size = value.size();
				}

				void operator()(const Uvec4Vector &value) const
					noexcept
				{
					m_size = value.size();
				}

				void operator()(const Vec4Vector &value) const
					noexcept
				{
					m_size = value.size();
				}

				void operator()(const Mat2x4Vector &value) const
					noexcept
				{
					m_size = value.size();
				}

				void operator()(const Mat3x4Vector &value) const
					noexcept
				{
					m_size = value.size();
				}

				void operator()(const Mat4x4Vector &value) const
					noexcept
				{
					m_size = value.size();
				}

		};

		class VariantToOutStream: public boost::static_visitor<>
		{
			private:
				OutStream &m_str;

			public:
				VariantToOutStream(OutStream &str): m_str(str)
				{
					m_str << std::boolalpha;
				}

				~VariantToOutStream() noexcept
				{
				}

				void operator()(const bool value) const
					noexcept
				{
					m_str << value;
				}

				void operator()(const float value) const
					noexcept
				{
					m_str << value;
				}

				void operator()(const Sint64 value) const
					noexcept
				{
					m_str << value;
				}

				void operator()(const glm::vec2 &value) const
					noexcept
				{
					m_str << value;
				}

				void operator()(const glm::vec3 &value) const
					noexcept
				{
					m_str << value;
				}

				void operator()(const glm::vec4 &value) const
					noexcept
				{
					m_str << value;
				}

				void operator()(const glm::ivec2 &value) const
					noexcept
				{
					m_str << value;
				}

				void operator()(const glm::ivec3 &value) const
					noexcept
				{
					m_str << value;
				}

				void operator()(const glm::ivec4 &value) const
					noexcept
				{
					m_str << value;
				}

				void operator()(const glm::uvec2 &value) const
					noexcept
				{
					m_str << value;
				}

				void operator()(const glm::uvec3 &value) const
					noexcept
				{
					m_str << value;
				}

				void operator()(const glm::uvec4 &value) const
					noexcept
				{
					m_str << value;
				}

				void operator()(const glm::bvec2 &value) const
					noexcept
				{
					m_str << value;
				}

				void operator()(const glm::bvec3 &value) const
					noexcept
				{
					m_str << value;
				}

				void operator()(const glm::bvec4 &value) const
					noexcept
				{
					m_str << value;
				}

				void operator()(const glm::mat2x2 &value) const
					noexcept
				{
					m_str << value;
				}

				void operator()(const glm::mat2x3 &value) const
					noexcept
				{
					m_str << value;
				}

				void operator()(const glm::mat2x4 &value) const
					noexcept
				{
					m_str << value;
				}

				void operator()(const glm::mat3x2 &value) const
					noexcept
				{
					m_str << value;
				}

				void operator()(const glm::mat3x3 &value) const
					noexcept
				{
					m_str << value;
				}

				void operator()(const glm::mat3x4 &value) const
					noexcept
				{
					m_str << value;
				}

				void operator()(const glm::mat4x2 &value) const
					noexcept
				{
					m_str << value;
				}

				void operator()(const glm::mat4x3 &value) const
					noexcept
				{
					m_str << value;
				}

				void operator()(const glm::mat4x4 &value) const
					noexcept
				{
					m_str << value;
				}

				void operator()(const Ivec4Vector &value) const
					noexcept
				{
					Uint32 i, count;

					count = value.size();

					if (count == 0)
					{
						return;
					}

					m_str << value[0];

					for (i = 1; i < count; ++i)
					{
						m_str << L" " << value[i];
					}
				}

				void operator()(const Uvec4Vector &value) const
					noexcept
				{
					Uint32 i, count;

					count = value.size();

					if (count == 0)
					{
						return;
					}

					m_str << value[0];

					for (i = 1; i < count; ++i)
					{
						m_str << L" " << value[i];
					}
				}

				void operator()(const Vec4Vector &value) const
					noexcept
				{
					Uint32 i, count;

					count = value.size();

					if (count == 0)
					{
						return;
					}

					m_str << value[0];

					for (i = 1; i < count; ++i)
					{
						m_str << L" " << value[i];
					}
				}

				void operator()(const Mat2x4Vector &value) const
					noexcept
				{
					Uint32 i, count;

					count = value.size();

					if (count == 0)
					{
						return;
					}

					m_str << value[0];

					for (i = 1; i < count; ++i)
					{
						m_str << L" " << value[i];
					}
				}

				void operator()(const Mat3x4Vector &value) const
					noexcept
				{
					Uint32 i, count;

					count = value.size();

					if (count == 0)
					{
						return;
					}

					m_str << value[0];

					for (i = 1; i < count; ++i)
					{
						m_str << L" " << value[i];
					}
				}

				void operator()(const Mat4x4Vector &value) const
					noexcept
				{
					Uint32 i, count;

					count = value.size();

					if (count == 0)
					{
						return;
					}

					m_str << value[0];

					for (i = 1; i < count; ++i)
					{
						m_str << L" " << value[i];
					}
				}

		};

		template<typename T>
		T get_data(const String &values) noexcept
		{
			T result;
			StringStream str;

			str << std::boolalpha;
			str << values;

			str >> result;

			return result;
		}

	}

	ParameterType VariantUtil::get_parameter_type(const Variant &variant)
	{
		ParameterType result;

		boost::apply_visitor(VariantParameterTypes(result), variant);

		return result;
	}

	Uint32 VariantUtil::get_size(const Variant &variant)
	{
		Uint32 result;

		boost::apply_visitor(VariantVectorSize(result), variant);

		return result;
	}

	const String &VariantUtil::get_type_string(const Variant &variant)
	{
		return ParameterUtil::get_str(get_parameter_type(variant)); 
	}

	String VariantUtil::get_value_string(const Variant &variant)
	{
		StringStream result;

		boost::apply_visitor(VariantToOutStream(result), variant);

		return String(result.str());
	}

	Variant VariantUtil::get_variant(const ParameterType type,
		const String &values)
	{
		Variant result;

		switch (type)
		{
			case pt_bool:
				result = get_data<bool>(values);
				break;
			case pt_bvec2:
				result = get_data<glm::bvec2>(values);
				break;
			case pt_bvec3:
				result = get_data<glm::bvec3>(values);
				break;
			case pt_bvec4:
				result = get_data<glm::bvec4>(values);
				break;
			case pt_int:
				result = static_cast<Sint64>(get_data<Sint32>(
					values));
				break;
			case pt_ivec2:
				result = get_data<glm::ivec2>(values);
				break;
			case pt_ivec3:
				result = get_data<glm::ivec3>(values);
				break;
			case pt_ivec4:
				result = get_data<glm::ivec4>(values);
				break;
			case pt_unsigned_int:
				result = static_cast<Sint64>(get_data<Uint32>(
					values));
				break;
			case pt_uvec2:
				result = get_data<glm::uvec2>(values);
				break;
			case pt_uvec3:
				result = get_data<glm::uvec3>(values);
				break;
			case pt_uvec4:
				result = get_data<glm::uvec4>(values);
				break;
			case pt_float:
				result = get_data<float>(values);
				break;
			case pt_vec2:
				result = get_data<glm::vec2>(values);
				break;
			case pt_vec3:
				result = get_data<glm::vec3>(values);
				break;
			case pt_vec4:
				result = get_data<glm::vec4>(values);
				break;
			case pt_mat2x2:
				result = get_data<glm::mat2x2>(values);
				break;
			case pt_mat2x3:
				result = get_data<glm::mat2x3>(values);
				break;
			case pt_mat2x4:
				result = get_data<glm::mat2x4>(values);
				break;
			case pt_mat3x2:
				result = get_data<glm::mat3x2>(values);
				break;
			case pt_mat3x3:
				result = get_data<glm::mat3x3>(values);
				break;
			case pt_mat3x4:
				result = get_data<glm::mat3x4>(values);
				break;
			case pt_mat4x2:
				result = get_data<glm::mat4x2>(values);
				break;
			case pt_mat4x3:
				result = get_data<glm::mat4x3>(values);
				break;
			case pt_mat4x4:
				result = get_data<glm::mat4x4>(values);
				break;
			case pt_sampler1D:
			case pt_sampler2D:
			case pt_samplerCube:
			case pt_sampler3D:
			case pt_sampler1DShadow:
			case pt_sampler2DShadow:
			case pt_sampler2DRect:
			case pt_sampler2DRectShadow:
			case pt_sampler1DArray:
			case pt_sampler2DArray:
			case pt_sampler1DArrayShadow:
			case pt_sampler2DArrayShadow:
			case pt_samplerCubeShadow:
			case pt_samplerBuffer:
			case pt_isampler1D:
			case pt_isampler2D:
			case pt_isamplerCube:
			case pt_isampler3D:
			case pt_isampler1DArray:
			case pt_isampler2DArray:
			case pt_isampler2DRect:
			case pt_isamplerBuffer:
			case pt_usampler1D:
			case pt_usampler2D:
			case pt_usamplerCube:
			case pt_usampler3D:
			case pt_usampler1DArray:
			case pt_usampler2DArray:
			case pt_usampler2DRect:
			case pt_usamplerBuffer:
				break;
		}

		return result;
	}

	Variant VariantUtil::get_variant(const String &type,
		const String &values)
	{
		ParameterType parameter_type;

		parameter_type = ParameterUtil::get_parameter(type);

		return get_variant(parameter_type, values);
	}

	OutStream& operator<<(OutStream &str, const Variant &value)
	{
		str << L"Variant ";
		str << L" [type] " << VariantUtil::get_parameter_type(value);
		str << L" [size] " << VariantUtil::get_size(value);
		str << L" [values] " << VariantUtil::get_value_string(value);

		return str;
	}

}
