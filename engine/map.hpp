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
			const MeshBuilderWeakPtr m_mesh_builder;
			const MeshCacheWeakPtr m_mesh_cache;
			const EffectCacheWeakPtr m_effect_cache;
			const TextureCacheWeakPtr m_texture_cache;
			const MaterialDescriptionCacheWeakPtr
				m_material_description_cache;
			boost::scoped_ptr<RStarTree> m_object_tree;
			boost::scoped_ptr<RStarTree> m_light_tree;
			boost::scoped_ptr<BasicTerrainManager> m_terrain;
			Uint32ObjectSharedPtrMap m_objects;
			Uint32LightSharedPtrMap m_lights;
			Uint16MultiArray2 m_height_map;
			glm::vec4 m_ambient;
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

			inline EffectCacheSharedPtr get_effect_cache() const
			{
				EffectCacheSharedPtr result;

				result = m_effect_cache.lock();

				assert(result.get() != 0);

				return result;
			}

			inline TextureCacheSharedPtr get_texture_cache() const
			{
				TextureCacheSharedPtr result;

				result = m_texture_cache.lock();

				assert(result.get() != 0);

				return result;
			}

			inline MaterialDescriptionCacheSharedPtr
				get_material_description_cache() const
			{
				MaterialDescriptionCacheSharedPtr result;

				result = m_material_description_cache.lock();

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
				const EffectCacheSharedPtr &effect_cache,
				const TextureCacheSharedPtr &texture_cache,
				const MaterialDescriptionCacheSharedPtr
					&material_description_cache,
				const String &name);

			/**
			 * Default destructor.
			 */
			~Map() throw();

			void add_object(const ObjectData &object_data);
			void add_object(const ObjectData &object_data,
				const MaterialEffectDescriptionVector
					&materials);
			void add_object(const ObjectData &object_data,
				const StringVector &material_names);
			void add_object(const InstanceData &instance_data);
			void remove_object(const Uint32 id);
			bool get_object_position(const Uint32 id,
				glm::vec3 &position);
			void add_light(const glm::vec3 &position,
				const glm::vec3 &color, const float radius,
				const Uint32 id);
			void remove_light(const Uint32 id);
			void clear();
			void load(const glm::vec3 &ambient,
				const bool dungeon);
			void intersect(const Frustum &frustum,
				ObjectVisitor &visitor) const;
			void intersect(const Frustum &frustum,
				LightVisitor &visitor) const;
			const BoundingBox &get_bounding_box() const;
			float get_terrain_height_scale() const;

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
