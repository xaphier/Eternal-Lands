#include "preferencesdialog.hpp"
#include <QKeyEvent>
#include <QFileDialog>

PreferencesDialog::PreferencesDialog(QWidget *parent): QDialog(parent)
{
	setupUi(this);

	shortcut->installEventFilter(this);
	QObject::connect(clear_button, SIGNAL(clicked()), this, SLOT(clear_shortcuts()));
	QObject::connect(reset_button, SIGNAL(clicked()), this, SLOT(action_reset()));
	QObject::connect(default_button, SIGNAL(clicked()), this, SLOT(action_default()));
	QObject::connect(ok_button, SIGNAL(clicked()), this, SLOT(set_action_shortcuts()));
	QObject::connect(action_list, SIGNAL(itemSelectionChanged()), this, SLOT(action_changed()));
	QObject::connect(textures_remove, SIGNAL(clicked()), this, SLOT(remove_texture()));
	QObject::connect(dirs_add, SIGNAL(clicked()), this, SLOT(add_dir()));
	QObject::connect(dirs_remove, SIGNAL(clicked()), this, SLOT(remove_dir()));
	QObject::connect(dirs_up, SIGNAL(clicked()), this, SLOT(up_dir()));
	QObject::connect(dirs_down, SIGNAL(clicked()), this, SLOT(down_dir()));

	click_button->addItem(mouse_button_to_str(Qt::LeftButton));
	click_button->addItem(mouse_button_to_str(Qt::MidButton));
	click_button->addItem(mouse_button_to_str(Qt::RightButton));

	wheel_zoom_x10->addItem(key_mod_to_str(Qt::NoModifier));
	wheel_zoom_x10->addItem(key_mod_to_str(Qt::AltModifier));
	wheel_zoom_x10->addItem(key_mod_to_str(Qt::ControlModifier));
	wheel_zoom_x10->addItem(key_mod_to_str(Qt::MetaModifier));
	wheel_zoom_x10->addItem(key_mod_to_str(Qt::ShiftModifier));
	wheel_zoom_x10->addItem(key_mod_to_str(Qt::KeypadModifier));

	m_textures_model = new QStringListModel(textures);
	textures->setModel(m_textures_model);

	m_dirs_model = new QStringListModel(dirs);
	dirs->setModel(m_dirs_model);

	m_dirs_model->setStringList(m_dirs);

	check_dir_enable();
}

void PreferencesDialog::add_dir()
{
	QString dir;

	dir = QFileDialog::getExistingDirectory(this, "Select directory");

	if (!dir.isEmpty())
	{
		m_dirs.append(dir);

		m_dirs_model->setStringList(m_dirs);
	}
}

void PreferencesDialog::remove_dir()
{
	if (dirs->currentIndex().isValid())
	{
		m_dirs.removeAt(dirs->currentIndex().row());

		m_dirs_model->setStringList(m_dirs);
		check_dir_enable();
	}
}

void PreferencesDialog::up_dir()
{
	int index;

	if (dirs->currentIndex().isValid())
	{
		index = dirs->currentIndex().row();

		if (index > 0)
		{
			m_dirs.move(index, index - 1);

			m_dirs_model->setStringList(m_dirs);

			check_dir_enable();
		}
	}
}

void PreferencesDialog::down_dir()
{
	int index;

	if (dirs->currentIndex().isValid())
	{
		index = dirs->currentIndex().row();

		if (index < (m_dirs.size() - 1))
		{
			m_dirs.move(index, index + 1);

			m_dirs_model->setStringList(m_dirs);

			check_dir_enable();
		}
	}
}

void PreferencesDialog::check_dir_enable()
{
	int index;

	if (dirs->currentIndex().isValid())
	{
		index = dirs->currentIndex().row();

		dirs_remove->setEnabled(true);

		dirs_up->setEnabled(index > 0);
		dirs_down->setEnabled(index < (m_dirs.size() - 1));
	}
	else
	{
		dirs_remove->setEnabled(false);
		dirs_up->setEnabled(false);
		dirs_down->setEnabled(false);
	}
}

void PreferencesDialog::remove_texture_enable()
{
	textures_remove->setEnabled(textures->currentIndex().isValid());
}

