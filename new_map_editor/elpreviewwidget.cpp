#include "elpreviewwidget.hpp"
#include <exceptions.hpp>
#include <objectdescription.hpp>
#include <logging.hpp>
#include <globalvars.hpp>
#include <filesystem.hpp>
#include <meshbuilder.hpp>
#include <meshdatacache.hpp>
#include <meshdatatool.hpp>
#include <boundingbox.hpp>
#include <lightdata.hpp>
#include <QtCore/QTimer>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/transform2.hpp>
#include <QTimer>

ELPreviewWidget::ELPreviewWidget(QWidget* parent): QGLWidget(parent),
	m_global_vars(boost::make_shared<GlobalVars>()),
	m_file_system(boost::make_shared<FileSystem>())
{
	m_timer = new QTimer(this);
	connect(m_timer, SIGNAL(timeout()), this, SLOT(updateGL()));
}

ELPreviewWidget::~ELPreviewWidget()
{
	delete m_scene;
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

void ELPreviewWidget::set_dirs(const QStringList &dirs)
{
	m_file_system->clear();

	m_file_system->add_dir(String(""));

	BOOST_FOREACH(const QString &dir, dirs)
	{
		m_file_system->add_dir(String(dir.toUtf8()));
	}
}

void ELPreviewWidget::initializeGL()
{
#ifdef OSX
	/* Test if that helps .... */
	glewExperimental = GL_TRUE;
#endif	/* OSX */
	glewInit();

	if (GLEW_VERSION_3_0)
	{
		m_global_vars->set_opengl_version(ovt_3_0);
	}

	if (GLEW_VERSION_3_1)
	{
		m_global_vars->set_opengl_version(ovt_3_1);
	}

	if (GLEW_VERSION_3_2)
	{
		m_global_vars->set_opengl_version(ovt_3_2);
	}

	if (GLEW_VERSION_3_3)
	{
		m_global_vars->set_opengl_version(ovt_3_3);
	}

	m_global_vars->set_optmize_shader_source(false);

	m_global_vars->set_view_distance(250.0f);

	m_scene = new Scene(m_global_vars, m_file_system);

	m_scene->set_lights(true);
	m_scene->set_ambient(glm::vec3(0.2f));
	m_scene->add_light(LightData(glm::vec3(0.0f, -5.0f, 0.0f),
		glm::vec3(1.0f), 10.0f, 0));

	m_timer->start(500);
}

void ELPreviewWidget::resizeGL(int width, int height)
{
	glViewport(0, 0, width, height);

	m_scene->set_view_port(glm::uvec4(0, 0, width, height));
	m_scene->set_perspective(40.0f, static_cast<float>(width) /
		static_cast<float>(height));
	m_scene->set_z_near(1.5f);

	updateGL();
}

void ELPreviewWidget::paintGL()
{
	glm::mat4 matrix;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	matrix = glm::lookAt(glm::vec3(0.0f, -2.0f, 1.0f),
		glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	m_scene->set_view_matrix(matrix);

	if (m_update_object)
	{
		update_object();
		m_update_object = false;
	}

	m_scene->cull();
	m_scene->draw();
}

void ELPreviewWidget::set_object(const QString &name, const QVector3D &rotation)
{
	m_object = name;
	m_rotation = rotation;
	m_update_object = true;
}

void ELPreviewWidget::update_object()
{
	try
	{
		Transformation world_transformation;
		BoundingBox bounding_box;
		MeshDataToolSharedPtr mesh_data_tool;
		String name;
		float max;

		m_scene->remove_object(0);

		name = String(m_object.toUtf8().data());

		if (!name.get().empty())
		{
			m_scene->get_scene_resources().get_mesh_data_cache(
				)->get_mesh_data(name, mesh_data_tool);

			mesh_data_tool->update_sub_meshs_bounding_box();
			mesh_data_tool->get_bounding_box(Transformation(),
				bounding_box);

			max = std::max(std::max(bounding_box.get_half_size().x,
				bounding_box.get_half_size().y),
				bounding_box.get_half_size().z) * 2.0f + 0.5f;

			world_transformation.set_rotation_angles(glm::vec3(
				m_rotation.x(), m_rotation.y(),
				m_rotation.z()));

			world_transformation.set_scale(1.0f / max);
			world_transformation.set_translation(
				-bounding_box.get_center() / max);

			m_scene->add_object(ObjectDescription(
				world_transformation, StringVector(),
				name, 1.0f, 0, st_select, bt_disabled));
		}
	}
	catch (...)
	{
	}
}
