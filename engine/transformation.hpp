/****************************************************************************
 *            transformation.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
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
			glm::mat2x4 m_data;

		public:
			inline Transformation()
			{
				set_rotation(glm::quat());
				set_translation(glm::vec3(0.0f));
				set_scale(1.0f);
			}

			inline ~Transformation() throw()
			{
			}

			inline void set_rotation(const glm::quat &rotation)
			{
				m_data[0].x = rotation.x;
				m_data[0].y = rotation.y;
				m_data[0].z = rotation.z;
				m_data[0].w = rotation.w;
			}

			inline void set_translation(const glm::vec3 &translate)
			{
				m_data[1].x = translate.x;
				m_data[1].y = translate.y;
				m_data[1].z = translate.z;
			}

			inline void set_scale(const float scale)
			{
				m_data[1].w = scale;
			}

			inline glm::quat get_rotation() const
			{
				return glm::quat(m_data[0].w, m_data[0].x,
					m_data[0].y, m_data[0].z);
			}

			inline glm::vec3 get_translation() const
			{
				return glm::vec3(m_data[1]);
			}

			inline float get_scale() const
			{
				return m_data[1].w;
			}

			inline glm::mat4x3 get_matrix() const
			{
				glm::mat4x3 result;

				result = glm::mat4x3(glm::mat3_cast(
					get_rotation()) * get_scale());
				result[3] = get_translation();

				return result;
			}

			inline const glm::mat2x4 &get_data() const
			{
				return m_data;
			}

			inline glm::vec3 transform_point(const glm::vec3 &point)
				const
			{
				glm::vec3 result;

				result = get_rotation() * point;
				result *= get_scale();
				result += get_translation();

				return result;
			}

	};

}

#endif	/* UUID_a5bd43ed_25fa_4620_a796_5f1c80088dd6 */
