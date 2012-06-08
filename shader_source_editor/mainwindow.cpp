/****************************************************************************
 *            mainwindow.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "mainwindow.hpp"
#include <QMessageBox>
#include <QFileDialog>
#include <QInputDialog>
#include "shader/shadersource.hpp"
#include "shader/shadersourcedata.hpp"
#include "shader/shadersourceparameter.hpp"
#include "shader/shadersourceutil.hpp"
#include "shader/shaderversionutil.hpp"
#include "glslhighlighter.hpp"

namespace el = eternal_lands;

MainWindow::MainWindow(QWidget* parent): QMainWindow(parent)
{
	QTreeWidgetItem* qtreewidgetitem;
	QString str;
	int i, count;

	setupUi(this);

	m_parameterdialog = new ParameterDialog(this);
	m_parameters_mapper = new QSignalMapper(this);
	m_source_mapper = new QSignalMapper(this);
	m_add_mapper = new QSignalMapper(this);
	m_edit_mapper = new QSignalMapper(this);
	m_remove_mapper = new QSignalMapper(this);
	m_copy_mapper = new QSignalMapper(this);

	m_parameters.push_back(parameters_0);
	m_parameters.push_back(parameters_1);
	m_parameters.push_back(parameters_2);
	m_parameters.push_back(parameters_3);

	m_sources.push_back(source_0);
	m_sources.push_back(source_1);
	m_sources.push_back(source_2);
	m_sources.push_back(source_3);

	m_glsl_versions.push_back(glsl_version_0);
	m_glsl_versions.push_back(glsl_version_1);
	m_glsl_versions.push_back(glsl_version_2);
	m_glsl_versions.push_back(glsl_version_3);

	foreach(QPlainTextEdit* source, m_sources)
	{
		m_highlighters.push_back(new GlslHighlighter(
			source->document()));
		m_plain_text.push_back(source->toPlainText());
	}

	foreach(QObject* object, children())
	{
		object->blockSignals(true);
	}

	count = m_parameters.size();

	for (i = 0; i < count; ++i)
	{
		m_parameters_mapper->setMapping(m_parameters[i], int(i));

		QObject::connect(m_parameters[i],
			SIGNAL(itemSelectionChanged()), m_parameters_mapper,
			SLOT(map()));
	}

	QObject::connect(m_parameters_mapper, SIGNAL(mapped(const int)), this,
		SLOT(selection_changed(const int)));

	m_add_mapper->setMapping(add_0, int(0));
	m_add_mapper->setMapping(add_1, int(1));
	m_add_mapper->setMapping(add_2, int(2));
	m_add_mapper->setMapping(add_3, int(3));

	QObject::connect(add_0, SIGNAL(clicked()), m_add_mapper, SLOT(map()));
	QObject::connect(add_1, SIGNAL(clicked()), m_add_mapper, SLOT(map()));
	QObject::connect(add_2, SIGNAL(clicked()), m_add_mapper, SLOT(map()));
	QObject::connect(add_3, SIGNAL(clicked()), m_add_mapper, SLOT(map()));
	QObject::connect(m_add_mapper, SIGNAL(mapped(const int)), this,
		SLOT(add_parameter(const int)));

	m_edit_mapper->setMapping(edit_0, int(0));
	m_edit_mapper->setMapping(edit_1, int(1));
	m_edit_mapper->setMapping(edit_2, int(2));
	m_edit_mapper->setMapping(edit_3, int(3));

	QObject::connect(edit_0, SIGNAL(clicked()), m_edit_mapper, SLOT(map()));
	QObject::connect(edit_1, SIGNAL(clicked()), m_edit_mapper, SLOT(map()));
	QObject::connect(edit_2, SIGNAL(clicked()), m_edit_mapper, SLOT(map()));
	QObject::connect(edit_3, SIGNAL(clicked()), m_edit_mapper, SLOT(map()));
	QObject::connect(m_edit_mapper, SIGNAL(mapped(const int)), this,
		SLOT(edit_parameter(const int)));

	m_remove_mapper->setMapping(remove_0, int(0));
	m_remove_mapper->setMapping(remove_1, int(1));
	m_remove_mapper->setMapping(remove_2, int(2));
	m_remove_mapper->setMapping(remove_3, int(3));

	QObject::connect(remove_0, SIGNAL(clicked()), m_remove_mapper,
		SLOT(map()));
	QObject::connect(remove_1, SIGNAL(clicked()), m_remove_mapper,
		SLOT(map()));
	QObject::connect(remove_2, SIGNAL(clicked()), m_remove_mapper,
		SLOT(map()));
	QObject::connect(remove_3, SIGNAL(clicked()), m_remove_mapper,
		SLOT(map()));
	QObject::connect(m_remove_mapper, SIGNAL(mapped(const int)), this,
		SLOT(remove_parameter(const int)));

	m_copy_mapper->setMapping(copy_0, int(0));
	m_copy_mapper->setMapping(copy_1, int(1));
	m_copy_mapper->setMapping(copy_2, int(2));
	m_copy_mapper->setMapping(copy_3, int(3));

	QObject::connect(copy_0, SIGNAL(clicked()), m_copy_mapper, SLOT(map()));
	QObject::connect(copy_1, SIGNAL(clicked()), m_copy_mapper, SLOT(map()));
	QObject::connect(copy_2, SIGNAL(clicked()), m_copy_mapper, SLOT(map()));
	QObject::connect(copy_3, SIGNAL(clicked()), m_copy_mapper, SLOT(map()));
	QObject::connect(m_copy_mapper, SIGNAL(mapped(const int)), this,
		SLOT(copy_parameters(const int)));

	source_data_count_changed(source_data_count->value());

	QObject::connect(source_data_count, SIGNAL(valueChanged(const int)),
		this, SLOT(source_data_count_changed(const int)));

	QObject::connect(m_parameterdialog,
		SIGNAL(check_name(const QString, bool &)),
		this, SLOT(check_name(const QString, bool &)));

	QObject::connect(action_open, SIGNAL(activated()), this, SLOT(load()));
	QObject::connect(action_save, SIGNAL(activated()), this, SLOT(save()));
	QObject::connect(action_save_as, SIGNAL(activated()), this,
		SLOT(save_as()));

	foreach(QTreeWidget* parameters, m_parameters)
	{
		qtreewidgetitem = new QTreeWidgetItem();

		qtreewidgetitem->setText(0, "name");
		qtreewidgetitem->setText(1, "type");
		qtreewidgetitem->setText(2, "qualifier");
		qtreewidgetitem->setText(3, "size");
		qtreewidgetitem->setText(4, "scale");

		parameters->setHeaderItem(qtreewidgetitem);
		parameters->header()->setVisible(true);
		parameters->setColumnWidth(0, 175);
		parameters->setColumnWidth(1, 175);
		parameters->setColumnWidth(2, 65);
		parameters->setColumnWidth(3, 135);
		parameters->setColumnWidth(4, 50);

		parameters->sortItems(0, Qt::AscendingOrder);
	}

	count = el::ShaderSourceUtil::get_shader_source_count();

	for (i = 0; i < count; ++i)
	{
		str = QString::fromUtf8(el::ShaderSourceUtil::get_str(
			static_cast<el::ShaderSourceType>(i)).get().c_str());

		type_value->addItem(str, i);
	}

	count = m_sources.size();

	for (i = 0; i < count; ++i)
	{
		m_source_mapper->setMapping(m_sources[i], int(i));

		QObject::connect(m_sources[i], SIGNAL(textChanged()),
			m_source_mapper, SLOT(map()));
	}

	QObject::connect(m_source_mapper, SIGNAL(mapped(const int)), this,
		SLOT(text_changed(const int)));

	foreach(QComboBox* glsl_version, m_glsl_versions)
	{
		QObject::connect(glsl_version, SIGNAL(currentIndexChanged(int)),
			this, SLOT(changed()));
	}

	QObject::connect(name_value, SIGNAL(textChanged(const QString &)),
		this, SLOT(changed()));
	QObject::connect(type_value, SIGNAL(currentIndexChanged(int)),
		this, SLOT(changed()));

	count = el::ShaderVersionUtil::get_shader_version_count();

	for (i = 0; i < count; ++i)
	{
		str = QString::fromUtf8(el::ShaderVersionUtil::get_str(
			static_cast<el::ShaderVersionType>(i)).get().c_str());

		foreach(QComboBox* glsl_version, m_glsl_versions)
		{
			glsl_version->addItem(str, i);
		}
	}

	action_new->setIcon(QIcon::fromTheme("document-new"));
	action_open->setIcon(QIcon::fromTheme("document-open"));
	action_save->setIcon(QIcon::fromTheme("document-save"));
	action_save_as->setIcon(QIcon::fromTheme("document-save-as"));
	action_exit->setIcon(QIcon::fromTheme("application-exit"));

	foreach(QObject* object, children())
	{
		object->blockSignals(false);
	}

	m_changed = false;
}

MainWindow::~MainWindow()
{
}

void MainWindow::changed()
{
	m_changed = true;
}

void MainWindow::do_new()
{
	if (!check_save())
	{
		return;
	}

	foreach(QObject* object, children())
	{
		object->blockSignals(true);
	}

	foreach(QPlainTextEdit* source, m_sources)
	{
		source->clear();
	}

	foreach(QTreeWidget* parameters, m_parameters)
	{
		parameters->clear();
	}

	foreach(QComboBox* glsl_version, m_glsl_versions)
	{
		glsl_version->setCurrentIndex(0);
	}

	source_data_count->setValue(1);

	foreach(QObject* object, children())
	{
		object->blockSignals(false);
	}

	m_changed = false;
}

void MainWindow::save()
{
	if (m_file_name.isEmpty())
	{
		save_as();

		return;
	}

	save(m_file_name);
}

void MainWindow::save_as()
{
	QString file_name;

	file_name = QFileDialog::getSaveFileName(this, "save", QString(),
		"xml (*.xml)");

	if (!file_name.isEmpty())
	{
		save(file_name);
		m_file_name = file_name;
	}
}

void MainWindow::save(const QString &file_name)
{
	QTreeWidgetItem* item;
	el::ShaderSource shader_source;
	el::ShaderSourceDataVector shader_source_datas;
	el::ShaderSourceParameterVector shader_source_parameters;
	int i, j, count, index, item_count;

	shader_source.set_name(el::String(name_value->text().toUtf8()));

	index = type_value->currentIndex();

	if (index > -1)
	{
		shader_source.set_type(static_cast<el::ShaderSourceType>(
			type_value->itemData(index).toInt()));
	}

	count = source_data_count->value();

	shader_source_datas.resize(count);

	for (i = 0; i < count; i++)
	{
		index = m_glsl_versions[i]->currentIndex();

		if (index > -1)
		{
			shader_source_datas[i].set_version(
				static_cast<el::ShaderVersionType>(
					m_glsl_versions[i]->itemData(
						index).toInt()));
		}

		shader_source_datas[i].set_source(el::String(
			m_sources[i]->toPlainText().toUtf8()));

		item_count = m_parameters[i]->topLevelItemCount();

		shader_source_parameters.resize(item_count);

		for (j = 0; j < item_count; j++)
		{
			item = m_parameters[i]->topLevelItem(j);

			shader_source_parameters[j].set_name(el::String(
				item->text(0).toUtf8()));
			shader_source_parameters[j].set_type(
				el::ParameterUtil::get_parameter(
					el::String(item->text(1).toUtf8())));
			shader_source_parameters[j].set_qualifier(
				el::ParameterQualifierUtil::
					get_parameter_qualifier(
						el::String(item->text(2).
							toUtf8())));
			shader_source_parameters[j].set_size(
				el::ParameterSizeUtil::get_parameter_size(
					el::String(item->text(3).toUtf8())));
			shader_source_parameters[j].set_scale(
				item->text(4).toInt());
		}

		shader_source_datas[i].set_parameters(shader_source_parameters);
	}

	shader_source.set_datas(shader_source_datas);

	try
	{
		shader_source.save_xml(el::String(file_name.toUtf8()));
	}
	catch (...)
	{
	}

	m_changed = false;
}

void MainWindow::load()
{
	QString file_name;

	if (!check_save())
	{
		return;
	}

	file_name = QFileDialog::getOpenFileName(this, "open", QString(),
		"xml (*.xml)");

	if (!file_name.isEmpty())
	{
		load(file_name);
		m_file_name = file_name;
	}
}

void MainWindow::load(const QString &file_name)
{
	QTreeWidgetItem* item;
	QStringList list;
	el::ShaderSource shader_source;
	int i, count, index;

	try
	{
		shader_source.load_xml(el::String(file_name.toUtf8()));
	}
	catch (const boost::exception &exception)
	{
		std::cout << boost::diagnostic_information(exception)
			<< std::endl;
		return;
	}
	catch (const std::exception &exception)
	{
		std::cout << exception.what() << std::endl;
		return;
	}
	catch (...)
	{
		return;
	}

	count = shader_source.get_datas().size();

	if (count < 1)
	{
		return;
	}

	foreach(QObject* object, children())
	{
		object->blockSignals(true);
	}

	foreach(QPlainTextEdit* source, m_sources)
	{
		source->clear();
	}

	foreach(QTreeWidget* parameters, m_parameters)
	{
		parameters->clear();
	}

	name_value->setText(QString::fromUtf8(shader_source.get_name().get(
		).c_str()));

	index = type_value->findData(shader_source.get_type());
	type_value->setCurrentIndex(index);

	source_data_count->setValue(count);

	count = source_data_count->value();

	for (i = 0; i < count; i++)
	{
		index = m_glsl_versions[i]->findData(shader_source.get_datas(
			)[i].get_version());
		m_glsl_versions[i]->setCurrentIndex(index);

		m_sources[i]->setPlainText(QString::fromUtf8(
			shader_source.get_datas()[i].get_source().get(
				).c_str()));

		BOOST_FOREACH(const el::ShaderSourceParameter &parameter,
			shader_source.get_datas()[i].get_parameters())
		{
			list.clear();

			list.push_back(QString::fromUtf8(parameter.get_name(
				).get().c_str()));
			list.push_back(QString::fromUtf8(
				el::ParameterUtil::get_str(
					parameter.get_type()).get().c_str()));
			list.push_back(QString::fromUtf8(
				el::ParameterQualifierUtil::get_str(
					parameter.get_qualifier()).get(
					).c_str()));
			list.push_back(QString::fromUtf8(
				el::ParameterSizeUtil::get_str(
					parameter.get_size()).get(
						).c_str()));
			list.push_back(QString::number(parameter.get_scale()));

			item = new QTreeWidgetItem((QTreeWidgetItem*)0, list);

			m_parameters[i]->addTopLevelItem(item);
		}

		m_parameters[i]->sortItems(0, Qt::AscendingOrder);
	}

	foreach(QObject* object, children())
	{
		object->blockSignals(false);
	}

	m_changed = false;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	QMessageBox message_box;

	if (!m_changed)
	{
		QMainWindow::closeEvent(event);
		return;
	}

	message_box.setText("Closing.");
	message_box.setInformativeText("Do you want to save your changes "
		"before closing?");
	message_box.setStandardButtons(QMessageBox::Save | QMessageBox::Discard
		| QMessageBox::Cancel);
	message_box.setDefaultButton(QMessageBox::Save);

	switch (message_box.exec())
	{
		case QMessageBox::Save:
			save();
			QMainWindow::closeEvent(event);
			return;
		case QMessageBox::Discard:
			QMainWindow::closeEvent(event);
			return;
		case QMessageBox::Cancel:
			return;
		default:
			return;
	}
}

bool MainWindow::check_save()
{
	QMessageBox message_box;

	if (!m_changed)
	{
		return true;
	}

	message_box.setText("The document has been modified.");
	message_box.setInformativeText("Do you want to save your changes?");
	message_box.setStandardButtons(QMessageBox::Yes | QMessageBox::No
		| QMessageBox::Cancel);
	message_box.setDefaultButton(QMessageBox::Yes);

	switch (message_box.exec())
	{
		case QMessageBox::Yes:
			save();
			return true;
		case QMessageBox::No:
			return true;
		case QMessageBox::Cancel:
			return false;
		default:
			return false;
	}
}

void MainWindow::add_parameter(const int index)
{
	QTreeWidgetItem* item;
	QStringList list;

	if (m_parameterdialog->add_parameter())
	{
		list.push_back(m_parameterdialog->get_parameter_name());
		list.push_back(m_parameterdialog->get_parameter_type());
		list.push_back(m_parameterdialog->get_parameter_qualifier());
		list.push_back(m_parameterdialog->get_parameter_size());

		item = new QTreeWidgetItem((QTreeWidgetItem*)0, list);

		m_parameters[index]->addTopLevelItem(item);
		m_parameters[index]->sortItems(0, Qt::AscendingOrder);

		changed();
	}
}

void MainWindow::edit_parameter(const int index)
{
	QTreeWidgetItem* item;
	QStringList list;

	if (m_parameters[index]->selectedItems().size() < 1)
	{
		return;
	}

	item = m_parameters[index]->selectedItems()[0];

	if (m_parameterdialog->edit_parameter(item->text(0), item->text(1),
		item->text(2), item->text(3)))
	{
		item->setText(0, m_parameterdialog->get_parameter_name());
		item->setText(1, m_parameterdialog->get_parameter_type());
		item->setText(2, m_parameterdialog->get_parameter_qualifier());
		item->setText(3, m_parameterdialog->get_parameter_size());

		m_parameters[index]->sortItems(0, Qt::AscendingOrder);

		changed();
	}
}

void MainWindow::remove_parameter(const int index)
{
	int idx;

	if (m_parameters[index]->selectedItems().size() < 1)
	{
		return;
	}

	idx = m_parameters[index]->indexOfTopLevelItem(
		m_parameters[index]->selectedItems()[0]);

	m_parameters[index]->takeTopLevelItem(idx);
	m_parameters[index]->sortItems(0, Qt::AscendingOrder);

	changed();
}

void MainWindow::selection_changed(const int index)
{
	QWidget* widget;

	widget = dynamic_cast<QWidget*>(m_edit_mapper->mapping(index));

	widget->setEnabled(m_parameters[index]->selectedItems().size() >= 1);

	widget = dynamic_cast<QWidget*>(m_remove_mapper->mapping(index));

	widget->setEnabled(m_parameters[index]->selectedItems().size() >= 1);
}

void MainWindow::copy_parameters(const int index)
{
	QTreeWidgetItem* item;
	QString result;
	QStringList items;
	int i, count, source;
	bool ok;

	count = source_data_count->value();

	for (i = 0; i < count; ++i)
	{
		if (i != index)
		{
			items.push_back(QString::number(i));
		}
	}

	result = QInputDialog::getItem(this, "copy", "copy parameters", items,
		0, false, &ok);

	if (!ok)
	{
		return;
	}

	source = result.toInt();

	count = m_parameters[source]->topLevelItemCount();

	for (i = 0; i < count; ++i)
	{
		item = new QTreeWidgetItem(
			*m_parameters[source]->topLevelItem(i));

		m_parameters[index]->addTopLevelItem(item);
	}

	m_parameters[index]->sortItems(0, Qt::AscendingOrder);

	changed();
}

void MainWindow::source_data_count_changed(const int index)
{
	copy_0->setEnabled(index > 1);
	copy_1->setEnabled(index > 1);
	copy_2->setEnabled(index > 2);
	copy_3->setEnabled(index > 3);

	source_datas->setTabEnabled(0, index > 0);
	source_datas->setTabEnabled(1, index > 1);
	source_datas->setTabEnabled(2, index > 2);
	source_datas->setTabEnabled(3, index > 3);

	changed();
}

void MainWindow::text_changed(const int index)
{
	if (m_plain_text[index] != m_sources[index]->toPlainText())
	{
		changed();
	}
}

void MainWindow::check_name(const QString &name, bool &valid)
{
	int index;

	index = source_datas->currentIndex();

	if (index < 0)
	{
		return;
	}

	valid = m_parameters[index]->findItems(name,
		Qt::MatchExactly).size() == 0;
}
