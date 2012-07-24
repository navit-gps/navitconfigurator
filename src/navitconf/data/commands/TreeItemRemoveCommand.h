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

#ifndef TREEITEMREMOVECOMMAND_H_
#define TREEITEMREMOVECOMMAND_H_

#include <QtCore/QModelIndex>

#include "AbstractTreeNodeCommand.h"
#include "../TreeItem.h"

/** forward declaration */
class TreeModel;

/**
 * Provides an undo command to delete a TreeItem in a TreeModel.
 */
class TreeItemRemoveCommand : public AbstractTreeNodeCommand {
public:
	TreeItemRemoveCommand(TreeModel& model, const QModelIndex& parentIndex, int row);
	virtual ~TreeItemRemoveCommand();
    void undo();
    void redo();
private:
    TreeItem* removedTreeItem;
};

#endif /* TREEITEMREMOVECOMMAND_H_ */
