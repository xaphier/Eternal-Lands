#include "node.hpp"
#include "qneport.hpp"
#include "../engine/shader/shadersourceparameter.hpp"

Node::Node(el::EffectNode* effect_node, QString name, QGraphicsItem* parent,
	QGraphicsScene* scene): QNEBlock(parent, scene),
	m_effect_node(effect_node)
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

typedef std::pair<el::String, el::BitSet16> StringBitSet16Pair;

void Node::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
	el::ShaderSourceParameterVector vertex_parameters, fragment_parameters;
	el::StringStream vertex_str, fragment_str;
	el::StringUint16Map parameters;
	el::EffectNodePtrSet vertex_written, fragment_written;
	el::EffectChangeType change;
	el::StringUint16Map::const_iterator it, end;
	bool glsl_120;

	change = el::ect_fragment;
	glsl_120 = false;

	m_effect_node->write(glsl_120, change, parameters,
		vertex_parameters, fragment_parameters, vertex_str,
		fragment_str, vertex_written, fragment_written);

	std::cout << "vertex_str: " << vertex_str.str() << std::endl;
	std::cout << "fragment_str: " << fragment_str.str() << std::endl;

	end = parameters.end();

	for (it = parameters.begin(); it != end; ++it)
	{
		std::cout << it->first << ": " << it->second << std::endl;
	}
}
