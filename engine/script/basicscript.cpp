/****************************************************************************
 *            basicscript.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "basicscript.hpp"
#include "logging.hpp"
#include "exceptions.hpp"
#include "scriptengine.hpp"

namespace eternal_lands
{

	BasicScript::BasicScript(const String &name): m_func(0), m_name(name)
	{
	}

	BasicScript::~BasicScript() noexcept
	{
		if (m_func != 0)
		{
			m_func->Release();
		}
	}

	void BasicScript::set_function(const String &module,
		const String &function,
		const ScriptEngineSharedPtr &script_engine)
	{
		m_func = script_engine->get_function(module, get_name(),
			function);

		if (m_func == 0)
		{
			EL_THROW_EXCEPTION(ItemNotFoundException()
				<< errinfo_message(UTF8("function not found"))
				<< errinfo_item_name(function));
		}
	}

	void BasicScript::prepare(asIScriptContext* context)
	{
		context->Prepare(m_func);
	}

	void BasicScript::log_exception(asIScriptContext* context)
	{
		StringStream str;
		const char* section;
		Uint32 i, count;
		int line, column;

		line = context->GetExceptionLineNumber(&column, &section);

		LOG_ERROR(lt_angel_script, UTF8("Script exception '%1%' at "
			"section '%2%', line %3%:%4%."),
			context->GetExceptionString() % section % line %
			column);

		count = context->GetCallstackSize();

		for (i = 0; i < count; ++i)
		{
			line = context->GetLineNumber(i, 0, &section);
			str << section << ":" << line << "; ";
			str << context->GetFunction(i)->GetDeclaration();
			str << std::endl;
		}

		LOG_DEBUG(lt_angel_script, UTF8("Callstack: %1%"), str.str());
	}

}
