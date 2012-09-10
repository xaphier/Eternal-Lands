/****************************************************************************
 *            map.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
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
			const GlobalVarsSharedPtr m_global_vars;
			const EffectCacheWeakPtr m_effect_cache;
			const MeshBuilderWeakPtr m_mesh_builder;
			const MeshCacheWeakPtr m_mesh_cache;
			const MaterialCacheWeakPtr m_material_cache;
			const MaterialBuilderWeakPtr m_material_builder;
			const TextureCacheWeakPtr m_texture_cache;
			boost::scoped_ptr<RStarTree> m_object_tree;
			boost::scoped_ptr<RStarTree> m_light_tree;
			AbstractTerrainManagerSharedPtr m_terrain_manager;
			Uint32ObjectSharedPtrMap m_objects;
			Uint32LightSharedPtrMap m_lights;
			Uint16MultiArray2 m_height_map;
			Uint16MultiArray2 m_tile_map;
			FloatMultiArray2 m_walk_height_map;
			ParticleDataVector m_particles;
			MaterialSharedPtr m_clipmap_material;
			glm::vec3 m_ambient;
			String m_name;
			Uint32 m_id;
			bool m_dungeon;
			const CodecManagerSharedPtr m_codec_manager;
			const FileSystemSharedPtr m_file_system;

			inline GlobalVarsSharedPtr get_global_vars() const
				noexcept
			{
				return m_global_vars;
			}

			inline EffectCacheSharedPtr get_effect_cache() const
				noexcept
			{
				EffectCacheSharedPtr result;

				result = m_effect_cache.lock();

				assert(result.get() != nullptr);

				return result;
			}

			inline MeshBuilderSharedPtr get_mesh_builder() const
				noexcept
			{
				MeshBuilderSharedPtr result;

				result = m_mesh_builder.lock();

				assert(result.get() != nullptr);

				return result;
			}

			inline MeshCacheSharedPtr get_mesh_cache() const
				noexcept
			{
				MeshCacheSharedPtr result;

				result = m_mesh_cache.lock();

				assert(result.get() != nullptr);

				return result;
			}

			inline MaterialCacheSharedPtr get_material_cache() const
				noexcept
			{
				MaterialCacheSharedPtr result;

				result = m_material_cache.lock();

				assert(result.get() != nullptr);

				return result;
			}

			inline MaterialBuilderSharedPtr get_material_builder()
				const noexcept
			{
				MaterialBuilderSharedPtr result;

				result = m_material_builder.lock();

				assert(result.get() != nullptr);

				return result;
			}

			inline TextureCacheSharedPtr get_texture_cache()
				const noexcept
			{
				TextureCacheSharedPtr result;

				result = m_texture_cache.lock();

				assert(result.get() != nullptr);

				return result;
			}

			void init_walk_height_map(
				const ImageSharedPtr &displacement_map);
			void init_terrain(
				const GlobalVarsSharedPtr &global_vars,
				const MeshBuilderSharedPtr &mesh_builder,
				const MeshCacheSharedPtr &mesh_cache,
				const MaterialCacheSharedPtr &material_cache);
			void load_data(const String& name);

		public:
			/**
			 * Default constructor.
			 */
			Map(const CodecManagerSharedPtr &codec_manager,
				const FileSystemSharedPtr &file_system,
				const GlobalVarsSharedPtr &global_vars,
				const EffectCacheWeakPtr &effect_cache,
				const MeshBuilderSharedPtr &mesh_builder,
				const MeshCacheSharedPtr &mesh_cache,
				const MaterialCacheSharedPtr &material_cache,
				const MaterialBuilderWeakPtr &material_builder,
				const TextureCacheWeakPtr &texture_cache);

			/**
			 * Default destructor.
			 */
			~Map() noexcept;

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
			void intersect_terrain(const Frustum &frustum,
				const glm::vec3 &camera,
				BoundingBox &bounding_box) const;
			void intersect_terrain(const Frustum &frustum,
				const glm::vec3 &camera,
				TerrainVisitor &terrain) const;
			void intersect(const Frustum &frustum,
				ObjectVisitor &visitor) const;
			void intersect(const Frustum &frustum,
				LightVisitor &visitor) const;
			BoundingBox get_bounding_box() const;
			void add_particle(const ParticleData &particle);
			glm::vec4 get_terrain_size_data() const;
			glm::vec2 get_terrain_size() const;
			void set_clipmap_texture(
				const TextureSharedPtr &texture);
			void set_terrain(const ImageSharedPtr &displacement_map,
				const ImageSharedPtr &normal_map,
				const ImageSharedPtr &dudv_map);
			void set_blend(const ImageSharedPtrVector &blend_maps);
			bool get_terrain() const;
			void build_clipmap_material(
				const ClipmapData &clipmap_data);

			inline const Uint32ObjectSharedPtrMap &get_objects()
				const noexcept
			{
				return m_objects;
			}

			inline const Uint32LightSharedPtrMap &get_lights()
				const noexcept
			{
				return m_lights;
			}

			inline const MaterialSharedPtr &get_clipmap_material()
				const noexcept
			{
				return m_clipmap_material;
			}

			inline glm::uvec2 get_height_map_size() const noexcept
			{
				glm::uvec2 result;

				result.x = m_height_map.shape()[0];
				result.y = m_height_map.shape()[1];

				return result;
			}

			inline glm::uvec2 get_walk_height_map_size() const
				noexcept
			{
				glm::uvec2 result;

				result.x = m_walk_height_map.shape()[0];
				result.y = m_walk_height_map.shape()[1];

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
				RANGE_CECK_MAX(x, m_height_map.shape()[0],
					UTF8("index value too big"));
				RANGE_CECK_MAX(y, m_height_map.shape()[1],
					UTF8("index value too big"));

				m_height_map[x][y] = height;
			}

			inline Uint16 get_height(const Uint16 x, const Uint16 y)
				const
			{
				RANGE_CECK_MAX(x, m_height_map.shape()[0],
					UTF8("index value too big"));
				RANGE_CECK_MAX(y, m_height_map.shape()[1],
					UTF8("index value too big"));

				return m_height_map[x][y];
			}

			inline float get_walk_height(const Uint16 x,
				const Uint16 y) const
			{
				RANGE_CECK_MAX(x, m_walk_height_map.shape()[0],
					UTF8("index value too big"));
				RANGE_CECK_MAX(y, m_walk_height_map.shape()[1],
					UTF8("index value too big"));

				return m_walk_height_map[x][y];
			}

			inline glm::uvec2 get_tile_map_size() const noexcept
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
				RANGE_CECK_MAX(x, m_tile_map.shape()[0],
					UTF8("index value too big"));
				RANGE_CECK_MAX(y, m_tile_map.shape()[1],
					UTF8("index value too big"));

				m_tile_map[x][y] = tile;
			}

			inline Uint16 get_tile(const Uint16 x, const Uint16 y)
				const
			{
				RANGE_CECK_MAX(x, m_tile_map.shape()[0],
					UTF8("index value too big"));
				RANGE_CECK_MAX(y, m_tile_map.shape()[1],
					UTF8("index value too big"));

				return m_tile_map[x][y];
			}

			inline void set_ambient(const glm::vec3 &ambient)
				noexcept
			{
				m_ambient = ambient;
			}

			inline const glm::vec3 &get_ambient() const noexcept
			{
				return m_ambient;
			}

			inline void set_dungeon(const bool dungeon) noexcept
			{
				m_dungeon = dungeon;
			}

			inline bool get_dungeon() const noexcept
			{
				return m_dungeon;
			}

			inline void set_name(const String &name) noexcept
			{
				m_name = name;
				load_data(name);
			}

			inline const String &get_name() const noexcept
			{
				return m_name;
			}

			inline Uint32 get_id() const noexcept
			{
				return m_id;
			}

			inline const ParticleDataVector &get_particles() const
				noexcept
			{
				return m_particles;
			}

	};

}

#endif	/* UUID_6a19b8b7_ab72_4de0_a4a6_34c9d01035b7 */
