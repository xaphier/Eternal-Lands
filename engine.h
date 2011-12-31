/****************************************************************************
 *            engine.h
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_2db3c541_2d94_44a5_83de_ce6a4cf1f7ed
#define	UUID_2db3c541_2d94_44a5_83de_ce6a4cf1f7ed

#include "platform.h"
#include "actors.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum
{
	st_none = 0,
	st_select = 1,
	st_harvest = 2,
	st_pick = 3,
	st_enter = 4,
	st_detect = 5
} SelectionType;

void init_engine();
void init_global_vars();
void init_file_system();
void exit_engine();
void exit_global_vars();
void exit_file_system();
void file_system_add_dir(const char* dir);
void load_map_engine(const char* name, const float r, const float g,
	const float b, const int dungeon);
void add_tile(const Uint16 x, const Uint16 y, const Uint8 tile);
void add_instanced_object_engine(const char* name, const float x_pos,
	const float y_pos, const float z_pos, const float x_rot,
	const float y_rot, const float z_rot, const char blended,
	const float r, const float g, const float b, const Uint32 id,
	const SelectionType selection);
void add_object_engine(const char* name, const float x_pos, const float y_pos,
	const float z_pos, const float x_rot, const float y_rot,
	const float z_rot, const char blended, const float r, const float g,
	const float b, const Uint32 id, const SelectionType selection);
void done_object_adding();
void add_light_engine(const float x_pos, const float y_pos, const float z_pos,
	const float r, const float g, const float b, const float radius,
	const Uint32 id);
void load_enhanced_actor_texture(actor *act);
Uint32 get_actor_texture_loaded(actor *act);
Uint32 get_object_under_mouse_pickable();
Uint32 get_object_under_mouse_harvestable();
Uint32 get_object_under_mouse_entrable();
void remove_object(const Uint32 id);
void get_object_position(const Uint32 id, float* x, float* y, float* z);
void remove_light(const Uint32 id);
Uint32 get_next_free_id();
void set_next_free_id(const Uint32 id);

void build_buffers(actor_types* a);
void build_actor_bounding_box(actor* a);
void set_transformation_buffers(actor* act);
struct CalModel *model_new(const Uint32 type_id, const Uint32 id,
	const char* name, const Uint32 kind_of_actor,
	const Uint32 enhanced_actor, Uint32* client_id);
void model_delete(const Uint32 client_id);
void model_attach_mesh(actor *act, int mesh_id);
void model_detach_mesh(actor *act, int mesh_id);
void load_harvestable_list();
void load_entrable_list();
void engine_update_actor_buffs(actor *act, Uint32 buffs);
void engine_set_shader_quality(const char* quality);
void engine_resize_root_window(const float fov, const float aspect,
	const float z_near, const float z_far);
void add_font(const char* file_name, const char* index, const float size);
void draw_text(const char* str, const char* index, const float x,
	const float y, const float r, const float g, const float b);
int command_lua(char *text, int len);

void engine_cull_scene();
void engine_draw_scene();

void set_shadow_map_size(const int value);
void set_shadow_quality(const int value);
void set_shadow_distance(const float value);
void set_view_distance(const float value);
void set_fog(const int value);
void set_alpha_to_coverage(const int value);
void set_optmize_shader_source(const int value);
void set_opengl_version(const int value);
void set_fog_data(const float* color, const float density);
void set_shadow_map_filter(const int value);
int get_opengl_3_0();
int get_opengl_3_1();
int get_opengl_3_2();
int get_opengl_3_3();

#ifdef __cplusplus
}
#endif

#endif	/* UUID_2db3c541_2d94_44a5_83de_ce6a4cf1f7ed */

