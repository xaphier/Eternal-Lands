#include "colornode.hpp"
#include <QColorDialog>
#include "qneport.hpp"

ColorNode::ColorNode(const el::EffectNodesSharedPtr &effect_nodes,
	el::EffectConstant* effect_constant, QString name,
	QGraphicsItem* parent, QGraphicsScene* scene):
	BasicNode(effect_nodes, effect_constant, name, parent, scene),
	m_effect_constant(effect_constant)
{
	m_pixmap = addPort("", 0, QNEPort::ImagePort);

	init_ports();

	m_pixmap->set_color(QColor::fromRgbF(m_effect_constant->get_value().r,
		m_effect_constant->get_value().g,
		m_effect_constant->get_value().b,
		m_effect_constant->get_value().a));
}

ColorNode::~ColorNode()
{
}

void ColorNode::mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event)
{
	select_color();
}

void ColorNode::select_color()
{
	QColor color;
	glm::vec4 value;

	value = m_effect_constant->get_value();

	color = QColor::fromRgbF(value.r, value.g, value.b, value.a);

	color = QColorDialog::getColor(color, 0, "Select color");

	m_effect_constant->set_value(glm::vec4(color.redF(), color.greenF(),
		color.blueF(), color.alphaF()));

	m_pixmap->set_color(color);
}
