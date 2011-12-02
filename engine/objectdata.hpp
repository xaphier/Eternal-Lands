/****************************************************************************
 *            objectdata.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_ab022a3e_b1f5_489b_8a77_2867286be10e
#define	UUID_ab022a3e_b1f5_489b_8a77_2867286be10e

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "selectionutil.hpp"

/**
 * @file
 * @brief The @c class Object.
 * This file contains the @c class Object.
 */
namespace eternal_lands
{

	/**
	 * @brief @c class for object data.
	 *
	 * @c class for object data.
	 */
	class ObjectData
	{
		private:
			glm::mat4x3 m_world_matrix;
			glm::vec4 m_color;
			String m_name;
			float m_transparency;
			Uint32 m_id;
			SelectionType m_selection;
			bool m_blend;

		public:
			/**
			 * Default constructor.
			 */
			ObjectData();

			/**
			 * Default constructor.
			 */
			ObjectData(const ObjectData &object_data);

			/**
			 * Default constructor.
			 */
			ObjectData(const glm::mat4x3 &world_matrix,
				const glm::vec4 &color, const String &name,
				const float transparency, const Uint32 id,
				const SelectionType selection,
				const bool blend);

			/**
			 * Default destructor.
			 */
			~ObjectData() throw();

			inline void set_world_matrix(
				const glm::mat4x3 &world_matrix)
			{
				m_world_matrix = world_matrix;
			}

			inline void set_color(const glm::vec4 &color)
			{
				m_color = color;
			}

			inline void set_name(const String &name)
			{
				m_name = name;
			}

			inline void set_transparency(const float transparency)
			{
				m_transparency = transparency;
			}

			inline void set_blend(const bool blend)
			{
				m_blend = blend;
			}

			inline void set_selection(const SelectionType selection)
			{
				m_selection = selection;
			}

			inline void set_id(const Uint32 id)
			{
				m_id = id;
			}

			inline const glm::mat4x3 &get_world_matrix() const
			{
				return m_world_matrix;
			}

			inline const glm::vec4 &get_color() const
			{
				return m_color;
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

			inline bool get_blend() const
			{
				return m_blend;
			}

			inline SelectionType get_selection() const
			{
				return m_selection;
			}

	};

}

#endif	/* UUID_ab022a3e_b1f5_489b_8a77_2867286be10e */
