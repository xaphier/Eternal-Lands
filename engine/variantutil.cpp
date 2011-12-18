/****************************************************************************
 *            variantutil.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
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

		class VariantTypeNames: public boost::static_visitor<>
		{
			private:
				String &m_name;

			public:
				VariantTypeNames(String &name): m_name(name)
				{
				}

				~VariantTypeNames() throw()
				{ 
				}

				void operator()(const bool value) const
				{
					m_name = String(UTF8("bool"));
				}

				void operator()(const float value) const
				{
					m_name = String(UTF8("float"));
				}

				void operator()(const Sint64 value) const
				{
					m_name = String(UTF8("int"));
				}

				void operator()(const glm::vec2 &value) const
				{
					m_name = String(UTF8("vec2"));
				}

				void operator()(const glm::vec3 &value) const
				{
					m_name = String(UTF8("vec3"));
				}

				void operator()(const glm::vec4 &value) const
				{
					m_name = String(UTF8("vec4"));
				}

				void operator()(const glm::ivec2 &value) const
				{
					m_name = String(UTF8("ivec2"));
				}

				void operator()(const glm::ivec3 &value) const
				{
					m_name = String(UTF8("ivec3"));
				}

				void operator()(const glm::ivec4 &value) const
				{
					m_name = String(UTF8("ivec4"));
				}

				void operator()(const glm::uvec2 &value) const
				{
					m_name = String(UTF8("uvec2"));
				}

				void operator()(const glm::uvec3 &value) const
				{
					m_name = String(UTF8("uvec3"));
				}

				void operator()(const glm::uvec4 &value) const
				{
					m_name = String(UTF8("uvec4"));
				}

				void operator()(const glm::bvec2 &value) const
				{
					m_name = String(UTF8("bvec2"));
				}

				void operator()(const glm::bvec3 &value) const
				{
					m_name = String(UTF8("bvec3"));
				}

				void operator()(const glm::bvec4 &value) const
				{
					m_name = String(UTF8("bvec4"));
				}

				void operator()(const glm::mat2x2 &value) const
				{
					m_name = String(UTF8("mat2x2"));
				}

				void operator()(const glm::mat2x3 &value) const
				{
					m_name = String(UTF8("mat2x3"));
				}

				void operator()(const glm::mat2x4 &value) const
				{
					m_name = String(UTF8("mat2x4"));
				}

				void operator()(const glm::mat3x2 &value) const
				{
					m_name = String(UTF8("mat3x2"));
				}

				void operator()(const glm::mat3x3 &value) const
				{
					m_name = String(UTF8("mat3x3"));
				}

				void operator()(const glm::mat3x4 &value) const
				{
					m_name = String(UTF8("mat3x4"));
				}

				void operator()(const glm::mat4x2 &value) const
				{
					m_name = String(UTF8("mat4x2"));
				}

				void operator()(const glm::mat4x3 &value) const
				{
					m_name = String(UTF8("mat4x3"));
				}

				void operator()(const glm::mat4x4 &value) const
				{
					m_name = String(UTF8("mat4x4"));
				}

				void operator()(const Ivec4Vector &value) const
				{
					m_name = String(UTF8("ivec4_vector"));
				}

				void operator()(const Uvec4Vector &value) const
				{
					m_name = String(UTF8("uvec4_vector"));
				}

				void operator()(const Vec4Vector &value) const
				{
					m_name = String(UTF8("vec4_vector"));
				}

				void operator()(const Mat2x4Vector &value) const
				{
					m_name = String(UTF8("mat2x4_vector"));
				}

				void operator()(const Mat3x4Vector &value) const
				{
					m_name = String(UTF8("mat3x4_vector"));
				}

				void operator()(const Mat4x4Vector &value) const
				{
					m_name = String(UTF8("mat4x4_vector"));
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

				~VariantVectorSize() throw()
				{
				}

				void operator()(const bool value) const
				{
					m_size = 1;
				}

				void operator()(const float value) const
				{
					m_size = 1;
				}

				void operator()(const Sint64 value) const
				{
					m_size = 1;
				}

				void operator()(const glm::vec2 &value) const
				{
					m_size = 1;
				}

				void operator()(const glm::vec3 &value) const
				{
					m_size = 1;
				}

				void operator()(const glm::vec4 &value) const
				{
					m_size = 1;
				}

				void operator()(const glm::ivec2 &value) const
				{
					m_size = 1;
				}

				void operator()(const glm::ivec3 &value) const
				{
					m_size = 1;
				}

				void operator()(const glm::ivec4 &value) const
				{
					m_size = 1;
				}

				void operator()(const glm::uvec2 &value) const
				{
					m_size = 1;
				}

				void operator()(const glm::uvec3 &value) const
				{
					m_size = 1;
				}

				void operator()(const glm::uvec4 &value) const
				{
					m_size = 1;
				}

				void operator()(const glm::bvec2 &value) const
				{
					m_size = 1;
				}

				void operator()(const glm::bvec3 &value) const
				{
					m_size = 1;
				}

				void operator()(const glm::bvec4 &value) const
				{
					m_size = 1;
				}

				void operator()(const glm::mat2x2 &value) const
				{
					m_size = 1;
				}

				void operator()(const glm::mat2x3 &value) const
				{
					m_size = 1;
				}

				void operator()(const glm::mat2x4 &value) const
				{
					m_size = 1;
				}

				void operator()(const glm::mat3x2 &value) const
				{
					m_size = 1;
				}

				void operator()(const glm::mat3x3 &value) const
				{
					m_size = 1;
				}

				void operator()(const glm::mat3x4 &value) const
				{
					m_size = 1;
				}

				void operator()(const glm::mat4x2 &value) const
				{
					m_size = 1;
				}

				void operator()(const glm::mat4x3 &value) const
				{
					m_size = 1;
				}

				void operator()(const glm::mat4x4 &value) const
				{
					m_size = 1;
				}

				void operator()(const Ivec4Vector &value) const
				{
					m_size = value.size();
				}

				void operator()(const Uvec4Vector &value) const
				{
					m_size = value.size();
				}

				void operator()(const Vec4Vector &value) const
				{
					m_size = value.size();
				}

				void operator()(const Mat2x4Vector &value) const
				{
					m_size = value.size();
				}

				void operator()(const Mat3x4Vector &value) const
				{
					m_size = value.size();
				}

				void operator()(const Mat4x4Vector &value) const
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

				~VariantToOutStream() throw()
				{
				}

				void operator()(const bool value) const
				{
					m_str << value;
				}

				void operator()(const float value) const
				{
					m_str << value;
				}

				void operator()(const Sint64 value) const
				{
					m_str << value;
				}

				void operator()(const glm::vec2 &value) const
				{
					m_str << value;
				}

				void operator()(const glm::vec3 &value) const
				{
					m_str << value;
				}

				void operator()(const glm::vec4 &value) const
				{
					m_str << value;
				}

				void operator()(const glm::ivec2 &value) const
				{
					m_str << value;
				}

				void operator()(const glm::ivec3 &value) const
				{
					m_str << value;
				}

				void operator()(const glm::ivec4 &value) const
				{
					m_str << value;
				}

				void operator()(const glm::uvec2 &value) const
				{
					m_str << value;
				}

				void operator()(const glm::uvec3 &value) const
				{
					m_str << value;
				}

				void operator()(const glm::uvec4 &value) const
				{
					m_str << value;
				}

				void operator()(const glm::bvec2 &value) const
				{
					m_str << value;
				}

				void operator()(const glm::bvec3 &value) const
				{
					m_str << value;
				}

				void operator()(const glm::bvec4 &value) const
				{
					m_str << value;
				}

				void operator()(const glm::mat2x2 &value) const
				{
					m_str << value;
				}

				void operator()(const glm::mat2x3 &value) const
				{
					m_str << value;
				}

				void operator()(const glm::mat2x4 &value) const
				{
					m_str << value;
				}

				void operator()(const glm::mat3x2 &value) const
				{
					m_str << value;
				}

				void operator()(const glm::mat3x3 &value) const
				{
					m_str << value;
				}

				void operator()(const glm::mat3x4 &value) const
				{
					m_str << value;
				}

				void operator()(const glm::mat4x2 &value) const
				{
					m_str << value;
				}

				void operator()(const glm::mat4x3 &value) const
				{
					m_str << value;
				}

				void operator()(const glm::mat4x4 &value) const
				{
					m_str << value;
				}

				void operator()(const Ivec4Vector &value) const
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

	}

	String VariantUtil::get_type_string(const Variant &variant)
	{
		String result;

		boost::apply_visitor(VariantTypeNames(result), variant);

		return result;
	}

	Uint32 VariantUtil::get_size(const Variant &variant)
	{
		Uint32 result;

		boost::apply_visitor(VariantVectorSize(result), variant);

		return result;
	}

	StringType VariantUtil::get_value_string(const Variant &variant)
	{
		StringStream result;

		boost::apply_visitor(VariantToOutStream(result), variant);

		return result.str();
	}

	OutStream& operator<<(OutStream &str, const Variant &value)
	{
		str << L"Variant ";
		str << L" [type] " << VariantUtil::get_type_string(value);
		str << L" [size] " << VariantUtil::get_size(value);
		str << L" [values] " << VariantUtil::get_value_string(value);

		return str;
	}

}
