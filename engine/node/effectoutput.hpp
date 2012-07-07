/****************************************************************************
 *            effectoutput.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_128af7c2_7e2b_4213_96df_2ffcfa8217ac
#define	UUID_128af7c2_7e2b_4213_96df_2ffcfa8217ac

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "effectnode.hpp"

/**
 * @file
 * @brief The @c class EffectOutput.
 * This file contains the @c class EffectOutput.
 */
namespace eternal_lands
{

	class EffectOutput: public EffectNode
	{
		private:
			virtual void do_write(const bool glsl_120,
				const EffectChangeType change,
				StringBitSet16Map &parameters_indices,
				ShaderSourceParameterVector &vertex_parameters,
				ShaderSourceParameterVector
					&fragment_parameters,
				OutStream &vertex_str, OutStream &fragment_str,
				EffectNodePtrSet &written);

		public:
			EffectOutput(const String &name);
			virtual ~EffectOutput() noexcept;

	};

}

#endif	/* UUID_128af7c2_7e2b_4213_96df_2ffcfa8217ac */
