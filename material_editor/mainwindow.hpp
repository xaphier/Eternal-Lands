#ifndef	UUID_30ae5d8f_adf5_453e_bb2c_5c0591ec8ae5
#define	UUID_30ae5d8f_adf5_453e_bb2c_5c0591ec8ae5

#include <QMainWindow>
#include "ui_mainwindow.h"

class QNodesEditor;

class MainWindow: public QMainWindow, public Ui::MainWindow
{
	Q_OBJECT

	private:
		QNodesEditor *m_nodes_editor;

	private slots:
		void add_constant();

	public:
		MainWindow(QWidget *parent = 0);
		virtual ~MainWindow();

};

#endif	/* UUID_30ae5d8f_adf5_453e_bb2c_5c0591ec8ae5 */
