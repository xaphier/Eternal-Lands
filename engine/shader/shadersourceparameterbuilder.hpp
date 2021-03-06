/****************************************************************************
 *            shadersourceparameterbuilder.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_4b26f07d_df16_4ae8_b0d5_b4c6cf87d03c
#define	UUID_4b26f07d_df16_4ae8_b0d5_b4c6cf87d03c

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "autoparameterutil.hpp"
#include "commonparameterutil.hpp"
#include "parameterutil.hpp"
#include "parameterqualifierutil.hpp"
#include "samplerparameterutil.hpp"

/**
 * @file
 * @brief The @c class ShaderSourceParameterBuilder.
 * This file contains the @c class ShaderSourceParameterBuilder.
 */
namespace eternal_lands
{

	class ShaderSourceParameterBuilder
	{
		public:
			static ShaderSourceParameter build(const String &source,
				const String &name, const ParameterType type,
				const ParameterQualifierType qualifier,
				const ParameterSizeType size = pst_one,
				const Uint16 scale = 1);

			static ShaderSourceParameter build(const String &source,
				const CommonParameterType common_parameter,
				const ParameterQualifierType qualifier);

			static ShaderSourceParameter build(const String &source,
				const AutoParameterType auto_parameter);

			static ShaderSourceParameter build(const String &source,
				const SamplerParameterType sampler_parameter,
				const ParameterType type);

			static bool add_parameter(const String &source,
				const CommonParameterType common_parameter,
				const ParameterQualifierType qualifier,
				ShaderSourceParameterVector &parameters);

			static bool add_parameter(const String &source,
				const SamplerParameterType sampler_parameter,
				const ParameterType type,
				ShaderSourceParameterVector &parameters);

			static bool add_parameter(const String &source,
				const AutoParameterType auto_parameter,
				ShaderSourceParameterVector &parameters);

			static bool add_parameter(const String &source,
				const String &name, const ParameterType type,
				const ParameterQualifierType qualifier,
				const ParameterSizeType size,
				const Uint16 scale,
				ShaderSourceParameterVector &parameters);

			static bool add_parameter(
				const ShaderSourceParameter &parameter,
				ShaderSourceParameterVector &parameters);

			static bool add_parameter(
				const ShaderSourceParameter &parameter,
				const ShaderSourceParameterVector &locals,
				ShaderSourceParameterVector &parameters,
				UniformBufferUsage &uniform_buffers);

			static bool add_local(
				const ShaderSourceParameter &local,
				ShaderSourceParameterVector &locals,
				const ShaderSourceParameterVector &parameters,
				const UniformBufferUsage &uniform_buffers);

	};

}

#endif	/* UUID_4b26f07d_df16_4ae8_b0d5_b4c6cf87d03c */
