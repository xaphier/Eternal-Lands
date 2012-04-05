/****************************************************************************
 *            threadstateutil.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_ae363f11_d260_444d_9156_ff3b6f303bd1
#define	UUID_ae363f11_d260_444d_9156_ff3b6f303bd1

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

namespace eternal_lands
{

	/**
	 * Types of blend modes.
	 * @{
	 */
	enum ThreadStateType
	{
		/**
		 * Thread is waiting.
		 */
		tst_waiting = 0,
		/**
		 * Thread is running.
		 */
		tst_running = 1,
		/**
		 * Thread exits.
		 */
		tst_exit = 2
	};
	/**
	 * @}
	 */

	class ThreadStateUtil
	{
		public:
			static const String &get_str(
				const ThreadStateType thread_state);
			static ThreadStateType get_thread_state(
				const String &str);
			static Uint32 get_thread_state_count();

	};

	std::ostream& operator<< (std::ostream &str,
		const ThreadStateType value);

}

#endif	/* UUID_ae363f11_d260_444d_9156_ff3b6f303bd1 */
