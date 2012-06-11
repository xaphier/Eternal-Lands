/****************************************************************************
 *            abstractterrainmanager.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_45c1c534_56a4_4295_a35c_b836fe76721d
#define	UUID_45c1c534_56a4_4295_a35c_b836fe76721d

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "terraindata.hpp"
#include "boundingbox.hpp"

/**
 * @file
 * @brief The @c class AbstractTerrainManager.
 * This file contains the @c class AbstractTerrainManager.
 */
namespace eternal_lands
{

	class AbstractTerrainManager: public boost::noncopyable
	{
		private:
			BoundingBox m_bounding_box;
			glm::vec4 m_terrain_size_data;
			glm::vec3 m_translation;

		protected:
			AbstractTerrainManager();

			inline void set_bounding_box(
				const BoundingBox &bounding_box) noexcept
			{
				m_bounding_box = bounding_box;
			}

			inline void set_translation(
				const glm::vec3 &translation) noexcept
			{
				m_translation = translation;
			}

			inline void set_terrain_size(
				const glm::vec2 &terrain_size) noexcept
			{
				m_terrain_size_data.x = terrain_size.x;
				m_terrain_size_data.y = terrain_size.y;
				m_terrain_size_data.z = 1.0f / terrain_size.x;
				m_terrain_size_data.w = 1.0f / terrain_size.y;
			}

		public:
			virtual ~AbstractTerrainManager() noexcept;
			virtual void intersect(const Frustum &frustum,
				ObjectVisitor &visitor) const = 0;
			virtual void intersect(const Frustum &frustum,
				const glm::vec3 &camera,
				BoundingBox &bounding_box) const = 0;
			virtual void intersect(const Frustum &frustum,
				const glm::vec3 &camera,
				TerrainVisitor &terrain) const = 0;
			virtual void set_clipmap_texture(
				const TextureSharedPtr &texture) = 0;
			virtual void update(const ImageSharedPtr &vector_map,
				const ImageSharedPtr &normal_map,
				const ImageSharedPtr &dudv_map) = 0;
			static const glm::vec3 &get_vector_scale() noexcept;

			inline const BoundingBox &get_bounding_box() const
				noexcept
			{
				return m_bounding_box;
			}

			inline const glm::vec3 &get_translation() const
				noexcept
			{
				return m_translation;
			}

			inline glm::vec2 get_terrain_size() const noexcept
			{
				return glm::vec2(m_terrain_size_data);
			}

			inline const glm::vec4 &get_terrain_size_data() const
				noexcept
			{
				return m_terrain_size_data;
			}

			static inline Uint16 get_tile_size() noexcept
			{
				return 32;
			}

			static inline float get_patch_scale() noexcept
			{
				return 0.25f;
			}

	};

}

#endif	/* UUID_45c1c534_56a4_4295_a35c_b836fe76721d */
