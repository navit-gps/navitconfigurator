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

#include "TreeItemRemoveCommand.h"

#include <cstddef>

#include "../TreeModel.h"

TreeItemRemoveCommand::TreeItemRemoveCommand(TreeModel& model, const QModelIndex& parentIndex, int row)
:
	AbstractTreeNodeCommand(model, parentIndex, row),
	removedTreeItem(NULL)
{
//	model.println(this->parentIndex);
}

TreeItemRemoveCommand::~TreeItemRemoveCommand() {
	if (removedTreeItem != NULL) {
		delete removedTreeItem;
	}
}

void TreeItemRemoveCommand::redo() {
//	model.println(parentIndex);
	removedTreeItem = &removeNode();
}

void TreeItemRemoveCommand::undo() {
	insertNode(*removedTreeItem);
	removedTreeItem = NULL;
}
