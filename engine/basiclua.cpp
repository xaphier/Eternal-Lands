/****************************************************************************
 *            basiclua.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "basiclua.hpp"
#include "exceptions.hpp"
#include "utf.hpp"

namespace eternal_lands
{

	namespace
	{

		StringType tostring(lua_State* lua, const Sint32 index)
		{
			if (lua_tostring(lua, index) != 0)
			{
				return utf8_to_string(lua_tostring(lua, index));
			}
			else
			{
				return L"";
			}
		}

		int throw_lua_exception(lua_State* lua)
		{
			EL_THROW_EXCEPTION(LuaException()
				<< errinfo_message(tostring(lua, -1)));
		}

		template <typename T>
		T load_matrix(BasicLua &lua)
		{
			T result;
			Uint32 i, j;

			if (!lua.is_table(-1))
			{
				EL_THROW_EXCEPTION(LuaVarTypeException()
					<< errinfo_message(L"Wrong type")
					<< errinfo_table_size(result.row_size()
						* result.col_size())
					<< errinfo_lua_type(lua.type(-1))
					<< errinfo_lua_type_name(
						lua.type_name(-1)));
			}

			lua.push_nil();

			i = 0;
			j = 0;

			while (lua.next(-2))
			{
				if ((i * result.col_size() + j) >=
					(result.row_size() * result.col_size()))
				{
					EL_THROW_EXCEPTION(
						LuaTableSizeException()
						<< errinfo_message(L"Too many"
							" elements")
						<< errinfo_table_index(i *
							result.col_size() + j)
						<< errinfo_table_size(
							result.row_size()
							* result.col_size()));
				}
				if (!lua.is_number(-1))
				{
					EL_THROW_EXCEPTION(LuaVarTypeException()
						<< errinfo_message(L"Wrong "
							"type, number needed")
						<< errinfo_table_index(i *
							result.col_size() + j)
						<< errinfo_lua_type(
							lua.type(-1))
						<< errinfo_lua_type_name(
							lua.type_name(-1)));
				}

				result[i][j] = lua.load_float();

				lua.pop(1);

				j++;

				if (j == result.col_size())
				{
					i++;
					j = 0;
				}
			}

			if ((i * result.col_size() + j) <
				(result.row_size() * result.col_size()))
			{
				EL_THROW_EXCEPTION(LuaTableSizeException()
					<< errinfo_message(L"Too few elements")
					<< errinfo_table_index(i *
						result.col_size() + j)
					<< errinfo_table_size(result.row_size()
						* result.col_size()));
			}

			return result;
		}

		template <typename T>
		std::vector<T> load_matrix_array(BasicLua &lua)
		{
			std::vector<T> result;

			if (!lua.is_table(-1))
			{
				EL_THROW_EXCEPTION(LuaVarTypeException()
					<< errinfo_message(L"Wrong type")
					<< errinfo_lua_type(lua.type(-1))
					<< errinfo_lua_type_name(
						lua.type_name(-1)));
			}

			lua.push_nil();

			while (lua.next(-2))
			{
				try
				{
					result.push_back(load_matrix<T>(lua));
				}
				catch (LuaException &exception)
				{
					exception << errinfo_array_index(
						result.size());
					throw;
				}

				lua.pop(1);
			}

			return result;
		}

		template <typename T>
		T load_vector(BasicLua &lua)
		{
			T result;
			Uint32 i;

			if (!lua.is_table(-1))
			{
				EL_THROW_EXCEPTION(LuaVarTypeException()
					<< errinfo_message(L"Wrong type")
					<< errinfo_table_size(result.length())
					<< errinfo_lua_type(lua.type(-1))
					<< errinfo_lua_type_name(
						lua.type_name(-1)));
			}

			lua.push_nil();

			i = 0;

			while (lua.next(-2))
			{
				if (i >= result.length())
				{
					EL_THROW_EXCEPTION(
						LuaTableSizeException()
						<< errinfo_message(L"Too many"
							" elements")
						<< errinfo_table_index(i)
						<< errinfo_table_size(
							result.length()));
				}
				if (!lua.is_number(-1))
				{
					EL_THROW_EXCEPTION(LuaVarTypeException()
						<< errinfo_message(L"Wrong "
							"type, number needed")
						<< errinfo_table_index(i)
						<< errinfo_lua_type(
							lua.type(-1))
						<< errinfo_lua_type_name(
							lua.type_name(-1)));
				}

				result[i] = lua.load_float();

				lua.pop(1);

				i++;
			}

			if (i < result.length())
			{
				EL_THROW_EXCEPTION(LuaTableSizeException()
					<< errinfo_message(L"Too few elements")
					<< errinfo_table_index(i)
					<< errinfo_table_size(result.length()));
			}

			return result;
		}

		template <typename T>
		std::vector<T> load_vector_array(BasicLua &lua)
		{
			std::vector<T> result;

			if (!lua.is_table(-1))
			{
				EL_THROW_EXCEPTION(LuaVarTypeException()
					<< errinfo_message(L"Wrong type")
					<< errinfo_lua_type(lua.type(-1))
					<< errinfo_lua_type_name(
						lua.type_name(-1)));
			}

			lua.push_nil();

			while (lua.next(-2))
			{
				try
				{
					result.push_back(load_vector<T>(lua));
				}
				catch (LuaException &exception)
				{
					exception << errinfo_array_index(
						result.size());
					throw;
				}

				lua.pop(1);
			}

			return result;
		}

		template <typename T>
		T load_bool_vector(BasicLua &lua)
		{
			T result;
			Uint32 i;

			if (!lua.is_table(-1))
			{
				EL_THROW_EXCEPTION(LuaVarTypeException()
					<< errinfo_message(L"Wrong type")
					<< errinfo_table_size(result.length())
					<< errinfo_lua_type(lua.type(-1))
					<< errinfo_lua_type_name(
						lua.type_name(-1)));
			}

			lua.push_nil();

			i = 0;

			while (lua.next(-2))
			{
				if (i >= result.length())
				{
					EL_THROW_EXCEPTION(
						LuaTableSizeException()
						<< errinfo_message(L"Too many"
							" elements")
						<< errinfo_table_index(i)
						<< errinfo_table_size(
							result.length()));
				}
				if (!lua.is_bool(-1))
				{
					EL_THROW_EXCEPTION(LuaVarTypeException()
						<< errinfo_message(L"Wrong "
							"type, bool needed")
						<< errinfo_table_index(i)
						<< errinfo_lua_type(
							lua.type(-1))
						<< errinfo_lua_type_name(
							lua.type_name(-1)));
				}

				result[i] = lua.load_bool();

				lua.pop(1);

				i++;
			}

			if (i < result.length())
			{
				EL_THROW_EXCEPTION(LuaTableSizeException()
					<< errinfo_message(L"Too few elements")
					<< errinfo_table_index(i)
					<< errinfo_table_size(result.length()));
			}

			return result;
		}

		template <typename T>
		void push_matrix(BasicLua &lua, const T &value)
		{
			Uint32 i, j, index;

			lua.create_table(value.col_size() * value.row_size(),
				0);

			index = 1;

			for (i = 0; i < value.row_size(); i++)
			{
				for (j = 0; j < value.col_size(); j++)
				{
					lua.push_int(index);		/* Push the table index */
					lua.push_float(value[i][j]);	/* Push the cell value */
					lua.raw_set(-3);		/* Stores the pair in the table */
					index++;
				}
			}
		}

		template <typename T>
		void push_matrix_array(BasicLua &lua, const std::vector<T> &values)
		{
			Uint32 i;

			lua.create_table(values.size(), 0);

			for (i = 0; i < values.size(); i++)
			{
				lua.push_int(i + 1);		/* Push the table index */
				push_matrix(lua, values[i]);	/* Push the cell value */
				lua.raw_set(-3);		/* Stores the pair in the table */
			}
		}

		template <typename T>
		void push_vector(BasicLua &lua, const T &value)
		{
			Uint32 i;

			lua.create_table(value.length(), 0);

			for (i = 0; i < value.length(); i++)
			{
				lua.push_int(i + 1);		/* Push the table index */
				lua.push_number(value[i]);	/* Push the cell value */
				lua.raw_set(-3);		/* Stores the pair in the table */
			}
		}

		template <typename T>
		void push_vector_array(BasicLua &lua, const std::vector<T> &values)
		{
			Uint32 i;

			lua.create_table(values.size(), 0);

			for (i = 0; i < values.size(); i++)
			{
				lua.push_int(i + 1);		/* Push the table index */
				push_vector(lua, values[i]);	/* Push the cell value */
				lua.raw_set(-3);		/* Stores the pair in the table */
			}
		}

		template <typename T>
		void push_bool_vector(BasicLua &lua, const T &value)
		{
			Uint32 i;

			lua.create_table(value.length(), 0);

			for (i = 0; i < value.length(); i++)
			{
				lua.push_int(i + 1);		/* Push the table index */
				lua.push_bool(value[i]);	/* Push the cell value */
				lua.raw_set(-3);		/* Stores the pair in the table */
			}
		}

		template <typename T>
		void push_int_vector(BasicLua &lua, const T &value)
		{
			Uint32 i;

			lua.create_table(value.length(), 0);

			for (i = 0; i < value.length(); i++)
			{
				lua.push_int(i + 1);	/* Push the table index */
				lua.push_int(value[i]);	/* Push the cell value */
				lua.raw_set(-3);	/* Stores the pair in the table */
			}
		}

		template <typename T>
		void push_int_vector_array(BasicLua &lua,
			const std::vector<T> &values)
		{
			Uint32 i;

			lua.create_table(values.size(), 0);

			for (i = 0; i < values.size(); i++)
			{
				lua.push_int(i + 1);			/* Push the table index */
				push_int_vector(lua, values[i]);	/* Push the cell value */
				lua.raw_set(-3);			/* Stores the pair in the table */
			}
		}

		class LuaPushVariant: public boost::static_visitor<>
		{
			private:
				BasicLua &m_lua;

			public:
				LuaPushVariant(BasicLua &lua): m_lua(lua)
				{
				}

				~LuaPushVariant() throw()
				{
				}

				void operator()(const bool value) const
				{
					m_lua.push_bool(value);
				}

				void operator()(const float value) const
				{
					m_lua.push_float(value);
				}

				void operator()(const Sint64 value) const
				{
					m_lua.push_int(value);
				}

				void operator()(const glm::vec2 &value) const
				{
					push_vector<glm::vec2>(m_lua, value);
				}

				void operator()(const glm::vec3 &value) const
				{
					push_vector<glm::vec3>(m_lua, value);
				}

				void operator()(const glm::vec4 &value) const
				{
					push_vector<glm::vec4>(m_lua, value);
				}

				void operator()(const glm::ivec2 &value) const
				{
					push_int_vector<glm::ivec2>(m_lua,
						value);
				}

				void operator()(const glm::ivec3 &value) const
				{
					push_int_vector<glm::ivec3>(m_lua,
						value);
				}

				void operator()(const glm::ivec4 &value) const
				{
					push_int_vector<glm::ivec4>(m_lua,
						value);
				}

				void operator()(const glm::uvec2 &value) const
				{
					push_int_vector<glm::uvec2>(m_lua,
						value);
				}

				void operator()(const glm::uvec3 &value) const
				{
					push_int_vector<glm::uvec3>(m_lua,
						value);
				}

				void operator()(const glm::uvec4 &value) const
				{
					push_int_vector<glm::uvec4>(m_lua,
						value);
				}

				void operator()(const glm::bvec2 &value) const
				{
					push_bool_vector<glm::bvec2>(m_lua,
						value);
				}

				void operator()(const glm::bvec3 &value) const
				{
					push_bool_vector<glm::bvec3>(m_lua,
						value);
				}

				void operator()(const glm::bvec4 &value) const
				{
					push_bool_vector<glm::bvec4>(m_lua,
						value);
				}

				void operator()(const glm::mat2x2 &value) const
				{
					push_matrix<glm::mat2x2>(m_lua, value);
				}

				void operator()(const glm::mat2x3 &value) const
				{
					push_matrix<glm::mat2x3>(m_lua, value);
				}

				void operator()(const glm::mat2x4 &value) const
				{
					push_matrix<glm::mat2x4>(m_lua, value);
				}

				void operator()(const glm::mat3x2 &value) const
				{
					push_matrix<glm::mat3x2>(m_lua, value);
				}

				void operator()(const glm::mat3x3 &value) const
				{
					push_matrix<glm::mat3x3>(m_lua, value);
				}

				void operator()(const glm::mat3x4 &value) const
				{
					push_matrix<glm::mat3x4>(m_lua, value);
				}

				void operator()(const glm::mat4x2 &value) const
				{
					push_matrix<glm::mat4x2>(m_lua, value);
				}

				void operator()(const glm::mat4x3 &value) const
				{
					push_matrix<glm::mat4x3>(m_lua, value);
				}

				void operator()(const glm::mat4x4 &value) const
				{
					push_matrix<glm::mat4x4>(m_lua, value);
				}

				void operator()(const Uvec4Vector &value) const
				{
					push_int_vector_array<glm::uvec4>(m_lua,
						value);
				}

				void operator()(const Ivec4Vector &value) const
				{
					push_int_vector_array<glm::ivec4>(m_lua,
						value);
				}

				void operator()(const Vec4Vector &value) const
				{
					push_vector_array<glm::vec4>(m_lua,
						value);
				}

				void operator()(const Mat2x4Vector &value) const
				{
					push_matrix_array<glm::mat2x4>(m_lua,
						value);
				}

				void operator()(const Mat3x4Vector &value) const
				{
					push_matrix_array<glm::mat3x4>(m_lua,
						value);
				}

				void operator()(const Mat4x4Vector &value) const
				{
					push_matrix_array<glm::mat4x4>(m_lua,
						value);
				}

		};

		class LuaLoadVariant: public boost::static_visitor<>
		{
			private:
				BasicLua &m_lua;

			public:
				LuaLoadVariant(BasicLua &lua): m_lua(lua)
				{
				}

				~LuaLoadVariant() throw()
				{
				}

				void operator()(bool &value) const
				{
					value = m_lua.load_bool();
				}

				void operator()(float &value) const
				{
					value = m_lua.load_float();
				}

				void operator()(Sint64 &value) const
				{
					value = m_lua.load_int();
				}

				void operator()(glm::vec2 &value) const
				{
					value = m_lua.load_vec2();
				}

				void operator()(glm::vec3 &value) const
				{
					value = m_lua.load_vec3();
				}

				void operator()(glm::vec4 &value) const
				{
					value = m_lua.load_vec4();
				}

				void operator()(glm::ivec2 &value) const
				{
					value = m_lua.load_ivec2();
				}

				void operator()(glm::ivec3 &value) const
				{
					value = m_lua.load_ivec3();
				}

				void operator()(glm::ivec4 &value) const
				{
					value = m_lua.load_ivec4();
				}

				void operator()(glm::uvec2 &value) const
				{
					value = m_lua.load_uvec2();
				}

				void operator()(glm::uvec3 &value) const
				{
					value = m_lua.load_uvec3();
				}

				void operator()(glm::uvec4 &value) const
				{
					value = m_lua.load_uvec4();
				}

				void operator()(glm::bvec2 &value) const
				{
					value = m_lua.load_bvec2();
				}

				void operator()(glm::bvec3 &value) const
				{
					value = m_lua.load_bvec3();
				}

				void operator()(glm::bvec4 &value) const
				{
					value = m_lua.load_bvec4();
				}

				void operator()(glm::mat2x2 &value) const
				{
					value = m_lua.load_mat2x2();
				}

				void operator()(glm::mat2x3 &value) const
				{
					value = m_lua.load_mat2x3();
				}

				void operator()(glm::mat2x4 &value) const
				{
					value = m_lua.load_mat2x4();
				}

				void operator()(glm::mat3x2 &value) const
				{
					value = m_lua.load_mat3x2();
				}

				void operator()(glm::mat3x3 &value) const
				{
					value = m_lua.load_mat3x3();
				}

				void operator()(glm::mat3x4 &value) const
				{
					value = m_lua.load_mat3x4();
				}

				void operator()(glm::mat4x2 &value) const
				{
					value = m_lua.load_mat4x2();
				}

				void operator()(glm::mat4x3 &value) const
				{
					value = m_lua.load_mat4x3();
				}

				void operator()(glm::mat4x4 &value) const
				{
					value = m_lua.load_mat4x4();
				}

				void operator()(Ivec4Vector &value) const
				{
					value = m_lua.load_ivec4s();
				}

				void operator()(Uvec4Vector &value) const
				{
					value = m_lua.load_uvec4s();
				}

				void operator()(Vec4Vector &value) const
				{
					value = m_lua.load_vec4s();
				}

				void operator()(Mat2x4Vector &value) const
				{
					value = m_lua.load_mat2x4s();
				}

				void operator()(Mat3x4Vector &value) const
				{
					value = m_lua.load_mat3x4s();
				}

				void operator()(Mat4x4Vector &value) const
				{
					value = m_lua.load_mat4x4s();
				}

		};

	}

	BasicLua::BasicLua(lua_State* lua): m_lua(lua)
	{
		if (m_lua == 0)
		{
			EL_THROW_EXCEPTION(LuaException()
				<< errinfo_message(L"lua state is null"));
		}

		lua_atpanic(m_lua, &throw_lua_exception);
	}

	BasicLua::~BasicLua() throw()
	{
	}

	void BasicLua::close() throw()
	{
		if (m_lua != 0)
		{
			clear_stack();

			lua_close(m_lua);

			m_lua = 0;
		}
	}

	void BasicLua::raw_set(const Sint32 index)
	{
		assert(m_lua != 0);
		lua_rawset(m_lua, index);
	}

	void BasicLua::clear_stack()
	{
		assert(m_lua != 0);
		lua_settop(m_lua, 0);
	}

	void BasicLua::get_global(const String &name)
	{
		assert(m_lua != 0);
		lua_getglobal(m_lua, string_to_utf8(name).c_str());
	}

	void BasicLua::get_global(const std::string &name)
	{
		assert(m_lua != 0);
		lua_getglobal(m_lua, name.c_str());
	}

	void BasicLua::get_table(const Sint32 index)
	{
		assert(m_lua != 0);
		lua_gettable(m_lua, index);
	}

	Sint32 BasicLua::type(const Sint32 index)
	{
		assert(m_lua != 0);
		return lua_type(m_lua, index);
	}

	String BasicLua::type_name(const Sint32 index)
	{
		switch (type(index))
		{
			case LUA_TNONE:
				return String(L"none");
			case LUA_TNIL:
				return String(L"nil");
			case LUA_TBOOLEAN:
				return String(L"boolean");
			case LUA_TLIGHTUSERDATA:
				return String(L"light_user_data");
			case LUA_TNUMBER:
				return String(L"number");
			case LUA_TSTRING:
				return String(L"string");
			case LUA_TTABLE:
				return String(L"table");
			case LUA_TFUNCTION:
				return String(L"function");
			case LUA_TUSERDATA:
				return String(L"user_data");
			case LUA_TTHREAD:
				return String(L"thread");
			default:
				return String(L"unknown");
		}			
	}

	bool BasicLua::is_table(const Sint32 index)
	{
		assert(m_lua != 0);
		return lua_istable(m_lua, index);
	}

	bool BasicLua::is_bool(const Sint32 index)
	{
		assert(m_lua != 0);
		return lua_isboolean(m_lua, index);
	}

	bool BasicLua::is_number(const Sint32 index)
	{
		assert(m_lua != 0);
		return lua_isnumber(m_lua, index);
	}

	bool BasicLua::is_string(const Sint32 index)
	{
		assert(m_lua != 0);
		return lua_isstring(m_lua, index);
	}

	bool BasicLua::is_nil(const Sint32 index)
	{
		assert(m_lua != 0);
		return lua_isnil(m_lua, index);
	}

	void BasicLua::push_bool(const bool value)
	{
		assert(m_lua != 0);
		lua_pushboolean(m_lua, value);
	}

	void BasicLua::push_float(const float value)
	{
		assert(m_lua != 0);
		lua_pushnumber(m_lua, value);
	}

	void BasicLua::push_int(const Sint32 value)
	{
		assert(m_lua != 0);
		lua_pushinteger(m_lua, value);
	}

	void BasicLua::push_number(const double value)
	{
		assert(m_lua != 0);
		lua_pushnumber(m_lua, value);
	}

	void BasicLua::push_string(const String &value)
	{
		assert(m_lua != 0);
		lua_pushstring(m_lua, string_to_utf8(value).c_str());
	}

	void BasicLua::push_string(const std::string &value)
	{
		assert(m_lua != 0);
		lua_pushstring(m_lua, value.c_str());
	}

	void BasicLua::push_nil()
	{
		assert(m_lua != 0);
		lua_pushnil(m_lua);
	}

	void BasicLua::create_table(const Sint32 narr, const Sint32 nrec)
	{
		assert(m_lua != 0);
		lua_createtable(m_lua, narr, nrec);
	}

	String BasicLua::to_string(const Sint32 index)
	{
		assert(m_lua != 0);

		return String(tostring(m_lua, index));
	}

	bool BasicLua::to_bool(const Sint32 index)
	{
		assert(m_lua != 0);

		return lua_toboolean(m_lua, index);
	}

	float BasicLua::to_float(const Sint32 index)
	{
		assert(m_lua != 0);

		return lua_tonumber(m_lua, index);
	}

	Sint64 BasicLua::to_int(const Sint32 index)
	{
		assert(m_lua != 0);

		return lua_tointeger(m_lua, index);
	}

	String BasicLua::load_string()
	{
		assert(m_lua != 0);

		return String(tostring(m_lua, -1));
	}

	bool BasicLua::load_bool()
	{
		assert(m_lua != 0);

		return lua_toboolean(m_lua, -1);
	}

	float BasicLua::load_float()
	{
		assert(m_lua != 0);

		return lua_tonumber(m_lua, -1);
	}

	Sint64 BasicLua::load_int()
	{
		assert(m_lua != 0);

		return lua_tointeger(m_lua, -1);
	}

	glm::vec2 BasicLua::load_vec2()
	{
		return load_vector<glm::vec2>(*this);
	}

	glm::vec3 BasicLua::load_vec3()
	{
		return load_vector<glm::vec3>(*this);
	}

	glm::vec4 BasicLua::load_vec4()
	{
		return load_vector<glm::vec4>(*this);
	}

	glm::ivec2 BasicLua::load_ivec2()
	{
		return load_vector<glm::ivec2>(*this);
	}

	glm::ivec3 BasicLua::load_ivec3()
	{
		return load_vector<glm::ivec3>(*this);
	}

	glm::ivec4 BasicLua::load_ivec4()
	{
		return load_vector<glm::ivec4>(*this);
	}

	glm::uvec2 BasicLua::load_uvec2()
	{
		return load_vector<glm::uvec2>(*this);
	}

	glm::uvec3 BasicLua::load_uvec3()
	{
		return load_vector<glm::uvec3>(*this);
	}

	glm::uvec4 BasicLua::load_uvec4()
	{
		return load_vector<glm::uvec4>(*this);
	}

	glm::bvec2 BasicLua::load_bvec2()
	{
		return load_bool_vector<glm::bvec2>(*this);
	}

	glm::bvec3 BasicLua::load_bvec3()
	{
		return load_bool_vector<glm::bvec3>(*this);
	}

	glm::bvec4 BasicLua::load_bvec4()
	{
		return load_bool_vector<glm::bvec4>(*this);
	}

	glm::mat2x2 BasicLua::load_mat2x2()
	{
		return load_matrix<glm::mat2x2>(*this);
	}

	glm::mat2x3 BasicLua::load_mat2x3()
	{
		return load_matrix<glm::mat2x3>(*this);
	}

	glm::mat2x4 BasicLua::load_mat2x4()
	{
		return load_matrix<glm::mat2x4>(*this);
	}

	glm::mat3x2 BasicLua::load_mat3x2()
	{
		return load_matrix<glm::mat3x2>(*this);
	}

	glm::mat3x3 BasicLua::load_mat3x3()
	{
		return load_matrix<glm::mat3x3>(*this);
	}

	glm::mat3x4 BasicLua::load_mat3x4()
	{
		return load_matrix<glm::mat3x4>(*this);
	}

	glm::mat4x2 BasicLua::load_mat4x2()
	{
		return load_matrix<glm::mat4x2>(*this);
	}

	glm::mat4x3 BasicLua::load_mat4x3()
	{
		return load_matrix<glm::mat4x3>(*this);
	}

	glm::mat4x4 BasicLua::load_mat4x4()
	{
		return load_matrix<glm::mat4x4>(*this);
	}

	BoolVector BasicLua::load_bools()
	{
		BoolVector result;

		if (!is_table(-1))
		{
			EL_THROW_EXCEPTION(LuaVarTypeException()
				<< errinfo_message(L"Wrong type, bool array "
					"needed")
				<< errinfo_lua_type(type(-1))
				<< errinfo_lua_type_name(type_name(-1)));
		}

		push_nil();

		while (next(-2))
		{
			if (!is_bool(-1))
			{
				EL_THROW_EXCEPTION(LuaVarTypeException()
					<< errinfo_message(L"Wrong type, bool "
						"needed")
					<< errinfo_array_index(result.size())
					<< errinfo_lua_type(type(-1))
					<< errinfo_lua_type_name(
						type_name(-1)));

			}

			result.push_back(load_bool());

			pop(1);
		}

		return result;
	}

	Sint32Vector BasicLua::load_ints()
	{
		Sint32Vector result;

		if (!is_table(-1))
		{
			EL_THROW_EXCEPTION(LuaVarTypeException()
				<< errinfo_message(L"Wrong type, int array "
					"needed")
				<< errinfo_lua_type(type(-1))
				<< errinfo_lua_type_name(type_name(-1)));
		}

		push_nil();

		while (next(-2))
		{
			if (!is_number(-1))
			{
				EL_THROW_EXCEPTION(LuaVarTypeException()
					<< errinfo_message(L"Wrong type, "
						"number needed")
					<< errinfo_array_index(result.size())
					<< errinfo_lua_type(type(-1))
					<< errinfo_lua_type_name(
						type_name(-1)));
			}

			result.push_back(load_int());

			pop(1);
		}

		return result;
	}

	Uint32Vector BasicLua::load_uints()
	{
		Uint32Vector result;

		if (!is_table(-1))
		{
			EL_THROW_EXCEPTION(LuaVarTypeException()
				<< errinfo_message(L"Wrong type, int array "
					"needed")
				<< errinfo_lua_type(type(-1))
				<< errinfo_lua_type_name(type_name(-1)));
		}

		push_nil();

		while (next(-2))
		{
			if (!is_number(-1))
			{
				EL_THROW_EXCEPTION(LuaVarTypeException()
					<< errinfo_message(L"Wrong type, "
						"number needed")
					<< errinfo_array_index(result.size())
					<< errinfo_lua_type(type(-1))
					<< errinfo_lua_type_name(
						type_name(-1)));
			}

			result.push_back(load_int());

			pop(1);
		}

		return result;
	}

	Sint32Vector BasicLua::load_bools_as_ints()
	{
		Sint32Vector result;

		if (!is_table(-1))
		{
			EL_THROW_EXCEPTION(LuaVarTypeException()
				<< errinfo_message(L"Wrong type, bool array "
					"needed")
				<< errinfo_lua_type(type(-1))
				<< errinfo_lua_type_name(type_name(-1)));
		}

		push_nil();

		while (next(-2))
		{
			if (!is_bool(-1))
			{
				EL_THROW_EXCEPTION(LuaVarTypeException()
					<< errinfo_message(L"Wrong type, bool"
						" needed")
					<< errinfo_array_index(result.size())
					<< errinfo_lua_type(type(-1))
					<< errinfo_lua_type_name(
						type_name(-1)));

			}

			result.push_back(load_bool());

			pop(1);
		}

		return result;
	}

	FloatVector BasicLua::load_floats()
	{
		FloatVector result;

		if (!is_table(-1))
		{
			EL_THROW_EXCEPTION(LuaVarTypeException()
				<< errinfo_message(L"Wrong type, float array "
					"needed")
				<< errinfo_lua_type(type(-1))
				<< errinfo_lua_type_name(type_name(-1)));
		}

		push_nil();

		while (next(-2))
		{
			if (!is_number(-1))
			{
				EL_THROW_EXCEPTION(LuaVarTypeException()
					<< errinfo_message(L"Wrong type, "
						"number needed")
					<< errinfo_array_index(result.size())
					<< errinfo_lua_type(type(-1))
					<< errinfo_lua_type_name(
						type_name(-1)));

			}

			result.push_back(load_float());

			pop(1);
		}

		return result;
	}

	StringVector BasicLua::load_strings()
	{
		StringVector result;

		if (!is_table(-1))
		{
			EL_THROW_EXCEPTION(LuaVarTypeException()
				<< errinfo_message(L"Wrong type, string array"
					" needed")
				<< errinfo_lua_type(type(-1))
				<< errinfo_lua_type_name(type_name(-1)));
		}

		push_nil();

		while (next(-2))
		{
			if (!is_string(-1))
			{
				EL_THROW_EXCEPTION(LuaVarTypeException()
					<< errinfo_message(L"Wrong type, "
						"string needed")
					<< errinfo_array_index(result.size())
					<< errinfo_lua_type(type(-1))
					<< errinfo_lua_type_name(
						type_name(-1)));

			}

			result.push_back(load_string());

			pop(1);
		}

		return result;
	}

	StringSet BasicLua::load_string_set()
	{
		StringSet result;

		if (!is_table(-1))
		{
			EL_THROW_EXCEPTION(LuaVarTypeException()
				<< errinfo_message(L"Wrong type, string array"
					" needed")
				<< errinfo_lua_type(type(-1))
				<< errinfo_lua_type_name(type_name(-1)));
		}

		push_nil();

		while (next(-2))
		{
			if (!is_string(-1))
			{
				EL_THROW_EXCEPTION(LuaVarTypeException()
					<< errinfo_message(L"Wrong type, "
						"string needed")
					<< errinfo_array_index(result.size())
					<< errinfo_lua_type(type(-1))
					<< errinfo_lua_type_name(
						type_name(-1)));

			}

			result.insert(load_string());

			pop(1);
		}

		return result;
	}

	Vec2Vector BasicLua::load_vec2s()
	{
		return load_vector_array<glm::vec2>(*this);
	}

	Vec3Vector BasicLua::load_vec3s()
	{
		return load_vector_array<glm::vec3>(*this);
	}

	Vec4Vector BasicLua::load_vec4s()
	{
		return load_vector_array<glm::vec4>(*this);
	}

	Ivec2Vector BasicLua::load_ivec2s()
	{
		return load_vector_array<glm::ivec2>(*this);
	}

	Ivec3Vector BasicLua::load_ivec3s()
	{
		return load_vector_array<glm::ivec3>(*this);
	}

	Ivec4Vector BasicLua::load_ivec4s()
	{
		return load_vector_array<glm::ivec4>(*this);
	}

	Uvec2Vector BasicLua::load_uvec2s()
	{
		return load_vector_array<glm::uvec2>(*this);
	}

	Uvec3Vector BasicLua::load_uvec3s()
	{
		return load_vector_array<glm::uvec3>(*this);
	}

	Uvec4Vector BasicLua::load_uvec4s()
	{
		return load_vector_array<glm::uvec4>(*this);
	}

	Mat2x2Vector BasicLua::load_mat2x2s()
	{
		return load_matrix_array<glm::mat2x2>(*this);
	}

	Mat2x3Vector BasicLua::load_mat2x3s()
	{
		return load_matrix_array<glm::mat2x3>(*this);
	}

	Mat2x4Vector BasicLua::load_mat2x4s()
	{
		return load_matrix_array<glm::mat2x4>(*this);
	}

	Mat3x2Vector BasicLua::load_mat3x2s()
	{
		return load_matrix_array<glm::mat3x2>(*this);
	}

	Mat3x3Vector BasicLua::load_mat3x3s()
	{
		return load_matrix_array<glm::mat3x3>(*this);
	}

	Mat3x4Vector BasicLua::load_mat3x4s()
	{
		return load_matrix_array<glm::mat3x4>(*this);
	}

	Mat4x2Vector BasicLua::load_mat4x2s()
	{
		return load_matrix_array<glm::mat4x2>(*this);
	}

	Mat4x3Vector BasicLua::load_mat4x3s()
	{
		return load_matrix_array<glm::mat4x3>(*this);
	}

	Mat4x4Vector BasicLua::load_mat4x4s()
	{
		return load_matrix_array<glm::mat4x4>(*this);
	}

	void BasicLua::push_bools(const BoolVector &value)
	{
		Uint32 i;

		create_table(value.size(), 0);

		for (i = 0; i < value.size(); i++)
		{
			push_bool(value[i]);
		}
	}

	void BasicLua::push_floats(const FloatVector &value)
	{
		Uint32 i;

		create_table(value.size(), 0);

		for (i = 0; i < value.size(); i++)
		{
			push_float(value[i]);
		}
	}

	void BasicLua::push_ints(const Sint32Vector &value)
	{
		Uint32 i;

		create_table(value.size(), 0);

		for (i = 0; i < value.size(); i++)
		{
			push_int(value[i]);
		}
	}

	void BasicLua::push_strings(const StringVector &value)
	{
		Uint32 i;

		create_table(value.size(), 0);

		for (i = 0; i < value.size(); i++)
		{
			push_string(value[i]);
		}
	}

	bool BasicLua::next(const Sint32 index)
	{
		assert(m_lua != 0);

		return lua_next(m_lua, index);
	}

	void BasicLua::pop(const Uint32 count)
	{
		assert(m_lua != 0);
		lua_pop(m_lua, count);
	}

	void BasicLua::do_string(const String &str, const String &file_name)
	{
		assert(m_lua != 0);

		m_file_name = file_name;

		if (luaL_dostring(m_lua, string_to_utf8(str).c_str()) != 0)
		{
			EL_THROW_EXCEPTION(LuaException()
				<< errinfo_message(load_string())
				<< errinfo_string_value(str)
				<< boost::errinfo_file_name(
					string_to_utf8(file_name)));
		}
	}

	void BasicLua::push_variant(const Variant &value)
	{
		boost::apply_visitor(LuaPushVariant(*this), value);
	}

	void BasicLua::load_variant(Variant &value)
	{
		boost::apply_visitor(LuaLoadVariant(*this), value);
	}

}
