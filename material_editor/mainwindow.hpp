/****************************************************************************
 *            mainwindow.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_65e7095a_a4f7_4fa7_8f54_e51e0234507c
#define	UUID_65e7095a_a4f7_4fa7_8f54_e51e0234507c

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "ui_main.h"
#include <QDir>

class MainWindow: public QMainWindow, private Ui::MainWindow
{
	Q_OBJECT

	private:
		QDir m_data_dir;

	public:
		MainWindow(QWidget *parent = 0);

	public slots:
		void set_diffuse_map();
		void set_second_diffuse_map();
		void set_normal_map();
		void set_specular_map();
		void set_emission_map();
		void set_blend_map();
		void set_names(const QStringList values);
		void set_uv_generation_names(const QStringList values);
		void set_diffuse_mapping_names(const QStringList values);
		void set_normal_mapping_names(const QStringList values);
		void set_specular_mapping_names(const QStringList values);
		void set_emission_mapping_names(const QStringList values);
		void set_uv_generation_name(const QString value);
		void set_diffuse_mapping_name(const QString value);
		void set_normal_mapping_name(const QString value);
		void set_specular_mapping_name(const QString value);
		void set_emission_mapping_name(const QString value);

};
 
#endif	/* UUID_65e7095a_a4f7_4fa7_8f54_e51e0234507c */
