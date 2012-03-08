/****************************************************************************
 *            editormapdata.hpp
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
#include "editorobjectdata.hpp"
#include "height.hpp"

/**
 * @file
 * @brief The @c class EditorMapData.
 * This file contains the @c class EditorMapData.
 */
namespace eternal_lands
{

	/**
	 * @brief @c class for maps.
	 *
	 * @c class for maps.
	 */
	class EditorMapData
	{
		private:
			std::map<Uint32, EditorObjectData> m_objects;
			std::map<Uint32, LightData> m_lights;
			std::map<Uint32, ParticleData> m_particles;
			boost::scoped_ptr<Scene> m_scene;
			ImageSharedPtr m_terrain_heights;
			ImageSharedPtr m_terrain_blend_values;
			Uint16MultiArray2 m_heights;
			Uint8MultiArray2 m_tiles;
			Uint32 m_id;
			bool m_selected;

		public:
			EditorMapData();
			~EditorMapData() throw();
			void add_object(const EditorObjectData &object);
			void add_light(const LightData &light);
			void add_particle(const ParticleData &particle);
			void modify_object(const EditorObjectData &object);
			void modify_light(const LightData &light);
			void modify_particle(const ParticleData &particle);
			bool remove_object(const Uint32 id);
			bool remove_light(const Uint32 id);
			bool remove_particle(const Uint32 id);
			bool get_object(const Uint32 id,
				EditorObjectData &object) const;
			bool get_light(const Uint32 id, LightData &light) const;
			bool get_particle(const Uint32 id,
				ParticleData &particle) const;
			void set_ambient_color(const glm::vec3 &color);
			const glm::vec3 &get_ambient_color() const;
			void set_tile(const Uint16 x, const Uint16 y,
				const Uint16 tile);
			Uint16 get_tile(const Uint16 x, const Uint16 y) const;
			Uint16Array2 get_tile_offset(const glm::vec2 &point)
				const; 
			void set_heights(const HeightVector &heights,
				const Uint16 id);
			void set_terrain_heights(const HeightVector &heights,
				const Uint16 id);
			void set_terrain_albedo_map(const String &name,
				const Uint16 index, const Uint16 id);
			void set_terrain_blend_map(const String &name,
				const Uint16 id);
			void set_terrain_height_map(const String &name,
				const Uint16 id);
			void set_terrain_dudv_map(const String &name,
				const Uint16 id);
			const String &get_terrain_albedo_map(const Uint16 index,
				const Uint16 id) const;
			const String &get_terrain_blend_map(const Uint16 id)
				const;
			const String &get_terrain_height_map(const Uint16 id)
				const;
			const String &get_terrain_dudv_map(const Uint16 id)
				const;
			Uint32 get_free_object_id() const;
			Uint32 get_free_light_id() const;

	};

}

#endif	/* UUID_6a19b8b7_ab72_4de0_a4a6_34c9d01035b7 */
