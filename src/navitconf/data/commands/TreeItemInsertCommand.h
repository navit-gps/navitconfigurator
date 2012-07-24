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

#ifndef TREEITEMINSERTCOMMAND_H_
#define TREEITEMINSERTCOMMAND_H_

#include <QtCore/QModelIndex>

#include "AbstractTreeNodeCommand.h"
#include "../TreeItem.h"

/** forward declaration */
class TreeModel;

/**
 * Provides an undo command to insert a TreeItem in a TreeModel.
 */
class TreeItemInsertCommand : public AbstractTreeNodeCommand {
public:
	TreeItemInsertCommand(TreeModel& model, const QModelIndex& parentIndex, int row, TreeItem* newTreeItem);
	virtual ~TreeItemInsertCommand();
    void undo();
    void redo();
private:
    TreeItem* newTreeItem;
};

#endif /* TREEITEMINSERTCOMMAND_H_ */
