/**
 * NavitConfigurator, makes it easy to configure Navit
 * Copyright (C) 2012 Raimar Bühmann
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MAPVIEWSCENE_H_
#define MAPVIEWSCENE_H_

#include <QtCore/QList>

#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsItem>
//#include <QtGui/QGraphicsSceneDragDropEvent>

#include "BoxGraphicsItemListener.h"

/**
 * Shows the GridLines and all BoxGraphicsItem the navit.xml contains.
 */
class MapViewScene: public QGraphicsScene {
public:
	MapViewScene(BoxGraphicsItemListener& listener);
	virtual ~MapViewScene();

	/**
	 * Draws the background grid in light gray with 10 pixels distance.
	 */
	void drawBackground(QPainter* painter, const QRectF& rect);
	/**
	 * @return The registered listener for box item events.
	 */
	BoxGraphicsItemListener& getBoxGraphicsItemListener();
	BoxGraphicsItem* getLastBoxGraphicsItem();
	BoxGraphicsItem* getSelectedBoxGraphicsItem();
	/**
	 * Select only the last box item.
	 */
	void selectLastBoxGraphicsItemOnly();
	void storeSelection();
	void restoreSelection();

//protected:
//	/** Override methods to get mouse events */
//	void mousePressEvent(QGraphicsSceneMouseEvent* event);
//	void dragEnterEvent(QGraphicsSceneDragDropEvent* event);
//	void dragMoveEvent(QGraphicsSceneDragDropEvent* event);
private:
	BoxGraphicsItemListener& listener;
	int gridPixel;
	QColor color;
	int cachedSize;
	QList<int> cachedSelectedItems;
};

#endif /* MAPVIEWSCENE_H_ */
