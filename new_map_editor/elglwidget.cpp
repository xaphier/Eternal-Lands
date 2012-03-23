#include "elglwidget.hpp"
#include <sceneresources.hpp>
#include <filesystem.hpp>
#include <globalvars.hpp>
#include <codec/codecmanager.hpp>
#include <lightdata.hpp>
#include "editor/editorobjectdata.hpp"
#include <QFileDialog>
#include <QMessageBox>

ELGLWidget::ELGLWidget(QWidget *parent): QGLWidget(parent)
{
	m_select = false;
	m_zoom = 10.0f;
	m_terrain_editing = false;
	m_terrain_type_index = 0;
	m_terrain_layer_index = 0;
	m_light = false;
	m_rotate_z = 0.0f;
	m_color = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
	m_blend = bt_disabled;

	m_global_vars = boost::make_shared<GlobalVars>();
	m_file_system = boost::make_shared<FileSystem>();

	m_editor.reset(new Editor(m_global_vars, m_file_system));
}

ELGLWidget::~ELGLWidget()
{
}

void ELGLWidget::get_points(const Sint32 x, const Sint32 y, glm::vec3 &p0,
	glm::vec3 &p1)
{
	glm::ivec4 view_port;

	view_port[0] = 0;
	view_port[1] = 0;
	view_port[2] = width();
	view_port[3] = height();

	p0 = glm::unProject(glm::vec3(x, y, 0), m_view, m_projection,
		view_port);
	p1 = glm::unProject(glm::vec3(x, y, 1), m_view, m_projection,
		view_port);
}

void ELGLWidget::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == m_click_button)
	{
		glm::vec3 p0, p1, dir;
		glm::vec3 position;

		if (get_terrain_editing())
		{
			m_terrain_index++;

			emit terrain_edit(event->x(), height() - event->y());
		}
		else
		{
/*			if (!m_object.empty())
			{
				get_points(event->x(), height() - event->y(), p0, p1);

				dir = p1 - p0;

				position = p0 - dir * (p0[2] / dir[2]);

				m_editor->add_3d_object(position, m_color, m_type,
					m_server_id, m_object);
				emit update_object(false);
				emit can_undo(m_editor->get_can_undo());
			}
			else
*/			{
				if (m_light)
				{
					get_points(event->x(), height() - event->y(), p0, p1);

					dir = p1 - p0;

					position = p0 - dir * (p0[2] / dir[2]);

					m_editor->add_light(position);
					emit update_light(false);
					emit can_undo(m_editor->get_can_undo());
				}
				else
				{
					m_select_pos[0] = event->x();
					m_select_pos[1] = height() - event->y();
					m_select = true;
				}
			}
		}
		updateGL();
	}
}

void ELGLWidget::mouseMoveEvent(QMouseEvent *event)
{
	if (get_terrain_editing())
	{
		emit terrain_edit(event->x(), height() - event->y());

		updateGL();
	}
}

void ELGLWidget::terrain_height_edit(const int x, const int y, const float strength,
	const float radius, const int brush_type)
{
	glm::vec3 p0, p1;

	get_points(x, y, p0, p1);

	m_editor->terrain_height_edit(m_terrain_index, p0, p1, strength, radius, brush_type);

	emit can_undo(m_editor->get_can_undo());
}

void ELGLWidget::terrain_layer_edit(const int x, const int y,
	const int terrain_layer_index, const float strength,
	const float radius, const int brush_type)
{
	glm::vec3 p0, p1;

	get_points(x, y, p0, p1);

	m_editor->terrain_layer_edit(m_terrain_index, p0, p1, terrain_layer_index,
		strength, radius, brush_type);

	emit can_undo(m_editor->get_can_undo());
}

void ELGLWidget::ground_tile_edit(const int x, const int y, const int tile)
{
	glm::vec3 p0, p1;

	get_points(x, y, p0, p1);

	m_editor->ground_tile_edit(p0, p1, tile);

	emit can_undo(m_editor->get_can_undo());
}

void ELGLWidget::water_tile_edit(const int x, const int y, const int water)
{
	glm::vec3 p0, p1;

	get_points(x, y, p0, p1);

	m_editor->water_tile_edit(p0, p1, water);

	emit can_undo(m_editor->get_can_undo());
}

void ELGLWidget::height_edit(const int x, const int y, const int height)
{
	glm::vec3 p0, p1;

	get_points(x, y, p0, p1);

	m_editor->height_edit(p0, p1, height);

	emit can_undo(m_editor->get_can_undo());
}

