#include "preferencesdialog.hpp"
#include <QKeyEvent>
#include <QFileDialog>

PreferencesDialog::PreferencesDialog(QWidget *parent): QDialog(parent)
{
	setupUi(this);

	shortcut->installEventFilter(this);
	rotate_x_key->installEventFilter(this);
	rotate_y_key->installEventFilter(this);
	rotate_z_key->installEventFilter(this);
	scale_key->installEventFilter(this);
	QObject::connect(clear_button, SIGNAL(clicked()), this, SLOT(clear_shortcuts()));
	QObject::connect(reset_button, SIGNAL(clicked()), this, SLOT(action_reset()));
	QObject::connect(default_button, SIGNAL(clicked()), this, SLOT(action_default()));
	QObject::connect(ok_button, SIGNAL(clicked()), this, SLOT(set_action_shortcuts()));
	QObject::connect(action_list, SIGNAL(itemSelectionChanged()), this, SLOT(action_changed()));
	QObject::connect(el_data_dir_button, SIGNAL(clicked()), this, SLOT(change_el_data_dir()));
	QObject::connect(el_extra_data_dir_button, SIGNAL(clicked()), this, SLOT(change_el_extra_data_dir()));

	click_button->addItem(mouse_button_to_str(Qt::LeftButton));
	click_button->addItem(mouse_button_to_str(Qt::MidButton));
	click_button->addItem(mouse_button_to_str(Qt::RightButton));

	wheel_zoom_x10->addItem(key_mod_to_str(Qt::NoModifier));
	wheel_zoom_x10->addItem(key_mod_to_str(Qt::AltModifier));
	wheel_zoom_x10->addItem(key_mod_to_str(Qt::ControlModifier));
	wheel_zoom_x10->addItem(key_mod_to_str(Qt::MetaModifier));
	wheel_zoom_x10->addItem(key_mod_to_str(Qt::ShiftModifier));
	wheel_zoom_x10->addItem(key_mod_to_str(Qt::KeypadModifier));
}

PreferencesDialog::~PreferencesDialog()
{
}

void PreferencesDialog::change_el_data_dir()
{
	QString dir;

	dir = QFileDialog::getExistingDirectory(this, "Select directory",
		m_el_data_dir);

	if (!dir.isEmpty())
	{
		set_el_data_dir(dir);
	}
}

