/****************************************************************************
 *            abstractthreadtask.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_efead350_426b_4cd8_89cf_afb47c13cc8a
#define	UUID_efead350_426b_4cd8_89cf_afb47c13cc8a

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

namespace eternal_lands
{

	class AbstractThreadTask
	{
		private:
			Uint32 m_group_id;

		protected:
			AbstractThreadTask();

		public:
			virtual ~AbstractThreadTask() noexcept;

			virtual void operator()() = 0;

	};

}

#endif	/* UUID_efead350_426b_4cd8_89cf_afb47c13cc8a */
