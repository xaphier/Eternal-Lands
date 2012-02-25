/****************************************************************************
 *            basiclua.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_e5e6db47_7a7e_418b_b6e2_4d1dbbb8a6ec
#define	UUID_e5e6db47_7a7e_418b_b6e2_4d1dbbb8a6ec

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class Lua.
 * This file contains the @c class Lua.
 */
namespace eternal_lands
{

	class BasicLua: public boost::noncopyable
	{
		private:
			lua_State* m_lua;
			String m_file_name;

		protected:
			void close() throw();

		public:
			BasicLua(lua_State* lua);
			~BasicLua() throw();
			void raw_set(const Sint32 index);
			void clear_stack();
			void get_global(const String &name);
			void get_global(const std::string &name);
			void get_table(const Sint32 index);
			Sint32 type(const Sint32 index);
			String type_name(const Sint32 index);
			bool is_table(const Sint32 index);
			bool is_bool(const Sint32 index);
			bool is_number(const Sint32 index);
			bool is_string(const Sint32 index);
			bool is_nil(const Sint32 index);
			void push_bool(const bool value);
			void push_float(const float value);
			void push_int(const Sint32 value);
			void push_number(const double value);
			void push_string(const String &value);
			void push_string(const std::string &value);
			void push_nil();
			void push_bools(const BoolVector &value);
			void push_floats(const FloatVector &value);
			void push_ints(const Sint32Vector &value);
			void push_strings(const StringVector &value);
			void push_variant(const Variant &value);
			void create_table(const Sint32 narr, const Sint32 nrec);
			String to_string(const Sint32 index);
			bool to_bool(const Sint32 index);
			float to_float(const Sint32 index);
			Sint64 to_int(const Sint32 index);
			String load_string();
			bool load_bool();
			float load_float();
			Sint64 load_int();
			glm::vec2 load_vec2();
			glm::vec3 load_vec3();
			glm::vec4 load_vec4();
			glm::ivec2 load_ivec2();
			glm::ivec3 load_ivec3();
			glm::ivec4 load_ivec4();
			glm::uvec2 load_uvec2();
			glm::uvec3 load_uvec3();
			glm::uvec4 load_uvec4();
			glm::bvec2 load_bvec2();
			glm::bvec3 load_bvec3();
			glm::bvec4 load_bvec4();
			glm::mat2x2 load_mat2x2();
			glm::mat2x3 load_mat2x3();
			glm::mat2x4 load_mat2x4();
			glm::mat3x2 load_mat3x2();
			glm::mat3x3 load_mat3x3();
			glm::mat3x4 load_mat3x4();
			glm::mat4x2 load_mat4x2();
			glm::mat4x3 load_mat4x3();
			glm::mat4x4 load_mat4x4();
			BoolVector load_bools();
			Uint32Vector load_uints();
			Sint32Vector load_ints();
			Sint32Vector load_bools_as_ints();
			FloatVector load_floats();
			StringVector load_strings();
			StringSet load_string_set();
			Vec2Vector load_vec2s();
			Vec3Vector load_vec3s();
			Vec4Vector load_vec4s();
			Ivec2Vector load_ivec2s();
			Ivec3Vector load_ivec3s();
			Ivec4Vector load_ivec4s();
			Uvec2Vector load_uvec2s();
			Uvec3Vector load_uvec3s();
			Uvec4Vector load_uvec4s();
			Mat2x2Vector load_mat2x2s();
			Mat2x3Vector load_mat2x3s();
			Mat2x4Vector load_mat2x4s();
			Mat3x2Vector load_mat3x2s();
			Mat3x3Vector load_mat3x3s();
			Mat3x4Vector load_mat3x4s();
			Mat4x2Vector load_mat4x2s();
			Mat4x3Vector load_mat4x3s();
			Mat4x4Vector load_mat4x4s();
			void load_variant(Variant &value);
			bool next(const Sint32 index);
			void pop(const Uint32 count);
			void do_string(const String &str,
				const String &file_name);

			inline const String &get_file_name() const
			{
				return m_file_name;
			}

			inline lua_State* get()
			{
				return m_lua;
			}

	};

}

#endif	/* UUID_e5e6db47_7a7e_418b_b6e2_4d1dbbb8a6ec */

