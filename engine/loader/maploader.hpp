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
			const ReaderSharedPtr m_reader;
			MapSharedPtr m_map;

		protected:
			inline const ReaderSharedPtr &get_reader() const
			{
				return m_reader;
			}

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
			void read_terrain(const Uint32 terrain_offset);
			void read_height_map(const Uint32 height_map_width,
				const Uint32 height_map_height,
				const Uint32 height_map_offset);
			void read_tile_map(const Uint32 tile_map_widht,
				const Uint32 tile_map_height,
				const Uint32 tile_map_offset);
			void read_material_names(
				const Uint32 material_name_size,
				const Uint32 material_name_count,
				const Uint32 material_name_offset);

			virtual void add_object(const String &name,
				const glm::vec3 &position,
				const glm::vec3 &rotation_angles,
				const float scale, const float transparency,
				const Uint32 id, const bool blended,
				const SelectionType selection,
				const StringVector &material_names);
			virtual void add_light(const glm::vec3 &position,
				const glm::vec3 &color, const float radius,
				const Uint32 id);
			virtual void add_particle(const glm::vec3 &position,
				const String &name);
			virtual void set_terrain(const StringArray4 &albedo,
				const String &blend, const String &height,
				const String &dudv,
				const glm::vec3 &translation,
				const glm::vec3 &rotation_angles,
				const float scale, const float height_scale);
			virtual void set_tile(const Uint16 x, const Uint16 y,
				const Uint16 tile);
			virtual void set_height(const Uint16 x, const Uint16 y,
				const Uint16 height);

		public:
			/**
			 * Default constructor.
			 */
			MapLoader(const ReaderSharedPtr &reader);

			/**
			 * Default destructor.
			 */
			virtual ~MapLoader() throw();

			void load(Map &map);

			static bool check_format(const Uint8Array8 &id);

	};

}

#endif	/* UUID_c299b4bb_40d2_4781_a02d_4fd650012aca */
