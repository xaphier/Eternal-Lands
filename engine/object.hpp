/****************************************************************************
 *            object.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
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
			MaterialSharedPtrVector m_materials;
			/**
			 * Data needed to draw instanced objects for selection.
			 */
			SubObjectVector m_sub_objects;
			boost::scoped_ptr<CalModel> m_model;
			Mat2x4Vector m_bones;

		protected:

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
				const MaterialSharedPtrVector &materials,
				CalCoreModel *core_model);

		public:
			/**
			 * Default constructor.
			 */
			Object(const ObjectData &object_data,
				const AbstractMeshSharedPtr &mesh,
				const MaterialSharedPtrVector &materials);

			/**
			 * Default constructor.
			 */
			Object(const InstanceData &instance_data,
				const AbstractMeshSharedPtr &mesh,
				const MaterialSharedPtrVector &materials,
				const SubObjectVector &sub_objects);

			/**
			 * Default destructor.
			 */
			virtual ~Object() noexcept;

			virtual void update_bounding_box();
			virtual void update_bones();

			inline void set_world_transformation(
				const Transformation &world_transformation)
				noexcept
			{
				m_object_data.set_world_transformation(
					world_transformation);
			}

			inline void set_transparency(const float transparency)
				noexcept
			{
				m_object_data.set_transparency(transparency);
			}

			inline void set_blend(const BlendType blend) noexcept
			{
				m_object_data.set_blend(blend);
			}

			inline void set_selection(const SelectionType selection)
				noexcept
			{
				m_object_data.set_selection(selection);
			}

			inline const Transformation &get_world_transformation()
				const noexcept
			{
				return m_object_data.get_world_transformation();
			}

			inline const String &get_name() const noexcept
			{
				return m_object_data.get_name();
			}

			inline const AbstractMeshSharedPtr &get_mesh() const
				noexcept
			{
				return m_mesh;
			}

			inline const MaterialSharedPtrVector &get_materials()
				const noexcept
			{
				return m_materials;
			}

			inline float get_transparency() const noexcept
			{
				return m_object_data.get_transparency();
			}

			inline Uint32 get_id() const noexcept
			{
				return m_object_data.get_id();
			}

			inline BlendType get_blend() const noexcept
			{
				return m_object_data.get_blend();
			}

			inline SelectionType get_selection() const noexcept
			{
				return m_object_data.get_selection();
			}

			inline const Mat2x4Vector &get_bones() const noexcept
			{
				return m_bones;
			}

			inline CalModel* get_model() noexcept
			{
				return m_model.get();
			}

			inline const SubObjectVector &get_sub_objects() const
				noexcept
			{
				return m_sub_objects;
			}

	};

}

#endif	/* UUID_887f3f6b_1f13_45e0_8dcb_8c6a37580971 */
