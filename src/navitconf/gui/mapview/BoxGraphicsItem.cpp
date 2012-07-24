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

#include "BoxGraphicsItem.h"

#include <QtCore/QObject>

#include <QtGui/QApplication>
#include <QtGui/QBrush>
#include <QtGui/QGraphicsScene>

#include <navitconf/file/Settings.h>

#include "MapViewScene.h"

// static part

/**
 * The string describing the list of all attributes separated by ",". Each attribute is defined by tree or four strings separated by "=",
 * which means:
 * <ul>
 *  <li>Type char: S = QString, B = bool, I = int</li>
 *  <li>Attribute key name, if the table line represents an attribute of the OSD-tag, otherwise no char.</li>
 *  <li>Description name shown in each line of the table.</li>
 *  <li>Optionally each attribute of type bool can have two describing names for the true and the false value separated by "|".</li>
 * </ul>
 */
static QStringList descriptionOfAttributes = QObject::tr(
		"S=type=Type Name,"
		"B=osd_configuration=OSD Configuration,"
		"S=label=Label,"
		"S=command=Command,"
		"S=src=Source,"
		"S=icon_src=Icon Source,"
		"B==Horizontal Align=Left|Right,"
		"B==Vertical Align=Top|Bottom,"
		"I=x=X-Position,"
		"I=y=Y-Position,"
		"I=w=Width,"
		"I=h=Height,"
		"B==Use Size,"
		"C=background_color=Background Color,"
		"C=text_color=Text Color").split(",");

int BoxGraphicsItem::getAttributeCount() {
	return descriptionOfAttributes.size();
}

QStringList BoxGraphicsItem::getAttributeNames() {
	QStringList names;
	for (int i = 0; i < descriptionOfAttributes.size(); i++) {
		QStringList triple = descriptionOfAttributes[i].split("=");
		names.append(triple[2]);
	}
	return names;
}

// non static part

BoxGraphicsItem::BoxGraphicsItem()
:
	QGraphicsRectItem()
{
}

BoxGraphicsItem* BoxGraphicsItem::init(const QModelIndex& index, const TreeItem& item, int sceneWidth, int sceneHeight) {
	int x = item.getAttribute("x").toInt();
	int y = item.getAttribute("y").toInt();
	int width  = item.getAttribute("w").toInt();
	int height = item.getAttribute("h").toInt();
	bool isLeft = x >= 0;
	bool isTop  = y >= 0;
	bool hasSize = !item.getAttribute("w").isEmpty() && width > 0 && !item.getAttribute("h").isEmpty() && height > 0;
	if (!isLeft) {
		x = sceneWidth + x; // x < 0 !!!
	}
	if (!isTop) {
		y = sceneHeight + y;  // y < 0 !!!
	}
	if (width < 1) {
		width = 16;
	}
	if (height < 1) {
		height = 16;
	}
	BoxGraphicsItem* boxItem = new BoxGraphicsItem(index, x, y, width, height);
	// initialize the values using the list of strings defining the attributes
	QList<BoxItemAttribute>& attributes = boxItem->attributes;
	for (int i = 0; i < descriptionOfAttributes.size(); i++) {
		QStringList triple = descriptionOfAttributes[i].split("=");
		QString trueValue;
		QString falseValue;
		if (triple.size() == 4) {
			QStringList boolValues = triple[3].split("|");
			trueValue = boolValues[0];
			falseValue = boolValues[1];
		}
		attributes.append(BoxItemAttribute(i, triple[0], triple[1], triple[2], trueValue, falseValue));
		attributes[i].setValueFromTreeItem(item);
	}
	boxItem->rightBottonPos.setX(sceneWidth - x);
	boxItem->rightBottonPos.setY(sceneHeight - y);
	attributes[BoxGraphicsItem::OSD_HORIZONTAL_ALIGN].setBool(isLeft);
	attributes[BoxGraphicsItem::OSD_VERTICAL_ALIGN].setBool(isTop);
	boxItem->setHasSize(hasSize);
	if (!boxItem->getAttribute(BoxGraphicsItem::OSD_SRC).getString().isEmpty()) {
		boxItem->setImage(Settings::getInstance().getDir().absolutePath() + "/res/drawable/" + boxItem->getAttribute(BoxGraphicsItem::OSD_SRC).getString());
	}
	return boxItem;
}

