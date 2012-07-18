/*
 * MapViewScene.h
 *
 *  Created on: 19.06.2012
 *      Author: remo
 */

#ifndef MAPVIEWSCENE_H_
#define MAPVIEWSCENE_H_

#include <QtCore/QList>

#include <QtGui/QGraphicsScene>
#include <QtGui/QGraphicsItem>
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
