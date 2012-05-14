/****************************************************************************
 *            sceneview.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
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
			glm::mat3x3 m_view_rotation_matrix;
			glm::mat4x4 m_view_matrix;
			Mat4x4Vector m_projection_matrices;
			Mat4x4Vector m_projection_view_matrices;
			glm::mat3x3 m_shadow_view_rotation_matrix;
			glm::mat4x4 m_shadow_view_matrix;
			Mat4x4Vector m_split_projection_view_matrices;
			Mat4x4Vector m_shadow_projection_matrices;
			Mat4x4Vector m_shadow_projection_view_matrices;
			Mat4x4Vector m_shadow_texture_matrices;
			glm::mat3x3 *m_current_view_rotation_matrix;
			glm::mat4x4 *m_current_view_matrix;
			Mat4x4Vector *m_current_projection_matrices;
			Mat4x4Vector *m_current_projection_view_matrices;
			glm::mat3x3 m_ortho_rotation_matrix;
			Mat4x4Vector m_ortho_matrices;
			glm::vec4 m_camera;
			glm::vec4 m_shadow_camera;
			glm::vec4 m_view_dir;
			glm::vec4 m_focus;
			glm::vec4 m_split_distances;
			glm::vec4 m_z_params;
			glm::ivec4 m_view_port;
			glm::uvec2 m_window_size;
			glm::uvec2 m_shadow_map_size;
			float m_fov, m_aspect, m_z_near, m_z_far;
			float m_shadow_z_far;
			Uint16 m_shadow_map_count, m_layer_count;
			bool m_exponential_shadow_maps;

			void build_shadow_matrix(
				const glm::dmat4x4 &shadow_view_matrix,
				const glm::dmat4x4 &basic_projection_matrix,
				const glm::dmat4x4 &basic_projection_view_matrix,
				const ConvexBody &convex_body,
				const Uint16 index);

			void build_shadow_matrix(
				const glm::dmat4x4 &shadow_view_matrix,
				const glm::dmat4x4 &basic_projection_matrix,
				const glm::dmat4x4 &basic_projection_view_matrix,
				const ConvexBody &convex_body,
				const glm::vec3 &dir, const float max_height,
				const Uint16 index);

			inline GlobalVarsSharedPtr get_global_vars() const
				noexcept
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
			~SceneView() noexcept;

			void update();
			void build_shadow_matrices(
				const glm::vec3 &light_direction,
				const SubFrustumsConvexBodys &convex_bodys,
				const float scene_max_height);
			void update_shadow_matrices(
				const SubFrustumsConvexBodys &convex_bodys,
				const SubFrustumsMask &mask);

			inline void set_ortho_view() noexcept
			{
				m_current_view_rotation_matrix =
					&m_ortho_rotation_matrix;
				m_current_view_matrix = &m_ortho_matrices[0];
				m_current_projection_matrices =
					&m_ortho_matrices;
				m_current_projection_view_matrices =
					&m_ortho_matrices;
			}

			inline void set_default_view() noexcept
			{
				m_current_view_rotation_matrix =
					&m_view_rotation_matrix;
				m_current_view_matrix = &m_view_matrix;
				m_current_projection_matrices =
					&m_projection_matrices;
				m_current_projection_view_matrices =
					&m_projection_view_matrices;
			}

			inline void set_shadow_view() noexcept
			{
				m_current_view_rotation_matrix =
					&m_shadow_view_rotation_matrix;
				m_current_view_matrix = &m_shadow_view_matrix;
				m_current_projection_matrices =
					&m_shadow_projection_matrices;
				m_current_projection_view_matrices =
					&m_shadow_projection_view_matrices;
			}

			inline void set_perspective(const float fov,
				const float aspect, const float z_near) noexcept
			{
				m_fov = fov;
				m_aspect = aspect;
				m_z_near = z_near;
			}

			inline void set_view_port(const glm::uvec4 &view_port)
				noexcept
			{
				m_view_port = view_port;
			}

			inline void set_window_size(
				const glm::uvec2 &window_size) noexcept
			{
				m_window_size = window_size;
			}

			inline void set_focus(const glm::vec3 &focus) noexcept
			{
				m_focus = glm::vec4(focus, 1.0f);
			}

			inline void set_view_matrix(
				const glm::mat4 &view_matrix) noexcept
			{
				m_view_matrix = view_matrix;
			}

			inline const glm::mat3x3
				&get_current_view_rotation_matrix() const
				noexcept
			{
				return *m_current_view_rotation_matrix;
			}

			inline const glm::mat4x4 &get_current_view_matrix()
				const noexcept
			{
				return *m_current_view_matrix;
			}

			inline const Mat4x4Vector
				&get_current_projection_matrices() const
				noexcept
			{
				return *m_current_projection_matrices;
			}

			inline const Mat4x4Vector
				&get_current_projection_view_matrices() const
				noexcept
			{
				return *m_current_projection_view_matrices;
			}

			inline const glm::mat3x3 &get_view_rotation_matrix()
				const noexcept
			{
				return m_view_rotation_matrix;
			}

			inline const glm::mat4x4 &get_view_matrix() const
				noexcept
			{
				return m_view_matrix;
			}

			inline const Mat4x4Vector &get_projection_matrices()
				const noexcept
			{
				return m_projection_matrices;
			}

			inline const Mat4x4Vector
				&get_projection_view_matrices() const noexcept
			{
				return m_projection_view_matrices;
			}

			inline const Mat4x4Vector
				&get_split_projection_view_matrices() const
				noexcept
			{
				return m_split_projection_view_matrices;
			}

			inline const glm::mat3x3
				&get_shadow_view_rotation_matrix() const
				noexcept
			{
				return m_shadow_view_rotation_matrix;
			}

			inline const glm::mat4x4 &get_shadow_view_matrix()
				const noexcept
			{
				return m_shadow_view_matrix;
			}

			inline const Mat4x4Vector
				&get_shadow_projection_view_matrices() const
				noexcept
			{
				return m_shadow_projection_view_matrices;
			}

			inline const Mat4x4Vector
				&get_shadow_texture_matrices() const noexcept
			{
				return m_shadow_texture_matrices;
			}

			inline const glm::ivec4 &get_view_port() const noexcept
			{
				return m_view_port;
			}

			inline const glm::vec4 &get_camera() const noexcept
			{
				return m_camera;
			}

			inline const glm::vec4 &get_view_dir() const noexcept
			{
				return m_view_dir;
			}

			inline const glm::vec4 &get_shadow_camera() const
				noexcept
			{
				return m_shadow_camera;
			}

			inline const glm::vec4 &get_split_distances() const
				noexcept
			{
				return m_split_distances;
			}

			inline const glm::vec4 &get_focus() const noexcept
			{
				return m_focus;
			}

			inline const glm::vec4 &get_z_params() const noexcept
			{
				return m_z_params;
			}

			inline float get_fov() const noexcept
			{
				return m_fov;
			}

			inline float get_aspect() const noexcept
			{
				return m_aspect;
			}

			inline float get_z_near() const noexcept
			{
				return m_z_near;
			}

			inline float get_z_far() const noexcept
			{
				return m_z_far;
			}

			inline float get_shadow_z_far() const noexcept
			{
				return m_shadow_z_far;
			}

			inline const glm::uvec2 &get_window_size() const
				noexcept
			{
				return m_window_size;
			}

			inline const glm::uvec2 &get_shadow_map_size() const
				noexcept
			{
				return m_shadow_map_size;
			}

			inline Uint16 get_shadow_map_count() const noexcept
			{
				return m_shadow_map_count;
			}

			inline Uint16 get_layer_count() const noexcept
			{
				return m_layer_count;
			}

			inline bool get_exponential_shadow_maps() const noexcept
			{
				return m_exponential_shadow_maps;
			}

	};

}

#endif	/* UUID_8899c0b4_cbe9_4424_9e9c_193ababb8980 */
