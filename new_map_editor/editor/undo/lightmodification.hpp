/****************************************************************************
 *            lightmodification.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_c133f155_8d7a_462d_a16a_5087a1a8a43f
#define	UUID_c133f155_8d7a_462d_a16a_5087a1a8a43f

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "modification.hpp"
#include "lightdata.hpp"

namespace eternal_lands
{

	class LightModification: public Modification
	{
		private:
			LightData m_data;
			const ModificationType m_type;

		public:
			LightModification(const LightData &data,
				const ModificationType type);
			virtual ~LightModification() throw();
			virtual ModificationType get_type() const;
			virtual bool merge(Modification* modification);
			virtual bool undo(EditorMapData &editor);

	};

}

#endif	/* UUID_c133f155_8d7a_462d_a16a_5087a1a8a43f */