/****************************************************************************
 *            map.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_6a19b8b7_ab72_4de0_a4a6_34c9d01035b7
#define	UUID_6a19b8b7_ab72_4de0_a4a6_34c9d01035b7

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "objectdata.hpp"
#include "exceptions.hpp"

/**
 * @file
 * @brief The @c class Map.
 * This file contains the @c class Map.
 */
namespace eternal_lands
{

	/**
	 * @brief @c class for maps.
	 *
	 * @c class for maps.
	 */
	class Map
	{
		private:
			const MeshBuilderWeakPtr m_mesh_builder;
			const MeshCacheWeakPtr m_mesh_cache;
			const MaterialCacheWeakPtr m_material_cache;
			boost::scoped_ptr<RStarTree> m_object_tree;
			boost::scoped_ptr<RStarTree> m_light_tree;
			AbstractTerrainManagerVector m_terrains;
			Uint32ObjectSharedPtrMap m_objects;
			Uint32LightSharedPtrMap m_lights;
			Uint16MultiArray2 m_height_map;
			Uint16MultiArray2 m_tile_map;
			ParticleDataVector m_particles;
			glm::vec3 m_ambient;
			String m_name;
			Uint32 m_id;
			bool m_dungeon;

			inline MeshBuilderSharedPtr get_mesh_builder() const
			{
				MeshBuilderSharedPtr result;

				result = m_mesh_builder.lock();

				assert(result.get() != 0);

				return result;
			}

			inline MeshCacheSharedPtr get_mesh_cache() const
			{
				MeshCacheSharedPtr result;

				result = m_mesh_cache.lock();

				assert(result.get() != 0);

				return result;
			}

			inline MaterialCacheSharedPtr get_material_cache() const
			{
				MaterialCacheSharedPtr result;

				result = m_material_cache.lock();

				assert(result.get() != 0);

				return result;
			}

		public:
			/**
			 * Default constructor.
			 */
			Map(const CodecManagerSharedPtr &codec_manager,
				const FileSystemSharedPtr &file_system,
				const GlobalVarsSharedPtr &global_vars,
				const MeshBuilderSharedPtr &mesh_builder,
				const MeshCacheSharedPtr &mesh_cache,
				const MaterialCacheSharedPtr &material_cache,
				const String &name);

			/**
			 * Default destructor.
			 */
			~Map() throw();

			void add_object(
				const ObjectDescription &object_description);
			void add_object(const ObjectData &object_data,
				const MaterialSharedPtrVector &materials);
			void add_object(const InstanceData &instance_data);
			void remove_object(const Uint32 id);
			bool get_object_position(const Uint32 id,
				glm::vec3 &position);
			void add_light(const LightData &light_data);
			void remove_light(const Uint32 id);
			void clear();
			void intersect(const Frustum &frustum,
				ObjectVisitor &visitor) const;
			void intersect(const Frustum &frustum,
				LightVisitor &visitor) const;
			const BoundingBox &get_bounding_box() const;
			void add_terrain(
				AbstractTerrainManagerAutoPtr &terrain);
			void add_particle(const ParticleData &particle);

			inline glm::uvec2 get_height_map_size() const
			{
				glm::uvec2 result;

				result.x = m_height_map.shape()[0];
				result.y = m_height_map.shape()[1];

				return result;
			}

			void set_height_map_size(const Uint16 width,
				const Uint16 height)
			{
				m_height_map.resize(
					boost::extents[width][height]);
			}

			inline void set_height(const Uint16 x, const Uint16 y,
				const Uint16 height)
			{
				RANGE_CECK(x, m_height_map.shape()[0],
					UTF8("index value too big"));
				RANGE_CECK(y, m_height_map.shape()[1],
					UTF8("index value too big"));

				m_height_map[x][y] = height;
			}

			inline Uint16 get_height(const Uint16 x, const Uint16 y)
				const
			{
				RANGE_CECK(x, m_height_map.shape()[0],
					UTF8("index value too big"));
				RANGE_CECK(y, m_height_map.shape()[1],
					UTF8("index value too big"));

				return m_height_map[x][y];
			}

			inline glm::uvec2 get_tile_map_size() const
			{
				glm::uvec2 result;

				result.x = m_tile_map.shape()[0];
				result.y = m_tile_map.shape()[1];

				return result;
			}

			void set_tile_map_size(const Uint16 width,
				const Uint16 height)
			{
				m_tile_map.resize(
					boost::extents[width][height]);
			}

			inline void set_tile(const Uint16 x, const Uint16 y,
				const Uint16 tile)
			{
				RANGE_CECK(x, m_tile_map.shape()[0],
					UTF8("index value too big"));
				RANGE_CECK(y, m_tile_map.shape()[1],
					UTF8("index value too big"));

				m_tile_map[x][y] = tile;
			}

			inline Uint16 get_tile(const Uint16 x, const Uint16 y)
				const
			{
				RANGE_CECK(x, m_tile_map.shape()[0],
					UTF8("index value too big"));
				RANGE_CECK(y, m_tile_map.shape()[1],
					UTF8("index value too big"));

				return m_tile_map[x][y];
			}

			inline void set_ambient(const glm::vec3 &ambient)
			{
				m_ambient = ambient;
			}

			inline const glm::vec3 &get_ambient() const
			{
				return m_ambient;
			}

			inline void set_dungeon(const bool dungeon)
			{
				m_dungeon = dungeon;
			}

			inline bool get_dungeon() const
			{
				return m_dungeon;
			}

			inline const String &get_name() const
			{
				return m_name;
			}

			inline Uint32 get_id() const
			{
				return m_id;
			}

			inline const ParticleDataVector &get_particles() const
			{
				return m_particles;
			}

	};

}

#endif	/* UUID_6a19b8b7_ab72_4de0_a4a6_34c9d01035b7 */
