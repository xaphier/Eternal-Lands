#include "texturedialog.hpp"
#include "../engine/texturetargetutil.hpp"
#include "../engine/effect/effecttextureutil.hpp"

namespace el = eternal_lands;

TextureDialog::TextureDialog(QWidget *parent): QDialog(parent)
{
	setupUi(this);

	connect(projection, SIGNAL(toggled(bool)), this,
		SLOT(projection_change(bool)));
}

TextureDialog::~TextureDialog()
{
}

bool TextureDialog::get_project() const
{
	return projection->isChecked();
}

int TextureDialog::get_texture_units() const
{
	return texture_unit->currentIndex();
}

void TextureDialog::projection_change(const bool value)
{
}

void TextureDialog::set_texture_units(const QStringList &names)
{
	texture_unit->clear();
	texture_unit->addItems(names);
}

void TextureDialog::set_target(const int target)
{
	switch (target)
	{
		case el::ttt_texture_2d:
			projection->setEnabled(true);
			parallax_texture->setEnabled(true);
			break;
		case el::ttt_texture_2d_array:
			projection->setEnabled(false);
			parallax_texture->setEnabled(true);
			break;
		case el::ttt_texture_1d:
		case el::ttt_texture_3d:
		case el::ttt_texture_rectangle:
			projection->setEnabled(true);
			parallax_texture->setEnabled(false);
			break;
		case el::ttt_texture_cube_map:
		case el::ttt_texture_1d_array:
		case el::ttt_texture_cube_map_array:
		case el::ttt_texture_2d_multisample:
		case el::ttt_texture_2d_multisample_array:
			projection->setEnabled(false);
			parallax_texture->setEnabled(false);
			break;
	}
}

int TextureDialog::get_texture() const
{
	if (albedo_texture->isChecked())
	{
		return el::ett_albedo;
	}

	if (uv_offset_texture->isChecked())
	{
		return el::ett_uv_offset;
	}

	if (normal_texture->isChecked())
	{
		return el::ett_normal;
	}

	if (parallax_texture->isChecked())
	{
		return el::ett_parallax;
	}

	return el::ett_default;
}
