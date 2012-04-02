/****************************************************************************
 *            basicscript.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "basicscript.hpp"
#include "logging.hpp"
#include "exceptions.hpp"

namespace eternal_lands
{

	BasicScript::BasicScript(): m_func(0)
	{
	}

	BasicScript::~BasicScript() throw()
	{
		if (m_func != 0)
		{
			m_func->Release();
		}
	}

	void BasicScript::set_function(const String &module,
		const String &function, asIScriptEngine* engine)
	{
		asIScriptModule* mod;

		mod = engine->GetModule(module.get().c_str());

		if (mod == 0)
		{
			EL_THROW_EXCEPTION(ItemNotFoundException()
				<< errinfo_message(UTF8("module not found"))
				<< errinfo_item_name(module));
		}

		assert(mod != 0);

		m_func = mod->GetFunctionByDecl(function.get().c_str());

		if (m_func == 0)
		{
			EL_THROW_EXCEPTION(ItemNotFoundException()
				<< errinfo_message(UTF8("function not found"))
				<< errinfo_item_name(function));
		}
	}

	void BasicScript::prepare(asIScriptContext* ctx)
	{
		ctx->Prepare(m_func);
	}

	bool BasicScript::execute(asIScriptContext* ctx)
	{
		StringStream str;
		const char* section;
		Uint32 i, count;
		int r, line, column;

		ctx->Prepare(m_func);

		r = ctx->Execute();

		if (r == asEXECUTION_EXCEPTION)
		{
			line = ctx->GetExceptionLineNumber(&column, &section);

			LOG_ERROR(UTF8("Script exception '%1%' at section "
				"'%2%', line %3%:%4%."),
				ctx->GetExceptionString() % section % line %
				column);

			count = ctx->GetCallstackSize();

			for (i = 0; i < count; ++i)
			{
				line = ctx->GetLineNumber(i, 0, &section);
				str << section << ":" << line << "; ";
				str << ctx->GetFunction(i)->GetDeclaration();
				str << std::endl;
			}

			LOG_DEBUG(UTF8("Callstack: %1%"), str.str());

			return false;
		}

		return r == asEXECUTION_FINISHED;
	}

}
