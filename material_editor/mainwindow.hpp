#ifndef	UUID_30ae5d8f_adf5_453e_bb2c_5c0591ec8ae5
#define	UUID_30ae5d8f_adf5_453e_bb2c_5c0591ec8ae5

#include "ui_mainwindow.h"
#include "../engine/prerequisites.hpp"

namespace el = eternal_lands;

class QNodesEditor;

class MainWindow: public QMainWindow, public Ui::MainWindow
{
	Q_OBJECT

	private:
		QNodesEditor *m_nodes_editor;
		el::Mt19937RandomUuidGenerator m_generator;

	private slots:
		void add_constant();
		void add_function();
		void add_parameter();
		void add_output();
		void edit_direction();

	public:
		MainWindow(QWidget *parent = 0);
		virtual ~MainWindow();

};

#endif	/* UUID_30ae5d8f_adf5_453e_bb2c_5c0591ec8ae5 */
