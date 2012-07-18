/*
 * MapViewScene.cpp
 *
 *  Created on: 19.06.2012
 *      Author: remo
 */

#include "MapViewScene.h"

#include <cstddef>

#include <QtCore/QDebug>

#include "BoxGraphicsItem.h"

MapViewScene::MapViewScene(BoxGraphicsItemListener& listener)
:
	QGraphicsScene()
	, listener(listener)
	, gridPixel(10)
	, color(0xA0, 0xA0, 0xA0, 0xFF)
	, cachedSize(0)
	, cachedSelectedItems()
{
}

MapViewScene::~MapViewScene() {
}

BoxGraphicsItemListener& MapViewScene::getBoxGraphicsItemListener() {
	return listener;
}

void MapViewScene::drawBackground(QPainter* painter, const QRectF& rect) {
	QGraphicsScene::drawBackground(painter, rect);
//	painter->save();
	painter->setPen(color);
	int w = width();
	int h = height();
	for (int x = gridPixel; x < w; x += gridPixel) {
		painter->drawLine(x, 0, x, h);
	}
	for (int y = gridPixel; y < h; y += gridPixel) {
		painter->drawLine(0, y, w, y);
	}
//	painter->end();
}

BoxGraphicsItem* MapViewScene::getLastBoxGraphicsItem() {
	QList<QGraphicsItem*> allItems = items();
	if (allItems.size() == 0) {
		return NULL;
	}
	return dynamic_cast<BoxGraphicsItem*>(allItems[0]);
}

BoxGraphicsItem* MapViewScene::getSelectedBoxGraphicsItem() {
	QList<QGraphicsItem*> allSelectedItems = selectedItems();
	if (allSelectedItems.size() == 0) {
		return NULL;
	}
	return dynamic_cast<BoxGraphicsItem*>(allSelectedItems[0]);
}

void MapViewScene::selectLastBoxGraphicsItemOnly() {
	QList<QGraphicsItem*> allItems = items();
	if (allItems.size() == 0) {
		return;
	}
	for (int i = 1; i < allItems.size(); i++) {
		allItems[i]->setSelected(false);
	}
	getLastBoxGraphicsItem()->setSelected(true);
}

void MapViewScene::storeSelection() {
	QList<QGraphicsItem*> allItems = items();
	cachedSize = allItems.size();
	cachedSelectedItems.clear();
	for (int i = 0; i < allItems.size(); i++) {
		if (allItems[i]->isSelected()) {
			cachedSelectedItems.append(i);
		}
	}
}

void MapViewScene::restoreSelection() {
	QList<QGraphicsItem*> allItems = items();
	if (cachedSize == allItems.size()) {
		for (int i = 0; i < allItems.size(); i++) {
			allItems[i]->setSelected(cachedSelectedItems.contains(i));
		}
	} else {
		selectLastBoxGraphicsItemOnly();
	}
}

//void MapViewScene::mousePressEvent(QGraphicsSceneMouseEvent* event) {
//	qDebug() << "MapViewScene::mousePressEvent";
//	QGraphicsScene::mousePressEvent(event);
//	if (event->isAccepted()) {
//		qDebug() << "Accepted...";
//	}
//}
//
//void MapViewScene::dragEnterEvent(QGraphicsSceneDragDropEvent* event) {
//	QGraphicsScene::dragEnterEvent(event);
//	qDebug() << "MapViewScene::dragMoveEvent";
//}
//
////
//void MapViewScene::dragMoveEvent(QGraphicsSceneDragDropEvent* event) {
//	qDebug() << "MapViewScene::dragMoveEvent";
//	QGraphicsScene::dragMoveEvent(event);
//}
