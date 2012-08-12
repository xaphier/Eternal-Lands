/****************************************************************************
 *            shaderoutpututil.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_e944dce3_af74_4500_b118_e9b8178c4b77
#define	UUID_e944dce3_af74_4500_b118_e9b8178c4b77

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

namespace eternal_lands
{

	/**
	 * Types of shader output.
	 * @{
	 */
	enum ShaderOutputType
	{
		sot_float = 0,
		sot_int,
		sot_uint
	};

	class ShaderOutputUtil
	{
		public:
			static const String &get_str(
				const ShaderOutputType shader_output);
			static ShaderOutputType get_shader_output(
				const String &str);
			static Uint32 get_shader_output_count() noexcept;

	};

	OutStream& operator<<(OutStream &str, const ShaderOutputType value);
	InStream& operator>>(InStream &str, ShaderOutputType &value);

}

#endif	/* UUID_e944dce3_af74_4500_b118_e9b8178c4b77 */
