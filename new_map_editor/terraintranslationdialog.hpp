#ifndef	UUID_e6084fc4_0827_436e_90ea_fa2926fdd2cb
#define UUID_e6084fc4_0827_436e_90ea_fa2926fdd2cb

#include "ui_terraintranslation.h"
#include <QDialog>
#include <QVector3D>

class TerrainTranslationDialog: public QDialog, public Ui::TerrainTranslation
{
	Q_OBJECT

	private:
		QVector3D m_translation;

	private slots:
		void change_translation();

	public:
		TerrainTranslationDialog(QWidget *parent = 0);
		virtual ~TerrainTranslationDialog();
		void set_translation(const QVector3D &translation);

		inline const QVector3D &get_translation() const
		{
			return m_translation;
		}

};

#endif	/* UUID_e6084fc4_0827_436e_90ea_fa2926fdd2cb */
