/****************************************************************************
 *            ambientmodification.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_57b3412c_36d3_492b_9728_880a87b14909
#define	UUID_57b3412c_36d3_492b_9728_880a87b14909

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "modification.hpp"

namespace eternal_lands
{

	class GroundHemisphereModification: public Modification
	{
		private:
			glm::vec4 m_ground_hemisphere;

			virtual bool do_merge(Modification* modification);

		public:
			GroundHemisphereModification(
				const glm::vec4 &ground_hemisphere,
				const Uint32 edit_id);
			virtual ~GroundHemisphereModification() throw();
			virtual bool undo(EditorMapData &editor);

	};

}

#endif	/* UUID_57b3412c_36d3_492b_9728_880a87b14909 */
