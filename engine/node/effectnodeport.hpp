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
			const String m_var;
			const String m_description;
			const Uint8Array4 m_swizzle;
			const EffectChangeType m_change;
			const bool m_input;

			void do_connect(const EffectNodePortPtr port);
			void do_disconnect(const EffectNodePortPtr port);
			bool check_connection(EffectNodePtrVector &checking)
				const;
			void update(EffectNodePtrSet &updated);
			Uint16 get_connected_value_count() const;
			EffectChangeType get_connected_change() const;
			void add_parameter(
				StringBitSet16Map &parameters_indices);
			BitSet16 get_var_indices() const;
			Uint16 get_var_count() const;
			String get_var_swizzled() const;
			bool do_check_connection(const EffectNodePortPtr port,
				EffectNodePtrVector &checking) const;

			inline const EffectNodePortPtr &get_connection()
				const noexcept
			{
				return *m_connections.begin();
			}

			inline const Uint8Array4 &get_swizzle() const noexcept
			{
				return m_swizzle;
			}

		public:
			EffectNodePort(const EffectNodePtr node,
				const String &var,
				const String &description,
				const String &swizzle,
				const EffectChangeType change,
				const bool input);
			EffectNodePort(const EffectNodePtr node,
				const String &var,
				const String &swizzle,
				const EffectChangeType change,
				const bool input);
			~EffectNodePort() noexcept;
			bool check_connection(const EffectNodePortPtr port);
			Uint16 get_value_count() const;
			Uint16 get_node_value_count() const;
			EffectChangeType get_change() const;
			EffectChangeType get_node_change() const;
			bool connect(const EffectNodePortPtr port);
			void disconnect(const EffectNodePortPtr port);
			bool get_convertable(const EffectNodePortPtr port)
				const;
			String get_connected_var_swizzled() const;
			void write(const bool glsl_120,
				const EffectChangeType change,
				StringUint16Map &parameters,
				ShaderSourceParameterVector &vertex_parameters,
				ShaderSourceParameterVector
					&fragment_parameters,
				OutStream &vertex_str, OutStream &fragment_str,
				EffectNodePtrSet &vertex_written,
				EffectNodePtrSet &fragment_written);

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

			inline const String &get_var() const noexcept
			{
				return m_var;
			}

			inline bool get_general_type() const noexcept
			{
				return m_swizzle[0] == UTF8('*');
			}

			inline bool get_undefined_change() const noexcept
			{
				return m_change == ect_undefined;
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
