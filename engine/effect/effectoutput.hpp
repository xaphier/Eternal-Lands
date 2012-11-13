/****************************************************************************
 *            effectoutput.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
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
		friend class EffectNodes;
		private:
			EffectOutput();
			virtual Uint16 get_initial_value_count() const;

		protected:
			virtual void save_xml(const XmlWriterSharedPtr &writer)
				const override;
			virtual void load_xml(const xmlNodePtr node) override;

		public:
			EffectOutput(const String &name, const Uint32 id,
				Mt19937RandomUuidGenerator &uuid_generator);
			virtual ~EffectOutput() noexcept override;
			void write(const Uint16StringMap &array_layers,
				const ShaderVersionType version,
				const EffectQualityType quality,
				const EffectChangeType change,
				StringUint16Map &parameters,
				ShaderSourceParameterVector &vertex_parameters,
				ShaderSourceParameterVector
					&fragment_parameters,
				OutStream &vertex_str, OutStream &fragment_str,
				UuidSet &vertex_written,
				UuidSet &fragment_written) const override;
			virtual String get_description() const override;

	};

}

#endif	/* UUID_128af7c2_7e2b_4213_96df_2ffcfa8217ac */
