#ifndef	UUID_30ae5d8f_adf5_453e_bb2c_5c0591ec8ae5
#define	UUID_30ae5d8f_adf5_453e_bb2c_5c0591ec8ae5

#include "ui_mainwindow.h"
#include "texturedialog.hpp"
#include "textureunitdialog.hpp"
#include "../engine/prerequisites.hpp"
#include <QSignalMapper>

namespace el = eternal_lands;

class QNodesEditor;

class MainWindow: public QMainWindow, public Ui::MainWindow
{
	Q_OBJECT

	private:
		TextureDialog* m_texture_dialog;
		TextureUnitDialog* m_texture_unit_dialog;
		QSignalMapper* m_texture_unit_mapper;
		QNodesEditor *m_nodes_editor;
		std::vector<QPushButton*> m_buttons;
		std::vector<int> m_targets;
		std::vector<QStringList> m_file_names;
		QStringList m_names;
		el::EffectNodesSharedPtr m_effect_nodes;

	private slots:
		void add_color();
		void add_direction();
		void add_constant();
		void add_function();
		void add_parameter();
		void add_texture();
		void add_output();
		void change_texture(const int index);
		void texture_unit_changed(const int value);
		void new_data();
		void load();
		void save();
		void save_as();

	public:
		MainWindow(QWidget *parent = 0);
		virtual ~MainWindow();

};

#endif	/* UUID_30ae5d8f_adf5_453e_bb2c_5c0591ec8ae5 */