BoxGraphicsItem::BoxGraphicsItem(const QModelIndex& index, int x, int y, int width, int height)
:
	QGraphicsRectItem(0, 0, width, height)
	, index(index)
	, attributes()
	, isResizing(false)
	, moveStartPos()
	, resizeStartEventPos()
	, resizeStartRect()
	, lastPosMovingFired()
	, lastRectResizingFired()
	, rightBottonPos()
{
	const QGraphicsItem::GraphicsItemFlags flags =
			QGraphicsItem::ItemIsMovable |    // for moving items with D&D
			QGraphicsItem::ItemIsSelectable | // for selecting items surrounded with dotted lines
			QGraphicsItem::ItemIsFocusable;   // for accepting key events on an item
	setFlags(flags);
    setAcceptHoverEvents(true);
    QGraphicsRectItem::setPos(x, y);
}

BoxGraphicsItem::~BoxGraphicsItem() {
}

BoxGraphicsItem& BoxGraphicsItem::operator=(const BoxGraphicsItem& other) {
	QGraphicsRectItem::setPos(other.pos());
	QGraphicsRectItem::setRect(other.rect());
	index = other.index;
	isResizing = other.isResizing;
	moveStartPos = other.moveStartPos;
	resizeStartEventPos = other.resizeStartEventPos;
	resizeStartRect = other.resizeStartRect;
	lastPosMovingFired = other.lastPosMovingFired;
	lastRectResizingFired = other.lastRectResizingFired;
	rightBottonPos = other.rightBottonPos;
	attributes = other.attributes;
	return *this;
}

bool BoxGraphicsItem::isValid() const {
	return index.isValid();
}

QModelIndex BoxGraphicsItem::getModelIndex() const {
	return index;
}

void BoxGraphicsItem::setPos(const QPointF& newPos) {
	QRectF r = rect();
	QPointF nP = newPos;
	if (nP.x() < 0) {
		nP.setX(0);
	} else if (nP.x() + r.width() > scene()->width()) {
		nP.setX(scene()->width() - r.width());
	}
	if (nP.y() < 0) {
		nP.setY(0);
	} else if (nP.y() + r.height() > scene()->height()) {
		nP.setY(scene()->height() - r.height());
	}
	attributes[OSD_X_POSITION].setInteger(nP.x());
	attributes[OSD_Y_POSITION].setInteger(nP.y());
	QGraphicsRectItem::setPos(nP);
	rightBottonPos.setX(scene()->width() - x());
	rightBottonPos.setY(scene()->height() - y());
	fireItemMoving();
}

void BoxGraphicsItem::setRect(const QRectF& newRect) {
	QRectF nR = newRect;
	if (nR.width() < 1) {
		nR.setWidth(1);
	} else if (x() + nR.width() > scene()->width()) {
		nR.setWidth(scene()->width() - x());
	}
	if (nR.height() < 1) {
		nR.setHeight(1);
	} else if (y() + nR.height() > scene()->height()) {
		nR.setHeight(scene()->height() - y());
	}
	attributes[OSD_WIDTH].setInteger(nR.width());
	attributes[OSD_HEIGHT].setInteger(nR.height());
	QGraphicsRectItem::setRect(nR);
	fireItemResizing();
}

void BoxGraphicsItem::setIsLeft(bool isLeft) {
	if (attributes[BoxGraphicsItem::OSD_HORIZONTAL_ALIGN].getBool() != isLeft) {
		QPointF pN = pos();
		pN.setX(scene()->width() - x() - rect().width());
		setPos(pN);
		attributes[BoxGraphicsItem::OSD_HORIZONTAL_ALIGN].setBool(isLeft);
	}
}

void BoxGraphicsItem::setIsTop(bool isTop) {
	if (attributes[BoxGraphicsItem::OSD_VERTICAL_ALIGN].getBool() != isTop) {
		QPointF pN = pos();
		pN.setY(scene()->height() - y() - rect().height());
		setPos(pN);
		attributes[BoxGraphicsItem::OSD_VERTICAL_ALIGN].setBool(isTop);
	}
}

