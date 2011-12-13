#include <stdlib.h>
#include <math.h>
#ifndef WINDOWS
 #include <locale.h>
#endif
#include "particles.h"
#include "asc.h"
#include "draw_scene.h"
#include "errors.h"
#include "gl_init.h"
#include "init.h"
#include "pathfinder.h"
#include "string.h"
#include "sound.h"
#include "textures.h"
#include "tiles.h"
#include "translate.h"
#include "vmath.h"
#ifdef CLUSTER_INSIDES
#include "cluster.h"
#endif
#include "eye_candy_wrapper.h"
#include "translate.h"
#include "io/elpathwrapper.h"
#include "io/elfilewrapper.h"
#include "lights.h"
#ifdef	NEW_TEXTURES
#include "image_loading.h"
#endif	/* NEW_TEXTURES */
#include "engine.h"

/* NOTE: This file contains implementations of the following, currently unused, and commented functions:
 *          Look at the end of the file.
 *
 * void dump_part_sys_info();
 */

#ifdef NEW_SOUND
int real_add_particle_sys (const char *file_name, float x_pos, float y_pos, float z_pos, unsigned int dynamic);
#endif // NEW_SOUND

#define TELEPORTER_PARTICLE_SYS 0
#define TELEPORT_PARTICLE_SYS 1
#define BAG_PARTICLE_SYS 2
#define BURST_PARTICLE_SYS 3
#define FIRE_PARTICLE_SYS 4
#define FOUNTAIN_PARTICLE_SYS 5

#define PARTICLE_RANDOM(min,max) (min+(max-min)*(rand()/(float)RAND_MAX))
#define PARTICLE_RANDOM2(min,max) (min+0.5*(max-min)+0.5*(max-min)/(float)((rand()%200)-100+0.5))

#define PART_SYS_VISIBLE_DIST_SQ 20*20

#define MAX_PARTICLE_TEXTURES 16 // The maximum number of textures used for particle systems

#ifdef ELC
int use_point_particles = 1;
#else
int use_point_particles = 0;
#endif
int enable_blood = 0;

/******************************************************
 *           PARTICLE SYSTEM DEFINITIONS              *
 ******************************************************/

int part_strcmp(char * s1, char *s2)
{
	while(*s1 && *s2)
		{
			if(*s1!=*s2 && (*s1!='/' && *s1!='\\' && *s2!='/' && *s2!='\\' )) return 1;
			s1++;s2++;
		}
	return *s1!=*s2;
}

static __inline__ void calc_particle_random_min_max(float f1, float f2, float* v_min, float* v_max)
{
	if (f1 < f2)
	{
		*v_min = f1;
		*v_max = f2;
	}
	else
	{
		*v_min = f2;
		*v_max = f1;
	}
}

static __inline__ void calc_particle_random2_min_max(float f1, float f2, float* v_min, float* v_max)
{	
	*v_min = (f1+f2)*0.5f-abs(f2-f1);
	*v_max = (f1+f2)*0.5f+abs(f2-f1);
}

#ifdef MAP_EDITOR2
#define MAP_EDITOR
#endif

#ifndef MAP_EDITOR
void add_fire_at_tile (int kind, Uint16 x_tile, Uint16 y_tile)
{
	float x = 0.5f * x_tile + 0.25f;
	float y = 0.5f * y_tile + 0.25f;
	float z = 0.0;
#ifdef NEW_SOUND
	int snd;
#endif // NEW_SOUND

	switch (kind)
	{
		case 2:
			ec_create_campfire(x, y, z, 0.0, 1.0, (poor_man ? 6 : 10), 3.1);
#ifdef NEW_SOUND
			snd = get_sound_index_for_particle_file_name("./particles/fire_big.part");
#endif // NEW_SOUND
			break;
		case 1:
		default:
			ec_create_campfire(x, y, z, 0.0, 1.0, (poor_man ? 6 : 10), 2.4);
#ifdef NEW_SOUND
			snd = get_sound_index_for_particle_file_name("./particles/fire_small.part");
#endif // NEW_SOUND
			break;
	}
#ifdef NEW_SOUND
	if (sound_on && snd >= 0)
	{
		add_particle_sound(snd, x_tile, y_tile);
	}
#endif // NEW_SOUND
}

