#include "directionnode.hpp"
#include "qneport.hpp"

DirectionNode::DirectionNode(el::EffectConstant* effect_constant, QString name,
	QGraphicsItem* parent, QGraphicsScene* scene): QNEBlock(parent, scene),
	m_effect_constant(effect_constant)
{
	addPort(name, 0, QNEPort::NamePort);

	m_direction = addPort("", 0, QNEPort::ImagePort);

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

	m_dialog = new DirectionDialog(0);

	select_direction();
}

DirectionNode::~DirectionNode()
{
}

void DirectionNode::mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event)
{
	select_direction();
}

void DirectionNode::select_direction()
{
	glm::vec4 value;
	QVector2D result;

	value = m_effect_constant->get_value();

	m_dialog->set_direction(QVector2D(value.x, value.y));

	if (m_dialog->exec() == QDialog::Accepted)
	{
		result = m_dialog->get_direction();

		m_effect_constant->set_value(glm::vec4(result.x(), result.y(),
			0.0f, 0.0f));

//		m_direction->set_color(color);
	}
}
