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
#ifdef MAP_EDITOR
#include "map_editor/lights.h"
#else
#include "lights.h"
#endif
#ifdef	NEW_TEXTURES
#include "image_loading.h"
#endif	/* NEW_TEXTURES */

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
int particles_percentage=100;
int enable_blood = 0;

#ifdef MAP_EDITOR2
#define MAP_EDITOR
#endif

#ifdef MAP_EDITOR
int save_particle_def(particle_sys_def *def)
{
	char cleanpath[128];
	FILE *f=NULL;

	clean_file_name(cleanpath, def->file_name, sizeof(cleanpath));

	f=open_file_data(cleanpath,"w");
	if(f == NULL){
		LOG_ERROR("%s: %s \"%s\"\n", reg_error_str, cant_open_file, cleanpath);
		return 0;
	}

	fprintf(f,"%i\n",PARTICLE_DEF_VERSION);

	// System info
	fprintf(f,"%i\n",def->part_sys_type);
	fprintf(f,"%x,%x\n",def->sblend,def->dblend);
	fprintf(f,"%i\n",def->total_particle_no);
	fprintf(f,"%i\n",def->ttl);
	fprintf(f,"%i\n",def->part_texture);
	fprintf(f,"%f\n",def->part_size);
	fprintf(f,"%i\n",def->random_func);
	// Particle creation info
	fprintf(f,"%f,%f,%f\n",def->minx,def->miny,def->minz);
	fprintf(f,"%f,%f,%f\n",def->maxx,def->maxy,def->maxz);
	fprintf(f,"%f\n",def->constrain_rad_sq);
	fprintf(f,"%f,%f,%f\n",def->vel_minx,def->vel_miny,def->vel_minz);
	fprintf(f,"%f,%f,%f\n",def->vel_maxx,def->vel_maxy,def->vel_maxz);
	fprintf(f,"%f,%f,%f,%f\n",def->minr,def->ming,def->minb,def->mina);
	fprintf(f,"%f,%f,%f,%f\n",def->maxr,def->maxg,def->maxb,def->maxa);
	// Particle update info
	fprintf(f,"%f,%f,%f\n",def->acc_minx,def->acc_miny,def->acc_minz);
	fprintf(f,"%f,%f,%f\n",def->acc_maxx,def->acc_maxy,def->acc_maxz);
	fprintf(f,"%f,%f,%f,%f\n",def->mindr,def->mindg,def->mindb,def->minda);
	fprintf(f,"%f,%f,%f,%f\n",def->maxdr,def->maxdg,def->maxdb,def->maxda);
	// Particle light info
	fprintf(f,"%i\n",def->use_light);
	fprintf(f,"%f,%f,%f\n",def->lightx,def->lighty,def->lightz);
	fprintf(f,"%f,%f,%f\n",def->lightr,def->lightg,def->lightb);

	fclose(f);
	return 1;
}
#endif
#ifdef MAP_EDITOR2
#undef MAP_EDITOR
#endif

/*******************************************************************
 *            INITIALIZATION AND CLEANUP FUNCTIONS                 *
 *******************************************************************/
//Threading support for particles_list
void init_particles ()
{
}

void end_particles ()
{
}

void destroy_all_particle_defs()
{
}

void destroy_all_particles()
{
}
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

#ifndef ELC
int have_point_sprite=0;
#endif

void display_particles()
{
}

void update_particles()
{
}

/******************************************************************************
 *                        MISC HELPER FUNCTIONS                               *
 ******************************************************************************/
#ifdef ELC
void add_teleporters_from_list (const Uint8 *teleport_list)
{
}
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

