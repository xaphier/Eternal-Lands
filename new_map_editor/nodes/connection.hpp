#ifndef	UUID_f264fbaf_f48d_4b92_b3ba_a6b3b7f55758
#define	UUID_f264fbaf_f48d_4b92_b3ba_a6b3b7f55758

#include <QGraphicsLineItem>
#include <QGraphicsPolygonItem>
#include <QGraphicsLineItem>
#include <QGraphicsScene>
#include <QRectF>
#include <QGraphicsSceneMouseEvent>
#include <QPainterPath>

class QNEPort;

class Connection: public QGraphicsPathItem//QGraphicsPathItem
{
	private:
		QColor m_color;
		QPolygonF m_arrow_head;
		QPointF m_pos1;
		QPointF m_pos2;
		QNEPort* m_port1;
		QNEPort* m_port2;

	public:
		enum { Type = QGraphicsItem::UserType + 2 };
		Connection(QGraphicsItem* parent = 0,
			QGraphicsScene* scene = 0);
		virtual ~Connection();
		int type() const { return Type; }

		inline void set_color(const QColor &color)
		{
			m_color = color;
		}

		inline const QColor &get_color() const
		{
			return m_color;
		}

		inline QNEPort* get_port1() const
		{
			return m_port1;
		}

		inline QNEPort* get_port2() const
		{
			return m_port2;
		}

		inline const QPointF &get_pos1() const
		{
			return m_pos1;
		}

		inline const QPointF &get_pos2() const
		{
			return m_pos2;
		}

		inline void set_pos1(const QPointF &p)
		{
			m_pos1 = p;
		}

		inline void set_pos2(const QPointF &p)
		{
			m_pos2 = p;
		}

		void set_port1(QNEPort *p);
		void set_port2(QNEPort *p);
		void update_pos_from_ports();
		void update_path();

};

#endif	/* UUID_f264fbaf_f48d_4b92_b3ba_a6b3b7f55758 */
