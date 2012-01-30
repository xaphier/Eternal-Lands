/****************************************************************************
 *            object.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_887f3f6b_1f13_45e0_8dcb_8c6a37580971
#define	UUID_887f3f6b_1f13_45e0_8dcb_8c6a37580971

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "boundedobject.hpp"
#include "objectdata.hpp"
#include "loddata.hpp"

/**
 * @file
 * @brief The @c class Object.
 * This file contains the @c class Object.
 */
namespace eternal_lands
{

	/**
	 * @brief @c class for objects.
	 *
	 * @c class for objects.
	 */
	class Object: public BoundedObject
	{
		private:
			ObjectData m_object_data;
			LodData m_lod_data;
			AbstractMeshSharedPtr m_mesh;
			MaterialEffectVector m_material_effects;
			/**
			 * Data needed to draw instanced objects for selection.
			 */
			SubObjectVector m_sub_objects;
			/**
			 * Data needed for optimized depth/shadow drawing.
			 */
			boost::scoped_ptr<CalModel> m_model;
			Vec4Vector m_bones;

		protected:

			inline MaterialEffectVector
				&get_modifiable_material_effects()
			{
				return m_material_effects;
			}

			inline void set_bone(const Uint32 index,
				const glm::vec4 &value)
			{
				m_bones[index] = value;
			}

			/**
			 * Default constructor.
			 */
			Object(const ObjectData &object_data,
				const AbstractMeshSharedPtr &mesh,
				const MaterialEffectDescriptionVector
					&materials,
				const EffectCacheWeakPtr &effect_cache,
				const TextureCacheWeakPtr &texture_cache,
				CalCoreModel *core_model);

		public:
			/**
			 * Default constructor.
			 */
			Object(const ObjectData &object_data,
				const AbstractMeshSharedPtr &mesh,
				const MaterialEffectDescriptionVector
					&materials,
				const EffectCacheWeakPtr &effect_cache,
				const TextureCacheWeakPtr &texture_cache);

			/**
			 * Default constructor.
			 */
			Object(const InstanceData &instance_data,
				const AbstractMeshSharedPtr &mesh,
				const MaterialEffectDescriptionVector
					&materials,
				const EffectCacheWeakPtr &effect_cache,
				const TextureCacheWeakPtr &texture_cache,
				const SubObjectVector &sub_objects);

			/**
			 * Default destructor.
			 */
			virtual ~Object() throw();

			virtual void update_bounding_box();
			virtual void update_bones();

			inline Uint16 get_lod(const Uint16 distance) const
			{
				return m_lod_data.get_lod(distance);
			}

			inline Uint16 get_lods_count(const Uint16 lod) const
			{
				return m_lod_data.get_lods_count(lod);
			}

			inline Uint16 get_lods_offset(const Uint16 lod) const
			{
				return m_lod_data.get_lods_offset(lod);
			}

			inline Uint16 get_material_effects_index(
				const Uint16 lod, const Uint16 index) const
			{
				return m_lod_data.get_material_effects_index(
					lod, index);
			}

			inline Uint16 get_mesh_index(const Uint16 lod,
				const Uint16 index) const
			{
				return m_lod_data.get_mesh_index(lod, index);
			}


			inline void set_world_matrix(
				const glm::mat4x3 &world_matrix)
			{
				m_object_data.set_world_matrix(world_matrix);
			}

			inline void set_color(const glm::vec4 &color)
			{
				m_object_data.set_color(color);
			}

			inline void set_transparency(const float transparency)
			{
				m_object_data.set_transparency(transparency);
			}

			inline void set_blend(const bool blend)
			{
				m_object_data.set_blend(blend);
			}

			inline void set_selection(const SelectionType selection)
			{
				m_object_data.set_selection(selection);
			}

			inline const glm::mat4x3 &get_world_matrix() const
			{
				return m_object_data.get_world_matrix();
			}

			inline const glm::vec4 &get_color() const
			{
				return m_object_data.get_color();
			}

			inline const String &get_name() const
			{
				return m_object_data.get_name();
			}

			inline const AbstractMeshSharedPtr &get_mesh() const
			{
				return m_mesh;
			}

			inline const MaterialEffectVector
				&get_material_effects() const
			{
				return m_material_effects;
			}

			inline float get_transparency() const
			{
				return m_object_data.get_transparency();
			}

			inline Uint32 get_id() const
			{
				return m_object_data.get_id();
			}

			inline bool get_blend() const
			{
				return m_object_data.get_blend();
			}

			inline SelectionType get_selection() const
			{
				return m_object_data.get_selection();
			}

			inline const Vec4Vector &get_bones() const
			{
				return m_bones;
			}

			inline CalModel* get_model()
			{
				return m_model.get();
			}

			inline const SubObjectVector &get_sub_objects() const
			{
				return m_sub_objects;
			}

	};

}

#endif	/* UUID_887f3f6b_1f13_45e0_8dcb_8c6a37580971 */
