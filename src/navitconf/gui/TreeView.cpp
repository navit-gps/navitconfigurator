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

#include "TreeView.h"

#include <cstddef>

#include <QtCore/qnamespace.h>
#include <QtCore/QList>

#include <QtGui/QMenu>
#include <QtGui/QHeaderView>

#include <navitconf/data/TreeItem.h>
#include <navitconf/data/TreeModel.h>

TreeView::TreeView()
:
	QTreeView()
{
	// set rows white and grey (and not only white)
	setAlternatingRowColors(true);
	// set every row to the same height for improving performance
	setUniformRowHeights(true);
	// only one cell can be selected
	setSelectionMode(SingleSelection);
	// one select for all cells in the row
	setSelectionBehavior(SelectRows);
	// allow a custom context menu in the tree view
	setContextMenuPolicy(Qt::CustomContextMenu);
	// enable drag and drop for move
//	setDragDropMode(QAbstractItemView::InternalMove);
	// connect tree view for context menu requests
	connect(this, SIGNAL(customContextMenuRequested(const QPoint&)),
			this,   SLOT(showContextMenu(const QPoint&)));
}

TreeView::~TreeView() {
}

void TreeView::setupUi(QWidget& parent, QGridLayout& gridLayout) {
	setParent(&parent);
	gridLayout.addWidget(this);
}

void TreeView::setModel(QAbstractItemModel* model) {
	QTreeView::setModel(model);
	expandToDepth(1);
	header()->setResizeMode(0, QHeaderView::ResizeToContents);
//	setColumnWidth(0, 170);
}

void TreeView::dataChanged(const QModelIndex& indexTopLeft, const QModelIndex& /* indexRightBottom */) {
	// deselect row(s)
	QModelIndexList list = selectionModel()->selectedIndexes();
	for (int i = 0; i < list.size(); i++) {
		selectionModel()->select(list[i], QItemSelectionModel::Deselect | QItemSelectionModel::Clear);
	}
	// select changed row
	selectionModel()->select(indexTopLeft, QItemSelectionModel::SelectCurrent | QItemSelectionModel::Rows);
	// ensure selected row is visible
	scrollTo(indexTopLeft);
}

void TreeView::showContextMenu(const QPoint& pos) {
	QModelIndex index = indexAt(pos);
	if (index.isValid()) {
		TreeModel& treeModel = *static_cast<TreeModel*>(model());
//		if (treeModel.getTreeItem(index).getDomNode().isElement()) {
			// create actions for context menu
			QList<QAction*> actions;
			actions.append(createAction(tr("&Insert Node"), ADD));
			actions.append(createAction(tr("&Rename Node"), RENAME_NODE, "F2"));
			actions.append(createAction(tr("&Delete Node"), DELETE));
			actions.append(createSeparator());
			actions.append(createAction(tr("&Expand All Children"), EXPAND_ALL_CHILDREN, "*"));
			actions.append(createAction(tr("&Collaps All Children"), COLLAPS_ALL_CHILDREN));
			actions.append(createAction(tr("Debug &Tree"), DEBUG_TREE));
			// show context menu
			QPoint topLeftPoint = mapToGlobal(pos) + QPoint(0, header()->height());
			QAction* selectedAction = QMenu::exec(actions, topLeftPoint);
			if (selectedAction != NULL) { // not clicked outside of context menu
				if (selectedAction->data() == ADD) {
					treeModel.insertRows(index.row(), 1, index.parent());
				} else if (selectedAction->data() == RENAME_NODE) {
					edit(index);
				} else if (selectedAction->data() == DELETE) {
					treeModel.removeRows(index.row(), 1, index.parent());
				} else if (selectedAction->data() == EXPAND_ALL_CHILDREN) {
					expandAllChildren(index);
				} else if (selectedAction->data() == COLLAPS_ALL_CHILDREN) {
					collapseAllChildren(index);
				} else if (selectedAction->data() == DEBUG_TREE) {
					show(index.parent());
				}
			}
			delete selectedAction;
//		}
	}
}

QAction* TreeView::createAction(const QString& name, MyContextAction id, const QString& shortcut) {
	QAction& action = *new QAction(name, this);
	action.setData(id);
	if (!shortcut.isEmpty()) {
		action.setShortcut(QKeySequence(shortcut));
	}
	return &action;
}

QAction* TreeView::createSeparator() {
	QAction& action = *new QAction("", this);
	action.setSeparator(true);
	return &action;
}

void TreeView::expandAllChildren(const QModelIndex& parentIndex) {
	int rows = model()->rowCount(parentIndex);
	for (int i = rows - 1; i >= 0; i--) {
		expandAllChildren(parentIndex.child(i, 0));
	}
	expand(parentIndex);
}

void TreeView::collapseAllChildren(const QModelIndex& parentIndex) {
	int rows = model()->rowCount(parentIndex);
	for (int i = rows - 1; i >= 0; i--) {
		collapseAllChildren(parentIndex.child(i, 0));
	}
	collapse(parentIndex);
}

void TreeView::show(const QModelIndex& index) {
	TreeModel& treeModel = *static_cast<TreeModel*>(model());
//	treeModel.println(index);
	for (int i = 0; i < treeModel.getTreeItem(index).countChilds(); i++) {
		show(index.child(i, 0));
	}
}
