/****************************************************************************
 *            qprogress.hpp
 *
 * Author: 2010  Daniel Jungmann <dsj@gmx.net>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "qprogress.hpp"

QProgress::QProgress()
{
	m_value = 0;
}

QProgress::~QProgress()
{

}

void QProgress::do_init(const Uint32 min_value, const Uint32 max_value)
{
	m_value = min_value;

	emit set_range(min_value, max_value);
}

void QProgress::do_set_text(const String &str)
{
	emit set_string(QString::fromStdString(str));
}

void QProgress::do_stepp(const Uint32 count)
{
	m_value += count;

	emit set_value(m_value);
}

bool QProgress::do_get_canceled() const
{
	return false;
}
