/****************************************************************************
 *            instancingdata.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_ab0cde40_6cc7_4d3a_bc9a_9cc2d7ec55c6
#define	UUID_ab0cde40_6cc7_4d3a_bc9a_9cc2d7ec55c6

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "objectdescription.hpp"
#include "boundingbox.hpp"

/**
 * @file
 * @brief The @c class InstancingData.
 * This file contains the @c class InstancingData.
 */
namespace eternal_lands
{

	class InstancingData: public ObjectDescription
	{
		private:
			BoundingBox m_bounding_box;
			MeshDataToolSharedPtr m_mesh_data_tool;
			StringVector m_effect_names;
			BoolVector m_simple_shadows;

		public:
			InstancingData(
				const MeshDataCacheSharedPtr &mesh_data_cache,
				const ObjectDescription &object_descrition);

			~InstancingData() noexcept;

			inline const BoundingBox &get_bounding_box() const
				noexcept
			{
				return m_bounding_box;
			}

			inline const MeshDataToolSharedPtr &get_mesh_data_tool()
				const noexcept
			{
				return m_mesh_data_tool;
			}

			inline const StringVector &get_effect_names() const noexcept
			{
				return m_effect_names;
			}

			inline const String &get_effect_name(const Uint32 index)
				const
			{
				return m_effect_names[index];
			}

			inline void set_effect_name(const Uint32 index,
				const String &effect)
			{
				m_effect_names[index] = effect;
			}

			inline const BoolVector &get_simple_shadows() const
				noexcept
			{
				return m_simple_shadows;
			}

			inline bool get_simple_shadow(const Uint32 index) const
			{
				return m_simple_shadows[index];
			}

			inline void set_simple_shadow(const Uint32 index,
				const bool enabled)
			{
				m_simple_shadows[index] = enabled;
			}

	};

}

#endif	/* UUID_ab0cde40_6cc7_4d3a_bc9a_9cc2d7ec55c6 */
