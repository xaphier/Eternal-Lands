#ifndef	UUID_38a27e65_2b08_45e8_9300_5149f5ecbfbf
#define UUID_38a27e65_2b08_45e8_9300_5149f5ecbfbf

#include "ui_terraintextures.h"
#include <QDialog>
#include "terraintexturedata.hpp"

class TerrainTexturesDialog: public QDialog, public Ui::TerrainTextures
{
	Q_OBJECT

	private:
		const QStringTerrainTextureDataQMap &m_terrain_texture_datas;

	private slots:
		void texture_clicked(QListWidgetItem* item,
			QListWidgetItem* previous);

	public:
		TerrainTexturesDialog(const QStringTerrainTextureDataQMap
			&terrain_texture_datas, QWidget *parent = 0);
		virtual ~TerrainTexturesDialog();
		void update_terrain_texture_datas();
		void set_icon_size(const QSize &size);
		void set_texture(const QString &texture);
		void set_use_blend_size(const bool enabled);
		void set_blend_size(const float value);
		void set_label(const QString &label);	
		QString get_texture() const;
		bool get_use_blend_size() const;
		float get_blend_size() const;

};

#endif	/* UUID_38a27e65_2b08_45e8_9300_5149f5ecbfbf */
