/****************************************************************************
 *            engine.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "engine.h"
#include "reflection.h"
#include "errors.h"
#include "gl_init.h"
#include "interface.h"
#include "multiplayer.h"
#include "sky.h"
#include "textures.h"
#include "cursors.h"
#include "init.h"
#include "sky.h"
#include "shadows.h"
#include "elc_private.h"
#include "asc.h"
#include "colors.h"
#include "io/elpathwrapper.h"
#include <sstream>
#include "engine/scene.hpp"
#include "engine/actor.hpp"
#include "engine/sceneresources.hpp"
#include "engine/instancesbuilder.hpp"
#include "engine/objectdata.hpp"
#include "engine/instancedata.hpp"
#include "engine/abstractmesh.hpp"
#include "engine/materialdescription.hpp"
#include "engine/actordatacache.hpp"
#include "engine/actortexturebuilder.hpp"
#include "engine/freeids.hpp"
#include "engine/effectcache.hpp"
#include "engine/lua.hpp"
#include "engine/globalvars.hpp"
#include "engine/exceptions.hpp"
#include "engine/shader/shadersourcebuilder.hpp"
#include "engine/utf.hpp"
#include "engine/logging.hpp"
#include "engine/filesystem.hpp"
#include "engine/filter.hpp"
#include "engine/font/text.hpp"
#include "engine/font/textattribute.hpp"
#include "engine/simd/simd.hpp"

namespace el = eternal_lands;

namespace
{
	// equivalent to calling flyweight<std::string>::init()
	el::String::initializer fwinit;
	boost::scoped_ptr<el::Scene> scene;
	boost::scoped_ptr<el::InstancesBuilder> instances_builder;
	Sint32 pick_frame = 0;
	el::SelectionType selection = el::st_none;
	el::StringSet harvestables;
	el::StringSet entrables;
	el::FreeIds free_ids;
	boost::scoped_ptr<el::Lua> lua;
	el::GlobalVarsSharedPtr global_vars;
	el::FileSystemSharedPtr file_system;

	el::Text get_text(const unsigned char* str, const char* font,
		const glm::vec4 &start_color,
		const Uint32 len = std::numeric_limits<Uint32>::max())
	{
		el::Text text;
		el::TextAttribute attribute;
		glm::vec4 color;
		el::Utf32String utf32_str;
		Uint32 i, count, index;

		count = strlen(reinterpret_cast<const char*>(str));
		count = std::min(count, len);

		attribute.set_size(12.0f);
		attribute.set_font(el::String(font));

		color = start_color;

		for (i = 0; i < count; ++i)
		{
			if (str[i] < 127)
			{
				utf32_str += L'\0' + str[i];

				continue;
			}

			if (is_color(str[i]))
			{
				index = from_color_char(str[i]);

				color.r = colors_list[index].r1 / 255.0f;
				color.g = colors_list[index].g1 / 255.0f;
				color.b = colors_list[index].b1 / 255.0f;

				if (utf32_str.length() > 0)
				{
					text.add(utf32_str, attribute);
				}

				attribute.set_color(color);
				utf32_str = L"";
			}
		}

		if (utf32_str.length() > 0)
		{
			text.add(utf32_str, attribute);
		}

		return text;
	}

	void disable_opengl2_stuff()
	{
		Uint16 i;

		try
		{
			DEBUG_CHECK_GL_ERROR();

			glActiveTexture(GL_TEXTURE0);
			glEnable(GL_TEXTURE_2D);
			glDisable(GL_ALPHA_TEST);
			glDisable(GL_CULL_FACE);
			glDisable(GL_BLEND);
			bind_texture_id(0);

			DEBUG_CHECK_GL_ERROR();

			glBindBufferARB(GL_ARRAY_BUFFER, 0);
			glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, 0);
			glBindBufferARB(GL_PIXEL_PACK_BUFFER, 0);
			glBindBufferARB(GL_PIXEL_UNPACK_BUFFER, 0);
			glUseProgram(0);

			DEBUG_CHECK_GL_ERROR();

			for (i = 0; i < 16; i++)
			{
				glDisableVertexAttribArray(i);
			}

			DEBUG_CHECK_GL_ERROR();

			glBlendColor(1.0f, 1.0f, 1.0f, 1.0f);
			glBlendFunc(GL_ZERO, GL_ONE);

			DEBUG_CHECK_GL_ERROR();
		}
		catch (const boost::exception &exception)
		{
			LOG_EXCEPTION(exception);
		}
		catch (const std::exception &exception)
		{
			LOG_EXCEPTION(exception);
		}
	}

	el::SelectionType get_selection_type(el::String name)
	{
		if (harvestables.count(name) > 0)
		{
			return el::st_harvest;
		}

		if (entrables.count(name) > 0)
		{
			return el::st_enter;
		}

		return el::st_select;
	}

	el::SelectionType get_selection_type(el::String name,
		const SelectionType selection)
	{
		switch (selection)
		{
			case st_none:
				return el::st_none;
			case st_select:
				return el::st_select;
			case st_harvest:
				return el::st_harvest;
			case st_pick:
				return el::st_pick;
			case st_enter:
				return el::st_enter;
			case st_detect:
				return get_selection_type(name);
		}

		return el::st_none;
	}

	el::String get_name(const char* name)
	{
		el::StringType str;
		std::size_t pos;

		if (name == 0)
		{
			return el::String();
		}

		str = el::utf8_to_string(name);

		pos = str.find(UTF8("./"));

		while (pos != std::string::npos)
		{
			str.erase(pos, 2);
			pos = str.find(UTF8("./"));
		}

		return el::String(str);
	}

	el::ObjectData get_object_data(const glm::vec3 &pos,
		const glm::vec3 &rot, const glm::vec4 &color, el::String name,
		const bool transparent, const Uint32 id,
		const SelectionType selection)
	{
		glm::mat4 matrix;
		float transparency;
		el::SelectionType el_selection;

		assert(glm::all(glm::lessThanEqual(glm::abs(pos),
			glm::vec3(1e7f))));

		matrix = glm::rotate(rot.z, glm::vec3(0.0f, 0.0f, 1.0f));
		matrix = glm::rotate(matrix, rot.x,
			glm::vec3(1.0f, 0.0f, 0.0f));
		matrix = glm::rotate(matrix, rot.y,
			glm::vec3(0.0f, 1.0f, 0.0f));
		matrix[3] = glm::vec4(pos, 1.0f);

		el_selection = get_selection_type(name, selection);

		if (transparent)
		{
			transparency = 0.7f;
		}
		else
		{
			transparency = 1.0f;
		}

		return el::ObjectData(glm::mat4x3(matrix), color, name,
			transparency, id, el_selection, transparent);
	}

	int el_rebuild_shader(lua_State *L)
	{
		Sint32 n;

		n = lua_gettop(L);  // Number of arguments

		if (n != 0)
		{
			return luaL_error(L,
				UTF8("Got %d arguments expected 0"), n); 
		}

		scene->get_scene_resources().get_effect_cache()->reload();

		return 0;
	}

	int el_reload_shader(lua_State *L)
	{
		Sint32 n;

		n = lua_gettop(L);  // Number of arguments

		if (n != 0)
		{
			return luaL_error(L,
				UTF8("Got %d arguments expected 0"), n); 
		}

		return 0;
	}

	int el_lua_print(lua_State *L)
	{
		std::string tmp;
		const char *str;
		Sint32 i, n;

		n = lua_gettop(L);  /* number of arguments */

		lua_getglobal(L, "tostring");

		for (i = 1; i <= n; i++)
		{
			lua_pushvalue(L, -1);  /* function to be called */
			lua_pushvalue(L, i);   /* value to print */
			lua_call(L, 1, 1);
			str = lua_tostring(L, -1);  /* get result */

			if (str == 0)
			{
				return luaL_error(L, LUA_QL("tostring")
					" must return a string to "
					LUA_QL("print"));
			}

			if (i > 1)
			{
				tmp += "\t";
			}

			tmp += str;

			lua_pop(L, 1);  /* pop result */
		}

		LOG_TO_CONSOLE(c_grey1, tmp.c_str());

		return 0;
	}

	int el_lua_to_string(lua_State *l)
	{
		el::BasicLua lua(l);

		luaL_checkany(lua.get(), 1);

		if (luaL_callmeta(lua.get(), 1, "__tostring"))  /* is there a metafield? */
		{
			return 1;  /* use its value */
		}
	
		switch (lua.type(1))
		{
			case LUA_TNONE:
				lua.push_string("none");
				break;
			case LUA_TNIL:
				lua.push_string("nil");
				break;
			case LUA_TNUMBER:
				lua.push_string(lua.to_string(1));
				break;
			case LUA_TSTRING:
				lua_pushvalue(lua.get(), 1);
				break;
			case LUA_TBOOLEAN:
				lua.push_string(lua.to_bool(1) ?
					"true" : "false");
				break;
			case LUA_TTABLE:
				lua.push_string("table");
				break;
			default:
				lua_pushfstring(lua.get(), "%s: %p",
					luaL_typename(lua.get(), 1),
					lua_topointer(lua.get(), 1));
				break;
		}

		return 1;
	}

	const luaL_Reg el_lua_functions[] = {
		{ "tostring", el_lua_to_string },
		{ "print", el_lua_print },
		{ "rebuild_shader", el_rebuild_shader },
		{ "reload_shader", el_reload_shader },
		{ 0, 0 }
	};

}

