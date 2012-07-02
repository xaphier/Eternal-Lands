/****************************************************************************
 *            effectnode.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_08e79691_15e8_455a_bab6_b75881a1c19f
#define	UUID_08e79691_15e8_455a_bab6_b75881a1c19f

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class EffectNode.
 * This file contains the @c class EffectNode.
 */
namespace eternal_lands
{

	class EffectNode
	{
		private:
			String m_name;
			Uint16 m_value_count;
			EffectNodePortVector m_ports;

		protected:
			EffectNode();

		public:
			~EffectNode() noexcept;
			bool check() const;

			inline const EffectNodePortVector &get_ports() const
				noexcept
			{
				return m_ports;
			}

			inline Uint16 get_value_count() const noexcept
			{
				return m_value_count;
			}

			inline void set_name(const String &name) noexcept
			{
				m_name = name;
			}

			inline const String &get_name() const noexcept
			{
				return m_name;
			}

	};

}

#endif	/* UUID_08e79691_15e8_455a_bab6_b75881a1c19f */
