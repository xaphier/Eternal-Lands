/****************************************************************************
 *            effectparameter.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
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
		friend class EffectNodes;
		private:
			EffectParameterType m_type;

			EffectParameter();
			virtual Uint16 get_initial_value_count() const;

		protected:
			virtual void save_xml(const XmlWriterSharedPtr &writer);
			virtual void load_xml(const xmlNodePtr node);

		public:
			EffectParameter(const String &name, const Uint32 id,
				const EffectParameterType type,
				Mt19937RandomUuidGenerator &uuid_generator);
			virtual ~EffectParameter() noexcept;
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
			virtual String get_description() const;

			inline EffectParameterType get_type() const noexcept
			{
				return m_type;
			}

	};

}

#endif	/* UUID_1654606c_e461_4d86_bc43_72886572aea4 */
