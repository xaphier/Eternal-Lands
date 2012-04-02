/****************************************************************************
 *            materialscriptmanager.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_129e6d33_5ed1_4d16_9393_cf824f3451cd
#define	UUID_129e6d33_5ed1_4d16_9393_cf824f3451cd

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class MaterialScriptManager.
 * This file contains the @c class MaterialScriptManager.
 */
namespace eternal_lands
{

	class MaterialScriptManager: public boost::noncopyable
	{
		private:
			const ScriptEngineWeakPtr m_script_engine;

		public:
			/**
			 * Default constructor.
			 */
			MaterialScriptManager(
				const ScriptEngineWeakPtr &script_engine);

			/**
			 * Default destructor.
			 */
			~MaterialScriptManager() throw();

	};

}

#endif	/* UUID_129e6d33_5ed1_4d16_9393_cf824f3451cd */
