#ifndef	UUID_3e9ba09a_f062_4917_a4aa_e140a0683822
#define UUID_3e9ba09a_f062_4917_a4aa_e140a0683822

#include "ui_loadmap.h"
#include <QDialog>

class LoadMapDialog: public QDialog, public Ui::LoadMap
{
	Q_OBJECT

	public:
		LoadMapDialog(QWidget *parent = 0);
		virtual ~LoadMapDialog();
		bool get_load_2d_objects() const;
		bool get_load_3d_objects() const;
		bool get_load_lights() const;
		bool get_load_particles() const;
		bool get_load_materials() const;
		bool get_load_height_map() const;
		bool get_load_tile_map() const;
		bool get_load_walk_map() const;
		bool get_load_terrain() const;
		bool get_load_water() const;

};

#endif	/* UUID_3e9ba09a_f062_4917_a4aa_e140a0683822 */
