/****************************************************************************
 *            clipmap.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_be9838f9_d41e_4dc0_9ba5_845dfef2f914
#define	UUID_be9838f9_d41e_4dc0_9ba5_845dfef2f914

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class Clipmap.
 * This file contains the @c class Clipmap.
 */
namespace eternal_lands
{

	class Clipmap
	{
		private:
			Mat2x3Vector m_texture_matrices;
			AbstractFrameBufferSharedPtr m_terrain_frame_buffer;
			glm::vec4 m_terrain_texture_size;
			glm::vec2 m_focus;
			glm::vec2 m_terrain_world_size;
			float m_world_size;
			float m_distance;
			Uint16 m_size;
			Uint16 m_dir_index;
			Uint16 m_slices;
			bool m_centered;

			Uint16 get_dir_index(const glm::vec2 &view_dir) const;

			inline Uint16 get_dir_index() const
			{
				return m_dir_index;
			}

		public:
			Clipmap();
			~Clipmap() throw();
			void rebuild(const glm::vec2 &terrain_world_size,
				const float view_distance,
				const float world_size,	const Uint16 size,
				const Uint16 slices);
			bool update(const glm::vec3 &camera,
				const glm::vec3 &view_dir,
				const glm::vec2 &focus);
			void update_slice(const Uint16 slice);

			inline void set_centered(const bool centered)
			{
				m_centered = centered;
			}

			inline const Mat2x3Vector &get_texture_matrices() const
			{
				return m_texture_matrices;
			}

			inline const glm::vec4 &get_terrain_texture_size() const
			{
				return m_terrain_texture_size;
			}

			inline const glm::vec2 &get_terrain_world_size() const
			{
				return m_terrain_world_size;
			}

			inline const glm::vec2 &get_focus() const
			{
				return m_focus;
			}

			inline float get_world_size() const
			{
				return m_world_size;
			}

			inline Uint16 get_slices() const
			{
				return m_slices;
			}

			inline bool get_centered() const
			{
				return m_centered;
			}

	};

}

#endif	/* UUID_be9838f9_d41e_4dc0_9ba5_845dfef2f914 */
