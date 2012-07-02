/****************************************************************************
 *            effectnodeportutil.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_44619255_b1e7_4d8a_abee_c83f3cb83304
#define	UUID_44619255_b1e7_4d8a_abee_c83f3cb83304

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class EffectNodePortUtil.
 * This file contains the @c class EffectNodePortUtil.
 */
namespace eternal_lands
{

	/**
	 * Basic types of the node ports.
	 * @{
	 */
	enum EffectNodePortType
	{
		enpt_r,
		enpt_g,
		enpt_b,
		enpt_a,
		enpt_rg,
		enpt_rgb,
		enpt_xyz,
		enpt_rgba,
		enpt_single,
		enpt_general
	};
	/**
	 * @}
	 */

	class EffectNodePortUtil
	{
		public:
			static const String &get_str(
				const EffectNodePortType effect_node_port);
			static const String &get_description(
				const EffectNodePortType effect_node_port);
			static EffectNodePortType get_effect_node_port(
				const String &str);
			static bool get_effect_node_port(const String &str,
				EffectNodePortType &effect_node_port) noexcept;
			static Uint16 get_count(
				const EffectNodePortType effect_node_port);
			static bool get_convertable(
				const EffectNodePortType effect_node_port);
			static Uint32 get_effect_node_port_count() noexcept;

	};

	OutStream& operator<<(OutStream &str, const EffectNodePortType value);
	InStream& operator>>(InStream &str, EffectNodePortType &value);

}

#endif	/* UUID_44619255_b1e7_4d8a_abee_c83f3cb83304 */
