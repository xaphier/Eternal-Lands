/****************************************************************************
 *            effectnode.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_08e79691_15e8_455a_bab6_b75881a1c19f
#define	UUID_08e79691_15e8_455a_bab6_b75881a1c19f

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "effectchangeutil.hpp"

/**
 * @file
 * @brief The @c class EffectNode.
 * This file contains the @c class EffectNode.
 */
namespace eternal_lands
{

	class EffectNode
	{
		friend class EffectNodePort;
		private:
			EffectNodePortVector m_ports;
			String m_name;
			EffectChangeType m_change;
			Uint16 m_value_count;

			bool check_connections(EffectNodePtrSet &checking);
			virtual void do_write(const bool glsl_120,
				const EffectChangeType change,
				StringBitSet16Map &parameters_indices,
				ShaderSourceParameterVector &vertex_parameters,
				ShaderSourceParameterVector
					&fragment_parameters,
				OutStream &vertex_str, OutStream &fragment_str,
				EffectNodePtrSet &written) = 0;
			virtual Uint16 get_initial_value_count() const = 0;

			inline void set_value_count(const Uint16 value_count)
				noexcept
			{
				m_value_count = value_count;
			}

			inline void set_change(const EffectChangeType change)
				noexcept
			{
				m_change = change;
			}

		protected:
			EffectNode(const String &name);
			void add_input_port(const String &description,
				const String &swizzle);
			void add_input_port(const String &swizzle);
			void add_output_port(const String &var,
				const String &description,
				const String &swizzle,
				const EffectChangeType change = ect_undefined);
			void add_output_port(const String &var,
				const String &swizzle,
				const EffectChangeType change = ect_undefined);
			String get_value_count_type_str() const;

		public:
			virtual ~EffectNode() noexcept;
			void write(const bool glsl_120,
				const EffectChangeType change,
				StringBitSet16Map &parameters_indices,
				ShaderSourceParameterVector &vertex_parameters,
				ShaderSourceParameterVector
					&fragment_parameters,
				OutStream &vertex_str, OutStream &fragment_str,
				EffectNodePtrSet &written);
			void update(EffectNodePtrSet &updated);
			void update();

			inline const EffectNodePortVector &get_ports() const
				noexcept
			{
				return m_ports;
			}

			inline EffectNodePortVector &get_ports() noexcept
			{
				return m_ports;
			}

			inline EffectChangeType get_change() const noexcept
			{
				return m_change;
			}

			inline Uint16 get_value_count() const noexcept
			{
				return m_value_count;
			}

			inline void set_name(const String &name) noexcept
			{
				m_name = name;
			}

			inline const String &get_name() const noexcept
			{
				return m_name;
			}

	};

}

#endif	/* UUID_08e79691_15e8_455a_bab6_b75881a1c19f */