void remove_fire_at_tile (Uint16 x_tile, Uint16 y_tile)
{
	float x = 0.5f * x_tile + 0.25f;
	float y = 0.5f * y_tile + 0.25f;
	
	ec_delete_effect_loc_type(x, y, EC_CAMPFIRE);
#ifdef NEW_SOUND
	stop_sound_at_location(x_tile, y_tile);
#endif // NEW_SOUND
	return;
}
#endif // !MAPEDITOR
/*********************************************************************
 *          CREATION OF NEW PARTICLES AND SYSTEMS                    *
 *********************************************************************/
#ifndef	MAP_EDITOR

void rotate_vector3f(float *vector, float x, float y, float z)
{
	// rotation matrixes
	float rot_x[9];
	float rot_y[9];
	float rot_z[9];
	float result_x[3];
	float result_y[3];
	MAT3_ROT_X(rot_x, x * (M_PI / 180.0f));
	MAT3_ROT_Y(rot_y, y * (M_PI / 180.0f));
	MAT3_ROT_Z(rot_z, z * (M_PI / 180.0f));
	// rotate around x achsis
	MAT3_VECT3_MULT(result_x, rot_x, vector);
	// rotate around y achsis
	MAT3_VECT3_MULT(result_y, rot_y, result_x);
	// rotate around z achsis
	MAT3_VECT3_MULT(vector, rot_z, result_y);
}

void add_ec_effect_to_e3d(object3d* e3d) 
{
	ec_bounds *bounds = ec_create_bounds_list();
	float shift[3] = { 0.0f, 0.0f, 0.0f };
	// useful for debugging: 
	// ec_create_fountain(e3d->x_pos + shift[0], e3d->y_pos + shift[1], e3d->z_pos + shift[2], 0.0, 1.0, (e3d->z_pos >= 0.8 ? e3d->z_pos - 0.8 : 0.0), 0, 1.0, (poor_man ? 6 : 10));
	// printf("%f %f %s %i\n", e3d->x_pos*2, e3d->y_pos*2, e3d->file_name, e3d->self_lit);
	if (strstr(e3d->file_name, "/lantern1.e3d"))
	{
		ec_add_smooth_polygon_bound(bounds, 2.0, 0.25);
		shift[2] += 0.25f; // add height
		rotate_vector3f(shift, e3d->x_rot, e3d->y_rot, e3d->z_rot);
		ec_create_fireflies(e3d->x_pos + shift[0], e3d->y_pos + shift[1], e3d->z_pos + shift[2], 1.0, 1.0, 0.00625, 1.0, bounds);
	}	
	else if (strstr(e3d->file_name, "/lantern2.e3d"))
	{
		ec_add_smooth_polygon_bound(bounds, 2.0, 0.25);
		shift[2] += 0.25f; // add height
		rotate_vector3f(shift, e3d->x_rot, e3d->y_rot, e3d->z_rot);
		ec_create_fireflies(e3d->x_pos + shift[0], e3d->y_pos + shift[1], e3d->z_pos + shift[2], 1.0, 1.0, 0.005, 1.0, bounds);
	}	
	else if (strstr(e3d->file_name, "/lantern3.e3d"))
	{
		ec_add_smooth_polygon_bound(bounds, 2.0, 0.25);
		shift[2] += 0.25f; // add height
		rotate_vector3f(shift, e3d->x_rot, e3d->y_rot, e3d->z_rot);
		ec_create_fireflies(e3d->x_pos + shift[0], e3d->y_pos + shift[1], e3d->z_pos + shift[2], 1.0, 1.0, 0.005, 1.0, bounds);
	}	
	else if (strstr(e3d->file_name, "/light1.e3d"))
	{
		ec_add_smooth_polygon_bound(bounds, 2.0, 0.33);
		shift[2] += 2.85f; // add height
		rotate_vector3f(shift, e3d->x_rot, e3d->y_rot, e3d->z_rot);
		ec_create_fireflies(e3d->x_pos + shift[0], e3d->y_pos + shift[1], e3d->z_pos + shift[2], 1.0, 1.0, 0.01, 1.0, bounds);
	}	
	else if (strstr(e3d->file_name, "/light2.e3d"))
	{
		ec_add_smooth_polygon_bound(bounds, 2.0, 0.4);
		shift[2] += 2.95f; // add height
		rotate_vector3f(shift, e3d->x_rot, e3d->y_rot, e3d->z_rot);
		ec_create_fireflies(e3d->x_pos + shift[0], e3d->y_pos + shift[1], e3d->z_pos + shift[2], 1.0, 1.0, 0.0125, 1.0, bounds);
	}	
	else if (strstr(e3d->file_name, "/light3.e3d"))
	{
		ec_add_smooth_polygon_bound(bounds, 2.0, 0.33);
		shift[2] += 3.5f; // add height
		shift[0] -= 0.33f; // the light is not centered
		rotate_vector3f(shift, e3d->x_rot, e3d->y_rot, e3d->z_rot);
		ec_create_fireflies(e3d->x_pos + shift[0], e3d->y_pos + shift[1], e3d->z_pos + shift[2], 1.0, 1.0, 0.015, 1.0, bounds);
	}	
	else if (strstr(e3d->file_name, "/light4.e3d"))
	{
		ec_add_smooth_polygon_bound(bounds, 2.0, 0.4);
		shift[2] += 1.75f; // add height
		ec_create_fireflies(e3d->x_pos, e3d->y_pos, e3d->z_pos + 1.75f, 1.0, 1.0, 0.0075, 1.0, bounds);
	}	
}

