/****************************************************************************
 *            simplexnoise.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_418435c1_82ec_4907_8d54_669b5eb5b6a4
#define	UUID_418435c1_82ec_4907_8d54_669b5eb5b6a4

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class SimplexNoise.
 * This file contains the @c class SimplexNoise.
 */

namespace eternal_lands
{

/** \file
		\brief Declares the SimplexNoise1234 class for producing Perlin simplex noise.
		\author Stefan Gustavson (stegu@itn.liu.se)
*/

/*
 * This is a clean, fast, modern and free Perlin Simplex noise class in C++.
 * Being a stand-alone class with no external dependencies, it is
 * highly reusable without source code modifications.
 *
 *
 * Note:
 * Replacing the "float" type with "double" can actually make this run faster
 * on some platforms. A templatized version of SimplexNoise1234 could be useful.
 */

	class SimplexNoise
	{
		private:
			static unsigned char perm[];
			static float grad(const int hash, const float value);
			static float grad(const int hash, const glm::vec2 &value);
			static float grad(const int hash, const glm::vec3 &value);
			static float grad(const int hash, const glm::vec4 &value);

		public:
			static float noise(const float value);
			static float noise(const glm::vec2 &value);
			static float noise(const glm::vec3 &value);
			static float noise(const glm::vec4 &value);

	};

}

#endif	/* UUID_418435c1_82ec_4907_8d54_669b5eb5b6a4 */
