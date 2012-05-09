/****************************************************************************
 *            autolock.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_31803c74_9766_4011_9420_6ba7fe4bc31c
#define	UUID_31803c74_9766_4011_9420_6ba7fe4bc31c

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

namespace eternal_lands
{

	class AutoLock
	{
		private:
			SDL_mutex* m_mutex;

		public:
			inline AutoLock(SDL_mutex* mutex): m_mutex(mutex)
			{
				SDL_LockMutex(m_mutex);
			}

			inline ~AutoLock() noexcept
			{
				SDL_UnlockMutex(m_mutex);
			}

	};

}

#endif	/* UUID_31803c74_9766_4011_9420_6ba7fe4bc31c */
