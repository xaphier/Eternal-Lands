/****************************************************************************
 *            samplerparameterutil.hpp
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
 * @brief The @c class SamplerParameterUtil.
 * This file contains the @c class SamplerParameterUtil.
 */
namespace eternal_lands
{

	enum SamplerParameterType
	{
		spt_albedo_0 = 0,
		spt_albedo_1 = 1,
		spt_albedo_2 = 2,
		spt_albedo_3 = 3,
		spt_normal = 4,
		spt_specular = 5,
		spt_emission = 6,
		spt_blend = 7,
		spt_vertex_vector = 8,
		spt_vertex_normal = 9,
		spt_vertex_dudv = 10,
		spt_light_positions = 11,
		spt_light_colors = 12,
		spt_light_indices = 13,
		spt_reflection = 14,
		spt_shadow = 15
	};

	class SamplerParameterUtil
	{
		public:
			static const String &get_str(
				const SamplerParameterType sampler_parameter);
			static SamplerParameterType get_sampler_parameter(
				const String &str);
			static bool get_sampler_parameter(const String &str,
				SamplerParameterType &sampler_parameter)
				noexcept;
			static Uint32 get_sampler_parameter_count() noexcept;

	};

	OutStream& operator<<(OutStream &str, const SamplerParameterType value);
	InStream& operator>>(InStream &str, SamplerParameterType &value);

}

#endif	/* UUID_75062fe6_6438_4c82_92b7_373e91c3dcd2 */
