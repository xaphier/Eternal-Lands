#include "elglwidget.hpp"
#include <sceneresources.hpp>
#include <filesystem.hpp>
#include <globalvars.hpp>
#include <logging.hpp>
#include <image.hpp>
#include <lightdata.hpp>
#include "editor/editorobjectdescription.hpp"
#include <QFileDialog>
#include <QMessageBox>
#include <QVector3D>
#include <QVector2D>
#include <QTimer>
#include <QImage>
#include <QIcon>

ELGLWidget::ELGLWidget(QWidget *parent): QGLWidget(parent)
{
	m_mouse_move = kpt_nothing;
	m_select = false;
	m_select_depth = false;
	m_zoom = 10.0f;
	m_terrain_editing = false;
	m_terrain_type_index = 0;
	m_terrain_layer_index = 0;
	m_object_adding = false;
	m_light_adding = false;
	m_blend = bt_disabled;
	m_camera_roll = 45.0f;
	m_camera_yaw = 0.0f;

	m_global_vars = boost::make_shared<GlobalVars>();
	m_file_system = boost::make_shared<FileSystem>();

	init_logging("log", true);

	m_timer = new QTimer(this);
	connect(m_timer, SIGNAL(timeout()), this, SLOT(updateGL()));

	m_rotate_x_key = Qt::Key_X;
	m_rotate_y_key = Qt::Key_Y;
	m_rotate_z_key = Qt::Key_Z;
	m_scale_key = Qt::Key_R;
	set_press_button(Qt::NoButton);
	set_click_button(Qt::LeftButton);
	set_grab_button(Qt::RightButton);
	set_view_rotate_button(Qt::MidButton);
}

ELGLWidget::~ELGLWidget()
{
}

void ELGLWidget::get_albedo_map_data(const QString &name,
	const QSize &icon_size, const QSize &image_size, QIcon &icon,
	bool &use_blend_size_sampler, bool &ok)
{
	ImageSharedPtr image;
	QImage tmp;
	QString message, format_str, valid_formats_str;
	TextureFormatType format;
	glm::vec4 color;
	Sint32 x, y, height, width, depth, mipmap, i, count;

	try
	{
		image = m_editor->get_image(String(name.toUtf8()), format);

		format_str = QString::fromUtf8(TextureFormatUtil::get_str(
			format).get().c_str());

		valid_formats_str = "{";
		valid_formats_str += QString::fromUtf8(
			TextureFormatUtil::get_str(tft_rgb_dxt1).get(
				).c_str());
		valid_formats_str += ", ";
		valid_formats_str += QString::fromUtf8(
			TextureFormatUtil::get_str(tft_rgba_dxt1).get(
				).c_str());
		valid_formats_str += ", ";
		valid_formats_str += QString::fromUtf8(
			TextureFormatUtil::get_str(tft_rgba_dxt5).get(
				).c_str());
		valid_formats_str = "}";

		width = image->get_width();
		height = image->get_height();
		depth = image->get_depth();
		count = image->get_mipmap_count() + 1;
		mipmap = 0;

		if ((width != image_size.width()) ||
			(height != image_size.height()) || (depth != 0))
		{
			QMessageBox::critical(this, tr("Error"), QString(tr(
				"File '%1' has wrong size of <%2, %3, %4> "
				"instead of <%5, %6, 0>")).arg(name).arg(
					width).arg(height).arg(depth).arg(
					image_size.width()).arg(
					image_size.height()));

			ok = false;

			return;
		}

		if ((format == tft_rgb_dxt1) || (format == tft_rgba_dxt1))
		{
			use_blend_size_sampler = false;
		}
		else
		{
			if (format == tft_rgba_dxt5)
			{
				use_blend_size_sampler = true;
			}
			else
			{
				QMessageBox::critical(this, tr("Error"),
					QString(tr("File '%1' has wrong format"
						" %2, only formats %3 "
						"supported")).arg(name).arg(
						format_str).arg(
						valid_formats_str));

				ok = false;

				return;
			}
		}

		for (i = 0; i < count; ++i)
		{
			if ((width <= icon_size.width()) &&
				(height <= icon_size.height()))
			{
				break;
			}

			width = std::max(width / 2, 1);
			height = std::max(height / 2, 1);
			mipmap++;
		}

		tmp = QImage(width, height, QImage::Format_ARGB32);

		for (y = 0; y < height; ++y)
		{
			for (x = 0; x < width; ++x)
			{
				color = image->get_pixel(x, y, 0, 0, mipmap);

				tmp.setPixel(x, y, QColor::fromRgbF(color[0],
					color[1], color[2], color[3]).rgba());
			}
		}

		icon = QIcon(QPixmap::fromImage(tmp.scaled(icon_size.width(),
			icon_size.height(), Qt::KeepAspectRatio,
			Qt::SmoothTransformation)));

		ok = true;
	}
	catch (const boost::exception &exception)
	{
		message = "";

		if (boost::get_error_info<errinfo_message>(exception) != 0)
		{
			message = QString::fromUtf8(boost::get_error_info<
				errinfo_message>(exception)->c_str());
		}

		QMessageBox::critical(this, tr("Error"), QString(tr(
			"Error '%1' reading file '%2'")).arg(message).arg(
			name));
		ok = false;
	}
	catch (const std::exception &exception)
	{
		QMessageBox::critical(this, tr("Error"), QString(tr(
			"Error '%1' reading file '%2'")).arg(
				exception.what()).arg(name));
		ok = false;
	}
	catch (...)
	{
		QMessageBox::critical(this, tr("Error"), QString(tr(
			"Error reading file '%1'")).arg(name));
		ok = false;
	}
}

