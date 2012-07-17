/****************************************************************************
 *            effectsamplerutil.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_b46370ca_6cb9_4aef_98ce_df6db6823ddf
#define	UUID_b46370ca_6cb9_4aef_98ce_df6db6823ddf

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class EffectSamplerUtil.
 * This file contains the @c class EffectSamplerUtil.
 */
namespace eternal_lands
{

	/**
	 * Basic types of the effect samplers.
	 * @{
	 */
	enum EffectSamplerType
	{
		est_sampler_1d = 0,
		est_sampler_2d,
		est_sampler_3d,
		est_sampler_cube_map,
		est_sampler_1d_array,
		est_sampler_2d_array,
		est_sampler_cube_map_array,
		est_sampler_rectangle,
		est_sampler_1d_project,
		est_sampler_2d_project,
		est_sampler_3d_project,
		est_sampler_rectangle_project
	};
	/**
	 * @}
	 */

	class EffectSamplerUtil
	{
		public:
			static const String &get_str(
				const EffectSamplerType effect_sampler);
			static EffectSamplerType get_effect_sampler(
				const String &str);
			static bool get_effect_sampler(const String &str,
				EffectSamplerType &effect_sampler) noexcept;
			static Uint32 get_effect_sampler_count() noexcept;

	};

	OutStream& operator<<(OutStream &str, const EffectSamplerType value);
	InStream& operator>>(InStream &str, EffectSamplerType &value);

}

#endif	/* UUID_b46370ca_6cb9_4aef_98ce_df6db6823ddf */
