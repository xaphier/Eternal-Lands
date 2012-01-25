#include "enginewidget.hpp"
#include "../engine/filesystem.hpp"
#include "../engine/globalvars.hpp"
#include "../engine/shader/shadersourcebuilder.hpp"
#include "../engine/materialdescriptioncache.hpp"
#include "../engine/materialdescription.hpp"
#include <QColorDialog> 

EngineWidget::EngineWidget(QWidget* parent): QGLWidget(parent)
{
	global_vars.reset(new el::GlobalVars());
	file_system.reset(new el::FileSystem());
	file_system->add_dir(el::String("/usr/share/games/EternalLands"));
}

void EngineWidget::set_opengl_version()
{
	if (GLEW_VERSION_3_3)
	{
		global_vars->set_opengl_version(el::ovt_3_3);
		return;
	}

	if (GLEW_VERSION_3_2)
	{
		global_vars->set_opengl_version(el::ovt_3_2);
		return;
	}

	if (GLEW_VERSION_3_1)
	{
		global_vars->set_opengl_version(el::ovt_3_1);
		return;
	}

	if (GLEW_VERSION_3_0)
	{
		global_vars->set_opengl_version(el::ovt_3_0);
		return;
	}

	if (GLEW_VERSION_2_1)
	{
		global_vars->set_opengl_version(el::ovt_2_1);
		return;
	}

	return;
}

void EngineWidget::initializeGL()
{
	QStringList values;
	el::StringVector strings;

	glewInit();

	set_opengl_version();

	scene.reset(new el::Scene(global_vars, file_system));
	scene->init(file_system);

	values.clear();

	strings = scene->get_scene_resources().get_shader_source_builder(
		)->get_shader_source_names(el::sst_uv);

	BOOST_FOREACH(const el::String &string, strings)
	{
		values.append(QString::fromUtf8(string.get().c_str()));
	}

	emit set_uv_generation_names(values);

	values.clear();

	strings = scene->get_scene_resources().get_shader_source_builder(
		)->get_shader_source_names(el::sst_diffuse_mapping);

	BOOST_FOREACH(const el::String &string, strings)
	{
		values.append(QString::fromUtf8(string.get().c_str()));
	}

	emit set_diffuse_mapping_names(values);

	values.clear();

	strings = scene->get_scene_resources().get_shader_source_builder(
		)->get_shader_source_names(el::sst_normal_mapping);

	BOOST_FOREACH(const el::String &string, strings)
	{
		values.append(QString::fromUtf8(string.get().c_str()));
	}

	emit set_normal_mapping_names(values);

	values.clear();

	strings = scene->get_scene_resources().get_shader_source_builder(
		)->get_shader_source_names(el::sst_specular_mapping);

	BOOST_FOREACH(const el::String &string, strings)
	{
		values.append(QString::fromUtf8(string.get().c_str()));
	}

	emit set_specular_mapping_names(values);

	values.clear();

	strings = scene->get_scene_resources().get_shader_source_builder(
		)->get_shader_source_names(el::sst_emission_mapping);

	BOOST_FOREACH(const el::String &string, strings)
	{
		values.append(QString::fromUtf8(string.get().c_str()));
	}

	emit set_emission_mapping_names(values);

	values.clear();

	strings = scene->get_scene_resources().get_material_description_cache(
		)->get_material_description_names();

	BOOST_FOREACH(const el::String &string, strings)
	{
		values.append(QString::fromUtf8(string.get().c_str()));
	}

	emit set_names(values);
}

void EngineWidget::resizeGL(int w, int h)
{
	float aspect;

	glViewport(0, 0, w, h);

	aspect = 1.0f;

	if (h > 0)
	{
		aspect = w / h;
	}

	scene->set_perspective(60.0f, aspect, 1.0f, 200.0f);
}

void EngineWidget::paintGL()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
/*
	scene->set_main_light_ambient(main_light_ambient);
	scene->set_main_light_color(main_light_color);
	scene->set_main_light_direction(main_light_direction);
	scene->set_night(!is_day);
*/
	scene->cull();
	scene->draw();
}

void EngineWidget::set_ambient_color()
{
	glm::vec3 color;

	m_ambient_color = QColorDialog::getColor(m_ambient_color, this,
		"Ambient color");

	scene->set_main_light_ambient(color);
}

void EngineWidget::set_sun_color()
{
	glm::vec3 color;

	m_sun_color = QColorDialog::getColor(m_sun_color, this,
		"Sun color");

	scene->set_main_light_color(color);
}

void EngineWidget::set_day_time()
{
}

void EngineWidget::set_name(const QString value)
{
	el::MaterialDescription material;
	el::String name;

	try
	{
		name = el::String(value.toUtf8().constData());
		material = scene->get_scene_resources(
			).get_material_description_cache(
			)->get_material_description(name);
	}
	catch (...)
	{
		return;
	}

	emit set_diffuse_map(QString::fromUtf8(
		material.get_texture(el::stt_diffuse_0).get().c_str()));
	emit set_second_diffuse_map(QString::fromUtf8(
		material.get_texture(el::stt_diffuse_1).get().c_str()));
	emit set_normal_map(QString::fromUtf8(
		material.get_texture(el::stt_normal_0).get().c_str()));
	emit set_specular_map(QString::fromUtf8(
		material.get_texture(el::stt_specular_0).get().c_str()));
	emit set_emission_map(QString::fromUtf8(
		material.get_texture(el::stt_emission_0).get().c_str()));
	emit set_blend_map(QString::fromUtf8(
		material.get_texture(el::stt_blend_0).get().c_str()));
	emit set_uv_generation_name(QString::fromUtf8(
		material.get_texture_coodrinates().get().c_str()));
	emit set_diffuse_mapping_name(QString::fromUtf8(
		material.get_diffuse_mapping().get().c_str()));
	emit set_normal_mapping_name(QString::fromUtf8(
		material.get_normal_mapping().get().c_str()));
	emit set_specular_mapping_name(QString::fromUtf8(
		material.get_specular_mapping().get().c_str()));
	emit set_emission_mapping_name(QString::fromUtf8(
		material.get_emission_mapping().get().c_str()));
	emit set_receives_shadows(material.get_receives_shadows());
	emit set_lighting(material.get_lighting());
}
