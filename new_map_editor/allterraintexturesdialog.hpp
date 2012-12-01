#ifndef	UUID_808958f5_5e06_4853_ba12_8ea10e8fcf4c
#define UUID_808958f5_5e06_4853_ba12_8ea10e8fcf4c

#include "ui_allterraintextures.h"
#include <QDialog>
#include "terraintexturedata.hpp"

class AllTerrainTexturesDialog: public QDialog, public Ui::AllTerrainTextures
{
	Q_OBJECT

	private:
		QStringTerrainTextureDataQMap m_terrain_texture_datas;
		const QString &m_el_data_dir;
		const QString &m_el_extra_data_dir;
		QSize m_icon_size;
		QSize m_image_size;

		void update_terrain_texture_datas();

	private slots:
		void clear_textures();
		void add_texture();
		void remove_texture();
		void remove_height_texture();
		void change_height_texture();
		void change_height_value(const double value);
		void remove_specular_texture();
		void change_specular_texture();
		void change_specular_value();
		void remove_gloss_texture();
		void change_gloss_texture();
		void change_gloss_value(const double value);
		void change_blend_size_value(const double value);
		void update_buttons_enabled();

	public:
		AllTerrainTexturesDialog(const QString &el_data_dir,
			const QString &el_extra_data_dir,
			QWidget *parent = 0);
		virtual ~AllTerrainTexturesDialog();
		void set_icon_size(const QSize &size);
		void set_image_size(const QSize &size);
		void set_terrain_texture_datas(
			const QStringTerrainTextureDataQMap
				&terrain_texture_datas);

		inline const QStringTerrainTextureDataQMap
			&get_terrain_texture_datas() const
		{
			return m_terrain_texture_datas;
		}

		inline const QSize &get_image_size() const
		{
			return m_image_size;
		}

	signals:
		void get_albedo_map_data(const QString &name,
			const QSize &icon_size, const QSize &image_size,
			QIcon &icon, bool &use_blend_size_texture, bool &ok);
		void get_specular_map_data(const QString &name,
			const QSize &image_size, bool &ok);
		void get_gloss_map_data(const QString &name,
			const QSize &image_size, bool &ok);
		void get_height_map_data(const QString &name,
			const QSize &image_size, bool &ok);

};

#endif	/* UUID_808958f5_5e06_4853_ba12_8ea10e8fcf4c */
