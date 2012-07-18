/*
 * TreeEditCommand.cpp
 *
 *  Created on: 14.05.2012
 *      Author: remo
 */

#include "TreeItemEditCommand.h"

#include <qdebug.h>

/** forward declaration */
#include <navitconf/data/TreeModel.h>

TreeItemRenameCommand::TreeItemRenameCommand(TreeModel& model, const QModelIndex& index, const QString& newTagName)
:
	QUndoCommand(),
	model(model),
	index(index),
	otherTagName(newTagName)
{
// redo() is called by adding an instance of this class to the undo stack
}

TreeItemRenameCommand::~TreeItemRenameCommand() {
}

void TreeItemRenameCommand::redo() {
	QDomNode& node = model.getTreeItem(index).getDomNode();
	QString tmpName = node.nodeName();
	node.toElement().setTagName(otherTagName);
	otherTagName = tmpName;
	model.emitDataChanged(index, index);
}

void TreeItemRenameCommand::undo() {
	redo();
}
