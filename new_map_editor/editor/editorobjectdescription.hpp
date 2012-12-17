/****************************************************************************
 *            editorobjectdescription.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_f62022f6_125d_4d0a_906c_3d68123c4896
#define	UUID_f62022f6_125d_4d0a_906c_3d68123c4896

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "objectdescription.hpp"

/**
 * @file
 * @brief The @c class EditorObjectDescription.
 * This file contains the @c class EditorObjectDescription.
 */
namespace eternal_lands
{

	/**
	 * @brief @c class for object data.
	 *
	 * @c class for object data.
	 */
	class EditorObjectDescription: public ObjectDescription
	{
		private:
			glm::vec3 m_rotation_angles;
			String m_description;

		public:
			/**
			 * Default constructor.
			 */
			EditorObjectDescription();

			/**
			 * Default constructor.
			 */
			EditorObjectDescription(const glm::vec3 &translation,
				const glm::vec3 &rotation_angles,
				const glm::vec3 &scale,
				const StringVector &material_names,
				const String &name, const BitSet64 blend_mask,
				const String &description,
				const float transparency, const float glow,
				const Uint32 id, const SelectionType selection,
				const BlendType blend);

			/**
			 * Default destructor.
			 */
			~EditorObjectDescription() throw();
			void set_translation(const glm::vec3 &translation);
			void set_rotation_angles(
				const glm::vec3 &rotation_angles);
			void set_scale(const glm::vec3 &scale);
			const glm::vec3 &get_translation() const;
			const glm::vec3 &get_scale() const;

			inline const glm::vec3 &get_rotation_angles() const
				noexcept
			{
				return m_rotation_angles;
			}

			inline void set_description(const String description)
				noexcept
			{
				m_description = description;
			}

			inline String get_description() const noexcept
			{
				return m_description;
			}

	};

	typedef std::vector<EditorObjectDescription>
		EditorObjectDescriptionVector;

}

#endif	/* UUID_f62022f6_125d_4d0a_906c_3d68123c4896 */
