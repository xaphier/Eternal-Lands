/****************************************************************************
 *            primitiveutil.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
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

				inline ~PrimitiveTypeData() throw()
				{
				}

				inline const String &get_name() const
				{
					return m_name;
				}

				inline Uint32 get_scale() const
				{
					return m_scale;
				}

				inline Uint32 get_offset() const
				{
					return m_offset;
				}

		};

		const PrimitiveTypeData primitive_type_datas[] =
		{
			PrimitiveTypeData(String(L"points"), 1, 0),
			PrimitiveTypeData(String(L"lines"), 2, 0),
			PrimitiveTypeData(String(L"triangles"), 3, 0),
			PrimitiveTypeData(String(L"triangle_strip"), 1, 2),
			PrimitiveTypeData(String(L"triangle_fan"), 1, 2)
		};

		const Uint32 primitive_type_datas_count =
			sizeof(primitive_type_datas) /
			sizeof(PrimitiveTypeData);

	}

	Uint32 PrimitiveUtil::get_index_count(const PrimitiveType primitive,
		const Uint32 count)
	{
		if (primitive_type_datas_count <= primitive)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					primitive_type_datas_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					primitive))
				<< boost::errinfo_type_info_name(
					"PrimitiveType"));
		}

		return primitive_type_datas[primitive].get_scale() * count +
			primitive_type_datas[primitive].get_offset();
	}

	String PrimitiveUtil::get_str(const PrimitiveType primitive)
	{
		if (primitive_type_datas_count <= primitive)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					primitive_type_datas_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					primitive))
				<< boost::errinfo_type_info_name(
					"PrimitiveType"));
		}

		return primitive_type_datas[primitive].get_name();
	}

	PrimitiveType PrimitiveUtil::get_type(const String &str)
	{
		Uint32 i;
		PrimitiveType primitive_type;

		for (i = 0; i < primitive_type_datas_count; i++)
		{
			primitive_type = static_cast<PrimitiveType>(i);

			if (str == get_str(primitive_type))
			{
				return primitive_type;
			}
		}

		EL_THROW_EXCEPTION(InvalidParameterException()
			<< errinfo_string_value(str)
			<< boost::errinfo_type_info_name(
				"PrimitiveType"));
	}

	OutStream& operator<<(OutStream &str, const PrimitiveType value)
	{
		str << PrimitiveUtil::get_str(value);

		return str;
	}

	InStream& operator>>(InStream &str, PrimitiveType &value)
	{
		StringType string;

		str >> string;

		value = PrimitiveUtil::get_type(String(string));

		return str;
	}

}
