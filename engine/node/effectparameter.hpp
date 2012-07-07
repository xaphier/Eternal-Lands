/****************************************************************************
 *            effectparameter.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_1654606c_e461_4d86_bc43_72886572aea4
#define	UUID_1654606c_e461_4d86_bc43_72886572aea4

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "effectnode.hpp"
#include "effectparameterutil.hpp"

/**
 * @file
 * @brief The @c class EffectParameter.
 * This file contains the @c class EffectParameter.
 */
namespace eternal_lands
{

	class EffectParameter: public EffectNode
	{
		private:
			EffectParameterType m_type;

			virtual void do_write(const bool glsl_120,
				const EffectChangeType change,
				StringBitSet16Map &parameters_indices,
				ShaderSourceParameterVector &vertex_parameters,
				ShaderSourceParameterVector
					&fragment_parameters,
				OutStream &vertex_str, OutStream &fragment_str,
				EffectNodePtrSet &written);

		public:
			EffectParameter(const String &name,
				const EffectParameterType type);
			virtual ~EffectParameter() noexcept;

			inline EffectParameterType get_type() const noexcept
			{
				return m_type;
			}

	};

}

#endif	/* UUID_1654606c_e461_4d86_bc43_72886572aea4 */
