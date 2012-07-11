#include "valuesnode.hpp"
#include "qneport.hpp"

ValuesNode::ValuesNode(el::EffectConstant* effect_constant, QString name,
	const int count, QGraphicsItem* parent, QGraphicsScene* scene):
	QNEBlock(parent, scene), m_effect_constant(effect_constant),
	m_count(count)
{
	addPort(name, 0, QNEPort::NamePort);

	m_value_1 = addPort("0", 0, QNEPort::TypePort);

	if (m_count > 1)
	{
		m_value_2 = addPort("0", 0, QNEPort::TypePort);
	}

	if (m_count > 2)
	{
		m_value_3 = addPort("0", 0, QNEPort::TypePort);
	}

	if (m_count > 3)
	{
		m_value_4 = addPort("0", 0, QNEPort::TypePort);
	}

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

	m_dialog = new ValuesDialog(0);
	m_dialog->set_value_count(count);

	select_values();
}

ValuesNode::~ValuesNode()
{
}

void ValuesNode::mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event)
{
	select_values();
}

void ValuesNode::select_values()
{
	glm::vec4 value;
	QVector4D result;
	QString name;

	value = m_effect_constant->get_value();

	m_dialog->set_values(QVector4D(value.x, value.y, value.z, value.w));

	if (m_dialog->exec() == QDialog::Accepted)
	{
		result = m_dialog->get_values();

		m_effect_constant->set_value(glm::vec4(result.x(), result.y(),
			result.z(), result.w()));

		m_value_1->setName(QString::number(result.x()));

		if (m_count > 1)
		{
			m_value_2->setName(QString::number(result.y()));
		}

		if (m_count > 2)
		{
			m_value_3->setName(QString::number(result.z()));
		}

		if (m_count > 3)
		{
			m_value_4->setName(QString::number(result.z()));
		}
	}
}