#ifdef NEW_SOUND
// Wrapper for map sounds (checks for existing sounds in similar location for multi-particle-system effects)
int add_map_particle_sys(const char *file_name, float x_pos, float y_pos, float z_pos, unsigned int dynamic)
{
	int snd;

	if (!no_sound)
	{
		snd = get_sound_index_for_particle_file_name(file_name);
		if (snd >= 0)
		{
			add_map_sound(snd, (x_pos - 0.25f) * 2, (y_pos - 0.25f) * 2);
		}
	}
	return real_add_particle_sys(file_name, x_pos, y_pos, z_pos, dynamic);
}

// Wrapper for regular (event triggered) particle systems (no location check)
int add_particle_sys (const char *file_name, float x_pos, float y_pos, float z_pos, unsigned int dynamic)
{
	int snd;

	if (!no_sound)
	{
		snd = get_sound_index_for_particle_file_name(file_name);
		if (snd >= 0)
		{
			add_particle_sound(snd, (x_pos - 0.25f) * 2, (y_pos - 0.25f) * 2);
		}
	}
	return real_add_particle_sys(file_name, x_pos, y_pos, z_pos, dynamic);
}

int real_add_particle_sys (const char *file_name, float x_pos, float y_pos, float z_pos, unsigned int dynamic)
#else
int add_particle_sys (const char *file_name, float x_pos, float y_pos, float z_pos, unsigned int dynamic)
#endif // NEW_SOUND
#else // !MAP_EDITOR
int add_particle_sys (const char *file_name, float x_pos, float y_pos, float z_pos)
#endif
{
	if (use_eye_candy)
	{
		if (!strncmp("fou", file_name + 12, 3))
			ec_create_fountain(x_pos, y_pos, z_pos + 0.15, 0.0, 1.0, (z_pos >= 0.8 ? z_pos - 0.8 : 0.0), 0, 1.0, (poor_man ? 6 : 10));
		else if ((use_fancy_smoke) && (!strncmp("smo", file_name + 12, 3)))
		{
			if (file_name[17] == '1')
				ec_create_smoke(x_pos, y_pos, z_pos, 0.0, 1.0, 0.3, (poor_man ? 6 : 10));
			else if (file_name[17] == '2')
				ec_create_smoke(x_pos, y_pos, z_pos, 0.0, 1.0, 0.45, (poor_man ? 6 : 10));
			else if (file_name[17] == '3')
				ec_create_smoke(x_pos, y_pos, z_pos, 0.0, 1.0, 1.6, (poor_man ? 6 : 10));
			else if (file_name[17] == '_')
				ec_create_smoke(x_pos, y_pos, z_pos, 0.0, 1.0, 1.1, (poor_man ? 6 : 10));
			else
				ec_create_smoke(x_pos, y_pos, z_pos, 0.0, 1.0, 0.5, (poor_man ? 6 : 10));
		}
		else if (!strncmp("tel", file_name + 12, 3))
		{
			if (file_name[21] == 'i')
			{
				ec_create_selfmagic_teleport_to_the_portals_room(x_pos, y_pos, z_pos, (poor_man ? 6 : 10));
			}
			else if (file_name[21] == 'o')
			{
				ec_create_selfmagic_teleport_to_the_portals_room(x_pos, y_pos, z_pos, (poor_man ? 6 : 10));
			}
			else
				ec_create_teleporter(x_pos, y_pos, z_pos, 0.0, 1.0, 1.0, (poor_man ? 6 : 10));
		}
		else if (!strncmp("fir", file_name + 12, 3))
		{
			if (!strncmp("big", file_name + 17, 3))
				ec_create_campfire(x_pos, y_pos, z_pos, 0.0, 1.0, (poor_man ? 6 : 10), 1.5);
			else if (!strncmp("for", file_name + 17, 3))
				ec_create_campfire(x_pos, y_pos - 0.2, z_pos, 0.0, 1.0, (poor_man ? 6 : 10), 2.0);
			else if (!strncmp("min", file_name + 17, 3))
				ec_create_campfire(x_pos, y_pos, z_pos, 0.0, 1.0, (poor_man ? 6 : 10), 0.4);
			else if (!strncmp("sma", file_name + 17, 3))
				ec_create_campfire(x_pos, y_pos, z_pos, 0.0, 1.0, (poor_man ? 6 : 10), 0.6);
			else if (!strncmp("tor", file_name + 17, 3))
				ec_create_lamp(x_pos, y_pos, z_pos, 0.0, 1.0, 1.6, (poor_man ? 6 : 10));
			else
			{
				return -1;
			}
		}
		else if (!strncmp("can", file_name + 12, 3))
			ec_create_candle(x_pos, y_pos, z_pos, 0.0, 1.0, 0.7, (poor_man ? 6 : 10));
		else
		{
				return -1;
		}
	}

	// If we got here, the eye candy system handled this particle
	// system. Return an invalid particle ID to signal that nothing
	// was added to particles_list, but not -1 since this is not an 
	// error.
	return -2;
}

