/****************************************************************************
 *            qprogress.hpp
 *
 * Author: 2010  Daniel Jungmann <dsj@gmx.net>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_73bc38a3_79f3_42c6_846e_a9bfa66faa88
#define	UUID_73bc38a3_79f3_42c6_846e_a9bfa66faa88

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "progress.hpp"
#include <QObject>

/**
 * @file
 * @brief The @c class QProgress.
 * This file contains the @c class QProgress.
 */

using namespace eternal_lands;

class QProgress: public QObject, public Progress
{
	Q_OBJECT

	private:
		Uint32 m_value;

		virtual void do_init(const Uint32 min_value, const Uint32 max_value);
		virtual void do_set_text(const String &str);
		virtual void do_stepp(const Uint32 count);
		virtual bool do_get_canceled() const;

	public:
		QProgress();
		virtual ~QProgress();

	signals:
		void set_range(const int min_value, const int max_value);
		void set_value(const int value);
		void set_string(const QString &str);

};

#endif	/* UUID_73bc38a3_79f3_42c6_846e_a9bfa66faa88 */
