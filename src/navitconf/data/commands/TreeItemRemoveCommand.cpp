/*
 * TreeRemoveTagCommand.cpp
 *
 *  Created on: 20.05.2012
 *      Author: remo
 */

#include "TreeItemRemoveCommand.h"

#include <cstddef>

#include "../TreeModel.h"

TreeItemRemoveCommand::TreeItemRemoveCommand(TreeModel& model, const QModelIndex& parentIndex, int row)
:
	AbstractTreeNodeCommand(model, parentIndex, row),
	removedTreeItem(NULL)
{
//	qDebug("TreeRemoveNodeCommand(row=%d) with parent:", row);
//	model.println(this->parentIndex);
}

TreeItemRemoveCommand::~TreeItemRemoveCommand() {
	if (removedTreeItem != NULL) {
		delete removedTreeItem;
	}
}

void TreeItemRemoveCommand::redo() {
//	qDebug("redo(): row=%d with parent:", row);
//	model.println(parentIndex);
	removedTreeItem = &removeNode();
}

void TreeItemRemoveCommand::undo() {
//	qDebug("undo(): row=%d", row);
	insertNode(*removedTreeItem);
	removedTreeItem = NULL;
}