#define	USE_GL_DEBUG_OUTPUT

#ifdef	USE_GL_DEBUG_OUTPUT
extern "C" void ARB_debug_output_to_file(GLenum source, GLenum type, GLuint id,
	GLenum severity, GLsizei length, const GLchar* message,
	GLvoid* userParam)
{
	std::string source_str, type_str, severity_str;
	FILE* file;

	file = fopen("debug.txt", "a");

	if (file)
	{
		switch (source)
		{
			case GL_DEBUG_SOURCE_API_ARB:
				source_str = "OpenGL";
				break;
			case GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB:
				source_str = "Windows";
				break;
			case GL_DEBUG_SOURCE_SHADER_COMPILER_ARB:
				source_str = "Shader Compiler";
				break;
			case GL_DEBUG_SOURCE_THIRD_PARTY_ARB:
				source_str = "Third Party";
				break;
			case GL_DEBUG_SOURCE_APPLICATION_ARB:
				source_str = "Application";
				break;
			case GL_DEBUG_SOURCE_OTHER_ARB:
				source_str = "Other";
				break;
			default:
				source_str = "Unkown";
				break;
		}

		switch (type)
		{
			case GL_DEBUG_TYPE_ERROR_ARB:
				type_str = "Error";
				break;
			case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB:
				type_str = "Deprecated behavior";
				break;
			case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB:
				type_str = "Undefined behavior";
				break;
			case GL_DEBUG_TYPE_PORTABILITY_ARB:
				type_str = "Portability";
				break;
			case GL_DEBUG_TYPE_PERFORMANCE_ARB:
				type_str = "Performance";
				break;
			case GL_DEBUG_TYPE_OTHER_ARB:
				type_str = "Other";
				break;
			default:
				type_str = "Unkown";
				break;
		}
 
		switch (severity)
		{
			case GL_DEBUG_SEVERITY_HIGH_ARB:
				severity_str = "High";
				break;
			case GL_DEBUG_SEVERITY_MEDIUM_ARB:
				severity_str = "Medium";
				break;
			case GL_DEBUG_SEVERITY_LOW_ARB:
				severity_str = "Low";
				break;
			default:
				severity_str = "Unkown";
				break;
		}

		fprintf(file,
			"Source:%s\tType:%s\tID:%d\tSeverity:%s\tMessage:%s\n",
			source_str.c_str(), type_str.c_str(), id,
			severity_str.c_str(), message);

		fclose(file);
	}
}
#endif
#if	1
#define	TRY_BLOCK	\
	try	\
	{

#define CATCH_BLOCK	\
	}	\
	catch (const boost::exception &exception)	\
	{	\
		LOG_EXCEPTION(exception);	\
	}	\
	catch (const std::exception &exception)	\
	{	\
		LOG_EXCEPTION(exception);	\
	}
