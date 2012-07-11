#include "colornode.hpp"
#include <QColorDialog>
#include "qneport.hpp"

ColorNode::ColorNode(el::EffectConstant* effect_constant, QString name,
	QGraphicsItem* parent, QGraphicsScene* scene): QNEBlock(parent, scene),
	m_effect_constant(effect_constant)
{
	addPort(name, 0, QNEPort::NamePort);

	m_pixmap = addPort("", 0, QNEPort::ImagePort);

	BOOST_FOREACH(el::EffectNodePort &port, effect_constant->get_ports())
	{
		if (port.get_input())
		{
			addInputPort(&port, QString::fromUtf8(
				port.get_description().get().c_str()));
		}
		else
		{
			addOutputPort(&port, QString::fromUtf8(
				port.get_description().get().c_str()));
		}
	}

	select_color();
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
