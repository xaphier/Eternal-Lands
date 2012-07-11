#include "valuesdialog.hpp"

ValuesDialog::ValuesDialog(QWidget *parent): QDialog(parent)
{
	setupUi(this);
}

ValuesDialog::~ValuesDialog()
{
}

void ValuesDialog::set_values(const QVector4D &values)
{
	x_value->setValue(values.x());
	y_value->setValue(values.y());
	z_value->setValue(values.z());
	w_value->setValue(values.w());
}

QVector4D ValuesDialog::get_values() const
{
	return QVector4D(x_value->value(), y_value->value(), z_value->value(),
		w_value->value());
}

void ValuesDialog::set_value_count(const int count)
{
	x_value->setEnabled(count > 0);
	x_label->setEnabled(count > 0);
	y_value->setEnabled(count > 1);
	y_label->setEnabled(count > 1);
	z_value->setEnabled(count > 2);
	z_label->setEnabled(count > 2);
	w_value->setEnabled(count > 3);
	w_label->setEnabled(count > 3);
}
