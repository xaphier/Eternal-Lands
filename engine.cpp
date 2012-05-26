/****************************************************************************
 *            engine.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
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
#include "particles.h"
#include "io/elpathwrapper.h"
#include "map.h"
#include <sstream>
#include "engine/scene.hpp"
#include "engine/actor.hpp"
#include "engine/sceneresources.hpp"
#include "engine/instancesbuilder.hpp"
#include "engine/objectdata.hpp"
#include "engine/instancedata.hpp"
#include "engine/abstractmesh.hpp"
#include "engine/materialdescription.hpp"
#include "engine/materialdescriptioncache.hpp"
#include "engine/actordatacache.hpp"
#include "engine/actortexturebuilder.hpp"
#include "engine/freeids.hpp"
#include "engine/effectcache.hpp"
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
#include "engine/lightdata.hpp"
#include "engine/freeidsmanager.hpp"
#include "engine/particledata.hpp"
#include "engine/script/scriptengine.hpp"
#include "engine/script/materialscriptmanager.hpp"
#include "engine/script/imagescript.hpp"

#include "engine/image.hpp"
#include "engine/codec/codecmanager.hpp"

using namespace eternal_lands;

namespace
{
	// equivalent to calling flyweight<std::string>::init()
	String::initializer fwinit;
	boost::scoped_ptr<Scene> scene;
	boost::scoped_ptr<InstancesBuilder> instances_builder;
	Sint32 pick_frame = 0;
	eternal_lands::SelectionType selection = eternal_lands::st_none;
	StringSet harvestables;
	StringSet entrables;
	GlobalVarsSharedPtr global_vars;
	FileSystemSharedPtr file_system;
	boost::shared_ptr<ScriptEngine> script_engine;
	Uint16 effect_debug = 0;

	String get_string(const char* str,
		const Uint32 len = std::numeric_limits<Uint32>::max())
	{
		std::string result;
		Uint32 i;

		if (str == 0)
		{
			return String("(null)");
		}

		i = 0;

		while (i < len)
		{
			if (str[i] == 0)
			{
				break;
			}

			if (str[i] < 127)
			{
				result += '\0' + str[i];
			}

			++i;
		}

		return String(result);
	}

	Text get_text(const unsigned char* str, const char* font,
		const glm::vec4 &start_color,
		const Uint32 len = std::numeric_limits<Uint32>::max())
	{
		Text text;
		TextAttribute attribute;
		glm::vec4 color;
		Utf32String utf32_str;
		Uint32 i, count, index;

		count = strlen(reinterpret_cast<const char*>(str));
		count = std::min(count, len);

		attribute.set_size(12.0f);
		attribute.set_font(String(font));

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

	SelectionType get_selection_type(String name)
	{
		if (harvestables.count(name) > 0)
		{
			return st_harvest;
		}

		if (entrables.count(name) > 0)
		{
			return st_enter;
		}

		return st_select;
	}

	SelectionType get_selection_type(String name,
		const ElSelectionType selection)
	{
		switch (selection)
		{
			case est_none:
				return st_none;
			case est_select:
				return st_select;
			case est_harvest:
				return st_harvest;
			case est_pick:
				return st_pick;
			case est_enter:
				return st_enter;
			case est_detect:
				return get_selection_type(name);
		}

		return st_none;
	}

	String get_name(const char* name)
	{
		StringType str;
		std::size_t pos;

		if (name == 0)
		{
			return String();
		}

		str = utf8_to_string(name);

		pos = str.find(UTF8("./"));

		while (pos != std::string::npos)
		{
			str.erase(pos, 2);
			pos = str.find(UTF8("./"));
		}

		return String(str);
	}

	ObjectDescription get_object_description(const glm::vec3 &pos,
		const glm::vec3 &rot, const glm::vec4 &color, String name,
		const bool transparent, const Uint32 id,
		const ElSelectionType selection)
	{
		Transformation transformation;
		glm::quat rotation;
		float transparency;
		SelectionType el_selection;
		BlendType el_blend;

		assert(glm::all(glm::lessThanEqual(glm::abs(pos),
			glm::vec3(1e7f))));

		rotation = glm::quat();
		rotation = glm::rotate(rotation, rot.z,
			glm::vec3(0.0f, 0.0f, 1.0f));
		rotation = glm::rotate(rotation, rot.x,
			glm::vec3(1.0f, 0.0f, 0.0f));
		rotation = glm::rotate(rotation, rot.y,
			glm::vec3(0.0f, 1.0f, 0.0f));

		transformation.set_rotation(rotation);
		transformation.set_translation(pos);

		el_selection = get_selection_type(name, selection);

		if (transparent)
		{
			transparency = 0.7f;
			el_blend = bt_alpha_transparency_source_value;
		}
		else
		{
			transparency = 1.0f;
			el_blend = bt_disabled;
		}

		return ObjectDescription(transformation, StringVector(),
			name, transparency, id, el_selection, el_blend);
	}

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
	std::vector<StringType> lines, line;
	std::vector<StringType>::iterator it;
	StringType str;
	String harvestables_str;

	harvestables_str = file_system->get_file_string(
		String(UTF8("harvestable.lst")));

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
			harvestables.insert(String(str));
		}

		it++;
	}
}

extern "C" void load_entrable_list()
{
	std::vector<StringType> lines, line;
	std::vector<StringType>::iterator it;
	StringType str;
	String entrable_str;

	entrable_str = file_system->get_file_string(
		String(UTF8("entrable.lst")));

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
			entrables.insert(String(str));
		}

		it++;
	}
}

extern "C" void init_global_vars()
{
	TRY_BLOCK

	global_vars.reset(new GlobalVars());

	CATCH_BLOCK
}

extern "C" void init_file_system()
{
	TRY_BLOCK

	el::StringStream str;
	el::StringType config_dir, update_dir, custom_dir;

	file_system.reset(new el::FileSystem());

	config_dir = el::string_to_utf8(get_path_config_base());

	file_system->add_dir(el::String(config_dir));

	str << VER_MAJOR << UTF8("_") << VER_MINOR << UTF8("_") << VER_RELEASE;

	update_dir = config_dir;
	update_dir += UTF8("/updates");
	update_dir += str.str();

	file_system->add_dir(el::String(update_dir));

	update_dir = config_dir;
	update_dir += UTF8("/updates/2_0_alpha");

	file_system->add_dir(el::String(update_dir));

	update_dir = config_dir;
	update_dir += UTF8("/updates/data");

	file_system->add_dir(el::String(update_dir));

	custom_dir = config_dir;
	custom_dir += UTF8("/custom");

	file_system->add_dir(el::String(custom_dir));

	CATCH_BLOCK
}

extern "C" void file_system_add_dir(const char* dir)
{
	TRY_BLOCK

	file_system->add_dir(el::String(el::string_to_utf8(dir)));

	CATCH_BLOCK
}

#ifdef	EL_TIME_FRAME_DEBUG
Uint32 do_el_time = 0;
GLuint el_timer_id = 0;
#endif
extern "C" void init_engine()
{
	TRY_BLOCK

	CHECK_GL_ERROR();

//	set_log_level(eternal_lands::llt_debug);

	scene.reset(new el::Scene(global_vars, file_system));
	scene->init();

	script_engine.reset(new el::ScriptEngine(file_system));

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

extern "C" Uint32 engine_load_map(const char* name)
{
	try
	{
		glm::uvec2 size;
		Uint32 x, y;

		CHECK_GL_ERROR();

		scene->load(el::String(el::utf8_to_string(name)));

		CHECK_GL_ERROR();

		if (scene->get_dungeon())
		{
			dungeon = 1;
		}
		else
		{
			dungeon = 0;
		}

		ambient_r = scene->get_ambient().r;
		ambient_g = scene->get_ambient().g;
		ambient_b = scene->get_ambient().b;

		size = scene->get_tile_map_size();

		tile_map_size_x = size.x;
		tile_map_size_y = size.y;

		tile_map = (Uint8*)calloc(size.x * size.y, 1);

		for (y = 0; y < size.y; ++y)
		{
			for (x = 0; x < size.x; ++x)
			{
				tile_map[x + y * size.x] =
					scene->get_tile(x, y);
			}
		}

		size = scene->get_height_map_size();

		height_map = (Uint8*)calloc(size.x * size.y, 1);

		for (y = 0; y < size.y; ++y)
		{
			for (x = 0; x < size.x; ++x)
			{
				height_map[x + y * size.x] =
					scene->get_height(x, y);
			}
		}

		BOOST_FOREACH(const el::ParticleData &particle,
			scene->get_particles())
		{
			std::string particle_name;

			particle_name = particle.get_name().get();

			if (particle_name.compare(0, 5, UTF8("ec://")))
			{
				ec_create_effect_from_map_code(
					particle_name.substr(5).c_str(),
					particle.get_position().x,
					particle.get_position().y,
					particle.get_position().z,
					poor_man ? 6 : 10);
			}
			else
			{
#ifdef NEW_SOUND
				add_map_particle_sys(particle_name.c_str(),
					particle.get_position().x,
					particle.get_position().y,
					particle.get_position().z, 0);
#else
				add_particle_sys(particle_name.c_str(),
					particle.get_position().x,
					particle.get_position().y,
					particle.get_position().z, 0);
#endif // NEW_SOUND
			}
		}
	}
	catch (const boost::exception &exception)
	{
		LOG_EXCEPTION(exception);
		return 0;
	}
	catch (const std::exception &exception)	
	{
		LOG_EXCEPTION(exception);
		return 0;
	}

	return 1;
}

extern "C" void exit_engine()
{
	TRY_BLOCK

	CHECK_GL_ERROR();

	scene.reset();
	instances_builder.reset();
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
	glm::mat4 view_matrix;
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

	glGetFloatv(GL_MODELVIEW_MATRIX, glm::value_ptr(view_matrix));

	scene->set_view_matrix(view_matrix);
	scene->set_main_light_ambient(main_light_ambient);
	scene->set_main_light_color(main_light_color);
	scene->set_main_light_direction(main_light_direction);
	scene->set_lights(!is_day);

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

	CHECK_GL_ERROR();

	CATCH_BLOCK

	disable_opengl2_stuff();
}

extern "C" void engine_add_dynamic_object(const char* name, const float x_pos,
	const float y_pos, const float z_pos, const float x_rot,
	const float y_rot, const float z_rot, const char blended, const float r,
	const float g, const float b, const Uint32 id,
	const ElSelectionType selection)
{
	TRY_BLOCK

	DEBUG_CHECK_GL_ERROR();

	scene->add_object(get_object_description(glm::vec3(x_pos, y_pos, z_pos),
		glm::vec3(x_rot, y_rot, z_rot), glm::vec4(r, g, b, 1.0f),
		get_name(name), blended != 0, id, selection));

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

	scene->add_light(el::LightData(position, color, radius * scale, id));

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
	el::Transformation transformation;
	glm::quat rotation;
	glm::vec3 offset, attachment_shift, translation;

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

	rotation = glm::quat();
	rotation = glm::rotate(rotation, 180.0f - actor->z_rot,
		glm::vec3(0.0f, 0.0f, 1.0f));
	rotation = glm::rotate(rotation, actor->x_rot,
		glm::vec3(1.0f, 0.0f, 0.0f));
	rotation = glm::rotate(rotation, actor->y_rot,
		glm::vec3(0.0f, 1.0f, 0.0f));

	transformation.set_rotation(rotation);
	translation = offset + rotation * attachment_shift;
	transformation.set_scale(actors_defs[actor->actor_type].actor_scale *
		actor->scale);
	transformation.set_translation(translation);

	reinterpret_cast<el::Actor*>(actor->calmodel->getUserData(
		))->update_bones();
	reinterpret_cast<el::Actor*>(actor->calmodel->getUserData(
		))->set_world_transformation(transformation);

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
		))->update_bounding_box();

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
		LOG_ERROR(el::lt_default, el::String("%1%"),
			el::String("Actor is null"));
		return;
	}

	if (act->calmodel == 0)
	{
		LOG_ERROR(el::lt_default, el::String("Actor '%1%'('%2%') "
			"calmodel null"), get_string(act->skin_name,
				sizeof(act->skin_name)) %
			get_string(act->actor_name,
				sizeof(act->actor_name)));
		return;
	}

	if (act->calmodel->getUserData() == 0)
	{
		LOG_ERROR(el::lt_default, el::String("Actor '%1%'('%2%') "
			"calmodel user data null"), get_string(act->skin_name,
				sizeof(act->skin_name)) %
			get_string(act->actor_name,
				sizeof(act->actor_name)));
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

extern "C" Uint32 engine_get_next_free_dynamic_object_id()
{
	return scene->get_free_ids()->get_next_free_object_id(
		el::it_dynamic_object);
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
	if ((text == 0) || (len <= 1))
	{
		return 0;
	}

	try
	{
		script_engine->run_main(el::String("console"),
			el::String(text + 1, len - 1));
	}
	catch (const el::ASException &exception)
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
}

extern "C" void engine_set_view_distance(const float value)
{
	global_vars->set_view_distance(value);

	if (scene.get() != 0)
	{
		scene->terrain_change();
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

		scene->get_scene_resources().get_effect_cache()->reload(
			effect_debug);
		scene->shadow_map_change();
	}
}

extern "C" void engine_set_fog(const int value)
{
	global_vars->set_fog(value != 0);

	if (scene.get() != 0)
	{
		scene->get_scene_resources().get_effect_cache()->reload(
			effect_debug);
	}
}

extern "C" void engine_set_optmize_shader_source(const int value)
{
	global_vars->set_optmize_shader_source(value != 0);

	if (scene.get() != 0)
	{
		scene->get_scene_resources().get_effect_cache()->reload(
			effect_debug);
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
#ifdef	USE_SSE2
	global_vars->set_use_simd(value != 0 && SDL_HasSSE2());
#else	/* USE_SSE2 */
	global_vars->set_use_simd(false);
