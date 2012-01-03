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
			glm::mat4x4 m_ortho_projection_matrix;
			Mat4x4Vector m_projection_view_matrices;
			Mat4x4Vector m_shadow_projection_matrices;
			Mat4x4Vector m_shadow_projection_view_matrices;
			Mat4x4Vector m_shadow_texture_matrices;
			glm::mat4x4 *m_current_view_matrix;
			glm::mat4x4 *m_current_projection_matrix;
			glm::mat4x4 *m_current_projection_view_matrix;
			glm::vec4 m_camera;
			glm::vec4 m_shadow_camera;
			glm::vec4 m_view_dir;
			glm::vec4 m_focus;
			glm::vec4 m_split_distances;
			glm::ivec4 m_view_port;
			glm::uvec2 m_window_size;
			glm::uvec2 m_shadow_map_size;
			float m_fov, m_aspect, m_z_near, m_z_far;
			float m_shadow_z_far;
			Uint16 m_shadow_map_count;
			bool m_exponential_shadow_maps;

			void build_shadow_matrix(
				const glm::mat4x4 &basic_projection_matrix,
				const glm::mat4x4 &basic_projection_view_matrix,
				const ConvexBody &convex_body,
				const Uint16 index);

			void build_shadow_matrix(
				const glm::mat4x4 &basic_projection_matrix,
				const glm::mat4x4 &basic_projection_view_matrix,
				const ConvexBody &convex_body,
				const glm::vec3 &dir, const float max_height,
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
				const SubFrustumsConvexBodys &convex_bodys,
				const float scene_max_height);
			void update_shadow_matrices(
				const SubFrustumsConvexBodys &convex_bodys);

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

			inline void set_view_port(const glm::uvec4 &view_port)
			{
				m_view_port = view_port;
			}

			inline void set_window_size(
				const glm::uvec2 &window_size)
			{
				m_window_size = window_size;
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

			inline const glm::mat4x4 &get_ortho_projection_matrix()
				const
			{
				return m_ortho_projection_matrix;
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

			inline const glm::uvec2 &get_window_size() const
			{
				return m_window_size;
			}

			inline const glm::uvec2 &get_shadow_map_size() const
			{
				return m_shadow_map_size;
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