void PreferencesDialog::remove_texture()
{
	if (textures->currentIndex().isValid())
	{
		m_textures.removeAt(textures->currentIndex().row());

		m_textures_model->setStringList(m_textures);
		remove_texture_enable();
	}
}

void PreferencesDialog::set_action_shortcuts()
{
	QTreeWidgetItem* item;
	QKeySequence key_sequence;
	int i, index;

	for (i = 0; i < action_list->topLevelItemCount(); i++)
	{
		item = action_list->topLevelItem(i);
		key_sequence = QKeySequence(item->text(1));
		index = item->type();

		m_actions[index]->setShortcut(key_sequence);
	}
}

void PreferencesDialog::action_changed()
{
	if (action_list->selectedItems().size() == 1)
	{
		m_key_sequence =
			QKeySequence(action_list->selectedItems()[0]->text(1));

		set_shortcut();
	}
}

void PreferencesDialog::action_reset()
{
	QTreeWidgetItem* item;
	QString str;
	int index;

	if (action_list->selectedItems().size() == 1)
	{
		item = action_list->selectedItems()[0];

		index = item->type();

		m_key_sequence = m_actions[index]->shortcut();

		set_shortcut();
	}
}

void PreferencesDialog::action_default()
{
	QTreeWidgetItem* item;
	QString str;
	int index;

	if (action_list->selectedItems().size() == 1)
	{
		item = action_list->selectedItems()[0];

		index = item->type();

		m_key_sequence =
			QKeySequence::mnemonic(m_actions[index]->text());

		set_shortcut();
	}
}

void PreferencesDialog::set_shortcut()
{
	QTreeWidgetItem* item;
	QString str;

	if (action_list->selectedItems().size() == 1)
	{
		item = action_list->selectedItems()[0];

		str = m_key_sequence.toString(QKeySequence::NativeText);

		item->setText(1, str);

		shortcut->setText(str);
	}
}

void PreferencesDialog::clear_shortcuts()
{
	m_key_sequence = QKeySequence();
	set_shortcut();
}

int PreferencesDialog::translate_modifiers(Qt::KeyboardModifiers state,
	const QString &text) const
{
	int result;

	result = 0;

	if ((state & Qt::ShiftModifier) && ((text.size() == 0) ||
		!text.at(0).isPrint() || text.at(0).isLetter() ||
		text.at(0).isSpace()))
	{
		result |= Qt::SHIFT;
	}

	if (state & Qt::ControlModifier)
	{
		result |= Qt::CTRL;
	}

	if (state & Qt::MetaModifier)
	{
		result |= Qt::META;
	}

	if (state & Qt::AltModifier)
	{
		result |= Qt::ALT;
	}

	return result;
}

void PreferencesDialog::handle_key_event(QKeyEvent *key_event)
{
	int next_key;

	next_key = key_event->key();

	if ((next_key == Qt::Key_Control) || (next_key == Qt::Key_Shift) ||
		(next_key == Qt::Key_Meta) || (next_key == Qt::Key_Alt) ||
		(next_key == Qt::Key_Super_L) || (next_key == Qt::Key_AltGr))
	{
		return;
	}

	next_key |= translate_modifiers(key_event->modifiers(),
		key_event->text());

	m_key_sequence = QKeySequence(next_key);

	set_shortcut();
}

bool PreferencesDialog::eventFilter(QObject *obj, QEvent *event)
{
	if ((obj == shortcut) && (event->type() == QEvent::KeyPress))
	{
		handle_key_event(static_cast<QKeyEvent*>(event));

		return true;
	}
	else
	{
		return false;
	}
}

void PreferencesDialog::set_actions(const QList<QAction*> &actions)
{
	QTreeWidgetItem* item;
	int index;

	action_list->clear();
	shortcut->clear();
	settings_tab->setCurrentIndex(0);
	m_actions.clear();

	foreach (QAction* action, actions)
	{
		if (action != 0)
		{
			if (!action->isSeparator() && (action->menu() == 0))
			{
				index = m_actions.size();
				m_actions.push_back(action);

				item = new QTreeWidgetItem(index);

				item->setText(0, action->data().toString());
				item->setIcon(0, action->icon());
				item->setText(1, action->shortcut().toString(
					QKeySequence::NativeText));

				action_list->addTopLevelItem(item);
			}
		}
	}
}

