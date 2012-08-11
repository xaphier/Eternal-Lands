#ifndef	UUID_3f573565_5f1a_4eda_80d0_33d07db6233c
#define	UUID_3f573565_5f1a_4eda_80d0_33d07db6233c

#include "ui_texturedialog.h"
#include <QDialog>

class TextureDialog: public QDialog, public Ui::TextureDialog
{
	Q_OBJECT

	private:

	private slots:
		void projection_change(const bool value);

	public:
		TextureDialog(QWidget *parent = 0);
		virtual ~TextureDialog();
		bool get_project() const;
		int get_texture() const;
		int get_texture_units() const;
		void set_texture_units(const QStringList &names);
		void set_target(const int target);

};

#endif	/* UUID_3f573565_5f1a_4eda_80d0_33d07db6233c */
