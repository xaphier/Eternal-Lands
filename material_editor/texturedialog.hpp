#ifndef	UUID_3f573565_5f1a_4eda_80d0_33d07db6233c
#define	UUID_3f573565_5f1a_4eda_80d0_33d07db6233c

#include "ui_texturedialog.h"
#include <QDialog>

class TextureDialog: public QDialog, public Ui::TextureDialog
{
	Q_OBJECT

	private:

	private slots:
		void texture_unit_change(const int value);
		void projection_change(const bool value);
		void texture_change(const int value);

	public:
		TextureDialog(QWidget *parent = 0);
		virtual ~TextureDialog();

};

#endif	/* UUID_3f573565_5f1a_4eda_80d0_33d07db6233c */
