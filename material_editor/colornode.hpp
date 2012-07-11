#ifndef	UUID_51f6e890_c51d_4062_98d2_bf875138e572
#define	UUID_51f6e890_c51d_4062_98d2_bf875138e572

#include "node.hpp"
#include "../engine/node/effectconstant.hpp"

namespace el = eternal_lands;

class ColorNode: public QNEBlock
{
	private:
		el::EffectConstant* m_effect_constant;
		QNEPort* m_pixmap;

	protected:
		virtual void mouseDoubleClickEvent(
			QGraphicsSceneMouseEvent * event);

	public:
		ColorNode(el::EffectConstant* effect_constant, QString name,
			QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);
		virtual ~ColorNode();
		void select_color();

};

#endif	/* UUID_51f6e890_c51d_4062_98d2_bf875138e572 */
