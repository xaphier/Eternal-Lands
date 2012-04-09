/****************************************************************************
 *            shadertextureutil.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_75062fe6_6438_4c82_92b7_373e91c3dcd2
#define	UUID_75062fe6_6438_4c82_92b7_373e91c3dcd2

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

	enum ShaderTextureType
	{
		stt_albedo_0 = 0,
		stt_albedo_1 = 1,
		stt_albedo_2 = 2,
		stt_albedo_3 = 3,
		stt_normal_0 = 4,
		stt_normal_1 = 8,
		stt_specular_0 = 5,
		stt_specular_1 = 9,
		stt_emission_0 = 6,
		stt_emission_1 = 10,
		stt_blend_0 = 7,
		stt_blend_1 = 11,
		stt_height = 12,
		stt_dudv = 13,
		stt_terrain = 14,
		stt_shadow = 15
	};

	const Uint16 shader_texture_count = 16;

	BOOST_STATIC_ASSERT(stt_shadow < shader_texture_count);

	class ShaderTextureUtil
	{
		public:
			static const String &get_str(
				const ShaderTextureType texture);
			static ShaderTextureType get_shader_texture(
				const String &str);
			static bool get_shader_texture(const String &str,
				ShaderTextureType &shader_texture);
			static Uint32 get_shader_texture_count();
			static Uint16 get_layer_index(
				const ShaderTextureType texture);
			static bool get_use_layer_index(
				const ShaderTextureType texture);

	};

	OutStream& operator<<(OutStream &str, const ShaderTextureType value);
	InStream& operator>>(InStream &str, ShaderTextureType &value);

}

#endif	/* UUID_75062fe6_6438_4c82_92b7_373e91c3dcd2 */
