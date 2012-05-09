/****************************************************************************
 *            materialscript.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "materialscript.hpp"
#include "materialdata.hpp"

namespace eternal_lands
{

	namespace
	{

		const String material_function = String(UTF8("void main("
			"vec4 & in time, MaterialData & out material)"));

	}

	MaterialScript::MaterialScript(
		const ScriptEngineSharedPtr &script_engine, const String &name):
		BasicScript(name), m_mutex(SDL_CreateMutex())
	{
		assert(m_mutex != 0);

		set_function(String(UTF8("Material")), material_function,
			script_engine);
	}

	MaterialScript::~MaterialScript() noexcept
	{
		if (m_mutex != 0)
		{
			SDL_DestroyMutex(m_mutex);
		}

		m_mutex = 0;
	}

	void MaterialScript::lock()
	{
		SDL_LockMutex(m_mutex);
	}

	void MaterialScript::unlock()
	{
		SDL_UnlockMutex(m_mutex);
	}

	bool MaterialScript::execute(const glm::vec4 &time,
		MaterialData &material, asIScriptContext* context)
	{
		glm::vec4 tmp;
		int r;

		prepare(context);

		tmp = time;

		context->SetArgObject(0, glm::value_ptr(tmp));
		context->SetArgObject(1, &material);

		r = context->Execute();

		if (r == asEXECUTION_EXCEPTION)
		{
			log_exception(context);

			return false;
		}

		return r == asEXECUTION_FINISHED;
	}

}
