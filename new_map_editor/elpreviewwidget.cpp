#include "elpreviewwidget.hpp"
#include <exceptions.hpp>
#include <QtCore/QTimer>

ELPreviewWidget::ELPreviewWidget(const QGLWidget *shareWidget, QWidget *parent):
	QGLWidget(parent, shareWidget)
{
	m_rotate = 0.0f;
	m_object = false;

	m_timer = new QTimer(this);
	QObject::connect(m_timer, SIGNAL(timeout()), this, SLOT(update_rotation()));
	m_timer->start(50);
}

ELPreviewWidget::~ELPreviewWidget()
{
}

void ELPreviewWidget::mousePressEvent(QMouseEvent *event)
{
}

void ELPreviewWidget::mouseMoveEvent(QMouseEvent *event)
{
}

void ELPreviewWidget::wheelEvent(QWheelEvent *event)
{
}

void ELPreviewWidget::initializeGL()
{
	eternal_lands::Uint32Array2 size;

	m_scene.load_resources("md/default_arb.xml");
	m_scene.init();

	size[0] = 1024;
	size[1] = 1024;

	m_scene.clear();;
	m_scene.add_map(0, size);
	update_view_matrix();
}

void ELPreviewWidget::resizeGL(int width, int height)
{
	try
	{
		m_scene.set_projectiv_frustum(60.0f, width, height, 2.5f, 15.0f);
	}
	CATCH_AND_LOG()
}

void ELPreviewWidget::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	try
	{
		m_scene.draw();
	}
	CATCH_AND_LOG()
}

void ELPreviewWidget::update_view_matrix()
{
	glm::mat4 matrix;
	glm::vec3 pos, dir, up;

	pos = glm::vec3(0.0f, -7.5f, 2.5f);
	dir = glm::normalize(-pos);
	dir[2] = 0.0f;
	up = glm::vec3(0.0f, 0.0f, 1.0f);

	matrix = glm::lookAt(pos, dir, up);

	m_scene.set_view_matrix(matrix);
}

void ELPreviewWidget::set_object(const QString &object)
{
	try
	{
		glm::vec3 translate, tmp;
		eternal_lands::Uint32Array2 size;
		float scale;

		m_object = false;

		m_scene.clear();

		if (!object.isEmpty())
		{
			eternal_lands::ScenePageReadWriteIntrusivePtr scene_page_read_write;

			size[0] = 1024;
			size[1] = 1024;

			m_scene.add_map(0, size);

			m_scene.get_scene_page_read_write(0, scene_page_read_write);

			m_mesh_object_data.set_mesh(object.toStdString());

			m_id = scene_page_read_write->add_object(m_mesh_object_data);

			tmp = scene_page_read_write->get_bounding_box().get_half_size();
			scale = 1.0f / std::max(tmp[0], std::max(tmp[1], tmp[2]));
			translate = -scene_page_read_write->get_bounding_box().get_center() / scale;

			m_mesh_object_data.set_translation(translate);
			m_mesh_object_data.set_scale(scale);
			m_mesh_object_data.set_rotation(glm::vec3(0.0f, 0.0f, -m_rotate));

			scene_page_read_write->modify_object(m_id.get_id(), m_mesh_object_data);
		}

		m_object = !object.isEmpty();

	}
	CATCH_AND_LOG()

	updateGL();
}

void ELPreviewWidget::update_rotation()
{
	m_rotate += 0.5f;

	if (m_rotate > 360.0f)
	{
		m_rotate -= 360.0f;
	}

	if (m_object)
	{
		m_mesh_object_data.set_rotation(glm::vec3(0.0f, 0.0f, -m_rotate));

		{
			eternal_lands::ScenePageReadWriteIntrusivePtr scene_page_read_write;

			m_scene.get_scene_page_read_write(m_id.get_page_id(),
				scene_page_read_write);

			scene_page_read_write->modify_object(m_id.get_id(), m_mesh_object_data);
		}

		m_scene.set_view_changed();

		updateGL();
	}
}
