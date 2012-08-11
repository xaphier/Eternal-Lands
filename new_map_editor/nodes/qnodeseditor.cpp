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

#include "qnodeseditor.hpp"

#include <QGraphicsScene>
#include <QEvent>
#include <QGraphicsSceneMouseEvent>

#include "qneport.hpp"
#include "connection.hpp"
#include "qneblock.hpp"

QNodesEditor::QNodesEditor(QObject *parent) :
    QObject(parent)
{
	conn = 0;
}

QNodesEditor::~QNodesEditor()
{
}

void QNodesEditor::install(QGraphicsScene *s)
{
	s->installEventFilter(this);
	scene = s;
}

QGraphicsItem* QNodesEditor::get_item_at(const QPointF &pos)
{
	QList<QGraphicsItem*> items = scene->items(QRectF(pos - QPointF(1,1),
		QSize(3,3)));

	foreach(QGraphicsItem *item, items)
		if (item->type() > QGraphicsItem::UserType)
			return item;

	return 0;
}

QGraphicsItem* QNodesEditor::get_item_at(const QPointF &pos, const int type)
{
	QList<QGraphicsItem*> items = scene->items(QRectF(pos - QPointF(1,1),
		QSize(3,3)));

	foreach(QGraphicsItem *item, items)
	{
		if (item->type() == type)
		{
			return item;
		}
	}

	return 0;
}

void QNodesEditor::update_tool_tips()
{
	QList<QGraphicsItem*> items = scene->items();

	foreach(QGraphicsItem *item, items)
	{
		if (item->type() == QNEPort::Type)
		{
			((QNEPort*)item)->update_tool_tip();
		}
	}
}

void QNodesEditor::fill_ports_map(
	std::map<el::EffectNodePortPtr, QNEPort*> &ports)
{
	QList<QGraphicsItem*> items = scene->items();

	foreach(QGraphicsItem *item, items)
	{
		if (item->type() == QNEPort::Type)
		{
			ports[((QNEPort*)item)->effect_port()] =
				(QNEPort*)item;
		}
	}
}

void QNodesEditor::update_connections()
{
	QList<QGraphicsItem*> items = scene->items();

	foreach(QGraphicsItem *item, items)
	{
		if (item->type() == Connection::Type)
		{
			((Connection*)item)->update_path();
		}
	}
}

bool QNodesEditor::eventFilter(QObject *o, QEvent *e)
{
	QGraphicsSceneMouseEvent *me = (QGraphicsSceneMouseEvent*) e;

	switch ((int) e->type())
	{
	case QEvent::GraphicsSceneMousePress:
	{
		switch ((int) me->button())
		{
		case Qt::LeftButton:
		{
			QGraphicsItem *item = get_item_at(me->scenePos());
			if (item && item->type() == QNEPort::Type)
			{
				conn = new Connection(0, scene);
				conn->set_port1((QNEPort*) item);
				conn->set_pos1(((QNEPort*) item)->connection_pos());
				conn->set_pos2(me->scenePos());
				conn->update_path();
				update_tool_tips();

				return true;
			} else if (item && item->type() == QNEBlock::Type)
			{
				/* if (selBlock)
					selBlock->setSelected(); */
				// selBlock = (QNEBlock*) item;
			}
			break;
		}
		case Qt::RightButton:
		{
			QGraphicsItem *item = get_item_at(me->scenePos());
			if (item && (item->type() == Connection::Type || item->type() == QNEBlock::Type))
			{
				delete item;
				update_tool_tips();
			}
			// if (selBlock == (QNEBlock*) item)
				// selBlock = 0;
			break;
		}
		}
	}
	case QEvent::GraphicsSceneMouseMove:
	{
		if (conn)
		{
			conn->set_pos2(me->scenePos());
			conn->update_path();
			update_tool_tips();
			return true;
		}
		break;
	}
	case QEvent::GraphicsSceneMouseRelease:
	{
		if (conn && me->button() == Qt::LeftButton)
		{
			QGraphicsItem *item = get_item_at(me->scenePos(),
				QNEPort::Type);

			if (item != 0)
			{
				QNEPort *port1 = conn->get_port1();
				QNEPort *port2 = (QNEPort*) item;

				if (port1->can_connect(port2))
				{
					conn->set_pos2(port2->connection_pos());
					conn->set_port2(port2);
					conn->update_path();
					conn = 0;
					update_tool_tips();
					return true;
				}
			}

			delete conn;
			conn = 0;
			update_tool_tips();
			return true;
		}
		break;
	}
	}
	return QObject::eventFilter(o, e);
}