#else
#define	TRY_BLOCK
#define CATCH_BLOCK
#endif

extern "C" void load_harvestable_list()
{
	std::vector<el::StringType> lines, line;
	std::vector<el::StringType>::iterator it;
	el::StringType str;
	el::String harvestables_str;

	harvestables_str = file_system->get_file_string(
		el::String(UTF8("harvestable.lst")));

	boost::split(lines, harvestables_str.get(), boost::is_any_of(
		UTF8("\n")), boost::token_compress_on);

	it = lines.begin();

	while (it != lines.end())
	{
		boost::algorithm::trim(*it);

		if (!it->empty())
		{
			str = UTF8("3dobjects/");
			str += *it;
			harvestables.insert(el::String(str));
		}

		it++;
	}
}

extern "C" void load_entrable_list()
{
	std::vector<el::StringType> lines, line;
	std::vector<el::StringType>::iterator it;
	el::StringType str;
	el::String entrable_str;

	entrable_str = file_system->get_file_string(
		el::String(UTF8("entrable.lst")));

	boost::split(lines, entrable_str.get(), boost::is_any_of(
		UTF8("\n")), boost::token_compress_on);

	it = lines.begin();

	while (it != lines.end())
	{
		boost::algorithm::trim(*it);

		if (!it->empty())
		{
			str = UTF8("3dobjects");
			str += *it;
			entrables.insert(el::String(str));
		}

		it++;
	}
}

extern "C" void init_global_vars()
{
	TRY_BLOCK

	global_vars.reset(new el::GlobalVars());

	CATCH_BLOCK
}

extern "C" void init_file_system()
{
	TRY_BLOCK

	file_system.reset(new el::FileSystem());
	file_system->add_dirs(
		el::String(el::string_to_utf8(get_path_config_base())),
		VER_MAJOR, VER_MINOR, VER_RELEASE);

	CATCH_BLOCK
}

extern "C" void file_system_add_dir(const char* dir)
{
	TRY_BLOCK

	file_system->add_dir(el::String(el::utf8_to_string(dir)));

	CATCH_BLOCK

	try
	{
//		file_system->add_zip(el::String(el::utf8_to_string(dir) +
//			UTF8("shaders.zip")));
	}
	catch (...)
	{
	}
}

#ifdef	EL_TIME_FRAME_DEBUG
Uint32 do_el_time = 0;
GLuint el_timer_id = 0;
#endif
extern "C" void init_engine()
{
	TRY_BLOCK

	CHECK_GL_ERROR();

	lua.reset(new el::Lua());

	/* set global EL */
	lua_pushvalue(lua->get(), LUA_GLOBALSINDEX);
	lua_setglobal(lua->get(), "EL");
	/* open lib into global table */
	luaL_register(lua->get(), "EL", el_lua_functions);  
	lua_pushvalue(lua->get(), -1);
	lua_setfield(lua->get(), -2, "__index");

	scene.reset(new el::Scene(global_vars, file_system));
	scene->init(file_system);

	CHECK_GL_ERROR();
#ifdef	USE_GL_DEBUG_OUTPUT
	if (GLEW_ARB_debug_output)
	{
		glDebugMessageCallbackARB((GLDEBUGPROCARB)&ARB_debug_output_to_file, 0);
	}
#endif
	CHECK_GL_ERROR();
#ifdef	EL_TIME_FRAME_DEBUG
	glGenQueries(1, &el_timer_id);
#endif
	CHECK_GL_ERROR();

	CATCH_BLOCK
}

extern "C" void engine_load_map(const char* name, const float r, const float g,
	const float b, const int dungeon)
{
	TRY_BLOCK

	CHECK_GL_ERROR();

	free_ids.clear();

	scene->load(el::String(el::utf8_to_string(name)),
		glm::vec3(r, g, b), dungeon != 0);

	instances_builder.reset(new el::InstancesBuilder(
		scene->get_scene_resources().get_mesh_data_cache(),
		8.0f, global_vars->get_use_simd(),
		global_vars->get_opengl_3_2()));

	CHECK_GL_ERROR();

	CATCH_BLOCK
}