void ELGLWidget::get_extra_map_data(const QString &name,
	const QSize &image_size, bool &ok)
{
	ImageSharedPtr image;
	QString message, format_str, valid_formats_str;
	glm::uvec3 size;
	TextureFormatType format;
	Uint16 mipmaps;
	bool cube_map, array;

	try
	{
		m_editor->get_image_data(String(name.toUtf8()), format, size,
			mipmaps, cube_map, array);

		format_str = QString::fromUtf8(TextureFormatUtil::get_str(
			format).get().c_str());
		valid_formats_str = QString::fromUtf8(
			TextureFormatUtil::get_str(tft_rg_rgtc2).get(
				).c_str());

		if ((size.x != image_size.width()) ||
			(size.y != image_size.height()) || (size.z != 0))
		{
			QMessageBox::critical(this, tr("Error"), QString(tr(
				"File '%1' has wrong size of <%2, %3, %4> "
				"instead of <%5, %6, 0>")).arg(name).arg(
					size.x).arg(size.y).arg(size.z).arg(
					image_size.width()).arg(
					image_size.height()));

			ok = false;

			return;
		}

		if (format != tft_rg_rgtc2)
		{
			QMessageBox::critical(this, tr("Error"),
				QString(tr("File '%1' has wrong format"
					" %2, only formats %3 supported")).arg(
					name).arg(format_str).arg(
					valid_formats_str));

			ok = false;

			return;
		}

		ok = true;
	}
	catch (const boost::exception &exception)
	{
		message = "";

		if (boost::get_error_info<errinfo_message>(exception) != 0)
		{
			message = QString::fromUtf8(boost::get_error_info<
				errinfo_message>(exception)->c_str());
		}

		QMessageBox::critical(this, tr("Error"), QString(tr(
			"Error '%1' reading file '%2'")).arg(message).arg(
			name));
		ok = false;
	}
	catch (const std::exception &exception)
	{
		QMessageBox::critical(this, tr("Error"), QString(tr(
			"Error '%1' reading file '%2'")).arg(
				exception.what()).arg(name));
		ok = false;
	}
	catch (...)
	{
		QMessageBox::critical(this, tr("Error"), QString(tr(
			"Error reading file '%1'")).arg(name));
		ok = false;
	}
}

