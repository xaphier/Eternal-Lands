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
			String m_name;
			String m_var;
			EffectTextureType m_type;

			virtual Uint16 get_initial_value_count() const;

		public:
			EffectTexture(const String &name,
				const EffectTextureType type,
				Uint32 &var_ids);
			virtual ~EffectTexture() noexcept;
			virtual void write(const bool glsl_120,
				const EffectChangeType change,
				StringUint16Map &parameters,
				ShaderSourceParameterVector &vertex_parameters,
				ShaderSourceParameterVector
					&fragment_parameters,
				OutStream &vertex_str, OutStream &fragment_str,
				EffectNodePtrSet &vertex_written,
				EffectNodePtrSet &fragment_written);

			inline EffectTextureType get_type() const noexcept
			{
				return m_type;
			}

			inline const String &get_value() const noexcept
			{
				return m_name;
			}

			inline void set_value(const String &value) noexcept
			{
				m_name = value;
			}

	};

}

#endif	/* UUID_00d1c233_ac27_41ab_991a_acbde4b2c2ae */
