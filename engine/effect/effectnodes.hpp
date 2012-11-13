/****************************************************************************
 *            effectnodes.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
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
#include "effecttextureunit.hpp"
#include "effectnodeport.hpp"
#include "../shader/abstractshadersource.hpp"

/**
 * @file
 * @brief The @c class EffectNodes.
 * This file contains the @c class EffectNodes.
 */
namespace eternal_lands
{

	class EffectNodes: public AbstractShaderSource
	{
		friend class EffectNode;
		friend class EffectNodePort;
		private:
			typedef boost::array<EffectTextureUnit, 12>
				EffectTextureUnitMapArray12;

			EffectNodeVector m_nodes;
			EffectTextureUnitMapArray12 m_texture_units;

			boost::mt19937 m_ran;
			Mt19937RandomUuidGenerator m_uuid_generator;
			Uint32 m_ids;

			virtual ShaderSourceParameterVector get_parameters(
				const ShaderType shader_type,
				const ShaderVersionType version) const
				override;
			virtual String get_source(const ShaderType shader_type,
				const ShaderVersionType version) const
				override;
			virtual void do_load_xml(const xmlNodePtr node)
				override;
			virtual void do_save_xml(
				const XmlWriterSharedPtr &xml_writer) const
				override;
			void load_nodes_xml(const xmlNodePtr node);
			void load_connection_xml(const xmlNodePtr node,
				UuidEffectNodePortPtrMap &ports);
			void load_connections_xml(const xmlNodePtr node);
			void get_ports(UuidEffectNodePortPtrMap &ports);

		public:
			EffectNodes();
			virtual ~EffectNodes() noexcept override;
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
				const EffectQualityType quality,
				ShaderSourceParameterVector &vertex_parameters,
				ShaderSourceParameterVector
					&fragment_parameters,
				OutStream &vertex_str, OutStream &fragment_str)
				const;
			void remove(const EffectNodePtr effect_node);
			void set_texture_unit(const String &name,
				const TextureTargetType target,
				const Uint16 index);
			void set_texture_unit(const String &name,
				const String &default_texture,
				const TextureTargetType target,
				const Uint16 index);
			void set_texture_unit(const String &name,
				const StringVector &default_texture,
				const TextureTargetType target,
				const Uint16 index);
			virtual ShaderSourceType get_type() const override;
			virtual bool get_has_data(const ShaderType shader_type,
				const ShaderVersionType version) const
				override;
			static String get_xml_id();

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

	};

}

#endif	/* UUID_8707c08f_7220_4673_a7d2_6a190da49af8 */
