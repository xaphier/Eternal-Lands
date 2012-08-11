#include "textureunitdialog.hpp"
#include "../engine/shader/samplerparameterutil.hpp"
#include "../engine/texturetargetutil.hpp"
#include <QFileDialog>

namespace el = eternal_lands;

TextureUnitDialog::TextureUnitDialog(QWidget *parent): QDialog(parent)
{
	QStringList targets;

	setupUi(this);

	targets << QString::fromUtf8(el::TextureTargetUtil::get_description(
		el::ttt_texture_1d).get().c_str());
	targets << QString::fromUtf8(el::TextureTargetUtil::get_description(
		el::ttt_texture_2d).get().c_str());
	targets << QString::fromUtf8(el::TextureTargetUtil::get_description(
		el::ttt_texture_3d).get().c_str());
	targets << QString::fromUtf8(el::TextureTargetUtil::get_description(
		el::ttt_texture_cube_map).get().c_str());
	targets << QString::fromUtf8(el::TextureTargetUtil::get_description(
		el::ttt_texture_1d_array).get().c_str());
	targets << QString::fromUtf8(el::TextureTargetUtil::get_description(
		el::ttt_texture_2d_array).get().c_str());
	targets << QString::fromUtf8(el::TextureTargetUtil::get_description(
		el::ttt_texture_cube_map_array).get().c_str());

	target->addItems(targets);

	connect(add, SIGNAL(clicked(bool)), this,
		SLOT(add_image()));
	connect(remove, SIGNAL(clicked(bool)), this,
		SLOT(remove_image()));
	connect(target, SIGNAL(currentIndexChanged(int)), this,
		SLOT(check()));
	connect(images, SIGNAL(itemSelectionChanged()), this,
		SLOT(check()));
	connect(button_box, SIGNAL(clicked(QAbstractButton*)), this,
		SLOT(button_clicked(QAbstractButton*)));
	connect(name, SIGNAL(textChanged(QString)), this,
		SLOT(check()));

	check();
}

TextureUnitDialog::~TextureUnitDialog()
{
}

void TextureUnitDialog::button_clicked(QAbstractButton* button)
{
	if (button_box->standardButton(button) == QDialogButtonBox::Reset)
	{
		name->setText(QString::fromUtf8(
			el::SamplerParameterUtil::get_str(
				static_cast<el::SamplerParameterType>(
					m_texture_unit)).get().c_str()));
		target->setCurrentIndex(el::ttt_texture_2d);
		images->clear();
		accept();
	}
}

QString TextureUnitDialog::get_name() const
{
	return name->text();
}

int TextureUnitDialog::get_target() const
{
	return target->currentIndex();
}

int TextureUnitDialog::get_texture_unit() const
{
	return m_texture_unit;
}

QStringList TextureUnitDialog::get_file_names() const
{
	QStringList result;
	int i, count;

	count = images->count();

	for (i = 0; i < count; ++i)
	{
		result << images->item(i)->text();
	}

	return result;
}

void TextureUnitDialog::set_name(const QString &value)
{
	name->setText(value);

	check();
}

void TextureUnitDialog::set_target(const int value)
{
	target->setCurrentIndex(value);

	check();
}

void TextureUnitDialog::set_texture_unit(const int texture_unit)
{
	m_texture_unit = texture_unit;
}

void TextureUnitDialog::set_file_names(const QStringList &value)
{
	images->clear();
	images->addItems(value);

	check();
}

void TextureUnitDialog::add_image()
{
	QStringList names;

	names = QFileDialog::getOpenFileNames(this, "images", QString(),
		"Images (*.dds *.png *.jpg *.jpeg)");

	if (names.count() > 0)
	{
		images->addItems(names);

		check();
	}
}

void TextureUnitDialog::remove_image()
{
	BOOST_FOREACH(QListWidgetItem* item, images->selectedItems())
	{
		delete item;
	}

	check();
}

void TextureUnitDialog::check()
{
	bool enabled;

	enabled = !name->text().isEmpty();

	switch (target->currentIndex())
	{
		case el::ttt_texture_1d:
		case el::ttt_texture_2d:
		case el::ttt_texture_3d:
		case el::ttt_texture_cube_map:
		case el::ttt_texture_rectangle:
			enabled &= images->count() == 1;
			break;
		case el::ttt_texture_1d_array:
		case el::ttt_texture_2d_array:
		case el::ttt_texture_cube_map_array:
			enabled &= images->count() >= 1;
			break;
		case el::ttt_texture_2d_multisample:
		case el::ttt_texture_2d_multisample_array:
			enabled = false;
			break;
	}

	button_box->button(QDialogButtonBox::Ok)->setEnabled(enabled);
	remove->setEnabled(images->selectedItems().count() > 0);
}
