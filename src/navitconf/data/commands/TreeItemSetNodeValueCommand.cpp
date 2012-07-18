/*
 * TreeEditCommand.cpp
 *
 *  Created on: 14.05.2012
 *      Author: remo
 */

#include "TreeItemSetNodeValueCommand.h"

/** forward declaration */
#include <navitconf/data/TreeModel.h>

TreeItemSetNodeValueCommand::TreeItemSetNodeValueCommand(TreeModel& model, const QModelIndex& index, const QString& newValue)
:
	QUndoCommand(),
	model(model),
	index(index),
	otherValue(newValue)
{
// redo() is called by adding an instance of this class to the undo stack
}

TreeItemSetNodeValueCommand::~TreeItemSetNodeValueCommand() {
}

void TreeItemSetNodeValueCommand::redo() {
	QDomNode& node = model.getTreeItem(index).getDomNode();
	//qDebug("TreeItemSetNodeValueCommand::redo() set attribute");
	QString tmpValue(node.nodeValue());
	node.setNodeValue(otherValue);
	otherValue = tmpValue;
	model.emitDataChanged(index, index.sibling(index.row(), 1));
}

void TreeItemSetNodeValueCommand::undo() {
	redo();
}