extern "C" void exit_engine()
{
	TRY_BLOCK

	CHECK_GL_ERROR();

	scene.reset();
	instances_builder.reset();
	lua.reset();
	global_vars.reset();

	CHECK_GL_ERROR();

#ifdef	EL_TIME_FRAME_DEBUG
	glDeleteQueries(1, &el_timer_id);
#endif
	CHECK_GL_ERROR();

	CATCH_BLOCK
}

extern "C" void exit_global_vars()
{
	TRY_BLOCK

	global_vars.reset();

	CATCH_BLOCK
}

extern "C" void exit_file_system()
{
	TRY_BLOCK

	file_system.reset();

	CATCH_BLOCK
}

extern "C" void engine_cull_scene()
{
	glm::vec3 focus, main_light_ambient, main_light_color;
	glm::vec3 main_light_direction;
	actor* me;
	Uint32 i;

	TRY_BLOCK

	CHECK_GL_ERROR();

	me = get_our_actor ();

	if (me != 0)
	{
		focus.x = me->x_pos + 0.25f;
		focus.y = me->y_pos + 0.25f;
		focus.z = -2.2f + height_map[me->y_tile_pos*tile_map_size_x*6+me->x_tile_pos]*0.2f;

		scene->set_focus(focus);
	}

	for (i = 0; i < 3; i++)
	{
		main_light_ambient[i] = skybox_light_ambient_color[i];
		main_light_color[i] = skybox_light_diffuse_color[i];
		main_light_direction[i] = sun_position[i];
	}

	if (main_light_direction.z < 0.0f)
	{
		main_light_direction = -main_light_direction;
	}

	scene->set_main_light_ambient(main_light_ambient);
	scene->set_main_light_color(main_light_color);
	scene->set_main_light_direction(main_light_direction);
	scene->set_night(!is_day);

	scene->cull();

	CHECK_GL_ERROR();

	CATCH_BLOCK
}

extern "C" void engine_draw_scene()
{
	Uint32 id;

	TRY_BLOCK

	CHECK_GL_ERROR();

	glEnable(GL_CULL_FACE);
	glDisable(GL_ALPHA_TEST);

	scene->draw();

	pick_frame++;

	if (pick_frame > mouse_limit)
	{
		thing_under_the_mouse = UNDER_MOUSE_NOTHING;
		actor_under_mouse = 0;
		object_under_mouse = -1;

		id = scene->pick(glm::vec2(mouse_x, window_height - mouse_y),
			glm::vec2(5.0f), selection);

		if (selection == el::st_npc)
		{
			thing_under_the_mouse = UNDER_MOUSE_NPC;
			object_under_mouse = id;
			actor_under_mouse = get_actor_ptr_from_id(object_under_mouse);
		}

		if (selection == el::st_player)
		{
			thing_under_the_mouse = UNDER_MOUSE_PLAYER;
			object_under_mouse = id;
			actor_under_mouse = get_actor_ptr_from_id(object_under_mouse);
		}

		if (selection == el::st_animal)
		{
			thing_under_the_mouse = UNDER_MOUSE_ANIMAL;
			object_under_mouse = id;
			actor_under_mouse = get_actor_ptr_from_id(object_under_mouse);
		}

		if ((selection == el::st_select) ||
			(selection == el::st_harvest) ||
			(selection == el::st_pick) ||
			(selection == el::st_enter))
		{
			thing_under_the_mouse = UNDER_MOUSE_3D_OBJ;
			object_under_mouse = id;
			actor_under_mouse = 0;
		}

		pick_frame = 0;
	}

	CHECK_GL_ERROR();

	CATCH_BLOCK

	disable_opengl2_stuff();
}

extern "C" void engine_clear()
{
	TRY_BLOCK

	CHECK_GL_ERROR();

	scene->clear();
	free_ids.clear();

	CHECK_GL_ERROR();

	CATCH_BLOCK

	disable_opengl2_stuff();
}

extern "C" void engine_add_tile(const Uint16 x, const Uint16 y, const Uint8 tile)
{
	el::MaterialDescriptionVector materials;
	el::MaterialDescription material;
	el::StringStream str;
	el::String file_name;
	glm::mat4 matrix;
	glm::vec3 offset;

	TRY_BLOCK

	DEBUG_CHECK_GL_ERROR();

	if (IS_WATER_TILE(tile))
	{
		offset = glm::vec3(glm::vec2(x, y) * 3.0f, -0.2501f);
	}
	else
	{
		offset = glm::vec3(glm::vec2(x, y) * 3.0f, -0.0011f);
	}

	assert(glm::all(glm::lessThanEqual(glm::abs(offset), glm::vec3(1e7f))));

	matrix[0][0] = 3.0f;
	matrix[1][1] = 3.0f;
	matrix[2][2] = 1.0f;
	matrix[3] = glm::vec4(offset, 1.0f);

	material.set_texture(el::String(UTF8("textures/gray.dds")),
		el::stt_normal_0);
	material.set_texture(el::String(UTF8("textures/white.dds")),
		el::stt_specular_0);

	if ((tile != 0) && (tile != 240))
	{
		str << UTF8("3dobjects/tile");
		str << static_cast<Uint16>(tile);

		material.set_texture(el::String(str.str() + UTF8(".dds")),
			el::stt_diffuse_0);
		material.set_effect(el::String(UTF8("mesh_solid")));

		file_name = el::String(str.str() + UTF8(".xml"));

		if (file_system->get_file_readable(file_name))
		{
			material.load_xml(file_system, file_name);
		}
	}
	else
	{
		if (tile == 240)
		{
			material.set_texture(el::String(UTF8(
				"textures/lava.dds")), el::stt_diffuse_0);
			material.set_texture(el::String(UTF8(
				"textures/noise.dds")), el::stt_diffuse_1);
			material.set_effect(el::String(UTF8("lava")));
		}
		else
		{
			material.set_texture(el::String(UTF8(
				"3dobjects/tile0.dds")), el::stt_diffuse_0);
			material.set_texture(el::String(UTF8(
				"textures/water_normal.dds")),
					el::stt_normal_0);
			material.set_effect(el::String(UTF8("mesh_solid")));
		}
	}

	materials.push_back(material);

	instances_builder->add(el::ObjectData(glm::mat4x3(matrix),
		glm::vec4(0.0f), el::String(UTF8("plane_4")), 0.0f,
		free_ids.get_next_free_id(), el::st_none, false), materials);

	DEBUG_CHECK_GL_ERROR();

	CATCH_BLOCK

	disable_opengl2_stuff();
}

