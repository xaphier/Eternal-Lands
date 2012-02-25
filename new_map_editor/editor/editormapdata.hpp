/****************************************************************************
 *            editormapdata.hpp
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
#include "editorobjectdata.hpp"

/**
 * @file
 * @brief The @c class EditorMapData.
 * This file contains the @c class EditorMapData.
 */
namespace eternal_lands
{

	/**
	 * @brief @c class for maps.
	 *
	 * @c class for maps.
	 */
	class EditorMapData
	{
		private:
			std::map<Uint32, EditorObjectData> m_objects;
			std::map<Uint32, LightData> m_lights;
			std::map<Uint32, ParticleData> m_particles;

		public:

			EditorMapData();
			~EditorMapData() throw();

	};

}

#endif	/* UUID_6a19b8b7_ab72_4de0_a4a6_34c9d01035b7 */
