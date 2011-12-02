#include <math.h>
#include <stdlib.h>
#include "tiles.h"

unsigned char *tile_map;
unsigned char *height_map;
int tile_map_size_x;
int tile_map_size_y;
int tile_list[256];
char dungeon=0;//no sun
float ambient_r=0;
float ambient_g=0;
float ambient_b=0;
char map_file_name[256];

#ifdef NEW_SOUND
int get_tile_type(int x, int y)
{
	int tile_num = (y / 6) * tile_map_size_x + (x / 6);
//	printf("X: %d, Y: %d, tile_num: %d\n", x, y, tile_num);
	if (x >= 0 && (x / 6) <= tile_map_size_x && y >= 0 && (y / 6) <= tile_map_size_y)
		return tile_map[tile_num];
	else
		return -1;
}
#endif // NEW_SOUND
