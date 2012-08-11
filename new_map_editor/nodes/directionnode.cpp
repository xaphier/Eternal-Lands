#include "directionnode.hpp"
#include "qneport.hpp"
#include <QGraphicsProxyWidget>

DirectionNode::DirectionNode(const el::EffectNodesSharedPtr &effect_nodes,
	el::EffectConstant* effect_constant, QString name,
	QGraphicsItem* parent, QGraphicsScene* scene):
	BasicNode(effect_nodes, effect_constant, name, parent, scene),
	m_effect_constant(effect_constant)
{
	float tmp;

	m_direction = new QDial();
	m_direction->setMaximum(179);
	m_direction->setMinimum(-180);
	m_proxi = new QGraphicsProxyWidget(this);
	m_proxi->setWidget(m_direction);

	init_ports();

	tmp = std::asin(m_effect_constant->get_value().x);

	m_direction->setValue((tmp * 360.0) / (2.0 * M_PI));

	QObject::connect(m_direction, SIGNAL(valueChanged(int)), this,
		SLOT(value_change(int)));
}

DirectionNode::~DirectionNode()
{
}

void DirectionNode::value_change(const int value)
{
	float x, y;

	x = std::sin((value / 360.0) * 2.0 * M_PI);
	y = std::cos((value / 360.0) * 2.0 * M_PI);

	m_effect_constant->set_value(glm::vec4(x, y, 0.0f, 0.0f));

	setToolTip(QString::fromUtf8(m_effect_constant->get_description().get(
		).c_str()));
}
