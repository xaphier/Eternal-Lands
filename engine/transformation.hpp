/****************************************************************************
 *            transformation.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_a5bd43ed_25fa_4620_a796_5f1c80088dd6
#define	UUID_a5bd43ed_25fa_4620_a796_5f1c80088dd6

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class Transformation.
 * This file contains the @c class Transformation.
 */
namespace eternal_lands
{

	/**
	 * @brief @c class for transformations.
	 *
	 * @c class for transformations.
	 */
	class Transformation
	{
		private:
			glm::mat4x3 m_matrix;
			glm::quat m_rotation;
			glm::vec3 m_translation;
			glm::vec3 m_scale;

			inline void rebuild()
			{
				glm::mat4 matrix;

				matrix = glm::translate(get_translation());
				matrix *= glm::mat4(glm::mat3_cast(
					get_rotation()));
				matrix = glm::scale(matrix, get_scale());

				m_matrix = glm::mat4x3(matrix);
			}

		public:
			inline Transformation(): m_scale(1.0f)
			{
			}

			inline ~Transformation() noexcept
			{
			}

			inline void set_rotation(const glm::quat &rotation)
				noexcept
			{
				m_rotation = rotation;

				rebuild();
			}

			inline void set_translation(
				const glm::vec3 &translation) noexcept
			{
				m_translation = translation;

				rebuild();
			}

			inline void set_scale(const glm::vec3 &scale) noexcept
			{
				m_scale = scale;

				rebuild();
			}

			inline const glm::quat &get_rotation() const noexcept
			{
				return m_rotation;
			}

			inline const glm::vec3 &get_translation() const noexcept
			{
				return m_translation;
			}

			inline const glm::vec3 &get_scale() const noexcept
			{
				return m_scale;
			}

			inline const glm::mat4x3 &get_matrix() const noexcept
			{
				return m_matrix;
			}

			inline void set_rotation_angles(
				const glm::vec3 &rotation_angles) noexcept
			{
				glm::quat rotation;

				rotation = glm::quat();
				rotation = glm::rotate(rotation,
					rotation_angles.z,
					glm::vec3(0.0f, 0.0f, 1.0f));
				rotation = glm::rotate(rotation,
					rotation_angles.x,
					glm::vec3(1.0f, 0.0f, 0.0f));
				rotation = glm::rotate(rotation,
					rotation_angles.y,
					glm::vec3(0.0f, 1.0f, 0.0f));

				set_rotation(rotation);
			}

			inline glm::vec3 transform_point(const glm::vec3 &point)
				const noexcept
			{
				return get_matrix() * glm::vec4(point, 1.0f);
			}

			inline bool get_flip_back_face_culling() const
			{
				return (get_scale().x < 0) ^
					(get_scale().y < 0) ^
					(get_scale().z < 0);
			}

	};

}

#endif	/* UUID_a5bd43ed_25fa_4620_a796_5f1c80088dd6 */
