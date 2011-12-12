/****************************************************************************
 *            utf.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_5ceee9d0_6bde_4053_8d0a_03dfa47109e1
#define	UUID_5ceee9d0_6bde_4053_8d0a_03dfa47109e1

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 */
namespace eternal_lands
{

	std::string utf8_to_string(const std::string &str);
	std::string string_to_utf8(const std::string &str);
	Utf32String utf8_to_utf32(const std::string &str);

}

#endif	/* UUID_5ceee9d0_6bde_4053_8d0a_03dfa47109e1 */
