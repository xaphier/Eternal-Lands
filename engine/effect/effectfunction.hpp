/****************************************************************************
 *            effectfunction.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_2d3da895_0197_46fa_994e_2e7f24dc1b7e
#define	UUID_2d3da895_0197_46fa_994e_2e7f24dc1b7e

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "effectnode.hpp"
#include "effectfunctionutil.hpp"

/**
 * @file
 * @brief The @c class EffectFunction.
 * This file contains the @c class EffectFunction.
 */
namespace eternal_lands
{

	class EffectFunction: public EffectNode
	{
		friend class EffectNodes;
		private:
			StringArray2 m_var_names;
			EffectFunctionType m_type;

			EffectFunction();
			virtual Uint16 get_initial_value_count() const;

		protected:
			virtual void save_xml(const XmlWriterSharedPtr &writer)
				const override;
			virtual void load_xml(const xmlNodePtr node) override;

		public:
			EffectFunction(const String &name, const Uint32 id,
				const EffectFunctionType type,
				Mt19937RandomUuidGenerator &uuid_generator);
			virtual ~EffectFunction() noexcept override;
			virtual void write(const Uint16StringMap &array_layers,
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

			inline EffectFunctionType get_type() const noexcept
			{
				return m_type;
			}

	};

}

#endif	/* UUID_2d3da895_0197_46fa_994e_2e7f24dc1b7e */
