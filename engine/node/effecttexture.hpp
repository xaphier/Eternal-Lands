/****************************************************************************
 *            effecttexture.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
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
		private:
			StringArray3 m_vars;
			EffectSamplerType m_sampler;
			EffectTextureType m_texture;
			Uint16 m_texture_unit;

			virtual Uint16 get_initial_value_count() const;

		public:
			EffectTexture(const String &name,
				const EffectSamplerType sampler,
				const EffectTextureType texture,
				const Uint16 texture_unit, Uint32 &var_ids);
			virtual ~EffectTexture() noexcept;
			virtual void write(const Uint16StringMap &array_layers,
				const ShaderVersionType version,
				const bool low_quality,
				const EffectChangeType change,
				StringUint16Map &parameters,
				ShaderSourceParameterVector &vertex_parameters,
				ShaderSourceParameterVector
					&fragment_parameters,
				OutStream &vertex_str, OutStream &fragment_str,
				EffectNodePtrSet &vertex_written,
				EffectNodePtrSet &fragment_written);
			static String get_sampler_name(
				const Uint16 texture_unit);

			inline String get_sampler_name() const
			{
				return get_sampler_name(get_texture_unit());
			}

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
