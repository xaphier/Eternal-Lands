/****************************************************************************
 *            blendmodification.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_fd571033_cc6f_4a61_9a2b_900bfedbbc95
#define	UUID_fd571033_cc6f_4a61_9a2b_900bfedbbc95

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "modification.hpp"
#include "../imagevalue.hpp"

namespace eternal_lands
{

	class BlendModification: public Modification
	{
		private:
			ImageValueVector m_blend_values;
			ImageSharedPtr m_blend_image;
			Uint16 m_page_id;

		public:
			BlendModification(const ImageValueVector &blend_values,
				const ImageSharedPtr &blend_image,
				const Uint16 page_id);
			virtual ~BlendModification();
			virtual ModificationType get_type() const;
			virtual bool merge(Modification* modification);
			virtual bool undo(EditorMapData &editor);

	};

}

#endif	/* UUID_fd571033_cc6f_4a61_9a2b_900bfedbbc95 */
