#ifndef	UUID_5c329861_9db9_456a_8500_8c0e20ca3155
#define UUID_5c329861_9db9_456a_8500_8c0e20ca3155

#include "elglwidget.hpp"
#include "qprogress.hpp"
#include <QProgressDialog>
#include <QRunnable>

class RelaxUV: public QObject, public QRunnable
{
	Q_OBJECT

	private:
		ELGLWidget* m_el_gl_widget;
		QProgressDialog* m_progress_dialog;
		boost::shared_ptr<QProgress> m_el_progress;

	public:
		RelaxUV(ELGLWidget* el_gl_widget,
			QProgressDialog* progress_dialog,
			boost::shared_ptr<QProgress> el_progress);
		virtual ~RelaxUV();
		virtual void run();

	signals:
		void done();

};

#endif	/* UUID_5c329861_9db9_456a_8500_8c0e20ca3155 */