#ifndef	MAP_EDITOR
int add_particle_sys_at_tile (const char *file_name, int x_tile, int y_tile, unsigned int dynamic)
#else
int add_particle_sys_at_tile (const char *file_name, int x_tile, int y_tile)
#endif
{
	int	height;
	
	//watch for unwalkable tiles getting particles
	if( (height_map[y_tile*tile_map_size_x*6+x_tile]&0x3F) > 0){
		height= height_map[y_tile*tile_map_size_x*6+x_tile];
	} else {
		int	count= 0;
		int	x, y;
		
		// this is an unwalkable tile, make a guess by averaging nearby heights
		height= 0;
		for(x= x_tile-1; x <= x_tile+1; x++){
			if(x >= 0 && x < tile_map_size_x*6)
			for(y= y_tile-1; y <= y_tile+1; y++){
				if(y >= 0 && y < tile_map_size_y*6 && (height_map[y*tile_map_size_x*6+x]&0x3F) > 0){
					height+= height_map[y*tile_map_size_x*6+x]&0x3F;
					count++;
				}
			}
		}
		if(count > 0){
			height/= count;
		}
		// if that didn't work, lets use a default height
		if(height <= 0){
			height= 8;
		}
	}
#ifndef	MAP_EDITOR
	return add_particle_sys (file_name, (float) x_tile / 2.0 + 0.25f, (float) y_tile / 2.0 + 0.25f, -2.2f + height * 0.2f, dynamic);
#else
	return add_particle_sys (file_name, (float) x_tile / 2.0 + 0.25f, (float) y_tile / 2.0 + 0.25f, -2.2f + height * 0.2f);
#endif
}

