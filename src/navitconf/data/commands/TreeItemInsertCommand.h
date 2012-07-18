/*
 * TreeRemoveTagCommand.h
 *
 *  Created on: 20.05.2012
 *      Author: remo
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
