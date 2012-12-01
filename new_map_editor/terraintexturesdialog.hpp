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
		QColor m_color;

	private slots:
		void texture_clicked(QListWidgetItem* item,
			QListWidgetItem* previous);
		void change_default_specular();

	public:
		TerrainTexturesDialog(const QStringTerrainTextureDataQMap
			&terrain_texture_datas, QWidget *parent = 0);
		virtual ~TerrainTexturesDialog();
		void update_terrain_texture_datas();
		void set_icon_size(const QSize &size);
		void set_texture(const QString &texture);
		void set_default_specular(const QColor &value);
		void set_default_gloss(const float value);
		void set_default_height(const float value);
		void set_blend_size(const float value);
		void set_label(const QString &label);	
		QString get_texture() const;
		float get_default_gloss() const;
		float get_default_height() const;
		float get_blend_size() const;

		inline const QColor &get_default_specular() const
		{
			return m_color;
		}

};

#endif	/* UUID_38a27e65_2b08_45e8_9300_5149f5ecbfbf */
