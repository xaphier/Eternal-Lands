/****************************************************************************
 *            materialscriptmanager.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_129e6d33_5ed1_4d16_9393_cf824f3451cd
#define	UUID_129e6d33_5ed1_4d16_9393_cf824f3451cd

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "thread/threadstateutil.hpp"

/**
 * @file
 * @brief The @c class MaterialScriptManager.
 * This file contains the @c class MaterialScriptManager.
 */
namespace eternal_lands
{

	class MaterialScriptManager: public boost::noncopyable
	{
		private:
			typedef std::set<Material*> MaterialPtrSet;

			asIScriptContext* m_context;
			SDL_mutex* m_mutex;
			SDL_cond* m_condition;
			SDL_Thread* m_thread;
			MaterialPtrSet m_materials;
			glm::vec4 m_time;
			volatile ThreadStateType m_state;

			void run();
			static int thread_main(void* material_script_manager); 

		public:
			/**
			 * Default constructor.
			 */
			MaterialScriptManager(
				const ScriptEngineSharedPtr &script_engine);

			/**
			 * Default destructor.
			 */
			~MaterialScriptManager() noexcept;

			void add_material(Material* material);
			void remove_material(Material* material);
			void execute_scripts(const glm::vec4 &time);

	};

}

#endif	/* UUID_129e6d33_5ed1_4d16_9393_cf824f3451cd */
