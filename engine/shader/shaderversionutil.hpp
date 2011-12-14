/****************************************************************************
 *            shaderversionutil.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
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
		svt_330
	};

	class ShaderVersionUtil
	{
		public:
			static const String &get_str(
				const ShaderVersionType version);
			static Uint16 get_shader_version_number(
				const ShaderVersionType version);
			static ShaderVersionType get_shader_version(
				const String &str);
			static bool get_shader_version(const String &str,
				ShaderVersionType &shader_version);
			static Uint32 get_shader_version_count();

	};

	OutStream& operator<<(OutStream &str, const ShaderVersionType value);
	InStream& operator>>(InStream &str, ShaderVersionType &value);

}

#endif	/* UUID_6f3c23a5_98ba_4a89_8aef_82a8687aca73 */
