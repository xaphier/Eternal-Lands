#ifndef	UUID_01513aaa_47e5_4448_80aa_cec72226f173
#define	UUID_01513aaa_47e5_4448_80aa_cec72226f173

#include "basicnode.hpp"
#include "../engine/node/effectconstant.hpp"
#include <QDoubleSpinBox>
#include <QSignalMapper>

namespace el = eternal_lands;

class ValuesNode: public QObject, public BasicNode
{
	Q_OBJECT

	private:
		el::EffectConstant* m_effect_constant;
		QVector<QDoubleSpinBox*> m_edits;
		QVector<QGraphicsProxyWidget*> m_proxis;
		QSignalMapper* m_edit_mapper;
		const int m_count;

	private slots:
		void value_change(const int index);

	public:
		ValuesNode(const el::EffectNodesSharedPtr &effect_nodes,
			el::EffectConstant* effect_constant, QString name,
			const int count, QGraphicsItem *parent = 0,
			QGraphicsScene *scene = 0);
		virtual ~ValuesNode();

};

#endif	/* UUID_01513aaa_47e5_4448_80aa_cec72226f173 */
