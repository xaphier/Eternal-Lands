/****************************************************************************
 *            terraintranslationmodification.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_1fe505f2_42aa_479b_a75f_25b0e14b0842
#define	UUID_1fe505f2_42aa_479b_a75f_25b0e14b0842

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "modification.hpp"

namespace eternal_lands
{

	class TerrainTranslationModification: public Modification
	{
		private:
			const glm::vec3 m_translation;

			virtual bool do_merge(Modification* modification);

		public:
			TerrainTranslationModification(
				const glm::vec3 &translation,
				const ModificationType type,
				const Uint32 edit_id);
			virtual ~TerrainTranslationModification() throw();
			virtual bool undo(EditorMapData &editor);

	};

}

#endif	/* UUID_1fe505f2_42aa_479b_a75f_25b0e14b0842 */
