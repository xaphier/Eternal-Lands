/****************************************************************************
 *            uniformbufferutil.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "uniformbufferutil.hpp"
#include "exceptions.hpp"

namespace eternal_lands
{

	namespace
	{

		class UniformBufferTypeData
		{
			private:
				const AutoParameterTypeVector m_auto_parameters;
				const String m_name;
				const String m_identifier;

			public:
				inline UniformBufferTypeData(
					const AutoParameterTypeVector
						&auto_parameters,
					const String &name,
					const String &identifier):
					m_auto_parameters(auto_parameters),
					m_name(name), m_identifier(identifier)
				{
				}

				inline ~UniformBufferTypeData() noexcept
				{
				}

				inline const AutoParameterTypeVector
					&get_auto_parameters() const noexcept
				{
					return m_auto_parameters;
				}

				inline const String &get_name() const noexcept
				{
					return m_name;
				}

				inline const String &get_identifier() const
					noexcept
				{
					return m_identifier;
				}

		};

		const UniformBufferTypeData uniform_buffer_type_datas[] =
		{
			UniformBufferTypeData(boost::assign::list_of
				(apt_view_rotation_matrix)
				(apt_view_matrix)
				(apt_projection_matrix)
				(apt_projection_view_matrix)
				(apt_reflection_matrix)
				(apt_ambient)
				(apt_time)
				(apt_fog_data)
				(apt_camera)
				(apt_z_params),
				String(UTF8("scene")),
				String(UTF8("Scene"))),
			UniformBufferTypeData(boost::assign::list_of
				(apt_texture_matrices)
				(apt_albedo_scale_offsets)
				(apt_emission_scale_offset)
				(apt_specular_scale_offset)
				(apt_dudv_scale),
				String(UTF8("material")),
				String(UTF8("Material"))),
			UniformBufferTypeData(boost::assign::list_of
				(apt_bones),
				String(UTF8("animation")),
				String(UTF8("Animation"))),
			UniformBufferTypeData(boost::assign::list_of
				(apt_shadow_distance_transform)
				(apt_shadow_texture_matrices)
				(apt_split_distances),
				String(UTF8("shadow")),
				String(UTF8("Shadow"))),
			UniformBufferTypeData(boost::assign::list_of
				(apt_terrain_scale)
				(apt_terrain_texture_size)
				(apt_clipmap_matrices),
				String(UTF8("terrain")),
				String(UTF8("Terrain"))),
			UniformBufferTypeData(boost::assign::list_of
				(apt_terrain_lod_offset)
				(apt_terrain_instances),
				String(UTF8("terrain_instances")),
				String(UTF8("TerrainInstances")))
		};

		const Uint32 uniform_buffer_type_datas_count =
			sizeof(uniform_buffer_type_datas) /
				sizeof(UniformBufferTypeData);

	}

	const String &UniformBufferUtil::get_identifier(
		const UniformBufferType uniform_buffer)
	{
		if (uniform_buffer_type_datas_count <= uniform_buffer)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					uniform_buffer_type_datas_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					uniform_buffer))
				<< boost::errinfo_type_info_name(UTF8(
					"UniformBufferType")));
		}

		return uniform_buffer_type_datas[
			uniform_buffer].get_identifier();
	}

	const String &UniformBufferUtil::get_str(
		const UniformBufferType uniform_buffer)
	{
		if (uniform_buffer_type_datas_count <= uniform_buffer)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					uniform_buffer_type_datas_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					uniform_buffer))
				<< boost::errinfo_type_info_name(UTF8(
					"UniformBufferType")));
		}

		return uniform_buffer_type_datas[uniform_buffer].get_name();
	}

	const AutoParameterTypeVector &UniformBufferUtil::get_auto_parameters(
		const UniformBufferType uniform_buffer)
	{
		if (uniform_buffer_type_datas_count <= uniform_buffer)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					uniform_buffer_type_datas_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					uniform_buffer))
				<< boost::errinfo_type_info_name(UTF8(
					"UniformBufferType")));
		}

		return uniform_buffer_type_datas[
			uniform_buffer].get_auto_parameters();
	}

	UniformBufferType UniformBufferUtil::get_uniform_buffer(
		const String &str)
	{
		Uint32 i;
		UniformBufferType uniform_buffer;

		for (i = 0; i < uniform_buffer_type_datas_count; ++i)
		{
			uniform_buffer = static_cast<UniformBufferType>(i);

			if (str == get_str(uniform_buffer))
			{
				return uniform_buffer;
			}
		}

		EL_THROW_EXCEPTION(InvalidParameterException()
			<< errinfo_string_value(str)
			<< boost::errinfo_type_info_name(UTF8(
				"UniformBufferType")));
	}

	UniformBufferType UniformBufferUtil::get_uniform_buffer_from_identifier(
		const String &identifier)
	{
		Uint32 i;
		UniformBufferType uniform_buffer;

		for (i = 0; i < uniform_buffer_type_datas_count; ++i)
		{
			uniform_buffer = static_cast<UniformBufferType>(i);

			if (identifier == get_identifier(uniform_buffer))
			{
				return uniform_buffer;
			}
		}

		EL_THROW_EXCEPTION(InvalidParameterException()
			<< errinfo_string_value(identifier)
			<< boost::errinfo_type_info_name(UTF8(
				"UniformBufferType")));
	}

	bool UniformBufferUtil::get_uniform_buffer(const String &str,
		UniformBufferType &uniform_buffer) noexcept
	{
		Uint32 i;
		UniformBufferType tmp;

		for (i = 0; i < uniform_buffer_type_datas_count; ++i)
		{
			tmp = static_cast<UniformBufferType>(i);

			if (str == get_str(tmp))
			{
				uniform_buffer = tmp;

				return true;
			}
		}

		return false;
	}

	bool UniformBufferUtil::get_uniform_buffer_from_identifier(
		const String &identifier, UniformBufferType &uniform_buffer)
		noexcept
	{
		Uint32 i;
		UniformBufferType tmp;

		for (i = 0; i < uniform_buffer_type_datas_count; ++i)
		{
			tmp = static_cast<UniformBufferType>(i);

			if (identifier == get_identifier(tmp))
			{
				uniform_buffer = tmp;

				return true;
			}
		}

		return false;
	}

	Uint32 UniformBufferUtil::get_uniform_buffer_count() noexcept
	{
		return uniform_buffer_type_datas_count;
	}

	OutStream& operator<<(OutStream &str, const UniformBufferType value)
	{
		str << UniformBufferUtil::get_str(value);

		return str;
	}

	InStream& operator>>(InStream &str, UniformBufferType &value)
	{
		StringType string;

		str >> string;

		value = UniformBufferUtil::get_uniform_buffer(String(string));

		return str;
	}

}

