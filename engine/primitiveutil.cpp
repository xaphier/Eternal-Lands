/****************************************************************************
 *            primitiveutil.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "primitiveutil.hpp"
#include "exceptions.hpp"

namespace eternal_lands
{

	namespace
	{

		class PrimitiveTypeData
		{
			private:
				const String m_name;
				const Uint32 m_scale;
				const Uint32 m_offset;

			public:
				inline PrimitiveTypeData(
					const String &name,
					const Uint32 scale,
					const Uint32 offset):
					m_name(name), m_scale(scale),
					m_offset(offset)
				{
					assert(get_scale() > 0);
				}

				inline ~PrimitiveTypeData() noexcept
				{
				}

				inline const String &get_name() const noexcept
				{
					return m_name;
				}

				inline Uint32 get_scale() const noexcept
				{
					return m_scale;
				}

				inline Uint32 get_offset() const noexcept
				{
					return m_offset;
				}

		};

		const PrimitiveTypeData primitive_datas[] =
		{
			PrimitiveTypeData(String(UTF8("points")), 1, 0),
			PrimitiveTypeData(String(UTF8("lines")), 2, 0),
			PrimitiveTypeData(String(UTF8("triangles")), 3, 0),
			PrimitiveTypeData(String(UTF8("triangle_strip")), 1, 2),
			PrimitiveTypeData(String(UTF8("triangle_fan")), 1, 2)
		};

		const Uint32 primitive_datas_count =
			sizeof(primitive_datas) /
			sizeof(PrimitiveTypeData);

	}

	Uint32 PrimitiveUtil::get_index_count(const PrimitiveType primitive,
		const Uint32 count)
	{
		if (primitive_datas_count <= primitive)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					primitive_datas_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					primitive))
				<< boost::errinfo_type_info_name(UTF8(
					"PrimitiveType")));
		}

		return primitive_datas[primitive].get_scale() * count +
			primitive_datas[primitive].get_offset();
	}

	String PrimitiveUtil::get_str(const PrimitiveType primitive)
	{
		if (primitive_datas_count <= primitive)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					primitive_datas_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					primitive))
				<< boost::errinfo_type_info_name(UTF8(
					"PrimitiveType")));
		}

		return primitive_datas[primitive].get_name();
	}

	PrimitiveType PrimitiveUtil::get_type(const String &str)
	{
		Uint32 i;
		PrimitiveType primitive;

		for (i = 0; i < primitive_datas_count; ++i)
		{
			primitive = static_cast<PrimitiveType>(i);

			if (str == get_str(primitive))
			{
				return primitive;
			}
		}

		EL_THROW_EXCEPTION(InvalidParameterException()
			<< errinfo_string_value(str)
			<< boost::errinfo_type_info_name(UTF8(
				"PrimitiveType")));
	}

	OutStream &operator<<(OutStream &str, const PrimitiveType value)
	{
		str << PrimitiveUtil::get_str(value);

		return str;
	}

	InStream &operator>>(InStream &str, PrimitiveType &value)
	{
		std::string string;

		str >> string;

		value = PrimitiveUtil::get_type(String(string));

		return str;
	}

}
