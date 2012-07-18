/*
 * TreeRemoveTagCommand.h
 *
 *  Created on: 20.05.2012
 *      Author: remo
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
