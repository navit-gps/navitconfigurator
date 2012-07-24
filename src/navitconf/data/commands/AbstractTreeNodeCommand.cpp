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
//	model.println(parentIndex);
	model.beginRemoveRows(parentIndex, row, row);
	TreeItem& removedTreeItem = *model.getTreeItem(parentIndex).removeChild(row);
	model.endRemoveRows();
	int rowOfLastChildAfterInsertion = model.getTreeItem(parentIndex).countChilds() - 1;
	model.emitDataChanged(parentIndex.child(row, 0), parentIndex.child(rowOfLastChildAfterInsertion, 1));
	return removedTreeItem;
}

void AbstractTreeNodeCommand::insertNode(TreeItem& newTreeItem) {
//	model.println(parentIndex);
	model.beginInsertRows(parentIndex, row, row);
	model.getTreeItem(parentIndex).insertChild(row, newTreeItem);
	model.endInsertRows();
	int rowOfLastChildAfterInsertion = model.getTreeItem(parentIndex).countChilds() - 1;
	model.emitDataChanged(parentIndex.child(row, 0), parentIndex.child(rowOfLastChildAfterInsertion, 1));
}
