/****************************************************************************
 *            lua.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "lua.hpp"

namespace eternal_lands
{

	Lua::Lua(): BasicLua(luaL_newstate())
	{
	}

	Lua::~Lua() throw()
	{
		close();
	}

}

