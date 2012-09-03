/****************************************************************************
 *            utf.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
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
	std::string utf32_to_utf8(const Utf32String &str);
	Utf32String utf8_to_utf32(const std::string &str);
	std::string wstring_to_utf8(const std::wstring &str);
	std::wstring utf8_to_wstring(const std::string &str);

}

#endif	/* UUID_5ceee9d0_6bde_4053_8d0a_03dfa47109e1 */
