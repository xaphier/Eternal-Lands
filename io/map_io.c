#include "map_io.h"
#include "../asc.h"
#include "../bbox_tree.h"
#include "../elconfig.h"
#include "../errors.h"
#include "../init.h"
#include "../lights.h"
#include "../map.h"
#include "../particles.h"
#include "../reflection.h"
#include "../tiles.h"
#include "../translate.h"
#include "elfilewrapper.h"
 #include "../eye_candy_wrapper.h"
#ifdef CLUSTER_INSIDES
#include "../cluster.h"
#endif
#include "../engine.h"

#ifndef SHOW_FLICKERING
const float offset_2d_increment = (1.0f / 32768.0f);	// (1.0f / 8388608.0f) is the minimum for 32-bit floating point.
float offset_2d = (1.0f / 32768.0f);
const float offset_2d_max = 0.01f;
#endif

int get_tile_map_sizes(const char *file_name, int *x, int *y)
{
	map_header cur_map_header;
	char* file_mem;
	el_file_ptr file;

	file = el_open(file_name);

	if (!file)
	{
		return 0;
	}

	file_mem = el_get_pointer(file);

	memcpy(&cur_map_header, file_mem, sizeof(cur_map_header));
	*x = SDL_SwapLE32(cur_map_header.tile_map_x_len);
	*y = SDL_SwapLE32(cur_map_header.tile_map_y_len);
	el_close(file);
	return 1;
}

