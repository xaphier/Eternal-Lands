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
	my_strcp(map_file_name, file_name);

	LOG_DEBUG_OLD("Loading map '%s'.", file_name);

	main_bbox_tree_items = create_bbox_items(1024);

	return engine_load_map(file_name);
}

int load_map(const char *file_name, update_func *update_function)
{
	int result;

	update_function(load_map_str, 0.0f);

	result = do_load_map(file_name, update_function);

	update_function(init_done_str, 100.0f);

	return result;
}

