#ifndef	UUID_035b45d2_7617_4950_a78d_378ddd76a8a4
#define	UUID_035b45d2_7617_4950_a78d_378ddd76a8a4

#include "qneblock.hpp"
#include "../engine/prerequisites.hpp"

namespace el = eternal_lands;

class BasicNode: public QNEBlock
{
	private:
		el::EffectNodesSharedPtr m_effect_nodes;
		el::EffectNodePtr m_effect_node;

	protected:
		void init_ports();
		virtual QVariant itemChange(GraphicsItemChange change,
			const QVariant &value);

	public:
		BasicNode(const el::EffectNodesSharedPtr &effect_nodes,
			el::EffectNodePtr effect_node, QString name,
			QGraphicsItem *parent = 0,
			QGraphicsScene *scene = 0);
		virtual ~BasicNode();
		void rebuild_connections(
			std::map<el::EffectNodePortPtr, QNEPort*> &ports);

};

#endif	/* UUID_035b45d2_7617_4950_a78d_378ddd76a8a4 */
