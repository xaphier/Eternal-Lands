#ifndef	UUID_f1f5aa38_071f_409f_bc35_a0e623634240
#define	UUID_f1f5aa38_071f_409f_bc35_a0e623634240

#include "basicnode.hpp"
#include "../engine/node/effectconstant.hpp"
#include "directiondialog.hpp"

namespace el = eternal_lands;

class DirectionNode: public BasicNode
{
	private:
		el::EffectConstant* m_effect_constant;
		QNEPort* m_direction;
		DirectionDialog* m_dialog;

	protected:
		virtual void mouseDoubleClickEvent(
			QGraphicsSceneMouseEvent * event);

	public:
		DirectionNode(const el::EffectNodesSharedPtr &effect_nodes,
			el::EffectConstant* effect_constant, QString name,
			QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);
		virtual ~DirectionNode();
		void select_direction();

};

#endif	/* UUID_f1f5aa38_071f_409f_bc35_a0e623634240 */