void ELGLWidget::get_image_data(const QString &name, QSize &size, bool &ok)
{
	QString message;
	glm::uvec3 image_size;
	TextureFormatType format;
	Uint16 mipmaps;
	bool cube_map, array;

	try
	{
		m_editor->get_image_data(String(name.toUtf8()), format,
			image_size, mipmaps, cube_map, array);

		size.setWidth(image_size.x);
		size.setHeight(image_size.y);

		ok = true;
	}
	catch (const boost::exception &exception)
	{
		message = "";

		if (boost::get_error_info<errinfo_message>(exception) != 0)
		{
			message = QString::fromUtf8(boost::get_error_info<
				errinfo_message>(exception)->c_str());
		}

		QMessageBox::critical(this, tr("Error"), QString(tr(
			"Error '%1' reading file '%2'")).arg(message).arg(
			name));
		ok = false;
	}
	catch (const std::exception &exception)
	{
		QMessageBox::critical(this, tr("Error"), QString(tr(
			"Error '%1' reading file '%2'")).arg(
				exception.what()).arg(name));
		ok = false;
	}
	catch (...)
	{
		QMessageBox::critical(this, tr("Error"), QString(tr(
			"Error reading file '%1'")).arg(name));
		ok = false;
	}
}

void ELGLWidget::mouse_click_action()
{
	m_editor->update_edit_id();

	if (get_terrain_editing())
	{
		emit terrain_edit();

		return;
	}

	if (get_object_adding())
	{
		m_editor->add_3d_object(m_world_position, m_object_name,
			st_select);
		emit update_object(false);
		emit can_undo(m_editor->get_can_undo());

		return;
	}

	if (get_light_adding())
	{
		m_editor->add_light(m_world_position, m_light_radius);
		emit update_light(false);
		emit can_undo(m_editor->get_can_undo());

		return;
	}
}

void ELGLWidget::keyPressEvent(QKeyEvent *event)
{
	if (event->key() == m_rotate_x_key)
	{
		m_mouse_move = kpt_rotate_x;
	}

	if (event->key() == m_rotate_y_key)
	{
		m_mouse_move = kpt_rotate_y;
	}

	if (event->key() == m_rotate_z_key)
	{
		m_mouse_move = kpt_rotate_z;
	}

	if (event->key() == m_scale_key)
	{
		m_mouse_move = kpt_scale;
	}
}

void ELGLWidget::keyReleaseEvent(QKeyEvent *event)
{
	if (event->key() == m_rotate_x_key)
	{
		if (m_mouse_move == kpt_rotate_x)
		{
			m_mouse_move = kpt_nothing;
		}
	}

	if (event->key() == m_rotate_y_key)
	{
		if (m_mouse_move == kpt_rotate_y)
		{
			m_mouse_move = kpt_nothing;
		}
	}

	if (event->key() == m_rotate_z_key)
	{
		if (m_mouse_move == kpt_rotate_z)
		{
			m_mouse_move = kpt_nothing;
		}
	}

	if (event->key() == m_scale_key)
	{
		if (m_mouse_move == kpt_scale)
		{
			m_mouse_move = kpt_nothing;
		}
	}
}

void ELGLWidget::mouse_move_action()
{
	EditorObjectDescription object_description;
	LightData light_data;
	glm::vec3 position, rotation;
	glm::vec2 dir;
	Uint16 rotate_index;
	bool rotating, scaling;

	if (get_terrain_editing())
	{
		emit terrain_edit();

		return;
	}

	rotate_index = 0;
	rotating = false;
	scaling = false;

	switch (m_mouse_move)
	{
		case kpt_nothing:
			break;
		case kpt_rotate_x:
			rotating = true;
			rotate_index = 0;
			break;
		case kpt_rotate_y:
			rotating = true;
			rotate_index = 1;
			break;
		case kpt_rotate_z:
			rotating = true;
			rotate_index = 2;
			break;
		case kpt_scale:
			scaling = true;
			break;
	}

	if (rotating)
	{
		dir = glm::normalize(glm::vec2(m_select_pos) -
			glm::vec2(m_selected_screen_position));

		switch (m_editor->get_renderable())
		{
			default:
			case rt_none:
			case rt_particle:
				break;
			case rt_light:
				break;
			case rt_object:
				get_object_description(object_description);

				rotation =
					object_description.get_rotation_angles(
						);

				rotation[rotate_index] = glm::degrees(
					glm::atan(dir.x, dir.y));

				if (get_invert_z_rotation())
				{
					rotation.z = 360.0f - rotation.z;
				}

				m_editor->set_object_rotation(rotation);
				emit can_undo(m_editor->get_can_undo());
				emit update_object(true);

				break;
		}

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
			case rt_particle:
				break;
			case rt_light:
				m_editor->get_light_data(light_data);

				m_move_offset = light_data.get_position();
				break;
			case rt_object:
				get_object_description(object_description);

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
			default:
			case rt_none:
			case rt_particle:
				break;
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
		}
	}
}

