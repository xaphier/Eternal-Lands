/****************************************************************************
 *            objectdata.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_ab022a3e_b1f5_489b_8a77_2867286be10e
#define	UUID_ab022a3e_b1f5_489b_8a77_2867286be10e

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "selectionutil.hpp"
#include "transformation.hpp"

/**
 * @file
 * @brief The @c class ObjectData.
 * This file contains the @c class ObjectData.
 */
namespace eternal_lands
{

	enum StateType
	{
		st_blend = 0,
		st_stencil = 1
	};

	/**
	 * @brief @c class for object data.
	 *
	 * @c class for object data.
	 */
	class ObjectData
	{
		private:
			Transformation m_world_transformation;
			String m_name;
			float m_transparency;
			Uint32 m_id;
			SelectionType m_selection;
			BitSet16 m_states;
			Uint8 m_stencil_value;

		public:
			/**
			 * Default constructor.
			 */
			ObjectData();

			/**
			 * Default constructor.
			 */
			ObjectData(const Transformation &world_transformation,
				const String &name, const float transparency,
				const Uint32 id, const SelectionType selection,
				const bool blend, const bool stencil,
				const Uint8 stencil_value);

			/**
			 * Default destructor.
			 */
			~ObjectData() throw();

			inline void set_world_transformation(
				const Transformation &world_transformation)
			{
				m_world_transformation = world_transformation;
			}

			inline void set_name(const String &name)
			{
				m_name = name;
			}

			inline void set_transparency(const float transparency)
			{
				m_transparency = transparency;
			}

			inline void set_state_blend(const bool blend)
			{
				m_states[st_blend] = blend;
			}

			inline void set_state_stencil(const bool stencil)
			{
				m_states[st_stencil] = stencil;
			}

			inline void set_selection(const SelectionType selection)
			{
				m_selection = selection;
			}

			inline void set_id(const Uint32 id)
			{
				m_id = id;
			}

			inline void set_stencil_value(const Uint8 stencil_value)
			{
				m_stencil_value = stencil_value;
			}

			inline const Transformation &get_world_transformation()
				const
			{
				return m_world_transformation;
			}

			inline const String &get_name() const
			{
				return m_name;
			}

			inline float get_transparency() const
			{
				return m_transparency;
			}

			inline Uint32 get_id() const
			{
				return m_id;
			}

			inline Uint8 get_stencil_value() const
			{
				return m_stencil_value;
			}

			inline bool get_state_blend() const
			{
				return m_states[st_blend];
			}

			inline bool get_state_stencil() const
			{
				return m_states[st_stencil];
			}

			inline SelectionType get_selection() const
			{
				return m_selection;
			}

	};

}

#endif	/* UUID_ab022a3e_b1f5_489b_8a77_2867286be10e */
