/****************************************************************************
 *            effecttexture.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_00d1c233_ac27_41ab_991a_acbde4b2c2ae
#define	UUID_00d1c233_ac27_41ab_991a_acbde4b2c2ae

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "effectnode.hpp"
#include "effectsamplerutil.hpp"
#include "effecttextureutil.hpp"

/**
 * @file
 * @brief The @c class EffectTexture.
 * This file contains the @c class EffectTexture.
 */
namespace eternal_lands
{

	class EffectTexture: public EffectNode
	{
		friend class EffectNodes;
		private:
			StringArray8 m_var_names;
			EffectSamplerType m_sampler;
			EffectTextureType m_texture;
			Uint16 m_texture_unit;

			EffectTexture();
			virtual Uint16 get_initial_value_count() const;

		protected:
			virtual void save_xml(const XmlWriterSharedPtr &writer)
				const override;
			virtual void load_xml(const xmlNodePtr node) override;

		public:
			EffectTexture(const String &name, const Uint32 id,
				const EffectSamplerType sampler,
				const EffectTextureType texture,
				const Uint16 texture_unit,
				Mt19937RandomUuidGenerator &uuid_generator);
			virtual ~EffectTexture() noexcept override;
			virtual void write(const Uint16StringMap &array_layers,
				const ShaderVersionType version,
				const EffectQualityType quality,
				const EffectChangeType change,
				StringUint16Map &parameters,
				ShaderSourceParameterVector &vertex_parameters,
				ShaderSourceParameterVector
					&fragment_parameters,
				OutStream &vertex_str, OutStream &fragment_str,
				EffectNodePtrSet &vertex_written,
				EffectNodePtrSet &fragment_written) override;
			virtual String get_description() const override;
			Uint16 get_layer();

			inline EffectSamplerType get_sampler() const noexcept
			{
				return m_sampler;
			}

			inline EffectTextureType get_texture() const noexcept
			{
				return m_texture;
			}

			inline Uint16 get_texture_unit() const noexcept
			{
				return m_texture_unit;
			}

	};

}

#endif	/* UUID_00d1c233_ac27_41ab_991a_acbde4b2c2ae */
