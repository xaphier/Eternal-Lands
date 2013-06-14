#include <math.h>
#include <stdlib.h>
#include "tiles.h"

unsigned char *height_map;
int tile_map_size_x;
int tile_map_size_y;
char dungeon=0;//no sun
float ambient_r=0;
float ambient_g=0;
float ambient_b=0;
char map_file_name[256];

#ifdef NEW_SOUND
int get_tile_type(int x, int y)
{
// deactivated, doesnt work with the new tile maps anyway
	return -1;
}
#endif // NEW_SOUND

int get_tile_walkable(const int x, const int y)
{
	if (!get_tile_valid(x, y))
	{
		return 0;
	}

	return (height_map[x * tile_map_size_x * 6 + y] & 0x3F) != 0;
}

int get_tile_valid(const int x, const int y)
{
	return (x >= 0) && (x < (tile_map_size_x * 6)) &&
		(y >= 0) && (y < (tile_map_size_y * 6));
}
