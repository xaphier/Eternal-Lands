#include "elglwidget.hpp"
#include <sceneresources.hpp>
#include <filesystem.hpp>
#include <globalvars.hpp>
#include <image.hpp>
#include <lightdata.hpp>
#include "editor/editorobjectdescription.hpp"
#include <QFileDialog>
#include <QMessageBox>
#include <QVector3D>
#include <QVector2D>
#include <QTimer>
#include <QImage>

ELGLWidget::ELGLWidget(QWidget *parent): QGLWidget(parent)
{
	m_select = false;
	m_select_depth = false;
	m_zoom = 10.0f;
	m_terrain_editing = false;
	m_terrain_type_index = 0;
	m_terrain_layer_index = 0;
	m_light = false;
	m_rotate_z = 0.0f;
	m_blend = bt_disabled;

	m_global_vars = boost::make_shared<GlobalVars>();
	m_file_system = boost::make_shared<FileSystem>();

	m_timer = new QTimer(this);
	connect(m_timer, SIGNAL(timeout()), this, SLOT(updateGL()));
}

ELGLWidget::~ELGLWidget()
{
}

QImage ELGLWidget::get_icon(const QString &name)
{
	ImageSharedPtr image;
	QImage result(32, 32, QImage::Format_ARGB32);
	glm::vec4 color;
	Uint32 x, y, height, width, mipmap, i, count;

	try
	{
		image = m_editor->get_image(String(name.toUtf8()));

		width = image->get_width();
		height = image->get_height();
		count = image->get_mipmap_count();
		mipmap = 0;

		for (i = 0; i < count; ++i)
		{
			if ((width <= 32) && (height <= 32))
			{
				break;
			}

			width = std::max(width / 2, 1u);
			height = std::max(height / 2, 1u);
			mipmap++;
		}

		result = QImage(std::max(width, 32u), std::max(height, 32u),
			QImage::Format_ARGB32);

		for (y = 0; y < height; ++y)
		{
			for (x = 0; x < width; ++x)
			{
				color = image->get_pixel(x, y, 0, 0, mipmap);

				result.setPixel(x, y, QColor::fromRgbF(color[0],
					color[1], color[2], color[3]).rgba());
			}
		}
	}
	catch (...)
	{
	}

	return result.scaled(32, 32, Qt::KeepAspectRatio,
		Qt::SmoothTransformation);
}

void ELGLWidget::mouse_click_action()
{
	m_editor->update_edit_id();

	if (get_terrain_editing())
	{
		emit terrain_edit();

		m_editor->update_edit_id();

		return;
	}

	if (m_object)
	{
		m_editor->add_3d_object(m_world_position, m_object_name,
			st_select);
		emit update_object(false);
		emit can_undo(m_editor->get_can_undo());

		return;
	}

	if (m_light)
	{
		m_editor->add_light(m_world_position, m_light_radius);
		emit update_light(false);
		emit can_undo(m_editor->get_can_undo());

		return;
	}
}

void ELGLWidget::mouse_move_action()
{
	EditorObjectDescription object_description;
	LightData light_data;
	glm::vec3 position;

	if (get_terrain_editing())
	{
		emit terrain_edit();

		m_editor->update_edit_id();

		return;
	}

	if (!m_grab_world_position_valid)
	{
		m_grab_world_position_valid = true;
		m_grab_world_position = m_world_position;

		switch (m_editor->get_renderable())
		{
			default:
			case rt_none:
				break;
			case rt_light:
				m_editor->get_light_data(light_data);

				m_move_offset = light_data.get_position();
				break;
			case rt_object:
				m_editor->get_object_description(
					object_description);

				m_move_offset =
					object_description.get_translation();
				break;
		}

		return;
	}

	if (m_grab_world_position_valid)
	{
		position = m_move_offset + m_world_position -
			m_grab_world_position;

		switch (m_editor->get_renderable())
		{
			case rt_light:
				m_editor->set_light_position(position);
				emit can_undo(m_editor->get_can_undo());
				emit update_light(true);
				break;
			case rt_object:
				m_editor->set_object_translation(position);
				emit can_undo(m_editor->get_can_undo());
				emit update_object(true);
				break;
			case rt_none:
			default:
				break;
		}
	}
}

void ELGLWidget::mouseReleaseEvent(QMouseEvent *event)
{
	if (event->button() != m_click_button)
	{
	}
}

