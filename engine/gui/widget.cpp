/****************************************************************************
 *            widget.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "widget.hpp"

namespace eternal_lands
{

	Widget::Widget(const WidgetWeakPtr parent): m_parent(parent)
	{
	}

	Widget::~Widget()
	{
	}

	void Widget::draw(GuiUtil &gui)
	{
		do_draw(gui);

		BOOST_FOREACH(const WidgetSharedPtr &widget, get_children())
		{
			if (widget->get_visible())
			{
				widget->draw(gui);
			}
		}
	}

	void Widget::on_active_event(const ApplicationStateType state,
		const bool status)
	{
	}
/*
	void Widget::on_key_down(const SDLKey sym, const KeyState key_mod,
		const wchar_t key)
	{
	}

	void Widget::on_key_up(const SDLKey sym, const KeyState key_mod,
		const wchar_t key)
	{
	}
*/
	void Widget::on_mouse_motion(const Uint16 x, const Uint16 y,
		const MouseButtonState buttons, const KeyState keys)
	{
		bool mouse_over;

		BOOST_FOREACH(const WidgetSharedPtr &widget, get_children())
		{
			widget->on_mouse_motion(x, y, buttons, keys);
		}

		mouse_over = is_inside(x, y);

		if (mouse_over != get_mouse_over())
		{
			m_mouse_over = mouse_over;

			if (get_mouse_over())
			{
				mouse_enter();
			}
			else
			{
				mouse_leave();
			}
		}
	}

	void Widget::on_mouse_button_down(const Uint16 x, const Uint16 y,
		const MouseButtonType button, const KeyState keys)
	{
		BOOST_FOREACH(const WidgetSharedPtr &widget, get_children())
		{
			widget->on_mouse_button_down(x, y, button, keys);
		}

		if (is_inside(x, y))
		{
			if (button == mbt_left)
			{
				m_selected = true;
			}

			if (button == mbt_right)
			{
				context_menu();
			}
		}
	}

	void Widget::on_mouse_button_up(const Uint16 x, const Uint16 y,
		const MouseButtonType button, const KeyState keys)
	{
		BOOST_FOREACH(const WidgetSharedPtr &widget, get_children())
		{
			widget->on_mouse_button_up(x, y, button, keys);
		}

		if (is_inside(x, y))
		{
			if ((button == mbt_left) && get_selected())
			{
				clicked();
			}
		}

		m_selected = false;
	}

	void Widget::mouse_enter()
	{
	}

	void Widget::mouse_leave()
	{
	}

	void Widget::clicked()
	{
	}

	void Widget::text_key(const wchar_t key)
	{
	}

	void Widget::do_draw(GuiUtil &gui)
	{
	}

}
