/****************************************************************************
 *            materialscript.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "materialscript.hpp"
#include "scriptengine.hpp"
#include "logging.hpp"
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
		m_mutex(SDL_CreateMutex()), m_func(0), m_name(name)
	{
		assert(m_mutex != 0);

		m_func = script_engine->get_function(String(UTF8("Material")),
			get_name(), material_function);
	}

	MaterialScript::~MaterialScript() throw()
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
		StringStream str;
		glm::vec4 tmp;
		const char* section;
		Uint32 i, count;
		int r, line, column;

		context->Prepare(m_func);

		tmp = time;

		context->SetArgObject(0, glm::value_ptr(tmp));
		context->SetArgObject(1, &material);

		r = context->Execute();

		if (r == asEXECUTION_EXCEPTION)
		{
			line = context->GetExceptionLineNumber(&column,
				&section);

			LOG_ERROR(UTF8("Script exception '%1%' at section "
				"'%2%', line %3%:%4%."),
				context->GetExceptionString() % section % line
				% column);

			count = context->GetCallstackSize();

			for (i = 0; i < count; ++i)
			{
				line = context->GetLineNumber(i, 0, &section);
				str << section << ":" << line << "; ";
				str << context->GetFunction(i
					)->GetDeclaration();
				str << std::endl;
			}

			LOG_DEBUG(UTF8("Callstack: %1%"), str.str());

			return false;
		}

		return r == asEXECUTION_FINISHED;
	}

}