void ELGLWidget::mousePressEvent(QMouseEvent *event)
{
	if (event->button() != m_click_button)
	{
		return;
	}

	if ((event->x() < 0) || (event->y() < 0) || (event->x() >= width())
		|| (event->y() >= height()))
	{
		return;
	}

	m_select_pos.x = event->x();
	m_select_pos.y = height() - event->y();

	m_select = !get_terrain_editing();
	m_select_depth = true;
	m_mouse_click_action = true;
	m_grab_world_position_valid = false;

	updateGL();
}

void ELGLWidget::mouseMoveEvent(QMouseEvent *event)
{
	if (!event->buttons().testFlag(m_click_button))
	{
		m_grab_world_position_valid = false;

		return;
	}

	if ((event->x() < 0) || (event->y() < 0) || (event->x() >= width())
		|| (event->y() >= height()))
	{
		return;
	}

	m_select_pos.x = event->x();
	m_select_pos.y = height() - event->y();
	m_select_depth = true;
	m_mouse_move_action = true;

	updateGL();
}

void ELGLWidget::change_terrain_values(const QVector3D &data,
	const QVector2D &size, const float attenuation_size, const int mask,
	const int attenuation, const int shape, const int effect)
{
	m_editor->change_terrain_values(m_world_position,
		glm::vec3(data.x(), data.y(), data.z()),
		glm::bvec3((mask & 1) != 0, (mask & 2) != 0, (mask & 4) != 0),
		glm::vec2(size.x(), size.y()), attenuation_size, attenuation,
		shape, effect);

	emit can_undo(m_editor->get_can_undo());
}

void ELGLWidget::terrain_layer_edit(const int terrain_layer_index,
	const float strength, const float radius, const int brush_type)
{
	m_editor->terrain_layer_edit(m_world_position, terrain_layer_index,
		strength, radius, brush_type);

	emit can_undo(m_editor->get_can_undo());
}

void ELGLWidget::ground_tile_edit(const int tile)
{
	m_editor->ground_tile_edit(m_world_position, tile);

	emit can_undo(m_editor->get_can_undo());
}

void ELGLWidget::water_tile_edit(const int water)
{
	m_editor->water_tile_edit(m_world_position, water);

	emit can_undo(m_editor->get_can_undo());
}

void ELGLWidget::height_edit(const int height)
{
	m_editor->height_edit(m_world_position, height);

	emit can_undo(m_editor->get_can_undo());
}

void ELGLWidget::wheelEvent(QWheelEvent *event)
{
	if (event->modifiers().testFlag(m_wheel_zoom_x10))
	{
		m_zoom += event->delta() / 5.0f * (m_swap_wheel_zoom ? -1.0f : 1.0f);
	}
	else
	{
		m_zoom += event->delta() / 50.0f * (m_swap_wheel_zoom ? -1.0f : 1.0f);
	}

	m_zoom = std::max(1.0f, std::min(50.0f, m_zoom));

	update_ortho();
}

void ELGLWidget::initializeGL()
{
	m_pos = glm::vec3(10.0f, 10.0f, 0.0f);
	m_half_size[0] = 1;
	m_half_size[1] = 1;

#ifdef OSX
	/* Test if that helps .... */
	glewExperimental = GL_TRUE;
#endif	/* OSX */
	glewInit();

	if (!GLEW_VERSION_2_1)
	{
		BoostFormat format_string(UTF8("OpenGL 2.1 needed, but '%1%'"
			" found!"));

		format_string % (const char*)glGetString(GL_VERSION);

		QMessageBox::critical(0, "Error", QString::fromUtf8(
			format_string.str().c_str()));

		exit(1);
	}

	m_global_vars->set_opengl_version(ovt_2_1);

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
	m_global_vars->set_view_distance(550.0f);

	m_global_vars->set_shadow_quality(sqt_no);
	m_global_vars->set_shadow_map_size(2);
	m_global_vars->set_clipmap_size(2048);
	m_global_vars->set_clipmap_world_size(8);
	m_global_vars->set_clipmap_slices(16);
	m_global_vars->set_fog(false);
	m_global_vars->set_use_simd(true);
	m_global_vars->set_use_s3tc_for_actors(true);
	m_global_vars->set_use_block(true);
	m_global_vars->set_use_in_out(true);
	m_global_vars->set_use_functions(false);
	m_global_vars->set_low_quality_terrain(false);
	m_global_vars->set_use_multisample_shadows(false);
	m_global_vars->set_effect_debug(false);
	m_global_vars->set_use_scene_fbo(false);

	m_editor.reset(new Editor(m_global_vars, m_file_system));
	m_editor->set_z_near(1.5f);

	m_timer->start(17);

	emit initialized();
}

