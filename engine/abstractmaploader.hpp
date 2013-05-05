/****************************************************************************
 *            abstractmaploader.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_4f58abca_9115_481b_bb8d_b6bf79f6ddf8
#define	UUID_4f58abca_9115_481b_bb8d_b6bf79f6ddf8

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "selectionutil.hpp"
#include "blendutil.hpp"

/**
 * @file
 * @brief The @c class AbstractMapLoader.
 * This file contains the @c class AbstractMapLoader.
 */
namespace eternal_lands
{

	enum MapItemsType
	{
		mit_2d_objects = 0,
		mit_3d_objects,
		mit_lights,
		mit_particles,
		mit_decals,
		mit_materials,
		mit_walk_height_map,
		mit_tile_map,
		mit_walk_map,
		mit_options,
		mit_terrain
	};

	typedef std::set<MapItemsType> MapItemsTypeSet;

	enum MapVersionType
	{
		mvt_1_0,
		mvt_1_1
	};

	/**
	 * @brief @c class for loading maps.
	 *
	 * @c class for loading maps.
	 */
	class AbstractMapLoader
	{
		private:
			const FileSystemConstSharedPtr m_file_system;
			const FreeIdsManagerSharedPtr m_free_ids;
			const GlobalVarsConstSharedPtr m_global_vars;
			ReaderSharedPtr m_reader;
			StringVector m_names;
			StringSet m_harvestables, m_entrables;

		protected:
			inline const FileSystemConstSharedPtr &get_file_system()
				const noexcept
			{
				return m_file_system;
			}

			inline FreeIdsManager &get_free_ids()
			{
				return *m_free_ids;
			}

			inline GlobalVarsConstSharedPtr get_global_vars() const
				noexcept
			{
				return m_global_vars;
			}

			inline const ReaderSharedPtr &get_reader() const
				noexcept
			{
				return m_reader;
			}

			void read_3d_object(const Uint32 index,
				const Uint32 offset,
				const MapVersionType version);
			void read_2d_object(const Uint32 index,
				const Uint32 offset,
				const MapVersionType version);
			void read_light(const Uint32 index,
				const Uint32 offset,
				const MapVersionType version);
			void read_particle(const Uint32 index,
				const Uint32 offset,
				const MapVersionType version);
			void read_decal(const Uint32 index,
				const Uint32 offset,
				const MapVersionType version);
			void read_name(const Uint32 index,
				const MapVersionType version);
			void read_tile_map(const Uint32 widht,
				const Uint32 height, const Uint32 index,
				const Uint32 offset, const float z_position,
				const MapVersionType version);

			void read_3d_objects(const Uint32 obj_3d_size,
				const Uint32 obj_3d_count,
				const Uint32 obj_3d_offset,
				const MapVersionType version);
			void read_2d_objects(const Uint32 obj_2d_size,
				const Uint32 obj_2d_count,
				const Uint32 obj_2d_offset,
				const MapVersionType version);
			void read_lights(const Uint32 light_size,
				const Uint32 light_count,
				const Uint32 light_offset,
				const MapVersionType version);
			void read_particles(const Uint32 particle_size,
				const Uint32 particle_count,
				const Uint32 particle_offset,
				const MapVersionType version);
			void read_decals(const Uint32 decal_size,
				const Uint32 decal_count,
				const Uint32 decal_offset,
				const MapVersionType version);
			void read_terrain(const Uint32 terrain_offset,
				const MapVersionType version);
			void read_names(const Uint32 name_count,
				const Uint32 name_offset,
				const MapVersionType version);
			void read_walk_height_map(
				const Uint32 walk_height_map_width,
				const Uint32 walk_height_map_height,
				const Uint32 walk_height_map_offset,
				const MapVersionType version);
			void read_old_tile_map(const Uint32 tile_map_width,
				const Uint32 tile_map_height,
				const Uint32 tile_map_offset);
			void read_tile_maps(const Uint32 tile_map_width,
				const Uint32 tile_map_height,
				const Uint32 tile_map_offset,
				const MapVersionType version);
			void read(const String &name,
				MapItemsTypeSet skip_items = MapItemsTypeSet());
			static ObjectDescription get_object_description(
				const glm::vec3 &translation,
				const glm::vec3 &rotation_angles,
				const glm::vec3 &scale, 
				const StringVector &material_names,
				const String &name, const BitSet64 blend_mask,
				const float transparency, const float glow,
				const Uint32 id, const SelectionType selection,
				const BlendType blend);
			SelectionType get_selection(const String &name) const
				noexcept;
			StringVector get_names(const Uint32 index,
				const Uint32 count) const;

