/****************************************************************************
 *            parametersizeutil.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_b423a746_854a_49a2_8600_ce975768f73e
#define	UUID_b423a746_854a_49a2_8600_ce975768f73e

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class ShaderConstantUtil.
 * This file contains the @c class ShaderConstantUtil.
 */
namespace eternal_lands
{

	enum ParameterSizeType
	{
		pst_one = 0,
		pst_lights_count,
		pst_bones_count,
		pst_shadow_maps_count,
		pst_clipmap_terrain_slices,
		pst_render_targets
	};

	typedef std::map<ParameterSizeType, Uint16> ParameterSizeTypeUint16Map;
	typedef std::pair<ParameterSizeType, Uint16>
		ParameterSizeTypeUint16Pair;

	class ParameterSizeUtil
	{
		public:
			static const String &get_str(
				const ParameterSizeType parameter_size);
			static ParameterSizeType get_parameter_size(
				const String &str);
			static bool get_parameter_size(const String &str,
				ParameterSizeType &parameter_size) noexcept;
			static Uint16 get_max_size(
				const ParameterSizeType parameter_size);
			static Uint32 get_parameter_size_count() noexcept;

	};

	OutStream &operator<<(OutStream &str, const ParameterSizeType value);
	InStream &operator>>(InStream &str, ParameterSizeType &value);

}

#endif	/* UUID_b423a746_854a_49a2_8600_ce975768f73e */