void ELGLWidget::mouseReleaseEvent(QMouseEvent *event)
{
	if (event->button() == get_press_button())
	{
		set_press_button(Qt::NoButton);
	}

	if (event->button() != get_click_button())
	{
		m_selection_rect.z = 0;
		m_selection_rect.w = 0;
	}
}

void ELGLWidget::mousePressEvent(QMouseEvent *event)
{
	if (!get_select_rect_event(event) && !get_click_event(event) &&
		!get_grab_event(event) && !get_view_rotate_event(event))
	{
		return;
	}

	if ((event->x() < 0) || (event->y() < 0) || (event->x() >= width())
		|| (event->y() >= height()))
	{
		return;
	}

	set_press_button(event->button());

	m_press_screen_position.x = event->x();
	m_press_screen_position.y = height() - event->y();
	m_press_pos = m_pos;
	m_camera_yaw_roll = glm::vec2(m_camera_yaw, m_camera_roll);

	if (get_select_rect_event(event))
	{
		m_select_pos.x = event->x();
		m_select_pos.y = height() - event->y();

		m_select = false;
		m_select_depth = false;
		m_mouse_click_action = false;
		m_grab_world_position_valid = false;

		return;
	}

	if (!get_click_event(event))
	{
		return;
	}

	m_select_pos.x = event->x();
	m_select_pos.y = height() - event->y();

	m_select = !(get_terrain_editing() || get_object_adding() || get_light_adding());
	m_select_depth = true;
	m_mouse_click_action = true;
	m_grab_world_position_valid = false;
	m_selection_rect.z = 0;
	m_selection_rect.w = 0;

	updateGL();
}

void ELGLWidget::mouseMoveEvent(QMouseEvent *event)
{
	glm::uvec2 min, max, pos;
	glm::vec2 move, tmp;

	if (!event->buttons().testFlag(get_press_button()))
	{
		return;
	}

	if ((event->x() < 0) || (event->y() < 0) || (event->x() >= width())
		|| (event->y() >= height()))
	{
		return;
	}

	pos.x = event->x();
	pos.y = height() - event->y();

	move = m_press_screen_position;
	move -= pos;

	move /= glm::vec2(width(), height());

	if (get_grab_event(event))
	{
		m_pos = m_press_pos + m_camera_yaw_rotate *
			glm::vec3(move * 2.5f * m_zoom, 0.0f);
		return;
	}

	if (get_view_rotate_event(event))
	{
		tmp = m_camera_yaw_roll + move * 90.0f;
		tmp.y = glm::clamp(tmp.y, 0.0f, 90.0f);

		while (tmp.x < -180.0f)
		{
			tmp.x += 360.0f;
		}

		while (tmp.x > 180.0f)
		{
			tmp.x -= 360.0f;
		}

		emit changed_camera_yaw(tmp.x);
		emit changed_camera_roll(tmp.y);

		rotate_yaw(tmp.x);
		rotate_roll(tmp.y);
		return;
	}

	if (get_select_rect_event(event))
	{
		min = glm::min(m_select_pos, pos);
		max = glm::max(m_select_pos, pos);

		m_selection_rect.x = min.x;
		m_selection_rect.y = min.y;
		m_selection_rect.z = max.x - min.x;
		m_selection_rect.w = max.y - min.y;

		return;
	}

	if (!get_click_event(event))
	{
		m_grab_world_position_valid = false;

		return;
	}

	m_select_pos.x = event->x();
	m_select_pos.y = height() - event->y();
	m_select_depth = true;
	m_mouse_move_action = true;
	m_selection_rect.z = 0;
	m_selection_rect.w = 0;

	updateGL();
}

