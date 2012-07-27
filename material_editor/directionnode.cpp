#include "directionnode.hpp"
#include "qneport.hpp"

DirectionNode::DirectionNode(const el::EffectNodesSharedPtr &effect_nodes,
	el::EffectConstant* effect_constant, QString name,
	QGraphicsItem* parent, QGraphicsScene* scene):
	BasicNode(effect_nodes, effect_constant, name, parent, scene),
	m_effect_constant(effect_constant)
{
	m_direction = addPort("", 0, QNEPort::ImagePort);

	init_ports();

	m_dialog = new DirectionDialog(0);
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
