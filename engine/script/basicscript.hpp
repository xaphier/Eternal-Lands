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

	class BasicScript: public boost::noncopyable
	{
		private:
			asIScriptFunction* m_func;
			const String m_name;

		protected:
			/**
			 * Default constructor.
			 */
			BasicScript(const String &name);
			void set_function(const String &module,
				const String &function,
				const ScriptEngineSharedPtr &script_engine);
			void prepare(asIScriptContext* context);
			void log_exception(asIScriptContext* context);

		public:
			/**
			 * Default destructor.
			 */
			~BasicScript() noexcept;

			inline const String &get_name() const
			{
				return m_name;
			}

	};

}

#endif	/* UUID_965ffa87_5bf4_404d_aaf3_195c779fce16 */
