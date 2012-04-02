/****************************************************************************
 *            basicscript.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_965ffa87_5bf4_404d_aaf3_195c779fce16
#define	UUID_965ffa87_5bf4_404d_aaf3_195c779fce16

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "angelscript/include/angelscript.h"

/**
 * @file
 * @brief The @c class BasicScript.
 * This file contains the @c class BasicScript.
 */
namespace eternal_lands
{

	class BasicScript
	{
		private:
			asIScriptFunction* m_func;

		protected:
			/**
			 * Default constructor.
			 */
			BasicScript();
			void set_function(const String &module,
				const String &function,
				asIScriptEngine* engine);
			void prepare(asIScriptContext* ctx);
			bool execute(asIScriptContext* ctx);

		public:
			/**
			 * Default destructor.
			 */
			~BasicScript() throw();

	};

}

#endif	/* UUID_965ffa87_5bf4_404d_aaf3_195c779fce16 */
