/****************************************************************************
 *            effectnodes.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_8707c08f_7220_4673_a7d2_6a190da49af8
#define	UUID_8707c08f_7220_4673_a7d2_6a190da49af8

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "effectconstant.hpp"
#include "effectfunction.hpp"
#include "effectparameter.hpp"
#include "effecttexture.hpp"
#include "../shader/shaderversionutil.hpp"

/**
 * @file
 * @brief The @c class EffectNodes.
 * This file contains the @c class EffectNodes.
 */
namespace eternal_lands
{

	class EffectNodes: public boost::noncopyable
	{
		friend class EffectNode;
		friend class EffectNodePort;
		private:
			EffectNodeVector m_nodes;
			boost::mt19937 m_ran;
			Mt19937RandomUuidGenerator m_uuid_generator;
			String m_name;
			Uint32 m_ids;

			EffectNodes();
			void save_xml(const XmlWriterSharedPtr &writer);
			void load_xml(const xmlNodePtr node);
			void load_nodes_xml(const xmlNodePtr node);
			void load_connection_xml(const xmlNodePtr node,
				UuidEffectNodePortPtrMap &ports);
			void load_connections_xml(const xmlNodePtr node);
			void get_ports(UuidEffectNodePortPtrMap &ports);

		public:
			EffectNodes(const String &name);
			~EffectNodes() noexcept;
			EffectNodePtr add_color(const String &name);
			EffectNodePtr add_direction(const String &name);
			EffectNodePtr add_constant(const String &name,
				const Uint16 count);
			EffectNodePtr add_function(const String &name,
				const EffectFunctionType function);
			EffectNodePtr add_parameter(const String &name,
				const EffectParameterType parameter);
			EffectNodePtr add_texture(const String &name,
				const EffectSamplerType sampler,
				const EffectTextureType texture,
				const Uint16 texture_unit);
			EffectNodePtr add_output(const String &name);
			void write(const Uint16StringMap &array_layers,
				const ShaderVersionType version,
				const bool low_quality,
				ShaderSourceParameterVector &vertex_parameters,
				ShaderSourceParameterVector
					&fragment_parameters,
				OutStream &vertex_str, OutStream &fragment_str);
			void save_xml(const String &file_name);
			void load_xml(const String &file_name);
			void load_xml(const FileSystemSharedPtr &file_system,
				const String &file_name);
			void remove(const EffectNodePtr effect_node);

			inline Uint32 get_node_count() const
			{
				return m_nodes.size();
			}

			inline const EffectNodeVector &get_nodes() const
			{
				return m_nodes;
			}

			inline EffectNodePtr get_node(const Uint32 index)
			{
				return &m_nodes[index];
			}

			inline Uint32 get_ids() const noexcept
			{
				return m_ids;
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

#endif	/* UUID_8707c08f_7220_4673_a7d2_6a190da49af8 */