#endif	/* USE_SSE2 */
}

extern "C" void engine_set_use_s3tc_for_actors(const int value)
{
	global_vars->set_use_s3tc_for_actors(value != 0);
}

extern "C" void engine_set_use_block(const int value)
{
	global_vars->set_use_block(value != 0);

	if (scene.get() != 0)
	{
		scene->get_scene_resources().get_effect_cache()->reload(
			effect_debug);
	}
}

extern "C" void engine_set_use_alias(const int value)
{
	global_vars->set_use_alias(value != 0);

	if (scene.get() != 0)
	{
		scene->get_scene_resources().get_effect_cache()->reload(
			effect_debug);
	}
}

extern "C" void engine_set_use_in_out(const int value)
{
	global_vars->set_use_in_out(value != 0);

	if (scene.get() != 0)
	{
		scene->get_scene_resources().get_effect_cache()->reload(
			effect_debug);
	}
}

extern "C" void engine_set_use_functions(const int value)
{
	global_vars->set_use_functions(value != 0);

	if (scene.get() != 0)
	{
		scene->get_scene_resources().get_effect_cache()->reload(
			effect_debug);
	}
}

extern "C" void engine_set_low_quality_terrain(const int value)
{
	global_vars->set_low_quality_terrain(value != 0);
}

