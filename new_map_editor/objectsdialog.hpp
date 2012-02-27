#ifndef	UUID_53098e02_ebc2_4f84_9468_8c19d5bffe6c
#define UUID_53098e02_ebc2_4f84_9468_8c19d5bffe6c

#include "ui_objects.h"
#include "gui/elpreviewwidget.hpp"
#include <QDialog>

class ObjectsDialog: public QDialog, public Ui::Objects
{
	Q_OBJECT

	private:
		ELPreviewWidget *widget;
		std::string m_object;
		unsigned short m_type;
		unsigned short m_server_id;

		void load_objects();

	public:
		ObjectsDialog(QWidget *parent, const QGLWidget *shareWidget);

		inline const std::string &get_object() const
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
