/****************************************************************************
 *            texturetargetutil.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "texturetargetutil.hpp"
#include "exceptions.hpp"

namespace eternal_lands
{

	namespace
	{

		class TextureTargetTypeData
		{
			private:
				const String m_name;
				const GLenum m_gl_type;

			public:
				inline TextureTargetTypeData(
					const String &name,
					const GLenum gl_type):
					m_name(name), m_gl_type(gl_type)
				{
				}

				inline ~TextureTargetTypeData() throw()
				{
				}

				inline const String &get_name() const
				{
					return m_name;
				}

				inline GLenum get_gl_type() const
				{
					return m_gl_type;
				}

		};

		const TextureTargetTypeData texture_target_datas[] =
		{
			TextureTargetTypeData(String(UTF8("texture_1d")),
				GL_TEXTURE_1D),
			TextureTargetTypeData(String(UTF8("texture_2d")),
				GL_TEXTURE_2D),
			TextureTargetTypeData(String(UTF8("texture_3d")),
				GL_TEXTURE_3D),
			TextureTargetTypeData(String(UTF8("texture_cube_map")),
				GL_TEXTURE_CUBE_MAP),
			TextureTargetTypeData(String(UTF8("texture_1d_array")),
				GL_TEXTURE_1D_ARRAY),
			TextureTargetTypeData(String(UTF8("texture_2d_array")),
				GL_TEXTURE_2D_ARRAY),
			TextureTargetTypeData(
				String(UTF8("texture_cube_map_array")),
				GL_TEXTURE_CUBE_MAP_ARRAY),
			TextureTargetTypeData(String(UTF8("texture_rectangle")),
				GL_TEXTURE_RECTANGLE),
		};

		const Uint32 texture_target_datas_count =
			sizeof(texture_target_datas) /
			sizeof(TextureTargetTypeData);

	}

	GLenum TextureTargetUtil::get_gl_type(
		const TextureTargetType texture_target)
	{
		if (texture_target_datas_count <= texture_target)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					texture_target_datas_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					texture_target))
				<< boost::errinfo_type_info_name(UTF8(
					"TextureTargetType")));
		}

		return texture_target_datas[texture_target].get_gl_type();
	}

	const String &TextureTargetUtil::get_str(
		const TextureTargetType texture_target)
	{
		if (texture_target_datas_count <= texture_target)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					texture_target_datas_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					texture_target))
				<< boost::errinfo_type_info_name(
					UTF8("TextureTargetType")));
		}

		return texture_target_datas[texture_target].get_name();
	}

	TextureTargetType TextureTargetUtil::get_texture_target(
		const String &str)
	{
		Uint32 i;
		TextureTargetType texture_target;

		for (i = 0; i < texture_target_datas_count; i++)
		{
			texture_target = static_cast<TextureTargetType>(i);

			if (str == get_str(texture_target))
			{
				return texture_target;
			}
		}

		EL_THROW_EXCEPTION(InvalidParameterException()
			<< errinfo_string_value(str)
			<< boost::errinfo_type_info_name(
				UTF8("TextureTargetType")));
	}

	Uint32 TextureTargetUtil::get_texture_target_count()
	{
		return texture_target_datas_count;
	}

	OutStream& operator<<(OutStream &str, const TextureTargetType value)
	{
		str << TextureTargetUtil::get_str(value);

		return str;
	}

	InStream& operator>>(InStream &str, TextureTargetType &value)
	{
		StringType string;

		str >> string;

		value = TextureTargetUtil::get_texture_target(String(string));

		return str;
	}

}