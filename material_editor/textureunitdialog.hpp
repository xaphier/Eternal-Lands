#ifndef	UUID_11fbd03f_4115_4beb_b3b7_ed9788a48d75
#define	UUID_11fbd03f_4115_4beb_b3b7_ed9788a48d75

#include "ui_textureunitdialog.h"
#include <QDialog>

class TextureUnitDialog: public QDialog, public Ui::TextureUnitDialog
{
	Q_OBJECT

	private:
		int m_texture_unit;

	private slots:
		void add_image();
		void remove_image();
		void check();
		void button_clicked(QAbstractButton* button);

	public:
		TextureUnitDialog(QWidget *parent = 0);
		virtual ~TextureUnitDialog();
		QString get_name() const;
		int get_target() const;
		int get_texture_unit() const;
		QStringList get_file_names() const;
		void set_name(const QString &value);
		void set_target(const int value);
		void set_texture_unit(const int texture_unit);
		void set_file_names(const QStringList &value);

};

#endif	/* UUID_11fbd03f_4115_4beb_b3b7_ed9788a48d75 */
