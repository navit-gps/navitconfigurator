/*
 * TreeEditCommand.h
 *
 *  Created on: 14.05.2012
 *      Author: remo
 */

#ifndef TREEITEMSETATTRIBUTECOMMAND_H_
#define TREEITEMSETATTRIBUTECOMMAND_H_

#include <QtCore/QModelIndex>
#include <QtCore/QString>

#include <QtGui/QUndoCommand>

/** forward declaration */
class TreeModel;

/**
 * Provides an undo command to set the value of an attribute in a TreeItem of a TreeModel.
 */
class TreeItemSetAttributeCommand : public QUndoCommand {
public:
	explicit TreeItemSetAttributeCommand(TreeModel& model, const QModelIndex& index, const QString& key, const QString& newValue);
	virtual ~TreeItemSetAttributeCommand();
    void redo();
    void undo();

private:
    TreeModel& model;
    QModelIndex index;
    QString key;
    QString otherValue;
};

#endif /* TREEITEMSETATTRIBUTECOMMAND_H_ */
