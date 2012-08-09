/****************************************************************************
 *            engine.h
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
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
	est_none = 0,
	est_select = 1,
	est_harvest = 2,
	est_pick = 3,
	est_enter = 4,
	est_detect = 5
} ElSelectionType;

void init_engine();
void init_global_vars();
void init_file_system();
void exit_engine();
void exit_global_vars();
void exit_file_system();
void file_system_add_dir(const char* dir);
Uint32 engine_load_map(const char* name);
void engine_add_dynamic_object(const char* name, const float x_pos,
	const float y_pos, const float z_pos, const float x_rot,
	const float y_rot, const float z_rot, const char blended,
	const float r, const float g, const float b, const Uint32 id,
	const ElSelectionType selection);
void engine_add_light(const float x_pos, const float y_pos, const float z_pos,
	const float r, const float g, const float b, const float radius,
	const Uint32 id);
void engine_load_enhanced_actor_texture(actor *act);
Uint32 engine_get_actor_texture_loaded(actor *act);
Uint32 engine_get_object_under_mouse_pickable();
Uint32 engine_get_object_under_mouse_harvestable();
Uint32 engine_get_object_under_mouse_entrable();
void engine_remove_object(const Uint32 id);
void engine_get_object_position(const Uint32 id, float* x, float* y, float* z);
void engine_remove_light(const Uint32 id);
Uint32 engine_get_next_free_dynamic_object_id();
void engine_set_window_size(const Uint32 widht, const Uint32 height,
	const Uint32 hud_x, const Uint32 hud_y);

void engine_build_buffers(actor_types* a);
void engine_build_actor_bounding_box(actor* a);
void engine_set_transformation_buffers(actor* act);
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
	const float z_near);
int command_lua(char *text, int len);

void engine_cull_scene();
void engine_draw_scene();

void engine_set_shadow_map_size(const int value);
void engine_set_shadow_quality(const int value);
void engine_set_shadow_distance(const float value);
void engine_set_view_distance(const float value);
void engine_set_fog(const int value);
void engine_set_optmize_shader_source(const int value);
void engine_set_opengl_version(const int value);
void engine_set_fog_data(const float* color, const float density);
void engine_set_shadow_map_filtering(const int value);
void engine_set_use_simd(const int value);
void engine_set_use_s3tc_for_actors(const int value);
void engine_set_use_block(const int value);
void engine_set_use_in_out(const int value);
void engine_set_use_functions(const int value);
void engine_set_low_quality_terrain(const int value);
void engine_set_clipmap_size(const int value);
void engine_set_clipmap_world_size(const int value);
void engine_set_clipmap_slices(const int value);
void engine_set_effect_debug(const int value);
void engine_set_use_multisample_shadows(const int value);
void engine_set_use_scene_fbo(const int value);
void engine_set_light_system(const int value);

float get_tile_height_linear(const float x, const float y);
int engine_has_terrain();

int engine_get_opengl_3_0();
int engine_get_opengl_3_1();
int engine_get_opengl_3_2();
int engine_get_opengl_3_3();
int engine_get_opengl_4_0();
int engine_get_opengl_4_1();
int engine_get_opengl_4_2();
void engine_update_materials();

void engine_init_console_logging();

#ifdef __cplusplus
}
#endif

#endif	/* UUID_2db3c541_2d94_44a5_83de_ce6a4cf1f7ed */
