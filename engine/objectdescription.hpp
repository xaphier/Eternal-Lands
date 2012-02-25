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
			MaterialEffectDescriptionVector m_materials;

		public:
			ObjectDescription(const ObjectData &object_data,
				const MaterialEffectDescriptionVector
					&materials);

			~ObjectDescription() throw();

			inline const MaterialEffectDescriptionVector
				&get_materials() const
			{
				return m_materials;
			}

	};

}

#endif	/* UUID_433a4ca6_0618_4d5a_8271_09426d974392 */

