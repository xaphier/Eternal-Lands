/****************************************************************************
 *            shaderutil.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_1692bc93_6c9b_4009_9193_bd7c6813511c
#define	UUID_1692bc93_6c9b_4009_9193_bd7c6813511c

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class Shader.
 * This file contains the @c class Shader.
 */
namespace eternal_lands
{

	enum ShaderType
	{
		st_vertex = 0,
		st_tess_control,
		st_tess_evaluation,
		st_geometry,
		st_fragment
	};

	typedef std::set<ShaderType> ShaderTypeSet;
	typedef std::map<ShaderType, String> ShaderTypeStringMap;

	class ShaderUtil
	{
		public:
			static ShaderType get_shader(const String &str);
			static const String &get_str(const ShaderType shader);
			static Uint32 get_shader_count() noexcept;
			static ShaderTypeSet get_shaders(const String &str);
			static String get_str(const ShaderTypeSet &shaders);

	};

	OutStream &operator<<(OutStream &str, const ShaderType value);
	OutStream &operator<<(OutStream &str, const ShaderTypeSet &value);
	InStream &operator>>(InStream &str, ShaderType &value);

}

#endif	/* UUID_1692bc93_6c9b_4009_9193_bd7c6813511c */
