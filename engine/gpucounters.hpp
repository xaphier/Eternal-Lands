/****************************************************************************
 *            gpucounters.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_6c5359ac_e1b8_4e1e_ac38_d7b3e5d09c3c
#define	UUID_6c5359ac_e1b8_4e1e_ac38_d7b3e5d09c3c

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

namespace eternal_lands
{

	class GpuCounters
	{
		private:
			std::queue<Uint32> m_session_ids;
			Uint32 m_session_id;

		public:
			GpuCounters();
			~GpuCounters() noexcept;

			void open_gl_context(void* context);
			void close_gl_context();
			void select_gl_context(void* context);
			void log_counters();
			void init_counters();
			void enable_counters();
			void disable_counters();
			void print_counters();
			void begin_session();
			void end_session();
			Uint32 get_required_passes();
			void begin_pass();
			void end_pass();
			void begin_sample(const Uint32 index);
			void end_sample();

	};

}

#endif	/* UUID_6c5359ac_e1b8_4e1e_ac38_d7b3e5d09c3c */