void BoxGraphicsItem::setHasSize(bool hasSize) {
	attributes[BoxGraphicsItem::OSD_USE_SIZE].setBool(hasSize);
}

bool BoxGraphicsItem::isOsdConfiguration() const {
	return attributes[BoxGraphicsItem::OSD_OSD_CONFIGURATION].getBool();
}

void BoxGraphicsItem::setIsOsdConfiguration(bool isOsdConfiguration) {
	attributes[BoxGraphicsItem::OSD_OSD_CONFIGURATION].setBool(isOsdConfiguration);
}

void BoxGraphicsItem::setAttribute(BoxItemAttributeId id, const QString& value) {
	attributes[id].setString(value);
	if (id == OSD_LABEL) {
		setToolTip(value);
	}
	switch (id) {
		case OSD_LABEL:
		case OSD_BACKGROUND_COLOR:
		case OSD_TEXT_COLOR:
			update();
			break;
		default:
			break;
	}
}

BoxItemAttribute& BoxGraphicsItem::getAttribute(BoxItemAttributeId id) {
	return attributes[id];
}

const BoxItemAttribute& BoxGraphicsItem::getAttribute(BoxItemAttributeId id) const {
	return attributes.at(id);
}

void BoxGraphicsItem::setImage(const QString& filename) {
	image = QImage(filename);
	QSizeF size = image.size();
	if (size.width() > 0) {
		QRectF nR = rect();
		nR.setWidth(size.width());
		nR.setHeight(size.height());
		QGraphicsRectItem::setRect(nR);
		setBrush(QBrush(Qt::transparent));
	}
}

void BoxGraphicsItem::sceneSizeChanged() {
	QPointF nP = pos();
	if (!attributes[OSD_HORIZONTAL_ALIGN].getBool()) {
		nP.setX(scene()->width() - rightBottonPos.x());
	}
	if (!attributes[OSD_VERTICAL_ALIGN].getBool()) {
		nP.setY(scene()->height() - rightBottonPos.y());
	}
	setPos(nP);
}

bool BoxGraphicsItem::equalsByRow(const BoxGraphicsItem& other) const {
	return index.row() == other.index.row();
}

void BoxGraphicsItem::storeChangedAttributes(TreeModel& model) {
	BoxGraphicsItem& other = *BoxGraphicsItem::init(
			index, model.getTreeItem(index),
			scene()->width(), scene()->height());
	model.getUndoStack().beginMacro("Box Item Changes");
	for (int i = 0; i < BoxGraphicsItem::OSD_COUNT; i++) {
		BoxItemAttributeId id = static_cast<BoxItemAttributeId>(i);
		if (getAttribute(id) != other.getAttribute(id)) {
			// set attribute with changed value
			model.setAttribute(index, getAttribute(id).getKey(), getAttribute(id).getString());
		}
	}
	if (other.x() != x() || other.y() != y()) {
		int tX = x();
		if (!attributes[OSD_HORIZONTAL_ALIGN].getBool()) { // convert to negative
			tX = tX - scene()->width();
		}
		model.setAttribute(index, "x", QString::number(tX));
		int tY = y();
		if (!attributes[OSD_VERTICAL_ALIGN].getBool()) { // convert to negative
			tY = tY - scene()->height();
		}
		model.setAttribute(index, "y", QString::number(tY));
	}
	QString tW;
	QString tH;
	if (attributes[BoxGraphicsItem::OSD_USE_SIZE].getBool()) {
		tW = QString::number(rect().width());
		tH = QString::number(rect().height());
	}
	if (model.getTreeItem(index).getAttribute("w") != tW) {
		model.setAttribute(index, "w", tW);
	}
	if (model.getTreeItem(index).getAttribute("h") != tH) {
		model.setAttribute(index, "h", tH);
	}
	model.getUndoStack().endMacro();
}