static int do_load_map(const char *file_name, update_func *update_function)
{
	map_header cur_map_header;
	char* file_mem;
#ifdef CLUSTER_INSIDES
	char* occupied = 0;
	int have_clusters;
#endif
	el_file_ptr file;

	file = el_open(file_name);

	if (!file)
	{
		return 0;
	}

#ifdef EXTRA_DEBUG
	ERR();
#endif
	file_mem = el_get_pointer(file);

	my_strcp(map_file_name, file_name);

	LOG_DEBUG("Loading map '%s'.", file_name);

	main_bbox_tree_items = create_bbox_items(1024);

	memcpy(&cur_map_header, file_mem, sizeof(cur_map_header));
	cur_map_header.tile_map_x_len = SDL_SwapLE32(cur_map_header.tile_map_x_len);
	cur_map_header.tile_map_y_len = SDL_SwapLE32(cur_map_header.tile_map_y_len);
	cur_map_header.tile_map_offset = SDL_SwapLE32(cur_map_header.tile_map_offset);
	cur_map_header.height_map_offset = SDL_SwapLE32(cur_map_header.height_map_offset);
	cur_map_header.obj_3d_struct_len = SDL_SwapLE32(cur_map_header.obj_3d_struct_len);
	cur_map_header.obj_3d_no = SDL_SwapLE32(cur_map_header.obj_3d_no);
	cur_map_header.obj_3d_offset = SDL_SwapLE32(cur_map_header.obj_3d_offset);
	cur_map_header.obj_2d_struct_len = SDL_SwapLE32(cur_map_header.obj_2d_struct_len);
	cur_map_header.obj_2d_no = SDL_SwapLE32(cur_map_header.obj_2d_no);
	cur_map_header.obj_2d_offset = SDL_SwapLE32(cur_map_header.obj_2d_offset);
	cur_map_header.lights_struct_len = SDL_SwapLE32(cur_map_header.lights_struct_len);
	cur_map_header.lights_no = SDL_SwapLE32(cur_map_header.lights_no);
	cur_map_header.lights_offset = SDL_SwapLE32(cur_map_header.lights_offset);

	cur_map_header.ambient_r = SwapLEFloat(cur_map_header.ambient_r);
	cur_map_header.ambient_g = SwapLEFloat(cur_map_header.ambient_g);
	cur_map_header.ambient_b = SwapLEFloat(cur_map_header.ambient_b);

	cur_map_header.particles_struct_len = SDL_SwapLE32(cur_map_header.particles_struct_len);
	cur_map_header.particles_no = SDL_SwapLE32(cur_map_header.particles_no);
	cur_map_header.particles_offset = SDL_SwapLE32(cur_map_header.particles_offset);

#ifdef CLUSTER_INSIDES
	cur_map_header.clusters_offset = SDL_SwapLE32(cur_map_header.clusters_offset);
#endif

	LOG_DEBUG("Checking map '%s' file signature.", file_name);

	//verify if we have a valid file
	if(cur_map_header.file_sig[0]!='e'||
	   cur_map_header.file_sig[1]!='l'||
	   cur_map_header.file_sig[2]!='m'||
	   cur_map_header.file_sig[3]!='f')
	{
		LOG_ERROR(invalid_map, map_file_name);
		exit_now = 1; // We might as well quit...
		el_close(file);

		return 0;
	}

	LOG_DEBUG("Checking map '%s' sizes.", file_name);

	// Check the sizes of structures. If they don't match, we have a
	// major problem, since these structures are supposed to be
	// written flat out to disk.
	if (cur_map_header.obj_3d_struct_len != sizeof (object3d_io)
	    || cur_map_header.obj_2d_struct_len != sizeof (obj_2d_io)
	    || cur_map_header.lights_struct_len != sizeof (light_io)
	    || (cur_map_header.particles_struct_len != sizeof (particles_io) && cur_map_header.particles_no > 0)
	   )
	{
		LOG_ERROR ("Invalid object size on map %s", map_file_name);
		exit_now = 1; // We might as well quit...
		el_close(file);

		return 0;
	}
/*
	engine_load_map_old(map_file_name, cur_map_header.ambient_r,
		cur_map_header.ambient_g, cur_map_header.ambient_b,
		cur_map_header.dungeon);
*/

	update_function(load_map_str, 0);

	//get the map size
	tile_map_size_x = cur_map_header.tile_map_x_len;
	tile_map_size_y = cur_map_header.tile_map_y_len;

	LOG_DEBUG("Map '%s' size <%d, %d>.", file_name, tile_map_size_x,
		tile_map_size_y);

	// allocate the tile map (it was destroyed), and fill it
	tile_map = calloc (tile_map_size_x*tile_map_size_y, 1);
	memcpy(tile_map, file_mem + cur_map_header.tile_map_offset, tile_map_size_x*tile_map_size_y);

	// allocate the height map, and fill it
	height_map = calloc (tile_map_size_x*tile_map_size_y*6*6, 1);
	memcpy(height_map, file_mem + cur_map_header.height_map_offset, tile_map_size_x*tile_map_size_y*6*6);

	engine_load_map(map_file_name);
/*
	for (i = 0; i < cur_map_header.particles_no; i++)
	{
		particles_io cur_particles_io = particles[i];

		cur_particles_io.x_pos = SwapLEFloat (cur_particles_io.x_pos);
		cur_particles_io.y_pos = SwapLEFloat (cur_particles_io.y_pos);
		cur_particles_io.z_pos = SwapLEFloat (cur_particles_io.z_pos);

		LOG_DEBUG("Adding particle(%d) '%s' at <%d, %f, %f>.", i,
			cur_particles_io.file_name, cur_particles_io.x_pos,
			cur_particles_io.y_pos, cur_particles_io.z_pos);

		if (!strncmp(cur_particles_io.file_name, "ec://", 5))
		{
			ec_create_effect_from_map_code(cur_particles_io.file_name + 5, cur_particles_io.x_pos, cur_particles_io.y_pos, cur_particles_io.z_pos, (poor_man ? 6 : 10));
		}
		else
		{
#ifdef NEW_SOUND
			add_map_particle_sys (cur_particles_io.file_name, cur_particles_io.x_pos, cur_particles_io.y_pos, cur_particles_io.z_pos, 0);
#else
			add_particle_sys (cur_particles_io.file_name, cur_particles_io.x_pos, cur_particles_io.y_pos, cur_particles_io.z_pos, 0);
#endif // NEW_SOUND
		}

#ifndef FASTER_MAP_LOAD
		if (i % 200 == 0)
		{
			update_function(load_particles_str, progress);
		}
#endif
	}
*/
	return 1;
}

int load_map(const char *file_name, update_func *update_function)
{
	int result;

	ENTER_DEBUG_MARK("load map");

	result = do_load_map(file_name, update_function);

	LEAVE_DEBUG_MARK("load map");

	return result;
}

