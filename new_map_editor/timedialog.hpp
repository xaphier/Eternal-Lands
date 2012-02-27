#ifndef	UUID_5dc729f3_b999_4c2a_83dd_927840396698
#define	UUID_5dc729f3_b999_4c2a_83dd_927840396698

#include "ui_time.h"
#include <QDialog>

class TimeDialog: public QDialog, public Ui::Time
{
	Q_OBJECT

	public:
		TimeDialog(QWidget *parent = 0);

};

#endif	/* UUID_5dc729f3_b999_4c2a_83dd_927840396698 */
