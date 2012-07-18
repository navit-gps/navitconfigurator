/*
 * TreeEditCommand.cpp
 *
 *  Created on: 14.05.2012
 *      Author: remo
 */

#include "TreeItemSetAttributeCommand.h"

#include <QtCore/QDebug>

/** forward declaration */
#include <navitconf/data/TreeModel.h>

TreeItemSetAttributeCommand::TreeItemSetAttributeCommand(TreeModel& model, const QModelIndex& index, const QString& key, const QString& newValue)
:
	QUndoCommand(),
	model(model),
	index(index),
	key(key),
	otherValue(newValue)
{
// redo() is called by adding an instance of this class to the undo stack
}

TreeItemSetAttributeCommand::~TreeItemSetAttributeCommand() {
}

void TreeItemSetAttributeCommand::redo() {
	QDomNode& node = model.getTreeItem(index).getDomNode();
	QString tmpValue;
	if (otherValue.isEmpty()) { // delete attribute
		if (node.attributes().contains(key)) { // attribute exists
			tmpValue = node.attributes().namedItem(key).nodeValue();
			node.attributes().removeNamedItem(key);
		}
	} else { // set attribute
		//qDebug("TreeItemSetAttributeCommand::redo() set attribute");
		if (node.attributes().contains(key)) { // rename
			tmpValue = node.attributes().namedItem(key).nodeValue();
		} else { // add new attribute
			node.attributes().setNamedItem(model.createDomAttribute(key));
		}
		node.attributes().namedItem(key).setNodeValue(otherValue);
	}
	otherValue = tmpValue;
	model.emitDataChanged(index.sibling(index.row(), 0), index.sibling(index.row(), 1));
}

void TreeItemSetAttributeCommand::undo() {
	redo();
}
