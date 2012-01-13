/****************************************************************************
 *            autoparameterutil.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "autoparameterutil.hpp"
#include "exceptions.hpp"

namespace eternal_lands
{


	namespace
	{

		class AutoParameterTypeData
		{
			private:
				const String m_name;
				const ParameterType m_type;
				const ParameterSizeType m_size;
				const Uint16 m_scale;

			public:
				inline AutoParameterTypeData(
					const String &name,
					const ParameterType type,
					const ParameterSizeType size = pst_one,
					const Uint16 scale = 1):
						m_name(name), m_type(type),
						m_size(size),
						m_scale(scale)
				{
				}

				inline ~AutoParameterTypeData() throw()
				{
				}

				inline const String &get_name() const
				{
					return m_name;
				}

				inline ParameterType get_type() const
				{
					return m_type;
				}

				inline ParameterSizeType get_size() const
				{
					return m_size;
				}

				inline Uint16 get_scale() const
				{
					return m_scale;
				}

		};

		const AutoParameterTypeData auto_parameter_type_datas[] =
		{
			AutoParameterTypeData(String(UTF8("world_matrix")),
				pt_mat4x3),
			AutoParameterTypeData(String(UTF8("view_matrix")),
				pt_mat4x4),
			AutoParameterTypeData(String(UTF8("projection_matrix")),
				pt_mat4x4),
			AutoParameterTypeData(String(UTF8(
				"projection_view_matrix")), pt_mat4x4),
			AutoParameterTypeData(String(UTF8(
				"projection_view_matrices")), pt_mat4x4,
				pst_layer_count),
			AutoParameterTypeData(String(UTF8("reflection_matrix")),
				pt_mat4x4),
			AutoParameterTypeData(String(UTF8(
				"shadow_texture_matrix")), pt_mat4x4,
				pst_shadow_map_count),
			AutoParameterTypeData(String(UTF8("light_positions")),
				pt_vec4, pst_light_count),
			AutoParameterTypeData(String(UTF8("light_colors")),
				pt_vec4, pst_light_count),
			AutoParameterTypeData(String(UTF8("ambient")), pt_vec4),
			AutoParameterTypeData(String(UTF8(
				"dynamic_light_count")), pt_ivec3),
			AutoParameterTypeData(String(UTF8("bones")), pt_vec4,
				pst_bone_count, 3),
			AutoParameterTypeData(String(UTF8("time")), pt_float),
			AutoParameterTypeData(String(UTF8("fog_data")),
				pt_vec4),
			AutoParameterTypeData(String(UTF8("camera")), pt_vec4),
			AutoParameterTypeData(String(UTF8("shadow_camera")),
				pt_vec4),
			AutoParameterTypeData(String(UTF8("split_distances")),
				pt_vec4),
			AutoParameterTypeData(String(UTF8("layers")), pt_uvec4)
		};

		const Uint32 auto_parameter_type_datas_count =
			sizeof(auto_parameter_type_datas) /
			sizeof(AutoParameterTypeData);

	}

	const String &AutoParameterUtil::get_str(
		const AutoParameterType auto_parameter)
	{
		if (auto_parameter_type_datas_count <= auto_parameter)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					auto_parameter_type_datas_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					auto_parameter))
				<< boost::errinfo_type_info_name(UTF8(
					"AutoParameterType")));
		}

		return auto_parameter_type_datas[auto_parameter].get_name();
	}

	ParameterType AutoParameterUtil::get_type(
		const AutoParameterType auto_parameter)
	{
		if (auto_parameter_type_datas_count <= auto_parameter)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					auto_parameter_type_datas_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					auto_parameter))
				<< boost::errinfo_type_info_name(UTF8(
					"AutoParameterType")));
		}

		return auto_parameter_type_datas[auto_parameter].get_type();
	}

	ParameterSizeType AutoParameterUtil::get_size(
		const AutoParameterType auto_parameter)
	{
		if (auto_parameter_type_datas_count <= auto_parameter)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					auto_parameter_type_datas_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					auto_parameter))
				<< boost::errinfo_type_info_name(UTF8(
					"AutoParameterType")));
		}

		return auto_parameter_type_datas[auto_parameter].get_size();
	}

	Uint16 AutoParameterUtil::get_scale(
		const AutoParameterType auto_parameter)
	{
		if (auto_parameter_type_datas_count <= auto_parameter)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					auto_parameter_type_datas_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					auto_parameter))
				<< boost::errinfo_type_info_name(UTF8(
					"AutoParameterType")));
		}

		return auto_parameter_type_datas[auto_parameter].get_scale();
	}

	AutoParameterType AutoParameterUtil::get_auto_parameter(
		const String &str)
	{
		Uint32 i;
		AutoParameterType auto_parameter;

		for (i = 0; i < auto_parameter_type_datas_count; ++i)
		{
			auto_parameter = static_cast<AutoParameterType>(i);

			if (str == get_str(auto_parameter))
			{
				return auto_parameter;
			}
		}

		EL_THROW_EXCEPTION(InvalidParameterException()
			<< errinfo_string_value(str)
			<< boost::errinfo_type_info_name(UTF8(
				"AutoParameterType")));
	}

	bool AutoParameterUtil::get_auto_parameter(const String &str,
		AutoParameterType &auto_parameter)
	{
		Uint32 i;
		AutoParameterType tmp;

		for (i = 0; i < auto_parameter_type_datas_count; ++i)
		{
			tmp = static_cast<AutoParameterType>(i);

			if (str == get_str(tmp))
			{
				auto_parameter = tmp;

				return true;
			}
		}

		return false;
	}

	Uint32 AutoParameterUtil::get_auto_parameter_count()
	{
		return auto_parameter_type_datas_count;
	}

	OutStream& operator<<(OutStream &str, const AutoParameterType value)
	{
		str << AutoParameterUtil::get_str(value);

		return str;
	}

	InStream& operator>>(InStream &str, AutoParameterType &value)
	{
		StringType string;

		str >> string;

		value = AutoParameterUtil::get_auto_parameter(String(string));

		return str;
	}

}
