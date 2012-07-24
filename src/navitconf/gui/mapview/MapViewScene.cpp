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

#include "MapViewScene.h"

#include <cstddef>

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
