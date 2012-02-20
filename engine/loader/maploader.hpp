/****************************************************************************
 *            maploader.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_c299b4bb_40d2_4781_a02d_4fd650012aca
#define	UUID_c299b4bb_40d2_4781_a02d_4fd650012aca

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "selectionutil.hpp"

/**
 * @file
 * @brief The @c class MapLoader.
 * This file contains the @c class MapLoader.
 */
namespace eternal_lands
{

	/**
	 * @brief @c class for loading maps.
	 *
	 * @c class for loading maps.
	 */
	class MapLoader
	{
		private:
			const CodecManagerSharedPtr m_codec_manager;
			const FileSystemSharedPtr m_file_system;
			const GlobalVarsSharedPtr m_global_vars;
			const MeshBuilderSharedPtr m_mesh_builder;
			const MeshCacheSharedPtr m_mesh_cache;
			const MeshDataCacheSharedPtr m_mesh_data_cache;
			const EffectCacheSharedPtr m_effect_cache;
			const TextureCacheSharedPtr m_texture_cache;
			const MaterialDescriptionCacheSharedPtr
				m_material_description_cache;
			const FreeIdsManagerSharedPtr m_free_ids;
			ReaderSharedPtr m_reader;
			MapSharedPtr m_map;
			boost::scoped_ptr<InstancesBuilder> m_instances_builder;
			StringVector m_material_names;
			StringSet m_harvestables, m_entrables;

		protected:
			inline const CodecManagerSharedPtr &get_codec_manager()
				const
			{
				return m_codec_manager;
			}

			inline const FileSystemSharedPtr &get_file_system()
				const
			{
				return m_file_system;
			}

			inline const GlobalVarsSharedPtr &get_global_vars()
				const
			{
				return m_global_vars;
			}

			inline const MeshBuilderSharedPtr &get_mesh_builder()
				const
			{
				return m_mesh_builder;
			}

			inline const MeshCacheSharedPtr &get_mesh_cache() const
			{
				return m_mesh_cache;
			}

			inline const MeshDataCacheSharedPtr
				&get_mesh_data_cache() const
			{
				return m_mesh_data_cache;
			}

			inline const EffectCacheSharedPtr &get_effect_cache()
				const
			{
				return m_effect_cache;
			}

			inline const TextureCacheSharedPtr &get_texture_cache()
				const
			{
				return m_texture_cache;
			}

			inline const MaterialDescriptionCacheSharedPtr
				&get_material_description_cache() const
			{
				return m_material_description_cache;
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
			void read();
			ObjectData get_object_data(const glm::vec3 &pos,
				const glm::vec3 &rotation_angles,
				const String &name, const float scale,
				const bool transparent, const Uint32 id,
				const SelectionType selection) const;
			SelectionType get_selection(const String &name) const;
			StringVector get_material_names(const Uint32 index,
				const Uint32 count) const;
			SelectionType get_selection_type(const String &name)
				const;

			virtual void add_object(const glm::vec3 &position,
				const glm::vec3 &rotation_angles,
				const String &name, const float scale,
				const float transparency, const Uint32 id,
				const bool blended,
				const SelectionType selection,
				const StringVector &material_names);
			virtual void add_light(const glm::vec3 &position,
				const glm::vec3 &color, const float radius,
				const Uint32 id);
			virtual void add_particle(const glm::vec3 &position,
				const String &name, const Uint32 id);
			virtual void add_terrain(const StringArray4 &albedo,
				const String &blend, const String &height,
				const String &dudv,
				const glm::vec3 &translation,
				const glm::vec3 &rotation_angles,
				const float scale);
			virtual void set_tile(const Uint16 x, const Uint16 y,
				const Uint16 tile);
			virtual void set_height(const Uint16 x, const Uint16 y,
				const Uint16 height);
			virtual void set_ambient(const glm::vec3 &ambient);
			virtual void set_height_map_size(const Uint16 width,
				const Uint16 height);
			virtual void set_tile_map_size(const Uint16 width,
				const Uint16 height);
			void instance();

		public:
			/**
			 * Default constructor.
			 */
			MapLoader(const CodecManagerSharedPtr &codec_manager,
				const FileSystemSharedPtr &file_system,
				const GlobalVarsSharedPtr &global_vars,
				const MeshBuilderSharedPtr &mesh_builder,
				const MeshCacheSharedPtr &mesh_cache,
				const MeshDataCacheSharedPtr &mesh_data_cache,
				const EffectCacheSharedPtr &effect_cache,
				const TextureCacheSharedPtr &texture_cache,
				const MaterialDescriptionCacheSharedPtr
					&material_description_cache,
				const FreeIdsManagerSharedPtr &free_ids);

			/**
			 * Default destructor.
			 */
			virtual ~MapLoader() throw();

			MapSharedPtr load(const String &name);

			static bool check_format(const Uint8Array8 &id);
			static StringSet load_harvestables(
				const FileSystemSharedPtr &file_system);
			static StringSet load_entrables(
				const FileSystemSharedPtr &file_system);

			static glm::quat get_rotation(
				const glm::vec3 &rotation_angles);
			static Transformation get_transformation(
				const glm::vec3 &translation,
				const glm::vec3 &rotation_angles,
				const float scale);

			static inline Sint8Array4 get_magic_number()
			{
				Sint8Array4 result;

				result[0] = 'e';
				result[1] = 'l';
				result[2] = 'm';
				result[3] = 'f';

				return result;
			}

			static inline Uint32 get_3d_object_size()
			{
				return 144;
			}

			static inline Uint32 get_2d_object_size()
			{
				return 128;
			}

			static inline Uint32 get_light_size()
			{
				return 40;
			}

			static inline Uint32 get_particle_size()
			{
				return 104;
			}

			static inline Uint32 get_terrain_size()
			{
				return 1024;
			}

			static inline Uint32 get_material_name_size()
			{
				return 128;
			}

			static inline Uint32 get_tile_size()
			{
				return 3;
			}

	};

}

#endif	/* UUID_c299b4bb_40d2_4781_a02d_4fd650012aca */
