/****************************************************************************
 *            blendutil.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_ed53a8e0_87bd_4f64_a69c_3d5b01405c76
#define	UUID_ed53a8e0_87bd_4f64_a69c_3d5b01405c76

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

namespace eternal_lands
{

	/**
	 * Types of blend modes.
	 * @{
	 */
	enum BlendType
	{
		/**
		 * Make the object transparent based on the final alpha value
		 * of the object.
		 */
		bt_alpha_transparency_source_value = 0,
		/**
		 * Make the object transparent based on the transparency value
		 * of the object.
		 */
		bt_alpha_transparency_value,
		/**
		 * Uses additive blending based on the final alpha values in
		 * the object.
		 */
		bt_additive
	};
	/**
	 * @}
	 */

	class BlendUtil
	{
		public:
			static const String &get_str(const BlendType blend);
			static BlendType get_blend(const String &str);
			static Uint32 get_blend_count() noexcept;

	};

	OutStream &operator<<(OutStream &str, const BlendType value);
	InStream &operator>>(InStream &str, BlendType &value);

}

#endif	/* UUID_ed53a8e0_87bd_4f64_a69c_3d5b01405c76 */
