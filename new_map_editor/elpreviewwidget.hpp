#ifndef	UUID_ff8c49df_c590_4aa4_abfe_fd687b640634
#define UUID_ff8c49df_c590_4aa4_abfe_fd687b640634

#include <scene.hpp>
#include <QtOpenGL/QGLWidget>
#include <QtGui/QKeyEvent>
#include <QVector3D>

using namespace eternal_lands;

class ELPreviewWidget: public QGLWidget
{
	Q_OBJECT

	private:
		GlobalVarsSharedPtr m_global_vars;
		FileSystemSharedPtr m_file_system;
		Scene* m_scene;
		QTimer* m_timer;
		QString m_object;
		QVector3D m_rotation;
		bool m_update_object;

		void update_object();

	protected:
		virtual void initializeGL();
		virtual void resizeGL(int width, int height);
		virtual void paintGL();

		virtual void mousePressEvent(QMouseEvent *event);
		virtual void mouseMoveEvent(QMouseEvent *event);
		virtual void wheelEvent(QWheelEvent *event);

	public:
		ELPreviewWidget(QWidget *parent = 0);
		virtual ~ELPreviewWidget();
		void set_object(const QString &name, const QVector3D &rotation);
		void set_dirs(const QStringList &dirs);

		inline const FileSystemSharedPtr &get_file_system() const
		{
			return m_file_system;
		}

};

#endif	/* UUID_ff8c49df_c590_4aa4_abfe_fd687b640634 */
