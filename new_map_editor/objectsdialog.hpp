#ifndef	UUID_53098e02_ebc2_4f84_9468_8c19d5bffe6c
#define UUID_53098e02_ebc2_4f84_9468_8c19d5bffe6c

#include "ui_objects.h"
#include "elpreviewwidget.hpp"
#include <QDialog>

using namespace eternal_lands;

class ObjectsDialog: public QDialog, public Ui::Objects
{
	Q_OBJECT

	private:
		QString m_object;
		unsigned short m_type;
		unsigned short m_server_id;

		void load_objects();

	public:
		ObjectsDialog(QWidget* parent);
		virtual ~ObjectsDialog();		
		void set_dirs(const QStringList &dirs);
		void set_object(const QString &object);

		inline const QString &get_object() const
		{
			return m_object;
		}

		inline unsigned short get_type() const
		{
			return m_type;
		}

		inline unsigned short get_server_id() const
		{
			return m_server_id;
		}

	public slots:
		void selected();

};

#endif	/* UUID_53098e02_ebc2_4f84_9468_8c19d5bffe6c */
