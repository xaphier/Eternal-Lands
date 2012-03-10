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

		protected:
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
			AbstractTerrainManager();
			virtual ~AbstractTerrainManager() throw();
			virtual void intersect(const Frustum &frustum,
				ObjectVisitor &visitor) const = 0;

			inline const StringArray4 &get_albedo_maps() const
			{
				return m_data.get_albedo_maps();
			}

			inline const String &get_albedo_map(const Uint16 index)
				const
			{
				return m_data.get_albedo_map(index);
			}

			inline const String &get_blend_map() const
			{
				return m_data.get_blend_map();
			}

			inline const String &get_height_map() const
			{
				return m_data.get_height_map();
			}

			inline const String &get_dudv_map() const
			{
				return m_data.get_dudv_map();
			}

			inline const glm::vec3 &get_translation() const
			{
				return m_data.get_translation();
			}

			inline const TerrainData &get_data() const
			{
				return m_data;
			}

			static inline float get_height_scale()
			{
				return 0.1f / 257.0f;
			}

			static inline Uint16 get_tile_size()
			{
				return 16;
			}

	};

}

#endif	/* UUID_45c1c534_56a4_4295_a35c_b836fe76721d */
