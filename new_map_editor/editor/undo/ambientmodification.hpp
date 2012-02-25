/****************************************************************************
 *            ambientmodification.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
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
			glm::vec4 m_color;
			Uint16 m_page_id;

		public:
			AmbientModification(const Uint16 page_id, const glm::vec4 &color);
			virtual ~AmbientModification();
			virtual ModificationType get_type() const;
			virtual bool merge(Modification* modification);
			virtual bool undo(Editor &editor);

	};

}

#endif	/* UUID_57b3412c_36d3_492b_9728_880a87b14909 */
