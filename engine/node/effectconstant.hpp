/****************************************************************************
 *            effectconstant.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_c3aa7f77_e79c_44c0_8887_8f4e3d58afec
#define	UUID_c3aa7f77_e79c_44c0_8887_8f4e3d58afec

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "effectnode.hpp"
#include "effectconstantutil.hpp"

/**
 * @file
 * @brief The @c class EffectConstant.
 * This file contains the @c class EffectConstant.
 */
namespace eternal_lands
{

	class EffectConstant: public EffectNode
	{
		friend class EffectNodes;
		private:
			glm::vec4 m_value;
			String m_var_name;
			EffectConstantType m_type;

			EffectConstant();
			virtual Uint16 get_initial_value_count() const;

		protected:
			virtual void save_xml(const XmlWriterSharedPtr &writer);
			virtual void load_xml(const xmlNodePtr node);

		public:
			EffectConstant(const String &name, const Uint32 id,
				const EffectConstantType type,
				Mt19937RandomUuidGenerator &uuid_generator);
			virtual ~EffectConstant() noexcept;
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
				EffectNodePtrSet &fragment_written);
			virtual String get_description() const;

			inline EffectConstantType get_type() const noexcept
			{
				return m_type;
			}

			inline const glm::vec4 &get_value() const noexcept
			{
				return m_value;
			}

			inline void set_value(const glm::vec4 &value) noexcept
			{
				m_value = value;
			}

	};

}

#endif	/* UUID_08e79691_15e8_455a_bab6_b75881a1c19f */
