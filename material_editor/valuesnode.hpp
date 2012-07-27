#ifndef	UUID_01513aaa_47e5_4448_80aa_cec72226f173
#define	UUID_01513aaa_47e5_4448_80aa_cec72226f173

#include "basicnode.hpp"
#include "../engine/node/effectconstant.hpp"
#include "valuesdialog.hpp"

namespace el = eternal_lands;

class ValuesNode: public BasicNode
{
	private:
		el::EffectConstant* m_effect_constant;
		ValuesDialog* m_dialog;
		QNEPort* m_value_1;
		QNEPort* m_value_2;
		QNEPort* m_value_3;
		QNEPort* m_value_4;
		const int m_count;

	protected:
		virtual void mouseDoubleClickEvent(
			QGraphicsSceneMouseEvent * event);

	public:
		ValuesNode(const el::EffectNodesSharedPtr &effect_nodes,
			el::EffectConstant* effect_constant, QString name,
			const int count, QGraphicsItem *parent = 0,
			QGraphicsScene *scene = 0);
		virtual ~ValuesNode();
		void select_values();

};

#endif	/* UUID_01513aaa_47e5_4448_80aa_cec72226f173 */
