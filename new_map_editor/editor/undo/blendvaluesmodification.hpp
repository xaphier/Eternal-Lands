/****************************************************************************
 *            blendvaluesmodification.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_fd571033_cc6f_4a61_9a2b_900bfedbbc95
#define	UUID_fd571033_cc6f_4a61_9a2b_900bfedbbc95

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "modification.hpp"
#include "../imagevalues.hpp"

namespace eternal_lands
{

	class BlendValuesModification: public Modification
	{
		private:
			ImageValuesVector m_blend_values;

			virtual bool do_merge(Modification* modification);

		public:
			BlendValuesModification(
				const ImageValuesVector &blend_values,
				const Uint32 edit_id);
			virtual ~BlendValuesModification() throw();
			virtual bool undo(EditorMapData &editor);
			bool add_needed(const Uint32 edit_id);

	};

}

#endif	/* UUID_fd571033_cc6f_4a61_9a2b_900bfedbbc95 */
