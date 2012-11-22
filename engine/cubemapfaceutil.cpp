/****************************************************************************
 *            cubemapfaceutil.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "cubemapfaceutil.hpp"
#include "exceptions.hpp"

namespace eternal_lands
{

	namespace
	{

		class CubeMapFaceTypeData
		{
			private:
				const String m_name;
				const GLenum m_gl_type;

			public:
				inline CubeMapFaceTypeData(const String &name,
					const GLenum gl_type): m_name(name),
					m_gl_type(gl_type)
				{
				}

				inline ~CubeMapFaceTypeData() noexcept
				{
				}

				inline const String &get_name() const noexcept
				{
					return m_name;
				}

				inline GLenum get_gl_type() const noexcept
				{
					return m_gl_type;
				}

		};

		const CubeMapFaceTypeData cube_map_face_datas[] =
		{
			CubeMapFaceTypeData(String(UTF8("negative_x")),
				GL_TEXTURE_CUBE_MAP_NEGATIVE_X),
			CubeMapFaceTypeData(String(UTF8("negative_y")),
				GL_TEXTURE_CUBE_MAP_NEGATIVE_Y),
			CubeMapFaceTypeData(String(UTF8("negative_z")),
				GL_TEXTURE_CUBE_MAP_NEGATIVE_Z),
			CubeMapFaceTypeData(String(UTF8("positive_x")),
				GL_TEXTURE_CUBE_MAP_POSITIVE_X),
			CubeMapFaceTypeData(String(UTF8("positive_y")),
				GL_TEXTURE_CUBE_MAP_POSITIVE_Y),
			CubeMapFaceTypeData(String(UTF8("positive_z")),
				GL_TEXTURE_CUBE_MAP_POSITIVE_Z)
		};

		const Uint32 cube_map_face_datas_count =
			sizeof(cube_map_face_datas) /
			sizeof(CubeMapFaceTypeData);

	}

	GLenum CubeMapFaceUtil::get_gl_type(
		const CubeMapFaceType cube_map_face)
	{
		if (cube_map_face_datas_count <= cube_map_face)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					cube_map_face_datas_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					cube_map_face))
				<< boost::errinfo_type_info_name(UTF8(
					"CubeMapFaceType")));
		}

		return cube_map_face_datas[cube_map_face].get_gl_type();
	}

	const String &CubeMapFaceUtil::get_str(
		const CubeMapFaceType cube_map_face)
	{
		if (cube_map_face_datas_count <= cube_map_face)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					cube_map_face_datas_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					cube_map_face))
				<< boost::errinfo_type_info_name(
					UTF8("CubeMapFaceType")));
		}

		return cube_map_face_datas[cube_map_face].get_name();
	}

	CubeMapFaceType CubeMapFaceUtil::get_cube_map_face(const String &str)
	{
		Uint32 i;
		CubeMapFaceType cube_map_face;

		for (i = 0; i < cube_map_face_datas_count; i++)
		{
			cube_map_face = static_cast<CubeMapFaceType>(i);

			if (str == get_str(cube_map_face))
			{
				return cube_map_face;
			}
		}

		EL_THROW_EXCEPTION(InvalidParameterException()
			<< errinfo_string_value(str)
			<< boost::errinfo_type_info_name(
				UTF8("CubeMapFaceType")));
	}

	Uint32 CubeMapFaceUtil::get_cube_map_face_count() noexcept
	{
		return cube_map_face_datas_count;
	}

	OutStream &operator<<(OutStream &str, const CubeMapFaceType value)
	{
		str << CubeMapFaceUtil::get_str(value);

		return str;
	}

	InStream &operator>>(InStream &str, CubeMapFaceType &value)
	{
		std::string string;

		str >> string;

		value = CubeMapFaceUtil::get_cube_map_face(String(string));

		return str;
	}

}