/******************************************************************************
 *                        MISC HELPER FUNCTIONS                               *
 ******************************************************************************/
#ifdef ELC
void add_teleporters_from_list (const Uint8 *teleport_list)
{
	Uint16 teleporters_no;
	int i;
	int teleport_x,teleport_y,my_offset;
	float x,y,z;

	teleporters_no=SDL_SwapLE16(*((Uint16 *)(teleport_list)));

	for (i = 0; i < teleporters_no; i++)
	{
		my_offset = i * 5 + 2;
		teleport_x=SDL_SwapLE16(*((Uint16 *)(teleport_list+my_offset)));
		teleport_y=SDL_SwapLE16(*((Uint16 *)(teleport_list+my_offset+2)));
						
		//later on, maybe we want to have different visual types
		//now, get the Z position
		if(teleport_y*tile_map_size_x*6+teleport_x<tile_map_size_y*6*tile_map_size_x*6)
			z=-2.2f+height_map[teleport_y*tile_map_size_x*6+teleport_x]*0.2f;
		else continue;
		//convert from height values to meters
		x=(float)teleport_x/2;
		y=(float)teleport_y/2;
		//center the object
		x += 0.25f;
		y += 0.25f;

		add_particle_sys ("./particles/teleporter.part", x, y, z, 1);
		add_object_engine("./3dobjects/portal1.e3d", x, y, z, 0.0f,
			0.0f, 0.0f, 0, 1.0f, 1.0f, 1.0f, get_next_free_id(),
			st_detect);
			
		//mark the teleporter as an unwalkable so that the pathfinder
		//won't try to plot a path through it

		pf_tile_map[teleport_y*tile_map_size_x*6+teleport_x].z = 0;
	}
}

/* currently UNUSED
void dump_part_sys_info()
{
	char str[256];
	int i,partdefs=0,partsys=0;
	LOG_TO_CONSOLE(c_grey1,particle_system_dump);
	if(!particles_percentage)
		{
			LOG_TO_CONSOLE(c_grey1,particles_disabled_str);
			return;
		}
	if(have_point_sprite)
	  LOG_TO_CONSOLE(c_grey1,point_sprites_enabled);
	else
	  LOG_TO_CONSOLE(c_grey1,using_textured_quads);
	LOG_TO_CONSOLE(c_grey1,definitions_str);
	for(i=0;i<MAX_PARTICLE_DEFS;i++)
		if(defs_list[i])
			{
				partdefs++;
				LOG_TO_CONSOLE(c_grey1,defs_list[i]->file_name);
			}
	safe_snprintf(str, sizeof(str), "#%s: %i",my_tolower(definitions_str),partdefs);
	LOG_TO_CONSOLE(c_grey1,str);
	for(i=0;i<MAX_PARTICLE_SYSTEMS;i++)
		if(particles_list[i])partsys++;
	safe_snprintf(str, sizeof(str), "#%s: %i",part_sys_str,partsys);
	LOG_TO_CONSOLE(c_grey1,str);
	safe_snprintf(str, sizeof(str), "#%s: %i%%",part_part_str,particles_percentage);
	LOG_TO_CONSOLE(c_grey1,str);
}
*/
#endif // ELC

#ifdef MAP_EDITOR
void get_and_set_particle_texture_id (int i)
{
#ifdef	NEW_TEXTURES
	bind_texture(particle_textures[i]);
#else	/* NEW_TEXTURES */
	get_and_set_texture_id(particle_textures[i]);
#endif	/* NEW_TEXTURES */
}
#endif // MAP_EDITOR
