/****************************************************************************
 *            terraintexturemodification.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_893b06d8_f2b4_481c_b7ff_ca0c75568bfc
#define	UUID_893b06d8_f2b4_481c_b7ff_ca0c75568bfc

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "modification.hpp"
#include "globalid.hpp"

namespace eternal_lands
{

	class TerrainTextureModification: public Modification
	{
		private:
			String m_texture;
			GlobalId m_global_id;
			Uint16 m_index;

		public:
			TerrainTextureModification(const String &texture,
				const GlobalId global_id, const Uint16 index);
			virtual ~TerrainTextureModification();
			virtual ModificationType get_type() const;
			virtual bool merge(Modification* modification);
			virtual bool undo(Editor &editor);

	};

}

#endif	/* UUID_893b06d8_f2b4_481c_b7ff_ca0c75568bfc */