void ELGLWidget::change_terrain_displacement_values(const QVector3D &data,
	const QVector2D &size, const float attenuation_size, const int mask,
	const int attenuation, const int shape, const int effect)
{
	m_editor->change_terrain_displacement_values(m_world_position,
		glm::vec3(data.x(), data.y(), data.z()),
		glm::bvec3((mask & 1) != 0, (mask & 2) != 0, (mask & 4) != 0),
		glm::vec2(size.x(), size.y()), attenuation_size, attenuation,
		shape, effect);

	emit can_undo(m_editor->get_can_undo());
}

void ELGLWidget::change_terrain_blend_values(const QVector2D &size,
	const float attenuation_size, const float data,
	const int attenuation, const int shape, const int effect,
	const int layer)
{
	m_editor->change_terrain_blend_values(m_world_position,
		glm::vec2(size.x(), size.y()), attenuation_size, data,
		attenuation, shape, effect, layer);

	emit can_undo(m_editor->get_can_undo());
}

void ELGLWidget::ground_tile_edit(const int tile)
{
	m_editor->ground_tile_edit(m_world_position, tile);

	emit can_undo(m_editor->get_can_undo());
}

void ELGLWidget::water_tile_edit(const int water)
{
	m_editor->water_tile_edit(m_start_position, m_world_position, water);

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

	m_zoom = std::max(1.0f, std::min(75.0f, m_zoom));

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
	m_global_vars->set_clipmap_terrain_size(2048);
	m_global_vars->set_clipmap_terrain_world_size(8);
	m_global_vars->set_clipmap_terrain_slices(16);
	m_global_vars->set_fog(false);
	m_global_vars->set_use_simd(true);
	m_global_vars->set_use_s3tc_for_actors(true);
	m_global_vars->set_use_block(true);
	m_global_vars->set_use_in_out(true);
	m_global_vars->set_use_functions(false);
	m_global_vars->set_low_quality_terrain(false);
	m_global_vars->set_use_multisample_shadows(false);
	m_global_vars->set_effect_debug(false);
	m_global_vars->set_use_scene_fbo(true);
	m_global_vars->set_use_cpu_rasterizer(false);

	m_editor.reset(new Editor(m_global_vars, m_file_system));
	m_editor->set_z_near(1.5f);

	m_timer->start(100);

	emit initialized();
}

void ELGLWidget::update_ortho()
{
	m_editor->set_ortho(glm::vec4(-m_aspect, m_aspect, -1.0f,
		1.0f) * m_zoom);
	m_editor->set_z_near(-150.0f);
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
	EditorObjectDescription object_description;
	LightData light_data;
	glm::mat4 view;
	glm::mat3 rotate;
	glm::ivec4 view_port;
	glm::vec3 dir, pos, selected_position;
	double selected_depth;
	Uint32 id;

	glEnable(GL_CULL_FACE);
	glDisable(GL_ALPHA_TEST);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	dir = glm::normalize(glm::vec3(0.0f, 0.0f, 1.0f));

	rotate = glm::mat3(glm::rotate(90.0f - m_camera_roll,
		glm::vec3(1.0f, 0.0f, 0.0f)));

	rotate = m_camera_yaw_rotate * rotate;

	pos = m_pos;

	dir = rotate * dir * m_zoom;

	if (m_camera_roll == 90)
	{
		view = glm::lookAt(pos + dir, pos,
			m_camera_yaw_rotate * glm::vec3(0.0f, 1.0f, 0.0f));
	}
	else
	{
		view = glm::lookAt(pos + dir, pos, glm::vec3(0.0f, 0.0f, 1.0f));
	}

	m_editor->set_view_matrix(view);
	m_editor->set_focus(pos);
	m_editor->set_depth_selection(m_select_pos);

	id = m_editor->get_id();

	m_editor->draw();

	if ((m_selection_rect.z != 0) || (m_selection_rect.w != 0))
	{
		m_editor->draw_selection(m_selection_rect);
	}

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
			case rt_particle:
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

		m_start_position = glm::unProject(glm::dvec3(m_select_pos,
			0.0), glm::dmat4(view),
			glm::dmat4(m_editor->get_projection_matrix()),
			view_port);
	}

	switch (m_editor->get_renderable())
	{
		default:
		case rt_none:
		case rt_particle:
			break;
		case rt_light:
			m_editor->get_light_data(light_data);

			selected_position = light_data.get_position();
			break;
		case rt_object:
			get_object_description(object_description);

			selected_position =
				object_description.get_translation();

			break;
	}

	m_selected_screen_position = glm::uvec2(glm::project(
		glm::dvec3(selected_position), glm::dmat4(view),
		glm::dmat4(m_editor->get_projection_matrix()), view_port));

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
	glm::vec3 rotation;

	m_editor->get_object_description(object_description);

	if (get_invert_z_rotation())
	{
		rotation = object_description.get_rotation_angles();

		rotation.z = 360.0f - rotation.z;

		object_description.set_rotation_angles(rotation);
	}
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

