#include "node.hpp"
#include "qneport.hpp"

Node::Node(el::EffectNode* effect_node, QString name, QGraphicsItem* parent,
	QGraphicsScene* scene): QNEBlock(parent, scene)
{
	addPort(name, 0, QNEPort::NamePort);

	if (!effect_node->get_name().get().empty())
	{
		addPort(QString::fromUtf8(effect_node->get_name().get(
			).c_str()), 0, QNEPort::TypePort);
	}

	BOOST_FOREACH(el::EffectNodePort &port, effect_node->get_ports())
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
}

Node::~Node()
{
}