void ELGLWidget::update_ortho()
{
	m_editor->set_ortho(glm::vec4(-m_aspect, m_aspect, -1.0f,
		1.0f) * m_zoom);
}

void ELGLWidget::resizeGL(int width, int height)
{
	glViewport(0, 0, width, height);

	m_editor->set_view_port(glm::uvec4(0, 0, width, height));

	m_aspect = 1.0f;

	if ((height * width) > 0)
	{
		m_aspect = static_cast<float>(width) /
			static_cast<float>(height);
	}

	update_ortho();

	updateGL();
}

void ELGLWidget::paintGL()
{
	glm::mat4 view;
	glm::ivec4 view_port;
	glm::vec3 dir, pos;
	double selected_depth;
	Uint32 id;

	glEnable(GL_CULL_FACE);
	glDisable(GL_ALPHA_TEST);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	dir = glm::normalize(glm::vec3(0.0f, -1.0f, 2.0f));

	pos = m_pos;

	dir = m_rotate * dir * 200.0f;

	view = glm::lookAt(pos + dir, pos, glm::normalize(glm::vec3(0.0f, 0.0f, 1.0f)));
	m_editor->set_view_matrix(view);
	m_editor->set_focus(pos);
	m_editor->set_depth_selection(m_select_pos);

	id = m_editor->get_id();

	m_editor->draw();

	if (m_select)
	{
		m_select = false;

		m_editor->select(m_select_pos, m_half_size);

		switch (m_editor->get_renderable())
		{
			default:
			case rt_none:
				emit deselect();
				break;
			case rt_light:
				emit update_light(true);
				break;
			case rt_object:
				emit update_object(true);
				break;
		}
	}

	if (m_select_depth)
	{
		m_select_depth = false;

		if (m_editor->get_object_selected() &&
			(m_editor->get_id() != id))
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			m_editor->draw();
		}

		selected_depth = m_editor->get_depth();

		view_port[0] = 0;
		view_port[1] = 0;
		view_port[2] = width();
		view_port[3] = height();

		m_world_position = glm::unProject(glm::dvec3(m_select_pos,
			selected_depth), glm::dmat4(view),
			glm::dmat4(m_editor->get_projection_matrix()),
			view_port);
	}

	if (m_mouse_click_action)
	{
		m_mouse_click_action = false;

		mouse_click_action();
	}

	if (m_mouse_move_action)
	{
		m_mouse_move_action = false;

		mouse_move_action();
	}
}

void ELGLWidget::get_object_description(
	EditorObjectDescription &object_description) const
{
	m_editor->get_object_description(object_description);
}

RenderableType ELGLWidget::get_renderable() const
{
	return m_editor->get_renderable();
}

void ELGLWidget::get_light_data(LightData &light) const
{
	m_editor->get_light_data(light);
}

void ELGLWidget::remove_object()
{
	switch (m_editor->get_renderable())
	{
		case rt_object:
			m_editor->remove_object();
			emit can_undo(m_editor->get_can_undo());
			m_select = false;
			break;
		case rt_light:
			m_editor->remove_light();
			emit can_undo(m_editor->get_can_undo());
			m_select = false;
			break;
		case rt_particle:
//			m_editor->remove_particle();
			emit can_undo(m_editor->get_can_undo());
			m_select = false;
			break;
		default:
			break;
	}
}

void ELGLWidget::set_object_blend(const BlendType value)
{
	m_editor->set_object_blend(value);
	emit can_undo(m_editor->get_can_undo());
}

void ELGLWidget::set_object_transparency(const float value)
{
	m_editor->set_object_transparency(value);
	emit can_undo(m_editor->get_can_undo());
}

void ELGLWidget::set_object_translation(const glm::vec3 &translation)
{
	m_editor->set_object_translation(translation);
	emit can_undo(m_editor->get_can_undo());
}

void ELGLWidget::set_object_rotation(const glm::vec3 &rotation)
{
	m_editor->set_object_rotation(rotation);
	emit can_undo(m_editor->get_can_undo());
}

