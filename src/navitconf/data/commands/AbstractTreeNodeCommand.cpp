/*
 * TreeRemoveTagCommand.cpp
 *
 *  Created on: 20.05.2012
 *      Author: remo
 */

#include "AbstractTreeNodeCommand.h"

#include <cstddef>

#include "../TreeModel.h"

AbstractTreeNodeCommand::AbstractTreeNodeCommand(TreeModel& model, const QModelIndex& parentIndex, int row)
:
	QUndoCommand(),
	model(model),
	parentIndex(parentIndex),
	row(row)
{
}

AbstractTreeNodeCommand::~AbstractTreeNodeCommand() {
}

TreeItem& AbstractTreeNodeCommand::removeNode() {
//	qDebug("removeTreeItem(): row=%d with parent", row);
//	model.println(parentIndex);
	model.beginRemoveRows(parentIndex, row, row);
	TreeItem& removedTreeItem = *model.getTreeItem(parentIndex).removeChild(row);
	model.endRemoveRows();
	int rowOfLastChildAfterInsertion = model.getTreeItem(parentIndex).countChilds() - 1;
	model.emitDataChanged(parentIndex.child(row, 0), parentIndex.child(rowOfLastChildAfterInsertion, 1));
	return removedTreeItem;
}

void AbstractTreeNodeCommand::insertNode(TreeItem& newTreeItem) {
//	qDebug("insertTreeItem(): row=%d with parent", row);
//	model.println(parentIndex);
	model.beginInsertRows(parentIndex, row, row);
	model.getTreeItem(parentIndex).insertChild(row, newTreeItem);
	model.endInsertRows();
	int rowOfLastChildAfterInsertion = model.getTreeItem(parentIndex).countChilds() - 1;
	model.emitDataChanged(parentIndex.child(row, 0), parentIndex.child(rowOfLastChildAfterInsertion, 1));
}