void ELGLWidget::remove_all_copies_of_object()
{
	m_editor->remove_all_copies_of_object();
	emit can_undo(m_editor->get_can_undo());
	m_select = false;
}

void ELGLWidget::set_object_blend(const BlendType value)
{
	m_editor->set_object_blend(value);
	emit can_undo(m_editor->get_can_undo());
}

void ELGLWidget::set_object_walkable(const bool value)
{
	m_editor->set_object_walkable(value);
	emit can_undo(m_editor->get_can_undo());
}

void ELGLWidget::set_object_transparency(const float value)
{
	m_editor->set_object_transparency(value);
	emit can_undo(m_editor->get_can_undo());
}

void ELGLWidget::set_translation(const glm::vec3 &translation)
{
	if (m_editor->get_objects_selected())
	{
		m_editor->set_objects_translation(translation);
	}

	if (m_editor->get_lights_selected())
	{
		m_editor->set_lights_position(translation);
	}

	if (m_editor->get_object_selected())
	{
		m_editor->set_object_translation(translation);
	}

	if (m_editor->get_light_selected())
	{
		m_editor->set_light_position(translation);
	}

	emit can_undo(m_editor->get_can_undo());
}

void ELGLWidget::set_rotation(const glm::vec3 &rotation)
{
	glm::vec3 tmp_rotation;

	tmp_rotation = rotation;

	if (get_invert_z_rotation())
	{
		tmp_rotation.z = 360.0f - tmp_rotation.z;
	}

	if (m_editor->get_objects_selected())
	{
		m_editor->set_objects_rotation(tmp_rotation);
	}

	if (m_editor->get_object_selected())
	{
		m_editor->set_object_rotation(tmp_rotation);
	}

	emit can_undo(m_editor->get_can_undo());
}

void ELGLWidget::set_scale(const glm::vec3 &scale)
{
	if (m_editor->get_objects_selected())
	{
		m_editor->set_objects_scale(scale);
	}

	if (m_editor->get_object_selected())
	{
		m_editor->set_object_scale(scale);
	}

	emit can_undo(m_editor->get_can_undo());
}

void ELGLWidget::set_objects_blend(const BlendType value)
{
	m_editor->set_objects_blend(value);
	emit can_undo(m_editor->get_can_undo());
}

void ELGLWidget::set_objects_walkable(const bool value)
{
	m_editor->set_objects_walkable(value);
	emit can_undo(m_editor->get_can_undo());
}

void ELGLWidget::set_objects_transparency(const float value)
{
	m_editor->set_objects_transparency(value);
	emit can_undo(m_editor->get_can_undo());
}

