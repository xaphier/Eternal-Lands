/****************************************************************************
 *            lua.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_4da5b58d_c873_4ef2_b11e_2b2ee7bf9cb8
#define	UUID_4da5b58d_c873_4ef2_b11e_2b2ee7bf9cb8

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "basiclua.hpp"

/**
 * @file
 * @brief The @c class Lua.
 * This file contains the @c class Lua.
 */
namespace eternal_lands
{

	class Lua: public BasicLua
	{
		public:
			Lua();
			~Lua() throw();

	};

}

#endif	/* UUID_4da5b58d_c873_4ef2_b11e_2b2ee7bf9cb8 */

