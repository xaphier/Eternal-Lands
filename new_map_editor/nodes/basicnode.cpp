#include "basicnode.hpp"
#include "qneport.hpp"
#include "connection.hpp"
#include "../engine/node/effectnode.hpp"
#include "../engine/node/effectnodes.hpp"
#include "../engine/node/effectnodeport.hpp"
#include <QTextDocument>

BasicNode::BasicNode(const el::EffectNodesSharedPtr &effect_nodes,
	el::EffectNodePtr effect_node, QString name, QGraphicsItem* parent,
	QGraphicsScene* scene): QNEBlock(parent, scene),
	m_effect_nodes(effect_nodes), m_effect_node(effect_node)
{
	QString tool_tip;
	QTextDocument document;

	addPort(name, 0, QNEPort::NamePort);

	tool_tip = QString::fromUtf8(effect_node->get_description().get(
		).c_str());

	document.setPlainText(tool_tip);

	tool_tip = QString("<FONT COLOR=black>");
	tool_tip += document.toHtml();
	tool_tip += QString("</FONT>");

	setToolTip(tool_tip);

	setPos(QPointF(effect_node->get_position().x,
		effect_node->get_position().y));

	setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}

BasicNode::~BasicNode()
{
	foreach(QGraphicsItem *child, childItems())
	{
		if (child->type() == QNEPort::Type)
		{
			delete child;
		}
	}

	foreach(QGraphicsItem *child, childItems())
	{
		delete child;
	}

	m_effect_nodes->remove(m_effect_node);
}

void BasicNode::init_ports()
{
	BOOST_FOREACH(el::EffectNodePort &port, m_effect_node->get_ports())
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

QVariant BasicNode::itemChange(GraphicsItemChange change, const QVariant &value)
{
	if (change == ItemPositionChange)
	{
		m_effect_node->set_position(glm::ivec2(value.toPointF().x(),
			value.toPointF().y()));
	}

	return QGraphicsPathItem::itemChange(change, value);
}

void BasicNode::rebuild_connections(
	std::map<el::EffectNodePortPtr, QNEPort*> &ports)
{
	std::map<el::EffectNodePortPtr, QNEPort*>::const_iterator found, end;
	QNEPort* port1;
	QNEPort* port2;

	end = ports.end();

	BOOST_FOREACH(el::EffectNodePort &effect_node_port,
		m_effect_node->get_ports())
	{
		if (effect_node_port.get_output())
		{
			continue;
		}

		if (!effect_node_port.get_connected())
		{
			continue;
		}

		found = ports.find(&effect_node_port);

		if (found == end)
		{
			continue;
		}

		port1 = found->second;

		found = ports.find((*effect_node_port.get_connections(
			).begin()));

		if (found == end)
		{
			continue;
		}

		port2 = found->second;

		Connection* conn = new Connection(0, scene());
		conn->set_port1(port1);
		conn->set_port2(port2);
		conn->update_pos_from_ports();
		conn->update_path();
	}
}
