/****************************************************************************
 *            uniformbufferutil.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_90d5e594_bf25_4f07_91a3_531559cb9250
#define	UUID_90d5e594_bf25_4f07_91a3_531559cb9250

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "autoparameterutil.hpp"

/**
 * @file
 * @brief The @c class UniformBufferUtil.
 * This file contains the @c class UniformBufferUtil.
 */
namespace eternal_lands
{

	enum UniformBufferType
	{
		ubt_scene,
		ubt_material,
		ubt_animation,
		ubt_shadow,
		ubt_terrain,
		ubt_terrain_instances
	};

	class UniformBufferUtil
	{
		public:
			static const String &get_identifier(
				const UniformBufferType uniform_buffer);
			static const String &get_str(
				const UniformBufferType uniform_buffer);
			static const AutoParameterTypeVector
				&get_auto_parameters(
					const UniformBufferType uniform_buffer);
			static UniformBufferType get_uniform_buffer(
				const String &str);
			static UniformBufferType
				get_uniform_buffer_from_identifier(
					const String &identifier);
			static bool get_uniform_buffer(const String &str,
				UniformBufferType &uniform_buffer) noexcept;
			static bool get_uniform_buffer_from_identifier(
				const String &identifier,
				UniformBufferType &uniform_buffer) noexcept;
			static Uint32 get_uniform_buffer_count() noexcept;

	};

	OutStream &operator<<(OutStream &str, const UniformBufferType value);
	InStream &operator>>(InStream &str, UniformBufferType &value);

}

#endif	/* UUID_90d5e594_bf25_4f07_91a3_531559cb9250 */
