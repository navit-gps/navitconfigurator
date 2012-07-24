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

#include "DialogManageNavitMenu.h"

#include <QtCore/QModelIndex>

#include <QtGui/QMessageBox>

#include <QtWebKit/QWebPage>
#include <QtWebKit/QWebFrame>

#include <navitconf/file/Settings.h>

// gui 1/#cdata-section"
const char* const DialogManageNavitMenu::MENU_DATA_PATH = "config/navit/gui[enabled=yes,type=internal]/#cdata-section";

DialogManageNavitMenu::DialogManageNavitMenu(QWidget& parent, TreeModel& treeModel)
:
	QDialog(&parent),
	ui(),
	treeModel(treeModel),
	treeItemIndex(),
	menuDocument(),
	nodeList(),
	model(),
	rootItem(),
	iconFolder(Settings::getInstance().getDir().absolutePath())
{
	ui.setupUi(this);
	// set rows white and grey (and not only white)
	ui.treeView->setAlternatingRowColors(true);
	// set every row to the same height for improving performance
	ui.treeView->setUniformRowHeights(true);
	// hide column header
	ui.treeView->setHeaderHidden(true);
	// enable drag and drop for moving items in tree
	ui.treeView->setDragDropMode(QAbstractItemView::InternalMove);
	try {
		init();
	} catch(QString& e) {
		QMessageBox::critical(this, tr("Error"), e);
	}
}

DialogManageNavitMenu::~DialogManageNavitMenu() {
}

void DialogManageNavitMenu::init() {
	treeItemIndex = treeModel.getModelIndexByDomPath(MENU_DATA_PATH);
	if (!treeItemIndex.isValid()) {
		throw tr("The path %1 has not been found in the navit configuration file.")
				.arg(MENU_DATA_PATH);
	}
	TreeItem& item = treeModel.getTreeItem(treeItemIndex);
	QString errMsg;
	int errorLine;
	int errorColumn;
	if (menuDocument.setContent(item.getDomNode().nodeValue(), &errMsg,
			&errorLine, &errorColumn)) {
		//		connect(ui.webView, SIGNAL(loadFinished(bool)),
		//				this,       SLOT  (loadFinished(bool)));
		fillDataModel();
		// insert all items in data model
		model.appendRow(&rootItem);
		ui.treeView->setModel(&model);
		// auto with of column
		ui.treeView->header()->setResizeMode(0, QHeaderView::ResizeToContents);
		// select main menu
		ui.treeView->selectionModel()->select(model.index(0, 0), QItemSelectionModel::Select);
		// open all nodes
		ui.treeView->expandAll();
		exec(); // show dialog
	} else {
		throw tr("Parsing error in line %1 column %2: %3").arg(
				item.getDomNode().lineNumber() + errorLine).arg(errorColumn).arg(
				errMsg);
	}
}

void DialogManageNavitMenu::fillDataModel() {
	QDomNode node = menuDocument.firstChild().firstChild();
	while (!node.isNull()) { // main menu items
		NavitMenuItem& parent = appendMenuToDataModel(addToNodeList(node));
		QDomNode childNode = node.firstChild().nextSibling(); // ignore text tag
		if (childNode.nodeName() != "script") { // a with child text and script is not a menu
			while (!childNode.isNull()) { // sub menu items
				parent.addChild(addToNodeList(childNode), iconFolder);
				childNode = childNode.nextSibling();
			}
		}
		node = node.nextSibling();
	}
}

QDomNode& DialogManageNavitMenu::addToNodeList(const QDomNode& node) {
	nodeList.append(node);
	return nodeList.last();
}

NavitMenuItem& DialogManageNavitMenu::appendMenuToDataModel(QDomNode& node) {
	if (rootItem.rowCount() == 0) {
		rootItem.setDomNode(node, iconFolder);
		return rootItem;
	} else {
		NavitMenuItem searchItem;
		searchItem.setDomNode(node, iconFolder);
		NavitMenuItem& parent = findMenu(rootItem, searchItem);
		if (&parent != &rootItem) {
			return parent;
		}
	}
	return rootItem.addChild(node, iconFolder);
}

void DialogManageNavitMenu::findTreeItem() {
	QModelIndex modelIndex = treeModel.getModelIndexByDomPath(MENU_DATA_PATH);
	if (modelIndex.isValid()) {
		treeItemIndex = modelIndex;
	} else {
		throw tr("The path %1 has not been found in the navit configuration file.")
				.arg(MENU_DATA_PATH);
	}
}

NavitMenuItem& DialogManageNavitMenu::findMenu(NavitMenuItem& item, const NavitMenuItem& searchItem) {
	if (item.getLinkTarget() == searchItem.getId()) {
		return item;
	}
	for (int i = 0; i < item.rowCount(); i++) {
		NavitMenuItem& parent = findMenu(item.getChild(i), searchItem);
		if (&rootItem != &parent) {
			return parent;
		}
	}
	return rootItem;
}

void DialogManageNavitMenu::accept() {
	treeModel.setNodeValue(treeItemIndex, getHtmlStringFromDataModel());
	setVisible(false); // close dialog
}

QString DialogManageNavitMenu::getHtmlStringFromDataModel() const {
	QDomDocument newDocument;
	QDomNode newRoot = newDocument.appendChild(newDocument.createElement("html"));
	appendMenuToDomDocument(rootItem, newDocument);
	return newDocument.toString();
}

void DialogManageNavitMenu::appendMenuToDomDocument(const NavitMenuItem& parentMenu, QDomDocument& newDocument) const {
	int rows = parentMenu.rowCount();
	// create direct children
	QDomNode a = newDocument.firstChild().appendChild(newDocument.createElement("a"));
	a.attributes().setNamedItem(newDocument.createAttribute("name"));
	a.attributes().namedItem("name").setNodeValue(
			parentMenu.getId().isEmpty() ? parentMenu.getLinkTarget() : parentMenu.getId());
	QDomNode text = newDocument.createElement("text");
	a.appendChild(text);
	text.appendChild(newDocument.createTextNode(parentMenu.getText()));
	for (int i = 0; i < rows; i++) {
		const NavitMenuItem& item = parentMenu.getChild(i);
		a.appendChild(item.getDomNode());
	}
	// create sub menus
	for (int i = 0; i < rows; i++) {
		const NavitMenuItem& item = parentMenu.getChild(i);
		if (item.rowCount() > 0) {
			appendMenuToDomDocument(item, newDocument);
		}
	}
}
