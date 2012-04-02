/****************************************************************************
 *            scriptengine.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_7220173a_dccd_4260_b982_f2909229f434
#define	UUID_7220173a_dccd_4260_b982_f2909229f434

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class ScriptEngine.
 * This file contains the @c class ScriptEngine.
 */
namespace eternal_lands
{

	class ScriptEngine: public boost::noncopyable
	{
		private:
			asIScriptEngine* m_engine;

			const FileSystemSharedPtr m_file_system;

			inline const FileSystemSharedPtr &get_file_system()
				const
			{
				return m_file_system;
			}

			void register_material_data();

		protected:
			inline asIScriptEngine* get_engine()
			{
				return m_engine;
			}

		public:
			/**
			 * Default constructor.
			 */
			ScriptEngine(const FileSystemSharedPtr &file_system);

			/**
			 * Default destructor.
			 */
			~ScriptEngine() throw();
			void init();
			void build_module(const String &name,
				const String &file);
			void run_main(const String &name, const String &file);

			asIScriptFunction* get_function(const String &name,
				const String &file, const String &function);
			asIScriptContext* get_context();

	};

}

#endif	/* UUID_7220173a_dccd_4260_b982_f2909229f434 */
