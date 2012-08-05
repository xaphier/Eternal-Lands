/****************************************************************************
 *            texturenode.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "texturenode.hpp"
#include "qneport.hpp"
#include <QGraphicsProxyWidget>

TextureNode::TextureNode(const el::EffectNodesSharedPtr &effect_nodes,
	el::EffectTexture* effect_texture, QString name, QGraphicsItem* parent,
	QGraphicsScene* scene):
	BasicNode(effect_nodes, effect_texture, name, parent, scene),
	m_effect_texture(effect_texture)
{
	if (!effect_texture->get_name().get().empty())
	{
		addPort(QString::fromUtf8(effect_texture->get_name().get(
			).c_str()), 0, QNEPort::TypePort);
	}

	init_ports();
}

TextureNode::~TextureNode()
{
}

void TextureNode::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
	std::cout << "layer: " << m_effect_texture->get_layer() << std::endl;
}
