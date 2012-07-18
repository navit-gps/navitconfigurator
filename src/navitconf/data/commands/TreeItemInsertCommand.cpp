/*
 * TreeRemoveTagCommand.cpp
 *
 *  Created on: 20.05.2012
 *      Author: remo
 */

#include "TreeItemInsertCommand.h"

#include <cstddef>

#include "../TreeModel.h"

TreeItemInsertCommand::TreeItemInsertCommand(TreeModel& model, const QModelIndex& parentIndex, int row, TreeItem* newTreeItem)
:
	AbstractTreeNodeCommand(model, parentIndex, row),
	newTreeItem(newTreeItem)
{
//	qDebug("TreeInsertNodeCommand(row=%d) with parent:", row);
//	model.println(this->parentIndex);
}

TreeItemInsertCommand::~TreeItemInsertCommand() {
	if (newTreeItem != NULL) {
		delete newTreeItem;
	}
}

void TreeItemInsertCommand::redo() {
//	qDebug("TreeInsertNodeCommand.redo()");
	insertNode(*newTreeItem);
	newTreeItem = NULL;
}

void TreeItemInsertCommand::undo() {
//	qDebug("TreeInsertNodeCommand.undo()");
	newTreeItem = &removeNode();
}