extern "C" void engine_set_clipmap_size(const int value)
{
	global_vars->set_clipmap_size(512 << value);

	if (scene.get() != 0)
	{
		scene->terrain_change();
	}
}

extern "C" void engine_set_clipmap_world_size(const int value)
{
	global_vars->set_clipmap_world_size(value);

	if (scene.get() != 0)
	{
		scene->terrain_change();
	}
}

extern "C" void engine_set_clipmap_slices(const int value)
{
	global_vars->set_clipmap_slices(value);

	if (scene.get() != 0)
	{
		scene->get_scene_resources().get_effect_cache()->reload(
			effect_debug);
		scene->terrain_change();
	}
}

extern "C" void engine_set_tile_world_size(const int value)
{
	global_vars->set_tile_world_size(value);

	if (scene.get() != 0)
	{
		scene->terrain_change();
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

extern "C" int engine_get_opengl_4_0()
{
	return global_vars->get_opengl_4_0();
}

extern "C" int engine_get_opengl_4_1()
{
	return global_vars->get_opengl_4_1();
}

extern "C" int engine_get_opengl_4_2()
{
	return global_vars->get_opengl_4_2();
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

extern "C" void engine_update_materials()
{
	TRY_BLOCK

	glm::vec4 time;

	time.x = real_game_minute / 60;
	time.y = real_game_minute % 60;
	time.z = real_game_second % 60;
	time.w = cur_time * 0.001f;

	scene->get_scene_resources().get_material_script_manager(
		)->execute_scripts(time);

	CATCH_BLOCK
}

extern "C" void engine_set_effect_debug(const int value)
{
	effect_debug = value;

	if (scene.get() != 0)
	{
		scene->get_scene_resources().get_effect_cache()->reload(
			effect_debug);
	}
}
