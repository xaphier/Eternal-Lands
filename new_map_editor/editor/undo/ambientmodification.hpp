/****************************************************************************
 *            ambientmodification.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
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

	class AmbientModification: public Modification
	{
		private:
			glm::vec3 m_color;

		public:
			AmbientModification(const glm::vec3 &color);
			virtual ~AmbientModification() throw();
			virtual ModificationType get_type() const;
			virtual bool merge(Modification* modification);
			virtual bool undo(EditorMapData &editor);

	};

}

#endif	/* UUID_57b3412c_36d3_492b_9728_880a87b14909 */
