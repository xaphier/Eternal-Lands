/* Copyright (c) 2012, STANISLAW ADASZEWSKI
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the m_name of STANISLAW ADASZEWSKI nor the
      m_names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL STANISLAW ADASZEWSKI BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. */

#include "qneport.hpp"

#include <QGraphicsScene>
#include <QFontMetrics>

#include <QPen>
#include <QInputDialog>
#include "connection.hpp"

#include "../engine/effect/effectnode.hpp"

QNEPort::QNEPort(el::EffectNodePortPtr effect_port, QGraphicsItem *parent,
	QGraphicsScene *scene): QGraphicsPathItem(parent, scene),
	m_isOutput(true), m_effect_port(effect_port)
{
	m_label = 0;
	m_pixmap = 0;

	setFlags(0);

	m_radius = 5;
	m_margin = 2;

	QPainterPath p;
	p.addEllipse(-m_radius, -m_radius, 2*m_radius, 2*m_radius);
	setPath(p);

	setPen(QPen(Qt::darkGray));
	setBrush(Qt::black);

	setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
}

QNEPort::~QNEPort()
{
	foreach(Connection *conn, m_connections)
	{
		delete conn;
	}
}

void QNEPort::setNEBlock(QNEBlock *b)
{
	m_block = b;
}

void QNEPort::setName(const QString &n)
{
	if (m_effect_port != 0)
	{
		m_effect_port->set_description(el::String(n.toUtf8()));
	}

	m_name = n;

	update();
}

void QNEPort::setIsOutput(bool o)
{
	m_isOutput = o;
}

int QNEPort::radius() const
{
	return m_radius;
}

int QNEPort::height() const
{
	return m_height;
}

int QNEPort::width() const
{
	return m_width;
}

int QNEPort::margin() const
{
	return m_margin;
}

const QString &QNEPort::name() const
{
	return m_name;
}

int QNEPort::flags() const
{
	return m_flags;
}

int QNEPort::type() const
{
	return Type;
}

void QNEPort::update()
{
	QPainterPath p;
	QBrush brush;

	m_margin = 2;

	if (m_label != 0)
	{
		m_label->setPlainText(name());

		m_height = ((m_label->boundingRect().height() + 1) / 2) * 2;
		m_width = m_label->boundingRect().width();
	}
	else
	{
		if (name().isEmpty())
		{
			QPixmap pixmap(QSize(32, 32));

			pixmap.fill(color());

			m_pixmap->setPixmap(pixmap);
		}
		else
		{
			m_pixmap->setPixmap(QPixmap(name()));
		}

		m_height = ((m_pixmap->boundingRect().height() + 1) / 2) * 2;
		m_width = m_pixmap->boundingRect().width();
	}

	m_radius = 0;

	if (m_flags == 0)
	{
		m_radius = m_height / 2 - margin();

		p.addEllipse(0, 0, 2 * radius(), 2 * radius());
	}

	setPath(p);

	if (m_label != 0)
	{
		if (m_isOutput)
		{
			m_label->setPos(margin() - width(), 0);
		}
		else
		{
			m_label->setPos(margin() + 2 * radius(), 0);
		}
	}
	else
	{
		m_pixmap->setPos(0, 0);
	}

	brush = QBrush(Qt::black);

	if (m_effect_port != 0)
	{
		if (m_effect_port->get_name().get() == "rgba")
		{
			brush = QBrush(QImage(":/icons/rgba.png").scaledToHeight(
				height(), Qt::SmoothTransformation));
		}

		if (m_effect_port->get_name().get() == "rgb")
		{
			brush = QBrush(QImage(":/icons/rgb.png").scaledToHeight(
				height(), Qt::SmoothTransformation));
		}

		if (m_effect_port->get_name().get() == "rg")
		{
			brush = QBrush(QImage(":/icons/rg.png").scaledToHeight(
				height(), Qt::SmoothTransformation));
		}

		if (m_effect_port->get_name().get() == "ba")
		{
			brush = QBrush(QImage(":/icons/ba.png").scaledToHeight(
				height(), Qt::SmoothTransformation));
		}

		if (m_effect_port->get_name().get() == "r")
		{
			brush = QBrush(Qt::red);
		}

		if (m_effect_port->get_name().get() == "g")
		{
			brush = QBrush(Qt::green);
		}

		if (m_effect_port->get_name().get() == "b")
		{
			brush = QBrush(Qt::blue);
		}

		if (m_effect_port->get_name().get() == "a")
		{
			brush = QBrush(Qt::white);
		}

		if (m_effect_port->get_name().get() == "a")
		{
			brush = QBrush(Qt::white);
		}

		setToolTip(QString::number(m_effect_port->get_value_count()));
	}

	setBrush(brush);
}

