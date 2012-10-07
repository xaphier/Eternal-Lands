/****************************************************************************
 *            blenddatamodification.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_2c953811_a713_4a6f_a2ef_499d36ea4fe9
#define	UUID_2c953811_a713_4a6f_a2ef_499d36ea4fe9

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "modification.hpp"
#include "shader/shaderblenddata.hpp"

namespace eternal_lands
{

	class BlendDataModification: public Modification
	{
		private:
			const ShaderBlendData m_blend_data;
			const Uint16 m_index;

			virtual bool do_merge(Modification* modification);

		public:
			BlendDataModification(
				const ShaderBlendData &blend_data,
				const Uint16 index,
				const ModificationType type,
				const Uint32 edit_id);
			virtual ~BlendDataModification() throw();
			virtual bool undo(EditorMapData &editor);

	};

}

#endif	/* UUID_2c953811_a713_4a6f_a2ef_499d36ea4fe9 */
