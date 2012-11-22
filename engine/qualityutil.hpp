/****************************************************************************
 *            qualityutil.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_70652e26_e4e4_4455_9170_574709caec46
#define	UUID_70652e26_e4e4_4455_9170_574709caec46

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class QualityUtil.
 * This file contains the @c class QualityUtil.
 */
namespace eternal_lands
{

	enum QualityType
	{
		qt_low = 0,
		qt_medium,
		qt_high
	};

	class QualityUtil
	{
		public:
			static const String &get_str(const QualityType quality);
			static QualityType get_quality(const String &str);
			static Uint32 get_quality_count() noexcept;

	};

	OutStream &operator<<(OutStream &str, const QualityType value);
	InStream &operator>>(InStream &str, QualityType &value);

}

#endif	/* UUID_70652e26_e4e4_4455_9170_574709caec46 */
