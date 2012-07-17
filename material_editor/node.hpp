#ifndef	UUID_c25614a9_0503_40f4_affe_f73519d6edd9
#define	UUID_c25614a9_0503_40f4_affe_f73519d6edd9

#include "qneblock.hpp"
#include "../engine/node/effectnode.hpp"
#include "../engine/node/effectnodeport.hpp"

namespace el = eternal_lands;

class Node: public QNEBlock
{
	private:
		el::EffectNodePtr m_effect_node;

	protected:
		virtual void mouseDoubleClickEvent(
			QGraphicsSceneMouseEvent * event);

	public:
		Node(const el::EffectNodePtr effect_node, QString name,
			QGraphicsItem *parent = 0,
			QGraphicsScene *scene = 0);
		virtual ~Node();

};

#endif	/* UUID_c25614a9_0503_40f4_affe_f73519d6edd9 */
