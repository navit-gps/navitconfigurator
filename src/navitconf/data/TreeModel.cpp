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

#include "TreeModel.h"

//#include <string>

#include <QtCore/QStringList>

#include <QtGui/QBrush>

#include "commands/TreeItemEditCommand.h"
#include "commands/TreeItemRemoveCommand.h"
#include "commands/TreeItemInsertCommand.h"
#include "commands/TreeItemSetAttributeCommand.h"
#include "commands/TreeItemSetNodeValueCommand.h"

TreeModel::TreeModel(QDomDocument& domDocument, QUndoStack& undoStack)
:
	QAbstractItemModel(),
	domDocument(domDocument),
	undoStack(undoStack),
	rootTreeItem(domDocument, 0)
{
	undoStack.clear();
}

TreeModel::~TreeModel() {
}

QUndoStack& TreeModel::getUndoStack() {
	return undoStack;
}

const TreeItem& TreeModel::getTreeItem(const QModelIndex& modelIndex) const {
	if (modelIndex.isValid()) {
		return *static_cast<const TreeItem*>(modelIndex.internalPointer());
	}
	return rootTreeItem;
}

TreeItem& TreeModel::getTreeItem(const QModelIndex& modelIndex) {
	if (modelIndex.isValid()) {
		return *static_cast<TreeItem*>(modelIndex.internalPointer());
	}
	return rootTreeItem;
}

int TreeModel::rowCount(const QModelIndex& parentIndex) const {
	if (parentIndex.isValid()) {
		if (hasTreeItem(parentIndex)) {
			const TreeItem& parentItem = getTreeItem(parentIndex);
			return parentItem.countChilds();
		}
		return 0;
	} else {
		return rootTreeItem.countChilds();
	}
}

int TreeModel::columnCount(const QModelIndex& /* modelIndex */) const {
	return 2;
}

QModelIndex TreeModel::parent(const QModelIndex& childIndex) const {
	if (childIndex.internalPointer() == NULL) {
		return QModelIndex();
	}
	const TreeItem& childItem = getTreeItem(childIndex);
	if (!childItem.hasParent()) {
		return QModelIndex();
	}
	const TreeItem& parentItem = childItem.getParent();
	if (!parentItem.hasParent()) {
		return QModelIndex();
	}
	return createIndexFromTreeItem(parentItem.getRow(), 0, parentItem);
}

QVariant TreeModel::data(const QModelIndex& index, int role) const {
	if (role == Qt::ForegroundRole) {
		if (hasTreeItem(index)) {
			if (getTreeItem(index).getDomNode().isComment()) {
				return QBrush(QColor::fromRgb(0, 0x80, 0));
			}
		}
		return QColor(Qt::black);
	} else if ((role == Qt::DisplayRole || role == Qt::EditRole) && index.isValid()) {
		const TreeItem& item = getTreeItem(index);
		const QDomNode& node = item.getDomNode();
		switch (index.column()) {
			case 0: {
				if (role == Qt::DisplayRole) {
					return item.getNameWithCounter();
				} else {
					return node.nodeName();
				}
			}
			case 1:
				if (node.isComment() || node.isCDATASection() || node.isText()) {
					return node.nodeValue().trimmed().replace('\r', ' ').replace('\n', ' ').replace('\t', ' ');
				} else if (item.hasOnlyTextChild()) {
					return node.firstChild().nodeValue().trimmed().replace('\r', ' ').replace('\n', ' ').replace('\t', ' ');
				} else {
					QStringList attributes;
					QDomNamedNodeMap attributeMap = node.attributes();
					for (int i = 0; i < attributeMap.count(); i++) {
						QDomNode attribute = attributeMap.item(i);
						attributes << attribute.nodeName() + "='"
								+ attribute.nodeValue().replace('\r', ' ').replace('\n', ' ').replace('\t', ' ') + "'";
					}
					return attributes.join(" ");
				}
			default:
				break;
		}
	}
    return QVariant(); // nothing to return
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex& parentIndex) const {
	if (parentIndex.isValid() && parentIndex.column() != 0) {
		return QModelIndex();
	}
	if (hasTreeItem(parentIndex)) {
		const TreeItem& parentTreeItem = getTreeItem(parentIndex);
		if (parentTreeItem.hasChild(row)) {
			return createIndexFromTreeItem(row, column, parentTreeItem.getChild(row));
		}
	} else {
		// void* cast necessary for const method!!! I see no other way to support the defined model interface methods.
		if (rootTreeItem.hasChild(row)) {
			void* item = (void*) &rootTreeItem.getChild(row);
			return createIndex(row, column, item);
		}
	}
	return QModelIndex();
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation, int role) const {
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole ) {
		switch (section) {
			case 0:
				return tr("Nodes");
			case 1:
				return tr("Node Attributes");
		}
	}
	return QVariant();
}

Qt::ItemFlags TreeModel::flags(const QModelIndex& index) const {
	if (!index.isValid() || !hasTreeItem(index)) {
		return 0;
	}
	const QDomNode& node = getTreeItem(index).getDomNode();
	Qt::ItemFlags flag = Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
	if (index.column() == 0 && !node.isComment() && !node.isCDATASection() && !node.isText()) {
		return flag | Qt::ItemIsEditable;
	} else {
		return flag;
	}
}

