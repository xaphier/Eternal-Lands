/****************************************************************************
 *            sceneview.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_8899c0b4_cbe9_4424_9e9c_193ababb8980
#define	UUID_8899c0b4_cbe9_4424_9e9c_193ababb8980

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class SceneView.
 * This file contains the @c class SceneView.
 */
namespace eternal_lands
{

	/**
	 * @brief @c class for scene view.
	 *
	 * @c class for scene view.
	 */
	class SceneView
	{
		private:
			GlobalVarsSharedPtr m_global_vars;
			glm::mat4x4 m_view_matrix;
			glm::mat4x4 m_projection_matrix;
			glm::mat4x4 m_projection_view_matrix;
			glm::mat4x4 m_shadow_view_matrix;
			Mat4x4Vector m_projection_view_matrices;
			Mat4x4Vector m_shadow_projection_matrices;
			Mat4x4Vector m_shadow_projection_view_matrices;
			Mat4x4Vector m_shadow_texture_matrices;
			glm::mat4x4 m_shadow_projection_matrix;
			glm::mat4x4 m_shadow_projection_view_matrix;
			glm::mat4x4 *m_current_view_matrix;
			glm::mat4x4 *m_current_projection_matrix;
			glm::mat4x4 *m_current_projection_view_matrix;
			glm::vec4 m_camera;
			glm::vec4 m_shadow_camera;
			glm::vec4 m_view_dir;
			glm::vec4 m_focus;
			glm::vec4 m_split_distances;
			glm::ivec4 m_view_port;
			float m_fov, m_aspect, m_z_near, m_z_far;
			float m_shadow_z_far;
			Uint32 m_shadow_map_width;
			Uint32 m_shadow_map_height;
			Uint16 m_shadow_map_count;
			bool m_exponential_shadow_maps;

			void build_shadow_matrix(
				const glm::mat4x4 &basic_projection_matrix,
				const glm::mat4x4 &basic_projection_view_matrix,
				const BoundingBox &split_box,
				const BoundingBox &receiver_box,
				const BoundingBox &caster_box,
				const Uint16 index);

			inline GlobalVarsSharedPtr get_global_vars() const
			{
				return m_global_vars;
			}

		public:
			/**
			 * Default constructor.
			 */
			SceneView(const GlobalVarsSharedPtr &global_vars);

			/**
			 * Default destructor.
			 */
			~SceneView() throw();

			void update();
			void build_shadow_matrices(
				const glm::vec3 &light_direction,
				const SubFrustumsBoundingBoxes &split_boxes,
				const SubFrustumsBoundingBoxes &receiver_boxes,
				const float scene_max_height);
			void update_shadow_matrices(
				const SubFrustumsBoundingBoxes &split_boxes,
				const SubFrustumsBoundingBoxes &receiver_boxes,
				const SubFrustumsBoundingBoxes &caster_boxes);

			inline void set_default_view()
			{
				m_current_view_matrix = &m_view_matrix;
				m_current_projection_matrix =
					&m_projection_matrix;
				m_current_projection_view_matrix =
					&m_projection_view_matrix;
			}

			inline void set_shadow_view(const Uint16 index)
			{
				m_current_view_matrix = &m_shadow_view_matrix;
				m_current_projection_matrix =
					&m_shadow_projection_matrices[index];
				m_current_projection_view_matrix =
					&m_shadow_projection_view_matrices[
						index];
			}

			inline void set_perspective(const float fov,
				const float aspect, const float z_near)
			{
				m_fov = fov;
				m_aspect = aspect;
				m_z_near = z_near;
			}

			inline void set_focus(const glm::vec3 &focus)
			{
				m_focus = glm::vec4(focus, 1.0f);
			}

			inline const glm::mat4x4 &get_current_view_matrix()
				const
			{
				return *m_current_view_matrix;
			}

			inline const glm::mat4x4
				&get_current_projection_matrix() const
			{
				return *m_current_projection_matrix;
			}

			inline const glm::mat4x4
				&get_current_projection_view_matrix() const
			{
				return *m_current_projection_view_matrix;
			}

			inline const glm::mat4x4 &get_view_matrix() const
			{
				return m_view_matrix;
			}

			inline const glm::mat4x4 &get_projection_matrix()
				const
			{
				return m_projection_matrix;
			}

			inline const glm::mat4x4
				&get_projection_view_matrix() const
			{
				return m_projection_view_matrix;
			}

			inline const Mat4x4Vector
				&get_projection_view_matrices() const
			{
				return m_projection_view_matrices;
			}

			inline const glm::mat4x4 &get_shadow_view_matrix() const
			{
				return m_shadow_view_matrix;
			}

			inline const glm::mat4x4 &get_shadow_projection_matrix(
				const Uint16 index) const
			{
				return m_shadow_projection_matrices[index];
			}

			inline const glm::mat4x4
				&get_shadow_projection_view_matrix(
					const Uint16 index) const
			{
				return m_shadow_projection_view_matrices[index];
			}

			inline const Mat4x4Vector
				&get_shadow_projection_view_matrices() const
			{
				return m_shadow_projection_view_matrices;
			}

			inline const Mat4x4Vector
				&get_shadow_texture_matrices() const
			{
				return m_shadow_texture_matrices;
			}

			inline const glm::mat4x4 &get_shadow_projection_matrix(
				) const
			{
				return m_shadow_projection_matrix;
			}

			inline const glm::mat4x4
				&get_shadow_projection_view_matrix() const
			{
				return m_shadow_projection_view_matrix;
			}

			inline const glm::ivec4 &get_view_port() const
			{
				return m_view_port;
			}

			inline const glm::vec4 &get_camera() const
			{
				return m_camera;
			}

			inline const glm::vec4 &get_shadow_camera() const
			{
				return m_shadow_camera;
			}

			inline const glm::vec4 &get_split_distances() const
			{
				return m_split_distances;
			}

			inline const glm::vec4 &get_focus() const
			{
				return m_focus;
			}

			inline float get_fov() const
			{
				return m_fov;
			}

			inline float get_aspect() const
			{
				return m_aspect;
			}

			inline float get_z_near() const
			{
				return m_z_near;
			}

			inline float get_z_far() const
			{
				return m_z_far;
			}

			inline float get_shadow_z_far() const
			{
				return m_shadow_z_far;
			}

			inline Uint32 get_shadow_map_width() const
			{
				return m_shadow_map_width;
			}

			inline Uint32 get_shadow_map_height() const
			{
				return m_shadow_map_height;
			}

			inline Uint16 get_shadow_map_count() const
			{
				return m_shadow_map_count;
			}

			inline bool get_exponential_shadow_maps() const
			{
				return m_exponential_shadow_maps;
			}

	};

}

#endif	/* UUID_8899c0b4_cbe9_4424_9e9c_193ababb8980 */
