/****************************************************************************
 *            objectdescription.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_433a4ca6_0618_4d5a_8271_09426d974392
#define	UUID_433a4ca6_0618_4d5a_8271_09426d974392

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "objectdata.hpp"

/**
 * @file
 * @brief The @c class ObjectDescription.
 * This file contains the @c class ObjectDescription.
 */
namespace eternal_lands
{

	class ObjectDescription: public ObjectData
	{
		private:
			StringVector m_material_names;

		public:
			inline ObjectDescription()
			{
			}

			inline ObjectDescription(const ObjectData &object_data,
				const StringVector &material_names):
				ObjectData(object_data),
				m_material_names(material_names)
			{
			}

			inline ObjectDescription(
				const Transformation &world_transformation,
				const StringVector &material_names,
				const String &name, const float transparency,
				const Uint32 id, const SelectionType selection,
				const BlendType blend): ObjectData(
					world_transformation, name,
					transparency, id, selection, blend),
				m_material_names(material_names)
			{
			}

			inline ~ObjectDescription() noexcept
			{
			}

			inline const StringVector &get_material_names() const
				noexcept
			{
				return m_material_names;
			}

			inline void set_material_names(
				const StringVector &material_names) noexcept
			{
				m_material_names = material_names;
			}

	};

}

#endif	/* UUID_433a4ca6_0618_4d5a_8271_09426d974392 */
