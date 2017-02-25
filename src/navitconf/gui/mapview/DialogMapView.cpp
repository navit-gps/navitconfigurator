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

#include "DialogMapView.h"

#include <cstddef> // for NULL

#include <QtCore/Qt>
#include <QtCore/QModelIndex>
#include <QtCore/QStringList>

#include <QtXml/QDomNode>

#include <QtGui/QStandardItem>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGraphicsRectItem>

#include <navitconf/file/Settings.h>
#include <navitconf/data/TreeItem.h>

#include "BoxGraphicsItem.h"
#include "BoxItemDelegate.h"

const char* const CONFIG_NAVIT_PATH = "config/navit";
const char* const CONFIG_NAVIT_OSD_PATH = "config/navit/osd";

DialogMapView::DialogMapView(QWidget& parent, TreeModel& treeModel)
:
	QDialog(&parent),
	ui(),
	treeModel(treeModel),
	scene(*this),
	tableModel(BoxGraphicsItem::getAttributeCount(), 1),
	checkBoxes(),
	isTableViewBoxPropertiesRefreshing(false)
{
	ui.setupUi(this);
	ui.comboBoxDisplaySize->setCurrentIndex(Settings::getInstance().getInt(Settings::DEVICE_DISPLAY_SIZE));
	ui.checkBoxLandscape->setChecked(Settings::getInstance().getBool(Settings::DEVICE_DISPLAY_IS_LANDSCAPE, false));
	initTableViewBoxProperties();
	ui.graphicsView->setScene(&scene);
	ui.graphicsView->setAcceptDrops(true);
	// connect slots for changed data in table view
	connect(&tableModel, SIGNAL(dataChanged                    (const QModelIndex&, const QModelIndex&)),
			this,        SLOT(tableViewBoxPropertiesDataChanged(const QModelIndex&)));
	// connect slots for undo state changes
	QUndoStack* undoStack = &treeModel.getUndoStack();
	connect(undoStack, SIGNAL(canUndoChanged(bool)),
			this,      SLOT(  undoStackStateChanged()));
	connect(undoStack, SIGNAL(canRedoChanged(bool)),
			this,      SLOT(  undoStackStateChanged()));
//	connect(undoStack, SIGNAL(cleanChanged(bool)),
//			this,      SLOT(  cleanChanged(bool)));
	// fill graphics view
	fillGraphicsView();
	comboBoxDisplaySizeIndexChanged();
	undoStackStateChanged();
	exec(); // show dialog
}

DialogMapView::~DialogMapView() {
}

void DialogMapView::initTableViewBoxProperties() {
	// select only one row
	ui.tableViewBoxProperties->setSelectionMode(QAbstractItemView::SingleSelection);
	ui.tableViewBoxProperties->setSelectionBehavior(QAbstractItemView::SelectRows);
	// set column names
	tableModel.setHorizontalHeaderItem(0, new QStandardItem(tr("Value")));
	QStringList verticalHeaderLabels;
	tableModel.setVerticalHeaderLabels(BoxGraphicsItem::getAttributeNames());
	// add rows
	for (int i = 0; i < BoxGraphicsItem::getAttributeCount(); i++) {
		QStandardItem* item = new QStandardItem("");
		item->setEditable(true);
		tableModel.setItem(i, item);
	}
	checkBoxes.append(BoxGraphicsItem::OSD_OSD_CONFIGURATION);
	checkBoxes.append(BoxGraphicsItem::OSD_USE_SIZE);
	checkBoxes.append(BoxGraphicsItem::OSD_HORIZONTAL_ALIGN);
	checkBoxes.append(BoxGraphicsItem::OSD_VERTICAL_ALIGN);
	for (int i = 0; i < checkBoxes.size(); i++) {
		tableModel.item(checkBoxes[i])->setCheckable(true);
		tableModel.item(checkBoxes[i])->setEditable(false);
	}
	// delegate changing of type name to an extra class
	ui.tableViewBoxProperties->setItemDelegate(new BoxItemDelegate(tableModel, *ui.tableViewBoxProperties));
	// edit cells in table view with click or F2
	ui.tableViewBoxProperties->setEditTriggers(QAbstractItemView::SelectedClicked | QAbstractItemView::EditKeyPressed);
	// set data model for table view
	ui.tableViewBoxProperties->setModel(&tableModel);
	// resize first column automatically
	ui.tableViewBoxProperties->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
}

