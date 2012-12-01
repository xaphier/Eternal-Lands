#ifndef	UUID_aa89effe_d26e_4b8d_931c_d277d3d3daca
#define UUID_aa89effe_d26e_4b8d_931c_d277d3d3daca

#include "elglwidget.hpp"
#include "qprogress.hpp"
#include <QProgressDialog>
#include <QRunnable>

class SaveMap: public QObject, public QRunnable
{
	Q_OBJECT

	private:
		QString m_name;
		ELGLWidget* m_el_gl_widget;
		QProgressDialog* m_progress_dialog;
		boost::shared_ptr<QProgress> m_el_progress;

	public:
		SaveMap(const QString &name, ELGLWidget* el_gl_widget,
			QProgressDialog* progress_dialog,
			boost::shared_ptr<QProgress> el_progress);
		virtual ~SaveMap();
		virtual void run();

	signals:
		void done();

};

#endif	/* UUID_aa89effe_d26e_4b8d_931c_d277d3d3daca */