void BoxGraphicsItem::paint(QPainter* painter,
		const QStyleOptionGraphicsItem* option, QWidget* widget) {
	// draw rect in background-color
	QString colorString = getAttribute(OSD_BACKGROUND_COLOR).getString();
	QColor color = QColor(colorString.left(7));
	if (colorString.size() > 7) {
		color.setAlpha(colorString.mid(7).toInt(0, 16));
	} else {
		color.setAlpha(0x60);
	}
	setBrush(QBrush(color));
	QGraphicsRectItem::paint(painter, option, widget);
	// draw icon if it exists
	if (!getAttribute(OSD_SRC).getString().isEmpty()) {
		painter->drawImage(QPointF(0, 0), image);
	} else {
		// draw label if it exists
		QString label = getAttribute(OSD_LABEL).getString();
		if (!label.isEmpty()) {
			colorString = getAttribute(OSD_TEXT_COLOR).getString();
			if (colorString.isEmpty()) {
				color = QColor(0xff, 0xff, 0xff);
			} else {
				color = QColor(colorString);
			}
			painter->setPen(color);
			painter->drawText(rect(), label);
		}
	}
}

void BoxGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent* event) {
	QGraphicsRectItem::mousePressEvent(event); // call parent class
	if (event->isAccepted()) { //
		moveStartPos = pos();
		isResizing = event->button() == Qt::RightButton;
		if (isResizing) {
			resizeStartEventPos = event->pos();
			resizeStartRect = rect();
			foreach(QGraphicsItem* item, scene()->selectedItems()) {
				item->setSelected(false); // disable selection of all items
			}
			setSelected(true); // select this item
			QApplication::setOverrideCursor(QCursor(Qt::SizeFDiagCursor));
		} else {
			QApplication::setOverrideCursor(QCursor(Qt::SizeAllCursor));
		}
		getListener().boxItemSelected(*this);
	}
}

void BoxGraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
	// move by distance from beginning of left click position is made by calling the parent
	QGraphicsRectItem::mouseMoveEvent(event);
	if (isResizing) {
		// resize width and height by distance from beginning of right click position
		QPointF delta = event->pos() - resizeStartEventPos;
		QRectF newRect(
				resizeStartRect.x(),
				resizeStartRect.y(),
				resizeStartRect.width() + delta.x(),
				resizeStartRect.height() + delta.y());
		setRect(newRect);
		fireItemResizing();
	} else {
		// set x and y coordinate of this item back to the view if necessary
		setPos(pos());
	}
}

void BoxGraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
	QGraphicsRectItem::mouseReleaseEvent(event); // call parent class
	QApplication::restoreOverrideCursor();
	QList<QGraphicsItem*> allSelectedItems = scene()->selectedItems();
	if (allSelectedItems.size() >= 1) {
		// after move of more than one box item, the other selected box items
		// could have moved outside of the scene
		for (int i = 0; i < allSelectedItems.size(); i++) {
			BoxGraphicsItem& boxItem = *dynamic_cast<BoxGraphicsItem*>(allSelectedItems[i]);
			if (this != &boxItem) { // this is not me
				boxItem.setPos(boxItem.pos()); // set position to validate it
				boxItem.fireItemMoveFinished();
			} else {
				fireItemMoveFinished();
			}

		}
	}
}

void BoxGraphicsItem::fireItemMoving() {
	if (lastPosMovingFired != pos()) {
		getListener().boxItemMoving(*this);
		lastPosMovingFired = pos();
	}
}

void BoxGraphicsItem::fireItemMoveFinished() {
	if (moveStartPos != pos()) {
		getListener().boxItemMoveFinished(*this);
	}
}

void BoxGraphicsItem::fireItemResizing() {
	if (lastRectResizingFired != rect()) {
		getListener().boxItemResizing(*this);
		lastRectResizingFired = rect();
	}
}

void BoxGraphicsItem::fireItemResizeFinished() {
	if (resizeStartRect != rect()) {
		getListener().boxItemResizeFinished(*this);
	}
}

BoxGraphicsItemListener& BoxGraphicsItem::getListener() {
	MapViewScene& s = *dynamic_cast<MapViewScene*>(scene());
	return s.getBoxGraphicsItemListener();
}
