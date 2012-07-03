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
#include "effectchangeutil.hpp"
#include "effectnodeportutil.hpp"

/**
 * @file
 * @brief The @c class EffectNode.
 * This file contains the @c class EffectNode.
 */
namespace eternal_lands
{

	class EffectNode
	{
		friend class EffectNodePort;
		private:
			EffectNodePortVector m_ports;
			String m_name;
			Uint16 m_value_count;

			bool check_connections(EffectNodePtrSet &checking);

		protected:
			EffectNode(const String &name);
			void add_input_port(const String &description,
				const EffectNodePortType type);
			void add_input_port(const EffectNodePortType type);
			void add_output_port(const String &var_name,
				const String &description,
				const EffectNodePortType type,
				const EffectChangeType change);
			void add_output_port(const String &var_name,
				const EffectNodePortType type,
				const EffectChangeType change);

			inline void set_value_count(const Uint16 value_count)
				noexcept
			{
				m_value_count = value_count;
			}

		public:
			virtual ~EffectNode() noexcept;
			virtual void write(OutStream &str) const = 0;
			void update(EffectNodePtrSet &updated);
			bool update();
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
