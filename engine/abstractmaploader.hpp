/****************************************************************************
 *            abstractmaploader.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
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
		mit_materials,
		mit_height_map,
		mit_tile_map,
		mit_walk_map,
		mit_ambient,
		mit_options
	};

	typedef std::set<MapItemsType> MapItemsTypeSet;

	/**
	 * @brief @c class for loading maps.
	 *
	 * @c class for loading maps.
	 */
	class AbstractMapLoader
	{
		private:
			const FileSystemSharedPtr m_file_system;
			const FreeIdsManagerSharedPtr m_free_ids;
			ReaderSharedPtr m_reader;
			StringVector m_material_names;
			StringSet m_harvestables, m_entrables;

		protected:
			inline const FileSystemSharedPtr &get_file_system()
				const
			{
				return m_file_system;
			}

			inline const ReaderSharedPtr &get_reader() const
			{
				return m_reader;
			}

			inline FreeIdsManager &get_free_ids()
			{
				return *m_free_ids;
			}

			void read_3d_object(const Uint32 index,
				const Uint32 offset);
			void read_2d_object(const Uint32 index,
				const Uint32 offset);
			void read_light(const Uint32 index,
				const Uint32 offset);
			void read_particle(const Uint32 index,
				const Uint32 offset);
			void read_terrain(const Uint32 index,
				const Uint32 offset);
			void read_material_name(const Uint32 index,
				const Uint32 offset);

			void read_3d_objects(const Uint32 obj_3d_size,
				const Uint32 obj_3d_count,
				const Uint32 obj_3d_offset);
			void read_2d_objects(const Uint32 obj_2d_size,
				const Uint32 obj_2d_count,
				const Uint32 obj_2d_offset);
			void read_lights(const Uint32 light_size,
				const Uint32 light_count,
				const Uint32 light_offset);
			void read_particles(const Uint32 particle_size,
				const Uint32 particle_count,
				const Uint32 particle_offset);
			void read_terrains(const Uint32 terrain_size,
				const Uint32 terrain_count,
				const Uint32 terrain_offset);
			void read_material_names(
				const Uint32 material_name_size,
				const Uint32 material_name_count,
				const Uint32 material_name_offset);
			void read_height_map(const Uint32 height_map_width,
				const Uint32 height_map_height,
				const Uint32 height_map_offset);
			void read_tile_map(const Uint32 tile_map_widht,
				const Uint32 tile_map_height,
				const Uint32 tile_map_offset);
			void read(const String &name);
			static ObjectDescription get_object_description(
				const glm::vec3 &translation,
				const glm::vec3 &rotation_angles,
				const StringVector &material_names,
				const String &name, const float scale,
				const Uint32 id, const SelectionType selection,
				const BlendType blend);
			SelectionType get_selection(const String &name) const
				noexcept;
			StringVector get_material_names(const Uint32 index,
				const Uint32 count) const;

			virtual void add_object(const glm::vec3 &position,
				const glm::vec3 &rotation_angles,
				const String &name, const float scale,
				const float transparency, const Uint32 id,
				const SelectionType selection,
				const BlendType blend,
				const StringVector &material_names) = 0;
			virtual void add_light(const glm::vec3 &position,
				const glm::vec3 &color, const float radius,
				const Uint32 id) = 0;
			virtual void add_particle(const glm::vec3 &position,
				const String &name, const Uint32 id) = 0;
			virtual void set_tile(const Uint16 x, const Uint16 y,
				const Uint16 tile) = 0;
			virtual void set_height(const Uint16 x, const Uint16 y,
				const Uint16 height) = 0;
			virtual void set_ambient(const glm::vec3 &ambient) = 0;
			virtual void set_height_map_size(const Uint16 width,
				const Uint16 height) = 0;
			virtual void set_tile_map_size(const Uint16 width,
				const Uint16 height) = 0;
			virtual void set_dungeon(const bool dungeon) = 0;
			virtual void instance() = 0;

		public:
			/**
			 * Default constructor.
			 */
			AbstractMapLoader(
				const FileSystemSharedPtr &file_system,
				const FreeIdsManagerSharedPtr &free_ids);

			/**
			 * Default destructor.
			 */
			virtual ~AbstractMapLoader() noexcept;

			static bool check_format(const Uint8Array8 &id)
				noexcept;
			static StringSet load_harvestables(
				const FileSystemSharedPtr &file_system);
			static StringSet load_entrables(
				const FileSystemSharedPtr &file_system);

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

			static inline Uint32 get_tile_size() noexcept
			{
				return 3;
			}

	};

}

#endif	/* UUID_4f58abca_9115_481b_bb8d_b6bf79f6ddf8 */