void ELGLWidget::wheelEvent(QWheelEvent *event)
{
	if (event->modifiers().testFlag(m_wheel_zoom_x10))
	{
		m_zoom += event->delta() / 12.0f * (m_swap_wheel_zoom ? -1.0f : 1.0f);
	}
	else
	{
		m_zoom += event->delta() / 120.0f * (m_swap_wheel_zoom ? -1.0f : 1.0f);
	}

	m_zoom = std::max(1.0f, std::min(200.0f, m_zoom));
	updateGL();
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

	if (!GLEW_VERSION_3_0)
	{
		BoostFormat format_string(UTF8("OpenGL 3.0 needed, but '%1%'"
			" found!"));

		format_string % (const char*)glGetString(GL_VERSION);

		QMessageBox::critical(0, "Error", QString::fromUtf8(
			format_string.str().c_str()));

		exit(1);
	}

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

	try
	{
		m_editor->init();
	}
	catch (...)
	{
	}
}

void ELGLWidget::resizeGL(int width, int height)
{
	glViewport(0, 0, width, height);

	m_editor->set_view_port(glm::uvec4(0, 0, width, height));

	m_editor->set_perspective(60.0f, static_cast<float>(width) /
		static_cast<float>(height), 1.5f, 250.0f);

	updateGL();

	m_projection = m_editor->get_projection_matrix();
}

void ELGLWidget::paintGL()
{
	glm::vec3 dir, pos;

	glEnable(GL_CULL_FACE);
	glDisable(GL_ALPHA_TEST);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	dir = glm::normalize(glm::vec3(0.0f, -1.0f, 1.0f));

	pos = m_pos;

	dir = m_rotate * dir * m_zoom;

	m_view = glm::lookAt(pos + dir, pos, glm::normalize(glm::vec3(0.0f, 0.0f, 1.0f)));
	m_editor->set_view_matrix(m_view);

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
				updateGL();
				break;
			case rt_object:
				emit update_object(true);
				updateGL();
				break;
		}
	}

	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindBufferARB(GL_ARRAY_BUFFER, 0);
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBufferARB(GL_PIXEL_PACK_BUFFER, 0);
	glBindBufferARB(GL_PIXEL_UNPACK_BUFFER, 0);
	glUseProgram(0);

	for (int i = 0; i < 16; i++)
	{
		glDisableVertexAttribArray(i);
	}

	glBlendColor(1.0f, 1.0f, 1.0f, 1.0f);
	glBlendFunc(GL_ZERO, GL_ONE);
}

void ELGLWidget::get_object_data(EditorObjectData &object_data)
	const
{
	m_editor->get_object_data(object_data);
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
			updateGL();
			break;
		case rt_light:
			m_editor->remove_light();
			emit can_undo(m_editor->get_can_undo());
			m_select = false;
			updateGL();
			break;
		case rt_particle:
//			m_editor->remove_particle();
			emit can_undo(m_editor->get_can_undo());
			m_select = false;
			updateGL();
			break;
		default:
			break;
	}
}

void ELGLWidget::set_object_blend(const BlendType value)
{
	m_editor->set_object_blend(value);
	emit can_undo(m_editor->get_can_undo());
	updateGL();
}

void ELGLWidget::set_object_translation(const glm::vec3 &translation)
{
	m_editor->set_object_translation(translation);
	emit can_undo(m_editor->get_can_undo());
	updateGL();
}

void ELGLWidget::set_object_rotation(const glm::vec3 &rotation)
{
	m_editor->set_object_rotation(rotation);
	emit can_undo(m_editor->get_can_undo());
	updateGL();
}

void ELGLWidget::set_object_scale(const float scale)
{
	m_editor->set_object_scale(scale);
	emit can_undo(m_editor->get_can_undo());
	updateGL();
}

void ELGLWidget::remove_light()
{
	m_editor->remove_light();
	emit can_undo(m_editor->get_can_undo());
	updateGL();
}

void ELGLWidget::set_light_position(const glm::vec3 &position)
{
	m_editor->set_light_position(position);
	emit can_undo(m_editor->get_can_undo());
	updateGL();
}

void ELGLWidget::set_light_radius(const double radius)
{
	m_editor->set_light_radius(radius);
	emit can_undo(m_editor->get_can_undo());
	updateGL();
}

void ELGLWidget::set_light_color(const glm::vec3 &color)
{
	m_editor->set_light_color(color);
	emit can_undo(m_editor->get_can_undo());
	updateGL();
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

	updateGL();
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

	updateGL();
}

void ELGLWidget::light_mode(const bool enabled)
{
//	m_editor->get_scene().set_draw_lights(enabled);

	updateGL();
}

void ELGLWidget::set_ambient(const glm::vec3 &color)
{
	m_editor->set_ambient(color);
	emit can_undo(m_editor->get_can_undo());
	updateGL();
}

const glm::vec3 &ELGLWidget::get_ambient() const
{
	return m_editor->get_ambient();
}

void ELGLWidget::move_left()
{
	m_pos += m_rotate * glm::vec3(-5.0f, 0.0f, 0.0f);

	updateGL();
}

void ELGLWidget::move_right()
{
	m_pos += m_rotate * glm::vec3(5.0f, 0.0f, 0.0f);

	updateGL();
}