void PreferencesDialog::change_el_extra_data_dir()
{
	QString dir;

	dir = QFileDialog::getExistingDirectory(this, "Select directory",
		m_el_extra_data_dir);

	if (!dir.isEmpty())
	{
		set_el_extra_data_dir(dir);
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

void PreferencesDialog::set_rotate_x_key()
{
	QKeySequence key_sequence;
	QString str;

	key_sequence = QKeySequence(m_rotate_x_key);

	str = key_sequence.toString(QKeySequence::NativeText);

	rotate_x_key->setText(str);
}

void PreferencesDialog::set_rotate_y_key()
{
	QKeySequence key_sequence;
	QString str;

	key_sequence = QKeySequence(m_rotate_y_key);

	str = key_sequence.toString(QKeySequence::NativeText);

	rotate_y_key->setText(str);
}

void PreferencesDialog::set_rotate_z_key()
{
	QKeySequence key_sequence;
	QString str;

	key_sequence = QKeySequence(m_rotate_z_key);

	str = key_sequence.toString(QKeySequence::NativeText);

	rotate_z_key->setText(str);
}

void PreferencesDialog::set_scale_key()
{
	QKeySequence key_sequence;
	QString str;

	key_sequence = QKeySequence(m_scale_key);

	str = key_sequence.toString(QKeySequence::NativeText);

	scale_key->setText(str);
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

void PreferencesDialog::handle_rotate_x_key_event(QKeyEvent *key_event)
{
	int next_key;

	next_key = key_event->key();

	if ((next_key == Qt::Key_Control) || (next_key == Qt::Key_Shift) ||
		(next_key == Qt::Key_Meta) || (next_key == Qt::Key_Alt) ||
		(next_key == Qt::Key_Super_L) || (next_key == Qt::Key_AltGr))
	{
		return;
	}

	set_rotate_x_key(static_cast<Qt::Key>(next_key));
}

void PreferencesDialog::handle_rotate_y_key_event(QKeyEvent *key_event)
{
	int next_key;

	next_key = key_event->key();

	if ((next_key == Qt::Key_Control) || (next_key == Qt::Key_Shift) ||
		(next_key == Qt::Key_Meta) || (next_key == Qt::Key_Alt) ||
		(next_key == Qt::Key_Super_L) || (next_key == Qt::Key_AltGr))
	{
		return;
	}

	set_rotate_y_key(static_cast<Qt::Key>(next_key));
}

void PreferencesDialog::handle_rotate_z_key_event(QKeyEvent *key_event)
{
	int next_key;

	next_key = key_event->key();

	if ((next_key == Qt::Key_Control) || (next_key == Qt::Key_Shift) ||
		(next_key == Qt::Key_Meta) || (next_key == Qt::Key_Alt) ||
		(next_key == Qt::Key_Super_L) || (next_key == Qt::Key_AltGr))
	{
		return;
	}

	set_rotate_z_key(static_cast<Qt::Key>(next_key));
}

void PreferencesDialog::handle_scale_key_event(QKeyEvent *key_event)
{
	int next_key;

	next_key = key_event->key();

	if ((next_key == Qt::Key_Control) || (next_key == Qt::Key_Shift) ||
		(next_key == Qt::Key_Meta) || (next_key == Qt::Key_Alt) ||
		(next_key == Qt::Key_Super_L) || (next_key == Qt::Key_AltGr))
	{
		return;
	}

	set_scale_key(static_cast<Qt::Key>(next_key));
}

bool PreferencesDialog::eventFilter(QObject *obj, QEvent *event)
{
	if ((obj == shortcut) && (event->type() == QEvent::KeyPress))
	{
		handle_key_event(static_cast<QKeyEvent*>(event));

		return true;
	}

	if ((obj == rotate_x_key) && (event->type() == QEvent::KeyPress))
	{
		handle_rotate_x_key_event(static_cast<QKeyEvent*>(event));

		return true;
	}

	if ((obj == rotate_y_key) && (event->type() == QEvent::KeyPress))
	{
		handle_rotate_y_key_event(static_cast<QKeyEvent*>(event));

		return true;
	}

	if ((obj == rotate_z_key) && (event->type() == QEvent::KeyPress))
	{
		handle_rotate_z_key_event(static_cast<QKeyEvent*>(event));

		return true;
	}

	if ((obj == scale_key) && (event->type() == QEvent::KeyPress))
	{
		handle_scale_key_event(static_cast<QKeyEvent*>(event));

		return true;
	}

	return false;
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

bool PreferencesDialog::get_invert_z_rotation() const
{
	return invert_z_rotation->isChecked();
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

void PreferencesDialog::set_invert_z_rotation(const bool value)
{
	invert_z_rotation->setChecked(value);
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

Qt::Key PreferencesDialog::str_to_key(const QString &str)
{
	QKeySequence key;

	key = QKeySequence(str);

	if (key.count() > 0)
	{
		return static_cast<Qt::Key>(key[0]);
	}

	return Qt::Key_unknown;
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

QString PreferencesDialog::key_to_str(const Qt::Key value)
{
	QKeySequence key;

	key = QKeySequence(value);

	return key.toString();
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
	if (icon_size_very_big->isChecked())
	{
		return icon_size_very_big->iconSize();
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
	if (size == icon_size_very_big->iconSize())
	{
		icon_size_very_big->setChecked(true);
		return;
	}

	icon_size_medium->setChecked(true);
}

QSize PreferencesDialog::get_texture_preview_size() const
{
	if (texture_size_very_small->isChecked())
	{
		return texture_size_very_small->iconSize();
	}
	if (texture_size_small->isChecked())
	{
		return texture_size_small->iconSize();
	}
	if (texture_size_medium->isChecked())
	{
		return texture_size_medium->iconSize();
	}
	if (texture_size_big->isChecked())
	{
		return texture_size_big->iconSize();
	}
	if (texture_size_very_big->isChecked())
	{
		return texture_size_very_big->iconSize();
	}

	return texture_size_medium->iconSize();
}

void PreferencesDialog::set_texture_preview_size(const QSize &size)
{
	if (size == texture_size_very_small->iconSize())
	{
		texture_size_very_small->setChecked(true);
		return;
	}
	if (size == texture_size_small->iconSize())
	{
		texture_size_small->setChecked(true);
		return;
	}
	if (size == texture_size_medium->iconSize())
	{
		texture_size_medium->setChecked(true);
		return;
	}
	if (size == texture_size_big->iconSize())
	{
		texture_size_big->setChecked(true);
		return;
	}
	if (size == texture_size_very_big->iconSize())
	{
		texture_size_very_big->setChecked(true);
		return;
	}

	texture_size_medium->setChecked(true);
}

void PreferencesDialog::set_el_data_dir(const QString &el_data_dir)
{
	m_el_data_dir = el_data_dir;

	el_data_dir_button->setText(el_data_dir);
}

void PreferencesDialog::set_el_extra_data_dir(const QString &el_extra_data_dir)
{
	m_el_extra_data_dir = el_extra_data_dir;

	el_extra_data_dir_button->setText(el_extra_data_dir);
}

void PreferencesDialog::set_rotate_x_key(const Qt::Key rotate_x_key)
{
	m_rotate_x_key = rotate_x_key;

	set_rotate_x_key();
}

void PreferencesDialog::set_rotate_y_key(const Qt::Key rotate_y_key)
{
	m_rotate_y_key = rotate_y_key;

	set_rotate_y_key();
}

void PreferencesDialog::set_rotate_z_key(const Qt::Key rotate_z_key)
{
	m_rotate_z_key = rotate_z_key;

	set_rotate_z_key();
}

void PreferencesDialog::set_scale_key(const Qt::Key scale_key)
{
	m_scale_key = scale_key;

	set_scale_key();
}
