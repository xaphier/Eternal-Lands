#ifndef	UUID_418fe94a_9ff0_48c8_b14c_35acbc54568b
#define	UUID_418fe94a_9ff0_48c8_b14c_35acbc54568b

#include "ui_newmap.h"
#include <QDialog>

class ELGLWidget;

class NewMapDialog: public QDialog, public Ui::NewMap
{
	Q_OBJECT

	private slots:
		void load_image();
		void check_ok();

	public:
		NewMapDialog(QWidget *parent);
		void set_diffuse_textures(const QStringList &textures);
		void set_normal_textures(const QStringList &textures);

};

#endif	/* UUID_418fe94a_9ff0_48c8_b14c_35acbc54568b */
