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
				const String m_description;
				const GLenum m_gl_type;
				const TextureTargetType m_depth_texture_target;

			public:
				inline TextureTargetTypeData(
					const String &name,
					const String &description,
					const GLenum gl_type,
					const TextureTargetType
						depth_texture_target):
					m_name(name),
					m_description(description),
					m_gl_type(gl_type),
					m_depth_texture_target(
						depth_texture_target)
				{
				}

				inline ~TextureTargetTypeData() noexcept
				{
				}

				inline const String &get_name() const noexcept
				{
					return m_name;
				}

				inline const String &get_description() const
					noexcept
				{
					return m_description;
				}

				inline GLenum get_gl_type() const noexcept
				{
					return m_gl_type;
				}

				inline TextureTargetType
					get_depth_texture_target() const
					noexcept
				{
					return m_depth_texture_target;
				}

		};

		const TextureTargetTypeData texture_target_datas[] =
		{
			TextureTargetTypeData(String(UTF8("texture_1d")),
				String(UTF8("1d")), GL_TEXTURE_1D,
				ttt_texture_1d),
			TextureTargetTypeData(String(UTF8("texture_2d")),
				String(UTF8("2d")), GL_TEXTURE_2D,
				ttt_texture_2d),
			TextureTargetTypeData(String(UTF8("texture_3d")),
				String(UTF8("3d")), GL_TEXTURE_3D,
				ttt_texture_2d),
			TextureTargetTypeData(String(UTF8("texture_cube_map")),
				String(UTF8("cube map")), GL_TEXTURE_CUBE_MAP,
				ttt_texture_2d),
			TextureTargetTypeData(String(UTF8("texture_1d_array")),
				String(UTF8("1d array")), GL_TEXTURE_1D_ARRAY,
				ttt_texture_1d),
			TextureTargetTypeData(String(UTF8("texture_2d_array")),
				String(UTF8("2d array")), GL_TEXTURE_2D_ARRAY,
				ttt_texture_2d),
			TextureTargetTypeData(
				String(UTF8("texture_cube_map_array")),
				String(UTF8("cube map array")),
				GL_TEXTURE_CUBE_MAP_ARRAY, ttt_texture_2d),
			TextureTargetTypeData(String(UTF8("texture_rectangle")),
				String(UTF8("rectangle")),
				GL_TEXTURE_RECTANGLE, ttt_texture_rectangle),
			TextureTargetTypeData(
				String(UTF8("texture_2d_multisample")),
				String(UTF8("2d multisample")), 
				GL_TEXTURE_2D_MULTISAMPLE, ttt_texture_2d),
			TextureTargetTypeData(
				String(UTF8("texture_2d_multisample_array")),
				String(UTF8("2d multisample array")), 
				GL_TEXTURE_2D_MULTISAMPLE_ARRAY, ttt_texture_2d)
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

	const String &TextureTargetUtil::get_description(
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

		return texture_target_datas[texture_target].get_description();
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

	TextureTargetType TextureTargetUtil::get_depth_texture_target(
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

		return texture_target_datas[
			texture_target].get_depth_texture_target();
	}

	Uint32 TextureTargetUtil::get_texture_target_count() noexcept
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
