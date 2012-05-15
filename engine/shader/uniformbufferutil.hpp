/****************************************************************************
 *            uniformbufferutil.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_90d5e594_bf25_4f07_91a3_531559cb9250
#define	UUID_90d5e594_bf25_4f07_91a3_531559cb9250

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class UniformBufferUtil.
 * This file contains the @c class UniformBufferUtil.
 */
namespace eternal_lands
{

	enum UniformBufferType
	{
		ubt_none,
		ubt_scene_data,
		ubt_material_data,
		ubt_terrain_data
	};

	class UniformBufferUtil
	{
		public:
			static const String &get_str(
				const UniformBufferType uniform_buffer);
			static UniformBufferType get_uniform_buffer(
				const String &str);
			static bool get_uniform_buffer(const String &str,
				UniformBufferType &shader_version) noexcept;
			static Uint32 get_uniform_buffer_count() noexcept;

	};

	OutStream& operator<<(OutStream &str, const UniformBufferType value);
	InStream& operator>>(InStream &str, UniformBufferType &value);

}

#endif	/* UUID_90d5e594_bf25_4f07_91a3_531559cb9250 */
