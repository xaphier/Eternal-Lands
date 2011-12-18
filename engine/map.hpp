/****************************************************************************
 *            map.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_6a19b8b7_ab72_4de0_a4a6_34c9d01035b7
#define	UUID_6a19b8b7_ab72_4de0_a4a6_34c9d01035b7

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "objectdata.hpp"

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
			MeshBuilderSharedPtr m_mesh_builder;
			MeshCacheSharedPtr m_mesh_cache;
			EffectCacheSharedPtr m_effect_cache;
			TextureCacheSharedPtr m_texture_cache;
			boost::scoped_ptr<RStarTree> m_object_tree;
			boost::scoped_ptr<RStarTree> m_light_tree;
			Uint32ObjectSharedPtrMap m_objects;
			Uint32LightSharedPtrMap m_lights;
			glm::vec4 m_ambient;
			String m_name;
			Uint32 m_id;
			bool m_dungeon;

			inline const MeshBuilderSharedPtr &get_mesh_builder()
				const
			{
				return m_mesh_builder;
			}

			inline const MeshCacheSharedPtr &get_mesh_cache()
				const
			{
				return m_mesh_cache;
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

		public:
			/**
			 * Default constructor.
			 */
			Map(const MeshBuilderSharedPtr &mesh_builder,
				const MeshCacheSharedPtr &mesh_cache,
				const EffectCacheSharedPtr &effect_cache,
				const TextureCacheSharedPtr &texture_cache);

			/**
			 * Default destructor.
			 */
			~Map() throw();

			void add_object(const ObjectData &object_data);
			void add_object(const InstanceData &instance_data);
			void remove_object(const Uint32 id);
			bool get_object_position(const Uint32 id,
				glm::vec3 &position);
			void add_light(const glm::vec3 &position,
				const glm::vec3 &color, const float ambient,
				const float radius, const Uint32 id);
			void remove_light(const Uint32 id);
			void clear();
			void load(const String &name, const glm::vec3 &ambient,
				const bool dungeon);

			inline const RStarTree &get_object_tree() const
			{
				return *m_object_tree;
			}

			inline const RStarTree &get_light_tree() const
			{
				return *m_light_tree;
			}

			inline void set_ambient(const glm::vec3 &ambient)
			{
				m_ambient = glm::vec4(ambient, 0.0f);
			}

			inline const glm::vec4 &get_ambient() const
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

	};

}

#endif	/* UUID_6a19b8b7_ab72_4de0_a4a6_34c9d01035b7 */
