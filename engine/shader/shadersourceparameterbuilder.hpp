/****************************************************************************
 *            shadersourceparameterbuilder.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
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
			static ShaderSourceParameter build(
				const String &name, const ParameterType type,
				const ParameterQualifierType qualifier,
				const Uint16 array_size = 1);

			static ShaderSourceParameter build(
				const String &name, const ParameterType type,
				const ParameterQualifierType qualifier,
				const ParameterSizeType size,
				const Uint16 array_size = 1);

			static ShaderSourceParameter build(
				const CommonParameterType common_parameter,
				const ParameterQualifierType qualifier);

			static ShaderSourceParameter build(
				const AutoParameterType auto_parameter);

			static void add_parameter(
				const ShaderSourceParameter &parameter,
				const ShaderSourceParameterVector &locals,
				ShaderSourceParameterVector &parameters);

			static void add_local(
				const ShaderSourceParameter &local,
				ShaderSourceParameterVector &locals,
				const ShaderSourceParameterVector &parameters);

	};

}

#endif	/* UUID_4b26f07d_df16_4ae8_b0d5_b4c6cf87d03c */