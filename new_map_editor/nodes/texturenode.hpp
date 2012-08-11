/****************************************************************************
 *            texturenode.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_daf6d58f_09fc_4c58_b3f6_e8d38bd5820c
#define	UUID_daf6d58f_09fc_4c58_b3f6_e8d38bd5820c

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "basicnode.hpp"
#include "../engine/node/effecttexture.hpp"

namespace el = eternal_lands;

class TextureNode: public QObject, public BasicNode
{
	Q_OBJECT

	private:
		el::EffectTexture* m_effect_texture;

	protected:
		virtual void mouseDoubleClickEvent(
			QGraphicsSceneMouseEvent * event);

	public:
		TextureNode(const el::EffectNodesSharedPtr &effect_nodes,
			el::EffectTexture* effect_texture, QString name,
			QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);
		virtual ~TextureNode();

};

#endif	/* UUID_daf6d58f_09fc_4c58_b3f6_e8d38bd5820c */
