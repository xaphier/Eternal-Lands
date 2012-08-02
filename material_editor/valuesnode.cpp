#include "valuesnode.hpp"
#include "qneport.hpp"
#include <QGraphicsProxyWidget>

ValuesNode::ValuesNode(const el::EffectNodesSharedPtr &effect_nodes,
	el::EffectConstant* effect_constant, QString name, const int count,
	QGraphicsItem* parent, QGraphicsScene* scene):
	BasicNode(effect_nodes, effect_constant, name, parent, scene),
	m_effect_constant(effect_constant), m_count(count)
{
	QDoubleSpinBox* edit;
	QGraphicsProxyWidget* proxi;
	int i;

	for (i = 0; i < m_count; ++i)
	{
		edit = new QDoubleSpinBox();
		edit->setMaximum(1000.0);
		edit->setMinimum(-1000.0);
		edit->setSingleStep(0.05);
		edit->setAccelerated(true);
		edit->setValue(m_effect_constant->get_value()[i]);

		proxi = new QGraphicsProxyWidget(this);
		proxi->setWidget(edit);

		m_edits.push_back(edit);
		m_proxis.push_back(proxi);
	}

	init_ports();

	m_edit_mapper = new QSignalMapper(this);

	for (i = 0; i < m_count; ++i)
	{
		m_edit_mapper->setMapping(m_edits[i], i);

		QObject::connect(m_edits[i], SIGNAL(valueChanged(double)),
			m_edit_mapper, SLOT(map()));
	}

	QObject::connect(m_edit_mapper, SIGNAL(mapped(const int)),
		this, SLOT(value_change(const int)));
}

ValuesNode::~ValuesNode()
{
}

void ValuesNode::value_change(const int index)
{
	glm::vec4 tmp;

	tmp = m_effect_constant->get_value();
	tmp[index] = m_edits[index]->value();

	m_effect_constant->set_value(tmp);

}
