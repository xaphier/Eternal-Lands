/****************************************************************************
 *            mainwindow.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_63697c24_9c23_4d13_a586_d7af50ae2091
#define UUID_63697c24_9c23_4d13_a586_d7af50ae2091

#include "ui_main.h"
#include <QSignalMapper>
#include <QSyntaxHighlighter>
#include "parameterdialog.hpp"

class MainWindow: public QMainWindow, public Ui::MainWindow
{
	Q_OBJECT

	private:
		std::vector<QTreeWidget*> m_parameters;
		std::vector<QPlainTextEdit*> m_sources;
		std::vector<QComboBox*> m_glsl_versions;
		std::vector<QSyntaxHighlighter*> m_highlighters;
		std::vector<QString> m_plain_text;
		std::vector<QCheckBox*> m_vertexes;
		std::vector<QCheckBox*> m_tess_controls;
		std::vector<QCheckBox*> m_tess_evaluations;
		std::vector<QCheckBox*> m_geometrys;
		std::vector<QCheckBox*> m_fragments;
		QSignalMapper* m_parameters_mapper;
		QSignalMapper* m_source_mapper;
		QSignalMapper* m_add_mapper;
		QSignalMapper* m_edit_mapper;
		QSignalMapper* m_remove_mapper;
		QSignalMapper* m_copy_mapper;
		ParameterDialog* m_parameterdialog;
		QString m_file_name;
		bool m_changed;

		void save(const QString &file_name);
		void load(const QString &file_name);
		bool check_save();

	private slots:
		void add_parameter(const int index);
		void edit_parameter(const int index);
		void remove_parameter(const int index);
		void selection_changed(const int index);
		void copy_parameters(const int index);
		void source_data_count_changed(const int index);
		void check_name(const QString &name, bool &valid);
		void text_changed(const int index);
		void save();
		void save_as();
		void load();
		void do_new();
		void changed();

	protected:
		virtual void closeEvent(QCloseEvent* event);

	public:
		MainWindow(QWidget* parent = 0);
		virtual ~MainWindow();

	public slots:

};

#endif	/* UUID_63697c24_9c23_4d13_a586_d7af50ae2091 */
