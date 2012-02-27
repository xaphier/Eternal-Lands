#ifndef	UUID_232e6640_0763_448d_a823_a2c2c0998d42
#define	UUID_232e6640_0763_448d_a823_a2c2c0998d42

#include "ui_settings.h"
#include <QDialog>

class SettingsDialog: public QDialog, public Ui::Settings
{
	Q_OBJECT

	private:
		QColor m_ambient_color;

	public:
		SettingsDialog(QWidget *parent = 0);

		inline const QColor &get_ambient_color() const
		{
			return m_ambient_color;
		}

		inline const QString get_blend_image_name() const
		{
			return blend_image->text();
		}

		inline void set_blend_image_name(const QString &blend_image_name)
		{
			blend_image->setText(blend_image_name);
		}

		void set_ambient_color(const QColor &ambient_color);

	public slots:
		void select_ambient_color();
		void select_blend_image();

};

#endif	/* UUID_232e6640_0763_448d_a823_a2c2c0998d42 */
