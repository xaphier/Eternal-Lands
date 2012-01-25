/****************************************************************************
 *            enginewidget.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_d83047f4_ddd2_4be6_a50e_cbebae8ca89b
#define	UUID_d83047f4_ddd2_4be6_a50e_cbebae8ca89b

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "../engine/scene.hpp"
#include <QGLWidget> 

namespace el = eternal_lands;

class EngineWidget: public QGLWidget
{
	Q_OBJECT

	private:
		boost::scoped_ptr<el::Scene> scene;
		el::GlobalVarsSharedPtr global_vars;
		el::FileSystemSharedPtr file_system;
		QString m_data_dir;
		QColor m_ambient_color;
		QColor m_sun_color;
		Uint32 m_day_time;

		void set_opengl_version();

	protected:
		virtual void initializeGL();
		virtual void resizeGL(int w, int h);
		virtual void paintGL();

	public:
		EngineWidget(QWidget *parent = 0);

	public slots:
		void set_ambient_color();
		void set_sun_color();
		void set_day_time();
		void set_name(const QString value);

	signals:
		void set_diffuse_map(const QString value);
		void set_second_diffuse_map(const QString value);
		void set_normal_map(const QString value);
		void set_specular_map(const QString value);
		void set_emission_map(const QString value);
		void set_blend_map(const QString value);
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
		void set_receives_shadows(const bool value);
		void set_lighting(const bool value);

};
 
#endif	/* UUID_d83047f4_ddd2_4be6_a50e_cbebae8ca89b */