Qt::MouseButton PreferencesDialog::get_click_button() const
{
	return str_to_mouse_button(click_button->currentText());
}

Qt::KeyboardModifier PreferencesDialog::get_wheel_zoom_x10() const
{
	return str_to_key_mod(wheel_zoom_x10->currentText());
}

bool PreferencesDialog::get_swap_wheel_zoom() const
{
	return swap_wheel_zoom->isChecked();
}

void PreferencesDialog::set_click_button(const Qt::MouseButton value)
{
	click_button->setCurrentIndex(click_button->findText(
		mouse_button_to_str(value)));
}

void PreferencesDialog::set_wheel_zoom_x10(const Qt::KeyboardModifier value)
{
	wheel_zoom_x10->setCurrentIndex(wheel_zoom_x10->findText(
		key_mod_to_str(value)));
}

void PreferencesDialog::set_swap_wheel_zoom(const bool value)
{
	swap_wheel_zoom->setChecked(value);
}

Qt::MouseButton PreferencesDialog::str_to_mouse_button(const QString &str)
{
	if (str == "Left")
	{
		return Qt::LeftButton;
	}
	if (str == "Middle")
	{
		return Qt::MidButton;
	}
	if (str == "Right")
	{
		return Qt::RightButton;
	}
	return Qt::LeftButton;
}

Qt::KeyboardModifier PreferencesDialog::str_to_key_mod(const QString &str)
{
	if (str == "Disabled")
	{
		return Qt::NoModifier;
	}
	if (str == "Alt")
	{
		return Qt::AltModifier;
	}
	if (str == "Control")
	{
		return Qt::ControlModifier;
	}
	if (str == "Meta")
	{
		return Qt::MetaModifier;
	}
	if (str == "Shift")
	{
		return Qt::ShiftModifier;
	}
	if (str == "NumLock")
	{
		return Qt::KeypadModifier;
	}
	return Qt::NoModifier;
}

QString PreferencesDialog::mouse_button_to_str(const Qt::MouseButton value)
{
	switch (value)
	{
		case Qt::LeftButton:
			return "Left";
		case  Qt::MidButton:
			return "Middle";
		case Qt::RightButton:
			return "Right";
		default:
			return "Left";
	}
}

QString PreferencesDialog::key_mod_to_str(const Qt::KeyboardModifier value)
{
	switch (value)
	{
		case Qt::NoModifier:
			return "Disabled";
		case  Qt::AltModifier:
			return "Alt";
		case Qt::ControlModifier:
			return "Control";
		case Qt::MetaModifier:
			return "Meta";
		case Qt::ShiftModifier:
			return "Shift";
		case Qt::KeypadModifier:
			return "NumLock";
		default:
			return "Disabled";
	}
}

QSize PreferencesDialog::get_toolbar_icon_size() const
{
	if (icon_size_small->isChecked())
	{
		return icon_size_small->iconSize();
	}
	if (icon_size_medium->isChecked())
	{
		return icon_size_medium->iconSize();
	}
	if (icon_size_big->isChecked())
	{
		return icon_size_big->iconSize();
	}
	if (icon_size_verry_big->isChecked())
	{
		return icon_size_verry_big->iconSize();
	}

	return icon_size_medium->iconSize();
}

void PreferencesDialog::set_toolbar_icon_size(const QSize &size)
{
	if (size == icon_size_small->iconSize())
	{
		icon_size_small->setChecked(true);
		return;
	}
	if (size == icon_size_medium->iconSize())
	{
		icon_size_medium->setChecked(true);
		return;
	}
	if (size == icon_size_big->iconSize())
	{
		icon_size_big->setChecked(true);
		return;
	}
	if (size == icon_size_verry_big->iconSize())
	{
		icon_size_verry_big->setChecked(true);
		return;
	}

	icon_size_medium->setChecked(true);
}

void PreferencesDialog::set_dirs(const QStringList &dirs)
{
	m_dirs = dirs;
	m_dirs_model->setStringList(m_dirs);
	check_dir_enable();
}

void PreferencesDialog::set_textures(const QStringList &textures)
{
	m_textures = textures;
	m_textures_model->setStringList(m_textures);
	remove_texture_enable();
}
