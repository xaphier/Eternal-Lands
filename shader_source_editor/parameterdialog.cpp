/****************************************************************************
 *            parameterdialog.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "parameterdialog.hpp"
#include "shader/autoparameterutil.hpp"
#include "shader/commonparameterutil.hpp"
#include "shader/samplerparameterutil.hpp"
#include "shader/parameterqualifierutil.hpp"
#include "shader/parametersizeutil.hpp"
#include "shader/parameterutil.hpp"
#include "vertexelement.hpp"
#include <QSortFilterProxyModel>
#include <QPushButton>

namespace el = eternal_lands;

ParameterDialog::ParameterDialog(QWidget* parent): QDialog(parent)
{
	QSortFilterProxyModel* proxy;
	QString str;
	int i, count;

	setupUi(this);

	count = el::SamplerParameterUtil::get_sampler_parameter_count();

	for (i = 0; i < count; ++i)
	{
		str = QString::fromUtf8(el::SamplerParameterUtil::get_str(
			static_cast<el::SamplerParameterType>(i)).get(
				).c_str());

		sampler_name_value->addItem(str, i);
	}

	count = el::AutoParameterUtil::get_auto_parameter_count();

	for (i = 0; i < count; ++i)
	{
		str = QString::fromUtf8(el::AutoParameterUtil::get_str(
			static_cast<el::AutoParameterType>(i)).get().c_str());

		auto_name_value->addItem(str, i);
	}

	proxy = new QSortFilterProxyModel(auto_name_value);
	proxy->setSourceModel(auto_name_value->model());
	auto_name_value->model()->setParent(proxy);
	auto_name_value->setModel(proxy);
	auto_name_value->model()->sort(0);
	auto_name_value->setCurrentIndex(0);

	count = el::CommonParameterUtil::get_common_parameter_count();

	for (i = 0; i < count; ++i)
	{
		str = QString::fromUtf8(el::CommonParameterUtil::get_str(
			static_cast<el::CommonParameterType>(i)).get().c_str());

		common_name_value->addItem(str, i);
	}

	proxy = new QSortFilterProxyModel(common_name_value);
	proxy->setSourceModel(common_name_value->model());
	common_name_value->model()->setParent(proxy);
	common_name_value->setModel(proxy);
	common_name_value->model()->sort(0);
	common_name_value->setCurrentIndex(0);

	count = el::VertexElement::get_vertex_semantic_count();

	for (i = 0; i < count; ++i)
	{
		str = QString::fromUtf8(el::VertexElement::get_str(
			static_cast<el::VertexSemanticType>(i)).get().c_str());

		attribute_name_value->addItem(str, i);
	}

	proxy = new QSortFilterProxyModel(attribute_name_value);
	proxy->setSourceModel(attribute_name_value->model());
	attribute_name_value->model()->setParent(proxy);
	attribute_name_value->setModel(proxy);
	attribute_name_value->model()->sort(0);
	attribute_name_value->setCurrentIndex(0);

	count = el::ParameterQualifierUtil::get_parameter_qualifier_count();

	for (i = 0; i < count; ++i)
	{
		str = QString::fromUtf8(el::ParameterQualifierUtil::get_str(
			static_cast<el::ParameterQualifierType>(i)).get(
				).c_str());

		qualifier_value->addItem(str, i);
	}

	count = el::ParameterSizeUtil::get_parameter_size_count();

	for (i = 0; i < count; ++i)
	{
		str = QString::fromUtf8(el::ParameterSizeUtil::get_str(
			static_cast<el::ParameterSizeType>(i)).get().c_str());

		size_value->addItem(str, i);
	}

	proxy = new QSortFilterProxyModel(size_value);
	proxy->setSourceModel(size_value->model());
	size_value->model()->setParent(proxy);
	size_value->setModel(proxy);
	size_value->model()->sort(0);

	set_types(sampler_name->isChecked());

	QObject::connect(custom_name_value, SIGNAL(textChanged(const QString &)),
		this, SLOT(set_ok_button()));
	QObject::connect(auto_name_value,
		SIGNAL(currentIndexChanged(const QString &)), this,
		SLOT(set_auto_name_value(const QString &)));
	QObject::connect(common_name_value,
		SIGNAL(currentIndexChanged(const QString &)), this,
		SLOT(set_common_name_value(const QString &)));

	QObject::connect(custom_name, SIGNAL(toggled(bool)), this,
		SLOT(set_custom_name()));
	QObject::connect(sampler_name, SIGNAL(toggled(bool)), this,
		SLOT(set_sampler_name()));
	QObject::connect(auto_name, SIGNAL(toggled(bool)), this,
		SLOT(set_auto_name()));
	QObject::connect(common_name, SIGNAL(toggled(bool)), this,
		SLOT(set_common_name()));
	QObject::connect(attribute_name, SIGNAL(toggled(bool)), this,
		SLOT(set_attribute_name()));

	QObject::connect(custom_name, SIGNAL(toggled(bool)), this,
		SLOT(set_ok_button()));
	QObject::connect(sampler_name, SIGNAL(toggled(bool)), this,
		SLOT(set_ok_button()));
	QObject::connect(auto_name, SIGNAL(toggled(bool)), this,
		SLOT(set_ok_button()));
	QObject::connect(common_name, SIGNAL(toggled(bool)), this,
		SLOT(set_ok_button()));
	QObject::connect(attribute_name, SIGNAL(toggled(bool)), this,
		SLOT(set_ok_button()));

	QObject::connect(custom_name_value,
		SIGNAL(textChanged(const QString &)), this,
		SLOT(set_ok_button()));
	QObject::connect(sampler_name_value,
		SIGNAL(currentIndexChanged(const int)), this,
		SLOT(set_ok_button()));
	QObject::connect(auto_name_value,
		SIGNAL(currentIndexChanged(const int)), this,
		SLOT(set_ok_button()));
	QObject::connect(common_name_value,
		SIGNAL(currentIndexChanged(const int)), this,
		SLOT(set_ok_button()));
	QObject::connect(attribute_name_value,
		SIGNAL(currentIndexChanged(const int)), this,
		SLOT(set_ok_button()));
}

ParameterDialog::~ParameterDialog()
{
}

void ParameterDialog::setVisible(bool visible)
{
	set_ok_button();

	QDialog::setVisible(visible);
}

void ParameterDialog::set_custom_name()
{
	set_types(false);

	button_box->button(QDialogButtonBox::Ok)->setEnabled(
		!custom_name_value->text().isEmpty());

	custom_name_value->setFocus(Qt::OtherFocusReason);
}

void ParameterDialog::set_sampler_name()
{
	set_types(true);

	qualifier_value->setCurrentIndex(qualifier_value->findData(el::pqt_in));
	size_value->setCurrentIndex(size_value->findData(el::pst_one));
	scale_value->setValue(1);

	sampler_name_value->setFocus(Qt::OtherFocusReason);
}

void ParameterDialog::set_auto_name()
{
	set_types(false);

	set_auto_name_value(auto_name_value->currentText());

	auto_name_value->setFocus(Qt::OtherFocusReason);
}

void ParameterDialog::set_common_name()
{
	set_types(false);

	set_common_name_value(common_name_value->currentText());

	common_name_value->setFocus(Qt::OtherFocusReason);
}

void ParameterDialog::set_attribute_name()
{
	set_types(false);

	type_value->setCurrentIndex(type_value->findData(el::pt_vec4));
	qualifier_value->setCurrentIndex(qualifier_value->findData(el::pqt_in));
	size_value->setCurrentIndex(size_value->findData(el::pst_one));
	scale_value->setValue(1);

	attribute_name_value->setFocus(Qt::OtherFocusReason);
}

void ParameterDialog::set_ok_button()
{
	bool valid;

	valid = true;

	if (m_name != get_parameter_name())
	{
		emit check_name(get_parameter_name(), valid);
	}

	button_box->button(QDialogButtonBox::Ok)->setEnabled(
		(!custom_name_value->text().isEmpty() ||
		!custom_name->isChecked()) && valid);
}

void ParameterDialog::set_auto_name_value(const QString &name)
{
	el::AutoParameterType auto_parameter;

	auto_parameter = el::AutoParameterUtil::get_auto_parameter(
		el::String(name.toUtf8()));

	type_value->setCurrentIndex(type_value->findData(
		el::AutoParameterUtil::get_type(auto_parameter)));
	size_value->setCurrentIndex(size_value->findData(
		el::AutoParameterUtil::get_size(auto_parameter)));
	qualifier_value->setCurrentIndex(qualifier_value->findData(el::pqt_in));
	scale_value->setValue(el::AutoParameterUtil::get_scale(auto_parameter));
}

void ParameterDialog::set_common_name_value(const QString &name)
{
	el::CommonParameterType common_parameter;

	common_parameter = el::CommonParameterUtil::get_common_parameter(
		el::String(name.toUtf8()));

	type_value->setCurrentIndex(type_value->findData(
		el::CommonParameterUtil::get_type(common_parameter)));
	size_value->setCurrentIndex(size_value->findData(
		el::CommonParameterUtil::get_size(common_parameter)));
	scale_value->setValue(el::CommonParameterUtil::get_scale(
		common_parameter));
}

void ParameterDialog::set_types(const bool sampler)
{
	QString str;
	int i, count;
	el::ParameterType type;

	type_value->clear();

	count = el::ParameterUtil::get_parameter_count();

	for (i = 0; i < count; ++i)
	{
		type = static_cast<el::ParameterType>(i);

		if (sampler != el::ParameterUtil::get_sampler(type))
		{
			continue;
		}

		str = QString::fromUtf8(el::ParameterUtil::get_str(type).get(
			).c_str());

		type_value->addItem(str, i);
	}
}

bool ParameterDialog::edit_parameter(const QString &name, const QString &type,
	const QString &qualifier, const QString &size, const QString &scale)
{
	int type_index, qualifier_index, size_index, scale_int;
	int sampler_index, auto_index, common_index, attribute_index;
	el::CommonParameterType common_parameter;
	el::AutoParameterType auto_parameter;
	el::ParameterQualifierType parameter_qualifier;
	el::ParameterSizeType parameter_size;
	el::ParameterType parameter;

	m_name = name;
	type_index = type_value->findText(type);
	qualifier_index = qualifier_value->findText(qualifier);
	size_index = size_value->findText(size);
	scale_int = scale.toInt();

	sampler_index = sampler_name_value->findText(name);
	auto_index = auto_name_value->findText(name);
	common_index = common_name_value->findText(name);
	attribute_index = attribute_name_value->findText(name);

	parameter = el::pt_float;
	parameter_qualifier = el::pqt_in;
	parameter_size = el::pst_one;

	if (sampler_index != -1)
	{
		sampler_name_value->setCurrentIndex(sampler_index);
		sampler_name->setChecked(true);

		parameter_size = el::pst_one;
		scale_int = 1;
		parameter_qualifier = el::pqt_in;

		size_index = -2;
		qualifier_index = -2;
	}

	if (auto_index != -1)
	{
		auto_name_value->setCurrentIndex(auto_index);
		auto_name->setChecked(true);

		auto_parameter = el::AutoParameterUtil::get_auto_parameter(
			el::String(name.toUtf8()));

		parameter = el::AutoParameterUtil::get_type(auto_parameter);
		parameter_size = el::AutoParameterUtil::get_size(
			auto_parameter);
		scale_int = el::AutoParameterUtil::get_scale(auto_parameter);
		parameter_qualifier = el::pqt_in;

		type_index = -2;
		size_index = -2;
		qualifier_index = -2;
	}

	if (common_index != -1)
	{
		common_name_value->setCurrentIndex(common_index);
		common_name->setChecked(true);

		common_parameter =
			el::CommonParameterUtil::get_common_parameter(
				el::String(name.toUtf8()));

		parameter = el::CommonParameterUtil::get_type(common_parameter);
		parameter_size = el::CommonParameterUtil::get_size(
			common_parameter);
		scale_int = el::CommonParameterUtil::get_scale(
			common_parameter);

		type_index = -2;
		size_index = -2;
	}

	if (attribute_index != -1)
	{
		attribute_name_value->setCurrentIndex(attribute_index);
		attribute_name->setChecked(true);

		parameter_size = el::pst_one;
		scale_int = 1;
		parameter_qualifier = el::pqt_in;

		parameter = el::pt_vec4;
		type_index = -2;
		size_index = -2;
		qualifier_index = -2;
	}

	if ((sampler_index == -1) && (auto_index == -1) &&
		(common_index == -1) && (attribute_index == -1))
	{
		custom_name_value->setText(name);
		custom_name->setChecked(true);
	}

	if (type_index == -2)
	{
		type_index = type_value->findData(parameter);
	}

	if (qualifier_index == -2)
	{
		qualifier_index = qualifier_value->findData(parameter_qualifier);
	}

	if (size_index == -2)
	{
		size_index = size_value->findData(parameter_size);
	}

	type_value->setCurrentIndex(type_index);
	qualifier_value->setCurrentIndex(qualifier_index);
	size_value->setCurrentIndex(size_index);
	scale_value->setValue(scale_int);

	return exec() == QDialog::Accepted;
}

bool ParameterDialog::add_parameter()
{
	m_name.clear();

	return exec() == QDialog::Accepted;
}

QString ParameterDialog::get_parameter_name() const
{
	if (sampler_name->isChecked())
	{
		return sampler_name_value->currentText();
	}

	if (auto_name->isChecked())
	{
		return auto_name_value->currentText();
	}

	if (common_name->isChecked())
	{
		return common_name_value->currentText();
	}

	if (attribute_name->isChecked())
	{
		return attribute_name_value->currentText();
	}

	return custom_name_value->text();
}

QString ParameterDialog::get_parameter_type() const
{
	return type_value->currentText();
}

QString ParameterDialog::get_parameter_qualifier() const
{
	return qualifier_value->currentText();
}

QString ParameterDialog::get_parameter_size() const
{
	return size_value->currentText();
}

QString ParameterDialog::get_parameter_scale() const
{
	return QString::number(scale_value->value());
}
