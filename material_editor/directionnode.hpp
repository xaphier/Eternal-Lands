#ifndef	UUID_f1f5aa38_071f_409f_bc35_a0e623634240
#define	UUID_f1f5aa38_071f_409f_bc35_a0e623634240

#include "basicnode.hpp"
#include "../engine/effect/effectconstant.hpp"
#include <QDial>

namespace el = eternal_lands;

class DirectionNode: public QObject, public BasicNode
{
	Q_OBJECT

	private:
		el::EffectConstant* m_effect_constant;
		QDial* m_direction;
		QGraphicsProxyWidget* m_proxi;

	private slots:
		void value_change(const int value);

	public:
		DirectionNode(const el::EffectNodesSharedPtr &effect_nodes,
			el::EffectConstant* effect_constant, QString name,
			QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);
		virtual ~DirectionNode();

};

#endif	/* UUID_f1f5aa38_071f_409f_bc35_a0e623634240 */
