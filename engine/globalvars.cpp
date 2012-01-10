/****************************************************************************
 *            globalvars.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "globalvars.hpp"
#include "exceptions.hpp"
#include "lua.hpp"

namespace eternal_lands
{
#if	0
	namespace
	{

		const char* lua_key = "el_lua_global_vars_key";

		int get_global_var(lua_State *l)
		{
			BasicLua lua(l);
			GlobalVars* global_vars;
			String index;
			Sint32 n;

			n = lua_gettop(lua.get());  // Number of arguments

			if (n != 1)
			{
				return luaL_error(lua.get(),
					"Got %d arguments, but expected 1", n); 
			}

			if (!lua.is_string(-1))
			{
				return luaL_error(lua.get(),
					"Got %s argument as first parameter, "
					"but expected string",
					lua.type_name(-1).get().c_str()); 
			}

			index = lua.to_string(-1);

			/* retrieve a number */
			lua_pushstring(lua.get(), lua_key);  /* push address */
			lua_gettable(lua.get(), LUA_REGISTRYINDEX);  /* retrieve value */
			global_vars = (GlobalVars*)lua_touserdata(lua.get(), -1);  /* convert to global vars ptr */
			lua.pop(1);

			try
			{
				lua.push_variant(global_vars->get_value(index));
			}
			catch (const boost::exception &exception)
			{
				return luaL_error(lua.get(), "%s",
					boost::diagnostic_information(
						exception).c_str());
			}

			return 1;
		}

		int set_global_var(lua_State *l)
		{
			BasicLua lua(l);
			GlobalVars* global_vars;
			String index;
			Sint32 n;
			Variant value;

			n = lua_gettop(lua.get());  // Number of arguments

			if (n != 2)
			{
				return luaL_error(lua.get(),
					"Got %d arguments, but expected 2", n); 
			}

			if (!lua.is_string(-2))
			{
				return luaL_error(lua.get(),
					"Got %s argument as first parameter, "
					"but expected string",
					lua.type_name(-1).get().c_str()); 
			}

			index = lua.to_string(-2);

			/* retrieve a number */
			lua_pushstring(lua.get(), lua_key);  /* push address */
			lua_gettable(lua.get(), LUA_REGISTRYINDEX);  /* retrieve value */
			global_vars = (GlobalVars*)lua_touserdata(lua.get(), -1);  /* convert to global vars ptr */
			lua.pop(1);

			try
			{
				value = global_vars->get_value(index);
				lua.load_variant(value);
				global_vars->set_value(index, value);
			}
			catch (const boost::exception &exception)
			{
				return luaL_error(lua.get(), "%s",
					boost::get_error_info<errinfo_message>(
						exception)->c_str());
			}

			return 0;
		}

		int list_global_vars(lua_State *L)
		{
			Sint32 n;

			n = lua_gettop(L);  // Number of arguments

			if (n != 0)
			{
				return luaL_error(L,
					"Got %d arguments expected 0", n); 
			}

			return 0;
		}

		const luaL_Reg global_vars_functions[] = {
			{ "get_global_var", get_global_var },
			{ "set_global_var", set_global_var },
			{ "list_global_vars", list_global_vars },
			{ 0, 0 }
		};

		template<typename T>
		T convert_variant(const Variant &value, const String &name)
		{
			boost::format format_string("Var %1% has type %2% and "
				"not %3%.");

			if (value.type() == typeid(T))
			{
				return boost::get<T>(value);
			}

			format_string % name % value.type().name() %
				typeid(T).name();

			EL_THROW_EXCEPTION(VariantException()
				<< errinfo_message(format_string.str())
				<< errinfo_parameter_name(name));
		}

	}

	GlobalVars::GlobalVars()
	{
		set_value(String("shader.bone_count"), 74);
		set_value(String("shader.shadow_scale"), 0.6f);
		set_value(String("shader.vertex_light_count"), 5);
		set_value(String("shader.fragment_light_count"), 3);
		set_value(String("shader.dynamic_light_count"), false);

		set_value(String("settings.shadow_count"), 2);
		set_value(String("settings.fog"), false);
		set_value(String("settings.quality"), String("medium"));
		set_value(String("settings.view_distance"), 100.0f);
		set_value(String("settings.shadow_distance"), 40.0f);
	}

	GlobalVars::~GlobalVars() throw()
	{
	}

	void GlobalVars::add_to_lua(Lua &lua)
	{
		/* set global EL */
		lua_pushvalue(lua.get(), LUA_GLOBALSINDEX);
		lua_setglobal(lua.get(), "EL");
		/* open lib into global table */
		luaL_register(lua.get(), "EL",
			global_vars_functions);  
		lua_pushvalue(lua.get(), -1);
		lua_setfield(lua.get(), -2, "__index");
		/* store global var ptr */
		lua_pushstring(lua.get(), lua_key);  /* push address */
		lua_pushlightuserdata(lua.get(), this);  /* push address */
		/* registry[lua_key] = *this */
		lua_settable(lua.get(), LUA_REGISTRYINDEX);
	}

	bool GlobalVars::has_value(const String &name) const
	{
		return m_vars.find(name) != m_vars.end();
	}

	String GlobalVars::get_value_type_name(const String &name) const
	{
		StringVariantMap::const_iterator found;
		std::string str;

		found = m_vars.find(name);

		if (found == m_vars.end())
		{
			str = "Invalid global var name ";
			str += name;

			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_message(str)
				<< errinfo_string_value(name));
		}

		return String(found->second.type().name());
	}

	const Variant &GlobalVars::get_value(const String &name) const
	{
		StringVariantMap::const_iterator found;
		std::string str;

		found = m_vars.find(name);

		if (found == m_vars.end())
		{
			str = "Invalid global var name ";
			str += name;

			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_message(str)
				<< errinfo_string_value(name));
		}

		return found->second;
	}

	String GlobalVars::get_string(const String &name) const
	{
		return convert_variant<String>(get_value(name), name);
	}

	bool GlobalVars::get_bool(const String &name) const
	{
		return convert_variant<bool>(get_value(name), name);
	}

	float GlobalVars::get_float(const String &name) const
	{
		return convert_variant<float>(get_value(name), name);
	}

	Sint32 GlobalVars::get_int(const String &name) const
	{
		return convert_variant<Sint32>(get_value(name), name);
	}

	glm::vec2 GlobalVars::get_vec2(const String &name) const
	{
		return convert_variant<glm::vec2>(get_value(name), name);
	}

	glm::vec3 GlobalVars::get_vec3(const String &name) const
	{
		return convert_variant<glm::vec3>(get_value(name), name);
	}

	glm::vec4 GlobalVars::get_vec4(const String &name) const
	{
		return convert_variant<glm::vec4>(get_value(name), name);
	}

	glm::ivec2 GlobalVars::get_ivec2(const String &name) const
	{
		return convert_variant<glm::ivec2>(get_value(name), name);
	}

	glm::ivec3 GlobalVars::get_ivec3(const String &name) const
	{
		return convert_variant<glm::ivec3>(get_value(name), name);
	}

	glm::ivec4 GlobalVars::get_ivec4(const String &name) const
	{
		return convert_variant<glm::ivec4>(get_value(name), name);
	}

	glm::bvec2 GlobalVars::get_bvec2(const String &name) const
	{
		return convert_variant<glm::bvec2>(get_value(name), name);
	}

	glm::bvec3 GlobalVars::get_bvec3(const String &name) const
	{
		return convert_variant<glm::bvec3>(get_value(name), name);
	}

	glm::bvec4 GlobalVars::get_bvec4(const String &name) const
	{
		return convert_variant<glm::bvec4>(get_value(name), name);
	}

	glm::mat2x2 GlobalVars::get_mat2x2(const String &name) const
	{
		return convert_variant<glm::mat2x2>(get_value(name), name);
	}

	glm::mat2x3 GlobalVars::get_mat2x3(const String &name) const
	{
		return convert_variant<glm::mat2x3>(get_value(name), name);
	}

	glm::mat2x4 GlobalVars::get_mat2x4(const String &name) const
	{
		return convert_variant<glm::mat2x4>(get_value(name), name);
	}

	glm::mat3x2 GlobalVars::get_mat3x2(const String &name) const
	{
		return convert_variant<glm::mat3x2>(get_value(name), name);
	}

	glm::mat3x3 GlobalVars::get_mat3x3(const String &name) const
	{
		return convert_variant<glm::mat3x3>(get_value(name), name);
	}

	glm::mat3x4 GlobalVars::get_mat3x4(const String &name) const
	{
		return convert_variant<glm::mat3x4>(get_value(name), name);
	}

	glm::mat4x2 GlobalVars::get_mat4x2(const String &name) const
	{
		return convert_variant<glm::mat4x2>(get_value(name), name);
	}

	glm::mat4x3 GlobalVars::get_mat4x3(const String &name) const
	{
		return convert_variant<glm::mat4x3>(get_value(name), name);
	}

	glm::mat4x4 GlobalVars::get_mat4x4(const String &name) const
	{
		return convert_variant<glm::mat4x4>(get_value(name), name);
	}

	BoolVector GlobalVars::get_bools(const String &name) const
	{
		return convert_variant<BoolVector>(get_value(name), name);
	}

	Sint32Vector GlobalVars::get_ints(const String &name) const
	{
		return convert_variant<Sint32Vector>(get_value(name), name);
	}

	FloatVector GlobalVars::get_floats(const String &name) const
	{
		return convert_variant<FloatVector>(get_value(name), name);
	}

	StringVector GlobalVars::get_strings(const String &name) const
	{
		return convert_variant<StringVector>(get_value(name), name);
	}

	Vec2Vector GlobalVars::get_vec2s(const String &name) const
	{
		return convert_variant<Vec2Vector>(get_value(name), name);
	}

	Vec3Vector GlobalVars::get_vec3s(const String &name) const
	{
		return convert_variant<Vec3Vector>(get_value(name), name);
	}

	Vec4Vector GlobalVars::get_vec4s(const String &name) const
	{
		return convert_variant<Vec4Vector>(get_value(name), name);
	}

	Ivec2Vector GlobalVars::get_ivec2s(const String &name) const
	{
		return convert_variant<Ivec2Vector>(get_value(name), name);
	}

	Ivec3Vector GlobalVars::get_ivec3s(const String &name) const
	{
		return convert_variant<Ivec3Vector>(get_value(name), name);
	}

	Ivec4Vector GlobalVars::get_ivec4s(const String &name) const
	{
		return convert_variant<Ivec4Vector>(get_value(name), name);
	}

	Mat2x2Vector GlobalVars::get_mat2x2s(const String &name) const
	{
		return convert_variant<Mat2x2Vector>(get_value(name), name);
	}

	Mat2x3Vector GlobalVars::get_mat2x3s(const String &name) const
	{
		return convert_variant<Mat2x3Vector>(get_value(name), name);
	}

	Mat2x4Vector GlobalVars::get_mat2x4s(const String &name) const
	{
		return convert_variant<Mat2x4Vector>(get_value(name), name);
	}

	Mat3x2Vector GlobalVars::get_mat3x2s(const String &name) const
	{
		return convert_variant<Mat3x2Vector>(get_value(name), name);
	}

	Mat3x3Vector GlobalVars::get_mat3x3s(const String &name) const
	{
		return convert_variant<Mat3x3Vector>(get_value(name), name);
	}

	Mat3x4Vector GlobalVars::get_mat3x4s(const String &name) const
	{
		return convert_variant<Mat3x4Vector>(get_value(name), name);
	}

	Mat4x2Vector GlobalVars::get_mat4x2s(const String &name) const
	{
		return convert_variant<Mat4x2Vector>(get_value(name), name);
	}

	Mat4x3Vector GlobalVars::get_mat4x3s(const String &name) const
	{
		return convert_variant<Mat4x3Vector>(get_value(name), name);
	}

	Mat4x4Vector GlobalVars::get_mat4x4s(const String &name) const
	{
		return convert_variant<Mat4x4Vector>(get_value(name), name);
	}

	void GlobalVars::set_value(const String &name, const Variant &value)
	{
		StringVariantMap::iterator found;

		found = m_vars.find(name);

		if (found == m_vars.end())
		{
			m_vars[name] = value;

			return;
		}
			
		if (found->second.type() != value.type())
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_message(L"Invalid type for global "
					"var")
				<< errinfo_name(name)
				<< errinfo_any_type_name(value.type().name())
				<< errinfo_type_name(
					found->second.type().name()));
		}

		found->second = value;
	}
#endif

	GlobalVars::GlobalVars()
	{
		m_quality = String(UTF8("high"));
		m_shadow_distance = 40.0f;
		m_view_distance = 40.0f;
		m_shadow_quality = sqt_no;
		m_shadow_map_size = 2;
		m_opengl_version = ovt_2_1;
		m_fog = true;
		m_optmize_shader_source = true;
		m_use_simd = false;
	}

	GlobalVars::~GlobalVars() throw()
	{
	}

}
