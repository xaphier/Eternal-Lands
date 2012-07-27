#include <QWidget>
#include <QBrush>
#include <QPen>
#include <math.h>
#include "connection.hpp"
#include "qneport.hpp"

Connection::Connection(QGraphicsItem *parent, QGraphicsScene *scene):
	QGraphicsPathItem(parent, scene), m_port1(0), m_port2(0)
{
	setFlag(QGraphicsItem::ItemIsSelectable, true);

	set_color(Qt::black);

	setPen(QPen(get_color(), 2, Qt::SolidLine, Qt::RoundCap,
		Qt::RoundJoin));
}

Connection::~Connection()
{
	if ((m_port1 != 0) && (m_port2 != 0))
	{
		m_port1->disconnect(m_port2);
		m_port2->disconnect(m_port1);
	}

	if (m_port1 != 0)
	{
		m_port1->connections().remove(m_port1->connections().indexOf(
			this));
	}

	if (m_port2 != 0)
	{
		m_port2->connections().remove(m_port2->connections().indexOf(
			this));
	}
}

void Connection::update_path()
{
	QLineF line;
	QPainterPath p;
	QPen current_pen;
	QPointF arrow_p1, arrow_p2, p1, p2, ctr1, ctr2;
	float arrow_size, angle, dx, dy, tmp;

	p1 = get_pos1();
	p2 = get_pos2();

	if (m_port1 != 0)
	{
		if (m_port1->isOutput())
		{
			p1 = get_pos2();
			p2 = get_pos1();
		}
	}

	line = QLineF(p1, p2);

	arrow_size = 15.0f;

	p.moveTo(p1);

	dx = p2.x() - p1.x();
	dy = p2.y() - p1.y();

	ctr1 = QPointF(p1.x() + dx * 0.25f, p1.y() + dy * 0.1f);
	ctr2 = QPointF(p1.x() + dx * 0.75f, p1.y() + dy * 0.9f);

	p.cubicTo(ctr1, ctr2, p2);

	tmp = 0.0f;

	if (p.length() > 0.0001f)
	{
		tmp = arrow_size / p.length();
		tmp = std::max(0.0f, std::min(1.0f, tmp));
	}

	angle = p.angleAtPercent(tmp);
	angle = angle / 180.0f * M_PI;

	arrow_p1 = line.p1() + QPointF(std::sin(angle + M_PI / 3.0f) *
		arrow_size, std::cos(angle + M_PI / 3.0f) * arrow_size);
	arrow_p2 = line.p1() + QPointF(std::sin(angle + M_PI - M_PI / 3.0f) *
		arrow_size, std::cos(angle + M_PI - M_PI / 3.0f) * arrow_size);

	p.moveTo(line.p1());
	p.lineTo(arrow_p1);
	p.moveTo(line.p1());
	p.lineTo(arrow_p2);

	setPath(p);
}

void Connection::set_port1(QNEPort *p)
{
	m_port1 = p;

	if ((m_port1 != 0) && (m_port2 != 0))
	{
		m_port2->connect(m_port1);
	}

	m_port1->connections().append(this);
}

void Connection::set_port2(QNEPort *p)
{
	m_port2 = p;

	if ((m_port1 != 0) && (m_port2 != 0))
	{
		m_port1->connect(m_port2);
	}

	m_port2->connections().append(this);
}

void Connection::update_pos_from_ports()
{
	set_pos1(m_port1->connection_pos());
	set_pos2(m_port2->connection_pos());
}