void DialogMapView::fillGraphicsView() {
	QModelIndex index = treeModel.getModelIndexByDomPath(CONFIG_NAVIT_OSD_PATH);
	BoxGraphicsItem* boxItem = NULL;
	while (index.isValid()) {
		TreeItem& treeItem = treeModel.getTreeItem(index);
		if (treeItem.getDomNode().nodeName() == "osd") {
			QString enabled = treeItem.getAttribute("enabled");
			if (enabled != "no" && enabled != "0") {
				boxItem = BoxGraphicsItem::init(index, treeItem, scene.width(), scene.height());
				scene.addItem(boxItem);
			}
		}
		index = index.sibling(index.row() + 1, 0);
	}
	if (boxItem != NULL) { // more than grid exists
		boxItem->setSelected(true); // select the inserted item
		boxItemSelected(*boxItem);
	}
}

void DialogMapView::comboBoxDisplaySizeIndexChanged() {
	QStringList sizeList = ui.comboBoxDisplaySize->currentText().split(" x ");
	int width = sizeList[0].toInt();
	int height = sizeList[1].split(" ")[0].toInt();
	if (!ui.checkBoxLandscape->isChecked()) {
		int tmp = width;
		width = height;
		height = tmp;
	}
	QMargins margins = ui.graphicsView->contentsMargins();
	ui.graphicsView->setMaximumSize(
			width + margins.left() + margins.right(),
			height + margins.top() + margins.bottom());
	scene.setSceneRect(0, 0, width, height);
	layout()->activate(); // validate layout
	setBaseSize(sizeHint()); // set new window size
	// inform all box items that scene size has changed
	QList<QGraphicsItem*> sceneItems = scene.items();
	for (int i = 0; i < sceneItems.size(); i++) {
		QGraphicsItem* item = sceneItems[i];
		BoxGraphicsItem* boxItem = dynamic_cast<BoxGraphicsItem*>(item);
		if (boxItem != NULL) { // use only items of type BoxGraphicsItem
			boxItem->sceneSizeChanged();
		}
	}
	BoxGraphicsItem* selected = scene.getSelectedBoxGraphicsItem();
	if (selected != NULL) {
		boxItemSelected(*selected);
	}
}

void DialogMapView::tableViewBoxPropertiesDataChanged(const QModelIndex& modelIndex) {
	if (!isTableViewBoxPropertiesRefreshing) {
		if (scene.selectedItems().size() == 0) {
			return; // no selected box item, so ignore changes
		}
		BoxGraphicsItem& boxItem = *dynamic_cast<BoxGraphicsItem*>(scene.selectedItems()[0]);
		QStandardItem& tableItem = *tableModel.item(modelIndex.row());
		QString text = tableItem.text();
		int number = text.toInt();
		bool isChecked = tableItem.checkState() == Qt::Checked;
		switch (modelIndex.row()) {
			case BoxGraphicsItem::OSD_OSD_CONFIGURATION:
				boxItem.setIsOsdConfiguration(isChecked);
				break;
			case BoxGraphicsItem::OSD_HORIZONTAL_ALIGN:
				boxItem.setIsLeft(isChecked);
				refreshHorizontalAlign(boxItem);
				break;
			case BoxGraphicsItem::OSD_VERTICAL_ALIGN:
				boxItem.setIsTop(isChecked);
				refreshVerticalAlign(boxItem);
				break;
			case BoxGraphicsItem::OSD_X_POSITION:
				boxItem.setPos(QPointF(number, boxItem.y()));
				break;
			case BoxGraphicsItem::OSD_Y_POSITION:
				boxItem.setPos(QPointF(boxItem.x(), number));
				break;
			case BoxGraphicsItem::OSD_WIDTH: {
				QRectF newRect = boxItem.rect();
				newRect.setWidth(number);
				boxItem.setRect(newRect);
				break;
			}
			case BoxGraphicsItem::OSD_HEIGHT: {
				QRectF newRect = boxItem.rect();
				newRect.setHeight(number);
				boxItem.setRect(newRect);
				break;
			}
			case BoxGraphicsItem::OSD_USE_SIZE:
				boxItem.setHasSize(isChecked);
				break;
			default:
				boxItem.setAttribute(static_cast<BoxGraphicsItem::BoxItemAttributeId>(modelIndex.row()), text);
				break;
		}
		boxItem.storeChangedAttributes(treeModel);
	}
}

void DialogMapView::undoStackStateChanged() {
	ui.buttonUndo->setEnabled(treeModel.getUndoStack().canUndo());
	ui.buttonRedo->setEnabled(treeModel.getUndoStack().canRedo());
}

