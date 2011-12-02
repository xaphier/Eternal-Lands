/*!
 * \file
 * \ingroup shadows
 * \brief Handles the handling of shadows.
 */
#ifndef __SHADOWS_H__
#define __SHADOWS_H__

#include "platform.h"

#ifdef __cplusplus
extern "C" {
#endif

extern float sun_position[4];

extern int shadows_on; /*!< flag indicating whether shadows are enabled or disabled */
extern int is_day; /*!< this flag shows whether it's day or night */
extern int use_shadow_mapping; /*!< flag whether to use shadow mapping or not */

#ifdef __cplusplus
} // extern "C"
#endif

#endif
