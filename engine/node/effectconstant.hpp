/****************************************************************************
 *            effectconstant.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
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
		private:
			glm::vec4 m_value;
			String m_var_name;
			EffectConstantType m_type;

			virtual Uint16 get_initial_value_count() const;

		public:
			EffectConstant(const String &name,
				const EffectConstantType type,
				Uint32 &var_ids);
			virtual ~EffectConstant() noexcept;
			virtual void write(const bool glsl_120,
				const EffectChangeType change,
				StringUint16Map &parameters,
				ShaderSourceParameterVector &vertex_parameters,
				ShaderSourceParameterVector
					&fragment_parameters,
				OutStream &vertex_str, OutStream &fragment_str,
				EffectNodePtrSet &vertex_written,
				EffectNodePtrSet &fragment_written);

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