void ELGLWidget::set_object_scale(const float scale)
{
	m_editor->set_object_scale(scale);
	emit can_undo(m_editor->get_can_undo());
}

void ELGLWidget::remove_light()
{
	m_editor->remove_light();
	emit can_undo(m_editor->get_can_undo());
}

void ELGLWidget::set_light_position(const glm::vec3 &position)
{
	m_editor->set_light_position(position);
	emit can_undo(m_editor->get_can_undo());
}

void ELGLWidget::set_light_radius(const double radius)
{
	m_editor->set_light_radius(radius);
	emit can_undo(m_editor->get_can_undo());
}

void ELGLWidget::set_light_color(const glm::vec3 &color)
{
	m_editor->set_light_color(color);
	emit can_undo(m_editor->get_can_undo());
}

void ELGLWidget::undo()
{
	bool result;

	result = m_editor->undo();

	emit can_undo(m_editor->get_can_undo());

	if (result)
	{
		emit deselect();
	}
	else
	{
		switch (m_editor->get_renderable())
		{
			default:
			case rt_none:
			case rt_particle:
				break;
			case rt_object:
				emit update_object(false);
				break;
			case rt_light:
				emit update_light(false);
				break;
		}
	}
}

void ELGLWidget::set_wire_frame(const bool enabled)
{
	if (enabled)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

void ELGLWidget::set_dungeon(const bool dungeon)
{
	m_editor->set_dungeon(dungeon);
	emit can_undo(m_editor->get_can_undo());
}

bool ELGLWidget::get_dungeon() const
{
	return m_editor->get_dungeon();
}

void ELGLWidget::set_ambient(const glm::vec3 &color)
{
	m_editor->set_ambient(color);
	emit can_undo(m_editor->get_can_undo());
}

const glm::vec3 &ELGLWidget::get_ambient() const
{
	return m_editor->get_ambient();
}

void ELGLWidget::move_left()
{
	m_pos += m_rotate * glm::vec3(-5.0f, 0.0f, 0.0f);
}

void ELGLWidget::move_right()
{
	m_pos += m_rotate * glm::vec3(5.0f, 0.0f, 0.0f);
}

void ELGLWidget::move_up()
{
	m_pos += m_rotate * glm::vec3(0.0f, 5.0f, 0.0f);
}

void ELGLWidget::move_down()
{
	m_pos += m_rotate * glm::vec3(0.0f, -5.0f, 0.0f);
}

void ELGLWidget::rotate_left()
{
	m_rotate_z -= 5.0f;

	if (m_rotate_z < 0.0f)
	{
		m_rotate_z += 360.0f;
	}

	m_rotate = glm::mat3(glm::rotate(m_rotate_z, glm::vec3(0.0f, 0.0f, 1.0f)));
}

void ELGLWidget::rotate_right()
{
	m_rotate_z += 5.0f;

	if (m_rotate_z >= 360.0f)
	{
		m_rotate_z -= 360.0f;
	}

	m_rotate = glm::mat3(glm::rotate(m_rotate_z, glm::vec3(0.0f, 0.0f, 1.0f)));
}

void ELGLWidget::zoom_in()
{
	m_zoom -= 0.5f;
	m_zoom = std::max(1.0f, std::min(50.0f, m_zoom));

	update_ortho();
}

void ELGLWidget::zoom_out()
{
	m_zoom += 0.5f;
	m_zoom = std::max(1.0f, std::min(50.0f, m_zoom));

	update_ortho();
}

void ELGLWidget::add_object(const String &object)
{
	m_light = false;
	m_object = true;
	m_object_name = object;
	m_light_radius = 0.0f;
}

void ELGLWidget::add_light(const float radius)
{
	m_light = true;
	m_object = false;
	m_object_name = String("");
	m_light_radius = radius;
}

QStringList ELGLWidget::get_terrain_albedo_maps() const
{
	QStringList result;

	result << QString::fromUtf8(m_editor->get_terrain_albedo_map(0).get(
		).c_str());
	result << QString::fromUtf8(m_editor->get_terrain_albedo_map(1).get(
		).c_str());
	result << QString::fromUtf8(m_editor->get_terrain_albedo_map(2).get(
		).c_str());
	result << QString::fromUtf8(m_editor->get_terrain_albedo_map(3).get(
		).c_str());

	return result;
}

QString ELGLWidget::get_terrain_height_map() const
{
	return QString::fromUtf8(m_editor->get_terrain_vector_map().get(
		).c_str());
}

QString ELGLWidget::get_terrain_blend_map() const
{
	return QString::fromUtf8(m_editor->get_terrain_blend_map(0).get(
		).c_str());
}

QString ELGLWidget::get_terrain_dudv_map() const
{
	return QString::fromUtf8(m_editor->get_terrain_dudv_map().get(
		).c_str());
}

void ELGLWidget::new_map(const int map_size_x, const int map_size_y, const int blend_image_size_x,
	const int blend_image_size_y, const QStringList textures)
{
/*
	MaterialData terrain_material;
	Uint16Array2 terrain_size;
	Uint16Array2 blend_imgae_size;
	int i;

	for (i = 0; i < 4; i++)
	{
		if (textures.size() > i)
		{
			terrain_material.set_diffuse_texture(textures[i].toStdString(), i);
		}

		if (textures.size() > (i + 4))
		{
			terrain_material.set_normal_texture(textures[i + 4].toStdString(), i);
		}
	}

	terrain_material.set_type("terrain");

	terrain_size[0] = map_size_x;
	terrain_size[1] = map_size_y;
	blend_imgae_size[0] = blend_image_size_x;
	blend_imgae_size[1] = blend_image_size_y;

	m_editor->set_terrain(terrain_material, terrain_size, blend_imgae_size);

	emit can_undo(m_editor->get_can_undo());
*/
}

void ELGLWidget::new_map(const QString &image, const int blend_image_size_x,
	const int blend_image_size_y, const QStringList textures)
{
/*
	MaterialData terrain_material;
	Uint16Array2 blend_imgae_size;
	int i;

	for (i = 0; i < 4; i++)
	{
		if (textures.size() > i)
		{
			terrain_material.set_diffuse_texture(textures[i].toStdString(), i);
		}

		if (textures.size() > (i + 4))
		{
			terrain_material.set_normal_texture(textures[i + 4].toStdString(), i);
		}
	}

	terrain_material.set_type("terrain");

	blend_imgae_size[0] = blend_image_size_x;
	blend_imgae_size[1] = blend_image_size_y;

	m_editor->set_terrain(terrain_material, image.toStdString(), blend_imgae_size);

	emit can_undo(m_editor->get_can_undo());
*/
}

void ELGLWidget::set_terrain_editing(const bool enabled)
{
	m_terrain_editing = enabled;
}

void ELGLWidget::set_terrain_type_index(const int index)
{
	m_terrain_type_index = std::max(index, 0);
}

void ELGLWidget::set_terrain_layer_index(const int index)
{
	m_terrain_layer_index = std::max(index, 0);
}

void ELGLWidget::open_map(const QString &file_name)
{
	if (!file_name.isEmpty())
	{
		m_editor->load_map(String(file_name.toUtf8()));

		emit can_undo(m_editor->get_can_undo());
	}
}

void ELGLWidget::set_dirs(const QStringList &dirs)
{
	m_file_system->clear();

	m_file_system->add_dir(String(""));

	BOOST_FOREACH(const QString &dir, dirs)
	{
		m_file_system->add_dir(String(dir.toUtf8()));
	}

	m_dirs = dirs;
}

void ELGLWidget::set_fog(const glm::vec3 &color, const float density)
{
//	m_editor->get_scene().set_fog(color, density);
}

void ELGLWidget::set_terrain_albedo_map(const QString &name, const Uint32 index)
{
	m_editor->set_terrain_albedo_map(String(name.toUtf8()), index);
	emit can_undo(m_editor->get_can_undo());
}

void ELGLWidget::set_object_selection(const SelectionType selection)
{
	m_editor->set_object_selection(selection);
	emit can_undo(m_editor->get_can_undo());
}

void ELGLWidget::set_random_translation_x(const bool value)
{
	m_editor->set_random_translation(value, 0);
}

void ELGLWidget::set_random_translation_y(const bool value)
{
	m_editor->set_random_translation(value, 1);
}

void ELGLWidget::set_random_translation_z(const bool value)
{
	m_editor->set_random_translation(value, 2);
}

void ELGLWidget::set_random_translation_x_min(const double value)
{
	m_editor->set_random_translation_min(value, 0);
}

void ELGLWidget::set_random_translation_y_min(const double value)
{
	m_editor->set_random_translation_min(value, 1);
}

void ELGLWidget::set_random_translation_z_min(const double value)
{
	m_editor->set_random_translation_min(value, 2);
}

void ELGLWidget::set_random_translation_x_max(const double value)
{
	m_editor->set_random_translation_max(value, 0);
}

void ELGLWidget::set_random_translation_y_max(const double value)
{
	m_editor->set_random_translation_max(value, 1);
}

void ELGLWidget::set_random_translation_z_max(const double value)
{
	m_editor->set_random_translation_max(value, 2);
}

void ELGLWidget::set_random_rotation_x(const bool value)
{
	m_editor->set_random_rotation(value, 0);
}

void ELGLWidget::set_random_rotation_y(const bool value)
{
	m_editor->set_random_rotation(value, 1);
}

void ELGLWidget::set_random_rotation_z(const bool value)
{
	m_editor->set_random_rotation(value, 2);
}

void ELGLWidget::set_random_rotation_x_min(const double value)
{
	m_editor->set_random_rotation_min(value, 0);
}

void ELGLWidget::set_random_rotation_y_min(const double value)
{
	m_editor->set_random_rotation_min(value, 1);
}

void ELGLWidget::set_random_rotation_z_min(const double value)
{
	m_editor->set_random_rotation_min(value, 2);
}

void ELGLWidget::set_random_rotation_x_max(const double value)
{
	m_editor->set_random_rotation_max(value, 0);
}

void ELGLWidget::set_random_rotation_y_max(const double value)
{
	m_editor->set_random_rotation_max(value, 1);
}

void ELGLWidget::set_random_rotation_z_max(const double value)
{
	m_editor->set_random_rotation_max(value, 2);
}

void ELGLWidget::set_random_scale(const bool value)
{
	m_editor->set_random_scale(value);
}

void ELGLWidget::set_random_scale_min(const double value)
{
	m_editor->set_random_scale_min(value / 100.0f);
}

void ELGLWidget::set_random_scale_max(const double value)
{
	m_editor->set_random_scale_max(value / 100.0f);
}

void ELGLWidget::disable_object()
{
	m_object = false;
	m_object_name = String("");
}

void ELGLWidget::disable_light()
{
	m_light = false;
	m_light_radius = 0.0f;
}

void ELGLWidget::save(const QString &name) const
{
	m_editor->save(String(name.toUtf8()));
}

QString ELGLWidget::get_blend_image_name() const
{
//	return QString::fromStdString(m_editor->get_blend_image_name());
	return QString();
}

void ELGLWidget::set_blend_image_name(const QString &blend_image)
{
//	m_editor->set_blend_image_name(String(blend_image.toUtf8()));
}

void ELGLWidget::set_game_minute(const int game_minute)
{
//	m_editor->get_scene().set_game_minute(game_minute);
}

glm::vec3 ELGLWidget::get_light_color() const
{
	LightData light;

	get_light_data(light);

	return light.get_color();
}

void ELGLWidget::set_draw_objects(const bool draw_objects)
{
	m_editor->set_draw_objects(draw_objects);
}

void ELGLWidget::set_draw_terrain(const bool draw_terrain)
{
	m_editor->set_draw_terrain(draw_terrain);
}

void ELGLWidget::set_draw_lights(const bool draw_lights)
{
	m_editor->set_draw_lights(draw_lights);
}

void ELGLWidget::set_draw_light_spheres(const bool draw_light_spheres)
{
	m_editor->set_draw_light_spheres(draw_light_spheres);
}

void ELGLWidget::set_lights_enabled(const bool enabled)
{
	m_editor->set_lights_enabled(enabled);
}

QStringList ELGLWidget::get_materials() const
{
	StringVector materials;
	QStringList result;

	materials = m_editor->get_materials();

	BOOST_FOREACH(const String &material, materials)
	{
		result.push_back(QString::fromUtf8(material.get().c_str()));
	}

	return result;
}

QStringList ELGLWidget::get_default_materials(const String &name) const
{
	StringVector materials;
	QStringList result;

	materials = m_editor->get_default_materials(name);

	BOOST_FOREACH(const String &material, materials)
	{
		result.push_back(QString::fromUtf8(material.get().c_str()));
	}

	return result;
}

void ELGLWidget::set_object_materials(const StringVector &materials)
{
	m_editor->set_object_materials(materials);
	emit can_undo(m_editor->get_can_undo());
}

/*
void ELGLWidget::get_codecs(QStringList &codecs)
{
	StringVector tmp;

	codecs.clear();

	SceneResources::get_codec_manager().get_supported_codecs(tmp);

	BOOST_FOREACH(const String &codec, tmp)
	{
		codecs.append(QString::fromStdString(codec));
	}
}

void ELGLWidget::get_file_extensions_filter(QString &filter)
{
	StringVector codecs;
	StringVector extensions;

	filter.clear();

	SceneResources::get_codec_manager().get_supported_file_extensions(extensions);

	BOOST_FOREACH(const String &extension, extensions)
	{
		if (!filter.isEmpty())
		{
			filter += tr(" ");
		}
		else
		{
			filter = tr("all image formats (");
		}

		filter += tr("*.") + QString::fromStdString(extension);
	}

	filter += tr(")");

	SceneResources::get_codec_manager().get_supported_codecs(codecs);

	BOOST_FOREACH(const String &codec, codecs)
	{
		QString tmp;

		SceneResources::get_codec_manager().get_supported_file_extensions(extensions,
			codec);

		BOOST_FOREACH(const String &extension, extensions)
		{
			if (!tmp.isEmpty())
			{
				tmp += tr(" ");
			}
			else
			{
				tmp = QString::fromStdString(codec) + tr(" image (");
			}

			tmp += tr("*.") + QString::fromStdString(extension);
		}

		tmp += tr(")");

		filter += tr(";;") + tmp;
	}
}

void ELGLWidget::get_file_extensions_filter(QString &filter, QString &default_extension,
	const QString &codec)
{
	StringVector extensions;

	filter.clear();

	SceneResources::get_codec_manager().get_supported_file_extensions(extensions,
		codec.toStdString());

	BOOST_FOREACH(const String &extension, extensions)
	{
		if (!filter.isEmpty())
		{
			filter += tr(" ");
		}
		else
		{
			filter = codec + tr(" image (");
			default_extension = QString::fromStdString(extension);
		}

		filter += tr("*.") + QString::fromStdString(extension);
	}

	filter += tr(")");
}

void ELGLWidget::export_blend_image(const QString &file_name, const QString &codec) const
{
	m_editor->export_blend_image(file_name.toUtf8(), codec.toUtf8());
}

void ELGLWidget::export_terrain_map(const QString &file_name, const QString &codec) const
{
	m_editor->export_terrain_map(file_name.toUtf8(), codec.toUtf8());
}

void ELGLWidget::import_terrain_map(const QString &file_name)
{
	m_editor->import_terrain_map(file_name.toUtf8());

	emit can_undo(m_editor->get_can_undo());
}
*/

void ELGLWidget::set_debug_mode(const int value)
{
	m_global_vars->set_effect_debug(value >= 0);
	m_editor->set_debug_mode(std::max(value, 0));
}

void ELGLWidget::init_terrain(const int width, const int height)
{
	m_editor->init_terrain(glm::uvec2(width, height));
}

QStringList ELGLWidget::get_debug_modes() const
{
	StringVector debug_modes;
	QStringList result;

	debug_modes = m_editor->get_debug_modes();

	BOOST_FOREACH(const String &debug_mode, debug_modes)
	{
		result.push_back(QString::fromUtf8(debug_mode.get().c_str()));
	}

	return result;
}

float ELGLWidget::get_terrain_offset_x() const
{
	return Editor::get_terrain_offset().x;
}

float ELGLWidget::get_terrain_offset_y() const
{
	return Editor::get_terrain_offset().y;
}

float ELGLWidget::get_terrain_offset_z() const
{
	return Editor::get_terrain_offset().z;
}

float ELGLWidget::get_terrain_offset_min_x() const
{
	return Editor::get_terrain_offset_min().x;
}

float ELGLWidget::get_terrain_offset_min_y() const
{
	return Editor::get_terrain_offset_min().y;
}

float ELGLWidget::get_terrain_offset_min_z() const
{
	return Editor::get_terrain_offset_min().z;
}

float ELGLWidget::get_terrain_offset_max_x() const
{
	return Editor::get_terrain_offset_max().x;
}

float ELGLWidget::get_terrain_offset_max_y() const
{
	return Editor::get_terrain_offset_max().y;
}

float ELGLWidget::get_terrain_offset_max_z() const
{
	return Editor::get_terrain_offset_max().z;
}
