/****************************************************************************
 *            terrainpage.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_392bd254_1ec0_4580_b5ce_019370d46218
#define	UUID_392bd254_1ec0_4580_b5ce_019370d46218

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "object.hpp"

/**
 * @file
 * @brief The @c class TerrainPage.
 * This file contains the @c class TerrainPage.
 */
namespace eternal_lands
{

	/**
	 * @brief @c class for terrain page.
	 *
	 * @c class for terrain page.
	 */
	class TerrainPage: public Object
	{
		private:
			float m_min_z, m_max_z;

		public:
			/**
			 * Default constructor.
			 */
			TerrainPage(const ObjectData &object_data,
				const AbstractMeshSharedPtr &mesh,
				const MaterialEffectDescriptionVector
					&materials,
				const EffectCacheWeakPtr &effect_cache,
				const TextureCacheWeakPtr &texture_cache,
				const float min_z, const float max_z);

			/**
			 * Default destructor.
			 */
			~TerrainPage() throw();
			virtual void update_bounding_box();

			inline float get_min_z() const
			{
				return m_min_z;
			}

			inline float get_max_z() const
			{
				return m_max_z;
			}

	};

}

#endif	/* UUID_392bd254_1ec0_4580_b5ce_019370d46218 */