/****************************************************************************
 *            shaderversionutil.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_6f3c23a5_98ba_4a89_8aef_82a8687aca73
#define	UUID_6f3c23a5_98ba_4a89_8aef_82a8687aca73

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class ShaderTextureUtil.
 * This file contains the @c class ShaderTextureUtil.
 */
namespace eternal_lands
{

	enum ShaderVersionType
	{
		svt_120,
		svt_130,
		svt_140,
		svt_150,
		svt_330,
		svt_400,
		svt_410,
		svt_420
	};

	class ShaderVersionUtil
	{
		public:
			static const String &get_str(
				const ShaderVersionType version);
			static Uint16 get_shader_version_number(
				const ShaderVersionType version);
			static ShaderVersionType get_shader_version(
				const Uint16 version_number);
			static ShaderVersionType get_shader_version(
				const String &str);
			static bool get_shader_version(const String &str,
				ShaderVersionType &shader_version) noexcept;
			static Uint32 get_shader_version_count() noexcept;

	};

	OutStream &operator<<(OutStream &str, const ShaderVersionType value);
	InStream &operator>>(InStream &str, ShaderVersionType &value);

}

#endif	/* UUID_6f3c23a5_98ba_4a89_8aef_82a8687aca73 */
