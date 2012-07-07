/****************************************************************************
 *            parameterutil.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_8fb515fd_12c6_48f1_b9e2_ffe4ed28369d
#define	UUID_8fb515fd_12c6_48f1_b9e2_ffe4ed28369d

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class ParameterUtil.
 * This file contains the @c class ParameterUtil.
 */
namespace eternal_lands
{

	/**
	 * Basic types of the shader parameter.
	 * @{
	 */
	enum ParameterType
	{
		pt_bool = 0,
		pt_bvec2,
		pt_bvec3,
		pt_bvec4,
		pt_int,
		pt_ivec2,
		pt_ivec3,
		pt_ivec4,
		pt_unsigned_int,
		pt_uvec2,
		pt_uvec3,
		pt_uvec4,
		pt_float,
		pt_vec2,
		pt_vec3,
		pt_vec4,
		pt_mat2x2,
		pt_mat2x3,
		pt_mat2x4,
		pt_mat3x2,
		pt_mat3x3,
		pt_mat3x4,
		pt_mat4x2,
		pt_mat4x3,
		pt_mat4x4,
		pt_sampler1D,
		pt_sampler2D,
		pt_samplerCube,
		pt_sampler3D,
		pt_sampler1DShadow,
		pt_sampler2DShadow,
		pt_sampler2DRect,
		pt_sampler2DRectShadow,
		pt_sampler1DArray,
		pt_sampler2DArray,
		pt_sampler1DArrayShadow,
		pt_sampler2DArrayShadow,
		pt_samplerCubeShadow,
		pt_samplerBuffer,
		pt_isampler1D,
		pt_isampler2D,
		pt_isamplerCube,
		pt_isampler3D,
		pt_isampler1DArray,
		pt_isampler2DArray,
		pt_isampler2DRect,
		pt_isamplerBuffer,
		pt_usampler1D,
		pt_usampler2D,
		pt_usamplerCube,
		pt_usampler3D,
		pt_usampler1DArray,
		pt_usampler2DArray,
		pt_usampler2DRect,
		pt_usamplerBuffer
	};
	/**
	 * @}
	 */

	class ParameterUtil
	{
		public:
			static Uint32 get_count(const ParameterType parameter);
			static bool get_sampler(const ParameterType parameter);
			static bool get_uint(const ParameterType parameter);
			static bool get_int(const ParameterType parameter);
			static bool get_bool(const ParameterType parameter);
			static bool get_float(const ParameterType parameter);
			static bool get_matrix(const ParameterType parameter);
			static GLenum get_gl(const ParameterType parameter);
			static const String &get_str(
				const ParameterType parameter);
			static ParameterType get_parameter(
				const String &str);
			static ParameterType get_parameter(
				const GLenum parameter);
			static bool get_parameter(const String &str,
				ParameterType &parameter) noexcept;
			static Uint32 get_parameter_count() noexcept;

	};

	OutStream& operator<<(OutStream &str, const ParameterType value);

}

#endif	/* UUID_8fb515fd_12c6_48f1_b9e2_ffe4ed28369d */