void DialogMapView::buttonAddClicked() {
	QModelIndex index = treeModel.getModelIndexByDomPath(CONFIG_NAVIT_PATH);
	int countChilds = treeModel.getTreeItem(index).getDomNode().childNodes().size();
	BoxGraphicsItem* sourceBoxItem = scene.getSelectedBoxGraphicsItem();
	treeModel.getUndoStack().beginMacro("Add Box Item");
	if (sourceBoxItem != NULL) { // selected box item exists
		treeModel.copyTreeItem(sourceBoxItem->getModelIndex(), countChilds, index);
	} else { // insert a new default box item, if there is no selected box item
		treeModel.insertTreeItem("osd", countChilds, index);
	}
	treeModel.getUndoStack().endMacro();
	initViewsAfterUndoRedo();
	scene.selectLastBoxGraphicsItemOnly();
	boxItemSelected(*scene.getLastBoxGraphicsItem());
}

void DialogMapView::buttonDeleteClicked() {
	QModelIndex index = treeModel.getModelIndexByDomPath(CONFIG_NAVIT_PATH);
	BoxGraphicsItem* sourceBoxItem = scene.getSelectedBoxGraphicsItem();
	if (sourceBoxItem != NULL) { // selected box item exists
		QModelIndex childIndex = sourceBoxItem->getModelIndex();
		treeModel.removeRow(childIndex.row(), childIndex.parent());
		initViewsAfterUndoRedo();
		scene.selectLastBoxGraphicsItemOnly();
		boxItemSelected(*scene.getLastBoxGraphicsItem());
	}
}

void DialogMapView::buttonUndoClicked() {
	scene.storeSelection();
	treeModel.getUndoStack().undo();
	initViewsAfterUndoRedo();
}

void DialogMapView::buttonRedoClicked() {
	scene.storeSelection();
	treeModel.getUndoStack().redo();
	initViewsAfterUndoRedo();
}

void DialogMapView::initViewsAfterUndoRedo() {
	scene.clear();
	fillGraphicsView();
	scene.restoreSelection();
	comboBoxDisplaySizeIndexChanged();
}

void DialogMapView::boxItemSelected(const BoxGraphicsItem& boxItem) {
	isTableViewBoxPropertiesRefreshing = true;
	for (int i = 0; i < BoxGraphicsItem::getAttributeCount(); i++) {
		boxItem.getAttribute(static_cast<BoxGraphicsItem::BoxItemAttributeId>(i)).fillCellInTable(tableModel);
	}
	isTableViewBoxPropertiesRefreshing = false;
	refreshHorizontalAlign(boxItem);
	refreshVerticalAlign(boxItem);
	boxItemMoving(boxItem);
	boxItemResizing(boxItem);
}

void DialogMapView::refreshHorizontalAlign(const BoxGraphicsItem& boxItem) {
	isTableViewBoxPropertiesRefreshing = true;
	boxItem.getAttribute(BoxGraphicsItem::OSD_HORIZONTAL_ALIGN).fillCellInTable(tableModel);
	isTableViewBoxPropertiesRefreshing = false;
}

void DialogMapView::refreshVerticalAlign(const BoxGraphicsItem& boxItem) {
	isTableViewBoxPropertiesRefreshing = true;
	boxItem.getAttribute(BoxGraphicsItem::OSD_VERTICAL_ALIGN).fillCellInTable(tableModel);
	isTableViewBoxPropertiesRefreshing = false;
}

void DialogMapView::boxItemMoving(const BoxGraphicsItem& boxItem) {
	isTableViewBoxPropertiesRefreshing = true;
	boxItem.getAttribute(BoxGraphicsItem::OSD_X_POSITION).fillCellInTable(tableModel);
	boxItem.getAttribute(BoxGraphicsItem::OSD_Y_POSITION).fillCellInTable(tableModel);
	isTableViewBoxPropertiesRefreshing = false;
}

void DialogMapView::boxItemResizing(const BoxGraphicsItem& boxItem) {
	isTableViewBoxPropertiesRefreshing = true;
	boxItem.getAttribute(BoxGraphicsItem::OSD_WIDTH).fillCellInTable(tableModel);
	boxItem.getAttribute(BoxGraphicsItem::OSD_HEIGHT).fillCellInTable(tableModel);
	isTableViewBoxPropertiesRefreshing = false;
}

void DialogMapView::boxItemMoveFinished(BoxGraphicsItem& boxItem) {
	boxItem.storeChangedAttributes(treeModel);
}

void DialogMapView::boxItemResizeFinished(BoxGraphicsItem& boxItem) {
	boxItem.storeChangedAttributes(treeModel);
}

void DialogMapView::accept() {
	Settings::getInstance().setInt(Settings::DEVICE_DISPLAY_SIZE, ui.comboBoxDisplaySize->currentIndex());
	Settings::getInstance().setBool(Settings::DEVICE_DISPLAY_IS_LANDSCAPE, ui.checkBoxLandscape->isChecked());
	setVisible(false); // close dialog
}