extern "C" void engine_add_instanced_object(const char* name, const float x_pos,
	const float y_pos, const float z_pos, const float x_rot,
	const float y_rot, const float z_rot, const char blended, const float r,
	const float g, const float b, const Uint32 id,
	const SelectionType selection)
{
	TRY_BLOCK

	DEBUG_CHECK_GL_ERROR();

	if (blended != 1)
	{
		instances_builder->add(get_object_data(
			glm::vec3(x_pos, y_pos, z_pos),
			glm::vec3(x_rot, y_rot, z_rot),
			glm::vec4(r, g, b, 1.0f), get_name(name), false, id,
			selection));
	}
	else
	{
		scene->add_object(get_object_data(
			glm::vec3(x_pos, y_pos, z_pos),
			glm::vec3(x_rot, y_rot, z_rot),
			glm::vec4(r, g, b, 1.0f), get_name(name), true, id,
			selection));
	}

	DEBUG_CHECK_GL_ERROR();

	CATCH_BLOCK

	disable_opengl2_stuff();
}

extern "C" void engine_add_object(const char* name, const float x_pos,
	const float y_pos, const float z_pos, const float x_rot,
	const float y_rot, const float z_rot, const char blended, const float r,
	const float g, const float b, const Uint32 id,
	const SelectionType selection)
{
	TRY_BLOCK

	DEBUG_CHECK_GL_ERROR();

	if (blended != 1)
	{
		scene->add_object(get_object_data(
			glm::vec3(x_pos, y_pos, z_pos),
			glm::vec3(x_rot, y_rot, z_rot),
			glm::vec4(r, g, b, 1.0f), get_name(name), false, id,
			selection));
	}
	else
	{
		scene->add_object(get_object_data(
			glm::vec3(x_pos, y_pos, z_pos),
			glm::vec3(x_rot, y_rot, z_rot),
			glm::vec4(r, g, b, 1.0f), get_name(name), true, id,
			selection));
	}

	DEBUG_CHECK_GL_ERROR();

	CATCH_BLOCK

	disable_opengl2_stuff();
}

extern "C" void engine_done_object_adding()
{
	el::InstanceDataVector instances;
	el::ObjectDataVector uninstanced;

	TRY_BLOCK

	DEBUG_CHECK_GL_ERROR();

	instances_builder->build(free_ids, instances, uninstanced);

	BOOST_FOREACH(const el::InstanceData &instance_data, instances)
	{
		DEBUG_CHECK_GL_ERROR();

		scene->add_object(instance_data);
	}

	BOOST_FOREACH(const el::ObjectData &object_data, uninstanced)
	{
		DEBUG_CHECK_GL_ERROR();

		scene->add_object(object_data);
	}

	DEBUG_CHECK_GL_ERROR();

	CATCH_BLOCK

	disable_opengl2_stuff();
}

extern "C" void engine_add_light(const float x_pos, const float y_pos,
	const float z_pos, const float r, const float g, const float b,
	const float radius, const Uint32 id)
{
	glm::vec3 position, color;
	float scale;

	TRY_BLOCK

	color = glm::vec3(r, g, b);
	position = glm::vec3(x_pos, y_pos, z_pos);

	assert(glm::all(glm::lessThanEqual(glm::abs(position),
		glm::vec3(1e7f))));

	scale = std::max(std::max(r, g), std::max(b, 1.0f));
	color /= scale;

	scene->add_light(position, color, 0.1f, radius * scale, id);

	CATCH_BLOCK
}

extern "C" void engine_build_buffers(actor_types* a)
{
	TRY_BLOCK

	DEBUG_CHECK_GL_ERROR();

	scene->get_scene_resources().get_actor_data_cache()->add_actor(
		a->actor_type, a->coremodel, 
		el::String(el::string_to_utf8(a->actor_name)),
		el::String(el::string_to_utf8(a->skin_name)),
		el::String(el::string_to_utf8(a->file_name)), a->actor_scale,
		a->scale, a->mesh_scale, a->skel_scale, a->ghost);

	DEBUG_CHECK_GL_ERROR();

	CATCH_BLOCK

	disable_opengl2_stuff();
}

