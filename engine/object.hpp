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
			AbstractMeshSharedPtr m_mesh;
			MaterialVector m_materials;
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

			inline MaterialVector &get_modifiable_materials()
			{
				return m_materials;
			}

		public:
			/**
			 * Default constructor.
			 */
			Object(const ObjectData &object_data,
				const AbstractMeshSharedPtr &mesh,
				const MaterialDescriptionVector &materials,
				const EffectCacheWeakPtr &effect_cache,
				const TextureCacheWeakPtr &texture_cache);

			/**
			 * Default constructor.
			 */
			Object(const InstanceData &instance_data,
				const AbstractMeshSharedPtr &mesh,
				const MaterialDescriptionVector &materials,
				const EffectCacheWeakPtr &effect_cache,
				const TextureCacheWeakPtr &texture_cache,
				const SubObjectVector &sub_objects);

			/**
			 * Default constructor.
			 */
			Object(const ObjectData &object_data,
				const AbstractMeshSharedPtr &mesh,
				const MaterialDescriptionVector &materials,
				const EffectCacheWeakPtr &effect_cache,
				const TextureCacheWeakPtr &texture_cache,
				CalCoreModel *core_model);

			/**
			 * Default destructor.
			 */
			virtual ~Object() throw();

			void update_bounding_box();
			void update_skeleton_bounding_box(
				const float scale = 1.0f);
			void update_bones();

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

			inline const MaterialVector &get_materials() const
			{
				return m_materials;
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
