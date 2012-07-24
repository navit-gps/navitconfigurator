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

#ifndef BOXGRAPHICSITEM_H_
#define BOXGRAPHICSITEM_H_

#include <QtCore/QRectF>
#include <QtCore/QPointF>
#include <QtCore/QString>

#include <QtGui/QGraphicsRectItem>
#include <QtGui/QGraphicsItem>
#include <QtGui/QGraphicsSceneMouseEvent>
#include <QtGui/QPainter>
#include <QtGui/QStyleOptionGraphicsItem>
#include <QtGui/QWidget>
#include <QtGui/QImage>

#include <navitconf/data/TreeItem.h>
#include <navitconf/data/TreeModel.h>

#include "BoxGraphicsItemListener.h"
#include "BoxItemAttribute.h"

/**
 * An OSD node of the navit.xml file shown in the MapViewScene.
 */
class BoxGraphicsItem: public QGraphicsRectItem {

public:
	/**
	 * Unique IDs for each box item property.
	 * Attention: Must be in sync with internal list defined in method
	 * {@link #init(const QModelIndex&,const TreeItem&,int,int)}
	 */
	enum BoxItemAttributeId {
		OSD_TYPE_NAME,
		OSD_OSD_CONFIGURATION,
		OSD_LABEL,
		OSD_COMMAND,
		OSD_SRC,
		OSD_ICON_SRC,
		OSD_HORIZONTAL_ALIGN,
		OSD_VERTICAL_ALIGN,
		OSD_X_POSITION,
		OSD_Y_POSITION,
		OSD_WIDTH,
		OSD_HEIGHT,
		OSD_USE_SIZE,
		OSD_BACKGROUND_COLOR,
		OSD_TEXT_COLOR,
		OSD_COUNT
	};

	static int getAttributeCount();
	static QStringList getAttributeNames();

	/**
	 * Method for creating an instance of this BoxGraohicsItem.
	 * @param index The tree index this item is associated with.
	 * @param sceneWidth The width in pixel of this scene.
	 * @param sceneHeight The height in pixel of this scene.
	 */
	static BoxGraphicsItem* init(const QModelIndex& index, const TreeItem& item, int sceneWidth, int sceneHeight);
	/**
	 * Not valid default constructor.
	 */
	BoxGraphicsItem();
	/**
	 * Normal constructor sets the same valued as the inherited class {@link QGraphicsRectItem#QGraphicsRectItem()}
	 */
	BoxGraphicsItem(const QModelIndex& index, int x, int y, int width, int height);
	virtual ~BoxGraphicsItem();
	/**
	 * Copy assignment.
	 */
	BoxGraphicsItem& operator=(const BoxGraphicsItem& other);

	/**
	 * @return True, if this class holds valid data, i. e. width >= 0, otherwise false. Normally only used by overloaded default constructor.
	 */
	bool isValid() const;
	QModelIndex getModelIndex() const;
	void setPos(const QPointF& newPos);
	void setRect(const QRectF& newRect);

	void setIsLeft(bool isLeft);
	void setIsTop(bool isTop);
	bool isOsdConfiguration() const;
	void setIsOsdConfiguration(bool isOsdConfiguration);
	void setAttribute(BoxItemAttributeId id, const QString& value);
	BoxItemAttribute& getAttribute(BoxItemAttributeId id);
	const BoxItemAttribute& getAttribute(BoxItemAttributeId id) const;
	void setHasSize(bool hasSize);
	void sceneSizeChanged();
	/**
	 * @return True, it both items has the same row number at the index,
	 *         <code>i. e. index.row() == other.index.row()</code>.
	 */
	bool equalsByRow(const BoxGraphicsItem& other) const;
	/**
	 * Stores the differences between the given last state and the current state into the tree model.
	 */
	void storeChangedAttributes(TreeModel& model);
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0);

protected:

	/**
	 * Called, when a mouse button is clicked:
	 * <ol>
	 *  <li>Left  mouse button: Begin moving   this item and showing moving   mouse cursor.</li>
	 *  <li>Right mouse button: Begin resizing this item and showing resizing mouse cursor.</li>
	 * </ol>
	 */
	virtual void mousePressEvent(QGraphicsSceneMouseEvent* event);

	/**
	 * Called, when mouse is moved after pressing left or right button
	 * to move and resize interactively.
	 * @see #mousePressEvent(QGraphicsSceneMouseEvent*)
	 */
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
	/**
	 * Called, when the mouse is released to show the normal mouse cursor.
	 */
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);

private:
	QModelIndex index;
	QList<BoxItemAttribute> attributes;
	bool isResizing;
	QPointF moveStartPos;
	QPointF resizeStartEventPos;
	QRectF resizeStartRect;
	QPointF lastPosMovingFired;
	QRectF lastRectResizingFired;
	QPointF rightBottonPos;
	QImage image;
	void setImage(const QString& filename);
	void fireItemMoving();
	void fireItemMoveFinished();
	void fireItemResizing();
	void fireItemResizeFinished();
	BoxGraphicsItemListener& getListener();
};

#endif /* BOXGRAPHICSITEM_H_ */
