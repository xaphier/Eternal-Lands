/****************************************************************************
 *            half.h
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	_HALF_H_
#define	_HALF_H_

#include <SDL_types.h>

#ifdef __cplusplus
extern "C" {
#endif

float half_to_float(const Uint16 value);

Uint16 float_to_half(const float value);

#ifdef __cplusplus
} // extern "C"
#endif

#endif	/* _HALF_H_ */

