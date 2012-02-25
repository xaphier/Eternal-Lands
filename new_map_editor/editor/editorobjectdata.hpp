/****************************************************************************
 *            editorobjectdata.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_f62022f6_125d_4d0a_906c_3d68123c4896
#define	UUID_f62022f6_125d_4d0a_906c_3d68123c4896

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "selectionutil.hpp"

/**
 * @file
 * @brief The @c class EditorObjectData.
 * This file contains the @c class EditorObjectData.
 */
namespace eternal_lands
{

	/**
	 * @brief @c class for object data.
	 *
	 * @c class for object data.
	 */
	class EditorObjectData
	{
		private:
			glm::vec3 m_translation;
			glm::vec3 m_rotation_angles;
			String m_name;
			float m_transparency;
			float m_scale;
			Uint32 m_id;
			SelectionType m_selection;
			bool m_blend;

		public:
			/**
			 * Default constructor.
			 */
			EditorObjectData();

			/**
			 * Default constructor.
			 */
			EditorObjectData(const glm::vec3 &translation,
				const glm::vec3 &rotation_angles,
				const String &name, const float transparency,
				const float scale, const Uint32 id,
				const SelectionType selection,
				const bool blend);

			/**
			 * Default destructor.
			 */
			~EditorObjectData() throw();

			inline void set_translation(
				const glm::vec3 &translation)
			{
				m_translation = translation;
			}

			inline void set_rotation_angles(
				const glm::vec3 &rotation_angles)
			{
				m_rotation_angles = rotation_angles;
			}

			inline void set_name(const String &name)
			{
				m_name = name;
			}

			inline void set_transparency(const float transparency)
			{
				m_transparency = transparency;
			}

			inline void set_scale(const float scale)
			{
				m_scale = scale;
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

			inline const glm::vec3 &get_translation() const
			{
				return m_translation;
			}

			inline const glm::vec3 &get_rotation_angles() const
			{
				return m_rotation_angles;
			}

			inline const String &get_name() const
			{
				return m_name;
			}

			inline float get_transparency() const
			{
				return m_transparency;
			}

			inline float get_scale() const
			{
				return m_scale;
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

#endif	/* UUID_f62022f6_125d_4d0a_906c_3d68123c4896 */
