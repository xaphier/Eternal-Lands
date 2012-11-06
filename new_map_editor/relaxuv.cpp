#include "relaxuv.hpp"

RelaxUV::RelaxUV(ELGLWidget* el_gl_widget, QProgressDialog* progress_dialog,
	boost::shared_ptr<QProgress> el_progress): m_el_gl_widget(el_gl_widget),
	m_progress_dialog(progress_dialog), m_el_progress(el_progress)
{
}

RelaxUV::~RelaxUV()
{
}

void RelaxUV::run()
{
	m_el_progress->cancel(false);

	m_el_gl_widget->relax_terrain_uv(m_el_progress, 5000);

	emit done();
}
