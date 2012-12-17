/****************************************************************************
 *            blendvaluemodification.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_194b0d29_d386_4e87_9737_8d2fe104f9c2
#define	UUID_194b0d29_d386_4e87_9737_8d2fe104f9c2

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "modification.hpp"
#include "../imagevalue.hpp"

namespace eternal_lands
{

	class BlendValueModification: public Modification
	{
		private:
			ImageValueVector m_blend_values;
			const Uint16 m_layer;

			virtual bool do_merge(Modification* modification);

		public:
			BlendValueModification(
				const ImageValueVector &blend_values,
				const Uint16 layer, const Uint32 edit_id);
			virtual ~BlendValueModification() throw();
			virtual bool undo(EditorMapData &editor);
			bool add_needed(const Uint32 edit_id);

	};

}

#endif	/* UUID_194b0d29_d386_4e87_9737_8d2fe104f9c2 */
