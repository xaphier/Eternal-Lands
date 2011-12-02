/****************************************************************************
 *            xmlutils.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_6f4e179c_3e00_4fc2_b256_d2cd4c5108c9
#define	UUID_6f4e179c_3e00_4fc2_b256_d2cd4c5108c9

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include <gtk/gtk.h>

/**
 * @file
 * @brief The @c class XmlUtils.
 * This file contains the @c class XmlUtils.
 */
namespace eternal_lands
{

	class XmlUtils
	{
		public:
			static void load(const gchar* name);

	};

}

#endif	/* UUID_6f4e179c_3e00_4fc2_b256_d2cd4c5108c9 */
