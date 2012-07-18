/*
 * TreeRemoveTagCommand.h
 *
 *  Created on: 20.05.2012
 *      Author: remo
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
