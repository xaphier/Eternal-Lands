/****************************************************************************
 *            clipmapterrain.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
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
 * @brief The @c class ClipmapTerrain.
 * This file contains the @c class ClipmapTerrain.
 */
namespace eternal_lands
{

	class ClipmapTerrain
	{
		private:
			const MaterialBuilderWeakPtr m_material_builder;
			Mat2x3Vector m_texture_matrices;
			glm::vec4 m_terrain_texture_size;
			glm::uvec2 m_slices;
			glm::vec2 m_terrain_world_size;
			glm::vec2 m_focus;
			float m_world_size;
			float m_distance;
			Uint16 m_size;
			Uint16 m_dir_index;
			bool m_center_at_focus;

			inline MaterialBuilderSharedPtr get_material_builder()
				const noexcept
			{
				MaterialBuilderSharedPtr result;

				result = m_material_builder.lock();

				assert(result.get() != nullptr);

				return result;
			}

			inline Uint16 get_dir_index() const noexcept
			{
				return m_dir_index;
			}

			Uint16 get_dir_index(const glm::vec2 &view_dir) const
				noexcept;

		public:
			ClipmapTerrain(
				const MaterialBuilderWeakPtr &material_builder);
			~ClipmapTerrain() noexcept;
			void rebuild(const glm::vec2 &terrain_world_size,
				const float view_distance,
				const float world_size,	const Uint16 size,
				const Uint16 slices);
			bool get_update_needed(const glm::vec2 &camera,
				const glm::vec2 &view_dir,
				const glm::vec2 &focus);
			void update_slice(const Uint16 slice);

			inline const Mat2x3Vector &get_texture_matrices() const
				noexcept
			{
				return m_texture_matrices;
			}

			inline const glm::vec4 &get_terrain_texture_size() const
				noexcept
			{
				return m_terrain_texture_size;
			}

			inline const glm::vec2 &get_terrain_world_size() const
				noexcept
			{
				return m_terrain_world_size;
			}

			inline const glm::vec2 &get_focus() const noexcept
			{
				return m_focus;
			}

			inline float get_world_size() const noexcept
			{
				return m_world_size;
			}

			inline Uint32 get_all_slices() const noexcept
			{
				return std::max(m_slices.x, m_slices.y);
			}

			inline Uint32 get_albedo_slices() const noexcept
			{
				return m_slices.x;
			}

			inline Uint32 get_normal_slices() const noexcept
			{
				return m_slices.y;
			}

			inline Uint32 get_specular_gloss_slices() const noexcept
			{
				return m_slices.y;
			}

			inline const glm::uvec2 &get_slice_data() const noexcept
			{
				return m_slices;
			}

			inline BitSet64 get_update_mask() const
			{
				return (1 << get_all_slices()) - 1;
			}

			inline bool get_center_at_focus() const noexcept
			{
				return m_center_at_focus;
			}

			inline void set_center_at_focus(
				const bool center_at_focus) noexcept
			{
				m_center_at_focus = center_at_focus;
			}

	};

}

#endif	/* UUID_be9838f9_d41e_4dc0_9ba5_845dfef2f914 */
