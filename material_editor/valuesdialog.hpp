#ifndef	UUID_b5d5c526_8351_4954_8842_2a3b5486ce5c
#define	UUID_b5d5c526_8351_4954_8842_2a3b5486ce5c

#include "ui_valuesdialog.h"
#include <QDialog>
#include <QVector4D>

class ValuesDialog: public QDialog, public Ui::ValuesDialog
{
	Q_OBJECT

	public:
		ValuesDialog(QWidget *parent = 0);
		virtual ~ValuesDialog();
		void set_values(const QVector4D &values);
		QVector4D get_values() const;
		void set_value_count(const int count);

};

#endif	/* UUID_b5d5c526_8351_4954_8842_2a3b5486ce5c */
