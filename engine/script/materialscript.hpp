/****************************************************************************
 *            materialscript.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_b114765b_0a36_423f_971b_64efd2cb81fe
#define	UUID_b114765b_0a36_423f_971b_64efd2cb81fe

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "basicscript.hpp"

/**
 * @file
 * @brief The @c class MaterialScript.
 * This file contains the @c class MaterialScript.
 */
namespace eternal_lands
{

	class MaterialScript: public BasicScript
	{
		private:
			SDL_mutex* m_mutex;

		public:
			MaterialScript(
				const ScriptEngineSharedPtr &script_engine,
				const String &name);
			~MaterialScript() noexcept;
			bool execute(const glm::vec4 &time,
				MaterialData &material,
				asIScriptContext* context);
			void lock();
			void unlock();

	};

}

#endif	/* UUID_b114765b_0a36_423f_971b_64efd2cb81fe */
