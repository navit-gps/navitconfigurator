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

#ifndef ABSTRACTTREENODECOMMAND_H_
#define ABSTRACTTREENODECOMMAND_H_

#include <QtCore/QModelIndex>

#include <QtGui/QUndoCommand>

#include "../TreeItem.h"

/** forward declaration */
class TreeModel;

/**
 * Abstract class for an undo command for a TreeItem of a TreeModel.
 * @see TreeItemInsertCommand
 * @see TreeItemRemoveCommand
 */
class AbstractTreeNodeCommand : public QUndoCommand {
public:
	/**
	 * @param model The model working on.
	 * @param parentIndex The parent index for removing or inserting the child node.
	 * @param row The row of the parent for removing or inserting the child node.
	 */
	AbstractTreeNodeCommand(TreeModel& model, const QModelIndex& parentIndex, int row);
	virtual ~AbstractTreeNodeCommand();
protected:
	/**
	 * Remove the child node specified by the constructor.
	 * @return The removed child tree item.
	 */
	TreeItem& removeNode();
	/**
	 * Insert the given node at the location specified by by constructor.
	 */
	void insertNode(TreeItem& newTreeItem);
private:
    TreeModel& model;
    QModelIndex parentIndex;
    int row;
};

#endif /* ABSTRACTTREENODECOMMAND_H_ */