bool TreeModel::setData(const QModelIndex& index, const QVariant& name, int role) {
	if (role != Qt::EditRole) {
		return false;
	}
	QVariant oldName = index.data(Qt::DisplayRole);
	if (name == oldName) {
		return false;
	}
	QString valideName = name.toString();
	valideName.replace(" ", "");
	if (valideName.isEmpty()) {
		return false;
	}
	// add command to undo stack for renaming node name and call redo() in TreeEditNodeCommand
	undoStack.push(new TreeItemRenameCommand(*this, index, name.toString()));
	return true; // for notifying changes
}

bool TreeModel::hasTreeItem(const QModelIndex& modelIndex) const {
	return modelIndex.internalPointer() != NULL;
}

bool TreeModel::insertRows(int row, int /* count */, const QModelIndex& parentIndex) {
	try {
		insertTreeItem("NewNode", row, parentIndex);
	} catch(QString& e) {
		return false;
	}
	return true;
}

TreeItem& TreeModel::insertTreeItem(const QString& name, int row, const QModelIndex& parentIndex) {
	QString valideName = name;
	valideName.replace(" ", "");
	if (valideName.isEmpty()) {
		throw QString(tr("Inserting node with empty name is not allowed."));
	}
	TreeItem* newTreeItem = new TreeItem(createDomElement(name), row, getTreeItem(parentIndex));
	undoStack.push(new TreeItemInsertCommand(*this, parentIndex, row, newTreeItem));
	return *newTreeItem;
}

TreeItem& TreeModel::copyTreeItem(const QModelIndex& sourceIndex, int row, const QModelIndex& parentIndex) {
	TreeItem& sourceItem = getTreeItem(sourceIndex);
	TreeItem& targetItem = insertTreeItem(sourceItem.getDomNode().nodeName(), row, parentIndex);
	QModelIndex targetIndex = index(row, 0, parentIndex);
	QDomNamedNodeMap attributes = sourceItem.getDomNode().attributes();
	for (int i = 0; i < attributes.size(); i++) {
		QDomNode attribute = attributes.item(i);
		setAttribute(targetIndex, attribute.nodeName(), attribute.nodeValue());
	}
	return targetItem;
}

bool TreeModel::removeRows(int row, int /* count */, const QModelIndex& parentIndex) {
	undoStack.push(new TreeItemRemoveCommand(*this, parentIndex, row));
	return true;
}

void TreeModel::setAttribute(const QModelIndex& index, const QString& key, const QString& value) {
	undoStack.push(new TreeItemSetAttributeCommand(*this, index, key, value));
}

void TreeModel::setNodeValue(const QModelIndex& index, const QString& value) {
	undoStack.push(new TreeItemSetNodeValueCommand(*this, index, value));
}

QModelIndex TreeModel::getModelIndexByDomPath(const QString& pathName) const {
	QStringList list = pathName.split('/');
	QModelIndex current = index(0, 0); // root index
	int i = 0;
	while (i < list.size() && current.isValid()) {
		const TreeItem& item = getTreeItem(current);
		QStringList nameAttributes = list[i].split("[");
		QString name = nameAttributes[0];
		QMap<QString, QString> attributeMap;
		if (nameAttributes.size() > 1) {
			QString attributes = nameAttributes[1];
			attributes.remove(attributes.size() - 1, 1); // remove trailing ]
			attributeMap = getAttributeMap(attributes);
		}
		const QDomNode& node = item.getDomNode();
		if (node.nodeName() == name && hasAttributes(node, attributeMap)) {
			i++;
			if (i == list.size()) {
				return current;
			} else {
				current = current.child(0, 0);
			}
		} else {
			current = current.sibling(current.row() + 1, 0);
		}
	}
	return QModelIndex();
}

void TreeModel::emitDataChanged(const QModelIndex& indexTopLeft,
		const QModelIndex& indexBottomRight) {
	emit dataChanged(indexTopLeft, indexBottomRight);
}

QDomElement TreeModel::createDomElement(const QString& name) {
	return domDocument.createElement(name);
}

QDomAttr TreeModel::createDomAttribute(const QString& name) {
	return domDocument.createAttribute(name);
}

QModelIndex TreeModel::createIndexFromTreeItem(int row, int column, const TreeItem& treeItem) const {
	// void* cast necessary for const method!!! I see no other way to support the defined model interface methods.
	void* item = (void*) &treeItem;
	return createIndex(row, column, item);
}

QMap<QString, QString> TreeModel::getAttributeMap(const QString& attributes) {
	QMap<QString, QString> map;
	QStringList listAttributes = attributes.split(',');
	for (int i = 0; i < listAttributes.size(); i++) {
		QStringList nameValue = listAttributes[0].split('=');
		map[nameValue[0]] = nameValue[1];
	}
	return map;
}

bool TreeModel::hasAttributes(const QDomNode& node, const QMap<QString, QString>& attributeMap) {
	for (QMap<QString, QString>::const_iterator it = attributeMap.begin(); it != attributeMap.end(); it++) {
		if (node.attributes().contains(it.key())) {
			if (!it.value().isEmpty() && node.attributes().namedItem(it.key()).nodeValue() != it.value()) {
				return false;
			}
		} else {
			return false;
		}
	}
	return true;
}