void QNEPort::update_tool_tip()
{
	if (m_effect_port != 0)
	{
		setToolTip(QString::number(m_effect_port->get_value_count()));
	}
}

bool QNEPort::isOutput()
{
	return m_isOutput;
}

QColor QNEPort::color()
{
	return m_color;
}

void QNEPort::set_color(const QColor &c)
{
	m_color = c;

	update();
}

QVector<Connection*>& QNEPort::connections()
{
	return m_connections;
}

void QNEPort::setFlags(int f)
{
	QFont font(scene()->font());
	m_flags = f;

	if (m_flags == ImagePort)
	{
		if (m_pixmap == 0)
		{
			m_pixmap = new QGraphicsPixmapItem(this);
		}

		if (name().isEmpty())
		{
			QPixmap pixmap(QSize(32, 32));

			pixmap.fill(color());

			m_pixmap->setPixmap(pixmap);
		}
		else
		{
			m_pixmap->setPixmap(QPixmap(name()));
		}

		delete m_label;
		m_label = 0;
	}
	else
	{
		if (m_label == 0)
		{
			m_label = new QGraphicsTextItem(this);
		}

		m_label->setPlainText(m_name);

		delete m_pixmap;
		m_pixmap = 0;

		if (m_flags == TypePort)
		{
			font.setItalic(true);
		}

		if (m_flags == NamePort)
		{
			font.setBold(true);
		}

		m_label->setFont(font);
	}

	update();
}

QNEBlock* QNEPort::block() const
{
	return m_block;
}

bool QNEPort::isConnected(QNEPort *other)
{
	foreach(Connection *conn, m_connections)
	{
		if ((conn->get_port1() == other) ||
			(conn->get_port2() == other))
		{
			return true;
		}
	}

	return false;
}

QPointF QNEPort::connection_pos() const
{
	QPointF result;

	result = scenePos();

	result.setY(result.y() + radius());

	if (m_isOutput)
	{
		result.setX(result.x() + 2 * radius());
	}
	else
	{
		result.setX(result.x());
	}

	return result;
}

QVariant QNEPort::itemChange(GraphicsItemChange change, const QVariant &value)
{
	if (change == ItemScenePositionHasChanged)
	{
		foreach(Connection *conn, m_connections)
		{
			conn->update_pos_from_ports();
			conn->update_path();
		}
	}
	return value;
}

bool QNEPort::can_connect(QNEPort* port)
{
	if ((effect_port() == 0) && (port->effect_port() == 0))
	{
		return true;
	}

	if ((effect_port() == 0) || (port->effect_port() == 0))
	{
		return false;
	}

	return effect_port()->check_connection(port->effect_port());
}

void QNEPort::connect(QNEPort* port)
{
	if ((effect_port() == 0) || (port->effect_port() == 0))
	{
		return;
	}

	effect_port()->connect(port->effect_port());
}

void QNEPort::disconnect(QNEPort* port)
{
	if ((effect_port() == 0) || (port->effect_port() == 0))
	{
		return;
	}

	effect_port()->disconnect(port->effect_port());
}

el::EffectNodePortPtr QNEPort::effect_port() const
{
	return m_effect_port;
}

void QNEPort::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
	QString str;

	str = QInputDialog::getText(0, "", "Text", QLineEdit::Normal, name());

	if (!str.isEmpty())
	{
		setName(str);
	}
}
