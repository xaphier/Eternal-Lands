#include "loadmapdialog.hpp"

LoadMapDialog::LoadMapDialog(QWidget *parent): QDialog(parent)
{
	setupUi(this);
}

LoadMapDialog::~LoadMapDialog()
{
}

bool LoadMapDialog::get_load_2d_objects() const
{
	return load_2d_objects->isChecked();
}

bool LoadMapDialog::get_load_3d_objects() const
{
	return load_3d_objects->isChecked();
}

bool LoadMapDialog::get_load_lights() const
{
	return load_lights->isChecked();
}

bool LoadMapDialog::get_load_particles() const
{
	return load_particles->isChecked();
}

bool LoadMapDialog::get_load_materials() const
{
	return load_materials->isChecked();
}

bool LoadMapDialog::get_load_height_map() const
{
	return load_height_map->isChecked();
}

bool LoadMapDialog::get_load_tile_map() const
{
	return load_tile_map->isChecked();
}

bool LoadMapDialog::get_load_walk_map() const
{
	return load_walk_map->isChecked();
}

bool LoadMapDialog::get_load_terrain() const
{
	return load_terrain->isChecked();
}