extern "C" void engine_set_transformation_buffers(actor* actor)
{
	glm::mat4 matrix;
	glm::vec3 offset, attachment_shift;

	TRY_BLOCK

	offset = glm::vec3(actor->x_pos, actor->y_pos, actor->z_pos);

	assert(glm::all(glm::lessThanEqual(glm::abs(offset), glm::vec3(1e7f))));

	if (offset.z == 0.0f)
	{
		//actor is walking, as opposed to flying, get the height underneath
		offset.z = -2.2f + height_map[actor->y_tile_pos * tile_map_size_x * 6
			+ actor->x_tile_pos] * 0.2f;
	}

	offset.x += 0.25f;
	offset.y += 0.25f;

	if (actor->attached_actor >= 0)
	{
		attachment_shift.x = actor->attachment_shift[0];
		attachment_shift.y = actor->attachment_shift[1];
		attachment_shift.z = actor->attachment_shift[2];
	}

	matrix = glm::translate(offset);
	matrix = glm::rotate(matrix, 180.0f - actor->z_rot, glm::vec3(0.0f, 0.0f, 1.0f));
	matrix = glm::rotate(matrix, actor->x_rot, glm::vec3(1.0f, 0.0f, 0.0f));
	matrix = glm::rotate(matrix, actor->y_rot, glm::vec3(0.0f, 1.0f, 0.0f));
	matrix = glm::translate(matrix, attachment_shift);
	matrix = glm::scale(matrix, glm::vec3(actors_defs[actor->actor_type].actor_scale));
	matrix = glm::scale(matrix, glm::vec3(actor->scale));

	reinterpret_cast<el::Actor*>(actor->calmodel->getUserData(
		))->update_bones();
	reinterpret_cast<el::Actor*>(actor->calmodel->getUserData(
		))->set_world_matrix(glm::mat4x3(matrix));

	CATCH_BLOCK
}

extern "C" void engine_build_actor_bounding_box(actor* a)
{
	CalSkeleton* cs;
	Uint32 i;
	float t;

	TRY_BLOCK

	if (a->calmodel)
	{
		cs = a->calmodel->getSkeleton();
		cs->getBoneBoundingBox(a->bbox.bbmin, a->bbox.bbmax);

		for (i = 0; i < 3; i++)
		{
			t = a->bbox.bbmax[i] - a->bbox.bbmin[i];
			a->bbox.bbmin[i] -= std::max(t * 0.25f - 0.25f, 0.1f);
			a->bbox.bbmax[i] += std::max(t * 0.25f - 0.25f, 0.1f);
		}
	}

	reinterpret_cast<el::Actor*>(a->calmodel->getUserData(
		))->update_skeleton_bounding_box(0.5f);

	CATCH_BLOCK
}

static Uint64 client_ids = 0;

extern "C" CalModel *model_new(const Uint32 type_id, const Uint32 id,
	const char* name, const Uint32 kind_of_actor,
	const Uint32 enhanced_actor, Uint32* client_id)
{
	CalModel* result;
	el::ActorSharedPtr actor;
	el::SelectionType selection;
	std::string source, dest;
	Uint32 i, count;

	result = 0;

	TRY_BLOCK

	DEBUG_CHECK_GL_ERROR();

	if (kind_of_actor == NPC)
	{
		selection = el::st_npc;
	}
	else
	{
		if ((kind_of_actor == HUMAN) ||
			(kind_of_actor == COMPUTER_CONTROLLED_HUMAN) ||
			(enhanced_actor &&
			((kind_of_actor == PKABLE_HUMAN) ||
			(kind_of_actor == PKABLE_COMPUTER_CONTROLLED))))
		{
			selection = el::st_player;
		}
		else
		{
			selection = el::st_animal;
		}
	}

	if (name != 0)
	{
		source = name;
	}

	count = source.length();

	for (i = 0; i < count; i++)
	{
		if (static_cast<Uint16>(source[i]) < 128)
		{
			dest += source[i];
		}
	}

	*client_id = client_ids++;

	actor = scene->add_actor(type_id, id, *client_id,
		el::String(el::utf8_to_string(dest)), selection,
		enhanced_actor != 0);

	result = actor->get_model();

	result->setUserData(actor.get());

	DEBUG_CHECK_GL_ERROR();

	CATCH_BLOCK

	disable_opengl2_stuff();

	return result;
}

extern "C" void model_delete(const Uint32 client_id)
{
	scene->remove_actor(client_id);
}

extern "C" void model_attach_mesh(actor *act, int mesh_id)
{
	if (mesh_id == -1)
	{
		return;
	}

	TRY_BLOCK

	el::Actor* actor;

	DEBUG_CHECK_GL_ERROR();

	assert(act);
	assert(act->calmodel);
	assert(act->calmodel->getUserData());
	act->calmodel->attachMesh(mesh_id);

	actor = reinterpret_cast<el::Actor*>(act->calmodel->getUserData());

	actor->add_mesh(mesh_id);

	DEBUG_CHECK_GL_ERROR();

	CATCH_BLOCK

	disable_opengl2_stuff();
}

extern "C" void model_detach_mesh(actor *act, int mesh_id)
{
	if (mesh_id == -1)
	{
		return;
	}

	TRY_BLOCK

	DEBUG_CHECK_GL_ERROR();

	el::Actor* actor;

	assert(act);
	assert(act->calmodel);
	assert(act->calmodel->getUserData());
	act->calmodel->detachMesh(mesh_id);

	actor = reinterpret_cast<el::Actor*>(act->calmodel->getUserData());

	actor->remove_mesh(mesh_id);

	DEBUG_CHECK_GL_ERROR();

	CATCH_BLOCK

	disable_opengl2_stuff();
}

