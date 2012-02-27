#ifndef	UUID_ff8c49df_c590_4aa4_abfe_fd687b640634
#define UUID_ff8c49df_c590_4aa4_abfe_fd687b640634

#include <scene.hpp>
#include "editor/editorobjectdata.hpp"
#include <QtOpenGL/QGLWidget>
#include <QtGui/QKeyEvent>

class ELPreviewWidget: public QGLWidget
{
	Q_OBJECT

	private:
		QTimer *m_timer;
		eternal_lands::Scene m_scene;
		eternal_lands::MeshObjectData m_mesh_object_data;
		eternal_lands::GlobalId m_id;
		float m_rotate;
		bool m_object;

		void update_view_matrix();

	private slots:
		void update_rotation();

	protected:
		virtual void initializeGL();
		virtual void resizeGL(int width, int height);
		virtual void paintGL();

		virtual void mousePressEvent(QMouseEvent *event);
		virtual void mouseMoveEvent(QMouseEvent *event);
		virtual void wheelEvent(QWheelEvent *event);

	public:
		ELPreviewWidget(const QGLWidget *shareWidget = 0, QWidget *parent = 0);
		~ELPreviewWidget();
		void set_object(const QString &object);

};

#endif	/* UUID_ff8c49df_c590_4aa4_abfe_fd687b640634 */
