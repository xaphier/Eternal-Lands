#ifndef	UUID_01b0c288_14d1_4a8d_bf89_4c4830505093
#define	UUID_01b0c288_14d1_4a8d_bf89_4c4830505093

#include "ui_directiondialog.h"
#include <QDialog>
#include <QVector2D>

class DirectionDialog: public QDialog, public Ui::DirectionDialog
{
	Q_OBJECT

	private:
		bool m_updating;

	private slots:
		void x_value_change(const double value);
		void y_value_change(const double value);
		void z_value_change(const int value);

	public:
		DirectionDialog(QWidget *parent = 0);
		virtual ~DirectionDialog();
		void set_direction(const QVector2D &dir);
		QVector2D get_direction() const;

};

#endif	/* UUID_01b0c288_14d1_4a8d_bf89_4c4830505093 */