extern "C" void engine_load_enhanced_actor_texture(actor *act)
{
	TRY_BLOCK

	el::Actor* actor;
	el::ActorPartTextureTypeStringMap parts;

	assert(act);
	assert(act->calmodel);
	assert(act->calmodel->getUserData());

	DEBUG_CHECK_GL_ERROR();

	actor = reinterpret_cast<el::Actor*>(act->calmodel->getUserData());

	assert(act->is_enhanced_model);
	assert(act->body_parts);

	parts[el::aptt_pants_tex] =
		el::utf8_to_string(act->body_parts->pants_tex);
	parts[el::aptt_pants_mask] =
		el::utf8_to_string(act->body_parts->pants_mask);

	parts[el::aptt_boots_tex] =
		el::utf8_to_string(act->body_parts->boots_tex);
	parts[el::aptt_boots_mask] =
		el::utf8_to_string(act->body_parts->boots_mask);

	parts[el::aptt_torso_tex] =
		el::utf8_to_string(act->body_parts->torso_tex);
	parts[el::aptt_arms_tex] =
		el::utf8_to_string(act->body_parts->arms_tex);
	parts[el::aptt_torso_mask] =
		el::utf8_to_string(act->body_parts->torso_mask);
	parts[el::aptt_arms_mask] =
		el::utf8_to_string(act->body_parts->arms_mask);

	parts[el::aptt_hands_tex] =
		el::utf8_to_string(act->body_parts->hands_tex);
	parts[el::aptt_head_tex] =
		el::utf8_to_string(act->body_parts->head_tex);
	parts[el::aptt_hands_mask] =
		el::utf8_to_string(act->body_parts->hands_mask);
	parts[el::aptt_head_mask] =
		el::utf8_to_string(act->body_parts->head_mask);

	parts[el::aptt_head_base] =
		el::utf8_to_string(act->body_parts->head_base);
	parts[el::aptt_body_base] =
		el::utf8_to_string(act->body_parts->body_base);
	parts[el::aptt_arms_base] =
		el::utf8_to_string(act->body_parts->arms_base);
	parts[el::aptt_legs_base] =
		el::utf8_to_string(act->body_parts->legs_base);
	parts[el::aptt_boots_base] =
		el::utf8_to_string(act->body_parts->boots_base);

	parts[el::aptt_hair_tex] =
		el::utf8_to_string(act->body_parts->hair_tex);
	parts[el::aptt_weapon_tex] =
		el::utf8_to_string(act->body_parts->weapon_tex);
	parts[el::aptt_shield_tex] =
		el::utf8_to_string(act->body_parts->shield_tex);
	parts[el::aptt_helmet_tex] =
		el::utf8_to_string(act->body_parts->helmet_tex);
	parts[el::aptt_neck_tex] =
		el::utf8_to_string(act->body_parts->neck_tex);
	parts[el::aptt_cape_tex] =
		el::utf8_to_string(act->body_parts->cape_tex);
	parts[el::aptt_hands_tex_save] =
		el::utf8_to_string(act->body_parts->hands_tex_save);

	DEBUG_CHECK_GL_ERROR();

	actor->set_parts(parts);

	DEBUG_CHECK_GL_ERROR();

	CATCH_BLOCK

	disable_opengl2_stuff();
}

extern "C" void engine_update_actor_buffs(actor *act, Uint32 buffs)
{
	if (act == 0)
	{
		return;
	}

	TRY_BLOCK

	DEBUG_CHECK_GL_ERROR();

	el::Actor* actor;

	assert(act);
	assert(act->calmodel);
	assert(act->calmodel->getUserData());

	actor = reinterpret_cast<el::Actor*>(act->calmodel->getUserData());

	actor->set_buffs(buffs);

	DEBUG_CHECK_GL_ERROR();

	CATCH_BLOCK
}

extern "C" Uint32 get_actor_texture_loaded(actor *act)
{
	return 1;
}

