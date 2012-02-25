/****************************************************************************
 *            object.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
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
			Mat2x4Vector m_bones;

		protected:

			inline MaterialEffectVector
				&get_modifiable_material_effects()
			{
				return m_material_effects;
			}

			inline void set_bone(const Uint32 index,
				const glm::mat2x4 &value)
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
			Object(const ObjectData &object_data,
				const AbstractMeshSharedPtr &mesh,
				const MaterialEffectDescriptionVector
					&materials,
				const EffectCacheWeakPtr &effect_cache,
				const TextureCacheWeakPtr &texture_cache,
				const LodData &lod_data);

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


			inline void set_world_transformation(
				const Transformation &world_transformation)
			{
				m_object_data.set_world_transformation(
					world_transformation);
			}

			inline void set_transparency(const float transparency)
			{
				m_object_data.set_transparency(transparency);
			}

			inline void set_state_blend(const bool blend)
			{
				m_object_data.set_state_blend(blend);
			}

			inline void set_state_stencil(const bool stencil)
			{
				m_object_data.set_state_stencil(stencil);
			}

			inline void set_selection(const SelectionType selection)
			{
				m_object_data.set_selection(selection);
			}

			inline void set_stencil_value(const Uint8 stencil_value)
			{
				m_object_data.set_stencil_value(stencil_value);
			}

			inline const Transformation &get_world_transformation()
				const
			{
				return m_object_data.get_world_transformation();
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

			inline Uint8 get_stencil_value() const
			{
				return m_object_data.get_stencil_value();
			}

			inline bool get_state_blend() const
			{
				return m_object_data.get_state_blend();
			}

			inline bool get_state_stencil() const
			{
				return m_object_data.get_state_stencil();
			}

			inline SelectionType get_selection() const
			{
				return m_object_data.get_selection();
			}

			inline const Mat2x4Vector &get_bones() const
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
