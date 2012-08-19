/****************************************************************************
 *            effectnodeport.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_25dc98bb_a21d_4b53_b0d5_79e18e6b7e92
#define	UUID_25dc98bb_a21d_4b53_b0d5_79e18e6b7e92

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "effectchangeutil.hpp"
#include "effectqualityutil.hpp"
#include "../shader/shaderversionutil.hpp"

/**
 * @file
 * @brief The @c class EffectNodePort.
 * This file contains the @c class EffectNodePort.
 */
namespace eternal_lands
{

	class EffectNodePort: public boost::noncopyable
	{
		friend class EffectNode;
		friend class EffectNodes;
		private:
			EffectNodePortPtrSet m_connections;
			EffectNodePtr m_node;
			boost::uuids::uuid m_uuid;
			String m_var;
			String m_name;
			String m_description;
			Uint8Array4 m_swizzle;
			EffectChangeType m_change;
			bool m_input;

			EffectNodePort();
			void do_connect(const EffectNodePortPtr port);
			void do_disconnect(const EffectNodePortPtr port);
			bool check_connection(EffectNodePtrVector &checking)
				const;
			void update(EffectNodePtrSet &updated);
			Uint16 get_connected_value_count() const;
			EffectChangeType get_connected_change() const;
			void add_parameter(
				StringBitSet16Map &parameters_indices);
			Uint16 get_var_count() const;
			String get_var_swizzled() const;
			bool do_check_connection(const EffectNodePortPtr port,
				EffectNodePtrVector &checking) const;
			void save_xml(const XmlWriterSharedPtr &writer) const;
			void save_connections_xml(
				const XmlWriterSharedPtr &writer) const;
			EffectNodePort(const EffectNodePtr effect_node,
				const xmlNodePtr node);

		public:
			EffectNodePort(const EffectNodePtr node,
				const String &var,
				const String &name,
				const String &swizzle,
				const boost::uuids::uuid &uuid,
				const EffectChangeType change,
				const bool input);
			EffectNodePort(const EffectNodePtr node,
				const String &var,
				const String &swizzle,
				const boost::uuids::uuid &uuid,
				const EffectChangeType change,
				const bool input);
			~EffectNodePort() noexcept;
			bool check_connection(
				const EffectNodePortPtr port);
			Uint16 get_value_count() const;
			Uint16 get_node_value_count() const;
			EffectChangeType get_change() const;
			EffectChangeType get_node_change() const;
			bool connect(const EffectNodePortPtr port);
			void disconnect(const EffectNodePortPtr port);
			bool get_convertable(const EffectNodePortPtr port)
				const;
			String get_connected_var_swizzled() const;
			void write(const Uint16StringMap &array_layers,
				const ShaderVersionType version,
				const EffectQualityType quality,
				const EffectChangeType change,
				StringUint16Map &parameters,
				ShaderSourceParameterVector &vertex_parameters,
				ShaderSourceParameterVector
					&fragment_parameters,
				OutStream &vertex_str, OutStream &fragment_str,
				EffectNodePtrSet &vertex_written,
				EffectNodePtrSet &fragment_written);
			BitSet16 get_var_indices() const;

			inline EffectNodePortPtr get_connection() const
				noexcept
			{
				assert(get_connected());
				return *m_connections.begin();
			}

			inline EffectNodePtr get_node_ptr() const noexcept
			{
				return m_node;
			}

			inline void set_description(const String &description)
			{
				m_description = description;
			}

			inline const EffectNodePortPtrSet &get_connections()
				const noexcept
			{
				return m_connections;
			}

			inline const String &get_name() const noexcept
			{
				return m_name;
			}

			inline const String &get_description() const noexcept
			{
				return m_description;
			}

			inline const String &get_var() const noexcept
			{
				return m_var;
			}

			inline const Uint8Array4 &get_swizzle() const noexcept
			{
				return m_swizzle;
			}

			inline bool get_general_type() const noexcept
			{
				return get_swizzle()[0] == UTF8('*');
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

			inline const boost::uuids::uuid &get_uuid() const
				noexcept
			{
				return m_uuid;
			}

	};

}

#endif	/* UUID_25dc98bb_a21d_4b53_b0d5_79e18e6b7e92 */
