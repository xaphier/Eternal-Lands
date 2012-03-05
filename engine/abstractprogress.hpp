/****************************************************************************
 *            abstractprogress.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_58265bc6_005f_44a0_8b31_e3f525097190
#define	UUID_58265bc6_005f_44a0_8b31_e3f525097190

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class AbstractProgress.
 * This file contains the @c class AbstractProgress.
 */
namespace eternal_lands
{

	class AbstractProgress
	{
		protected:
			AbstractProgress();

		public:
			virtual ~AbstractProgress() throw();
			virtual void init(const Uint32 min_value,
				const Uint32 max_value) = 0;
			virtual void set_text(const String &str) = 0;
			virtual void stepp(const Uint32 count) = 0;
			virtual bool get_canceled() const = 0;

	};

}

#endif	/* UUID_58265bc6_005f_44a0_8b31_e3f525097190 */
