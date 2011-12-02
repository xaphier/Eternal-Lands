/*!
 * \file
 * \ingroup particles
 * \brief data structures and functions for the particle system
 */
#ifndef __PARTICLES_H__
#define __PARTICLES_H__

#include "platform.h"
#include "bbox_tree.h"
#include "e3d.h"
#include "threads.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \ingroup other
 * \brief Adds a fire
 *
 *      Adds a fire using the information from the server
 *
 * \param kind		the kind of fire (1: small, 2: big)
 * \param x_tile	the x value of the tile
 * \param y_tile	the y value of the tile
 *
 * \callgraph
 */
void add_fire_at_tile (int kind, Uint16 x_tile, Uint16 y_tile);

/*!
 * \ingroup other
 * \brief Removes a fire particle system at the given location
 *
 *      Removes a fire particle system at the location (x,y)
 *
 * \param x_tile	x value of the tile the fire is on
 * \param y_tile	y value of the tile the fire is on
 */
void remove_fire_at_tile (Uint16 x_tile, Uint16 y_tile);

/*!
 * \ingroup particles
 * \brief Adds a new particle system from the file given in file_name at the given position.
 *
 *      Adds a new particle system from the file given in file_name at the position (x_pos,y_pos,z_pos).
 *
 * \param file_name filename of the file that contains the particle systems description.
 * \param x_pos	x coordinate where the particle system should appear
 * \param y_pos	y coordinate where the particle system should appear
 * \param z_pos	z coordinate where the particle system should appear
 * \retval int -1 on error, -2 if the eye candy system handled, or the 
 *             index in particles_list otherwise
 * \callgraph
 */
#ifndef	MAP_EDITOR
int add_particle_sys (const char *file_name, float x_pos, float y_pos, float z_pos, unsigned int dynamic);
#else
int add_particle_sys (const char *file_name, float x_pos, float y_pos, float z_pos);
#endif

#ifdef NEW_SOUND
// Wrapper function for map particles
int add_map_particle_sys (const char *file_name, float x_pos, float y_pos, float z_pos, unsigned int dynamic);
#endif // NEW_SOUND
void add_ec_effect_to_e3d(object3d* e3d);

/*!
 * \ingroup particles
 * \brief Adds a new particle system from the given file file_name at the specified tile position.
 *
 *      Adds a new particle system from the given file file_name at the specified tile position.
 *
 * \param file_name	filename of the file that contains the particly systems definition.
 * \param x_tile	x coordinate of the tile where the particle system should be added
 * \param y_tile	y coordinate of the tile where the particle system should be added
 * \retval int -1 on error, -2 if the eye candy system handled, or the 
 *             index in particles_list otherwise
 * \callgraph
 */
#ifndef	MAP_EDITOR
int add_particle_sys_at_tile (const char *file_name, int x_tile, int y_tile, unsigned int dynamic);
#else
int add_particle_sys_at_tile (const char *file_name, int x_tile, int y_tile);
#endif

//MISC HELPER FUNCTIONS
/*!
 * \ingroup misc_utils
 * \brief Adds all teleporters from the given list.
 *
 *      Adds all teleporters from the given list.
 *
 * \param teleport_list     an array of teleporter particle systems to add
 *
 * \callgraph
 */
void add_teleporters_from_list (const Uint8 *teleport_list);

/*!
 * \ingroup loadsave
 * \brief Saves the given particle system definition to a file
 *
 *      Saves the given particle system definition to a file.
 *
 * \param def       the particle system header to save
 * \retval int
 * \callgraph
 */
#ifdef MAP_EDITOR
/*!
 * \brief Set the particle texture as the current texture
 *
 *	Set the particle texture \a i as the next texture to be drawn.
 * 
 * \param i The number of the texture file, \c part_texture field of the 
 *          particle system definition.
 */
void get_and_set_particle_texture_id (int i);

int save_particle_def(particle_sys_def *def);
#elif defined(MAP_EDITOR2)
int save_particle_def(particle_sys_def *def);
#endif

extern int use_point_particles; /*!< specifies if we use point particles or not */
extern int enable_blood; /*!< specifies whether or not to use the blood special effect in combat */

#ifdef __cplusplus
} // extern "C"
#endif

#endif // __PARTICLES_H__

