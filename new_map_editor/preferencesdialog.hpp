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
		QStringListModel* m_textures_model;
		QStringListModel* m_dirs_model;
		QStringList m_textures;
		QStringList m_dirs;

		bool eventFilter(QObject *obj, QEvent *event);
		void handle_key_event(QKeyEvent *key_event);
		int translate_modifiers(Qt::KeyboardModifiers state,
			const QString &text) const;
		void set_shortcut();
		void key_sequence_changed();
		void remove_texture_enable();
		void check_dir_enable();

	public:
		PreferencesDialog(QWidget *parent = 0);
		void set_actions(const QList<QAction*> &actions);
		Qt::MouseButton get_click_button() const;
		Qt::KeyboardModifier get_wheel_zoom_x10() const;
		bool get_swap_wheel_zoom() const;
		QSize get_toolbar_icon_size() const;
		void set_click_button(const Qt::MouseButton value);
		void set_wheel_zoom_x10(const Qt::KeyboardModifier value);
		void set_swap_wheel_zoom(const bool value);
		void set_toolbar_icon_size(const QSize &size);
		void set_textures(const QStringList &textures);
		void set_dirs(const QStringList &dirs);

		inline const QStringList &get_textures() const
		{
			return m_textures;
		}

		inline const QStringList &get_dirs() const
		{
			return m_dirs;
		}

		static Qt::MouseButton str_to_mouse_button(const QString &str);
		static Qt::KeyboardModifier str_to_key_mod(const QString &str);
		static QString mouse_button_to_str(const Qt::MouseButton value);
		static QString key_mod_to_str(const Qt::KeyboardModifier value);

	public slots:
		void clear_shortcuts();
		void action_changed();
		void action_reset();
		void action_default();
		void set_action_shortcuts();
		void remove_texture();
		void add_dir();
		void remove_dir();
		void up_dir();
		void down_dir();

};

#endif	/* UUID_4ed3e5ab_3e46_4dee_8d41_54f85133a926 */