void ELGLWidget::move_up()
{
	m_pos += m_rotate * glm::vec3(0.0f, 5.0f, 0.0f);

	updateGL();
}

void ELGLWidget::move_down()
{
	m_pos += m_rotate * glm::vec3(0.0f, -5.0f, 0.0f);

	updateGL();
}

void ELGLWidget::rotate_left()
{
	m_rotate_z -= 5.0f;

	if (m_rotate_z < 0.0f)
	{
		m_rotate_z += 360.0f;
	}

	m_rotate = glm::mat3(glm::rotate(m_rotate_z, glm::vec3(0.0f, 0.0f, 1.0f)));

	updateGL();
}

void ELGLWidget::rotate_right()
{
	m_rotate_z += 5.0f;

	if (m_rotate_z >= 360.0f)
	{
		m_rotate_z -= 360.0f;
	}

	m_rotate = glm::mat3(glm::rotate(m_rotate_z, glm::vec3(0.0f, 0.0f, 1.0f)));

	updateGL();
}

void ELGLWidget::zoom_in()
{
	m_zoom -= 1.0f;
	m_zoom = std::max(1.0f, std::min(200.0f, m_zoom));
	updateGL();
}

void ELGLWidget::zoom_out()
{
	m_zoom += 1.0f;
	m_zoom = std::max(1.0f, std::min(200.0f, m_zoom));
	updateGL();
}

void ELGLWidget::add_object(const glm::vec4 &color, const Uint16 type, const Uint16 server_id,
	const String &object)
{
	m_object = object;
	m_color = color;
	m_light = false;
}

void ELGLWidget::add_light()
{
	m_light = true;
	m_object = String("");
}

QStringList ELGLWidget::get_terrain_albedo_maps() const
{
	QStringList result;

	result << QString::fromUtf8(m_editor->get_terrain_albedo_map(0,
		get_terrain_index()).get().c_str());
	result << QString::fromUtf8(m_editor->get_terrain_albedo_map(1,
		get_terrain_index()).get().c_str());
	result << QString::fromUtf8(m_editor->get_terrain_albedo_map(2,
		get_terrain_index()).get().c_str());
	result << QString::fromUtf8(m_editor->get_terrain_albedo_map(3,
		get_terrain_index()).get().c_str());

	return result;
}

QString ELGLWidget::get_terrain_height_map() const
{
	return QString::fromUtf8(m_editor->get_terrain_height_map(
		get_terrain_index()).get().c_str());
}

QString ELGLWidget::get_terrain_blend_map() const
{
	return QString::fromUtf8(m_editor->get_terrain_blend_map(
		get_terrain_index()).get().c_str());
}

QString ELGLWidget::get_terrain_dudv_map() const
{
	return QString::fromUtf8(m_editor->get_terrain_dudv_map(
		get_terrain_index()).get().c_str());
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
	updateGL();
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
	updateGL();
*/
}

void ELGLWidget::set_terrain_editing(const bool enabled)
{
	m_terrain_editing = enabled;
}

void ELGLWidget::set_terrain_type_index(const int index)
{
	m_terrain_type_index = boost::numeric_cast<Uint32>(index);
}

void ELGLWidget::set_terrain_layer_index(const int index)
{
	m_terrain_layer_index = boost::numeric_cast<Uint32>(index);
}

void ELGLWidget::open_map(const QString &file_name)
{
	if (!file_name.isEmpty())
	{
		m_editor->load_map(String(file_name.toUtf8()));

		emit can_undo(m_editor->get_can_undo());
		updateGL();
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
	updateGL();
}

void ELGLWidget::set_object_selection(const SelectionType selection)
{
	m_editor->set_object_selection(selection);
	emit can_undo(m_editor->get_can_undo());
	updateGL();
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
	m_object = String("");
}

void ELGLWidget::disable_light()
{
	m_light = false;
}

void ELGLWidget::save(const QString &name) const
{
//	m_editor->save(String(name.toUtf8()));
}

QString ELGLWidget::get_blend_image_name() const
{
	return QString::fromStdString(m_editor->get_blend_image_name());
}

void ELGLWidget::set_blend_image_name(const QString &blend_image)
{
	m_editor->set_blend_image_name(String(blend_image.toUtf8()));
}

void ELGLWidget::set_game_minute(const int game_minute)
{
//	m_editor->get_scene().set_game_minute(game_minute);
	updateGL();
}

glm::vec3 ELGLWidget::get_light_color() const
{
	LightData light;

	get_light_data(light);

	return light.get_color();
}

void ELGLWidget::set_draw_lights(const bool draw_lights)
{
	m_editor->set_draw_lights(draw_lights);
}

void ELGLWidget::set_draw_light_spheres(const bool draw_light_spheres)
{
	m_editor->set_draw_light_spheres(draw_light_spheres);
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
	updateGL();
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

	updateGL();
}
*/
