#include "directiondialog.hpp"
#include <cmath>

namespace
{

	class UpdatingLock
	{
		private:
			bool &m_updating;
			bool m_locked;

		public:
			inline UpdatingLock(bool &updating):
				m_updating(updating), m_locked(m_updating)
			{
				m_updating = true;
			}

			inline ~UpdatingLock()
			{
				m_updating = get_locked();
			}

			inline bool get_locked() const
			{
				return m_locked;
			}

	};

}

DirectionDialog::DirectionDialog(QWidget *parent): QDialog(parent)
{
	setupUi(this);

	m_updating = false;

	connect(x_value, SIGNAL(valueChanged(double)), this,
		SLOT(x_value_change(double)));
	connect(y_value, SIGNAL(valueChanged(double)), this,
		SLOT(y_value_change(double)));
	connect(z_value, SIGNAL(valueChanged(int)), this,
		SLOT(z_value_change(int)));
}

DirectionDialog::~DirectionDialog()
{
}

void DirectionDialog::set_direction(const QVector2D &dir)
{
	x_value->setValue(dir.x());
	y_value->setValue(dir.y());
}

QVector2D DirectionDialog::get_direction() const
{
	return QVector2D(x_value->value(), y_value->value());
}

void DirectionDialog::x_value_change(const double value)
{
	UpdatingLock lock(m_updating);
	float tmp;

	if (lock.get_locked())
	{
		return;
	}

	tmp = std::sqrt(1.0 - value * value);

	if (y_value->value() < 0.0)
	{
		y_value->setValue(-tmp);
	}
	else
	{
		y_value->setValue(tmp);
	}

	tmp = std::asin(value);

	z_value->setValue((tmp * 360.0) / (2.0 * M_PI));
}

void DirectionDialog::y_value_change(const double value)
{
	UpdatingLock lock(m_updating);
	float tmp;

	if (lock.get_locked())
	{
		return;
	}

	tmp = std::sqrt(1.0 - value * value);

	if (x_value->value() < 0.0)
	{
		x_value->setValue(-tmp);
	}
	else
	{
		x_value->setValue(tmp);
	}

	tmp = std::acos(value);

	z_value->setValue((tmp * 360.0) / (2.0 * M_PI));
}

void DirectionDialog::z_value_change(const int value)
{
	UpdatingLock lock(m_updating);
	float x, y;

	if (lock.get_locked())
	{
		return;
	}

	x = std::sin((value / 360.0) * 2.0 * M_PI);
	y = std::cos((value / 360.0) * 2.0 * M_PI);

	x_value->setValue(x);
	y_value->setValue(y);
}
