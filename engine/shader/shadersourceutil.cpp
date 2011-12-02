/****************************************************************************
 *            shadersourceutil.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "shadersourceutil.hpp"
#include "commonparameterutil.hpp"
#include "parameterqualifierutil.hpp"
#include "exceptions.hpp"

namespace eternal_lands
{

	namespace
	{

		typedef std::map<CommonParameterType, ParameterQualifierType>
			CommonParameters;

		class ShaderSourceTypeData
		{
			private:
				const String m_name;
				const CommonParameters m_parameters;

			public:
				inline ShaderSourceTypeData(
					const String &name,
					const CommonParameters parameters):
						m_name(name),
						m_parameters(parameters)
				{
				}

				inline ~ShaderSourceTypeData() throw()
				{
				}

				inline const String &get_name() const
				{
					return m_name;
				}

				inline CommonParameters get_parameters() const
				{
					return m_parameters;
				}

		};

		const ShaderSourceTypeData shader_source_type_datas[] =
		{
			ShaderSourceTypeData(String(L"world_depth_transform"),
				boost::assign::map_list_of
					(cpt_world_position, pqt_out)),
			ShaderSourceTypeData(String(L"world_normal_transform"),
				boost::assign::map_list_of
					(cpt_world_position, pqt_out)
					(cpt_world_normal, pqt_out)),
			ShaderSourceTypeData(String(L"world_tangent_transform"),
				boost::assign::map_list_of
					(cpt_world_position, pqt_out)
					(cpt_world_normal, pqt_out)
					(cpt_world_tangent, pqt_out)),
			ShaderSourceTypeData(String(L"view_direction"),
				boost::assign::map_list_of
					(cpt_world_position, pqt_in)
					(cpt_world_view_direction, pqt_out)),
			ShaderSourceTypeData(String(L"view_transform"),
				boost::assign::map_list_of
					(cpt_world_position, pqt_in)
					(cpt_view_position, pqt_out)),
			ShaderSourceTypeData(String(L"fog"),
				boost::assign::map_list_of
					(cpt_view_position, pqt_in)
					(cpt_fog, pqt_out)),
			ShaderSourceTypeData(String(L"light"),
				boost::assign::map_list_of
					(cpt_light_color, pqt_in)
					(cpt_light_position, pqt_in)
					(cpt_lighting_normal, pqt_in)
					(cpt_world_position, pqt_in)
					(cpt_diffuse_color, pqt_out)
					(cpt_specular_color, pqt_out)),
			ShaderSourceTypeData(String(L"uv"),
				boost::assign::map_list_of
					(cpt_world_uv, pqt_out)),
			ShaderSourceTypeData(String(L"shadow_uv"),
				boost::assign::map_list_of
					(cpt_shadow_uv, pqt_out)),
			ShaderSourceTypeData(String(L"shadow_mapping"),
				boost::assign::map_list_of
					(cpt_shadow_uv, pqt_in)
					(cpt_shadow, pqt_out)),
			ShaderSourceTypeData(String(L"normal_mapping"),
				boost::assign::map_list_of
					(cpt_world_uv, pqt_in)
					(cpt_world_normal, pqt_in)
					(cpt_fragment_uv, pqt_out)
					(cpt_fragment_normal, pqt_out)),
			ShaderSourceTypeData(String(L"normal_depth_mapping"),
				boost::assign::map_list_of
					(cpt_world_uv, pqt_in)
					(cpt_fragment_uv, pqt_out)),
			ShaderSourceTypeData(String(L"diffuse_mapping"),
				boost::assign::map_list_of
					(cpt_fragment_uv, pqt_in)
					(cpt_diffuse, pqt_out)),
			ShaderSourceTypeData(String(L"specular_mapping"),
				boost::assign::map_list_of
					(cpt_fragment_uv, pqt_in)
					(cpt_specular, pqt_out)),
			ShaderSourceTypeData(String(L"transparent"),
				boost::assign::map_list_of
					(cpt_diffuse, pqt_in)),
			ShaderSourceTypeData(String(L"shadow_map"),
				boost::assign::map_list_of
					(cpt_shadow_map_data, pqt_out))
		};

		const Uint32 shader_source_type_datas_count =
			sizeof(shader_source_type_datas) /
			sizeof(ShaderSourceTypeData);

	}

	const String &ShaderSourceUtil::get_str(const ShaderSourceType source)
	{
		if (shader_source_type_datas_count <= source)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					shader_source_type_datas_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					source))
				<< boost::errinfo_type_info_name(
					"ShaderSourceType"));
		}

		return shader_source_type_datas[source].get_name();
	}

	ShaderSourceType ShaderSourceUtil::get_shader_source(const String &str)
	{
		Uint32 i;
		ShaderSourceType shader_source_type;

		for (i = 0; i < shader_source_type_datas_count; i++)
		{
			shader_source_type =
				static_cast<ShaderSourceType>(i);

			if (str == get_str(shader_source_type))
			{
				return shader_source_type;
			}
		}

		EL_THROW_EXCEPTION(InvalidParameterException()
			<< errinfo_string_value(str)
			<< boost::errinfo_type_info_name("ShaderSourceType"));
	}

	Uint32 ShaderSourceUtil::get_shader_source_count()
	{
		return shader_source_type_datas_count;
	}

	OutStream& operator<<(OutStream &str, const ShaderSourceType value)
	{
		str << ShaderSourceUtil::get_str(value);

		return str;
	}

	InStream& operator>>(InStream &str, ShaderSourceType &value)
	{
		StringType string;

		str >> string;

		value = ShaderSourceUtil::get_shader_source(String(string));

		return str;
	}

}