void ELGLWidget::remove_light()
{
	m_editor->remove_light();
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

void ELGLWidget::set_lights_radius(const double radius)
{
	m_editor->set_lights_radius(radius);
	emit can_undo(m_editor->get_can_undo());
}

void ELGLWidget::set_lights_color(const glm::vec3 &color)
{
	m_editor->set_lights_color(color);
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
		emit update_terrain(m_editor->get_terrain());
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
	m_pos += m_camera_yaw_rotate * glm::vec3(-0.5f * m_zoom, 0.0f, 0.0f);
}

void ELGLWidget::move_right()
{
	m_pos += m_camera_yaw_rotate * glm::vec3(0.5f * m_zoom, 0.0f, 0.0f);
}

void ELGLWidget::move_up()
{
	m_pos += m_camera_yaw_rotate * glm::vec3(0.0f, 0.5f * m_zoom, 0.0f);
}

void ELGLWidget::move_down()
{
	m_pos += m_camera_yaw_rotate * glm::vec3(0.0f, -0.5f * m_zoom, 0.0f);
}

void ELGLWidget::rotate_yaw(const int angle)
{
	m_camera_yaw = angle;
	m_camera_yaw_rotate = glm::mat3(glm::rotate(
		static_cast<float>(m_camera_yaw),
		glm::vec3(0.0f, 0.0f, 1.0f)));
}

void ELGLWidget::rotate_roll(const int angle)
{
	m_camera_roll = static_cast<float>(angle);
}

void ELGLWidget::zoom_in()
{
	m_zoom -= 0.5f;
	m_zoom = std::max(1.0f, std::min(75.0f, m_zoom));

	update_ortho();
}

void ELGLWidget::zoom_out()
{
	m_zoom += 0.5f;
	m_zoom = std::max(1.0f, std::min(75.0f, m_zoom));

	update_ortho();
}

void ELGLWidget::add_object(const String &object)
{
	m_light_adding = false;
	m_object_adding = true;
	m_object_name = object;
	m_light_radius = 0.0f;
}

void ELGLWidget::add_light(const float radius)
{
	m_light_adding = true;
	m_object_adding = false;
	m_object_name = String("");
	m_light_radius = radius;
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

	if (enabled)
	{
		m_light_adding = false;
		m_object_adding = false;
	}
}

void ELGLWidget::set_terrain_type_index(const int index)
{
	m_terrain_type_index = std::max(index, 0);
}

void ELGLWidget::set_terrain_layer_index(const int index)
{
	m_terrain_layer_index = std::max(index, 0);
}

void ELGLWidget::load_map(const QString &file_name,
	const bool load_2d_objects, const bool load_3d_objects,
	const bool load_lights, const bool load_particles,
	const bool load_materials, const bool load_height_map,
	const bool load_tile_map, const bool load_walk_map,
	const bool load_terrain, const bool load_water)
{
	if (!file_name.isEmpty())
	{
		m_editor->load_map(String(file_name.toUtf8()),
			load_2d_objects, load_3d_objects, load_lights,
			load_particles, load_materials, load_height_map,
			load_tile_map, load_walk_map, load_terrain,
			load_water);

		emit update_terrain(m_editor->get_terrain());

		emit can_undo(m_editor->get_can_undo());

		m_pos = m_editor->get_map_max() + m_editor->get_map_min();
		m_pos *= 0.5f;
		m_pos.z = 0.0f;
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

void ELGLWidget::set_terrain_material(const QString &albedo_map,
	const QString &extra_map, const float blend_size,
	const bool use_blend_size_sampler, const bool use_blend_size,
	const bool use_extra_map, const int index)
{
	m_editor->set_terrain_material(String(albedo_map.toUtf8()),
		String(extra_map.toUtf8()), blend_size, use_blend_size_sampler,
		use_blend_size, use_extra_map, index);
	emit can_undo(m_editor->get_can_undo());
}

void ELGLWidget::get_terrain_material(QString &albedo_map,
	QString &extra_map, float &blend_size, bool &use_blend_size_sampler,
	bool &use_blend_size, bool &use_extra_map, const int index) const
{
	String tmp_albedo_map, tmp_extra_map;

	m_editor->get_terrain_material(tmp_albedo_map, tmp_extra_map,
		blend_size, use_blend_size_sampler, use_blend_size,
		use_extra_map, index);

	albedo_map = QString::fromUtf8(tmp_albedo_map.get().c_str());
	extra_map = QString::fromUtf8(tmp_extra_map.get().c_str());
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
	m_object_adding = false;
	m_object_name = String("");
}

void ELGLWidget::disable_light()
{
	m_light_adding = false;
	m_light_radius = 0.0f;
}

void ELGLWidget::save(const QString &name) const
{
	m_editor->save(String(name.toUtf8()));
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

void ELGLWidget::set_draw_heights(const bool draw_heights)
{
	m_editor->set_draw_heights(draw_heights);
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

void ELGLWidget::set_debug_mode(const int value)
{
	m_global_vars->set_effect_debug(value >= 0);
	m_editor->set_debug_mode(std::max(value, 0));
}

void ELGLWidget::init_terrain(const QSize &size, const QString &albedo_map,
	const QString &extra_map, const bool use_blend_size_sampler,
	const bool use_extra_map)
{
	m_editor->init_terrain(glm::uvec2(size.width(), size.height()),
		String(albedo_map.toUtf8()), String(extra_map.toUtf8()),
		use_blend_size_sampler, use_extra_map);

	emit update_terrain(m_editor->get_terrain());
}

void ELGLWidget::init_terrain(const QString &height_map, const QSize &size,
	const QString &albedo_map, const QString &extra_map,
	const bool use_blend_size_sampler, const bool use_extra_map)
{
	m_editor->init_terrain(String(height_map.toUtf8()),
		glm::uvec2(size.width(), size.height()),
		String(albedo_map.toUtf8()), String(extra_map.toUtf8()),
		use_blend_size_sampler, use_extra_map);

	emit update_terrain(m_editor->get_terrain());
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

QVector3D ELGLWidget::get_terrain_offset_min()
{
	return QVector3D(Editor::get_terrain_offset_min().x,
		Editor::get_terrain_offset_min().y,
		Editor::get_terrain_offset_min().z);
}

QVector3D ELGLWidget::get_terrain_offset_max()
{
	return QVector3D(Editor::get_terrain_offset_max().x,
		Editor::get_terrain_offset_max().y,
		Editor::get_terrain_offset_max().z);
}

void ELGLWidget::relax_terrain_uv(const AbstractProgressSharedPtr &progress,
	const int count)
{
	m_editor->relax_terrain_uv(progress, count);
}

void ELGLWidget::update_terrain_dudv()
{
	m_editor->update_terrain_dudv();
}

void ELGLWidget::import_terrain_height_map(const QString &name)
{
	m_editor->import_terrain_height_map(String(name.toUtf8()));
}

void ELGLWidget::import_terrain_blend_map(const QString &name)
{
	m_editor->import_terrain_blend_map(String(name.toUtf8()));
}

bool ELGLWidget::get_terrain() const
{
	return m_editor->get_terrain();
}

int ELGLWidget::get_terrain_layer_count() const
{
	return m_editor->get_terrain_layer_count();
}

QVector3D ELGLWidget::get_map_min() const
{
	glm::vec3 map_min;

	map_min = m_editor->get_map_min();

	return QVector3D(map_min.x, map_min.y, map_min.z);
}

QVector3D ELGLWidget::get_map_max() const
{
	glm::vec3 map_max;

	map_max = m_editor->get_map_max();

	return QVector3D(map_max.x, map_max.y, map_max.z);
}

QSize ELGLWidget::get_map_size() const
{
	glm::uvec2 map_size;

	map_size = m_editor->get_map_size();

	return QSize(map_size.x, map_size.y);
}

void ELGLWidget::get_terrain_layers_usage(QVector<int> &use_layer_pixels,
	int &pixels) const
{
	Uint32Vector tmp;
	Uint32 i, count, temp;

	m_editor->get_terrain_layers_usage(tmp, temp);

	count = tmp.size();

	use_layer_pixels.resize(count);

	for (i = 0; i < count; ++i)
	{
		use_layer_pixels[i] = tmp[i];
	}

	pixels = temp;
}

void ELGLWidget::clear_invisible_terrain_layers()
{
	m_editor->clear_invisible_terrain_layers();
}

void ELGLWidget::pack_terrain_layers()
{
	m_editor->pack_terrain_layers();
}
