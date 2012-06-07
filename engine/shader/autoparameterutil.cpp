/****************************************************************************
 *            autoparameterutil.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
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
				const Uint16 m_size;

			public:
				inline AutoParameterTypeData(
					const String &name,
					const ParameterType type,
					const Uint16 size = 1): m_name(name),
						m_type(type), m_size(size)
				{
				}

				inline ~AutoParameterTypeData() noexcept
				{
				}

				inline const String &get_name() const noexcept
				{
					return m_name;
				}

				inline ParameterType get_type() const noexcept
				{
					return m_type;
				}

				inline Uint16 get_size() const noexcept
				{
					return m_size;
				}

		};

		const AutoParameterTypeData auto_parameter_datas[] =
		{
			AutoParameterTypeData(
				String(UTF8("world_transformation")),
				pt_mat2x4),
			AutoParameterTypeData(
				String(UTF8("view_rotation_matrix")),
				pt_mat3x3),
			AutoParameterTypeData(String(UTF8("view_matrix")),
				pt_mat4x4),
			AutoParameterTypeData(
				String(UTF8("projection_matrix")),
				pt_mat4x4),
			AutoParameterTypeData(String(UTF8(
				"projection_view_matrix")), pt_mat4x4),
			AutoParameterTypeData(String(UTF8("reflection_matrix")),
				pt_mat4x4),
			AutoParameterTypeData(String(UTF8(
				"shadow_texture_matrices")), pt_mat4x4, 3),
			AutoParameterTypeData(String(UTF8("light_positions")),
				pt_vec4, 8),
			AutoParameterTypeData(String(UTF8("light_colors")),
				pt_vec4, 8),
			AutoParameterTypeData(String(UTF8("ambient")), pt_vec4),
			AutoParameterTypeData(String(UTF8(
				"dynamic_light_count")), pt_int),
			AutoParameterTypeData(String(UTF8("bones")), pt_mat2x4,
				80),
			AutoParameterTypeData(String(UTF8("time")), pt_float),
			AutoParameterTypeData(String(UTF8("fog_data")),
				pt_vec4),
			AutoParameterTypeData(String(UTF8("camera")), pt_vec4),
			AutoParameterTypeData(
				String(UTF8("shadow_distance_transform")),
				pt_vec4),
			AutoParameterTypeData(String(UTF8("split_distances")),
				pt_vec4),
			AutoParameterTypeData(String(UTF8("layers")), pt_ivec4),
			AutoParameterTypeData(
				String(UTF8("texture_matrices")), pt_mat2x3, 2),
			AutoParameterTypeData(
				String(UTF8("albedo_scale_offsets")),
				pt_mat2x4, 4),
			AutoParameterTypeData(
				String(UTF8("emission_scale_offset")),
				pt_mat2x3),
			AutoParameterTypeData(
				String(UTF8("specular_scale_offset")), pt_vec4),
			AutoParameterTypeData(String(UTF8("dudv_scale")),
				pt_vec2),
			AutoParameterTypeData(
				String(UTF8("terrain_scale")), pt_vec4),
			AutoParameterTypeData(
				String(UTF8("terrain_texture_size")), pt_vec4),
			AutoParameterTypeData(String(UTF8("clipmap_matrices")),
				pt_mat2x3, 6),
			AutoParameterTypeData(String(UTF8("z_params")),
				pt_vec4),
			AutoParameterTypeData(String(UTF8("terrain_instances")),
				pt_mat2x4, 1000),
			AutoParameterTypeData(
				String(UTF8("terrain_lod_offset")), pt_vec4)
		};

		const Uint32 auto_parameter_datas_count =
			sizeof(auto_parameter_datas) /
			sizeof(AutoParameterTypeData);

	}

	const String &AutoParameterUtil::get_str(
		const AutoParameterType auto_parameter)
	{
		if (auto_parameter_datas_count <= auto_parameter)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					auto_parameter_datas_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					auto_parameter))
				<< boost::errinfo_type_info_name(UTF8(
					"AutoParameterType")));
		}

		return auto_parameter_datas[auto_parameter].get_name();
	}

	ParameterType AutoParameterUtil::get_type(
		const AutoParameterType auto_parameter)
	{
		if (auto_parameter_datas_count <= auto_parameter)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					auto_parameter_datas_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					auto_parameter))
				<< boost::errinfo_type_info_name(UTF8(
					"AutoParameterType")));
		}

		return auto_parameter_datas[auto_parameter].get_type();
	}

	Uint16 AutoParameterUtil::get_size(
		const AutoParameterType auto_parameter)
	{
		if (auto_parameter_datas_count <= auto_parameter)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					auto_parameter_datas_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					auto_parameter))
				<< boost::errinfo_type_info_name(UTF8(
					"AutoParameterType")));
		}

		return auto_parameter_datas[auto_parameter].get_size();
	}

	AutoParameterType AutoParameterUtil::get_auto_parameter(
		const String &str)
	{
		Uint32 i;
		AutoParameterType auto_parameter;

		for (i = 0; i < auto_parameter_datas_count; ++i)
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
		AutoParameterType &auto_parameter) noexcept
	{
		Uint32 i;
		AutoParameterType tmp;

		for (i = 0; i < auto_parameter_datas_count; ++i)
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

	Uint32 AutoParameterUtil::get_auto_parameter_count() noexcept
	{
		return auto_parameter_datas_count;
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
