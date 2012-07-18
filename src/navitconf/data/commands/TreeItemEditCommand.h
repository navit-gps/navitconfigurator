/*
 * TreeEditCommand.h
 *
 *  Created on: 14.05.2012
 *      Author: remo
 */

#ifndef TREEITEMRENAMECOMMAND_H_
#define TREEITEMRENAMECOMMAND_H_

#include <QtCore/QModelIndex>
#include <QtCore/QVariant>
#include <QtCore/QString>

#include <QtGui/QUndoCommand>

/** forward declaration */
class TreeModel;

/**
 * Provides an undo command to rename a TreeItem of a TreeModel.
 */
class TreeItemRenameCommand : public QUndoCommand {
public:
	explicit TreeItemRenameCommand(TreeModel& model, const QModelIndex& index, const QString& newTagName);
	virtual ~TreeItemRenameCommand();
    void redo();
    void undo();

private:
    TreeModel& model;
    QModelIndex index;
    QString otherTagName;
};

#endif /* TREEITEMRENAMECOMMAND_H_ */
