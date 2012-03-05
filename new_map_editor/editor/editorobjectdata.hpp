/****************************************************************************
 *            editorobjectdata.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_f62022f6_125d_4d0a_906c_3d68123c4896
#define	UUID_f62022f6_125d_4d0a_906c_3d68123c4896

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "objectdata.hpp"

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
	class EditorObjectData: public ObjectData
	{
		private:
			StringVector m_materials;
			glm::vec3 m_rotation_angles;

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
				const StringVector &materials,
				const String &name, const float transparency,
				const float scale, const Uint32 id,
				const SelectionType selection,
				const BlendType blend);

			/**
			 * Default destructor.
			 */
			~EditorObjectData() throw();

			inline void set_rotation_angles(
				const glm::vec3 &rotation_angles)
			{
				m_rotation_angles = rotation_angles;
			}

			inline void set_materials(const StringVector &materials)
			{
				m_materials = materials;
			}

			inline const glm::vec3 &get_rotation_angles() const
			{
				return m_rotation_angles;
			}

			inline const StringVector &get_materials() const
			{
				return m_materials;
			}

	};

}

#endif	/* UUID_f62022f6_125d_4d0a_906c_3d68123c4896 */
