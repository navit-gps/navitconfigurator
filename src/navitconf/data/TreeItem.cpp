/*
 * TreeItem.cpp
 *
 *  Created on: 09.05.2012
 *      Author: Raimar
 */

#include "TreeItem.h"

#include <cstddef> // NULL

#include <QtCore/QDebug>
#include <QtCore/QObject>

TreeItem::TreeItem(const QDomNode& domNode, int row)
:
	domNode(domNode),
	row(row),
	parent(NULL),
	childs()
{
	init();
}

TreeItem::TreeItem(const QDomNode& domNode, int row, TreeItem& parent)
:
	domNode(domNode),
	row(row),
	parent(&parent),
	childs()
{
	init();
}

void TreeItem::init() {
	if (!hasOnlyTextChild()) {
		for (int i = 0; i < domNode.childNodes().size(); i++) {
			childs.append(new TreeItem(
							domNode.childNodes().item(i), i, *this));
		}
	}
}

TreeItem::~TreeItem() {
	for (int i = 0; i < childs.size(); i++) {
		if (childs[i] != NULL) {
			delete childs[i];
		}
	}
}

const QDomNode& TreeItem::getDomNode() const {
	return domNode;
}

QDomNode& TreeItem::getDomNode() {
	return domNode;
}

QString TreeItem::getAttribute(const QString& attributeName) const {
	QDomNode attributeNode = domNode.attributes().namedItem(attributeName);
	if (attributeNode.isNull()) {
		return "";
	} else {
		return attributeNode.nodeValue();
	}
}

int TreeItem::getRow() const {
	return row;
}

bool TreeItem::hasParent() const {
	return parent != NULL;
}

const TreeItem& TreeItem::getParent() const {
	return *parent;
}

TreeItem& TreeItem::getParent() {
	return *parent;
}

bool TreeItem::hasChild(int row) const {
	if (hasOnlyTextChild()) {
		return false;
	}
	return row >= 0 && row < childs.size();
}

const TreeItem& TreeItem::getChild(int childRow) const {
	return *childs[childRow];
}

TreeItem& TreeItem::getChild(int childRow) {
	return *childs[childRow];
}

int TreeItem::countChilds() const {
	return childs.size();
}

TreeItem* TreeItem::removeChild(int row) {
	TreeItem* removedTreeItem = childs[row];
	childs.remove(row);
	for (int i = row; i < childs.size() - 1; i++) {
		childs[i]->row = i;
	}
	domNode.removeChild(domNode.childNodes().item(row));
	return removedTreeItem;
}

void TreeItem::insertChild(int row, TreeItem& oldTreeItem) {
	if (domNode.childNodes().size() == row) {
		childs.append(&oldTreeItem);
		domNode.appendChild(oldTreeItem.getDomNode());
	} else if (row == 0) {
		childs.insert(row, &oldTreeItem);
		QDomNode firstNode = domNode.childNodes().item(0);
		domNode.insertBefore(oldTreeItem.getDomNode(), firstNode);
	} else {
		childs.insert(row, &oldTreeItem);
		QDomNode previousNode = domNode.childNodes().item(row - 1);
		domNode.insertAfter(oldTreeItem.getDomNode(), previousNode);
	}
	for (int i = row; i < childs.size(); i++) {
		if (childs[i] != NULL) {
			childs[i]->row = i;
		}
	}
}

int TreeItem::getCounterInSistersWithSameName() const {
	int count = 0;
	int position = -1;
	QString name = domNode.nodeName();
	int size = parent->countChilds();
	for (int i = 0; i < size; i++) {
		if (i == row) {
			position = count;
			count++;
		} else if (name == parent->getChild(i).getDomNode().nodeName()) {
			count++;
		}
	}
	if (count == 1) {
		position = -1;
	}
	return position;
}

QString TreeItem::getNameWithCounter() const {
	int counter = getCounterInSistersWithSameName();
	if (counter < 0) { // no other sister with this name
		return domNode.nodeName();
	} else {
		return domNode.nodeName().append(QString(" %1").arg(counter));
	}
}

QString TreeItem::getItemPath() const {
	if (hasParent()) {
		QString path = getNameWithCounter();
		getParent().insertItemPath(path);
		return path;
	} else {
		return getNameWithCounter();
	}
}

bool TreeItem::hasOnlyTextChild() const {
	return domNode.attributes().size() == 0 && domNode.childNodes().size() == 1 && domNode.firstChild().isText();
}

void TreeItem::insertItemPath(QString& childPath) const {
	if (hasParent()) {
		childPath.insert(0, getNameWithCounter().append('/'));
		getParent().insertItemPath(childPath);
	}
}

void TreeItem::println() const {
	qDebug() << "item: " << domNode.nodeName() <<" row=" << row;
}
