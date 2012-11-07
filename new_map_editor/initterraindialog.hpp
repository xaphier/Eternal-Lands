#ifndef	UUID_ad180c4f_fa6b_438f_9c33_06508243d25e
#define UUID_ad180c4f_fa6b_438f_9c33_06508243d25e

#include "ui_initterrain.h"
#include <QDialog>
#include <QSize>

class InitTerrainDialog: public QDialog, public Ui::InitTerrain
{
	Q_OBJECT

	private:
		QSize m_size;

	private slots:
		void change_height_map_toggle(const bool toggle);
		void change_height_map();
		void change_patches();

	public:
		InitTerrainDialog(QWidget *parent = 0);
		virtual ~InitTerrainDialog();
		QString get_height_map() const;
		void set_map_size(const QSize &size);

		inline const QSize &get_size() const
		{
			return m_size;
		}

	signals:
		void get_height_map_data(const QString &name,
			QSize &image_size, bool &ok);

};

#endif	/* UUID_ad180c4f_fa6b_438f_9c33_06508243d25e */
