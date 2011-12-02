/**
 * @file
 * @ingroup reflections
 * @brief handles the reflection of reflective surfaces
 */
#ifndef __REFLECTION_H__
#define __REFLECTION_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * defines whether a tile is a water tile or not
 */
#define IS_WATER_TILE(i) (!i || (i>230 && i<255))

/**
 * The following macro tests if a _water tile_ is reflecting
 */
#define IS_REFLECTING(i) (i<240)

#ifdef __cplusplus
} // extern "C"
#endif

#endif