extern "C" Uint32 engine_get_object_under_mouse_pickable()
{
	if (selection == el::st_pick)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

extern "C" Uint32 engine_get_object_under_mouse_harvestable()
{
	if (selection == el::st_harvest)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

extern "C" Uint32 engine_get_object_under_mouse_entrable()
{
	if (selection == el::st_enter)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

extern "C" void engine_remove_object(const Uint32 id)
{
	free_ids.free_id(id);
	scene->remove_object(id);
}

extern "C" void engine_remove_light(const Uint32 id)
{
	scene->remove_light(id);
}

extern "C" void engine_get_object_position(const Uint32 id, float* x, float* y,
	float* z)
{
	glm::vec3 position;

	scene->get_object_position(id, position);

	*x = position.x;
	*y = position.y;
	*z = position.z;
}

extern "C" Uint32 engine_get_next_free_id()
{
	return free_ids.get_next_free_id();
}

extern "C" void engine_set_next_free_id(const Uint32 id)
{
	TRY_BLOCK

	free_ids.set_next_free_id(id);

	CATCH_BLOCK
}

extern "C" void engine_set_shader_quality(const char* quality)
{
	if (quality == 0)
	{
		return;
	}

	TRY_BLOCK
#if	0
	el::SceneResources::get_effect_cache().set_effect_quality(
		el::String(quality));
#endif
	CATCH_BLOCK
}

extern "C" void engine_resize_root_window(const float fov, const float aspect,
	const float z_near, const float z_far)
{
	TRY_BLOCK

	if (scene.get() != 0)
	{
		scene->set_perspective(fov, aspect, z_near, z_far);
	}

	CATCH_BLOCK
}

extern "C" int command_lua(char *text, int len)
{
	if ((text == 0) || (len <= 0))
	{
		return 0;
	}

	try
	{
		lua->do_string(el::String(el::utf8_to_string(std::string(text,
			len))), el::String(UTF8("command")));
	}
	catch (const el::LuaException &exception)
	{
		LOG_TO_CONSOLE(c_red1,
			boost::get_error_info<el::errinfo_message>(
				exception)->c_str());
	}
	catch (const boost::exception &exception)
	{
		LOG_EXCEPTION(exception);
	}
	catch (const std::exception &exception)	
	{
		LOG_EXCEPTION(exception);
	}

	return 1;
}

extern "C" void engine_set_shadow_map_size(const int value)
{
	global_vars->set_shadow_map_size(value);

	if (scene.get() != 0)
	{
		scene->shadow_map_change();
	}
}

extern "C" void engine_set_shadow_distance(const float value)
{
	global_vars->set_shadow_distance(value);

	if (scene.get() != 0)
	{
		scene->get_scene_resources().get_effect_cache()->reload();
	}
}

extern "C" void engine_set_view_distance(const float value)
{
	global_vars->set_view_distance(value);

	if (scene.get() != 0)
	{
		scene->get_scene_resources().get_effect_cache()->reload();
	}
}

extern "C" void engine_set_shadow_quality(const int value)
{
	el::ShadowQualityType shadow_quality;

	if (value <= el::sqt_no)
	{
		shadow_quality = el::sqt_no;
	}
	else
	{
		if (value >= el::sqt_ultra)
		{
			shadow_quality = el::sqt_ultra;
		}
		else
		{
			shadow_quality = static_cast<el::ShadowQualityType>(
				value);
		}
	}

	global_vars->set_shadow_quality(shadow_quality);

	if (scene.get() != 0)
	{
		if (global_vars->get_exponential_shadow_maps())
		{
			scene->get_scene_resources().get_shader_source_builder(
				)->set_shadow_map_type(el::String(UTF8("esm")));
		}
		else
		{
			scene->get_scene_resources().get_shader_source_builder(
				)->set_shadow_map_type(el::String(
					UTF8("default")));
		}

		scene->get_scene_resources().get_effect_cache()->reload();
		scene->shadow_map_change();
	}
}

extern "C" void engine_set_fog(const int value)
{
	global_vars->set_fog(value != 0);

	if (scene.get() != 0)
	{
		scene->get_scene_resources().get_effect_cache()->reload();
	}
}

extern "C" void engine_set_optmize_shader_source(const int value)
{
	global_vars->set_optmize_shader_source(value != 0);

	if (scene.get() != 0)
	{
		scene->get_scene_resources().get_effect_cache()->reload();
	}
}

extern "C" void engine_set_opengl_version(const int value)
{
	el::OpenglVerionType version;

	if (value <= el::ovt_2_1)
	{
		version = el::ovt_2_1;
	}
	else
	{
		if (value >= el::ovt_3_3)
		{
			version = el::ovt_3_3;
		}
		else
		{
			version = static_cast<el::OpenglVerionType>(value);
		}
	}

	global_vars->set_opengl_version(version);
}

extern "C" void engine_set_use_simd(const int value)
{
	global_vars->set_fog(value != 0 && el::SIMD::get_supported());
}

extern "C" void engine_set_shadow_map_filter(const int value)
{
	el::FilterType shadow_map_filter;

	if (value <= el::ft_gauss_5_tap)
	{
		shadow_map_filter = el::ft_gauss_5_tap;
	}
	else
	{
		if (value >= el::ft_box_13_tap)
		{
			shadow_map_filter = el::ft_box_13_tap;
		}
		else
		{
			shadow_map_filter = static_cast<el::FilterType>(value);
		}
	}

	if (scene.get() != 0)
	{
		scene->set_shadow_map_filter(shadow_map_filter);
	}
}

extern "C" int engine_get_opengl_3_0()
{
	return global_vars->get_opengl_3_0();
}	

extern "C" int engine_get_opengl_3_1()
{
	return global_vars->get_opengl_3_1();
}	

extern "C" int engine_get_opengl_3_2()
{
	return global_vars->get_opengl_3_2();
}	

extern "C" int engine_get_opengl_3_3()
{
	return global_vars->get_opengl_3_3();
}	

extern "C" void engine_set_fog_data(const float* color, const float density)
{
	if (scene.get() != 0)
	{
		scene->set_fog(glm::vec3(color[0], color[1], color[2]),
			density);
	}
}

extern "C" void engine_set_window_size(const Uint32 width, const Uint32 height,
	const Uint32 hud_x, const Uint32 hud_y)
{
	TRY_BLOCK

	glm::uvec4 view_port;
	glm::uvec2 window_size;

	view_port.x = 0;
	view_port.y = hud_y;
	view_port.z = width - hud_x;
	view_port.w = height - hud_y;

	window_size.x = width;
	window_size.y = height;

	if (scene.get() != 0)
	{
		scene->set_view_port(view_port);
		scene->set_window_size(window_size);
	}

	CATCH_BLOCK
}

#ifdef	DEBUG
extern "C" void engine_set_draw_objects(const int value)
{
	global_vars->set_draw_objects(value != 0);
}

extern "C" void engine_set_draw_actors(const int value)
{
	global_vars->set_draw_actors(value != 0);
}
#endif	/* DEBUG */