			virtual void add_object(const glm::vec3 &position,
				const glm::vec3 &rotation_angles,
				const glm::vec3 &scale, const String &name,
				const BitSet64 blend_mask,
				const float transparency, const float glow,
				const Uint32 id, const SelectionType selection,
				const BlendType blend,
				const StringVector &material_names) = 0;
			virtual void add_light(const glm::vec3 &position,
				const glm::vec3 &color, const float radius,
				const Uint32 id) = 0;
			virtual void add_particle(const glm::vec3 &position,
				const String &name, const Uint32 id) = 0;
			virtual void add_decal(const glm::vec2 &position,
				const glm::vec2 &scale, const float rotation,
				const String &texture, const Uint32 id) = 0;
			virtual void set_ground_hemisphere(
				const glm::vec4 &ground_hemisphere) = 0;
			virtual void set_tile_layer_heights(
				const glm::vec4 &heights) = 0;
			virtual void set_map_size(const glm::uvec2 &size) = 0;
			virtual void set_tile_layer(
				const Uint8MultiArray2 &tile_map,
				const float z_position, const Uint16 layer) = 0;
			virtual void set_walk_height_map(
				const Uint8MultiArray2 &walk_height_map) = 0;
			virtual void set_dungeon(const bool dungeon) = 0;
			virtual void instance() = 0;
			virtual void set_terrain(
				const ImageConstSharedPtr &displacement_map,
				const ImageConstSharedPtr &normal_tangent_map,
				const ImageConstSharedPtr &dudv_map,
				const ImageConstSharedPtr &blend_map,
				const StringVector &albedo_maps,
				const StringVector &specular_maps,
				const StringVector &gloss_maps,
				const StringVector &height_maps,
				const TerrainMaterialData &material_data,
				const glm::vec4 &dudv_scale_offset,
				const glm::vec3 &translation,
				const glm::uvec2 &sizes) = 0;

		public:
			/**
			 * Default constructor.
			 */
			AbstractMapLoader(
				const FileSystemConstSharedPtr &file_system,
				const FreeIdsManagerSharedPtr &free_ids,
				const GlobalVarsConstSharedPtr &global_vars);

			/**
			 * Default destructor.
			 */
			virtual ~AbstractMapLoader() noexcept;

			static bool check_format(const Uint8Array8 &id)
				noexcept;
			static StringSet load_harvestables(
				const FileSystemConstSharedPtr &file_system);
			static StringSet load_entrables(
				const FileSystemConstSharedPtr &file_system);

			static inline Sint8Array4 get_magic_number() noexcept
			{
				Sint8Array4 result;

				result[0] = 'e';
				result[1] = 'l';
				result[2] = 'm';
				result[3] = 'f';

				return result;
			}

			static inline Uint32 get_3d_object_size() noexcept
			{
				return 144;
			}

			static inline Uint32 get_2d_object_size() noexcept
			{
				return 128;
			}

			static inline Uint32 get_light_size() noexcept
			{
				return 40;
			}

			static inline Uint32 get_particle_size() noexcept
			{
				return 104;
			}

			static inline Uint32 get_terrain_size() noexcept
			{
				return 1024;
			}

			static inline Uint32 get_material_name_size() noexcept
			{
				return 128;
			}

			static inline Uint32 get_old_tile_size() noexcept
			{
				return 3;
			}

	};

}

#endif	/* UUID_4f58abca_9115_481b_bb8d_b6bf79f6ddf8 */
