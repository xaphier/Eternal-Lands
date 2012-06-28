/****************************************************************************
 *            scriptengine.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "scriptengine.hpp"
#include "angelscript/scriptarray/scriptarray.h"
#include "angelscript/scriptstdstring/scriptstdstring.h"
#include "angelscript/scriptbuilder/scriptbuilder.h"
#include "angelscript/scriptglm/scriptglm.h"
#include "logging.hpp"
#include "filesystem.hpp"
#include "reader.hpp"
#include "materialdata.hpp"
#include "exceptions.hpp"

namespace eternal_lands
{

	namespace
	{

		void message_callback(const asSMessageInfo *message,
			void *param)
		{
			switch (message->type)
			{
				case asMSGTYPE_ERROR:
					LOG_ERROR(lt_angel_script,
						UTF8("Script error at line "
						"%1% [%2%], section '%3%': "
						"%4%"), message->row %
						message->col % message->section
						% message->message);
					break;
				case asMSGTYPE_WARNING:
					LOG_WARNING(lt_angel_script,
						UTF8("Script warning at "
						"line %1% [%2%], section '%3%'"
						": %4%"), message->row %
						message->col % message->section
						% message->message);
					break;
				case asMSGTYPE_INFORMATION:
					LOG_INFO(lt_angel_script,
						UTF8("Script info at line "
						"%1% [%2%], section '%3%': "
						"%4%"), message->row %
						message->col % message->section
						% message->message);
					break;
			}
		}

		int include_callback(const char* include, const char* from,
			CScriptBuilder* builder, void* userParam)
		{
			ReaderSharedPtr reader;
			String file_name, str;

			file_name = FileSystem::get_strip_relative_path(
				String(include));

			if (!static_cast<FileSystem*>(userParam
				)->get_file_if_readable(file_name, reader))
			{
				return -1;
			}

			str = reader->read_utf8_string(reader->get_size());

			return builder->AddSectionFromMemory(str.get().c_str(),
				file_name.get().c_str());
		}

		void as_print(const std::string &str)
		{
			printf("%s\n", str.c_str());
		}

	}

	ScriptEngine::ScriptEngine(const FileSystemSharedPtr &file_system):
		m_file_system(file_system)
	{
		Sint32 r;

		// Create the script engine
		m_engine = asCreateScriptEngine(ANGELSCRIPT_VERSION);
		assert(get_engine() != 0);

		get_engine()->SetEngineProperty(asEP_DISALLOW_GLOBAL_VARS, false);

		r = get_engine()->SetMessageCallback(
			asFUNCTION(message_callback), 0, asCALL_CDECL);
		assert(r >= 0);

		init();
	}

	void ScriptEngine::init()
	{
		Sint32 r;

		RegisterScriptArray(get_engine(), true);
		RegisterStdString(get_engine());
		RegisterStdStringUtils(get_engine());
		RegisterGlm(get_engine());
		register_material_data();

		r = get_engine()->RegisterGlobalFunction(
			"void print(const string &in)", asFUNCTION(as_print),
			asCALL_CDECL);
		assert(r >= 0);
	}

	void constructor_material_data(MaterialData *self)
	{
		new(self) MaterialData();
	}

	void ScriptEngine::register_material_data()
	{
		int r;

		r = get_engine()->RegisterObjectType("MaterialData",
			sizeof(MaterialData),
			asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CAK);
		assert(r >= 0);

		r = get_engine()->RegisterObjectBehaviour("MaterialData",
			asBEHAVE_CONSTRUCT, "void f()",
			asFUNCTION(constructor_material_data),
			asCALL_CDECL_OBJLAST);
		assert(r >= 0);

		r = get_engine()->RegisterObjectMethod("MaterialData",
			"void set_blend_size(const vec4 &in, uint)",
			asMETHODPR(MaterialData, set_blend_size,
			(const glm::vec4 &, const Uint16), void),
			asCALL_THISCALL);
		assert(r >= 0);

		r = get_engine()->RegisterObjectMethod("MaterialData",
			"void set_albedo_scale_offset(const mat2x4 &in, uint)",
			asMETHODPR(MaterialData, set_albedo_scale_offset,
			(const glm::mat2x4 &, const Uint16), void),
			asCALL_THISCALL);
		assert(r >= 0);

		r = get_engine()->RegisterObjectMethod("MaterialData",
			"void set_texture_matrix(const mat2x3 &in, uint)",
			asMETHODPR(MaterialData, set_texture_matrix,
			(const glm::mat2x3 &, const Uint16), void),
			asCALL_THISCALL);
		assert(r >= 0);

		r = get_engine()->RegisterObjectMethod("MaterialData",
			"void set_emission_scale_offset(const mat2x3 &in)",
			asMETHODPR(MaterialData, set_emission_scale_offset,
			(const glm::mat2x3 &), void), asCALL_THISCALL);
		assert(r >= 0);

		r = get_engine()->RegisterObjectMethod("MaterialData",
			"void set_specular_scale_offset(const vec4 &in)",
			asMETHODPR(MaterialData, set_specular_scale_offset,
			(const glm::vec4 &), void), asCALL_THISCALL);
		assert(r >= 0);

		r = get_engine()->RegisterObjectMethod("MaterialData",
			"void set_color(const vec4 &in)",
			asMETHODPR(MaterialData, set_color,
			(const glm::vec4 &), void), asCALL_THISCALL);
		assert(r >= 0);

		r = get_engine()->RegisterObjectMethod("MaterialData",
			"void set_cast_shadows(bool)",
			asMETHODPR(MaterialData, set_cast_shadows,
			(const bool), void), asCALL_THISCALL);
		assert(r >= 0);

		r = get_engine()->RegisterObjectMethod("MaterialData",
			"void set_culling(bool)",
			asMETHODPR(MaterialData, set_culling,
			(const bool), void), asCALL_THISCALL);
		assert(r >= 0);

		r = get_engine()->RegisterObjectMethod("MaterialData",
			"const vec4 &get_blend_size(uint) const",
			asMETHODPR(MaterialData, get_blend_size,
			(const Uint16) const, const glm::vec4 &),
			asCALL_THISCALL);
		assert(r >= 0);

		r = get_engine()->RegisterObjectMethod("MaterialData",
			"const mat2x4 &get_albedo_scale_offset(uint) const",
			asMETHODPR(MaterialData, get_albedo_scale_offset,
			(const Uint16) const, const glm::mat2x4 &),
			asCALL_THISCALL);
		assert(r >= 0);

		r = get_engine()->RegisterObjectMethod("MaterialData",
			"const mat2x3 &get_texture_matrix(uint)",
			asMETHODPR(MaterialData, get_texture_matrix,
			(const Uint16) const, const glm::mat2x3 &),
			asCALL_THISCALL);
		assert(r >= 0);

		r = get_engine()->RegisterObjectMethod("MaterialData",
			"const mat2x3 &get_emission_scale_offset()",
			asMETHODPR(MaterialData , get_emission_scale_offset,
			() const, const glm::mat2x3 &), asCALL_THISCALL);
		assert(r >= 0);

		r = get_engine()->RegisterObjectMethod("MaterialData",
			"const vec4 &get_specular_scale_offset()",
			asMETHODPR(MaterialData, get_specular_scale_offset,
			() const, const glm::vec4 &), asCALL_THISCALL);
		assert(r >= 0);

		r = get_engine()->RegisterObjectMethod("MaterialData",
			"const vec4 &get_color()",
			asMETHODPR(MaterialData, get_color, () const,
			const glm::vec4 &), asCALL_THISCALL);
		assert(r >= 0);

		r = get_engine()->RegisterObjectMethod("MaterialData",
			"bool get_cast_shadows()",
			asMETHODPR(MaterialData, get_cast_shadows, () const,
			bool), asCALL_THISCALL);
		assert(r >= 0);

		r = get_engine()->RegisterObjectMethod("MaterialData",
			"bool get_culling()",
			asMETHODPR(MaterialData, get_culling, () const, bool),
			asCALL_THISCALL);
		assert(r >= 0);
	}

	ScriptEngine::~ScriptEngine() noexcept
	{
		get_engine()->Release();
	}

	asIScriptContext* ScriptEngine::get_context()
	{
		return get_engine()->CreateContext();
	}

	asIScriptFunction* ScriptEngine::get_function(const String &name,
		const String &file, const String &function)
	{
		CScriptBuilder builder;
		ReaderSharedPtr reader;
		asIScriptModule* module;
		asIScriptFunction* result;
		String file_name, str;
		Sint32 r;

		file_name = FileSystem::get_strip_relative_path(file);

		r = builder.StartNewModule(get_engine(), name.get().c_str());

		if (r < 0) 
		{
			EL_THROW_EXCEPTION(BadAllocException()
				<< errinfo_message(UTF8("Out of memory")));
		}

		reader = get_file_system()->get_file(file_name);

		str = reader->read_utf8_string(reader->get_size());

		r = builder.AddSectionFromMemory(str.get().c_str(),
			file_name.get().c_str());

		if (r < 0)
		{
			EL_THROW_EXCEPTION(ASException()
				<< errinfo_message(UTF8("Unkonw error")));
		}

		r = builder.BuildModule();

		if (r < 0)
		{
			EL_THROW_EXCEPTION(ASException()
				<< errinfo_message(UTF8("Module build error")));
		}

		module = get_engine()->GetModule(name.get().c_str());

		if (module == 0)
		{
			EL_THROW_EXCEPTION(ASException()
				<< errinfo_message(UTF8("Module not found")));
		}

		result = module->GetFunctionByDecl(function.get().c_str());

		if (result == 0)
		{
			EL_THROW_EXCEPTION(ASException()
				<< errinfo_message(UTF8("Function not found")));
		}

		return result;
	}

	void ScriptEngine::build_module(const String &name, const String &file)
	{
		CScriptBuilder builder;
		ReaderSharedPtr reader;
		String file_name, str;
		Sint32 r;

		r = builder.StartNewModule(get_engine(), name.get().c_str());

		if (r < 0) 
		{
			// If the code fails here it is usually because there
			// is no more memory to allocate the module
			printf("Unrecoverable error while starting a new module.\n");
			return;
		}

		file_name = FileSystem::get_strip_relative_path(file);

		reader = get_file_system()->get_file(file_name);

		str = reader->read_utf8_string(reader->get_size());

		r = builder.AddSectionFromMemory(str.get().c_str(),
			file_name.get().c_str());

		if (r < 0)
		{
			// The builder wasn't able to load the file. Maybe the file
			// has been removed, or the wrong name was given, or some
			// preprocessing commands are incorrectly written.
			printf("Please correct the errors in the script and try again.\n");
			return;
		}

		r = builder.BuildModule();

		if (r < 0)
		{
			// An error occurred. Instruct the script writer to fix the 
			// compilation errors that were listed in the output stream.
			printf("Please correct the errors in the script and try again.\n");
			return;
		}
	}

	void ScriptEngine::run_main(const String &name, const String &file)
	{
		CScriptBuilder builder;
		ReaderSharedPtr reader;
		String file_name, str;
		asIScriptContext* ctx;
		asIScriptModule* mod;
		asIScriptFunction* func;
		Sint32 r;

		r = builder.StartNewModule(get_engine(), name.get().c_str());

		if (r < 0) 
		{
			// If the code fails here it is usually because there
			// is no more memory to allocate the module
			printf("Unrecoverable error while starting a new module.\n");
			return;
		}

		file_name = FileSystem::get_strip_relative_path(file);

		reader = get_file_system()->get_file(file_name);

		str = reader->read_utf8_string(reader->get_size());

		r = builder.AddSectionFromMemory(str.get().c_str(),
			file_name.get().c_str());

		if (r < 0)
		{
			// The builder wasn't able to load the file. Maybe the file
			// has been removed, or the wrong name was given, or some
			// preprocessing commands are incorrectly written.
			printf("Please correct the errors in the script and try again.\n");
			return;
		}

		r = builder.BuildModule();

		if (r < 0)
		{
			// An error occurred. Instruct the script writer to fix the 
			// compilation errors that were listed in the output stream.
			printf("Please correct the errors in the script and try again.\n");
			return;
		}

		// Find the function that is to be called. 
		mod = get_engine()->GetModule(name.get().c_str());

		assert(mod != 0);

		func = mod->GetFunctionByDecl("void main()");

		if (func == 0)
		{
			// The function couldn't be found. Instruct the script writer
			// to include the expected function in the script.
			printf("The script must have the function 'void main()'. Please add it and try again.\n");
			return;
		}

		// Create our context, prepare it, and then execute
		ctx = get_engine()->CreateContext();

		ctx->Prepare(func);

		r = ctx->Execute();

		if (r != asEXECUTION_FINISHED)
		{
			// The execution didn't complete as expected. Determine what happened.
			if( r == asEXECUTION_EXCEPTION )
			{
				// An exception occurred, let the script writer know what happened so it can be corrected.
				printf("An exception '%s' occurred. Please correct the code and try again.\n", ctx->GetExceptionString());
			}
		}

		ctx->Release();

		get_engine()->DiscardModule(name.get().c_str());
	}

}
