#ifndef	UUID_4ed3e5ab_3e46_4dee_8d41_54f85133a926
#define UUID_4ed3e5ab_3e46_4dee_8d41_54f85133a926

#include "ui_preferences.h"
#include <QDialog>
#include <QStringListModel>

class PreferencesDialog: public QDialog, public Ui::Preferences
{
	Q_OBJECT

	private:
		QList<QAction*> m_actions;
		QKeySequence m_key_sequence;
		QString m_el_data_dir;
		QString m_el_extra_data_dir;
		Qt::Key m_rotate_x_key;
		Qt::Key m_rotate_y_key;
		Qt::Key m_rotate_z_key;
		Qt::Key m_scale_key;

		bool eventFilter(QObject *obj, QEvent *event);
		void handle_key_event(QKeyEvent *key_event);
		void handle_rotate_x_key_event(QKeyEvent *key_event);
		void handle_rotate_y_key_event(QKeyEvent *key_event);
		void handle_rotate_z_key_event(QKeyEvent *key_event);
		void handle_scale_key_event(QKeyEvent *key_event);
		int translate_modifiers(Qt::KeyboardModifiers state,
			const QString &text) const;
		void set_shortcut();
		void set_rotate_x_key();
		void set_rotate_y_key();
		void set_rotate_z_key();
		void set_scale_key();
		void key_sequence_changed();
		void remove_texture_enable();

	private slots:
		void clear_shortcuts();
		void action_changed();
		void action_reset();
		void action_default();
		void set_action_shortcuts();
		void change_el_data_dir();
		void change_el_extra_data_dir();

		void change_select_rect_button(const int index);
		void change_click_button(const int index);
		void change_grab_button(const int index);
		void change_view_rotate_button(const int index);
		void change_click_key_mod(const int index);
		void change_select_rect_key_mod(const int index);
		void change_grab_key_mod(const int index);
		void change_view_rotate_key_mod(const int index);

	public:
		PreferencesDialog(QWidget *parent = 0);
		virtual ~PreferencesDialog();
		void set_actions(const QList<QAction*> &actions);
		Qt::MouseButton get_select_rect_button() const;
		Qt::MouseButton get_click_button() const;
		Qt::MouseButton get_grab_button() const;
		Qt::MouseButton get_view_rotate_button() const;
		Qt::KeyboardModifier get_select_rect_key_mod() const;
		Qt::KeyboardModifier get_click_key_mod() const;
		Qt::KeyboardModifier get_grab_key_mod() const;
		Qt::KeyboardModifier get_view_rotate_key_mod() const;
		Qt::KeyboardModifier get_wheel_zoom_x10() const;
		bool get_swap_wheel_zoom() const;
		bool get_invert_z_rotation() const;
		QSize get_toolbar_icon_size() const;
		QSize get_texture_preview_size() const;
		void set_select_rect_button(const Qt::MouseButton value);
		void set_click_button(const Qt::MouseButton value);
		void set_grab_button(const Qt::MouseButton value);
		void set_view_rotate_button(const Qt::MouseButton value);
		void set_select_rect_key_mod(
			const Qt::KeyboardModifier value);
		void set_click_key_mod(const Qt::KeyboardModifier value);
		void set_grab_key_mod(const Qt::KeyboardModifier value);
		void set_view_rotate_key_mod(
			const Qt::KeyboardModifier value);
		void set_wheel_zoom_x10(const Qt::KeyboardModifier value);
		void set_swap_wheel_zoom(const bool value);
		void set_toolbar_icon_size(const QSize &size);
		void set_texture_preview_size(const QSize &size);
		void set_textures(const QStringList &textures);
		void set_el_data_dir(const QString &el_data_dir);
		void set_el_extra_data_dir(const QString &el_extra_data_dir);
		void set_rotate_x_key(const Qt::Key rotate_x_key);
		void set_rotate_y_key(const Qt::Key rotate_y_key);
		void set_rotate_z_key(const Qt::Key rotate_z_key);
		void set_scale_key(const Qt::Key scale_key);
		void set_invert_z_rotation(const bool value);
		void disable_all_mouse_button_checks();
		void enable_all_mouse_button_checks();

		inline const QString &get_el_data_dir() const
		{
			return m_el_data_dir;
		}

		inline const QString &get_el_extra_data_dir() const
		{
			return m_el_extra_data_dir;
		}

		inline Qt::Key get_rotate_x_key() const
		{
			return m_rotate_x_key;
		}

		inline Qt::Key get_rotate_y_key() const
		{
			return m_rotate_y_key;
		}

		inline Qt::Key get_rotate_z_key() const
		{
			return m_rotate_z_key;
		}

		inline Qt::Key get_scale_key() const
		{
			return m_scale_key;
		}

		static Qt::MouseButton str_to_mouse_button(const QString &str);
		static Qt::KeyboardModifier str_to_key_mod(const QString &str);
		static Qt::Key str_to_key(const QString &str);
		static QString mouse_button_to_str(const Qt::MouseButton value);
		static QString key_mod_to_str(const Qt::KeyboardModifier value);
		static QString key_to_str(const Qt::Key value);

};

#endif	/* UUID_4ed3e5ab_3e46_4dee_8d41_54f85133a926 */
