/****************************************************************************
 *            samplerparameterutil.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
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
		spt_effect_0 = 0,
		spt_effect_1 = 1,
		spt_effect_2 = 2,
		spt_effect_3 = 3,
		spt_effect_4 = 4,
		spt_effect_5 = 5,
		spt_effect_6 = 6,
		spt_effect_7 = 7,
		spt_effect_8 = 8,
		spt_effect_9 = 9,
		spt_effect_10 = 10,
		spt_effect_11 = 11,
		/**
		 * Used for light positions.
		 */
		spt_effect_12 = 12,
		/**
		 * Used for light colors.
		 */
		spt_effect_13 = 13,
		/**
		 * Used for light indices.
		 */
		spt_effect_14 = 14,
		/**
		 * Used for shadows.
		 */
		spt_effect_15 = 15
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
