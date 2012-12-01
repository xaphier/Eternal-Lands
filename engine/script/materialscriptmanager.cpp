/****************************************************************************
 *            materialscriptmanager.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "materialscriptmanager.hpp"
#include "scriptengine.hpp"
#include "material.hpp"
#include "thread/autolock.hpp"

namespace eternal_lands
{

	namespace
	{

		class MaterialLock
		{
			private:
				Material* m_material;

			public:
				inline MaterialLock(Material* material):
					m_material(material)
				{
					m_material->lock();
				}

				inline ~MaterialLock()
				{
					m_material->unlock();
				}

				inline Material* operator->()
				{
					return m_material;
				}

		};

	}


	int MaterialScriptManager::thread_main(void* material_script_manager)
	{
		static_cast<MaterialScriptManager*>(
			material_script_manager)->run();

		return 1;
	}

	MaterialScriptManager::MaterialScriptManager(
		const ScriptEngineSharedPtr &script_engine)
	{
		m_context = script_engine->get_context();
		m_mutex = SDL_CreateMutex();
		m_condition = SDL_CreateCond();
		m_state = tst_waiting;

#if	SDL_VERSION_ATLEAST(2, 0, 0)
		m_thread = SDL_CreateThread(thread_main,
			UTF8("MaterialScriptManager"), this);
#else	/* SDL_VERSION_ATLEAST(2, 0, 0) */
		m_thread = SDL_CreateThread(thread_main, this);
#endif	/* SDL_VERSION_ATLEAST(2, 0, 0) */
	}

	MaterialScriptManager::~MaterialScriptManager() noexcept
	{
		m_state = tst_exit;

		SDL_CondSignal(m_condition);
		SDL_WaitThread(m_thread, 0);

		m_context->Release();

		SDL_DestroyMutex(m_mutex);
		SDL_DestroyCond(m_condition);
	}

	void MaterialScriptManager::run()
	{
		while (true)
		{
			AutoLock lock(m_mutex);

			while (m_state == tst_waiting)
			{
				SDL_CondWait(m_condition, m_mutex);
			}

			if (m_state == tst_exit)
			{
				return;
			}

			BOOST_FOREACH(Material* material, m_materials)
			{
				MaterialLock locked_material(material);

				locked_material->execute_script(m_time,
					m_context);
			}

			m_state = tst_waiting;
		}
	}

	void MaterialScriptManager::add_material(Material* material)
	{
		AutoLock lock(m_mutex);

		m_materials.insert(material);
	}

	void MaterialScriptManager::remove_material(Material* material)
	{
		AutoLock lock(m_mutex);

		m_materials.erase(material);
	}

	void MaterialScriptManager::execute_scripts(const glm::vec4 &time)
	{
		AutoLock lock(m_mutex);

		m_time = time;

		if (m_state == tst_waiting)
		{
			m_state = tst_running;

			SDL_CondSignal(m_condition);
		}
	}

}
