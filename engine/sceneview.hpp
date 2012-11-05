/****************************************************************************
 *            sceneview.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
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

	enum ProjectionType
	{
		pt_perspective = 0,
		pt_ortho = 1
	};

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
			glm::mat4x4 m_scale_view_matrix;
			glm::mat4x4 m_ortho_matrix;
			Mat4x4Vector m_split_projection_view_matrices;
			Mat4x4Vector m_shadow_view_matrices;
			Mat4x4Vector m_shadow_projection_matrices;
			Mat4x4Vector m_shadow_projection_view_matrices;
			Mat4x4Vector m_shadow_texture_matrices;
			Vec4Vector m_shadow_distance_transforms;
			Vec4Vector m_shadow_cameras;
			glm::mat4x4 *m_current_view_matrix;
			glm::mat4x4 *m_current_projection_matrix;
			glm::mat4x4 *m_current_projection_view_matrix;
			glm::vec4 m_camera;
			glm::vec4 m_view_dir;
			glm::vec4 m_focus;
			glm::vec4 m_split_distances;
			glm::vec4 m_z_params;
			glm::vec4 m_ortho;
			glm::uvec4 m_view_port;
			Uint32 m_shadow_map_size;
			float m_fov, m_aspect, m_z_near, m_z_far;
			float m_shadow_z_far;
			Uint16 m_shadow_map_count, m_layer_count;
			Uint16 m_shadow_view_index;
			bool m_exponential_shadow_maps;
			ProjectionType m_projection;

			glm::mat4 build_projection_matrix(const float z_near,
				const float z_far) const;

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
				const float scene_max_height,
				const Uint16 index);
			void set_scale_view(const BoundingBox &bounding_box);

			inline void set_ortho_view() noexcept
			{
				m_current_view_matrix = &m_ortho_matrix;
				m_current_projection_matrix =
					&m_ortho_matrix;
				m_current_projection_view_matrix =
					&m_ortho_matrix;
			}

			inline void set_ortho_scale_view() noexcept
			{
				m_current_view_matrix = &m_scale_view_matrix;
				m_current_projection_matrix =
					&m_ortho_matrix;
				m_current_projection_view_matrix =
					&m_scale_view_matrix;
			}

			inline void set_default_view() noexcept
			{
				m_current_view_matrix = &m_view_matrix;
				m_current_projection_matrix =
					&m_projection_matrix;
				m_current_projection_view_matrix =
					&m_projection_view_matrix;
			}

			inline void set_shadow_view(const Uint16 index) noexcept
			{
				m_current_view_matrix =
					&m_shadow_view_matrices[index];
				m_current_projection_matrix =
					&m_shadow_projection_matrices[index];
				m_current_projection_view_matrix =
					&m_shadow_projection_view_matrices[
						index];
				m_shadow_view_index = index;
			}

			inline void set_z_near(const float z_near) noexcept
			{
				m_z_near = z_near;
			}

			inline void set_perspective(const float fov,
				const float aspect) noexcept
			{
				m_fov = fov;
				m_aspect = aspect;
				m_projection = pt_perspective;
			}

			inline void set_ortho(const glm::vec4 &ortho) noexcept
			{
				m_ortho = ortho;
				m_projection = pt_ortho;
			}

			inline void set_view_port(const glm::uvec4 &view_port)
				noexcept
			{
				m_view_port = view_port;
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

			inline const glm::mat4x4 &get_current_view_matrix()
				const noexcept
			{
				return *m_current_view_matrix;
			}

			inline const glm::mat4x4
				&get_current_projection_matrix() const
				noexcept
			{
				return *m_current_projection_matrix;
			}

			inline const glm::mat4x4
				&get_current_projection_view_matrix() const
				noexcept
			{
				return *m_current_projection_view_matrix;
			}

			inline const glm::mat4x4 &get_view_matrix() const
				noexcept
			{
				return m_view_matrix;
			}

			inline const glm::mat4x4 &get_projection_matrix()
				const noexcept
			{
				return m_projection_matrix;
			}

			inline const glm::mat4x4
				&get_projection_view_matrix() const noexcept
			{
				return m_projection_view_matrix;
			}

			inline const glm::mat4x4 &get_scale_view_matrix() const
				noexcept
			{
				return m_scale_view_matrix;
			}

			inline const glm::mat4x4 &get_ortho_matrix()
				const noexcept
			{
				return m_ortho_matrix;
			}

			inline const Mat4x4Vector
				&get_split_projection_view_matrices() const
				noexcept
			{
				return m_split_projection_view_matrices;
			}

			inline const Mat4x4Vector &get_shadow_view_matrices()
				const noexcept
			{
				return m_shadow_view_matrices;
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

			inline const glm::uvec4 &get_view_port() const noexcept
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

			inline const Vec4Vector &get_shadow_cameras() const
				noexcept
			{
				return m_shadow_cameras;
			}

			inline const Vec4Vector
				&get_shadow_distance_transforms() const
				noexcept
			{
				return m_shadow_distance_transforms;
			}

			inline const glm::vec4
				&get_current_shadow_distance_transform() const
				noexcept
			{
				return get_shadow_distance_transforms()[
					get_shadow_view_index()];
			}

			inline const glm::vec4 &get_split_distances() const
				noexcept
			{
				return m_split_distances;
			}

			inline const glm::vec4 &get_ortho() const noexcept
			{
				return m_ortho;
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

			inline glm::uvec2 get_shadow_map_size() const noexcept
			{
				return glm::uvec2(m_shadow_map_size);
			}

			inline Uint16 get_shadow_map_count() const noexcept
			{
				return m_shadow_map_count;
			}

			inline Uint16 get_layer_count() const noexcept
			{
				return m_layer_count;
			}

			inline Uint16 get_shadow_view_index() const noexcept
			{
				return m_shadow_view_index;
			}

			inline bool get_exponential_shadow_maps() const noexcept
			{
				return m_exponential_shadow_maps;
			}

			inline ProjectionType get_projection() const
			{
				return m_projection;
			}

	};

}

#endif	/* UUID_8899c0b4_cbe9_4424_9e9c_193ababb8980 */
