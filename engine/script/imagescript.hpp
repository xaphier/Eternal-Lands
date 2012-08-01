/****************************************************************************
 *            imagescript.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_7ce59ed1_de48_43e9_8326_c219d7e5f12e
#define	UUID_7ce59ed1_de48_43e9_8326_c219d7e5f12e

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "basicscript.hpp"

/**
 * @file
 * @brief The @c class ImageScript.
 * This file contains the @c class ImageScript.
 */
namespace eternal_lands
{

	class ImageScript: public BasicScript
	{
		public:
			ImageScript(const ScriptEngineSharedPtr &script_engine,
				const String &name);
			~ImageScript() noexcept;
			bool execute(const ImageSharedPtr &image,
				const Uint16 face, const Uint16 mipmap,
				asIScriptContext* context);

	};

}

#endif	/* UUID_7ce59ed1_de48_43e9_8326_c219d7e5f12e */
