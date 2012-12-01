#include "terraintranslationdialog.hpp"

TerrainTranslationDialog::TerrainTranslationDialog(QWidget *parent):
	QDialog(parent)
{
	setupUi(this);

	QObject::connect(translation_x, SIGNAL(valueChanged(const double)),
		this, SLOT(change_translation()));
	QObject::connect(translation_y, SIGNAL(valueChanged(const double)),
		this, SLOT(change_translation()));
	QObject::connect(translation_z, SIGNAL(valueChanged(const double)),
		this, SLOT(change_translation()));
}

TerrainTranslationDialog::~TerrainTranslationDialog()
{
}

void TerrainTranslationDialog::change_translation()
{
	m_translation = QVector3D(translation_x->value(),
		translation_y->value(), translation_z->value());
}

void TerrainTranslationDialog::set_translation(const QVector3D &translation)
{
	translation_x->setValue(translation.x());
	translation_y->setValue(translation.y());
	translation_z->setValue(translation.z());

	m_translation = translation;
}
