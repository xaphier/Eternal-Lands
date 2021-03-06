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
			const GlobalVarsConstSharedPtr m_global_vars;
			const MeshBuilderConstWeakPtr m_mesh_builder;
			const MeshCacheWeakPtr m_mesh_cache;
			const MaterialCacheWeakPtr m_material_cache;
			const TerrainBuilderConstWeakPtr m_terrain_builder;
			const TextureCacheWeakPtr m_texture_cache;
			boost::scoped_ptr<RStarTree> m_object_tree;
			boost::scoped_ptr<RStarTree> m_light_tree;
			ShaderSourceTerrainSharedPtr m_terrain_effect;
			AbstractTerrainSharedPtr m_terrain;
			Uint32ObjectSharedPtrMap m_objects;
			Uint32LightSharedPtrMap m_lights;
			Uint16MultiArray2 m_walk_height_map;
			ParticleDataVector m_particles;
			glm::vec4 m_ground_hemisphere;
			glm::uvec2 m_size;
			String m_name;
			Uint32 m_id;
			bool m_dungeon;

			inline GlobalVarsConstSharedPtr get_global_vars() const
				noexcept
			{
				return m_global_vars;
			}

			inline MeshBuilderConstSharedPtr get_mesh_builder() const
				noexcept
			{
				MeshBuilderConstSharedPtr result;

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

			inline TerrainBuilderConstSharedPtr get_terrain_builder()
				const noexcept
			{
				TerrainBuilderConstSharedPtr result;

				result = m_terrain_builder.lock();

				assert(result.get() != nullptr);

				return result;
			}

			inline TextureCacheSharedPtr get_texture_cache() const
				noexcept
			{
				TextureCacheSharedPtr result;

				result = m_texture_cache.lock();

				assert(result.get() != nullptr);

				return result;
			}

		public:
			/**
			 * Default constructor.
			 */
			Map(const GlobalVarsConstSharedPtr &global_vars,
				const MeshBuilderConstSharedPtr &mesh_builder,
				const MeshCacheSharedPtr &mesh_cache,
				const MaterialCacheSharedPtr &material_cache,
				const TerrainBuilderConstWeakPtr &terrain_builder,
				const TextureCacheWeakPtr &texture_cache);

			/**
			 * Default destructor.
			 */
			~Map() noexcept;

			void add_object(
				const ObjectDescription &object_description);
			void add_object(const ObjectData &object_data,
				const MaterialSharedPtrVector &materials);
			void add_object(const ObjectData &object_data,
				const AbstractMeshSharedPtr &mesh,
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
			void init_terrain_rendering_data(
				TerrainRenderingData &rendering_data) const;
			BoundingBox get_bounding_box() const;
			void add_particle(const ParticleData &particle);
			glm::vec4 get_terrain_size_data() const;
			glm::vec3 get_terrain_translation() const;
			glm::vec2 get_terrain_size() const;
			void set_clipmap_terrain_texture(
				const TextureSharedPtr &texture);
			void set_clipmap_terrain_specular_gloss_texture(
				const TextureSharedPtr &texture);
			void set_clipmap_terrain_normal_texture(
				const TextureSharedPtr &texture);
			void set_terrain_geometry_maps(
				const ImageConstSharedPtr &displacement_map,
				const ImageConstSharedPtr &normal_map,
				const ImageConstSharedPtr &dudv_map,
				const glm::vec3 &translation);
			void set_terrain_blend_map(
				const ImageConstSharedPtr &blend_map);
			void update_terrain_geometry_maps(
				const ImageConstSharedPtr &displacement_map,
				const ImageConstSharedPtr &normal_tangent_map,
				const ImageConstSharedPtr &dudv_map,
				const glm::vec3 &translation);
			void update_terrain_blend_map(
				const ImageConstSharedPtr &blend_map,
				const BitSet64 &layers);
			void set_terrain_material(
				const StringVector &albedo_maps,
				const StringVector &specular_maps,
				const StringVector &gloss_maps,
				const StringVector &height_maps,
				const TerrainMaterialData &material_data);
			bool get_terrain() const;
			const MaterialSharedPtr &get_clipmap_terrain_material(
				const bool write_height,
				const bool write_specular_gloss) const;
			const MaterialSharedPtr &get_terrain_material() const;
			void set_terrain_dudv_scale_offset(
				const glm::vec4 &dudv_scale_offset);
			const glm::vec4 &get_terrain_dudv_scale_offset() const;

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

			inline glm::uvec2 get_walk_height_map_size() const
				noexcept
			{
				glm::uvec2 result;

				result.x = m_walk_height_map.shape()[0];
				result.y = m_walk_height_map.shape()[1];

				return result;
			}

			inline void set_walk_height_map_size(
				const glm::uvec2 &size)
			{
				m_walk_height_map.resize(
					boost::extents[size.x][size.y]);
			}

			inline void set_walk_height_map(
				const Uint8MultiArray2 &walk_height_map)
			{
				m_walk_height_map.resize(boost::extents[walk_height_map.shape()[0]][walk_height_map.shape()[1]]);
				m_walk_height_map = walk_height_map;
			}

			inline void set_walk_height(const Uint16 x,
				const Uint16 y, const Uint16 walk_height)
			{
				RANGE_CECK_MAX(x,
					m_walk_height_map.shape()[0] - 1,
					UTF8("index value too big"));
				RANGE_CECK_MAX(y,
					m_walk_height_map.shape()[1] - 1,
					UTF8("index value too big"));

				m_walk_height_map[x][y] = walk_height;
			}

			inline Uint16 get_walk_height(const Uint16 x,
				const Uint16 y) const
			{
				RANGE_CECK_MAX(x,
					m_walk_height_map.shape()[0] - 1,
					UTF8("index value too big"));
				RANGE_CECK_MAX(y,
					m_walk_height_map.shape()[1] - 1,
					UTF8("index value too big"));

				return m_walk_height_map[x][y];
			}

			inline void set_ground_hemisphere(
				const glm::vec4 &ground_hemisphere) noexcept
			{
				m_ground_hemisphere = ground_hemisphere;
			}

			inline const glm::vec4 &get_ground_hemisphere() const
				noexcept
			{
				return m_ground_hemisphere;
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
			}

			inline const String &get_name() const noexcept
			{
				return m_name;
			}

			inline void set_size(const glm::uvec2 &size) noexcept
			{
				m_size = size;
			}

			inline const glm::uvec2 &get_size() const noexcept
			{
				return m_size;
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
