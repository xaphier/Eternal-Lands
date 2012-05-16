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
			TerrainData m_data;
			glm::vec4 m_terrain_size_data;

		protected:
			AbstractTerrainManager();
			inline void set_terrain_size(
				const glm::uvec2 &terrain_size) noexcept
			{
				m_terrain_size_data.x = terrain_size.x;
				m_terrain_size_data.y = terrain_size.y;
				m_terrain_size_data.z = 1.0f / terrain_size.x;
				m_terrain_size_data.w = 1.0f / terrain_size.y;
			}

			inline void set_data(const TerrainData &data)
			{
				m_data = data;
			}

			inline void set_albedo_maps(
				const StringArray4 &albedo_maps)
			{
				m_data.set_albedo_maps(albedo_maps);
			}

			inline void set_albedo_map(const String &albedo_map,
				const Uint16 index)
			{
				m_data.set_albedo_map(albedo_map, index);
			}

			inline void set_blend_map(const String &blend_map)
			{
				m_data.set_blend_map(blend_map);
			}

			inline void set_height_map(const String &height_map)
			{
				m_data.set_height_map(height_map);
			}

			inline void set_dudv_map(const String &dudv_map)
			{
				m_data.set_dudv_map(dudv_map);
			}

			inline void set_translation(
				const glm::vec3 &translation)
			{
				m_data.set_translation(translation);
			}

		public:
			virtual ~AbstractTerrainManager() noexcept;
			virtual void intersect(const Frustum &frustum,
				ObjectVisitor &visitor) const = 0;
			static const glm::vec3 &get_vector_scale() noexcept;
			static const glm::vec2 &get_position_scale() noexcept;

			inline const StringArray4 &get_albedo_maps() const
				noexcept
			{
				return m_data.get_albedo_maps();
			}

			inline const String &get_albedo_map(const Uint16 index)
				const noexcept
			{
				return m_data.get_albedo_map(index);
			}

			inline const String &get_blend_map() const noexcept
			{
				return m_data.get_blend_map();
			}

			inline const String &get_height_map() const noexcept
			{
				return m_data.get_height_map();
			}

			inline const String &get_dudv_map() const noexcept
			{
				return m_data.get_dudv_map();
			}

			inline const glm::vec3 &get_translation() const
				noexcept
			{
				return m_data.get_translation();
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

			inline const TerrainData &get_data() const noexcept
			{
				return m_data;
			}

			static inline Uint16 get_tile_size() noexcept
			{
				return 32;
			}

	};

}

#endif	/* UUID_45c1c534_56a4_4295_a35c_b836fe76721d */
