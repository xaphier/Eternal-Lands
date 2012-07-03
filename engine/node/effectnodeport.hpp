/****************************************************************************
 *            effectnodeport.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_25dc98bb_a21d_4b53_b0d5_79e18e6b7e92
#define	UUID_25dc98bb_a21d_4b53_b0d5_79e18e6b7e92

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "effectchangeutil.hpp"
#include "effectnodeportutil.hpp"

/**
 * @file
 * @brief The @c class EffectNodePort.
 * This file contains the @c class EffectNodePort.
 */
namespace eternal_lands
{

	class EffectNodePort
	{
		friend class EffectNode;
		private:
			EffectNodePortPtrSet m_connections;
			EffectNodePtr m_node;
			const String m_var_name;
			const String m_description;
			const EffectNodePortType m_type;
			const EffectChangeType m_change;
			const bool m_input;

			void do_connect(const EffectNodePortPtr port);
			void do_disconnect(const EffectNodePortPtr port);
			bool check_connection(EffectNodePtrSet &checking) const;
			void update(EffectNodePtrSet &updated);
			Uint16 get_connected_value_count() const;
			static bool get_convertable(
				const EffectNodePortType effect_node_port_0,
				const EffectNodePortType effect_node_port_1,
				const Uint16 count_0, const Uint16 count_1);

			inline const EffectNodePortPtr &get_connection()
				const noexcept
			{
				return *m_connections.begin();
			}

		public:	
			EffectNodePort(const EffectNodePtr node,
				const String &var_name,
				const String &description,
				const EffectNodePortType type,
				const EffectChangeType change,
				const bool input);
			EffectNodePort(const EffectNodePtr node,
				const String &var_name,
				const EffectNodePortType type,
				const EffectChangeType change,
				const bool input);
			~EffectNodePort() noexcept;
			bool check_connection(const EffectNodePortPtr port,
				EffectNodePtrSet &checking) const;
			Uint16 get_value_count() const noexcept;
			Uint16 get_node_value_count() const noexcept;
			bool connect(const EffectNodePortPtr port);
			void disconnect(const EffectNodePortPtr port);
			bool get_convertable(const EffectNodePortPtr port)
				const;
			String get_connected_var_name() const;

			inline const EffectNode &get_node() const noexcept
			{
				return *m_node;
			}

			inline const EffectNodePtr get_node_ptr() const noexcept
			{
				return m_node;
			}

			inline const EffectNodePortPtrSet &get_connections()
				const noexcept
			{
				return m_connections;
			}

			inline const String &get_description() const noexcept
			{
				return m_description;
			}

			inline const String &get_var_name() const noexcept
			{
				return m_var_name;
			}

			inline EffectNodePortType get_type() const noexcept
			{
				return m_type;
			}

			inline bool get_general_type() const noexcept
			{
				return enpt_general == get_type();
			}

			inline bool get_input() const noexcept
			{
				return m_input;
			}

			inline bool get_output() const noexcept
			{
				return !get_input();
			}

			inline bool get_connected() const noexcept
			{
				return m_connections.size() > 0;
			}

	};

}

#endif	/* UUID_25dc98bb_a21d_4b53_b0d5_79e18e6b7e92 */
