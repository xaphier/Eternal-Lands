#include "texturedialog.hpp"

TextureDialog::TextureDialog(QWidget *parent): QDialog(parent)
{
	setupUi(this);

	connect(texture_unit, SIGNAL(valueChanged(int)), this,
		SLOT(texture_unit_change(int)));
	connect(projection, SIGNAL(toggled(bool)), this,
		SLOT(projection_change(bool)));
//	connect(z_value, SIGNAL(valueChanged(int)), this,
//		SLOT(texture_change(int)));
}

TextureDialog::~TextureDialog()
{
}

void TextureDialog::texture_unit_change(const int value)
{
}

void TextureDialog::projection_change(const bool value)
{
}

void TextureDialog::texture_change(const int value)
{
}
