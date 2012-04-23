#include <qapplication.h>
#include <QMessageBox>
#include "mainwindow.hpp"
#include <boost/exception/diagnostic_information.hpp>

class ElApplication: public QApplication
{
	public:
		inline ElApplication(int &argc, char *argv[]):
			QApplication(argc, argv)
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
				QMessageBox::critical(0, "boost::exception",
					boost::diagnostic_information(
						exception).c_str());
			}
			catch (const std::exception &exception)
			{
				QMessageBox::critical(0, "std::exception",
					exception.what());
			}
			exit(-1);
			return false;
		}

};

int main(int argc, char *argv[])
{
	ElApplication app(argc, argv);

	try
	{
		MainWindow* window;

		QCoreApplication::setOrganizationName("Eternal Lands");
		QCoreApplication::setOrganizationDomain("eternal-lands.com");
		QCoreApplication::setApplicationName("Shader Source Editor");
		window = new MainWindow();

		window->show();
	}
	catch (const boost::exception &exception)
	{
		QMessageBox::critical(0, "boost::exception",
			boost::diagnostic_information(exception).c_str());
	}
	catch (const std::exception &exception)
	{
		QMessageBox::critical(0, "std::exception", exception.what());
	}

	return app.exec();
}
