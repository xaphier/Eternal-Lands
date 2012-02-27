#ifndef	UUID_2f1c33dd_be99_46cd_a113_2b899cc3a972
#define UUID_2f1c33dd_be99_46cd_a113_2b899cc3a972

#include "ui_infos.h"
#include <QDialog>

class InfosDialog: public QDialog, public Ui::Infos
{
	Q_OBJECT

	public:
		InfosDialog(QWidget *parent = 0);

};

#endif	/* UUID_2f1c33dd_be99_46cd_a113_2b899cc3a972 */
