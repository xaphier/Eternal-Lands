/****************************************************************************
 *            globalvars.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_8462a53b_dda9_4345_9d1b_b0baf4406ee4
#define	UUID_8462a53b_dda9_4345_9d1b_b0baf4406ee4

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class GlobalVars.
 * This file contains the @c class GlobalVars.
 */
namespace eternal_lands
{

	class GlobalVars
	{
		private:
			#define GLOBAL_VAR_INT(name, group, description_short, description_long, def, min, max)	\
				int m_##name;
			#define GLOBAL_VAR_BOOL(name, group, description_short, description_long, def)	\
				bool m_##name;
			#define GLOBAL_VAR_FLOAT(name, group, description_short, description_long, def, min, max, step)	\
				float m_##name;
			#define GLOBAL_VAR_STRING(name, group, description_short, description_long, def)	\
				String m_##name;
			#define GLOBAL_VAR_ENUM(name, group, description_short, description_long, def, list)	\
				String m_##name;
			#include "globalvarslist.txt"
			#undef GLOBAL_VAR_INT
			#undef GLOBAL_VAR_BOOL
			#undef GLOBAL_VAR_FLOAT
			#undef GLOBAL_VAR_STRING
			#undef GLOBAL_VAR_ENUM
		public:
			GlobalVars();
			~GlobalVars() throw();

			#define GLOBAL_VAR_INT(name, group, description_short, description_long, def, min, max)	\
				inline Sint64 get_##name() const	\
				{	\
					return m_##name;	\
				}
			#define GLOBAL_VAR_BOOL(name, group, description_short, description_long, def)	\
				inline bool get_##name() const	\
				{	\
					return m_##name;	\
				}
			#define GLOBAL_VAR_FLOAT(name, group, description_short, description_long, def, min, max, step)	\
				inline float get_##name() const	\
				{	\
					return m_##name;	\
				}
			#define GLOBAL_VAR_STRING(name, group, description_short, description_long, def)	\
				inline const String &get_##name() const	\
				{	\
					return m_##name;	\
				}
			#define GLOBAL_VAR_ENUM(name, group, description_short, description_long, def, list)	\
				inline const String &get_##name() const	\
				{	\
					return m_##name;	\
				}
			#include "globalvarslist.txt"
			#undef GLOBAL_VAR_INT
			#undef GLOBAL_VAR_BOOL
			#undef GLOBAL_VAR_FLOAT
			#undef GLOBAL_VAR_STRING
			#undef GLOBAL_VAR_ENUM

			#define GLOBAL_VAR_INT(name, group, description_short, description_long, def, min, max)	\
				inline void set_##name(const Sint64 value)	\
				{	\
					m_##name = value;	\
				}
			#define GLOBAL_VAR_BOOL(name, group, description_short, description_long, def)	\
				inline void set_##name(const bool value)	\
				{	\
					m_##name = value;	\
				}
			#define GLOBAL_VAR_FLOAT(name, group, description_short, description_long, def, min, max, step)	\
				inline void set_##name(const float value)	\
				{	\
					m_##name = value;	\
				}
			#define GLOBAL_VAR_STRING(name, group, description_short, description_long, def)	\
				inline void set_##name(const String &value)	\
				{	\
					m_##name = value;	\
				}
			#define GLOBAL_VAR_ENUM(name, group, description_short, description_long, def, list)	\
				inline void set_##name(const String &value)	\
				{	\
					m_##name = value;	\
				}
			#include "globalvarslist.txt"
			#undef GLOBAL_VAR_INT
			#undef GLOBAL_VAR_BOOL
			#undef GLOBAL_VAR_FLOAT
			#undef GLOBAL_VAR_STRING
			#undef GLOBAL_VAR_ENUM

	};

}

#endif	/* UUID_8462a53b_dda9_4345_9d1b_b0baf4406ee4 */
