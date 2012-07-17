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
			virtual Uint16 get_initial_value_count() const;

		public:
			EffectOutput(const String &name);
			virtual ~EffectOutput() noexcept;
			void write(const Uint16StringMap &array_layers,
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

	};

}

#endif	/* UUID_128af7c2_7e2b_4213_96df_2ffcfa8217ac */
