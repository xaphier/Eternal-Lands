#include "savemap.hpp"

SaveMap::SaveMap(const QString &name, ELGLWidget* el_gl_widget,
	QProgressDialog* progress_dialog,
	boost::shared_ptr<QProgress> el_progress): m_name(name),
	m_el_gl_widget(el_gl_widget), m_progress_dialog(progress_dialog),
	m_el_progress(el_progress)
{
}

SaveMap::~SaveMap()
{
}

void SaveMap::run()
{
	m_el_progress->cancel(false);

	m_el_gl_widget->save(m_el_progress, m_name);

	emit done();
}
