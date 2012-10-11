/****************************************************************************
 *            threadpool.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "threadpool.hpp"
#include "autolock.hpp"
#include "abstractthreadtask.hpp"

namespace eternal_lands
{

	void pool_inner_loop_function(ThreadPool* thread_pool)
	{
		boost::scoped_ptr<AbstractThreadTask> task;

		{
			AutoLock lock(thread_pool->m_mutex);

			while (thread_pool->get_task_count() == 0)
			{
				if (!thread_pool->get_running())
				{
					return;
				}

				SDL_CondSignal(thread_pool->m_wait_condition);

				SDL_CondWait(thread_pool->m_thread_condition,
					thread_pool->m_mutex);
			}

			task.reset(thread_pool->m_tasks.pop_back().release());
		}

		try
		{
			(*task)();
		}
		catch (...)
		{
			std::cerr << "error!" << std::endl;
		}

		{
			AutoLock lock(thread_pool->m_mutex);

			thread_pool->m_done_tasks++;
		}
	}

	int pool_function(void* data)
	{
		ThreadPool* thread_pool;

		thread_pool = static_cast<ThreadPool*>(data);

		while (thread_pool->get_running())
		{
			pool_inner_loop_function(thread_pool);
		}

		return 0;
	}

	ThreadPool::ThreadPool(const Uint16 threads)
	{
		Uint16 i;

		m_running = true;

		m_mutex = SDL_CreateMutex();
		m_thread_condition = SDL_CreateCond();
		m_wait_condition = SDL_CreateCond();

		for (i = 0; i < threads; ++i)
		{
			m_threads.push_back(SDL_CreateThread(&pool_function,
				this));
		}
	}

	ThreadPool::~ThreadPool() noexcept
	{
		Uint32 i, count;
		int status;

		m_running = false;

		SDL_CondBroadcast(m_thread_condition);

		count = m_threads.size();

		for (i = 0; i < count; ++i)
		{
			SDL_WaitThread(m_threads[i], &status);
		}

		SDL_DestroyMutex(m_mutex);
		SDL_DestroyCond(m_thread_condition);
		SDL_DestroyCond(m_wait_condition);
	}

	void ThreadPool::add(std::auto_ptr<AbstractThreadTask> &task)
	{
		{
			AutoLock lock(m_mutex);

			m_tasks.push_back(task);

			m_total_tasks++;
		}

		SDL_CondSignal(m_thread_condition);
	}

	void ThreadPool::wait()
	{
		AutoLock lock(m_mutex);

		while (m_total_tasks == m_done_tasks)
		{
			SDL_CondWait(m_wait_condition, m_mutex);
		}
	}

}
