/***************************************************************************
    File                 : WorksheetElementContainerPrivate.h
    Project              : LabPlot
    Description          : Private members of WorksheetElementContainer.
    --------------------------------------------------------------------
    Copyright            : (C) 2009 Tilman Benkert (thzs@gmx.net)
	Copyright            : (C) 2012-2015 by Alexander Semke (alexander.semke@web.de)

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *  This program is free software; you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation; either version 2 of the License, or      *
 *  (at your option) any later version.                                    *
 *                                                                         *
 *  This program is distributed in the hope that it will be useful,        *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *  GNU General Public License for more details.                           *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the Free Software           *
 *   Foundation, Inc., 51 Franklin Street, Fifth Floor,                    *
 *   Boston, MA  02110-1301  USA                                           *
 *                                                                         *
 ***************************************************************************/

#ifndef WORKSHEETELEMENTCONTAINERPRIVATE_H
#define WORKSHEETELEMENTCONTAINERPRIVATE_H

#include <QGraphicsItem>

class QGraphicsSceneContextMenuEvent;
class WorksheetElementContainer;

class WorksheetElementContainerPrivate : public QGraphicsItem {
	public:
		explicit WorksheetElementContainerPrivate(WorksheetElementContainer*);
		virtual ~WorksheetElementContainerPrivate(){}

		QString name() const;
		virtual QRectF boundingRect() const;
		virtual void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget* widget = 0);
		virtual void hoverEnterEvent(QGraphicsSceneHoverEvent*);
		virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent*);

		bool swapVisible(bool on);
		void prepareGeometryChangeRequested();
		void recalcShapeAndBoundingRect();
		void contextMenuEvent(QGraphicsSceneContextMenuEvent*);

		WorksheetElementContainer* q;
		QRectF boundingRectangle;
		QPainterPath containerShape;
		QRectF rect;
		bool m_hovered;
		bool m_printing;

	signals:
		void selectedChange(QGraphicsItem*);
};

#endif
