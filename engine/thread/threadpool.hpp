/****************************************************************************
 *            threadpool.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_f0abb968_849b_47f3_b2b9_5560878a6d5e
#define	UUID_f0abb968_849b_47f3_b2b9_5560878a6d5e

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

namespace eternal_lands
{

	class ThreadPool: public boost::noncopyable
	{
		friend void pool_inner_loop_function(ThreadPool* thread_pool);
		private:
			boost::ptr_deque<AbstractThreadTask> m_tasks;
			std::vector<SDL_Thread*> m_threads;
			SDL_mutex* m_mutex;
			SDL_cond* m_thread_condition;
			SDL_cond* m_wait_condition;
			Uint64 m_total_tasks;
			Uint64 m_done_tasks;
			bool m_running;

		public:
			ThreadPool(const Uint16 threads);

			~ThreadPool() noexcept;

			void add(std::auto_ptr<AbstractThreadTask> &task);

			void wait();

			inline bool get_running() const noexcept
			{
				return m_running;
			}

			inline Uint32 get_task_count() const
			{
				return m_tasks.size();
			}

	};

}

#endif	/* UUID_f0abb968_849b_47f3_b2b9_5560878a6d5e */
