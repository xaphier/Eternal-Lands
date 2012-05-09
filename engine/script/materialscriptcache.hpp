/****************************************************************************
 *            materialscriptcache.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_7b71a487_18dd_4e8f_aee9_902a293d6698
#define	UUID_7b71a487_18dd_4e8f_aee9_902a293d6698

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class MaterialScriptCache.
 * This file contains the @c class MaterialScriptCache.
 */
namespace eternal_lands
{

	class MaterialScriptCache: public boost::noncopyable
	{
		private:
			typedef std::map<String, MaterialScriptSharedPtr>
				StringMaterialScriptSharedPtrMap;

			StringMaterialScriptSharedPtrMap m_material_scripts;
			const ScriptEngineWeakPtr m_script_engine;

			inline ScriptEngineSharedPtr get_script_engine() const
			{
				ScriptEngineSharedPtr result;

				result = m_script_engine.lock();

				assert(result.get() != nullptr);

				return result;
			}

		public:
			/**
			 * Default constructor.
			 */
			MaterialScriptCache(
				const ScriptEngineWeakPtr &script_engine);

			/**
			 * Default destructor.
			 */
			~MaterialScriptCache() noexcept;

			const MaterialScriptSharedPtr &get_material_script(
				const String &name);

	};

}

#endif	/* UUID_7b71a487_18dd_4e8f_aee9_902a293d6698 */
