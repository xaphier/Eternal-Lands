/****************************************************************************
 *            widget.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_56fc74f1_e8cf_45b4_9830_4f2246d56054
#define	UUID_56fc74f1_e8cf_45b4_9830_4f2246d56054

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class Widget.
 * This file contains the @c class Widget.
 */
namespace eternal_lands
{

	class Widget;

	typedef boost::weak_ptr<Widget> WidgetWeakPtr;
	typedef boost::shared_ptr<Widget> WidgetSharedPtr;
	typedef std::vector<WidgetSharedPtr> WidgetSharedPtrVector;

	enum ApplicationStateType
	{
		ast_mouse_focus = 1,
		ast_input_focus = 2,
		ast_active = 4
	};

	enum MouseButtonType
	{
		mbt_left = 0,
		mbt_middle = 1,
		mbt_right = 2,
		mbt_wheel_up = 3,
		mbt_wheel_down = 4,
		mbt_x0 = 5,
		mbt_x1 = 6,
		mbt_x2 = 7,
		mbt_x3 = 8
	};

	enum KeyStateType
	{
		kst_left_shift = 0,
		kst_right_shift = 1,
		kst_left_control = 2,
		kst_right_control = 3,
		kst_left_alt = 4,
		kst_right_alt = 5,
		kst_left_meta = 6,
		kst_right_meta = 7,
		kst_numlock = 8,
		kst_caps = 9,
		kst_mode = 10
	};

	typedef std::bitset<11> KeyState;
	typedef std::bitset<8> MouseButtonState;

	class GuiUtil;

	/**
	 * @brief @c class for widgets.
	 *
	 * @c class for widgets.
	 */
	class Widget: public boost::enable_shared_from_this<Widget>,
		 public boost::noncopyable
	{
		private:
			WidgetWeakPtr m_parent;
			WidgetSharedPtrVector m_children;
			Uint16 m_x;
			Uint16 m_y;
			Uint16 m_height;
			Uint16 m_width;
			bool m_visible;
			bool m_enabled;
			bool m_can_focus;
			bool m_focus;
			bool m_selected;
			bool m_mouse_over;

		protected:
			Widget(const WidgetWeakPtr m_parent);
			void on_active_event(const ApplicationStateType state,
				const bool status);
			void on_key_down(const SDLKey sym,
				const KeyState key_mod, const wchar_t key);
			void on_key_up(const SDLKey sym,
				const KeyState key_mod, const wchar_t key);
			void on_mouse_motion(const Uint16 x, const Uint16 y,
				const MouseButtonState buttons,
				const KeyState keys);
			void on_mouse_button_down(const Uint16 x,
				const Uint16 y, const MouseButtonType button,
				const KeyState key_mod);
			void on_mouse_button_up(const Uint16 x,
				const Uint16 y, const MouseButtonType button,
				const KeyState key_mod);

			virtual void active_event(
				const ApplicationStateType state,
				const bool status);
			virtual void key_down(const SDLKey sym,
				const KeyState key_mod, const wchar_t key);
			virtual void key_up(const SDLKey sym,
				const KeyState key_mod, const wchar_t key);
			virtual void mouse_motion(const Uint16 x,
				const Uint16 y, const MouseButtonState buttons,
				const KeyState keys);
			virtual void mouse_button_down(const Uint16 x,
				const Uint16 y, const MouseButtonType button,
				const KeyState key_mod);
			virtual void mouse_button_up(const Uint16 x,
				const Uint16 y, const MouseButtonType button,
				const KeyState key_mod);

			virtual void mouse_enter();
			virtual void mouse_leave();
			virtual void clicked();
			virtual void selected();
			virtual void context_menu();
			virtual void text_key(const wchar_t key);
			virtual void do_draw(GuiUtil &gui);
			void draw(GuiUtil &gui);

			inline WidgetSharedPtr get_shared_from_this()
			{
				return shared_from_this();
			}

			inline const WidgetSharedPtrVector &get_children() const
			{
				return m_children;
			}

			inline const WidgetSharedPtr &get_child(
				const Uint32 index) const
			{
				return m_children[index];
			}

		public:
			virtual ~Widget();

			inline bool is_inside(const Uint16 x, const Uint16 y)
			{
				return (x >= get_x()) && (y >= get_y()) &&
					(x < (get_x() + get_width())) &&
					(y< (get_y() + get_height()));
			}

			inline Uint32 get_x() const
			{
				return m_x;
			}

			inline Uint32 get_y() const
			{
				return m_y;
			}

			inline Uint32 get_height() const
			{
				return m_height;
			}

			inline Uint32 get_width() const
			{
				return m_width;
			}

			inline bool get_visible() const
			{
				return m_visible;
			}

			inline bool get_enabled() const
			{
				return m_enabled;
			}

			inline bool get_can_focus() const
			{
				return m_can_focus;
			}

			inline bool get_focus() const
			{
				return m_focus;
			}

			inline bool get_selected() const
			{
				return m_selected;
			}

			inline bool get_mouse_over() const
			{
				return m_mouse_over;
			}

			inline WidgetSharedPtr get_parent() const
			{
				return m_parent.lock();
			}

			inline bool get_has_parent() const
			{
				return m_parent.lock() != 0;
			}

			inline bool get_parent_is_visible() const
			{
				if (get_has_parent())
				{
					return get_parent()->get_visible();
				}

				return true;
			}

			inline bool get_parent_is_enabled() const
			{
				if (get_has_parent())
				{
					return get_parent()->get_enabled();
				}

				return true;
			}

			inline bool get_is_visible() const
			{
				return get_visible() && get_parent_is_visible();
			}

			inline bool get_is_enabled() const
			{
				return get_enabled() && get_parent_is_enabled();
			}

	};

}

#endif	/* UUID_56fc74f1_e8cf_45b4_9830_4f2246d56054 */
