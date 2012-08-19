/* Copyright (c) 2012, STANISLAW ADASZEWSKI
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of STANISLAW ADASZEWSKI nor the
      names of its contributors may be used to endorse or promote products
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

#include "qneblock.hpp"

#include <QPen>
#include <QGraphicsScene>
#include <QFontMetrics>
#include <QPainter>

#include "qneport.hpp"

#include "../engine/effect/effectnode.hpp"

QNEBlock::QNEBlock(QGraphicsItem *parent, QGraphicsScene *scene) : QGraphicsPathItem(parent, scene)
{
	QPainterPath p;
	p.addRoundedRect(-50, -15, 100, 30, 5, 5);
	setPath(p);
	setPen(QPen(Qt::darkGreen));
	setBrush(Qt::green);
	setFlag(QGraphicsItem::ItemIsMovable);
	setFlag(QGraphicsItem::ItemIsSelectable);
	horzMargin = 20;
	vertMargin = 10;
	width = horzMargin;
	height = vertMargin;
}

QNEBlock::~QNEBlock()
{
}

QNEPort* QNEBlock::addPort(const QString &name, bool isOutput, int flags)
{
	return addPort(0, name, isOutput, flags);
}

QNEPort* QNEBlock::addPort(el::EffectNodePortPtr effect_port,
	const QString &name, bool isOutput, int flags)
{
	QNEPort* port;
	QPainterPath p;
	int y, w, h, h_in, h_out, w_in, w_out, y_in, y_out;

	port = new QNEPort(effect_port, this);
	port->setName(name);
	port->setIsOutput(isOutput);
	port->setNEBlock(this);
	port->setFlags(flags);

	w = 0;
	h = 0;
	h_in = 0;
	h_out = 0;
	w_in = 0;
	w_out = 0;

	foreach(QGraphicsItem *port_, children())
	{
		if (port_->type() != QNEPort::Type)
		{
			w = std::max(w, (int)port_->boundingRect().size().width());
			h += port_->boundingRect().size().height() + 2;

			continue;
		}

		port = (QNEPort*) port_;

		if (port->flags() == 0)
		{
			if (port->isOutput())
			{
				w_out = std::max(w_out, port->width());
				h_out += port->height();
			}
			else
			{
				w_in = std::max(w_in, port->width());
				h_in += port->height();
			}
		}
		else
		{
			w = std::max(w, port->width());
			h += port->height();
		}
	}

	w = std::max(w, w_in + w_out);
	h += std::max(h_in, h_out);

	width = w + horzMargin;
	height = h + vertMargin;

	p.addRoundedRect(-width/2, -height/2, width, height, 5, 5);
	setPath(p);

	y = -height / 2 + vertMargin;
	y_in = y;
	y_out = y;

	foreach(QGraphicsItem *port_, children())
	{
		if (port_->type() != QNEPort::Type)
		{
			y = std::max(y_in, y_out);

			port_->setPos(-w/2, y);

			y += port->height() + 2;
			y_in = y;
			y_out = y;

			continue;
		}

		QNEPort *port = (QNEPort*) port_;

		if (port->flags() != 0)
		{
			y = std::max(y_in, y_out);
			y_in = y;
			y_out = y;
		}

		if (port->flags() == QNEPort::ImagePort)
		{
			port->setPos(-w/2, y);
		}
		else
		{
			if (port->isOutput())
			{
				port->setPos(width/2, y_out);
			}
			else
			{
				port->setPos(-width/2 - 2 * port->radius(),
					y_in);
			}
		}

		if (port->flags() == 0)
		{
			if (port->isOutput())
			{
				y_out += port->height();
			}
			else
			{
				y_in += port->height();
			}
		}
		else
		{
			y += port->height();
			y_in = y;
			y_out = y;
		}
	}

	return port;
}

void QNEBlock::addInputPort(el::EffectNodePortPtr effect_port,
	const QString &name)
{
	addPort(effect_port, name, false);
}

void QNEBlock::addOutputPort(el::EffectNodePortPtr effect_port,
	const QString &name)
{
	addPort(effect_port, name, true);
}

#include <QStyleOptionGraphicsItem>

void QNEBlock::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(option)
	Q_UNUSED(widget)

	if (isSelected())
	{
		painter->setPen(QPen(Qt::darkGray));
		painter->setBrush(Qt::gray);
	}
	else
	{
		painter->setPen(QPen(Qt::darkGray));
		painter->setBrush(Qt::lightGray);
	}

	painter->drawPath(path());
}

QNEBlock* QNEBlock::clone()
{
	QNEBlock *b = new QNEBlock(0, scene());

	foreach(QGraphicsItem *port_, childItems())
	{
		if (port_->type() == QNEPort::Type)
		{
			QNEPort *port = (QNEPort*) port_;
			b->addPort(port->effect_port(), port->name(),
				port->isOutput(), port->flags());
		}
	}

	return b;
}

QVector<QNEPort*> QNEBlock::ports()
{
	QVector<QNEPort*> res;
	foreach(QGraphicsItem *port_, childItems())
	{
		if (port_->type() == QNEPort::Type)
			res.append((QNEPort*) port_);
	}
	return res;
}

QVariant QNEBlock::itemChange(GraphicsItemChange change, const QVariant &value)
{
	return value;
}

