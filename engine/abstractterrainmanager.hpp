/****************************************************************************
 *            abstractterrainmanager.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
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

		protected:
			AbstractTerrainManager();

			inline void set_bounding_box(
				const BoundingBox &bounding_box) noexcept
			{
				m_bounding_box = bounding_box;
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
			virtual void clear() = 0;
			static const glm::vec3 &get_vector_scale() noexcept;
			static const glm::vec3 &get_vector_min() noexcept;
			static const glm::vec3 &get_vector_max() noexcept;
			static const glm::vec4 &get_vector_scale_rgb10_a2()
				noexcept;
			static const glm::vec2 &get_vector_offset_rgb10_a2()
				noexcept;
			static const glm::vec4 &get_vector_rgb10_a2() noexcept;

			inline bool get_empty() const noexcept
			{
				return get_bounding_box().get_empty();
			}

			inline const BoundingBox &get_bounding_box() const
				noexcept
			{
				return m_bounding_box;
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

			static inline glm::vec3 get_offset_scaled_rgb10_a2(
				const glm::uvec4 &value)
			{
				glm::vec4 tmp;
				glm::vec3 result;

				tmp = value;
				tmp *= glm::vec4(1.0f / 1023.0f,
					1.0f / 1023.0f, 1.0f / 1023.0f,
					1.0f / 3.0f);

				tmp *= get_vector_scale_rgb10_a2();

				result = glm::vec3(tmp);

				result.x += get_vector_offset_rgb10_a2().x;
				result.y += get_vector_offset_rgb10_a2().y;
				result.z += tmp.w;

				return result;
			}

			static inline glm::uvec4 get_value_scaled_rgb10_a2(
				const glm::vec3 &offset)
			{
				glm::uvec4 result;
				glm::vec3 tmp;

				tmp = offset / get_vector_scale();
				tmp.x = tmp.x * 0.5f + 0.5f;
				tmp.y = tmp.y * 0.5f + 0.5f;

				tmp = glm::clamp(tmp, glm::vec3(0.0f),
					glm::vec3(1.0f));

				tmp *= glm::vec3(1023.0f, 1023.0f, 4095.0f);
				tmp += 0.5f;

				result.x = tmp.x;
				result.y = tmp.y;
				result.z = tmp.z;
				result.w = result.z % 4;
				result.z = result.z / 4;

				return result;
			}

			static inline glm::vec3 get_offset_rgb10_a2(
				const glm::uvec4 &value)
			{
				glm::vec4 tmp;
				glm::vec3 result;

				tmp = value;
				tmp *= glm::vec4(1.0f / 1023.0f,
					1.0f / 1023.0f, 1.0f / 1023.0f,
					1.0f / 3.0f);

				tmp *= get_vector_rgb10_a2();

				result = glm::vec3(tmp);

				result.x -= 1.0f;
				result.y -= 1.0f;
				result.z += tmp.w;

				return result;
			}

			static inline glm::uvec4 get_value_rgb10_a2(
				const glm::vec3 &offset)
			{
				glm::uvec4 result;
				glm::vec3 tmp;

				tmp = offset;

				tmp.x = tmp.x * 0.5f + 0.5f;
				tmp.y = tmp.y * 0.5f + 0.5f;

				tmp = glm::clamp(tmp, glm::vec3(0.0f),
					glm::vec3(1.0f));

				tmp.x *= 1023.0f;
				tmp.y *= 1023.0f;
				tmp.z *= 4095.0f;

				result.x = tmp.x + 0.5f;
				result.y = tmp.y + 0.5f;
				result.z = tmp.z + 0.5f;
				result.w = result.z % 4;
				result.z = result.z / 4;

				return result;
			}

	};

}

#endif	/* UUID_45c1c534_56a4_4295_a35c_b836fe76721d */
