/****************************************************************************
 *            cubemapfaceutil.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_8688bb11_f1af_4a74_97d5_3379de559f2c
#define	UUID_8688bb11_f1af_4a74_97d5_3379de559f2c

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class CubeMapFaceUtil.
 * This file contains the @c class CubeMapFaceUtil.
 */
namespace eternal_lands
{

	/**
	 * Types of cube map faces.
	 * @{
	 */
	enum CubeMapFaceType
	{
		cmft_negative_x = 0,
		cmft_negative_y,
		cmft_negative_z,
		cmft_positive_x,
		cmft_positive_y,
		cmft_positive_z
	};
	/**
	 * @}
	 */

	/**
	 * @brief @c class for cubemap faces.
	 *
	 * @c class for cubemap faces.
	 */
	class CubeMapFaceUtil
	{
		public:
			static const String &get_str(
				const CubeMapFaceType cube_map_face);
			static CubeMapFaceType get_cube_map_face(
				const String &str);
			static GLenum get_gl_type(
				const CubeMapFaceType cube_map_face);
			static Uint32 get_cube_map_face_count() noexcept;

	};

	OutStream &operator<<(OutStream &str, const CubeMapFaceType value);
	InStream &operator>>(InStream &str, CubeMapFaceType &value);

}

#endif	/* UUID_8688bb11_f1af_4a74_97d5_3379de559f2c */
