#include <qapplication.h>
#include <QMessageBox>
#include "mainwindow.hpp"
#include "exceptions.hpp"
#include "logmanager.hpp"
#include <boost/exception/diagnostic_information.hpp>

class ElApplication: public QApplication
{
	public:
		inline ElApplication(int &argc, char *argv[]): QApplication(argc, argv)
		{
		}

		virtual inline ~ElApplication()
		{
		}

		virtual inline bool notify(QObject* receiver, QEvent* event)
		{
			try
			{
				return QApplication::notify(receiver, event);
			}
			catch (const boost::exception &exception)
			{
				LOG_EXCEPTION(exception);
				QMessageBox::critical(0, "boost::exception",
					boost::diagnostic_information(exception).c_str());
			}
			catch (const std::exception &exception)
			{
				LOG_EXCEPTION(exception);
				QMessageBox::critical(0, "std::exception", exception.what());
			}
			exit(-1);
			return false;
		}

};

int main(int argc, char *argv[])
{
	ElApplication app(argc, argv);
	QGLFormat gl_format;

	gl_format.setDoubleBuffer(true);
	gl_format.setRgba(true);
	gl_format.setDepth(true);

	QGLFormat::setDefaultFormat(gl_format);

	try
	{
		QCoreApplication::setOrganizationName("Eternal Lands");
		QCoreApplication::setOrganizationDomain("eternal-lands.com");
		QCoreApplication::setApplicationName("Map Editor");
		MainWindow *window = new MainWindow();

		window->show();
	}
	catch (const boost::exception &exception)
	{
		LOG_EXCEPTION(exception);
		QMessageBox::critical(0, "boost::exception",
			boost::diagnostic_information(exception).c_str());
	}
	catch (const std::exception &exception)
	{
		LOG_EXCEPTION(exception);
		QMessageBox::critical(0, "std::exception", exception.what());
	}

	return app.exec();
}
