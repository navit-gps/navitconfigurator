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

#include "TreeItemInsertCommand.h"

#include <cstddef>

#include "../TreeModel.h"

TreeItemInsertCommand::TreeItemInsertCommand(TreeModel& model, const QModelIndex& parentIndex, int row, TreeItem* newTreeItem)
:
	AbstractTreeNodeCommand(model, parentIndex, row),
	newTreeItem(newTreeItem)
{
//	model.println(this->parentIndex);
}

TreeItemInsertCommand::~TreeItemInsertCommand() {
	if (newTreeItem != NULL) {
		delete newTreeItem;
	}
}

void TreeItemInsertCommand::redo() {
	insertNode(*newTreeItem);
	newTreeItem = NULL;
}

void TreeItemInsertCommand::undo() {
	newTreeItem = &removeNode();
}
