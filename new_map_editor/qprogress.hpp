/****************************************************************************
 *            qprogress.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_73bc38a3_79f3_42c6_846e_a9bfa66faa88
#define	UUID_73bc38a3_79f3_42c6_846e_a9bfa66faa88

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "abstractprogress.hpp"
#include <QObject>

/**
 * @file
 * @brief The @c class QProgress.
 * This file contains the @c class QProgress.
 */
using namespace eternal_lands;

class QProgress: public QObject, public AbstractProgress
{
	Q_OBJECT

	private:
		Uint32 m_value;
		bool m_canceled;

	public:
		QProgress();
		virtual ~QProgress() noexcept;
		virtual void init(const Uint32 min_value,
			const Uint32 max_value);
		virtual void set_text(const String &str);
		virtual void stepp(const Uint32 count);
		virtual bool get_canceled() const;

	public slots:
		void cancel(const bool value = true);

	signals:
		void set_range(const int min_value, const int max_value);
		void set_value(const int value);
		void set_string(const QString &str);
		void start();

};

#endif	/* UUID_73bc38a3_79f3_42c6_846e_a9bfa66faa88 */
